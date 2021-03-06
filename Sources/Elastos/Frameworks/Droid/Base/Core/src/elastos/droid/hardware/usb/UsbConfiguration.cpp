
#include "hardware/usb/UsbConfiguration.h"
#include <elastos/core/StringBuffer.h>

using Elastos::Core::StringBuffer;

namespace Elastos {
namespace Droid {
namespace Hardware {
namespace Usb {

CAR_INTERFACE_IMPL_2(UsbConfiguration, Object, IUsbConfiguration, IParcelable);

const Int32 UsbConfiguration::ATTR_SELF_POWERED = 1 << 6;
const Int32 UsbConfiguration::ATTR_REMOTE_WAKEUP = 1 << 5;

ECode UsbConfiguration::constructor()
{
    return NOERROR;
}

ECode UsbConfiguration::constructor(
    /* [in] */ Int32 id,
    /* [in] */ const String& name,
    /* [in] */ Int32 attributes,
    /* [in] */ Int32 maxPower)
{
    mId = id;
    mName = name;
    mAttributes = attributes;
    mMaxPower = maxPower;

    return NOERROR;
}

ECode UsbConfiguration::GetId(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = mId;
    return NOERROR;
}

ECode UsbConfiguration::GetName(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    *str = mName;
    return NOERROR;
}

ECode UsbConfiguration::IsSelfPowered(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);

    *value = (mAttributes & ATTR_SELF_POWERED) != 0;
    return NOERROR;
}

ECode UsbConfiguration::IsRemoteWakeup(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);

    *value = (mAttributes & ATTR_REMOTE_WAKEUP) != 0;
    return NOERROR;
}

ECode UsbConfiguration::GetMaxPower(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = mMaxPower * 2;
    return NOERROR;
}

ECode UsbConfiguration::GetInterfaceCount(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);

    *value = mInterfaces->GetLength();
    return NOERROR;
}

ECode UsbConfiguration::GetInterface(
    /* [in] */ Int32 index,
    /* [out] */ IUsbInterface** outui)
{
    VALIDATE_NOT_NULL(outui);

    *outui = IUsbInterface::Probe((*mInterfaces)[index]);
    REFCOUNT_ADD(*outui);
    return NOERROR;
}

ECode UsbConfiguration::SetInterfaces(
    /* [in] */ ArrayOf<IParcelable*>* interfaces)
{
    mInterfaces = interfaces;
    return NOERROR;
}

ECode UsbConfiguration::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    StringBuffer buf;
    buf += "UsbConfiguration[mId=";
    buf += mId;

    buf += ",mName=";
    buf += mName;

    buf += ",mAttributes=";
    buf += mAttributes;

    buf += ",mMaxPower=";
    buf += mMaxPower;

    buf += ",mInterfaces=[";

    for (Int32 i = 0; i < mInterfaces->GetLength(); i++) {
        buf += "\n";
        String tmp;
        IObject::Probe((*mInterfaces)[i])->ToString(&tmp);
        buf+= tmp;
    }

    buf += "]";

    return buf.ToString(str);
}

ECode UsbConfiguration::DescribeContents(
    /* [out] */ Int32* contents)
{
    VALIDATE_NOT_NULL(contents);
    *contents = 0;

    return NOERROR;
}

ECode UsbConfiguration::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    FAIL_RETURN(source->ReadInt32(&mId))
    FAIL_RETURN(source->ReadString(&mName))
    FAIL_RETURN(source->ReadInt32(&mAttributes))
    FAIL_RETURN(source->ReadInt32(&mMaxPower))
    FAIL_RETURN(source->ReadArrayOf((Handle32*)&mInterfaces))

    return NOERROR;
}

ECode UsbConfiguration::WriteToParcel(
    /* [out] */ IParcel* dest)
{
    VALIDATE_NOT_NULL(dest);

    FAIL_RETURN(dest->WriteInt32(mId))
    FAIL_RETURN(dest->WriteString(mName))
    FAIL_RETURN(dest->WriteInt32(mAttributes))
    FAIL_RETURN(dest->WriteInt32(mMaxPower))
    FAIL_RETURN(dest->WriteArrayOf((Handle32)mInterfaces.Get()))

    return NOERROR;
}

} // namespace Usb
} // namespace Hardware
} // namespace Droid
} // namespace Elastos
