#ifndef __ELASTOS_NET_CINET6ADDRESSHELPER_H__
#define __ELASTOS_NET_CINET6ADDRESSHELPER_H__

#include "_Elastos_Net_CInet6AddressHelper.h"
#include "InetAddress.h"
#include "Singleton.h"


namespace Elastos {
namespace Net {

CarClass(CInet6AddressHelper)
    , public Singleton
    , public IInet6AddressHelper
{
    CAR_SINGLETON_DECL()

    CAR_INTERFACE_DECL()

    CARAPI GetByAddress(
        /* [in] */ const String& host,
        /* [in] */ ArrayOf<Byte>* addr,
        /* [in] */ Int32 scope_id,
        /* [out] */ IInet6Address** address);

    CARAPI GetByAddress(
        /* [in] */ const String& host,
        /* [in] */ ArrayOf<Byte>* addr,
        /* [in] */ INetworkInterface* nif,
        /* [out] */ IInet6Address** address);
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_CINET6ADDRESSHELPER_H__
