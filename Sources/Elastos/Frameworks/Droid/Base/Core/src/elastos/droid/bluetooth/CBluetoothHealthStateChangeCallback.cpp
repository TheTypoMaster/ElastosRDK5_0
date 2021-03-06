
#include "CBluetoothHealthStateChangeCallback.h"
#include "content/CIntent.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;

namespace Elastos {
namespace Droid {
namespace Bluetooth {

ECode CBluetoothHealthStateChangeCallback::OnBluetoothStateChange(
    /* [in] */ Boolean up)
{
    if (BluetoothHealth::DBG) Logger::D(BluetoothHealth::TAG, "onBluetoothStateChange: up=%d", up);
    if (!up) {
        if (BluetoothHealth::VDBG) Logger::D(BluetoothHealth::TAG, "Unbinding service...");
        AutoLock lock(mHost->mConnectionLock);
        // try {
        mHost->mService = NULL;
        ECode ec = mHost->mContext->UnbindService(mHost->mConnection);
        if (FAILED(ec)) {
            Logger::E(BluetoothHealth::TAG, "0x%08x", ec);
        }
        // } catch (Exception re) {
        //     Log.e(TAG,"",re);
        // }
    }
    else {
        AutoLock lock(mHost->mConnectionLock);
        // try {
        if (mHost->mService == NULL) {
            if (BluetoothHealth::VDBG) Logger::D(BluetoothHealth::TAG, "Binding service...");
            AutoPtr<IIntent> intent;
            CIntent::New(String("IBluetoothHealth")/*IBluetoothHealth.class.getName()*/, (IIntent**)&intent);
            Boolean result;
            ECode ec = mHost->mContext->BindService(intent, mHost->mConnection, 0, &result);
            if (FAILED(ec)) {
                Logger::E(BluetoothHealth::TAG, "0x%08x", ec);
            }
            else if (!result) {
                Logger::E(BluetoothHealth::TAG, "Could not bind to Bluetooth Headset Service");
            }
        }
        // } catch (Exception re) {
        //     Log.e(TAG,"",re);
        // }
    }
    return NOERROR;
}

ECode CBluetoothHealthStateChangeCallback::constructor(
    /* [in] */ IInterface* host)
{
	mHost = (BluetoothHealth*)host;
    return NOERROR;
}

} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos
