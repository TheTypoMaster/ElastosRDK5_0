
#ifndef __ORG_APACHE_HARMONY_XNET_PROVIDER_JSSE_CHELLOREQUEST_H__
#define __ORG_APACHE_HARMONY_XNET_PROVIDER_JSSE_CHELLOREQUEST_H__

#include "_CHelloRequest.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CHelloRequest)
{
public:
    CARAPI GetLength(
        /* [out] */ Int32 * pLength);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Org::Apache::Harmony::Xnet::Provider::Jsse::IHandshakeIODataStream * pIds,
        /* [in] */ Int32 length);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_XNET_PROVIDER_JSSE_CHELLOREQUEST_H__
