
#ifndef __ELASTOS_SQL_SQLLITE_CBACKUP_H__
#define __ELASTOS_SQL_SQLLITE_CBACKUP_H__

#include "_Elastos_Sql_SQLite_CBackup.h"
#include "Object.h"

namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CBackup)
    , public Object
    , public IBackup
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CARAPI Finish();

    CARAPI Finalize();

    CARAPI _Finalize();

    CARAPI Step(
        /* [in] */ Int32 n,
        /* [out] */ Boolean* isCompleted);

    CARAPI Backup();

    CARAPI Remaining(
        /* [out] */ Int32* number);

    CARAPI Pagecount(
        /* [out] */ Int32* number);

    CBackup();

private:
    CARAPI _Step(
        /* [in] */ Int32 n,
        /* [out] */ Boolean* state);

    CARAPI_(Int32) _Remaining();

    CARAPI_(Int32) _Pagecount();

protected:
    Int64 mHandle;
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __ELASTOS_SQL_SQLLITE_CBACKUP_H__
