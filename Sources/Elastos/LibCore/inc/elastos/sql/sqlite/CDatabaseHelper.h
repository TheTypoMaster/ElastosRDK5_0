
#ifndef __CDATABASEHELPER_H__
#define __CDATABASEHELPER_H__

#include "_Elastos_Sql_SQLite_CDatabaseHelper.h"
#include "Object.h"


namespace Elastos {
namespace Sql {
namespace SQLite {

CarClass(CDatabaseHelper)
    , public Object
    , public IDatabaseHelper
{
public:
    CAR_INTERFACE_DECL();

    CAR_SINGLETON_DECL();

    CARAPI Complete(
        /* [in] */ const String& sql,
        /* [out] */ Boolean * value);

    CARAPI Version(
        /* [out] */ String * str);

    CARAPI ErrorString(
        /* [in] */ Int32 error_code,
        /* [out] */ String * str);

    CARAPI Status(
        /* [in] */ Int32 op,
        /* [in] */ const ArrayOf<Int32>& info,
        /* [in] */ Boolean flag,
        /* [out] */ Int32 * value);

    CARAPI LongFromJulian(
        /* [in] */ Double d,
        /* [out] */ Int64 * value);

    CARAPI LongFromJulian(
        /* [in] */ const String& s ,
        /* [out] */ Int64 * value);

    CARAPI JulianFromLong(
        /* [in] */ Int64 ms,
        /* [out] */ Double * value);
};

} // namespace SQLite
} // namespace Sql
} // namespace Elastos

#endif // __CDATABASEHELPER_H__
