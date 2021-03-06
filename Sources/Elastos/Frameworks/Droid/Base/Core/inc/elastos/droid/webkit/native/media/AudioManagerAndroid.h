
#ifndef __ELASTOS_DROID_WEBKIT_MEDIA_AUDIOMANAGERANDROID_H__
#define __ELASTOS_DROID_WEBKIT_MEDIA_AUDIOMANAGERANDROID_H__

// import android.bluetooth.BluetoothAdapter;
// import android.bluetooth.BluetoothManager;
// import android.content.BroadcastReceiver;
// import android.content.ContentResolver;
// import android.content.Context;
// import android.content.Intent;
// import android.content.IntentFilter;
// import android.content.pm.PackageManager;
// import android.database.ContentObserver;
// import android.media.AudioFormat;
// import android.media.AudioManager;
// import android.media.AudioRecord;
// import android.media.AudioTrack;
// import android.media.audiofx.AcousticEchoCanceler;
// import android.os.Build;
// import android.os.Handler;
// import android.os.HandlerThread;
// import android.os.Process;
// import android.provider.Settings;
// import android.util.Log;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

// import java.util.ArrayList;
// import java.util.Arrays;
// import java.util.List;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

//@JNINamespace("media")
class AudioManagerAndroid
{
private:
    /**
     * NonThreadSafe is a helper class used to help verify that methods of a
     * class are called from the same thread.
     * Inspired by class in package com.google.android.apps.chrome.utilities.
     * Is only utilized when DEBUG is set to true.
     */
    class NonThreadSafe
    {
    public:
        NonThreadSafe();

        /**
         * Checks if the method is called on the valid thread.
         * Assigns the current thread if no thread was assigned.
         */
        CARAPI_(Boolean) CalledOnValidThread();

    private:
        const Int64 mThreadId;
    };

    /** Simple container for device information. */
    class AudioDeviceName
    {
    private:
        AudioDeviceName(
            /* [in] */ Int32 id,
            /* [in] */ String name);

        //@CalledByNative("AudioDeviceName")
        CARAPI_(String) Id();

        //@CalledByNative("AudioDeviceName")
        CARAPI_(String) Name();

    private:
        const Int32 mId;
        const String mName;
    };

    class WiredHeadsetBroadcastReceiver
        : public Object
        , public IBroadcastReceiver
    {
    public:
        WiredHeadsetBroadcastReceiver(
            /* [in] */ AudioManagerAndroid* owner);

        //@Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        AudioManagerAndroid* mOwner;

        static const Int32 STATE_UNPLUGGED = 0;
        static const Int32 STATE_PLUGGED = 1;
        static const Int32 HAS_NO_MIC = 0;
        static const Int32 HAS_MIC = 1;
    };

    class BluetoothHeadsetBroadcastReceiver
        : public Object
        , public IBroadcastReceiver
    {
    public:
        BluetoothHeadsetBroadcastReceiver(
            /* [in] */ AudioManagerAndroid* owner);

        //@Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        AudioManagerAndroid* mOwner;
    };

    class BluetoothScoIntentBroadcastReceiver
        : public Object
        , public IBroadcastReceiver
    {
    public:
        BluetoothScoIntentBroadcastReceiver(
            /* [in] */ AudioManagerAndroid* owner);

        //@Override
        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    private:
        AudioManagerAndroid* mOwner;
    };

    class InnerContentObserver
        : public Object
        , public IContentObserver
    {
    public:
        InnerContentObserver(
            /* [in] */ AudioManagerAndroid* owner);

        ECode OnChange(
            /* [in] */ Boolean selfChange);

    private:
        AudioManagerAndroid* mOwner;
    };

private:
    AudioManagerAndroid(
        /* [in] */ IContext* context,
        /* [in] */ Int64 nativeAudioManagerAndroid);

    static CARAPI_(Boolean) RunningOnJellyBeanOrHigher();

    static CARAPI_(Boolean) RunningOnJellyBeanMR1OrHigher();

    static CARAPI_(Boolean) RunningOnJellyBeanMR2OrHigher();

    /** Construction */
    //@CalledByNative
    static CARAPI_(AutoPtr<AudioManagerAndroid>) CreateAudioManagerAndroid(
        /* [in] */ IContext* context,
        /* [in] */ Int64 nativeAudioManagerAndroid);

