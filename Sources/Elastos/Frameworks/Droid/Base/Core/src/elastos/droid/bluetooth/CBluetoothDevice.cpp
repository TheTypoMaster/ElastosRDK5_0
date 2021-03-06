
#include "ext/frameworkdef.h"
#include "CBluetoothDevice.h"
#include "CBluetoothClass.h"
#include "BluetoothSocket.h"
#include "CBluetoothAdapter.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Bluetooth {

const String CBluetoothDevice::TAG("BluetoothDevice");
const Boolean CBluetoothDevice::DBG = FALSE;
AutoPtr<IIBluetooth> CBluetoothDevice::sService;

static AutoPtr<IIBluetoothManagerCallback> InitStateChangeCallback()
{
    AutoPtr<IIBluetoothManagerCallback> callback;
    CBluetoothDeviceManagerCallback::New((IIBluetoothManagerCallback**)&callback);
    return callback;
}
AutoPtr<IIBluetoothManagerCallback> CBluetoothDevice::mStateChangeCallback = InitStateChangeCallback();

Mutex CBluetoothDevice::sLock;

AutoPtr<IIBluetooth> CBluetoothDevice::GetService()
{
    AutoLock lock(sLock);

    if (sService == NULL) {
        AutoPtr<IBluetoothAdapter> adapter = CBluetoothAdapter::GetDefaultAdapter();
        if(adapter != NULL){
            sService = ((CBluetoothAdapter*)adapter.Get())->GetBluetoothService(mStateChangeCallback);
        }
    }

    return sService;
}

ECode CBluetoothDevice::Equals(
    /* [in] */ IInterface *obj,
    /* [out] */ Boolean *equal)
{
    VALIDATE_NOT_NULL(equal)
    *equal = FALSE;
    AutoPtr<IBluetoothDevice> device = IBluetoothDevice::Probe(obj);
    if (device != NULL) {
        String addr;
        device->GetAddress(&addr);
        *equal = mAddress.Equals(addr);
    }
    return NOERROR;
}

ECode CBluetoothDevice::ToString(
    /* [out] */ String* s)
{
    VALIDATE_NOT_NULL(s)
    *s = mAddress;
    return NOERROR;
}

ECode CBluetoothDevice::DescribeContents(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    *value = 0;
    return NOERROR;
}

ECode CBluetoothDevice::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadString(&mAddress);
    return NOERROR;
}

ECode CBluetoothDevice::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteString(mAddress);
    return NOERROR;
}

ECode CBluetoothDevice::GetAddress(
    /* [out] */ String* address)
{
    VALIDATE_NOT_NULL(address)

    if (DBG) {
        Logger::D(TAG, String("mAddress: ") + mAddress);
    }

    *address = mAddress;
    return NOERROR;
}

ECode CBluetoothDevice::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    *name = String(NULL);

    if (sService == NULL) {
        Logger::E(TAG, "BT not enabled. Cannot get Remote Device name");
        return NOERROR;
    }
//    try {
    return sService->GetRemoteName(this, name);
//    } catch (RemoteException e) {Log.e(TAG, "", e);}
}

ECode CBluetoothDevice::GetAlias(
    /* [out] */ String* alias)
{
    VALIDATE_NOT_NULL(alias)
    *alias = String(NULL);
    if (sService == NULL) {
        Logger::E(TAG, "BT not enabled. Cannot get Remote Device Alias");
        return NOERROR;
    }
//    try {
    return sService->GetRemoteAlias(this, alias);
//    } catch (RemoteException e) {Log.e(TAG, "", e);}
}

ECode CBluetoothDevice::SetAlias(
    /* [in] */ const String& alias,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    if (sService == NULL) {
        Logger::E(TAG, "BT not enabled. Cannot set Remote Device name");
        return NOERROR;
    }
//    try {
    return sService->SetRemoteAlias(this, alias, result);
//    } catch (RemoteException e) {Log.e(TAG, "", e);}
}

ECode CBluetoothDevice::GetAliasName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    String alias;
    GetAlias(&alias);
    if (alias.IsNull()) {
        GetName(&alias);
    }
    *name = alias;
    return NOERROR;
}

ECode CBluetoothDevice::CreateBond(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    if (sService == NULL) {
        Logger::E(TAG, "BT not enabled. Cannot create bond to Remote Device");
        return NOERROR;
    }
//    try {
    return sService->CreateBond(this, result);
//    } catch (RemoteException e) {Log.e(TAG, "", e);}
}

