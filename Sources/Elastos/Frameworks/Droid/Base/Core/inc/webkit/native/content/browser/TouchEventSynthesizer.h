// wuweizuo automatic build .h file from .java file.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_TOUCHEVENTSYNTHESIZER_H_
#define _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_TOUCHEVENTSYNTHESIZER_H_

#include "elatypes.h"
#include "elautoptr.h"
#include "ext/frameworkext.h"
#include "view/CMotionEvent.h"

// package org.chromium.content.browser;
// import android.view.MotionEvent;
// import android.view.MotionEvent.PointerCoords;
// import android.view.MotionEvent.PointerProperties;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

using Elastos::Droid::View::IMotionEvent;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
  * Provides a Java-side implementation for injecting synthetic touch events.
  */
// @JNINamespace("content")
class TouchEventSynthesizer : public Object
{
public:
    TouchEventSynthesizer(
        /* [in] */ ContentViewCore* contentViewCore);

    // @CalledByNative
    virtual CARAPI SetPointer(
        /* [in] */ Int32 index,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 id);

    // @CalledByNative
    virtual CARAPI Inject(
        /* [in] */ Int32 action,
        /* [in] */ Int32 pointerCount,
        /* [in] */ Int64 timeInMs);

private:
    static const Int32 MAX_NUM_POINTERS = 16;
    static const Int32 ACTION_START = 0;
    static const Int32 ACTION_MOVE = 1;
    static const Int32 ACTION_CANCEL = 2;
    static const Int32 ACTION_END = 3;
    /*const*/ AutoPtr<ContentViewCore> mContentViewCore;
    /*const*/ AutoPtr< ArrayOf<IPointerProperties> > mPointerProperties;
    /*const*/ AutoPtr< ArrayOf<IPointerCoords> > mPointerCoords;
    Int64 mDownTimeInMs;
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_TOUCHEVENTSYNTHESIZER_H_

