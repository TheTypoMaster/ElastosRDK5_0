
#ifndef __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICERESPONSE_H__
#define __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICERESPONSE_H__

#include "_Elastos_Droid_Net_Wifi_P2p_Nsd_CWifiP2pServiceResponse.h"
#include "WifiP2pServiceResponse.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Wifi {
namespace P2p {
namespace Nsd {

CarClass(CWifiP2pServiceResponse), public WifiP2pServiceResponse
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 serviceType,
        /* [in] */ Int32 status,
        /* [in] */ Int32 transId,
        /* [in] */ IWifiP2pDevice* device,
        /* [in] */ ArrayOf<Byte>* data);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI GetServiceType(
        /* [out] */ Int32* serviceType);

    CARAPI GetStatus(
        /* [out] */ Int32* status);

    CARAPI GetTransactionId(
        /* [out] */ Int32 * pTransactionId);

    CARAPI GetRawData(
        /* [out, callee] */ ArrayOf<Byte>** rawData);

    CARAPI GetSrcDevice(
        /* [out] */ IWifiP2pDevice** srcDevice);

    CARAPI SetSrcDevice(
        /* [in] */ IWifiP2pDevice* dev);

    CARAPI ToString(
        /* [out] */ String* string);

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* isEqual);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    static CARAPI NewInstance(
        /* [in] */ const String& supplicantEvent,
        /* [out, callee] */ ArrayOf<IWifiP2pServiceResponse*>** list);
};

}
}
}
}
}
}
#endif // __ELASTOS_DROID_NET_WIFI_P2P_NSD_CWIFIP2PSERVICERESPONSE_H__