ECode CBluetoothDevice::CreateBondOutOfBand(
    /* [in] */ ArrayOf<Byte>* hash,
    /* [in] */ ArrayOf<Byte>* randomizer,
    /* [out] */ Boolean* result)
{
    //TODO(BT)

    /*
    try {
        return sService.createBondOutOfBand(this, hash, randomizer);
    } catch (RemoteException e) {Log.e(TAG, "", e);}*/

    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode CBluetoothDevice::SetDeviceOutOfBandData(
    /* [in] */ ArrayOf<Byte>* hash,
    /* [in] */ ArrayOf<Byte>* randomizer,
    /* [out] */ Boolean* result)
{
    //TODO(BT)
    /*
    try {
    return sService.setDeviceOutOfBandData(this, hash, randomizer);
    } catch (RemoteException e) {Log.e(TAG, "", e);} */
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode CBluetoothDevice::CancelBondProcess(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    if (sService == NULL) {
        Logger::E(TAG, "BT not enabled. Cannot cancel Remote Device bond");
        return NOERROR;
    }
//    try {
    return sService->CancelBondProcess(this, result);
//    } catch (RemoteException e) {Log.e(TAG, "", e);}
}

ECode CBluetoothDevice::RemoveBond(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    if (sService == NULL) {
        Logger::E(TAG, "BT not enabled. Cannot remove Remote Device bond");
        return NOERROR;
    }

//    try {
        return sService->RemoveBond(this, result);
//    } catch (RemoteException e) {Log.e(TAG, "", e);}
}

ECode CBluetoothDevice::GetBondState(
    /* [in] */ Int32* state)
{
    VALIDATE_NOT_NULL(state)
    *state = IBluetoothDevice::BOND_NONE;
    if (sService == NULL) {
        Logger::E(TAG, "BT not enabled. Cannot get bond state");
        return NOERROR;
    }

//    try {
    return sService->GetBondState(this, state);
//    } catch (RemoteException e) {Log.e(TAG, "", e);}
//    catch (NullPointerException npe) {
//        // Handle case where bluetooth service proxy
//        // is already null.
//        Log.e(TAG, "NullPointerException for getBondState() of device ("+
//            getAddress()+")", npe);
//    }
}

ECode CBluetoothDevice::GetBluetoothClass(
    /* [out] */ IBluetoothClass** bluetoothClass)
{
    VALIDATE_NOT_NULL(bluetoothClass)
    *bluetoothClass = NULL;
    if (sService == NULL) {
        Logger::E(TAG, "BT not enabled. Cannot get Bluetooth Class");
        return NOERROR;
    }

//    try {
    Int32 classInt;
    sService->GetRemoteClass(this, &classInt);
    if (classInt == IBluetoothClass::ERROR) {
        *bluetoothClass = NULL;
        return NOERROR;
    }
    return CBluetoothClass::New(classInt, bluetoothClass);
//    } catch (RemoteException e) {Log.e(TAG, "", e);}
}

ECode CBluetoothDevice::GetTrustState(
    /* [out] */ Boolean* state)
{
    //TODO(BT)
    /*
    try {
        return sService.getTrustState(this);
    } catch (RemoteException e) {
        Log.e(TAG, "", e);
    }*/
    VALIDATE_NOT_NULL(state)
    *state = FALSE;
    return NOERROR;
}

ECode CBluetoothDevice::SetTrust(
    /* [in] */ Boolean value,
    /* [out] */ Boolean* result)
{
    //TODO(BT)
    /*
    try {
        return sService.setTrust(this, value);
    } catch (RemoteException e) {
        Log.e(TAG, "", e);
    }*/
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode CBluetoothDevice::GetUuids(
    /* [out, callee] */ ArrayOf<IParcelUuid *>** uuids)
{
    VALIDATE_NOT_NULL(uuids)
    *uuids = NULL;
    if (sService == NULL) {
        Logger::E(TAG, "BT not enabled. Cannot get remote device Uuids");
        return NOERROR;
     }
//    try {
    return sService->GetRemoteUuids(this, uuids);
//    } catch (RemoteException e) {Log.e(TAG, "", e);}
}

ECode CBluetoothDevice::FetchUuidsWithSdp(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
//    try {
    return sService->FetchRemoteUuids(this, result);
//    } catch (RemoteException e) {Log.e(TAG, "", e);}
}

ECode CBluetoothDevice::GetServiceChannel(
    /* [in] */ IParcelUuid* uuid,
    /* [out] */ Int32* channel)
{
    //TODO(BT)
    /*
     try {
         return sService.getRemoteServiceChannel(this, uuid);
     } catch (RemoteException e) {Log.e(TAG, "", e);}*/
    VALIDATE_NOT_NULL(channel)
    *channel = IBluetoothDevice::ERROR;
    return NOERROR;
}

ECode CBluetoothDevice::SetPin(
    /* [in] */ ArrayOf<Byte>* pin,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    if (sService == NULL) {
        Logger::E(TAG, "BT not enabled. Cannot set Remote Device pin");
        return NOERROR;
    }
//    try {
    return sService->SetPin(this, TRUE, pin->GetLength(), pin, result);
//    } catch (RemoteException e) {Log.e(TAG, "", e);}
}

ECode CBluetoothDevice::SetPasskey(
    /* [in] */ Int32 passkey,
    /* [out] */ Boolean* result)
{
    //TODO(BT)
    /*
    try {
        return sService.setPasskey(this, true, 4, passkey);
    } catch (RemoteException e) {Log.e(TAG, "", e);}*/
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode CBluetoothDevice::SetPairingConfirmation(
    /* [in] */ Boolean confirm,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    if (sService == NULL) {
        Logger::E(TAG, "BT not enabled. Cannot set pairing confirmation");
        return NOERROR;
    }
//    try {
    return sService->SetPairingConfirmation(this, confirm, result);
//    } catch (RemoteException e) {Log.e(TAG, "", e);}
}

ECode CBluetoothDevice::SetRemoteOutOfBandData(
    /* [out] */ Boolean* result)
{
    // TODO(BT)
    /*
    try {
      return sService.setRemoteOutOfBandData(this);
  } catch (RemoteException e) {Log.e(TAG, "", e);}*/
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode CBluetoothDevice::CancelPairingUserInput(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    if (sService == NULL) {
        Logger::E(TAG, "BT not enabled. Cannot create pairing user input");
        return NOERROR;
    }
//    try {
    return sService->CancelBondProcess(this, result);
//    } catch (RemoteException e) {Log.e(TAG, "", e);}
}

ECode CBluetoothDevice::IsBluetoothDock(
    /* [out] */ Boolean* isBluetoothDock)
{
    // TODO(BT)
    /*
    try {
        return sService.isBluetoothDock(this);
    } catch (RemoteException e) {Log.e(TAG, "", e);}*/

    VALIDATE_NOT_NULL(isBluetoothDock)
    *isBluetoothDock = FALSE;
    return NOERROR;

}

ECode CBluetoothDevice::CreateRfcommSocket(
    /* [in] */ Int32 channel,
    /* [out] */ IBluetoothSocket** socket)
{
    VALIDATE_NOT_NULL(socket)
    AutoPtr<IBluetoothSocket> _socket = (IBluetoothSocket*)new BluetoothSocket(BluetoothSocket::TYPE_RFCOMM, -1, TRUE, TRUE, this, channel, NULL);
    *socket = _socket;
    REFCOUNT_ADD(*socket)
    return NOERROR;
}

ECode CBluetoothDevice::CreateRfcommSocketToServiceRecord(
    /* [in] */ IUUID* uuid,
    /* [out] */ IBluetoothSocket** socket)
{
    VALIDATE_NOT_NULL(socket)
    assert(0);
//    return new BluetoothSocket(BluetoothSocket.TYPE_RFCOMM, -1, true, true, this, -1, new ParcelUuid(uuid));
    return E_NOT_IMPLEMENTED;
}

ECode CBluetoothDevice::CreateInsecureRfcommSocketToServiceRecord(
    /* [in] */ IUUID* uuid,
    /* [out] */ IBluetoothSocket** socket)
{
    VALIDATE_NOT_NULL(socket)
    assert(0);
//    return new BluetoothSocket(BluetoothSocket.TYPE_RFCOMM, -1, false, false, this, -1, new ParcelUuid(uuid));
    return E_NOT_IMPLEMENTED;
}

ECode CBluetoothDevice::CreateInsecureRfcommSocket(
    /* [in] */ Int32 port,
    /* [out] */ IBluetoothSocket** socket)
{
    VALIDATE_NOT_NULL(socket)
    AutoPtr<IBluetoothSocket> _socket = (IBluetoothSocket*)new BluetoothSocket(BluetoothSocket::TYPE_RFCOMM, -1, FALSE, FALSE, this, port, NULL);
    *socket = _socket;
    REFCOUNT_ADD(*socket)
    return NOERROR;
}

ECode CBluetoothDevice::CreateScoSocket(
    /* [out] */ IBluetoothSocket** socket)
{
    VALIDATE_NOT_NULL(socket)
    AutoPtr<IBluetoothSocket> _socket = (IBluetoothSocket*)new BluetoothSocket(BluetoothSocket::TYPE_SCO, -1, TRUE, TRUE, this, -1, NULL);
    *socket = _socket;
    REFCOUNT_ADD(*socket)
    return NOERROR;
}

AutoPtr< ArrayOf<Byte> > CBluetoothDevice::ConvertPinToBytes(
    /* [in] */ const String& pin)
{
    if (pin == NULL) {
        return NULL;
    }
    // try {
    AutoPtr< ArrayOf<Byte> > pinBytes = ArrayOf<Byte>::Alloc((Byte*)const_cast<char*>(pin.string()), pin.GetByteLength());
    // pinBytes = pin.getBytes("UTF-8");
    // } catch (UnsupportedEncodingException uee) {
    //     Log.e(TAG, "UTF-8 not supported?!?");  // this should not happen
    //     return null;
    // }
    if (pinBytes->GetLength() <= 0 || pinBytes->GetLength() > 16) {
        return NULL;
    }
    return pinBytes;
}

ECode CBluetoothDevice::constructor()
{
    return NOERROR;
}

ECode CBluetoothDevice::constructor(
    /* [in] */ const String& address)
{
    GetService();  // ensures sService is initialized

    if (!CBluetoothAdapter::CheckBluetoothAddress(address)) {
//        throw new IllegalArgumentException(address + " is not a valid Bluetooth address");
        Logger::E(TAG, "%s is not a valid Bluetooth address", address.string());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mAddress = address;
    return NOERROR;
}

} // Bluetooth
} // Droid
} // Elastos

