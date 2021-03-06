// wuweizuo automatic build .cpp file from .java file.

#include "ScreenOrientationProvider.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                      ScreenOrientationProvider
//=====================================================================
const String ScreenOrientationProvider::TAG("ScreenOrientationProvider");

AutoPtr<ScreenOrientationProvider> ScreenOrientationProvider::Create()
{
    // ==================before translated======================
    // return new ScreenOrientationProvider();
    assert(0);
    AutoPtr<ScreenOrientationProvider> empty;
    return empty;
}

ECode ScreenOrientationProvider::LockOrientation(
    /* [in] */ Byte orientations)
{
    // ==================before translated======================
    // Activity activity = ApplicationStatus.getLastTrackedFocusedActivity();
    // if (activity == null) {
    //     return;
    // }
    //
    // int orientation = getOrientationFromWebScreenOrientations(orientations);
    // if (orientation == ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED) {
    //     return;
    // }
    //
    // activity.setRequestedOrientation(orientation);
    assert(0);
    return NOERROR;
}

ECode ScreenOrientationProvider::UnlockOrientation()
{
    // ==================before translated======================
    // Activity activity = ApplicationStatus.getLastTrackedFocusedActivity();
    // if (activity == null) {
    //     return;
    // }
    //
    // activity.setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED);
    assert(0);
    return NOERROR;
}

ScreenOrientationProvider::ScreenOrientationProvider()
{
}

Int32 ScreenOrientationProvider::GetOrientationFromWebScreenOrientations(
    /* [in] */ Byte orientations)
{
    // ==================before translated======================
    // switch (orientations) {
    //     case ScreenOrientationValues.DEFAULT:
    //         return ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED;
    //     case ScreenOrientationValues.PORTRAIT_PRIMARY:
    //         return ActivityInfo.SCREEN_ORIENTATION_PORTRAIT;
    //     case ScreenOrientationValues.PORTRAIT_SECONDARY:
    //         return ActivityInfo.SCREEN_ORIENTATION_REVERSE_PORTRAIT;
    //     case ScreenOrientationValues.LANDSCAPE_PRIMARY:
    //         return ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE;
    //     case ScreenOrientationValues.LANDSCAPE_SECONDARY:
    //         return ActivityInfo.SCREEN_ORIENTATION_REVERSE_LANDSCAPE;
    //     case ScreenOrientationValues.PORTRAIT:
    //         return ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT;
    //     case ScreenOrientationValues.LANDSCAPE:
    //         return ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE;
    //     case ScreenOrientationValues.ANY:
    //         return ActivityInfo.SCREEN_ORIENTATION_FULL_SENSOR;
    //     default:
    //         Log.w(TAG, "Trying to lock to unsupported orientation!");
    //         return ActivityInfo.SCREEN_ORIENTATION_UNSPECIFIED;
    // }
    assert(0);
    return 0;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


