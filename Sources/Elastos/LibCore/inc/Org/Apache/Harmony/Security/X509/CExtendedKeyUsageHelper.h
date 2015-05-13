
#ifndef __CEXTENDEDKEYUSAGEHELPER_H__
#define __CEXTENDEDKEYUSAGEHELPER_H__

#include "_CExtendedKeyUsageHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CExtendedKeyUsageHelper)
{
public:
    CARAPI GetASN1(
        /* [out] */ Org::Apache::Harmony::Security::Asn1::IASN1Type ** ppAsn1);

    CARAPI SetASN1(
        /* [in] */ Org::Apache::Harmony::Security::Asn1::IASN1Type * pAsn1);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CEXTENDEDKEYUSAGEHELPER_H__
