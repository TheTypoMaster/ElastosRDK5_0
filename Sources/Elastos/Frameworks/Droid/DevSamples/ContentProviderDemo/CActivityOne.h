
#ifndef __CACTIVITYONE_H__
#define __CACTIVITYONE_H__

#include "app/Activity.h"
#include "_CActivityOne.h"

using Elastos::Droid::App::Activity;

namespace Elastos {
namespace Droid {
namespace DevSamples {
namespace ContentProviderDemo {

class CActivityOne : public Activity
{
protected:
    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI OnStart();

    CARAPI OnResume();

    CARAPI OnPause();

    CARAPI OnStop();

    CARAPI OnDestroy();

private:
    CARAPI OnActivityResult(
        /* [in] */ Int32 requestCode,
        /* [in] */ Int32 resultCode,
        /* [in] */ IIntent* data);
};

} // namespace ContentProviderDemo
} // namespace DevSamples
} // namespace Droid
} // namespace Elastos

#endif // __CACTIVITYONE_H__
