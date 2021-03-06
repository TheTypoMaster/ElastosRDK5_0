
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

//==================================================================
//   TwoFieldDatePicker::InnerNumberPickerOnValueChangeListener
//==================================================================

TwoFieldDatePicker::InnerNumberPickerOnValueChangeListener::InnerNumberPickerOnValueChangeListener(
    /* [in] */ TwoFieldDatePicker* owner)
    : mOwner(owner)
{
}

ECode TwoFieldDatePicker::InnerNumberPickerOnValueChangeListener::OnValueChange(
    /* [in] */ INumberPicker* picker,
    /* [in] */ Int32 oldVal,
    /* [in] */ Int32 newVal)
{
    VALIDATE_NOT_NULL(picker);

    Int32 year = mOwner->GetYear();
    Int32 positionInYear = mOwner->GetPositionInYear();
    // take care of wrapping of days and months to update greater fields
    if (picker == mOwner->mPositionInYearSpinner) {
        positionInYear = newVal;
        Int32 maxValue, minValue;
        picker->GetMaxValue(&maxValue);
        picker->GetMinValue(&minValue);
        if (oldVal == maxValue && newVal == minValue) {
            year += 1;
            positionInYear = mOwner->GetMinPositionInYear(year);
        }
        else if (oldVal == minValue && newVal == maxValue) {
            year -= 1;
            positionInYear = mOwner->GetMaxPositionInYear(year);
        }
    }
    else if (picker == mOwner->mYearSpinner) {
        year = newVal;
    }
    else {
        //throw new IllegalArgumentException();
        assert(0);
    }

    // now set the date to the adjusted one
    mOwner->SetCurrentDate(year, positionInYear);
    mOwner->UpdateSpinners();
    mOwner->NotifyDateChanged();

    return NOERROR;
}

//==================================================================
//                       TwoFieldDatePicker
//==================================================================

TwoFieldDatePicker::TwoFieldDatePicker(
    /* [in] */ IContext* context,
    /* [in] */ Double minValue,
    /* [in] */ Double maxValue)
{
    super(context, NULL, android::R::attr::datePickerStyle);

    AutoPtr<ILayoutInflater> inflater;
    context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICEj, (IInterface**)&inflater);
    inflater->Inflate(R::layout::two_field_date_picker, this, TRUE);

    AutoPtr<INumberPickerOnValueChangeListener> onChangeListener = new InnerNumberPickerOnValueChangeListener(this);

    AutoPtr<ICalendarHelper> helper;
    CCalendarHelper::AcquireSingleton((ICalendarHelper**)&helper);
    mCurrentDate
    helper->GetInstance(TimeZone::GetTimeZone(String("UTC")), (ICalendar**)&mCurrentDate);
    if (minValue >= maxValue) {
        helper->GetInstance(TimeZone::GetTimeZone(String("UTC")), (ICalendar**)&mMinDate);
        mMinDate->Set(0, 0, 1);
        helper->GetInstance(TimeZone::GetTimeZone(String("UTC")), (ICalendar**)&mMaxDate);
        mMaxDate->Set(9999, 0, 1);
    }
    else {
        mMinDate = GetDateForValue(minValue);
        mMaxDate = GetDateForValue(maxValue);
    }

    // month
    FindViewById(R::id::position_in_year, (IView**)&mPositionInYearSpinner);
    mPositionInYearSpinner->SetOnLongPressUpdateInterval(200);
    mPositionInYearSpinner->SetOnValueChangedListener(onChangeListener);

    // year
    FindViewById(R::id::year, (IView**)&mYearSpinner);
    mYearSpinner->SetOnLongPressUpdateInterval(100);
    mYearSpinner->SetOnValueChangedListener(onChangeListener);
}

/**
 * Initialize the state. If the provided values designate an inconsistent
 * date the values are normalized before updating the spinners.
 *
 * @param year The initial year.
 * @param positionInYear The initial month <strong>starting from zero</strong> or week in year.
 * @param onMonthOrWeekChangedListener How user is notified date is changed by
 *            user, can be null.
 */
void TwoFieldDatePicker::Init(
    /* [in] */ Int32 year,
    /* [in] */ Int32 positionInYear,
    /* [in] */ OnMonthOrWeekChangedListener* onMonthOrWeekChangedListener)
{
    SetCurrentDate(year, positionInYear);
    UpdateSpinners();
    mMonthOrWeekChangedListener = onMonthOrWeekChangedListener;
}

