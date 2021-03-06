// wuweizuo automatic build .h file from .java file.
// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_TIMEZONEMONITOR_H_
#define _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_TIMEZONEMONITOR_H_

#include "ext/frameworkext.h"

// package org.chromium.content.browser;
// import android.content.BroadcastReceiver;
// import android.content.Context;
// import android.content.Intent;
// import android.content.IntentFilter;
// import android.util.Log;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Util::ILog;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
  * Android implementation details for content::TimeZoneMonitorAndroid.
  */
// @JNINamespace("content")
class TimeZoneMonitor : public Object
{
public:
    class InnerBroadcastReceiver
        : public Object
        , public BroadcastReceiver
    {
    public:
        InnerBroadcastReceiver(
            /* [in] */ TimeZoneMonitor* owner);

        // @Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        TimeZoneMonitor* mOwner;
    };

public:
    // @CalledByNative
    static CARAPI_(AutoPtr<TimeZoneMonitor>) GetInstance(
        /* [in] */ IContext* context,
        /* [in] */ Int64 nativePtr);

    /**
      * Stop listening for intents.
      */
    // @CalledByNative
    virtual CARAPI Stop();

private:
    /**
      * Start listening for intents.
      * @param nativePtr The native content::TimeZoneMonitorAndroid to notify of time zone changes.
      */
    TimeZoneMonitor(
        /* [in] */ IContext* context,
        /* [in] */ Int64 nativePtr);

    /**
      * Native JNI call to content::TimeZoneMonitorAndroid::TimeZoneChanged.
      * See content/browser/time_zone_monitor_android.cc.
      */
    CARAPI NativeTimeZoneChangedFromJava(
        /* [in] */ Int64 nativeTimeZoneMonitorAndroid);

private:
    static const String TAG;
    /*const*/ AutoPtr<IContext> mAppContext;
    /*const*/ AutoPtr<IIntentFilter> mFilter;
    /*const*/ AutoPtr<IBroadcastReceiver> mBroadcastReceiver;
    Int64 mNativePtr;
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_TIMEZONEMONITOR_H_