    /**
     * Saves the initial speakerphone and microphone state.
     * Populates the list of available audio devices and registers receivers
     * for broadcast intents related to wired headset and Bluetooth devices.
     */
    //@CalledByNative
    CARAPI_(void) Init();

    /**
     * Unregister all previously registered intent receivers and restore
     * the stored state (stored in {@link #init()}).
     */
    //@CalledByNative
    CARAPI_(void) Close();

    /**
     * Saves current audio mode and sets audio mode to MODE_IN_COMMUNICATION
     * if input parameter is true. Restores saved audio mode if input parameter
     * is false.
     * Required permission: android.Manifest.permission.MODIFY_AUDIO_SETTINGS.
     */
    //@CalledByNative
    CARAPI_(void) SetCommunicationAudioModeOn(
        /* [in] */ Boolean on);

    /**
     * Activates, i.e., starts routing audio to, the specified audio device.
     *
     * @param deviceId Unique device ID (integer converted to string)
     * representing the selected device. This string is empty if the so-called
     * default device is requested.
     * Required permissions: android.Manifest.permission.MODIFY_AUDIO_SETTINGS
     * and android.Manifest.permission.RECORD_AUDIO.
     */
    //@CalledByNative
    CARAPI_(Boolean) SetDevice(
        /* [in] */ String deviceId);

    /**
     * @return the current list of available audio devices.
     * Note that this call does not trigger any update of the list of devices,
     * it only copies the current state in to the output array.
     * Required permissions: android.Manifest.permission.MODIFY_AUDIO_SETTINGS
     * and android.Manifest.permission.RECORD_AUDIO.
     */
    //@CalledByNative
    CARAPI_(AutoPtr< ArrayOf<AudioDeviceName> >) GetAudioInputDeviceNames();

    //@CalledByNative
    CARAPI_(Int32) GetNativeOutputSampleRate();

  /**
   * Returns the minimum frame size required for audio input.
   *
   * @param sampleRate sampling rate
   * @param channels number of channels
   */
    //@CalledByNative
    static CARAPI_(Int32) GetMinInputFrameSize(
        /* [in] */ Int32 sampleRate,
        /* [in] */ Int32 channels);

  /**
   * Returns the minimum frame size required for audio output.
   *
   * @param sampleRate sampling rate
   * @param channels number of channels
   */
    //@CalledByNative
    static CARAPI_(Int32) GetMinOutputFrameSize(
        /* [in] */ Int32 sampleRate,
        /* [in] */ Int32 channels);

    //@CalledByNative
    CARAPI_(Boolean) IsAudioLowLatencySupported();

    //@CalledByNative
    CARAPI_(Int32) GetAudioLowLatencyOutputFrameSize();

    //@CalledByNative
    static CARAPI_(Boolean) ShouldUseAcousticEchoCanceler();

    /**
     * Helper method for debugging purposes. Ensures that method is
     * called on same thread as this object was created on.
     */
    CARAPI_(void) CheckIfCalledOnValidThread();

    /**
     * Register for BT intents if we have the BLUETOOTH permission.
     * Also extends the list of available devices with a BT device if one exists.
     */
    CARAPI_(void) RegisterBluetoothIntentsIfNeeded();

    /** Unregister for BT intents if a registration has been made. */
    CARAPI_(void) UnregisterBluetoothIntentsIfNeeded();

    /** Sets the speaker phone mode. */
    CARAPI_(void) SetSpeakerphoneOn(
        /* [in] */ Boolean on);

    /** Sets the microphone mute state. */
    CARAPI_(void) SetMicrophoneMute(
        /* [in] */ Boolean on);

    /** Gets  the current microphone mute state. */
    CARAPI_(Boolean) IsMicrophoneMute();

    /** Gets the current earpiece state. */
    CARAPI_(Boolean) HasEarpiece();

    /**
     * Checks whether a wired headset is connected or not.
     * This is not a valid indication that audio playback is actually over
     * the wired headset as audio routing depends on other conditions. We
     * only use it as an early indicator (during initialization) of an attached
     * wired headset.
     */
    //@Deprecated
    CARAPI_(Boolean) HasWiredHeadset();

    /** Checks if the process has as specified permission or not. */
    CARAPI_(Boolean) HasPermission(
        /* [in] */ String permission);

