// wuweizuo automatic build .h file from .java file.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_TRACINGCONTROLLERANDROID_H_
#define _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_TRACINGCONTROLLERANDROID_H_

#include "ext/frameworkext.h"

// package org.chromium.content.browser;
// import android.content.BroadcastReceiver;
// import android.content.Context;
// import android.content.Intent;
// import android.content.IntentFilter;
// import android.os.Environment;
// import android.text.TextUtils;
// import android.util.Log;
// import android.widget.Toast;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.content.R;
// import java.io.File;
// import java.text.SimpleDateFormat;
// import java.util.Date;
// import java.util.Locale;
// import java.util.TimeZone;

using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Os::IEnvironment;
using Elastos::Droid::Text::ITextUtils;
using Elastos::Droid::Util::ILog;
using Elastos::Droid::Widget::IToast;
using Elastos::Io::IFile;
using Elastos::Text::ISimpleDateFormat;
using Elastos::Utility::IDate;
using Elastos::Utility::ILocale;
using Elastos::Utility::ITimeZone;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
  * Controller for Chrome's tracing feature.
  *
  * We don't have any UI per se. Just call startTracing() to start and
  * stopTracing() to stop. We'll report progress to the user with Toasts.
  *
  * If the host application registers this class's BroadcastReceiver, you can
  * also start and stop the tracer with a broadcast intent, as follows:
  * <ul>
  * <li>To start tracing: am broadcast -a org.chromium.content_shell_apk.GPU_PROFILER_START
  * <li>Add "-e file /foo/bar/xyzzy" to log trace data to a specific file.
  * <li>To stop tracing: am broadcast -a org.chromium.content_shell_apk.GPU_PROFILER_STOP
  * </ul>
  * Note that the name of these intents change depending on which application
  * is being traced, but the general form is [app package name].GPU_PROFILER_{START,STOP}.
  */
// @JNINamespace("content")
class TracingControllerAndroid : public Object
{
public:
    class TracingBroadcastReceiver
        : public Object
        , public BroadcastReceiver
    {
    public:
        // @Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);
    };

private:
    class TracingIntentFilter
        : public Object
        , public IntentFilter
    {
    public:
        TracingIntentFilter(
            /* [in] */ IContext* context);
    };

public:
    TracingControllerAndroid(
        /* [in] */ IContext* context);

    /**
      * Get a BroadcastReceiver that can handle profiler intents.
      */
    virtual CARAPI_(AutoPtr<IBroadcastReceiver>) GetBroadcastReceiver();

    /**
      * Get an IntentFilter for profiler intents.
      */
    virtual CARAPI_(AutoPtr<IIntentFilter>) GetIntentFilter();

    /**
      * Register a BroadcastReceiver in the given context.
      */
    virtual CARAPI RegisterReceiver(
        /* [in] */ IContext* context);

    /**
      * Unregister the GPU BroadcastReceiver in the given context.
      * @param context
      */
    virtual CARAPI UnregisterReceiver(
        /* [in] */ IContext* context);

    /**
      * Returns true if we're currently profiling.
      */
    virtual CARAPI_(Boolean) IsTracing();

    /**
      * Returns the path of the current output file. Null if isTracing() false.
      */
    virtual CARAPI_(String) GetOutputPath();

    /**
      * Start profiling to a new file in the Downloads directory.
      *
      * Calls #startTracing(String, boolean, String, boolean) with a new timestamped filename.
      * @see #startTracing(String, boolean, String, boolean)
      */
    virtual CARAPI_(Boolean) StartTracing(
        /* [in] */ Boolean showToasts,
        /* [in] */ String categories,
        /* [in] */ Boolean recordContinuously);

    /**
      * Start profiling to the specified file. Returns true on success.
      *
      * Only one TracingControllerAndroid can be running at the same time. If another profiler
      * is running when this method is called, it will be cancelled. If this
      * profiler is already running, this method does nothing and returns false.
      *
      * @param filename The name of the file to output the profile data to.
      * @param showToasts Whether or not we want to show toasts during this profiling session.
      * When we are timing the profile run we might not want to incur extra draw overhead of showing
      * notifications about the profiling system.
      * @param categories Which categories to trace. See TracingControllerAndroid::BeginTracing()
      * (in content/public/browser/trace_controller.h) for the format.
      * @param recordContinuously Record until the user ends the trace. The trace buffer is fixed
      * size and we use it as a ring buffer during recording.
      */
    virtual CARAPI_(Boolean) StartTracing(
        /* [in] */ String filename,
        /* [in] */ Boolean showToasts,
        /* [in] */ String categories,
        /* [in] */ Boolean recordContinuously);

    /**
      * Stop profiling. This won't take effect until Chrome has flushed its file.
      */
    virtual CARAPI StopTracing();

    /**
      * Get known category groups.
      */
    virtual CARAPI GetCategoryGroups();

protected:
    /**
      * Called by native code when the profiler's output file is closed.
      */
    // @CalledByNative
    virtual CARAPI OnTracingStopped();

    // @Override
    CARAPI Finalize();

private:
    /**
      * Generates a unique filename to be used for tracing in the Downloads directory.
      */
    // @CalledByNative
    static CARAPI_(String) GenerateTracingFilePath();

    CARAPI InitializeNativeControllerIfNeeded();

    CARAPI LogAndToastError(
        /* [in] */ String str);

    // The |str| string needs to match the ones that adb_chrome_profiler looks for.
    CARAPI LogForProfiler(
        /* [in] */ String str);

    CARAPI ShowToast(
        /* [in] */ String str);

    CARAPI_(Int64) NativeInit();

    CARAPI NativeDestroy(
        /* [in] */ Int64 nativeTracingControllerAndroid);

    CARAPI_(Boolean) NativeStartTracing(
        /* [in] */ Int64 nativeTracingControllerAndroid,
        /* [in] */ String categories,
        /* [in] */ Boolean recordContinuously);

    CARAPI NativeStopTracing(
        /* [in] */ Int64 nativeTracingControllerAndroid,
        /* [in] */ String filename);

    CARAPI_(Boolean) NativeGetKnownCategoryGroupsAsync(
        /* [in] */ Int64 nativeTracingControllerAndroid);

    CARAPI_(String) NativeGetDefaultCategories();

private:
    static const String TAG;
    static const String ACTION_START;
    static const String ACTION_STOP;
    static const String ACTION_LIST_CATEGORIES;
    static const String FILE_EXTRA;
    static const String CATEGORIES_EXTRA;
    static const String RECORD_CONTINUOUSLY_EXTRA;
    static const String DEFAULT_CHROME_CATEGORIES_PLACE_HOLDER;
    // These strings must match the ones expected by adb_profile_chrome.
    static const String PROFILER_STARTED_FMT;
    static const String PROFILER_FINISHED_FMT;
    /*const*/ AutoPtr<IContext> mContext;
    /*const*/ AutoPtr<TracingBroadcastReceiver> mBroadcastReceiver;
    /*const*/ AutoPtr<TracingIntentFilter> mIntentFilter;
    Boolean mIsTracing;
    // We might not want to always show toasts when we start the profiler, especially if
    // showing the toast impacts performance.  This gives us the chance to disable them.
    Boolean mShowToasts;
    String mFilename;
    Int64 mNativeTracingControllerAndroid;
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_TRACINGCONTROLLERANDROID_H_
