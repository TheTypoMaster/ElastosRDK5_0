
#ifndef __CSQLWARNING_H__
#define __CSQLWARNING_H__

#include "_Elastos_Sql_CSQLWarning.h"
#include "Object.h"

using Elastos::Core::Object;

namespace Elastos {
namespace Sql {

CarClass(CSQLWarning)
    , public Object
    , public ISQLWarning
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& theReason);

    CARAPI constructor(
        /* [in] */ const String& theReason,
        /* [in] */ const String& theSQLState);

    CARAPI constructor(
        /* [in] */ const String& theReason,
        /* [in] */ const String& theSQLState,
        /* [in] */ Int32 theErrorCode);

    // CARAPI constructor(
    //     /* [in] */ IThrowable * cause);

    // CARAPI constructor(
    //     /* [in] */ const String& reason,
    //     /* [in] */ IThrowable * cause);

    // CARAPI constructor(
    //     /* [in] */ const String& reason,
    //      [in]  const String& SQLState,
    //     /* [in] */ IThrowable * cause);

    // CARAPI constructor(
    //     /* [in] */ const String& reason,
    //     /* [in] */ const String& SQLState,
    //     /* [in] */ Int32 vendorCode,
    //     /* [in] */ IThrowable * cause);

    CARAPI GetNextWarning(
        /* [out] */ ISQLWarning ** sqlwar);

    CARAPI SetNextWarning(
        /* [in] */ ISQLWarning * w);
};

} // namespace Sql
} // namespace Elastos

#endif // __CSQLWARNING_H__
