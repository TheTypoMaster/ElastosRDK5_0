
#ifndef __ELASTOS_DROID_APP_CFRAGMENTMANAGERIMPLHELPER_H__
#define __ELASTOS_DROID_APP_CFRAGMENTMANAGERIMPLHELPER_H__

#include "_Elastos_Droid_App_CFragmentManagerImplHelper.h"

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CFragmentManagerImplHelper)
{
public:
    CARAPI ReverseTransit(
        /* [in] */ Int32 transit,
        /* [out] */ Int32* reverse);

    CARAPI TransitToStyleIndex(
        /* [in] */ Int32 transit,
        /* [in] */ Boolean enter,
        /* [out] */ Int32* index);

    CARAPI SetDEBUG(
        /* [in] */ Boolean debug);

    CARAPI GetDEBUG(
        /* [out] */ Boolean* debug);
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_CFRAGMENTMANAGERIMPLHELPER_H__
