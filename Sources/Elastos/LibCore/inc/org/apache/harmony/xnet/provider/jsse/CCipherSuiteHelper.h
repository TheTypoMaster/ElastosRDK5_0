
#ifndef __ORG_APACHE_HARMONY_XNET_PROVIDER_JSSE_CCIPHERSUITEHELPER_H__
#define __ORG_APACHE_HARMONY_XNET_PROVIDER_JSSE_CCIPHERSUITEHELPER_H__

#include "_CCipherSuiteHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CCipherSuiteHelper)
{
public:
    CARAPI GetByName(
        /* [in] */ const String& name,
        /* [out] */ Org::Apache::Harmony::Xnet::Provider::Jsse::ICipherSuite ** ppSuite);

    CARAPI GetByCode(
        /* [in] */ Byte b1,
        /* [in] */ Byte b2,
        /* [out] */ Org::Apache::Harmony::Xnet::Provider::Jsse::ICipherSuite ** ppSuite);

    CARAPI GetByCodeEx(
        /* [in] */ Byte b1,
        /* [in] */ Byte b2,
        /* [in] */ Byte b3,
        /* [out] */ Org::Apache::Harmony::Xnet::Provider::Jsse::ICipherSuite ** ppSuite);

    CARAPI GetSupported(
        /* [out, callee] */ ArrayOf<Org::Apache::Harmony::Xnet::Provider::Jsse::ICipherSuite *> ** ppSupported);

    CARAPI GetSupportedCipherSuiteNames(
        /* [out, callee] */ ArrayOf<String> ** ppNames);

    CARAPI GetClientKeyType(
        /* [in] */ Byte keyType,
        /* [out] */ String * pType);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_XNET_PROVIDER_JSSE_CCIPHERSUITEHELPER_H__
