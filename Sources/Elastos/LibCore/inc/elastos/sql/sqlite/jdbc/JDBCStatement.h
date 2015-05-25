
#ifndef __JDBCSTATEMENT_H__
#define __JDBCSTATEMENT_H__

#include <cmdef.h>
#include <elastos.h>
#include <elastos/List.h>
#include "Elastos.CoreLibrary_server.h"
#include <elastos/Mutex.h>

using Elastos::Utility::List;
using Elastos::Core::Mutex;
using Elastos::Sql::IResultSet;
using Elastos::Sql::ISQLWarning;

namespace Elastos {
namespace Sql {
namespace SQLite {
namespace JDBC{

class JDBCStatement
{
public:
    JDBCStatement();

    JDBCStatement(
        /* [in] */ AutoPtr<IJDBCConnection> conn);

    virtual CARAPI AddBatch(
        /* [in] */ const String& sql);

    virtual CARAPI Cancel();

    virtual CARAPI ClearBatch();

    virtual CARAPI ClearWarnings();

    virtual CARAPI Close();

    virtual CARAPI_(Boolean) Execute(
        /* [in] */ const String& sql);

    virtual CARAPI_(Boolean) Execute(
        /* [in] */ const String& sql,
        /* [in] */ Int32 autoGeneratedKeys);

    virtual CARAPI_(Boolean) Execute(
        /* [in] */ const String& sql,
        /* [in] */ const ArrayOf<Int32>& columnIndexes);

    virtual CARAPI_(Boolean) Execute(
        /* [in] */ const String& sql,
        /* [in] */ const ArrayOf<String>& columnNames);

    virtual CARAPI_(AutoPtr<ArrayOf<Int32> >) ExecuteBatch();

    virtual CARAPI_(AutoPtr<IResultSet>) ExecuteQuery(
        /* [in] */ const String& sql);

    virtual CARAPI_(Int32) ExecuteUpdate(
        /* [in] */ const String& sql);

    virtual CARAPI_(Int32) ExecuteUpdate(
        /* [in] */ const String& sql,
        /* [in] */ Int32 autoGeneratedKeys);

    virtual CARAPI_(Int32) ExecuteUpdate(
        /* [in] */ const String& sql,
        /* [in] */ const ArrayOf<Int32>& columnIndexes);

    virtual CARAPI_(Int32) ExecuteUpdate(
        /* [in] */ const String& sql,
        /* [in] */ const ArrayOf<String>& columnNames);

    virtual CARAPI_(AutoPtr<IConnection>) GetConnection();

    virtual CARAPI_(Int32) GetFetchDirection();

    virtual CARAPI_(Int32) GetFetchSize();

    virtual CARAPI_(AutoPtr<IResultSet>) GetGeneratedKeys();

    virtual CARAPI_(Int32) GetMaxFieldSize();

    virtual CARAPI_(Int32) GetMaxRows();

    virtual CARAPI_(Boolean) GetMoreResults();

    virtual CARAPI_(Boolean) GetMoreResults(
        /* [in] */ Int32 current);

    virtual CARAPI_(Int32) GetQueryTimeout();

    virtual CARAPI_(AutoPtr<IResultSet>) GetResultSet();

    virtual CARAPI_(Int32) GetResultSetConcurrency();

    virtual CARAPI_(Int32) GetResultSetHoldability();

    virtual CARAPI_(Int32) GetResultSetType();

    virtual CARAPI_(Int32) GetUpdateCount();

    virtual CARAPI_(AutoPtr<ISQLWarning>) GetWarnings();

    virtual CARAPI SetCursorName(
        /* [in] */ const String& name);

    virtual CARAPI SetEscapeProcessing(
        /* [in] */ Boolean isable);

    virtual CARAPI SetFetchDirection(
        /* [in] */ Int32 direction);

    virtual CARAPI SetFetchSize(
        /* [in] */ Int32 fetchSize);

    virtual CARAPI SetMaxFieldSize(
        /* [in] */ Int32 max);

    virtual CARAPI SetMaxRows(
        /* [in] */ Int32 max);

    virtual CARAPI SetQueryTimeout(
        /* [in] */ Int32 seconds);

    virtual CARAPI_(Boolean) IsClosed();

    virtual CARAPI SetPoolable(
        /* [in] */ Boolean poolable);

    virtual CARAPI_(Boolean) IsPoolable();

    CARAPI_(AutoPtr<IResultSet>) ExecuteQuery(
        /* [in] */ const String& sql,
        /* [in] */ const ArrayOf<String>& args,
        /* [in] */ Boolean updonly);

    virtual CARAPI_(PInterface) Unwrap(
        /* [in] */ PInterface iface);

    CARAPI_(Boolean) IsWrapperFor(
        /* [in] */ AutoPtr<IInterface> iface);

    CARAPI Init(
        /* [in] */ AutoPtr<IJDBCConnection> iconn);

    virtual CARAPI_(PInterface) Probe();

public:
    AutoPtr<IJDBCConnection> conn;
    AutoPtr<IJDBCResultSet> rs;
    Int32 updcnt;
    Int32 maxrows;

private:
    List<String> batch;
    Mutex mSyncLock;
};

} // namespace JDBC
} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __JDBCSTATEMENT_H__
