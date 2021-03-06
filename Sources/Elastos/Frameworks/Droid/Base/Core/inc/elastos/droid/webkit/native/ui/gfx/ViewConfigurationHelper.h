// wuweizuo automatic build .h file from .java file.
// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_GFX_VIEWCONFIGURATIONHELPER_H_
#define _ELASTOS_DROID_WEBKIT_UI_GFX_VIEWCONFIGURATIONHELPER_H_

#include "elatypes.h"
#include "elautoptr.h"
#include "ext/frameworkext.h"
#include "content/Context.h"
#include "content/res/CConfiguration.h"
#include "content/res/CResources.h"
#include "util/CTypedValue.h"
#include "view/CViewConfiguration.h"

// package org.chromium.ui.gfx;
// import android.content.ComponentCallbacks;
// import android.content.Context;
// import android.content.res.Configuration;
// import android.content.res.Resources;
// import android.util.TypedValue;
// import android.view.ViewConfiguration;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.ui.R;

using Elastos::Droid::Content::IComponentCallbacks;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Util::ITypedValue;
using Elastos::Droid::View::IViewConfiguration;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Gfx {

/**
  * This class facilitates access to ViewConfiguration-related properties, also
  * providing native-code notifications when such properties have changed.
  *
  */
// @JNINamespace("gfx")
class ViewConfigurationHelper : public Object
{
public:
    class InnerComponentCallbacks
        : public Object
        , public ComponentCallbacks
    {
    public:
        InnerComponentCallbacks(
            /* [in] */ ViewConfigurationHelper* owner);

        // @Override
        CARAPI OnConfigurationChanged(
            /* [in] */ IConfiguration* configuration);

        // @Override
        CARAPI OnLowMemory();

    private:
        ViewConfigurationHelper* mOwner;
    };

private:
    ViewConfigurationHelper(
        /* [in] */ IContext* context);

    CARAPI RegisterListener();

    CARAPI UpdateNativeViewConfigurationIfNecessary();

    // @CalledByNative
    static CARAPI_(Int32) GetDoubleTapTimeout();

    // @CalledByNative
    static CARAPI_(Int32) GetLongPressTimeout();

    // @CalledByNative
    static CARAPI_(Int32) GetTapTimeout();

    // @CalledByNative
    static CARAPI_(Float) GetScrollFriction();

    // @CalledByNative
    CARAPI_(Int32) GetScaledMaximumFlingVelocity();

    // @CalledByNative
    CARAPI_(Int32) GetScaledMinimumFlingVelocity();

    // @CalledByNative
    CARAPI_(Int32) GetScaledTouchSlop();

    // @CalledByNative
    CARAPI_(Int32) GetScaledDoubleTapSlop();

    // @CalledByNative
    CARAPI_(Int32) GetScaledMinScalingSpan();

    // @CalledByNative
    CARAPI_(Int32) GetScaledMinScalingTouchMajor();

    // @CalledByNative
    static CARAPI_(AutoPtr<ViewConfigurationHelper>) CreateWithListener(
        /* [in] */ IContext* context);

    CARAPI NativeUpdateSharedViewConfiguration(
        /* [in] */ Int32 scaledMaximumFlingVelocity,
        /* [in] */ Int32 scaledMinimumFlingVelocity,
        /* [in] */ Int32 scaledTouchSlop,
        /* [in] */ Int32 scaledDoubleTapSlop,
        /* [in] */ Int32 scaledMinScalingSpan,
        /* [in] */ Int32 scaledMinScalingTouchMajor);

private:
    // Fallback constants when resource lookup fails, see
    // ui/android/java/res/values/dimens.xml.
    static const Float MIN_SCALING_SPAN_MM;
    static const Float MIN_SCALING_TOUCH_MAJOR_DIP;
    /*const*/ AutoPtr<IContext> mAppContext;
    AutoPtr<IViewConfiguration> mViewConfiguration;
};

} // namespace Gfx
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_GFX_VIEWCONFIGURATIONHELPER_H_

