
#include "BluetoothInputDevice.h"
#include "CBluetoothAdapter.h"
#include "CBluetoothInputDeviceStateChangeCallback.h"
#include "content/CIntent.h"
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Core::StringUtils;
using Elastos::Droid::Content::EIID_IServiceConnection;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;

namespace Elastos {
namespace Droid {
namespace Bluetooth {

/////////////////////////////////////////////////////
// ServiceConnection
/////////////////////////////////////////////////////

BluetoothInputDevice::ServiceConnection::ServiceConnection(
    /* [in] */ BluetoothInputDevice* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(BluetoothInputDevice::ServiceConnection, IServiceConnection)

ECode BluetoothInputDevice::ServiceConnection::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    if (DBG) Logger::D(TAG, "Proxy object connected");
    mHost->mService = IIBluetoothInputDevice::Probe(service);

    if (mHost->mServiceListener != NULL) {
        mHost->mServiceListener->OnServiceConnected(IBluetoothProfile::INPUT_DEVICE, (IBluetoothProfile*)mHost);
    }
    return NOERROR;
}

ECode BluetoothInputDevice::ServiceConnection::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    if (DBG) Logger::D(TAG, "Proxy object disconnected");
    mHost->mService = NULL;
    if (mHost->mServiceListener != NULL) {
        mHost->mServiceListener->OnServiceDisconnected(IBluetoothProfile::INPUT_DEVICE);
    }
    return NOERROR;
}


/////////////////////////////////////////////////////
// BluetoothInputDevice
/////////////////////////////////////////////////////

const String BluetoothInputDevice::ACTION_CONNECTION_STATE_CHANGED ("android.bluetooth.input.profile.action.CONNECTION_STATE_CHANGED");
const String BluetoothInputDevice::ACTION_PROTOCOL_MODE_CHANGED("android.bluetooth.input.profile.action.PROTOCOL_MODE_CHANGED");
const String BluetoothInputDevice::ACTION_VIRTUAL_UNPLUG_STATUS("android.bluetooth.input.profile.action.VIRTUAL_UNPLUG_STATUS");
const Int32 BluetoothInputDevice::INPUT_DISCONNECT_FAILED_NOT_CONNECTED;
const Int32 BluetoothInputDevice::INPUT_CONNECT_FAILED_ALREADY_CONNECTED;
const Int32 BluetoothInputDevice::INPUT_CONNECT_FAILED_ATTEMPT_FAILED;
const Int32 BluetoothInputDevice::INPUT_OPERATION_GENERIC_FAILURE;
const Int32 BluetoothInputDevice::INPUT_OPERATION_SUCCESS;
const Int32 BluetoothInputDevice::PROTOCOL_REPORT_MODE;
const Int32 BluetoothInputDevice::PROTOCOL_BOOT_MODE;
const Int32 BluetoothInputDevice::PROTOCOL_UNSUPPORTED_MODE;
const Byte BluetoothInputDevice::REPORT_TYPE_INPUT;
const Byte BluetoothInputDevice::REPORT_TYPE_OUTPUT;
const Byte BluetoothInputDevice::REPORT_TYPE_FEATURE;
const Int32 BluetoothInputDevice::VIRTUAL_UNPLUG_STATUS_SUCCESS;
const Int32 BluetoothInputDevice::VIRTUAL_UNPLUG_STATUS_FAIL;
const String BluetoothInputDevice::EXTRA_PROTOCOL_MODE("android.bluetooth.BluetoothInputDevice.extra.PROTOCOL_MODE");
const String BluetoothInputDevice::EXTRA_REPORT_TYPE("android.bluetooth.BluetoothInputDevice.extra.REPORT_TYPE");
const String BluetoothInputDevice::EXTRA_REPORT_ID("android.bluetooth.BluetoothInputDevice.extra.REPORT_ID");
const String BluetoothInputDevice::EXTRA_REPORT_BUFFER_SIZE("android.bluetooth.BluetoothInputDevice.extra.REPORT_BUFFER_SIZE");
const String BluetoothInputDevice::EXTRA_REPORT("android.bluetooth.BluetoothInputDevice.extra.REPORT");
const String BluetoothInputDevice::EXTRA_VIRTUAL_UNPLUG_STATUS("android.bluetooth.BluetoothInputDevice.extra.VIRTUAL_UNPLUG_STATUS");
const String BluetoothInputDevice::TAG("BluetoothInputDevice");
const Boolean BluetoothInputDevice::DBG = TRUE;
const Boolean BluetoothInputDevice::VDBG = FALSE;

BluetoothInputDevice::BluetoothInputDevice(
    /* [in] */ IContext* context,
    /* [in] */ IBluetoothProfileServiceListener* listener)
    : mContext(context)
    , mServiceListener(listener)
{
    CBluetoothInputDeviceStateChangeCallback::New(this, (IIBluetoothStateChangeCallback**)&mBluetoothStateChangeCallback);
    mConnection = new ServiceConnection(this);

    mAdapter = CBluetoothAdapter::GetDefaultAdapter();
    if(mAdapter != NULL){
        AutoPtr<IIBluetoothManager> mgr = ((CBluetoothAdapter*)mAdapter.Get())->GetBluetoothManager();
        if (mgr != NULL) {
//            try {
            ECode ec = mgr->RegisterStateChangeCallback(mBluetoothStateChangeCallback);
            if (FAILED(ec)) {
                Logger::E(TAG, "0x%08x", ec);
            }
//            } catch (RemoteException e) {
//                Log.e(TAG,"",e);
//            }
        }
    }

    AutoPtr<IIntent> intent;
    CIntent::New(String("IBluetoothInputDevice")/*IBluetoothInputDevice.class.getName()*/, (IIntent**)&intent);
    Boolean result;
    if (context->BindService(intent, mConnection, 0, &result), !result) {
        Logger::E(TAG, "Could not bind to Bluetooth HID Service");
    }
}

CAR_INTERFACE_IMPL(BluetoothInputDevice, IBluetoothProfile)

ECode BluetoothInputDevice::Close()
{
    if (VDBG) {
        Logger::D(TAG, "close()");
    }

    if(mAdapter != NULL){
        AutoPtr<IIBluetoothManager> mgr = ((CBluetoothAdapter*)mAdapter.Get())->GetBluetoothManager();
        if (mgr != NULL) {
    //        try {
            ECode ec = mgr->UnregisterStateChangeCallback(mBluetoothStateChangeCallback);
            if (FAILED(ec)) {
                Logger::E(TAG, "0x%08x", ec);
            }
    //        } catch (Exception e) {
    //            Log.e(TAG,"",e);
    //        }
        }
    }

    AutoLock lock(mConnectionLock);
    if (mService != NULL) {
//            try {
        mService = NULL;
        ECode ec = mContext->UnbindService(mConnection);
        if (FAILED(ec)) {
            Logger::E(TAG, "0x%08x", ec);
        }
//            } catch (Exception re) {
//                Log.e(TAG,"",re);
//            }
    }
    mServiceListener = NULL;
    return NOERROR;
}

ECode BluetoothInputDevice::Connect(
    /* [in] */ IBluetoothDevice* device,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    if (DBG) {
        String str;
        IObject::Probe(device)->ToString(&str);
        Logger::D(TAG, "connect(%s)", str.string());
    }
    if (mService != NULL && IsEnabled() && IsValidDevice(device)) {
//        try {
        return mService->Connect(device, result);
//        } catch (RemoteException e) {
//            Log.e(TAG, "Stack:" + Log.getStackTraceString(new Throwable()));
//            return false;
//        }
    }
    if (mService == NULL) {
        Logger::W(TAG, "Proxy not attached to service");
    }
    return NOERROR;
}

ECode BluetoothInputDevice::Disconnect(
    /* [in] */ IBluetoothDevice* device,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    if (DBG) {
        String str;
        IObject::Probe(device)->ToString(&str);
        Logger::D(TAG, "disconnect(%s)", str.string());
    }

    if (mService != NULL && IsEnabled() && IsValidDevice(device)) {
//        try {
        return mService->Disconnect(device, result);
//        } catch (RemoteException e) {
//            Log.e(TAG, "Stack:" + Log.getStackTraceString(new Throwable()));
//            return false;
//        }
    }
    if (mService == NULL) {
        Logger::W(TAG, "Proxy not attached to service");
    }
    return NOERROR;
}

ECode BluetoothInputDevice::GetConnectedDevices(
    /* [out, callee] */ ArrayOf<IBluetoothDevice*>** devices)
{
    VALIDATE_NOT_NULL(devices)
    *devices = NULL;
    AutoPtr< ArrayOf<IBluetoothDevice*> > _devices = ArrayOf<IBluetoothDevice*>::Alloc(0);

    if (VDBG) Logger::E(TAG, "getConnectedDevices()");
    if (mService != NULL && IsEnabled()) {
        // try {
        return mService->GetConnectedDevices(devices);
        // } catch (RemoteException e) {
        //     Log.e(TAG, "Stack:" + Log.getStackTraceString(new Throwable()));
        //     return new ArrayList<BluetoothDevice>();
        // }
    }
    if (mService == NULL) Logger::W(TAG, "Proxy not attached to service");
    *devices = _devices;
    REFCOUNT_ADD(*devices)
    return NOERROR;
}

ECode BluetoothInputDevice::GetDevicesMatchingConnectionStates(
    /* [in] */ ArrayOf<Int32>* states,
    /* [out, callee] */ ArrayOf<IBluetoothDevice*>** devices)
{
    VALIDATE_NOT_NULL(*devices)
    *devices = NULL;
    AutoPtr< ArrayOf<IBluetoothDevice*> > _devices = ArrayOf<IBluetoothDevice*>::Alloc(0);

    if (VDBG) Logger::D(TAG, "getDevicesMatchingStates()");
    if (mService != NULL && IsEnabled()) {
        // try {
        return mService->GetDevicesMatchingConnectionStates(states, devices);
        // } catch (RemoteException e) {
        //     Log.e(TAG, "Stack:" + Log.getStackTraceString(new Throwable()));
        //     return new ArrayList<BluetoothDevice>();
        // }
    }
    if (mService == NULL) Logger::W(TAG, "Proxy not attached to service");
    *devices = _devices;
    REFCOUNT_ADD(*devices)
    return NOERROR;
}

ECode BluetoothInputDevice::GetConnectionState(
    /* [in] */ IBluetoothDevice* device,
    /* [out] */ Int32* state)
{
    VALIDATE_NOT_NULL(state)
    *state = IBluetoothProfile::STATE_DISCONNECTED;

    if (VDBG) {
        String str;
        IObject::Probe(device)->ToString(&str);
        Logger::D(TAG, "getState(%s)", str.string());
    }

    if (mService != NULL && IsEnabled() && IsValidDevice(device)) {
//        try {
        return mService->GetConnectionState(device, state);
//        } catch (RemoteException e) {
//            Log.e(TAG, "Stack:" + Log.getStackTraceString(new Throwable()));
//            return BluetoothProfile.STATE_DISCONNECTED;
//        }
    }
    if (mService == NULL) {
        Logger::W(TAG, "Proxy not attached to service");
    }
    return NOERROR;
}

ECode BluetoothInputDevice::SetPriority(
    /* [in] */ IBluetoothDevice* device,
    /* [in] */ Int32 priority,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    if (DBG) {
        String str;
        IObject::Probe(device)->ToString(&str);
        Logger::D(TAG, "setPriority(%s, %d)", str.string(), priority);
    }

    if (mService != NULL && IsEnabled() && IsValidDevice(device)) {
        if (priority != IBluetoothProfile::PRIORITY_OFF && priority != IBluetoothProfile::PRIORITY_ON) {
            return NOERROR;
        }
//        try {
        return mService->SetPriority(device, priority, result);
//        } catch (RemoteException e) {
//            Log.e(TAG, "Stack:" + Log.getStackTraceString(new Throwable()));
//            return false;
//        }
    }
    if (mService == NULL) {
        Logger::W(TAG, "Proxy not attached to service");
    }
    return NOERROR;
}

ECode BluetoothInputDevice::GetPriority(
    /* [in] */ IBluetoothDevice* device,
    /* [out] */ Int32* priority)
{
    VALIDATE_NOT_NULL(priority)
    *priority = IBluetoothProfile::PRIORITY_OFF;

    if (VDBG) {
        String str;
        IObject::Probe(device)->ToString(&str);
        Logger::D(TAG, "getPriority(%s)", str.string());
    }

    if (mService != NULL && IsEnabled() && IsValidDevice(device)) {
//        try {
        return mService->GetPriority(device, priority);
//        } catch (RemoteException e) {
//            Log.e(TAG, "Stack:" + Log.getStackTraceString(new Throwable()));
//            return BluetoothProfile.PRIORITY_OFF;
//        }
    }
    if (mService == NULL) {
        Logger::W(TAG, "Proxy not attached to service");
    }
    return NOERROR;
}

ECode BluetoothInputDevice::VirtualUnplug(
    /* [in] */ IBluetoothDevice* device,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    if (DBG) {
        String str;
        IObject::Probe(device)->ToString(&str);
        Logger::D(TAG, "virtualUnplug(%s)", str.string());
    }
    if (mService != NULL && IsEnabled() && IsValidDevice(device)) {
//        try {
        return mService->VirtualUnplug(device, result);
//        } catch (RemoteException e) {
//            Log.e(TAG, "Stack:" + Log.getStackTraceString(new Throwable()));
//            return false;
//        }
    }
    if (mService == NULL) {
        Logger::W(TAG, "Proxy not attached to service");
    }
    return NOERROR;

}

Boolean BluetoothInputDevice::IsEnabled()
{
    if (mAdapter != NULL){
        Int32 state;
        if (mAdapter->GetState(&state), state == IBluetoothAdapter::STATE_ON) {
            return TRUE;
        }
    }

    return FALSE;
}

Boolean BluetoothInputDevice::IsValidDevice(
    /* [in] */ IBluetoothDevice* device)
{
    if (device == NULL) {
        return FALSE;
    }
    String address;
    device->GetAddress(&address);
    if (CBluetoothAdapter::CheckBluetoothAddress(address)) {
        return TRUE;
    }
    return FALSE;
}

ECode BluetoothInputDevice::GetProtocolMode(
    /* [in] */ IBluetoothDevice* device,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    if (VDBG) {
        String str;
        IObject::Probe(device)->ToString(&str);
        Logger::D(TAG, "getProtocolMode(%s)", str.string());
    }
    if (mService != NULL && IsEnabled() && IsValidDevice(device)) {
//        try {
        return mService->GetProtocolMode(device, result);
//        } catch (RemoteException e) {
//            Log.e(TAG, "Stack:" + Log.getStackTraceString(new Throwable()));
//            return false;
//        }
    }
    if (mService == NULL) {
        Logger::W(TAG, "Proxy not attached to service");
    }
    return NOERROR;
}

ECode BluetoothInputDevice::SetProtocolMode(
    /* [in] */ IBluetoothDevice* device,
    /* [in] */ Int32 protocolMode,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    if (DBG) {
        String str;
        IObject::Probe(device)->ToString(&str);
        Logger::D(TAG, "setProtocolMode(%s)", str.string());
    }
    if (mService != NULL && IsEnabled() && IsValidDevice(device)) {
//        try {
        return mService->SetProtocolMode(device, protocolMode, result);
//        } catch (RemoteException e) {
//            Log.e(TAG, "Stack:" + Log.getStackTraceString(new Throwable()));
//            return false;
//        }
    }
    if (mService == NULL) {
        Logger::W(TAG, "Proxy not attached to service");
    }
    return NOERROR;
}

ECode BluetoothInputDevice::GetReport(
    /* [in] */ IBluetoothDevice* device,
    /* [in] */ Byte reportType,
    /* [in] */ Byte reportId,
    /* [in] */ Int32 bufferSize,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    if (VDBG) {
        String str;
        IObject::Probe(device)->ToString(&str);
        Logger::D(TAG, "getReport(%s), reportType=%d reportId=%d bufferSize=%d", str.string(), reportType, reportId, bufferSize);
    }
    if (mService != NULL && IsEnabled() && IsValidDevice(device)) {
//        try {
        return mService->GetReport(device, reportType, reportId, bufferSize, result);
//        } catch (RemoteException e) {
//            Log.e(TAG, "Stack:" + Log.getStackTraceString(new Throwable()));
//            return false;
//        }
    }
    if (mService == NULL) {
        Logger::W(TAG, "Proxy not attached to service");
    }
    return NOERROR;
}

ECode BluetoothInputDevice::SetReport(
    /* [in] */ IBluetoothDevice* device,
    /* [in] */ Byte reportType,
    /* [in] */ const String& report,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    if (DBG) {
        String str;
        IObject::Probe(device)->ToString(&str);
        Logger::D(TAG, "setReport(%s), reportType=%d report=%s", str.string(), reportType, report.string());
    }
    if (mService != NULL && IsEnabled() && IsValidDevice(device)) {
//        try {
        return mService->SetReport(device, reportType, report, result);
//        } catch (RemoteException e) {
//            Log.e(TAG, "Stack:" + Log.getStackTraceString(new Throwable()));
//            return false;
//        }
    }
    if (mService == NULL) {
        Logger::W(TAG, "Proxy not attached to service");
    }
    return NOERROR;
}

ECode BluetoothInputDevice::SendData(
    /* [in] */ IBluetoothDevice* device,
    /* [in] */ const String& report,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;

    if (DBG) {
        String str;
        IObject::Probe(device)->ToString(&str);
        Logger::D(TAG, "sendData(%s), report=%s", str.string(), report.string());
    }

    if (mService != NULL && IsEnabled() && IsValidDevice(device)) {
//        try {
        return mService->SendData(device, report, result);
//        } catch (RemoteException e) {
//            Log.e(TAG, "Stack:" + Log.getStackTraceString(new Throwable()));
//            return false;
//        }
    }
    if (mService == NULL) {
        Logger::W(TAG, "Proxy not attached to service");
    }
    return NOERROR;
}

} // Bluetooth
} // Droid
} // Elastos
