
#ifndef __ELASTOS_DROID_VIEW_CSURFACESESSION_H__
#define __ELASTOS_DROID_VIEW_CSURFACESESSION_H__

#include "_Elastos_Droid_View_CSurfaceSession.h"
#include <gui/SurfaceComposerClient.h>

namespace Elastos {
namespace Droid {
namespace View {

CarClass(CSurfaceSession)
{
    friend class CSurface;

public:
    CSurfaceSession();

    ~CSurfaceSession();

    /**
     * Forcibly detach native resources associated with this object.
     * Unlike destroy(), after this call any surfaces that were created
     * from the session will no longer work.
     */
    CARAPI Kill();

private:
    // Note: This field is accessed by native code.
    android::sp<android::SurfaceComposerClient> mNativeClient;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_VIEW_CSURFACESESSION_H__
