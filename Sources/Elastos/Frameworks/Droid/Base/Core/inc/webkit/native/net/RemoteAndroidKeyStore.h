// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_NET_REMOTEANDROIDKEYSTORE_H_
#define _ELASTOS_DROID_WEBKIT_NET_REMOTEANDROIDKEYSTORE_H_

//package org.chromium.net;

//import android.os.RemoteException;
//import android.util.Log;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

/**
 * Provides a remoted implementation of AndroidKeyStore where all calls are forwarded via
 * binder to an external process.
 */
class RemoteAndroidKeyStore : public AndroidKeyStore
{
private:
    class RemotePrivateKey : public AndroidPrivateKey
    {
    public:
        CARAPI_(Int32) GetHandle();

        //@Override
        CARAPI_(AutoPtr<AndroidKeyStore>) GetKeyStore();

    private:
        RemotePrivateKey(
            /* [in] */ Int32 handle,
            /* [in] */ RemoteAndroidKeyStore* store);

    private:
        // Reference to the key on a remote store.
        const Int32 mHandle;
        // Key store handling this key.
        const RemoteAndroidKeyStore* mStore;
    };

public:
    RemoteAndroidKeyStore(
        /* [in] */ RemoteAndroidKeyStore* manager);

    //@Override
    CARAPI_(AutoPtr< ArrayOf<Byte> >) GetRSAKeyModulus(
        /* [in] */ AndroidPrivateKey* key);

    //@Override
    CARAPI_(AutoPtr< ArrayOf<Byte> >) GetDSAKeyParamQ(
        /* [in] */ AndroidPrivateKey* key);

    //@Override
    CARAPI_(AutoPtr< ArrayOf<Byte> >) GetECKeyOrder(
        /* [in] */ AndroidPrivateKey* key);

    //@Override
    CARAPI_(AutoPtr< ArrayOf<Byte> >) RawSignDigestWithPrivateKey(
        /* [in] */ AndroidPrivateKey* key,
        /* [in] */ ArrayOf<Byte>* message);

    //@Override
    CARAPI_(Int32) GetPrivateKeyType(
        /* [in] */ AndroidPrivateKey* key);

    //@Override
    CARAPI_(AutoPtr< ArrayOf<Byte> >) GetPrivateKeyEncodedBytes(
        /* [in] */ AndroidPrivateKey* key);

    //@Override
    CARAPI_(Int64) GetOpenSSLHandleForPrivateKey(
        /* [in] */ AndroidPrivateKey* privateKey);

    virtual CARAPI_(AutoPtr<AndroidPrivateKey>) CreateKey(
        /* [in] */ String alias);

    //@Override
    CARAPI ReleaseKey(
        /* [in] */ AndroidPrivateKey* key);

private:
    static const String TAG;
    const AutoPtr<RemoteAndroidKeyStore> mRemoteManager;
};

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_NET_REMOTEANDROIDKEYSTORE_H_
