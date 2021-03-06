
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Input {

WeekPicker::WeekPicker(
    /* [in] */ IContext* context,
    /* [in] */ Double minValue,
    /* [in] */ Double maxValue)
    : TwoFieldDatePicker(context, minValue, maxValue)
{
    GetPositionInYearSpinner()->SetContentDescription(
            GetResources()->GetString(R::string::accessibility_date_picker_week));

    // initialize to current date
    AutoPtr<ICalendarHelper> helper;
    CCalendarHelper::AcquireSingleton((ICalendarHelper**)&helper);
    AutoPtr<ICalendar> cal;
    helper->GetInstance(TimeZone::GetTimeZone(String("UTC")), (ICalendar**)&cal);
    cal->SetFirstDayOfWeek(ICalendar::MONDAY);
    cal->SetMinimalDaysInFirstWeek(4);
    cal->SetTimeInMillis(System::CurrentTimeMillis());
    Init(GetISOWeekYearForDate(cal), GetWeekForDate(cal), NULL);
}

/**
 * Creates a date object from the |year| and |week|.
 */
AutoPtr<ICalendar> WeekPicker::CreateDateFromWeek(
    /* [in] */ Int32 year,
    /* [in] */ Int32 week)
{
    AutoPtr<ICalendarHelper> helper;
    CCalendarHelper::AcquireSingleton((ICalendarHelper**)&helper);
    AutoPtr<ICalendar> date;
    helper->GetInstance(TimeZone::GetTimeZone(String("UTC")), (ICalendar**)&date);
    date->Clear();
    date->SetFirstDayOfWeek(ICalendar::MONDAY);
    date->SetMinimalDaysInFirstWeek(4);
    date->Set(ICalendar::DAY_OF_WEEK, ICalendar::MONDAY);
    date->Set(ICalendar::YEAR, year);
    date->Set(ICalendar::WEEK_OF_YEAR, week);
    return date;
}

/**
 * Creates a date object from the |value| which is milliseconds since epoch.
 */
AutoPtr<ICalendar> WeekPicker::CreateDateFromValue(
    /* [in] */ Double value)
{
    AutoPtr<ICalendarHelper> helper;
    CCalendarHelper::AcquireSingleton((ICalendarHelper**)&helper);
    AutoPtr<ICalendar> date;
    helper->GetInstance(TimeZone::GetTimeZone(String("UTC")), (ICalendar**)&date);
    date->Clear();
    date->SetFirstDayOfWeek(Calendar.MONDAY);
    date->SetMinimalDaysInFirstWeek(4);
    date->SetTimeInMillis((long) value);
    return date;
}

//@Override
AutoPtr<ICalendar> WeekPicker::GetDateForValue(
    /* [in] */ Double value)
{
    return WeekPicker::CreateDateFromValue(value);
}

Int32 WeekPicker::GetISOWeekYearForDate(
    /* [in] */ ICalendar* date)
{
    Int32 year;
    date->Get(ICalendar::YEAR, &year);
    Int32 month;
    date->Get(ICalendar::MONTH, &month);
    Int32 week;
    date->Get(ICalendar::WEEK_OF_YEAR, &week);
    if (month == 0 && week > 51) {
        year--;
    }
    else if (month == 11 && week == 1) {
        year++;
    }

    return year;
}

Int32 WeekPicker::GetWeekForDate(
    /* [in] */ ICalendar* date)
{
    Int32 weekOfYear;
    date->Get(ICalendar::WEEK_OF_YEAR, &weekOfYear);
    return weekOfYear;
}

//@Override
void WeekPicker::SetCurrentDate(
    /* [in] */ Int32 year,
    /* [in] */ Int32 week)
{
    AutoPtr<ICalendar> date = CreateDateFromWeek(year, week);
    Boolean bBefore, bAfter;
    date->Before(GetMinDate(), &bBefore);
    if (bBefore) {
        SetCurrentDate(GetMinDate());
    }
    else if (date->After(GetMaxDate(), &bAfter), bAfter) {
        SetCurrentDate(GetMaxDate());
    }
    else {
        SetCurrentDate(date);
    }
}

Int32 WeekPicker::GetNumberOfWeeks(
    /* [in] */ Int32 year)
{
    // Create a date in the middle of the year, where the week year matches the year.
    AutoPtr<ICalendar> date = CreateDateFromWeek(year, 20);
    Int32 weekOfYear;
    date->GetActualMaximum(ICalendar::WEEK_OF_YEAR, &weekOfYear);
    return weekOfYear;
}

/**
 * @return The selected year.
 */
//@Override
Int32 WeekPicker::GetYear()
{
    return GetISOWeekYearForDate(GetCurrentDate());
}

/**
 * @return The selected week.
 */
Int32 WeekPicker::GetWeek()
{
    return GetWeekForDate(GetCurrentDate());
}

//@Override
Int32 WeekPicker::GetPositionInYear()
{
    return GetWeek();
}

//@Override
Int32 WeekPicker::GetMaxYear()
{
    return GetISOWeekYearForDate(GetMaxDate());
}

//@Override
Int32 WeekPicker::GetMinYear()
{
    return GetISOWeekYearForDate(GetMinDate());
}

//@Override
Int32 WeekPicker::GetMaxPositionInYear(
    /* [in] */ Int32 year)
{
    if (year == GetISOWeekYearForDate(GetMaxDate())) {
        return GetWeekForDate(GetMaxDate());
    }

    return GetNumberOfWeeks(year);
}

//@Override
Int32 WeekPicker::GetMinPositionInYear(
    /* [in] */ Int32 year)
{
    if (year == GetISOWeekYearForDate(GetMinDate())) {
        return GetWeekForDate(GetMinDate());
    }

    return 1;
}

} // namespace Input
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
