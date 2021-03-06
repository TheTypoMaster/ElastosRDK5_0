
#ifndef __CREMOTEVIEWSFACTORYNATIVE_H__
#define __CREMOTEVIEWSFACTORYNATIVE_H__

#include "_CRemoteViewsFactoryNative.h"
#include <jni.h>
#include "ext/frameworkdef.h"

using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Widget::IRemoteViews;

namespace Elastos {
namespace Droid {
namespace JavaProxy {

CarClass(CRemoteViewsFactoryNative)
{
public:
    ~CRemoteViewsFactoryNative();

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI OnDataSetChanged();

    CARAPI OnDataSetChangedAsync();

    CARAPI OnDestroy(
        /* [in] */ IIntent* intent);

    CARAPI GetCount(
        /* [out] */ Int32* count);

    CARAPI GetViewAt(
        /* [in] */ Int32 position,
        /* [out] */ IParcelable** views);

    CARAPI GetLoadingView(
        /* [out] */ IParcelable** views);

    CARAPI GetViewTypeCount(
        /* [out] */ Int32* count);

    CARAPI GetItemId(
        /* [in] */ Int32 position,
        /* [out] */ Int64* id);

    CARAPI HasStableIds(
        /* [out] */ Boolean* res);

    CARAPI IsCreated(
        /* [out] */ Boolean* res);

    CARAPI constructor(
        /* [in] */ Handle32 jVM,
        /* [in] */ Handle32 jInstance);

private:
    static const String TAG;

    JavaVM* mJVM;
    jobject mJInstance;
};

}
}
}

#endif // __CREMOTEVIEWSFACTORYNATIVE_H__
