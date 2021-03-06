
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

void DateDialogNormalizer::SetLimits(
    /* [in] */ IDatePicker* picker,
    /* [in] */ Int64 minMillis,
    /* [in] */ Int64 maxMillis)
{
    // DatePicker intervals are non inclusive, the DatePicker will throw an
    // exception when setting the min/max attribute to the current date
    // so make sure this never happens
    if (maxMillis <= minMillis) {
        return;
    }

    AutoPtr<ICalendar> minCal = TrimToDate(minMillis);
    AutoPtr<ICalendar> maxCal = TrimToDate(maxMillis);
    Int32 currentYear;
    picker->GetYear(&currentYear);
    Int32 currentMonth;
    picker->GetMonth(&currentMonth);
    Int32 currentDayOfMonth;
    picker->GetDayOfMonth(&currentDayOfMonth);
    Int32 year, month, dayOfMonth;
    maxCal->Get(ICalendar::YEAR, &year);
    maxCal->Get(ICalendar::MONTH, &month);
    maxCal->Get(ICalendar::DAY_OF_MONTH, &dayOfMonth);
    picker->UpdateDate(year,
            month,
            dayOfMonth);
    Int64 timeInMillis;
    minCal->GetTimeInMillis(&timeInMillis);
    picker->SetMinDate(timeInMillis);
    Int32 _year, _month, _dayOfMonth;
    minCal->Get(ICalendar::YEAR, _year);
    minCal->Get(ICalendar::MONTH, &_month);
    minCal->Get(ICalendar::DAY_OF_MONTH, _dayOfMonth);
    picker->UpdateDate(_year,
            _month,
            _dayOfMonth);
    Int64 _timeInMillis;
    maxCal->GetTimeInMillis(&_timeInMillis);
    picker->SetMaxDate(_timeInMillis);

    // Restore the current date, this will keep the min/max settings
    // previously set into account.
    picker->UpdateDate(currentYear, currentMonth, currentDayOfMonth);
}

AutoPtr<ICalendar> DateDialogNormalizer::TrimToDate(
    /* [in] */ Int64 time)
{
    AutoPtr<ICalendarHelper> helper;
    CCalendarHelper::AcquireSingleton((ICalendarHelper**)&helper);
    AutoPtr<ICalendar> cal;
    helper->GetInstance(TimeZone::GetTimeZone(String("GMT")), (ICalendar**)&cal);
    cal->Clear();
    cal->SetTimeInMillis(time);
    AutoPtr<ICalendar> result;
    helper->GetInstance(TimeZone::GetTimeZone(String("GMT")), (ICalendar**)&result);
    result->Clear();
    Int32 year, month, dayOfMonth;
    cal->Get(ICalendar::YEAR, &year);
    cal->Get(ICalendar::MONTH, &month);
    cal->Get(ICalendar::DAY_OF_MONTH, &dayOfMonth);
    result->Set(year, month, dayOfMonth,
            0, 0, 0);
    return result;
}

/**
 * Normalizes an existing DateDialogPicker changing the default date if
 * needed to comply with the {@code min} and {@code max} attributes.
 */
void DateDialogNormalizer::Normalize(
    /* [in] */ IDatePicker* picker,
    /* [in] */ IOnDateChangedListener* listener,
    /* [in] */ Int32 year,
    /* [in] */ Int32 month,
    /* [in] */ Int32 day,
    /* [in] */ Int32 hour,
    /* [in] */ Int32 minute,
    /* [in] */ Int64 minMillis,
    /* [in] */ Int64 maxMillis)
{
    AutoPtr<ICalendarHelper> helper;
    CCalendarHelper::AcquireSingleton((ICalendarHelper**)&helper);
    AutoPtr<ICalendar> calendar;
    helper->GetInstance(TimeZone::GetTimeZone(String("GMT")), (ICalendar**)&calendar);
    calendar->Clear();
    calendar->Set(year, month, day, hour, minute, 0);
    Int64 timeInMillis;
    calendar->GetTimeInMillis(&timeInMillis);
    if (timeInMillis < minMillis) {
        calendar->Clear();
        calendar->SetTimeInMillis(minMillis);
    }
    else if (timeInMillis > maxMillis) {
        calendar->Clear();
        calendar->SetTimeInMillis(maxMillis);
    }

    Int32 year, month, dayOfMonth;
    calendar->Get(ICalendar::YEAR, &year);
    calendar->Get(ICalendar::MONTH, &month);
    calendar->Get(ICalendar::DAY_OF_MONTH, &dayOfMonth);
    picker->Init(
            year, month,
            dayOfMonth, listener);

    SetLimits(picker, minMillis, maxMillis);
}

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
