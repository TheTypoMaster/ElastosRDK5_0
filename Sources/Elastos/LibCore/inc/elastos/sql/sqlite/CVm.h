
#ifndef __ELASTOS_SQL_SQLITE_CVM_H__
#define __ELASTOS_SQL_SQLITE_CVM_H__

#include "_Elastos_Sql_SQLite_CVm.h"
#include "Object.h"

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CVm)
    , public Object
    , public IVm
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CARAPI Step(
        /* [in] */ ICallback * cb,
        /* [out] */ Boolean * value);

    CARAPI Compile(
        /* [out] */ Boolean * value);

    CARAPI Stop();

    CARAPI Finalize();

    CVm();

    ~CVm();

public:
    Int32 mError_code;
    Int64 mHandle;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __ELASTOS_SQL_SQLITE_CVM_H__
