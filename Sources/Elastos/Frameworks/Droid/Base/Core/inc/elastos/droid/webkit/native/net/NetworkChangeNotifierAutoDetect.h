// wuweizuo automatic build .h file from .java file.
// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_NET_NETWORKCHANGENOTIFIERAUTODETECT_H_
#define _ELASTOS_DROID_WEBKIT_NET_NETWORKCHANGENOTIFIERAUTODETECT_H_

#include "elatypes.h"
#include "elautoptr.h"
#include "ext/frameworkext.h"
#include "content/BroadcastReceiver.h"
#include "content/Context.h"
#include "content/Intent.h"
#include "content/IntentFilter.h"
#include "net/CConnectivityManager.h"
#include "net/wifi/CWifiInfo.h"
#include "net/wifi/CWifiManager.h"
#include "telephony/CTelephonyManager.h"
#include "webkit/native/base/ApplicationState.h"
#include "webkit/native/base/ApplicationStatus.h"

// package org.chromium.net;
// import android.content.BroadcastReceiver;
// import android.content.Context;
// import android.content.Intent;
// import android.content.IntentFilter;
// import android.net.ConnectivityManager;
// import android.net.wifi.WifiInfo;
// import android.net.wifi.WifiManager;
// import android.telephony.TelephonyManager;
// import android.util.Log;
// import org.chromium.base.ApplicationState;
// import org.chromium.base.ApplicationStatus;

using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Net::IConnectivityManager;
using Elastos::Droid::Net::Wifi::IWifiInfo;
using Elastos::Droid::Net::Wifi::IWifiManager;
using Elastos::Droid::Telephony::ITelephonyManager;
using Elastos::Droid::Util::ILog;
using Elastos::Droid::Webkit::Base::ApplicationState;
using Elastos::Droid::Webkit::Base::ApplicationStatus;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

/**
  * Used by the NetworkChangeNotifier to listens to platform changes in connectivity.
  * Note that use of this class requires that the app have the platform
  * ACCESS_NETWORK_STATE permission.
  */
class NetworkChangeNotifierAutoDetect
    : public Object
    , public BroadcastReceiver
    , public ApplicationStatus::ApplicationStateListener
{
public:
    /** Queries the ConnectivityManager for information about the current connection. */
    class ConnectivityManagerDelegate : public Object
    {
    public:
        ConnectivityManagerDelegate(
            /* [in] */ IContext* context);

        // For testing.
        ConnectivityManagerDelegate();

        virtual CARAPI_(Boolean) ActiveNetworkExists();

        virtual CARAPI_(Boolean) IsConnected();

        virtual CARAPI_(Int32) GetNetworkType();

        virtual CARAPI_(Int32) GetNetworkSubtype();

    private:
        /*const*/ AutoPtr<IConnectivityManager> mConnectivityManager;
    };

    /** Queries the WifiManager for SSID of the current Wifi connection. */
    class WifiManagerDelegate : public Object
    {
    public:
        WifiManagerDelegate(
            /* [in] */ IContext* context);

        // For testing.
        WifiManagerDelegate();

        virtual CARAPI_(String) GetWifiSSID();

    private:
        /*const*/ AutoPtr<IWifiManager> mWifiManager;
    };

    /**
      * Observer notified on the UI thread whenever a new connection type was detected.
      */
    class Observer
    {
    public:
        virtual CARAPI OnConnectionTypeChanged(
            /* [in] */ Int32 newConnectionType) = 0;
    };

private:
    class NetworkConnectivityIntentFilter
        : public Object
        , public IntentFilter
    {
    public:
        NetworkConnectivityIntentFilter();
    };

public:
    NetworkChangeNotifierAutoDetect(
        /* [in] */ Observer* observer,
        /* [in] */ IContext* context);

    /**
      * Allows overriding the ConnectivityManagerDelegate for tests.
      */
    virtual CARAPI SetConnectivityManagerDelegateForTests(
        /* [in] */ ConnectivityManagerDelegate* delegate);

    /**
      * Allows overriding the WifiManagerDelegate for tests.
      */
    virtual CARAPI SetWifiManagerDelegateForTests(
        /* [in] */ WifiManagerDelegate* delegate);

    virtual CARAPI Destroy();

    virtual CARAPI_(Int32) GetCurrentConnectionType();

    // BroadcastReceiver
    // @Override
    CARAPI OnReceive(
        /* [in] */ IContext* context,
        /* [in] */ IIntent* intent);

    // ApplicationStatus.ApplicationStateListener
    // @Override
    CARAPI OnApplicationStateChange(
        /* [in] */ Int32 newState);

private:
    /**
      * Register a BroadcastReceiver in the given context.
      */
    CARAPI RegisterReceiver();

    /**
      * Unregister the BroadcastReceiver in the given context.
      */
    CARAPI UnregisterReceiver();

    CARAPI_(String) GetCurrentWifiSSID();

    CARAPI ConnectionTypeChanged();

private:
    static const String TAG;
    /*const*/ AutoPtr<NetworkConnectivityIntentFilter> mIntentFilter;
    /*const*/ AutoPtr<Observer> mObserver;
    /*const*/ AutoPtr<IContext> mContext;
    AutoPtr<ConnectivityManagerDelegate> mConnectivityManagerDelegate;
    AutoPtr<WifiManagerDelegate> mWifiManagerDelegate;
    Boolean mRegistered;
    Int32 mConnectionType;
    String mWifiSSID;
};

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_NET_NETWORKCHANGENOTIFIERAUTODETECT_H_

