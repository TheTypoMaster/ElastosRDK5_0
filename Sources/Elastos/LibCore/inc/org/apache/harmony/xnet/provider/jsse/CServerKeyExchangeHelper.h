
#ifndef __ORG_APACHE_HARMONY_XNET_PROVIDER_JSSE_CSERVERKEYEXCHANGEHELPER_H__
#define __ORG_APACHE_HARMONY_XNET_PROVIDER_JSSE_CSERVERKEYEXCHANGEHELPER_H__

#include "_CServerKeyExchangeHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CServerKeyExchangeHelper)
{
public:
    CARAPI ToUnsignedByteArray(
        /* [in] */ Elastos::Math::IBigInteger * pBi,
        /* [out, callee] */ ArrayOf<Byte> ** ppByteArray);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_XNET_PROVIDER_JSSE_CSERVERKEYEXCHANGEHELPER_H__
