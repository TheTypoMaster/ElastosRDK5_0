
#ifndef __ELASTOS_DROID_BLUETOOTH_CBLUETOOTHADAPTER_H__
#define __ELASTOS_DROID_BLUETOOTH_CBLUETOOTHADAPTER_H__

#include "_Elastos_Droid_Bluetooth_CBluetoothAdapter.h"
#include "ext/frameworkdef.h"
#include "CBluetoothAdapterManagerCallback.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Utility::IUUID;
using Elastos::Droid::Os::IHandler;

namespace Elastos {
namespace Droid {
namespace Bluetooth {

/**
 * Represents the local device Bluetooth adapter. The {@link BluetoothAdapter}
 * lets you perform fundamental Bluetooth tasks, such as initiate
 * device discovery, query a list of bonded (paired) devices,
 * instantiate a {@link BluetoothDevice} using a known MAC address, and create
 * a {@link BluetoothServerSocket} to listen for connection requests from other
 * devices.
 *
 * <p>To get a {@link BluetoothAdapter} representing the local Bluetooth
 * adapter, call the static {@link #getDefaultAdapter} method.
 * Fundamentally, this is your starting point for all
 * Bluetooth actions. Once you have the local adapter, you can get a set of
 * {@link BluetoothDevice} objects representing all paired devices with
 * {@link #getBondedDevices()}; start device discovery with
 * {@link #startDiscovery()}; or create a {@link BluetoothServerSocket} to
 * listen for incoming connection requests with
 * {@link #listenUsingRfcommWithServiceRecord(String,UUID)}.
 *
 * <p class="note"><strong>Note:</strong>
 * Most methods require the {@link android.Manifest.permission#BLUETOOTH}
 * permission and some also require the
 * {@link android.Manifest.permission#BLUETOOTH_ADMIN} permission.
 *
 * <div class="special reference">
 * <h3>Developer Guides</h3>
 * <p>For more information about using Bluetooth, read the
 * <a href="{@docRoot}guide/topics/wireless/bluetooth.html">Bluetooth</a> developer guide.</p>
 * </div>
 *
 * {@see BluetoothDevice}
 * {@see BluetoothServerSocket}
 */
CarClass(CBluetoothAdapter)
{
public:
    interface IBluetoothStateChangeCallback : public IInterface
    {
    public:
        virtual CARAPI_(void) OnBluetoothStateChange(
            /* [in] */ Boolean on) = 0;
    };

public:
    CBluetoothAdapter();

    ~CBluetoothAdapter();

    /**
    * Helper to check if this device has FEATURE_BLUETOOTH, but without using
    * a context.
    * Equivalent to
    * context.getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH)
    */
    static CARAPI_(Boolean) HasBluetoothFeature();

    static CARAPI_(AutoPtr<IBluetoothAdapter>) GetDefaultAdapter();

    CARAPI GetRemoteDevice(
        /* [in] */ const String& address,
        /* [out] */ IBluetoothDevice** device);

    CARAPI GetRemoteDevice(
        /* [in] */ ArrayOf<Byte>* address,
        /* [out] */ IBluetoothDevice** device);

    CARAPI IsEnabled(
        /* [out] */ Boolean* isEnable);

    CARAPI GetState(
        /* [out] */ Int32* state);

    CARAPI Enable(
        /* [out] */ Boolean* result);

    CARAPI Disable(
        /* [out] */ Boolean* result);

    CARAPI Disable(
        /* [in] */ Boolean persist,
        /* [out] */ Boolean* result);

    CARAPI GetAddress(
        /* [out] */ String* address);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetUuids(
        /* [out, callee] */ ArrayOf<Elastos::Droid::Os::IParcelUuid *>** uuids);

    CARAPI SetName(
        /* [in] */ const String& name,
        /* [out] */ Boolean* result);

    CARAPI GetScanMode(
        /* [out] */ Int32* mode);

    CARAPI SetScanMode(
        /* [in] */ Int32 mode,
        /* [in] */ Int32 duration,
        /* [out] */ Boolean* result);

    CARAPI SetScanMode(
        /* [in] */ Int32 mode,
        /* [out] */ Boolean* result);

    CARAPI GetDiscoverableTimeout(
        /* [out] */ Int32* timeout);

    CARAPI SetDiscoverableTimeout(
        /* [in] */ Int32 timeout);

    CARAPI StartDiscovery(
        /* [out] */ Boolean* result);

    CARAPI CancelDiscovery(
        /* [out] */ Boolean* result);

    CARAPI IsDiscovering(
        /* [out] */ Boolean* isDiscovering);

    CARAPI GetBondedDevices(
        /* [out, callee] */ ArrayOf<IBluetoothDevice*>** device);

    CARAPI GetConnectionState(
        /* [out] */ Int32* state);

    CARAPI GetProfileConnectionState(
        /* [in] */ Int32 profile,
        /* [out] */ Int32* state);

    CARAPI ListenUsingRfcommOn(
        /* [in] */ Int32 channel,
        /* [out] */ IBluetoothServerSocket** socket);

    CARAPI ListenUsingRfcommWithServiceRecord(
        /* [in] */ const String& name,
        /* [in] */ Elastos::Utility::IUUID* uuid,
        /* [out] */ IBluetoothServerSocket** socket);

    CARAPI ListenUsingInsecureRfcommWithServiceRecord(
        /* [in] */ const String& name,
        /* [in] */ Elastos::Utility::IUUID* uuid,
        /* [out] */ IBluetoothServerSocket** socket);

    CARAPI ListenUsingEncryptedRfcommWithServiceRecord(
        /* [in] */ const String& name,
        /* [in] */ Elastos::Utility::IUUID* uuid,
        /* [out] */ IBluetoothServerSocket** socket);

    CARAPI ListenUsingInsecureRfcommOn(
        /* [in] */ Int32 port,
        /* [out] */ IBluetoothServerSocket** socket);

    CARAPI ListenUsingEncryptedRfcommOn(
        /* [in] */ Int32 port,
        /* [out] */ IBluetoothServerSocket** socket);

    static CARAPI ListenUsingScoOn(
        /** [out] */ IBluetoothServerSocket** socket);

    CARAPI ReadOutOfBandData(
        /* [out] */ IInterface** data);

    CARAPI GetProfileProxy(
        /* [in] */ Elastos::Droid::Content::IContext* context,
        /* [in] */ IBluetoothProfileServiceListener* listener,
        /* [in] */ Int32 profile,
        /* [out] */ Boolean* result);

    CARAPI CloseProfileProxy(
        /* [in] */ Int32 profile,
        /* [in] */ IBluetoothProfile* proxy);

    CARAPI EnableNoAutoConnect(
        /* [out] */ Boolean* result);

    CARAPI ChangeApplicationBluetoothState(
        /* [in] */ Boolean on,
        /* [in] */ IIBluetoothStateChangeCallback* stateChangeCallback,
        /* [out] */ Boolean* result);

    static CARAPI_(Boolean) CheckBluetoothAddress(
        /* [in] */ const String& address);

    CARAPI_(AutoPtr<IIBluetoothManager>) GetBluetoothManager();

    CARAPI_(AutoPtr<IIBluetooth>) GetBluetoothService(
        /* [in] */ IIBluetoothManagerCallback* cb);

    CARAPI_(void) RemoveServiceStateCallback(
        /* [in] */ IIBluetoothManagerCallback* cb);

    CARAPI constructor(
        /* [in] */ IIBluetoothManager* managerService);

private:
    CARAPI CreateNewRfcommSocketAndRecord(
        /* [in] */ const String& name,
        /* [in] */ IUUID* uuid,
        /* [in] */ Boolean auth,
        /* [in] */ Boolean encrypt,
        /* [out] */ IBluetoothServerSocket** socket);

private:
    const static String TAG;
    const static Boolean DBG;
    const static Boolean VDBG;
    const static Int32 ADDRESS_LENGTH = 17;

    /**
     * Lazily initialized singleton. Guaranteed final after first object
     * constructed.
     */
    static AutoPtr<IBluetoothAdapter> sAdapter;

    AutoPtr<IIBluetoothManager> mManagerService;
    AutoPtr<IIBluetooth> mService;

    AutoPtr<IHandler> mServiceRecordHandler;
    AutoPtr<IIBluetoothManagerCallback> mManagerCallback;
    Object mManagerCallbackLock;
    List<AutoPtr<IIBluetoothManagerCallback> > mProxyServiceStateCallbacks;
    static Object sLock;

    friend class CBluetoothAdapterManagerCallback;
};

}
}
}

#endif // __ELASTOS_DROID_BLUETOOTH_CBLUETOOTHADAPTER_H__
