
#ifndef __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_INPUT_MULTIFIELDTIMEPICKERDIALOG_H__
#define __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_INPUT_MULTIFIELDTIMEPICKERDIALOG_H__

// import android.app.AlertDialog;
// import android.content.Context;
// import android.content.DialogInterface;
// import android.content.DialogInterface.OnClickListener;
// import android.view.LayoutInflater;
// import android.view.View;
// import android.widget.NumberPicker;

// import org.chromium.content.R;

// import java.util.ArrayList;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

/**
 * A time picker dialog with upto 5 number pickers left to right:
 *  hour, minute, second, milli, AM/PM.
 *
 * If is24hourFormat is true then AM/PM picker is not displayed and
 * hour range is 0..23. Otherwise hour range is 1..12.
 * The milli picker is not displayed if step >= SECOND_IN_MILLIS
 * The second picker is not displayed if step >= MINUTE_IN_MILLIS.
 */
class MultiFieldTimePickerDialog
    : public IAlertDialog
    , public IOnClickListener
{
public:
    class OnMultiFieldTimeSetListener
    {
    public:
        virtual CARAPI_(void) OnTimeSet(
            /* [in] */ Int32 hourOfDay,
            /* [in] */ Int32 minute,
            /* [in] */ Int32 second,
            /* [in] */ Int32 milli) = 0;
    };

private:
    class NumberFormatter
        : public Object
        , public INumberPickerFormatter
    {
    public:
        NumberFormatter(
            /* [in] */ String format);

        //@Override
        CARAPI Format(
            /* [in] */ Int32 value,
            /* [out] */ String* str);

    private:
        const String mFormat;
    };

public:
    MultiFieldTimePickerDialog(
        /* [in] */ IContext* context,
        /* [in] */ Int32 theme,
        /* [in] */ Int32 hour,
        /* [in] */ Int32 minute,
        /* [in] */ Int32 second,
        /* [in] */ Int32 milli,
        /* [in] */ Int32 min,
        /* [in] */ Int32 max,
        /* [in] */ Int32 step,
        /* [in] */ Boolean is24hourFormat,
        /* [in] */ OnMultiFieldTimeSetListener* listener);

    //@Override
    CARAPI OnClick(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 which);

private:
    CARAPI_(void) NotifyDateSet();

private:
    const AutoPtr<INumberPicker> mHourSpinner;
    const AutoPtr<INumberPicker> mMinuteSpinner;
    const AutoPtr<INumberPicker> mSecSpinner;
    const AutoPtr<INumberPicker> mMilliSpinner;
    const AutoPtr<INumberPicker> mAmPmSpinner;
    const AutoPtr<OnMultiFieldTimeSetListener> mListener;
    const Int32 mStep;
    const Int32 mBaseMilli;
    const Boolean mIs24hourFormat;

    static const Int32 SECOND_IN_MILLIS = 1000;
    static const Int32 MINUTE_IN_MILLIS = 60 * SECOND_IN_MILLIS;
    static const Int32 HOUR_IN_MILLIS = 60 * MINUTE_IN_MILLIS;
}

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_INPUT_MULTIFIELDTIMEPICKERDIALOG_H__
