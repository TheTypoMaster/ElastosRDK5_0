
#include "CTestProgressHandler.h"

using Elastos::Sql::SQLite::EIID_IProgressHandler;
using Elastos::Sql::SQLite::EIID_ICallback;

namespace Elastos {
namespace Sql {
namespace SQLite {

CAR_OBJECT_IMPL(CTestProgressHandler);

CAR_INTERFACE_IMPL_2(CTestProgressHandler, Object, IDBDump, ICallback);

ECode CTestProgressHandler::Progress(
    /* [out] */ Boolean* value)
{
    progressed = TRUE;
    counter++;
    *value = TRUE;
    return NOERROR;
}

ECode CTestProgressHandler::Columns(
    /* [in] */ ArrayOf<String> * coldata)
{
    return NOERROR;
}

ECode CTestProgressHandler::Types(
    /* [in] */ ArrayOf<String> * types)
{
    return NOERROR;
}

ECode CTestProgressHandler::Newrow(
    /* [in] */ ArrayOf<String> * rowdata,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    *value = FALSE;
    return NOERROR;
}

ECode CTestProgressHandler::constructor()
{
    progressed = FALSE;
    counter = 0;
    return NOERROR;
}

Int32 CTestProgressHandler::GetCounts()
{
    return counter;
}

} // namespace SQLite
} // namespace Sql
} // namespace Elastos