    /**
     * Gets the current Bluetooth headset state.
     * android.bluetooth.BluetoothAdapter.getProfileConnectionState() requires
     * the BLUETOOTH permission.
     */
    CARAPI_(Boolean) HasBluetoothHeadset();

    /**
     * Registers receiver for the broadcasted intent when a wired headset is
     * plugged in or unplugged. The received intent will have an extra
     * 'state' value where 0 means unplugged, and 1 means plugged.
     */
    CARAPI_(void) RegisterForWiredHeadsetIntentBroadcast();

    /** Unregister receiver for broadcasted ACTION_HEADSET_PLUG intent. */
    CARAPI_(void) UnregisterForWiredHeadsetIntentBroadcast();

    /**
     * Registers receiver for the broadcasted intent related to BT headset
     * availability or a change in connection state of the local Bluetooth
     * adapter. Example: triggers when the BT device is turned on or off.
     * BLUETOOTH permission is required to receive this one.
     */
    CARAPI_(void) RegisterForBluetoothHeadsetIntentBroadcast();

    CARAPI_(void) UnregisterForBluetoothHeadsetIntentBroadcast();

    /**
     * Registers receiver for the broadcasted intent related the existence
     * of a BT SCO channel. Indicates if BT SCO streaming is on or off.
     */
    CARAPI_(void) RegisterForBluetoothScoIntentBroadcast();

    CARAPI_(void) UnregisterForBluetoothScoIntentBroadcast();

    /** Enables BT audio using the SCO audio channel. */
    CARAPI_(void) StartBluetoothSco();

    /** Disables BT audio using the SCO audio channel. */
    CARAPI_(void) StopBluetoothSco();

    /**
     * Changes selection of the currently active audio device.
     *
     * @param device Specifies the selected audio device.
     */
    CARAPI_(void) SetAudioDevice(
        /* [in] */ Int32 device);

    /**
     * Use a special selection scheme if the default device is selected.
     * The "most unique" device will be selected; Wired headset first,
     * then Bluetooth and last the speaker phone.
     */
    static CARAPI_(Int32) SelectDefaultDevice(
        /* [in] */ ArrayOf<Boolean>* devices);

    /** Returns true if setDevice() has been called with a valid device id. */
    CARAPI_(Boolean) DeviceHasBeenRequested();

    /**
     * Updates the active device given the current list of devices and
     * information about if a specific device has been selected or if
     * the default device is selected.
     */
    CARAPI_(void) UpdateDeviceActivation();

    /** Returns number of available devices */
    static CARAPI_(Int32) GetNumOfAudioDevices(
        /* [in] */ ArrayOf<Boolean>* devices);

    /**
     * For now, just log the state change but the idea is that we should
     * notify a registered state change listener (if any) that there has
     * been a change in the state.
     * TODO(henrika): add support for state change listener.
     */
    CARAPI_(void) ReportUpdate();

    /** Information about the current build, taken from system properties. */
    CARAPI_(void) LogDeviceInfo();

    /** Trivial helper method for debug logging */
    static CARAPI_(void) Logd(
        /* [in] */ String msg);

    /** Trivial helper method for error logging */
    static CARAPI_(void) Loge(
        /* [in] */ String msg);

    /** Start thread which observes volume changes on the voice stream. */
    CARAPI_(void) StartObservingVolumeChanges();

    /** Quit observer thread and stop listening for volume changes. */
    CARAPI_(void) StopObservingVolumeChanges();

    CARAPI_(void) NativeSetMute(
        /* [in] */ Int64 nativeAudioManagerAndroid,
        /* [in] */ Boolean muted);

private:
    static const String TAG;

    // Set to true to enable debug logs. Avoid in production builds.
    // NOTE: always check in as false.
    static const Boolean DEBUG;

    // List if device models which have been vetted for good quality platform
    // echo cancellation.
    // NOTE: only add new devices to this list if manual tests have been
    // performed where the AEC performance is evaluated using e.g. a WebRTC
    // audio client such as https://apprtc.appspot.com/?r=<ROOM NAME>.
    static const AutoPtr< ArrayOf<String> > SUPPORTED_AEC_MODELS;

