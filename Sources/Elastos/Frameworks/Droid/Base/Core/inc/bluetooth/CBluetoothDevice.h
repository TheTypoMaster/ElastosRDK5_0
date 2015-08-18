
#ifndef __ELASTOS_DROID_BLUETOOTH_CBLUETOOTHDEVICE_H__
#define __ELASTOS_DROID_BLUETOOTH_CBLUETOOTHDEVICE_H__

#include "_Elastos_Droid_Bluetooth_CBluetoothDevice.h"
#include "CBluetoothDeviceManagerCallback.h"

using Elastos::Utility::IUUID;
using Elastos::Droid::Os::IParcelUuid;

namespace Elastos {
namespace Droid {
namespace Bluetooth {

CarClass(CBluetoothDevice)
{
public:
    static CARAPI_(AutoPtr<IIBluetooth>) GetService();

    CARAPI Equals(
        /* [in] */ IInterface *obj,
        /* [out] */ Boolean *equal);

    CARAPI ToString(
        /* [out] */ String* s);

    CARAPI DescribeContents(
        /* [out] */ Int32* value);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetAddress(
        /* [out] */ String* address);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetAlias(
        /* [out] */ String* alias);

    CARAPI SetAlias(
        /* [in] */ const String& alias,
        /* [out] */ Boolean* result);

    CARAPI GetAliasName(
        /* [out] */ String* name);

    CARAPI CreateBond(
        /* [out] */ Boolean* result);

    CARAPI CreateBondOutOfBand(
        /* [in] */ ArrayOf<Byte>* hash,
        /* [in] */ ArrayOf<Byte>* randomizer,
        /* [out] */ Boolean* result);

    CARAPI SetDeviceOutOfBandData(
        /* [in] */ ArrayOf<Byte>* hash,
        /* [in] */ ArrayOf<Byte>* randomizer,
        /* [out] */ Boolean* result);

    CARAPI CancelBondProcess(
        /* [out] */ Boolean* result);

    CARAPI RemoveBond(
        /* [out] */ Boolean* result);

    CARAPI GetBondState(
        /* [in] */ Int32* state);

    CARAPI GetBluetoothClass(
        /* [out] */ IBluetoothClass** bluetoothClass);

    CARAPI GetTrustState(
        /* [out] */ Boolean* state);

    CARAPI SetTrust(
        /* [in] */ Boolean value,
        /* [out] */ Boolean* result);

    CARAPI GetUuids(
        /* [out, callee] */ ArrayOf<IParcelUuid*>** uuids);

    CARAPI FetchUuidsWithSdp(
        /* [out] */ Boolean* result);

    CARAPI GetServiceChannel(
        /* [in] */ IParcelUuid* uuid,
        /* [out] */ Int32* channel);

    CARAPI SetPin(
        /* [in] */ ArrayOf<Byte>* pin,
        /* [out] */ Boolean* result);

    CARAPI SetPasskey(
        /* [in] */ Int32 passkey,
        /* [out] */ Boolean* result);

    CARAPI SetPairingConfirmation(
        /* [in] */ Boolean confirm,
        /* [out] */ Boolean* result);

    CARAPI SetRemoteOutOfBandData(
        /* [out] */ Boolean* result);

    CARAPI CancelPairingUserInput(
        /* [out] */ Boolean* result);

    CARAPI IsBluetoothDock(
        /* [out] */ Boolean* isBluetoothDock);

    CARAPI CreateRfcommSocket(
        /* [in] */ Int32 channel,
        /* [out] */ IBluetoothSocket** socket);

    CARAPI CreateRfcommSocketToServiceRecord(
        /* [in] */ IUUID* uuid,
        /* [out] */ IBluetoothSocket** socket);

    CARAPI CreateInsecureRfcommSocketToServiceRecord(
        /* [in] */ IUUID* uuid,
        /* [out] */ IBluetoothSocket** socket);

    CARAPI CreateInsecureRfcommSocket(
        /* [in] */ Int32 port,
        /* [out] */ IBluetoothSocket** socket);

    CARAPI CreateScoSocket(
        /* [out] */ IBluetoothSocket** socket);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    static CARAPI_(AutoPtr< ArrayOf<Byte> >) ConvertPinToBytes(
        /* [in] */ const String& pin);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& address);

private:
    const static String TAG;
    const static Boolean DBG;

    static AutoPtr<IIBluetooth> sService;
    String mAddress;
    static AutoPtr<IIBluetoothManagerCallback> mStateChangeCallback;
    static Object sLock;

    friend class CBluetoothDeviceManagerCallback;
};

} // Bluetooth
} // Droid
} // Elastos

#endif // __ELASTOS_DROID_BLUETOOTH_CBLUETOOTHDEVICE_H__
