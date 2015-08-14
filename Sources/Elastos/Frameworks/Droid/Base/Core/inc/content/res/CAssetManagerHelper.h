
#ifndef __ELASTOS_DROID_CONTENT_RES_CASSETMANAGERHELPER_H__
#define __ELASTOS_DROID_CONTENT_RES_CASSETMANAGERHELPER_H__

#include "_CAssetManagerHelper.h"

namespace Elastos {
namespace Droid {
namespace Content {
namespace Res {

CarClass(CAssetManagerHelper)
{
public:
    /**
     * Return a global shared asset manager that provides access to only
     * system assets (no application assets).
     * {@hide}
     */
    CARAPI GetSystem(
        /* [out] */ IAssetManager** am);

    CARAPI GetGlobalAssetCount(
        /* [out] */ Int32* count);

    CARAPI GetAssetAllocations(
        /* [out] */ String* allocations);

    CARAPI GetGlobalAssetManagerCount(
        /* [out] */ Int32* count);
};

} // namespace Res
} // namespace Content
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_CONTENT_RES_CASSETMANAGERHELPER_H__
