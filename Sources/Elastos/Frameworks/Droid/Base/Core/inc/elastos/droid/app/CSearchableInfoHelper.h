#ifndef __ELASTOS_DROID_APP_CSEARCHABLEINFOHELPER_H__
#define __ELASTOS_DROID_APP_CSEARCHABLEINFOHELPER_H__

#include "_Elastos_Droid_App_CSearchableInfoHelper.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Pm::IActivityInfo;

namespace Elastos {
namespace Droid {
namespace App {

CarClass(CSearchableInfoHelper)
{
public:

    CARAPI constructor();

    CARAPI GetActivityMetaData(
        /* [in] */ IContext* ctx,
        /* [in] */ IActivityInfo* activityInfo,
        /* [in] */ Int32 userId,
        /* [out] */ ISearchableInfo** info);
};

}// namespace App
}// namespace Droid
}// namespace Elastos

#endif
