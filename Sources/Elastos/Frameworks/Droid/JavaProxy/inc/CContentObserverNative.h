
#ifndef __CCONTENTOBSERVERNATIVE_H__
#define __CCONTENTOBSERVERNATIVE_H__

#include "_CContentObserverNative.h"
#include <jni.h>

using Elastos::Droid::Database::IIContentObserver;
using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace JavaProxy {

CarClass(CContentObserverNative)
{
public:
    ~CContentObserverNative();
    
    CARAPI constructor(
        /* [in] */ Handle32 jVM,
        /* [in] */ Handle32 jInstance);

    CARAPI GetContentObserver(
        /* [out] */ IIContentObserver** observer);

    CARAPI ReleaseContentObserver(
        /* [out] */ IIContentObserver** oldObserver);

    CARAPI DeliverSelfNotifications(
        /* [out] */ Boolean* result);

    CARAPI OnChange(
        /* [in] */ Boolean selfChange);

    CARAPI OnChangeEx(
        /* [in] */ Boolean selfChange,
        /* [in] */ IUri* uri);

    CARAPI DispatchChange(
        /* [in] */ Boolean selfChange);

    CARAPI DispatchChangeEx(
        /* [in] */ Boolean selfChange,
        /* [in] */ IUri* uri);

private:
    static const String TAG;

    JavaVM* mJVM;
    jobject mJInstance;
};

}
}
}

#endif // __CCONTENTOBSERVERNATIVE_H__
