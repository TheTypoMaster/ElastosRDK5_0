
#include "CellInfo.h"
#include <elastos/StringBuilder.h>
#include <elastos/Math.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::Math;

namespace Elastos {
namespace Droid {
namespace Telephony {

const Int32 CellInfo::TYPE_GSM = 1;
const Int32 CellInfo::TYPE_CDMA = 2;
const Int32 CellInfo::TYPE_LTE = 3;

CellInfo::CellInfo()
    : mRegistered(FALSE)
    , mTimeStamp(Elastos::Core::Math::INT64_MAX_VALUE)
    , mTimeStampType(ICellInfo::TIMESTAMP_TYPE_UNKNOWN)
{}

ECode CellInfo::Init(
    /* [in] */ ICellInfo* ci)
{
    Boolean isRegistered;
    Int64 timeStamp;
    Int32 timeStampType;
    ci->IsRegistered(&isRegistered);
    ci->GetTimeStamp(&timeStamp);
    ci->GetTimeStampType(&timeStampType);
    mRegistered = isRegistered;
    mTimeStampType = timeStampType;
    mTimeStamp = timeStamp;
    return NOERROR;
}

ECode CellInfo::IsRegistered(
    /* [out] */ Boolean* res)
{
    *res = mRegistered;
    return NOERROR;
}

ECode CellInfo::SetRegisterd(
    /* [in] */ Boolean registered)
{
    mRegistered = registered;
    return NOERROR;
}

ECode CellInfo::GetTimeStamp(
    /* [out] */ Int64* timeStamp)
{
    *timeStamp = mTimeStamp;
    return NOERROR;
}

ECode CellInfo::SetTimeStamp(
    /* [in] */ Int64 timeStamp)
{
    mTimeStamp = timeStamp;
    return NOERROR;
}

ECode CellInfo::GetTimeStampType(
    /* [out] */ Int32* timeStampType)
{
    *timeStampType = mTimeStampType;
    return NOERROR;
}

ECode CellInfo::SetTimeStampType(
    /* [in] */ Int32 timeStampType)
{
    if (timeStampType < ICellInfo::TIMESTAMP_TYPE_UNKNOWN || timeStampType > ICellInfo::TIMESTAMP_TYPE_JAVA_RIL) {
        mTimeStampType = ICellInfo::TIMESTAMP_TYPE_UNKNOWN;
    }
    else {
        mTimeStampType = timeStampType;
    }
    return NOERROR;
}

ECode CellInfo::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    Int32 primeNum = 31;
    *hashCode = ((mRegistered ? 0 : 1) * primeNum) + ((Int32)(mTimeStamp / 1000) * primeNum)
            + (mTimeStampType * primeNum);
    return NOERROR;
}

ECode CellInfo::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* result)
{
    if (other == NULL) {
        *result = FALSE;
        return NOERROR;
    }
    if ((ICellInfo*)this->Probe(EIID_ICellInfo) == ICellInfo::Probe(other)) {
        *result = TRUE;
        return NOERROR;
    }
    AutoPtr<ICellInfo> o = ICellInfo::Probe(other);
    if (o != NULL) {
        Boolean isRegistered;
        Int64 timeStamp;
        Int32 timeStampType;
        o->IsRegistered(&isRegistered);
        o->GetTimeStamp(&timeStamp);
        o->GetTimeStampType(&timeStampType);
        *result = (mRegistered == isRegistered && mTimeStamp == timeStamp && mTimeStampType == timeStampType);
    }
    else *result = FALSE;
    return NOERROR;
}

String CellInfo::TimeStampTypeToString(
    /* [in] */ Int32 type)
{
    switch (type) {
        case 1:
            return String("antenna");
        case 2:
            return String("modem");
        case 3:
            return String("oem_ril");
        case 4:
            return String("java_ril");
        default:
            return String("unknown");
    }
}

ECode CellInfo::ToString(
    /* [out] */ String* str)
{
    StringBuilder sb;
    String timeStampType;

    sb.AppendCStr(" mRegistered=");
    sb.AppendCStr(mRegistered ? "YES" : "NO");
    timeStampType = TimeStampTypeToString(mTimeStampType);
    sb.AppendCStr(" mTimeStampType=");
    sb.AppendString(timeStampType);
    sb.AppendCStr(" mTimeStamp=");
    sb.AppendInt64(mTimeStamp);
    sb.AppendCStr("ns");
    *str = sb.ToString();
    return NOERROR;
}

ECode CellInfo::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadBoolean(&mRegistered);
    source->ReadInt32(&mTimeStampType);
    source->ReadInt64(&mTimeStamp);
    return NOERROR;
}

ECode CellInfo::WriteToParcel(
    /* [in] */ IParcel* dest,
    /* [in] */ Int32 type)
{
    dest->WriteInt32(type);
    dest->WriteBoolean(mRegistered);
    dest->WriteInt32(mTimeStampType);
    dest->WriteInt64(mTimeStamp);
    return NOERROR;
}

} // namespace Telephony
} // namespace Droid
} // namespace Elastos