    // Supported audio device types.
    static const Int32 DEVICE_DEFAULT = -2;
    static const Int32 DEVICE_INVALID = -1;
    static const Int32 DEVICE_SPEAKERPHONE = 0;
    static const Int32 DEVICE_WIRED_HEADSET = 1;
    static const Int32 DEVICE_EARPIECE = 2;
    static const Int32 DEVICE_BLUETOOTH_HEADSET = 3;
    static const Int32 DEVICE_COUNT = 4;

    // Maps audio device types to string values. This map must be in sync
    // with the device types above.
    // TODO(henrika): add support for proper detection of device names and
    // localize the name strings by using resource strings.
    // See http://crbug.com/333208 for details.
    static const AutoPtr< ArrayOf<String> > DEVICE_NAMES;

    // List of valid device types.
    static const AutoPtr< ArrayOf<Int32> > VALID_DEVICES;

    // Bluetooth audio SCO states. Example of valid state sequence:
    // SCO_INVALID -> SCO_TURNING_ON -> SCO_ON -> SCO_TURNING_OFF -> SCO_OFF.
    static const Int32 STATE_BLUETOOTH_SCO_INVALID = -1;
    static const Int32 STATE_BLUETOOTH_SCO_OFF = 0;
    static const Int32 STATE_BLUETOOTH_SCO_ON = 1;
    static const Int32 STATE_BLUETOOTH_SCO_TURNING_ON = 2;
    static const Int32 STATE_BLUETOOTH_SCO_TURNING_OFF = 3;

    // Use 44.1kHz as the default sampling rate.
    static const Int32 DEFAULT_SAMPLING_RATE = 44100;

    // Randomly picked up frame size which is close to return value on N4.
    // Return this value when getProperty(PROPERTY_OUTPUT_FRAMES_PER_BUFFER)
    // fails.
    static const Int32 DEFAULT_FRAME_PER_BUFFER = 256;

    const AutoPtr<IAudioManager> mAudioManager;
    const AutoPtr<IContext> mContext;
    const Int64 mNativeAudioManagerAndroid;

    // Enabled during initialization if MODIFY_AUDIO_SETTINGS permission is
    // granted. Required to shift system-wide audio settings.
    Boolean mHasModifyAudioSettingsPermission;

    // Enabled during initialization if RECORD_AUDIO permission is granted.
    Boolean mHasRecordAudioPermission;

    // Enabled during initialization if BLUETOOTH permission is granted.
    Boolean mHasBluetoothPermission;

    Int32 mSavedAudioMode;

    // Stores the audio states related to Bluetooth SCO audio, where some
    // states are needed to keep track of intermediate states while the SCO
    // channel is enabled or disabled (switching state can take a few seconds).
    Int32 mBluetoothScoState;

    Boolean mIsInitialized;
    Boolean mSavedIsSpeakerphoneOn;
    Boolean mSavedIsMicrophoneMute;

    // Id of the requested audio device. Can only be modified by
    // call to setDevice().
    Int32 mRequestedAudioDevice;

    // This class should be created, initialized and closed on the audio thread
    // in the audio manager. We use |mNonThreadSafe| to ensure that this is
    // the case. Only active when |DEBUG| is set to true.
    const NonThreadSafe mNonThreadSafe;

    // Lock to protect |mAudioDevices| and |mRequestedAudioDevice| which can
    // be accessed from the main thread and the audio manager thread.
    const Object mLock;

    // Contains a list of currently available audio devices.
    AutoPtr< ArrayOf<Boolean> > mAudioDevices;

    const AutoPtr<IContentResolver> mContentResolver;
    AutoPtr<IContentObserver> mSettingsObserver;
    AutoPtr<IHandlerThread> mSettingsObserverThread;
    Int32 mCurrentVolume;

    // Broadcast receiver for wired headset intent broadcasts.
    AutoPtr<IBroadcastReceiver> mWiredHeadsetReceiver;

    // Broadcast receiver for Bluetooth headset intent broadcasts.
    // Utilized to detect changes in Bluetooth headset availability.
    AutoPtr<IBroadcastReceiver> mBluetoothHeadsetReceiver;

    // Broadcast receiver for Bluetooth SCO broadcasts.
    // Utilized to detect if BT SCO streaming is on or off.
    AutoPtr<IBroadcastReceiver> mBluetoothScoReceiver;
};

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_MEDIA_AUDIOMANAGERANDROID_H__
