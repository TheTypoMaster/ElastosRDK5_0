
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

/**
 * @param context The context the dialog is to run in.
 * @param callBack How the parent is notified that the date is set.
 * @param year The initial year of the dialog.
 * @param monthOfYear The initial month of the dialog.
 * @param dayOfMonth The initial day of the dialog.
 */
DateTimePickerDialog::DateTimePickerDialog(
    /* [in] */ IContext* context,
    /* [in] */ IOnDateTimeSetListener* callBack,
    /* [in] */ Int32 year,
    /* [in] */ Int32 monthOfYear,
    /* [in] */ Int32 dayOfMonth,
    /* [in] */ Int32 hourOfDay,
    /* [in] */ Int32 minute,
    /* [in] */ Boolean is24HourView,
    /* [in] */ Double min,
    /* [in] */ Double max)
    : AlertDialog(context, 0)
    , mMinTimeMillis((Int64)min)
    , mMaxTimeMillis((Int64)max)
    , mCallBack(callBack)
{
    AutoPtr<ICharSequence> setCS;
    context->GetText(
            R::string::date_picker_dialog_set, (ICharSequence**)&setCS);
    SetButton(BUTTON_POSITIVE, setCS, this);
    AutoPtr<ICharSequence> cancelCS;
    context->GetText(android::R::string::cancel, (ICharSequence**)&cancelCS);
    SetButton(BUTTON_NEGATIVE, cancelCS,
            NULL);
    SetIcon(0);
    AutoPtr<ICharSequence> titleCS;
    context->GetText(R::string::date_time_picker_dialog_title, (ICharSequence**)&titleCS);
    SetTitle(titleCS);

    AutoPtr<ILayoutInflater> inflater;
    context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (ILayoutInflater**)&inflater);
    AutoPtr<IView> view;
    inflater->Inflate(R::layout::date_time_picker_dialog, NULL, (IView**)&view);
    SetView(view);
    view->FindViewById(R::id::date_picker, (IView**)&mDatePicker);
    DateDialogNormalizer::Normalize(mDatePicker, this,
            year, monthOfYear, dayOfMonth, hourOfDay, minute, mMinTimeMillis, mMaxTimeMillis);

    view->FindViewById(R::id::time_picker, (IView**)&mTimePicker);
    mTimePicker->SetIs24HourView(is24HourView);
    mTimePicker->SetCurrentHour(hourOfDay);
    mTimePicker->SetCurrentMinute(minute);
    mTimePicker->SetOnTimeChangedListener(this);
    Int32 currentHour, currentMinute;
    mTimePicker->GetCurrentHour(&currentHour);
    mTimePicker->GetCurrentMinute(&currentMinute);
    OnTimeChanged(mTimePicker, currentHour,
            currentMinute);
}

//@Override
ECode DateTimePickerDialog::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which)
{
    TryNotifyDateTimeSet();
    return NOERROR;
}

ECode DateTimePickerDialog::TryNotifyDateTimeSet()
{
    if (mCallBack != NULL) {
        mDatePicker->ClearFocus();
        Int32 year, month, dayOfMonth, currentHour, currentMinute;
        mDatePicker->GetYear(&year);
        mDatePicker->GetMonth(&month);
        mDatePicker->GetDayOfMonth(&dayOfMonth);
        mTimePicker->GetCurrentHour(&hour);
        mTimePicker->GetCurrentMinute(&minute);
        mCallBack->OnDateTimeSet(mDatePicker, mTimePicker, year,
                month, dayOfMonth, currentHour, currentMinute);
    }

    return NOERROR;
}

//@Override
ECode DateTimePickerDialog::OnDateChanged(
    /* [in] */ IDatePicker* view,
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day)
{
    // Signal a time change so the max/min checks can be applied.
    if (mTimePicker != null) {
        onTimeChanged(mTimePicker, mTimePicker.getCurrentHour(),
                mTimePicker.getCurrentMinute());
    }

    return NOERROR;
}

//@Override
void DateTimePickerDialog::OnTimeChanged(
    /* [in] */ ITimePicker* view,
    /* [in] */ Int32 hourOfDay,
    /* [in] */ Int32 minute)
{
    AutoPtr<ITime> time;
    CTime::New((ITime**)&time);
    Int32 currentMinute, currentHour, dayOfMonth, month, year;
    mTimePicker->GetCurrentMinute(&currentMinute);
    mTimePicker->GetCurrentHour(&hour);
    mDatePicker->GetDayOfMonth(&dayOfMonth);
    mDatePicker->GetMonth(&month);
    mDatePicker->GetYear(&year);
    time->Set(0, currentMinute,
            currentHour, dayOfMonth,
            month, year);

    Int64 millis;
    time->ToMillis(TRUE, &millis);
    if (millis < mMinTimeMillis) {
        time->Set(mMinTimeMillis);
    }
    else if (millis > mMaxTimeMillis) {
        time->Set(mMaxTimeMillis);
    }

    Int32 tHour, tMinute;
    time->GetHour(&tHour);
    time->GetMinute(&tMinute);
    mTimePicker->SetCurrentHour(tHour);
    mTimePicker->SetCurrentMinute(tMinute);
}

/**
 * Sets the current date.
 *
 * @param year The date year.
 * @param monthOfYear The date month.
 * @param dayOfMonth The date day of month.
 */
void DateTimePickerDialog::UpdateDateTime(
    /* [in] */ Int32 year,
    /* [in] */ Int32 monthOfYear,
    /* [in] */ Int32 dayOfMonth,
    /* [in] */ Int32 hourOfDay,
    /* [in] */ Int32 minutOfHour)
{
    mDatePicker->UpdateDate(year, monthOfYear, dayOfMonth);
    mTimePicker->SetCurrentHour(hourOfDay);
    mTimePicker->SetCurrentMinute(minutOfHour);
}

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
