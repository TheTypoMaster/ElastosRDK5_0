
#ifndef __ELASTOS_NET_CINETSOCKETADDRESSHELPER_H__
#define __ELASTOS_NET_CINETSOCKETADDRESSHELPER_H__

#include "_ELASTOS_NET_CInetSocketAddressHelper.h"
#include "Singleton.h"

namespace Elastos {
namespace Net {

CarClass(CInetSocketAddressHelper)
    , public Singleton
    , public IInetSocketAddressHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI CreateUnresolved(
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [out] */ IInetSocketAddress** addr);

};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_CINETSOCKETADDRESSHELPER_H__
