
#ifndef __BATTERYSTATUSMAMAGER_H__
#define __BATTERYSTATUSMAMAGER_H__

// import android.content.BroadcastReceiver;
// import android.content.Context;
// import android.content.Intent;
// import android.content.IntentFilter;
// import android.os.BatteryManager;
// import android.os.Build;
// import android.util.Log;

// import com.google.common.annotations.VisibleForTesting;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
 * Android implementation of the battery status APIs.
 */
//@JNINamespace("content")
class BatteryStatusManager
{
private:
  class InnerBroadcastReceiver
    : public Object
    , public IBroadcastReceiver
  {
  public:
    InnerBroadcastReceiver(
      /* [in] */ BatteryStatusManager* owner);

    CARAPI OnReceive(
      /* [in] */ IContext* context,
      /* [in] */ IIntent* intent);

  private:
    BatteryStatusManager* mOwner;
  };

public:
    //@CalledByNative
    static CARAPI_(AutoPtr<BatteryStatusManager>) GetInstance(
      /* [in] */ IContext* appContext);

    /**
     * Start listening for intents
     * @return True on success.
     */
    //@CalledByNative
    CARAPI_(Boolean) Start(
      /* [in] */ Int64 nativePtr);

    /**
     * Stop listening to intents.
     */
    //@CalledByNative
    CARAPI_(void) Stop();

    //@VisibleForTesting
    CARAPI_(void) OnReceive(
      /* [in] */ IIntent* intent);

protected:
    BatteryStatusManager(
      /* [in] */ IContext* context);

    /**
     * Returns whether the BatteryStatusManager should ignore the battery present state.
     * It is required for some devices that incorrectly set the EXTRA_PRESENT property.
     */
    CARAPI_(Boolean) IgnoreBatteryPresentState();

    CARAPI_(void) GotBatteryStatus(
      /* [in] */ Boolean charging,
      /* [in] */ Double chargingTime,
      /* [in] */ Double dischargingTime,
      /* [in] */ Double level);

private:
    /**
     * Native JNI call
     * see content/browser/battery_status/battery_status_manager.cc
     */
    CARAPI_(void) NativeGotBatteryStatus(
      /* [in] */ Int64 nativeBatteryStatusManager,
      /* [in] */ Boolean charging,
      /* [in] */ Double chargingTime,
      /* [in] */ Double dischargingTime,
      /* [in] */ Double level);

private:
    static const String TAG;

    // A reference to the application context in order to acquire the SensorService.
    const AutoPtr<IContext> mAppContext;
    const AutoPtr<IIntentFilter> mFilter;
    const AutoPtr<IBroadcastReceiver> mReceiver;

    // Non-zero if and only if we're listening for events.
    // To avoid race conditions on the C++ side, access must be synchronized.
    Int64 mNativePtr;

    // The lock to access the mNativePtr.
    const Object mNativePtrLock;

    Boolean mEnabled;
};

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__BATTERYSTATUSMAMAGER_H__
