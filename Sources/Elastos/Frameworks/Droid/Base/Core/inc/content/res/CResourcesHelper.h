
#ifndef __ELASTOS_DROID_CONTENT_RES_CRESOURCESHELPER_H__
#define __ELASTOS_DROID_CONTENT_RES_CRESOURCESHELPER_H__

#include "_CResourcesHelper.h"

using Elastos::Droid::Utility::IDisplayMetrics;

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

CarClass(CResourcesHelper)
{
public:
    CARAPI SelectDefaultTheme(
        /* [in] */ Int32 curTheme,
        /* [in] */ Int32 targetSdkVersion,
        /* [out] */ Int32* value);

    CARAPI SelectSystemTheme(
        /* [in] */ Int32 curTheme,
        /* [in] */ Int32 targetSdkVersion,
        /* [in] */ Int32 orig,
        /* [in] */ Int32 holo,
        /* [in] */ Int32 deviceDefault,
        /* [out] */ Int32* value);

    CARAPI GetSystem(
        /* [out] */ IResources** res);

    CARAPI UpdateSystemConfiguration(
        /* [in] */ IConfiguration* config,
        /* [in] */ IDisplayMetrics* metrics,
        /* [in] */ ICompatibilityInfo* compat);

    CARAPI UpdateSystemConfiguration(
        /* [in] */ IConfiguration* config,
        /* [in] */ IDisplayMetrics* metrics);
};

} // namespace Res
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_CONTENT_RES_CRESOURCESHELPER_H__
