#include "app/CApplicationErrorReportRunningServiceInfo.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace App {

CApplicationErrorReportRunningServiceInfo::CApplicationErrorReportRunningServiceInfo()
    : mDurationMillis(0)
{
}

/**
 * Create an uninitialized instance of RunningServiceInfo.
 */
ECode CApplicationErrorReportRunningServiceInfo::constructor()
{
    return NOERROR;
}

/**
 * Duration in milliseconds that the service has been running.
 */
ECode CApplicationErrorReportRunningServiceInfo::GetDurationMillis(
    /* [out] */ Int64* durationMillis)
{
    VALIDATE_NOT_NULL(durationMillis);
    *durationMillis = mDurationMillis;
    return NOERROR;
}

ECode CApplicationErrorReportRunningServiceInfo::SetDurationMillis(
    /* [in] */ Int64 durationMillis)
{
    mDurationMillis = durationMillis;
    return NOERROR;
}

/**
 * Dump of debug information about the service.
 */
ECode CApplicationErrorReportRunningServiceInfo::GetServiceDetails(
    /* [out] */ String* serviceDetails)
{
    VALIDATE_NOT_NULL(serviceDetails);
    *serviceDetails = mServiceDetails;
    return NOERROR;
}

ECode CApplicationErrorReportRunningServiceInfo::SetServiceDetails(
    /* [in] */ const String& serviceDetails)
{
    mServiceDetails = serviceDetails;
    return NOERROR;
}

/**
 * Dump a RunningService instance to a Printer.
 */
ECode CApplicationErrorReportRunningServiceInfo::Dump(
    /* [in] */ IPrinter* pw,
    /* [in] */ const String& prefix)
{
    VALIDATE_NOT_NULL(pw);

    StringBuilder sb(128);
    sb.Append(prefix);
    sb.Append("durationMillis:");
    sb.Append(mDurationMillis);
    String str = sb.ToString();
    FAIL_RETURN(pw->Println(str));

    sb.Reset();
    sb.Append(prefix);
    sb.Append("serviceDetails:");
    sb.Append(mServiceDetails);
    str = sb.ToString();
    FAIL_RETURN(pw->Println(str));

    return NOERROR;
}

ECode CApplicationErrorReportRunningServiceInfo::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    VALIDATE_NOT_NULL(parcel);

    FAIL_RETURN(parcel->ReadInt64(&mDurationMillis));
    FAIL_RETURN(parcel->ReadString(&mServiceDetails));

    return NOERROR;
}

ECode CApplicationErrorReportRunningServiceInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);

    FAIL_RETURN(dest->WriteInt64(mDurationMillis));
    FAIL_RETURN(dest->WriteString(mServiceDetails));

    return NOERROR;
}


} // namespace App
} // namespace Droid
} // namespace Elastos
