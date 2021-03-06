
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

GamepadDevice::GamepadDevice(
    /* [in] */ Int32 index,
    /* [in] */ IInputDevice* inputDevice)
{
    mIsStandardGamepad = FALSE;
    mAxisValues = ArrayOf<Float>::Alloc(CanonicalAxisIndex::NUM_CANONICAL_AXES);
    mButtonsValues = ArrayOf<Float>::Alloc(CanonicalButtonIndex::NUM_CANONICAL_BUTTONS);
    mRawButtons = ArrayOf<Float>::Alloc(256);
    mRawAxes = ArrayOf<Float>::Alloc(256);
    mDeviceIndex = index;
    inputDevice->GetId(&mDeviceId);
    inputDevice->GetName(&mDeviceName);
    mTimestamp = SystemClock::UptimeMillis();
    // Get axis ids and initialize axes values.
    final List<MotionRange> ranges = inputDevice.getMotionRanges();
    mAxes = ArrayOf<Int32>::Alloc(ranges.size());
    Int32 i = 0;
    for (MotionRange range : ranges) {
        if ((range.getSource() & InputDevice.SOURCE_CLASS_JOYSTICK) != 0) {
            int axis = range.getAxis();
            assert(axis < 256);
            mAxes[i++] = axis;
        }
    }
}

/**
 * Updates the axes and buttons maping of a gamepad device to a standard gamepad format.
 */
void GamepadDevice::updateButtonsAndAxesMapping()
{
    mIsStandardGamepad = GamepadMappings::MapToStandardGamepad(
            mAxisValues, mButtonsValues, mRawAxes, mRawButtons, mDeviceName);
}

/**
 * @return Device Id of the gamepad device.
 */
Int32 GamepadDevice::GetId()
{
    return mDeviceId;
}

/**
 * @return Mapping status of the gamepad device.
 */
Boolean GamepadDevice::IsStandardGamepad()
{
    return mIsStandardGamepad;
}

/**
 * @return Device name of the gamepad device.
 */
String GamepadDevice::GetName()
{
    return mDeviceName;
}

/**
 * @return Device index of the gamepad device.
 */
Int32 GamepadDevice::GetIndex()
{
    return mDeviceIndex;
}

/**
 * @return The timestamp when the gamepad device was last interacted.
 */
Int64 GamepadDevice::GetTimestamp()
{
    return mTimestamp;
}

/**
 * @return The axes state of the gamepad device.
 */
AutoPtr< ArrayOf<Float> > GamepadDevice::GetAxes()
{
    return mAxisValues;
}

/**
 * @return The buttons state of the gamepad device.
 */
AutoPtr< ArrayOf<Float> > GamepadDevice::GetButtons()
{
    return mButtonsValues;
}

/**
 * Reset the axes and buttons data of the gamepad device everytime gamepad data access is
 * paused.
 */
void GamepadDevice::ClearData()
{
    Arrays::Fill(mAxisValues, 0);
    Arrays::Fill(mRawAxes, 0);
    Arrays::Fill(mButtonsValues, 0);
    Arrays::Fill(mRawButtons, 0);
}

/**
 * Handles key event from the gamepad device.
 * @return True if the key event from the gamepad device has been consumed.
 */
Boolean GamepadDevice::HandleKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    // Ignore event if it is not for standard gamepad key.
    if (!GamepadList::IsGamepadEvent(event)) return FALSE;
    Int32 keyCode;
    event->GetKeyCode(&keyCode);
    assert(keyCode < 256);
    // Button value 0.0 must mean fully unpressed, and 1.0 must mean fully pressed.
    Int32 action;
    event->GetAction(&action)
    if (action == IKeyEvent::ACTION_DOWN) {
        (*mRawButtons)[keyCode] = 1.0f;
    }
    else if (action == IKeyEvent::ACTION_UP) {
        (*mRawButtons)[keyCode] = 0.0f;
    }

    event->GetEventTime(&mTimestamp);

    return TRUE;
}

/**
 * Handles motion event from the gamepad device.
 * @return True if the motion event from the gamepad device has been consumed.
 */
Boolean GamepadDevice::HandleMotionEvent(
    /* [in] */ IMotionEvent* event)
{
    // Ignore event if it is not a standard gamepad motion event.
    if (!GamepadList::IsGamepadEvent(event)) return FALSE;
    // Update axes values.
    for (Int32 i = 0; i < mAxes->GetLength(); i++) {
        Int32 axis = (*mAxes)[i];
        event->GetAxisValue(axis, &((*mRawAxes)[axis]));
    }

    event->GetEventTime(&mTimestamp);

    return TRUE;
}

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
