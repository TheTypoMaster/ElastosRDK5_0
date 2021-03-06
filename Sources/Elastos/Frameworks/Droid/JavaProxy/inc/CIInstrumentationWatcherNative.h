
#ifndef __CIINSTRUMENTATIONWATCHERNATIVE_H__
#define __CIINSTRUMENTATIONWATCHERNATIVE_H__

#include "_CIInstrumentationWatcherNative.h"
#include <jni.h>

using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace JavaProxy {

CarClass(CIInstrumentationWatcherNative)
{
public:
    ~CIInstrumentationWatcherNative();

    CARAPI constructor(
        /* [in] */ Handle32 jVM,
        /* [in] */ Handle32 jInstance);

    CARAPI InstrumentationStatus(
        /* [in] */ IComponentName* name,
        /* [in] */ Int32 resultCode,
        /* [in] */ IBundle* results);

    CARAPI InstrumentationFinished(
        /* [in] */ IComponentName* name,
        /* [in] */ Int32 resultCode,
        /* [in] */ IBundle* results);

    CARAPI ToString(
        /* [out] */ String* str);

private:
    static const String TAG;

    JavaVM* mJVM;
    jobject mJInstance;
};

}
}
}

#endif // __CIINSTRUMENTATIONWATCHERNATIVE_H__
