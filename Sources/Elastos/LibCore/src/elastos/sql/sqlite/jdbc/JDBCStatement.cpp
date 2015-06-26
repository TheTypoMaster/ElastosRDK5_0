
#include "JDBCStatement.h"
#include "CJDBCConnection.h"
#include "CJDBCResultSet.h"
#include <sys/wait.h>
#include "AutoLock.h"

namespace Elastos {
namespace Sql {
namespace SQLite {
namespace JDBC{

CAR_INTERFACE_IMPL_3(JDBCStatement, Object, IJDBCStatement, IStatement, IWrapper);

JDBCStatement::JDBCStatement()
{
    updcnt = 0;
    maxrows = 0;
    rs = NULL ;
}

JDBCStatement::JDBCStatement(IJDBCConnection* iconn)
{
    constructor(iconn);
}

ECode JDBCStatement::constructor(IJDBCConnection* iconn)
{
    conn = iconn;
    updcnt = 0;
    maxrows = 0;
    rs = NULL ;
    return NOERROR;
}

ECode JDBCStatement::AddBatch(
    /* [in] */ const String& sql)
{
    batch.PushBack(sql);
    return NOERROR;
}

ECode JDBCStatement::Cancel()
{
    if (conn == NULL || ((CJDBCConnection *)&conn)->mDb == NULL) {
        // throw new SQLException("stale connection");
        return E_SQL_EXCEPTION;
    }
    ((CJDBCConnection *)&conn)->mDb->Interrupt();
    return NOERROR;
}

ECode JDBCStatement::ClearBatch()
{
    if (!batch.IsEmpty()) {
        batch.Clear();
    }
    return NOERROR;
}

ECode JDBCStatement::ClearWarnings()
{
    return NOERROR;
}

ECode JDBCStatement::Close()
{
    ClearBatch();
    conn = NULL;
    return NOERROR;
}

ECode JDBCStatement::Execute(
    /* [in] */ const String& sql,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<IResultSet> set;
    ECode ec = ExecuteQuery(sql, (IResultSet**)&set);
    *value = set != NULL;
    return ec;
}

ECode JDBCStatement::Execute(
    /* [in] */ const String& sql,
    /* [in] */ Int32 autoGeneratedKeys,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    if (autoGeneratedKeys != Elastos::Sql::IStatement::NO_GENERATED_KEYS) {
        //throw new SQLFeatureNotSupportedException("autogenerated keys not supported");
        return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
    }
    return Execute(sql, value);
}

ECode JDBCStatement::Execute(
    /* [in] */ const String& sql,
    /* [in] */ ArrayOf<Int32> * columnIndexes,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = FALSE;
    // throw new SQLFeatureNotSupportedException();
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCStatement::Execute(
    /* [in] */ const String& sql,
    /* [in] */ ArrayOf<String> * columnNames,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = FALSE;
    // throw new SQLFeatureNotSupportedException();
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCStatement::ExecuteBatch(
    /* [out, callee] */ ArrayOf<Int32> ** value)
{
    VALIDATE_NOT_NULL(*value);
    AutoPtr<ArrayOf<Int32> > ret;
    if (batch.IsEmpty()) {
        *value = ArrayOf<Int32>::Alloc(0);
        REFCOUNT_ADD(*value);
        return NOERROR;
    }
    ret = ArrayOf<Int32>::Alloc(batch.GetSize());
    for (Int32 i = 0; i < ret->GetLength(); i++) {
        (*ret)[i] = Elastos::Sql::IStatement::EXECUTE_FAILED;
    }
    Int32 errs = 0;
    for (Int32 i = 0; i < ret->GetLength(); i++) {
        // try {
        Boolean Exflag = FALSE;
        Execute(batch[i], &Exflag);
        (*ret)[i] = updcnt;
        // } catch (SQLException e) {
        if (!Exflag)
        {
            ++errs;
        }
        // }
    }
    if (errs > 0) {
        // throw new BatchUpdateException("batch failed", ret);
        *value = ArrayOf<Int32>::Alloc(0);
        REFCOUNT_ADD(*value);
        return NOERROR;
    }

    *value = ret;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode JDBCStatement::ExecuteQuery(
    /* [in] */ const String& sql,
    /* [out] */ IResultSet** resultset)
{
    return ExecuteQuery(sql, NULL, FALSE, resultset);
}

ECode JDBCStatement::ExecuteQuery(
    /* [in] */ const String& sql,
    /* [in] */ ArrayOf<String> * args,
    /* [in] */ Boolean updonly,
    /* [out] */ IResultSet** resultset)
{
    VALIDATE_NOT_NULL(resultset);
    *resultset = NULL;

    AutoPtr<SQLite::ITableResult> tr;
    if (rs != NULL) {
        ((CJDBCResultSet *)&rs)->Close();
        rs = NULL;
    }
    updcnt = -1;
    if (conn == NULL || ((CJDBCConnection *)&conn)->mDb == NULL) {
        //throw new SQLException("stale connection");
        return E_SQL_EXCEPTION;
    }
    Int32 busy = 0;
    Boolean starttrans = !((CJDBCConnection *)&conn)->mAutocommit && !((CJDBCConnection *)&conn)->mIntrans;
    while (TRUE) {
        // try {
            ECode ecreturn = NOERROR;
            Boolean flagreturn = FALSE;
            if (starttrans) {
                ecreturn = ((CJDBCConnection *)&conn)->mDb->Exec(String("BEGIN TRANSACTION"), NULL);
                if (ecreturn != NOERROR)
                {
                    flagreturn = TRUE;
                }
                ((CJDBCConnection *)&conn)->mIntrans = TRUE;
            }
            if (args && args->GetLength() > 0) {
                if (updonly) {
                    ecreturn = ((CJDBCConnection *)&conn)->mDb->Exec(sql, NULL);
                    if (ecreturn != NOERROR)
                    {
                        flagreturn = TRUE;
                    }
                } else {
                    ecreturn = ((CJDBCConnection *)&conn)->mDb->GetTable(sql, maxrows,(ITableResult **)&tr);
                    if (ecreturn != NOERROR)
                    {
                        flagreturn = TRUE;
                    }
                }
            }
            else {
                if (updonly) {
                    ecreturn = ((CJDBCConnection *)&conn)->mDb->Exec(sql, NULL, args);
                    if (ecreturn != NOERROR)
                    {
                        flagreturn = TRUE;
                    }
                } else {
                    ecreturn = ((CJDBCConnection *)&conn)->mDb->GetTable(sql, maxrows, args, (ITableResult **)&tr);
                    if (ecreturn != NOERROR)
                    {
                        flagreturn = TRUE;
                    }
                }
            }
            ((CJDBCConnection *)&conn)->mDb->Changes((Int64 *)&updcnt);
        // }
        // catch (SQLite.Exception e) {
            if(flagreturn){
                Int32 lasterr = 0;
                ((CJDBCConnection *)&conn)->mDb->LastError(&lasterr);
                Boolean is3flag = FALSE;
                ((CJDBCConnection *)&conn)->mDb->Is3(&is3flag);
                if (is3flag && lasterr == SQLite::IConstants::SQLITE_BUSY &&
                    ((CJDBCConnection *)&conn)->Busy3(((CJDBCConnection *)&conn)->mDb, ++busy)) {
                        // try {
                        if (starttrans && ((CJDBCConnection *)&conn)->mIntrans) {
                            ((CJDBCConnection *)&conn)->mDb->Exec(String("ROLLBACK"), NULL);
                            ((CJDBCConnection *)&conn)->mIntrans = FALSE;
                        }
                        // } catch (SQLite.Exception ee) {
                        // }
                    // try {
                    Int32 ms = 20 + busy * 10;
                    if (ms > 1000) {
                        ms = 1000;
                    }

                    synchronized(this) {
                        Wait(ms);
                    }
                    // }
                    // catch (java.lang.Exception eee) {
                    // }
                    continue;
                }
                // throw new SQLException(e.toString());
                return E_SQL_EXCEPTION;
            // }
        }
        break;
    }
    if (!updonly && tr == NULL) {
        //throw new SQLException("no result set produced");
        return E_SQL_EXCEPTION;
    }
    if (!updonly && tr != NULL) {
        CJDBCResultSet::New(tr, (IJDBCStatement *)this->Probe(EIID_IJDBCStatement),(IJDBCResultSet **)&rs);
    }

    *resultset = IResultSet::Probe(rs);
    REFCOUNT_ADD(*resultset);
    return NOERROR;
}

ECode JDBCStatement::ExecuteUpdate(
    /* [in] */ const String& sql,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    AutoPtr<ArrayOf<String> > args = NULL ;
    AutoPtr<IResultSet> set;
    ECode ec = ExecuteQuery(sql, *args, TRUE, (IResultSet**)&set);
    *value = updcnt;
    return ec;
}

ECode JDBCStatement::ExecuteUpdate(
    /* [in] */ const String& sql,
    /* [in] */ Int32 autoGeneratedKeys,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    if (autoGeneratedKeys != Elastos::Sql::IStatement::NO_GENERATED_KEYS) {
        // throw new SQLFeatureNotSupportedException("generated keys not supported");
        *value = 0;
        return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
    }
    return ExecuteUpdate(sql, value);
}

ECode JDBCStatement::ExecuteUpdate(
    /* [in] */ const String& sql,
    /* [in] */ ArrayOf<Int32> * columnIndexes,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = 0;
    // throw new SQLFeatureNotSupportedException();
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCStatement::ExecuteUpdate(
    /* [in] */ const String& sql,
    /* [in] */ ArrayOf<String> * columnNames,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = 0;
    // throw new SQLFeatureNotSupportedException();
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCStatement::GetConnection(
    /* [out] */ IConnection ** value)
{
    VALIDATE_NOT_NULL(*value);
    *value = IConnection::Probe(conn);
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode JDBCStatement::GetFetchDirection(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = IResultSet::FETCH_UNKNOWN;
    return NOERROR;
}

ECode JDBCStatement::GetFetchSize(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = 1;
    return NOERROR;
}

ECode JDBCStatement::GetGeneratedKeys(
    /* [out] */ IResultSet** value)
{
    VALIDATE_NOT_NULL(*value);
    *value = NULL;
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCStatement::GetMaxFieldSize(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = 0;
    return NOERROR;
}

ECode JDBCStatement::GetMaxRows(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = maxrows;
    return NOERROR;
}

ECode JDBCStatement::GetMoreResults(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    if (rs != NULL) {
        ((CJDBCResultSet *)&rs)->Close();
        rs = NULL;
    }
    *value = FALSE;
    return NOERROR;
}

ECode JDBCStatement::GetMoreResults(
    /* [in] */ Int32 current,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(*value);
    *value = FALSE;
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCStatement::GetQueryTimeout(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = ((CJDBCConnection *)&conn)->timeout/1000; // android-changed: should return seconds
    return NOERROR;
}

ECode JDBCStatement::GetResultSet(
    /* [out] */ IResultSet** value)
{
    VALIDATE_NOT_NULL(*value);
    *value = IResultSet::Probe(rs);
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode JDBCStatement::GetResultSetConcurrency(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = IResultSet::CONCUR_READ_ONLY;
    return NOERROR;
}

ECode JDBCStatement::GetResultSetHoldability(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(*value);
    *value = Elastos::Sql::IResultSet::HOLD_CURSORS_OVER_COMMIT;
    return NOERROR;
}

ECode JDBCStatement::GetResultSetType(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = IResultSet::TYPE_SCROLL_INSENSITIVE;
    return NOERROR;
}

ECode JDBCStatement::GetUpdateCount(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    *value = updcnt;
    return NOERROR;
}

ECode JDBCStatement::GetWarnings(
    /* [out] */ ISQLWarning** value)
{
    VALIDATE_NOT_NULL(*value);
    *value = NULL;
    return NOERROR;
}

ECode JDBCStatement::SetCursorName(
    /* [in] */ const String& name)
{
    //throw new SQLFeatureNotSupportedException();
    return E_SQL_FEATURE_NOT_SUPPORTED_EXCEPTION;
}

ECode JDBCStatement::SetEscapeProcessing(
    /* [in] */ Boolean isable)
{
    // throw new SQLException("not supported");
    return E_SQL_EXCEPTION;
}

ECode JDBCStatement::SetFetchDirection(
    /* [in] */ Int32 direction)
{
    // throw new SQLException("not supported");
    return E_SQL_EXCEPTION;
}

ECode JDBCStatement::SetFetchSize(
    /* [in] */ Int32 fetchSize)
{
    if (fetchSize != 1) {
        //throw new SQLException("fetch size not 1");
        return E_SQL_EXCEPTION;
    }
    return NOERROR;
}

ECode JDBCStatement::SetMaxFieldSize(
    /* [in] */ Int32 max)
{
    // throw new SQLException("not supported");
    return E_SQL_EXCEPTION;
}

ECode JDBCStatement::SetMaxRows(
    /* [in] */ Int32 max)
{
    if (max < 0) {
        //throw new SQLException("max must be >= 0 (was " + max + ")");
        return E_SQL_EXCEPTION;
    }
    maxrows = max;
    return NOERROR;
}

ECode JDBCStatement::SetQueryTimeout(
    /* [in] */ Int32 seconds)
{
    // BEGIN android-changed: more closely follow specification:
    // "[throws SQLException if] this method is called on a closed Statement or the condition
    // seconds >= 0 is not satisfied"
    // (http://java.sun.com/javase/6/docs/api/java/sql/Statement.html#setQueryTimeout(int))
    Boolean closed = FALSE;
    if (IsClosed(&closed), closed) {
        //throw new SQLException("can't set a query timeout on a closed statement");'
        return E_SQL_EXCEPTION;
    } else if (seconds < 0) {
        //throw new SQLException("can't set a query timeout of less than 0 seconds");
        return E_SQL_EXCEPTION;
    } else if (seconds == 0) {
        // An argument of 0 seconds should set an unlimited timeout. However, since this was not
        // done previously, I assume it isn't implemented and use the same implementation.
        ((CJDBCConnection *)&conn)->timeout = 5000;
    } else {
        ((CJDBCConnection *)&conn)->timeout = seconds * 1000;
    }
    // END android-changed
    return NOERROR;
}

ECode JDBCStatement::IsClosed(
    /* [out] */ Boolean * value)
{
    VALIDATE_NOT_NULL(value);
    *value = conn == NULL; // android-changed: pretty sure this is correct, since it matches what's done in close()
    return NOERROR;
}

ECode JDBCStatement::SetPoolable(
    /* [in] */ Boolean poolable)
{
    if (poolable) {
        // throw new SQLException("poolable statements not supported");
        return E_SQL_EXCEPTION;
    }
    return NOERROR;
}

ECode JDBCStatement::IsPoolable(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = FALSE;
    return NOERROR;
}

ECode JDBCStatement::IsWrapperFor(
    /* [in] */ IInterface* iface,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = FALSE;
    return NOERROR;
}

ECode JDBCStatement::Unwrap(
    /* [in] */ PInterface iface,
    /* [out] */ IInterface ** oface)
{
    VALIDATE_NOT_NULL(oface);
    *oface = NULL;
    return NOERROR;
}

} // namespace JDBC
} // namespace SQLite
} // namespace Sql
} // namespace Elastos
