
#ifndef __CBLUETOOTHHEALTHAPPCONFIGURATION_H__
#define __CBLUETOOTHHEALTHAPPCONFIGURATION_H__

#include "_CBluetoothHealthAppConfiguration.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {

CarClass(CBluetoothHealthAppConfiguration)
{
public:
    CBluetoothHealthAppConfiguration();

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* equal);

    CARAPI GetHashCode(
        /* [out] */ Int32 *hashCode);

    CARAPI ToString(
        /* [out] */ String* s);

    CARAPI GetDataType(
        /* [out] */ Int32* type);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetRole(
        /* [out] */ Int32* role);

    CARAPI GetChannelType(
        /* [out] */ Int32* type);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ Int32 dataType);

    CARAPI constructor(
        /* [in] */ const String& name,
        /* [in] */ Int32 dataType,
        /* [in] */ Int32 role,
        /* [in] */ Int32 channelType);

private:
    String mName;
    Int32 mDataType;
    Int32 mRole;
    Int32 mChannelType;
};

}
}
}

#endif // __CBLUETOOTHHEALTHAPPCONFIGURATION_H__
