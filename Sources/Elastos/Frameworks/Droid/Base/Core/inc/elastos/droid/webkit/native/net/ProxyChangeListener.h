// wuweizuo automatic build .h file from .java file.
// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_NET_PROXYCHANGELISTENER_H_
#define _ELASTOS_DROID_WEBKIT_NET_PROXYCHANGELISTENER_H_

#include "elatypes.h"
#include "elautoptr.h"
#include "ext/frameworkext.h"
#include "content/BroadcastReceiver.h"
#include "content/Context.h"
#include "content/Intent.h"
#include "content/IntentFilter.h"
#include "net/CProxy.h"
#include "os/Build.h"

// package org.chromium.net;
// import android.content.BroadcastReceiver;
// import android.content.Context;
// import android.content.Intent;
// import android.content.IntentFilter;
// import android.net.Proxy;
// import android.os.Build;
// import android.util.Log;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.base.NativeClassQualifiedName;
// import java.lang.reflect.InvocationTargetException;
// import java.lang.reflect.Method;

using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Net::IProxy;
using Elastos::Droid::Os::IBuild;
using Elastos::Droid::Util::ILog;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

/**
  * This class partners with native ProxyConfigServiceAndroid to listen for
  * proxy change notifications from Android.
  */
// @JNINamespace("net")
class ProxyChangeListener : public Object
{
public:
    class Delegate
    {
    public:
        virtual CARAPI ProxySettingsChanged() = 0;
    };

private:
    class ProxyConfig : public Object
    {
    public:
        ProxyConfig(
            /* [in] */ String host,
            /* [in] */ Int32 port);

    public:
        /*const*/ String mHost;
        /*const*/ Int32 mPort;
    };

    class ProxyReceiver
        : public Object
        , public BroadcastReceiver
    {
    public:
        // @Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        // Extract a ProxyConfig object from the supplied Intent's extra data
        // bundle. The android.net.ProxyProperties class is not exported from
        // tne Android SDK, so we have to use reflection to get at it and invoke
        // methods on it. If we fail, return an empty proxy config (meaning
        // 'direct').
        // TODO(sgurun): once android.net.ProxyInfo is public, rewrite this.
        CARAPI_(AutoPtr<ProxyConfig>) ExtractNewProxy(
            /* [in] */ IIntent* intent);
    };

public:
    static CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    virtual CARAPI SetDelegateForTesting(
        /* [in] */ Delegate* delegate);

    // @CalledByNative
    static CARAPI_(AutoPtr<ProxyChangeListener>) Create(
        /* [in] */ IContext* context);

    // @CalledByNative
    static CARAPI_(String) GetProperty(
        /* [in] */ String property);

    // @CalledByNative
    virtual CARAPI Start(
        /* [in] */ Int64 nativePtr);

    // @CalledByNative
    virtual CARAPI Stop();

private:
    ProxyChangeListener(
        /* [in] */ IContext* context);

    CARAPI ProxySettingsChanged(
        /* [in] */ ProxyConfig* cfg);

    CARAPI RegisterReceiver();

    CARAPI UnregisterReceiver();

    /**
      * See net/proxy/proxy_config_service_android.cc
      */
    // @NativeClassQualifiedName("ProxyConfigServiceAndroid::JNIDelegate")
    CARAPI NativeProxySettingsChangedTo(
        /* [in] */ Int64 nativePtr,
        /* [in] */ String host,
        /* [in] */ Int32 port);

    // @NativeClassQualifiedName("ProxyConfigServiceAndroid::JNIDelegate")
    CARAPI NativeProxySettingsChanged(
        /* [in] */ Int64 nativePtr);

private:
    static const String TAG;
    static Boolean sEnabled;
    Int64 mNativePtr;
    AutoPtr<IContext> mContext;
    AutoPtr<ProxyReceiver> mProxyReceiver;
    AutoPtr<Delegate> mDelegate;
};

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_NET_PROXYCHANGELISTENER_H_