Boolean TwoFieldDatePicker::IsNewDate(
    /* [in] */ Int32 year,
    /* [in] */ Int32 positionInYear)
{
    return (GetYear() != year || GetPositionInYear() != positionInYear);
}


/**
 * Updates the current date.
 *
 * @param year The year.
 * @param positionInYear The month or week in year.
 */
void TwoFieldDatePicker::UpdateDate(
    /* [in] */ Int32 year,
    /* [in] */ Int32 positionInYear)
{
    if (!IsNewDate(year, positionInYear)) {
        return;
    }

    SetCurrentDate(year, positionInYear);
    UpdateSpinners();
    NotifyDateChanged();
}

void TwoFieldDatePicker::SetCurrentDate(
    /* [in] */ ICalendar* date)
{
    mCurrentDate = date;
}

//@Override
ECode TwoFieldDatePicker::DispatchPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    OnPopulateAccessibilityEvent(event);
    *result = TRUE;
    return NOERROR;
}

//@Override
ECode TwoFieldDatePicker::OnPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    super.onPopulateAccessibilityEvent(event);

    const Int32 flags = DateUtils::FORMAT_SHOW_DATE | DateUtils::FORMAT_SHOW_YEAR;
    AutoPtr<IDateUtils> dateUtils;
    CDateUtils::AcquireSingleton((IDateUtils**)&dateUtils);
    Int64 millis;
    mCurrentDate->GetTimeInMillis(&millis);
    String selectedDateUtterance;
    dateUtils->FormatDateTime(GetContext(),
            millis, flags);
    AutoPtr<IObjectContainer> text;
    event->GetText((IObjectContainer**)&text);
    text->Add(selectedDateUtterance);

    return NOERROR;
}

/**
 * @return The selected year.
 */
Int32 TwoFieldDatePicker::GetYear()
{
    Int32 year;
    mCurrentDate->Get(ICalendar::YEAR, &year);
    return year;
}

AutoPtr<ICalendar> TwoFieldDatePicker::GetMaxDate()
{
    return mMaxDate;
}

AutoPtr<ICalendar> TwoFieldDatePicker::GetMinDate()
{
    return mMinDate;
}

AutoPtr<ICalendar> TwoFieldDatePicker::GetCurrentDate()
{
    return mCurrentDate;
}

AutoPtr<INumberPicker> TwoFieldDatePicker::GetPositionInYearSpinner()
{
    return mPositionInYearSpinner;
}

AutoPtr<INumberPicker> TwoFieldDatePicker::GetYearSpinner()
{
    return mYearSpinner;
}

/**
 * This method should be subclassed to update the spinners based on mCurrentDate.
 */
void TwoFieldDatePicker::UpdateSpinners()
{
    mPositionInYearSpinner->SetDisplayedValues(NULL);

    // set the spinner ranges respecting the min and max dates
    mPositionInYearSpinner->SetMinValue(GetMinPositionInYear(GetYear()));
    mPositionInYearSpinner->SetMaxValue(GetMaxPositionInYear(GetYear()));
    Boolean bMinDate, bMaxDate;
    mCurrentDate->Equals(mMinDate, &bMinDate);
    mCurrentDate->Equals(mMaxDate, &maxValue);
    mPositionInYearSpinner->SetWrapSelectorWheel(
            !bMinDate && !maxValue);

    // year spinner range does not change based on the current date
    mYearSpinner->SetMinValue(GetMinYear());
    mYearSpinner->SetMaxValue(GetMaxYear());
    mYearSpinner->SetWrapSelectorWheel(FALSE);

    // set the spinner values
    mYearSpinner->SetValue(GetYear());
    mPositionInYearSpinner->SetValue(GetPositionInYear());
}

/**
 * Notifies the listener, if such, for a change in the selected date.
 */
void TwoFieldDatePicker::NotifyDateChanged()
{
    SendAccessibilityEvent(IAccessibilityEvent::TYPE_VIEW_SELECTED);
    if (mMonthOrWeekChangedListener != NULL) {
        mMonthOrWeekChangedListener->OnMonthOrWeekChanged(this, GetYear(), GetPositionInYear());
    }
}

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
