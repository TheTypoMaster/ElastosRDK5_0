
#ifndef __ORG_APACHE_HARMONY_SECURITY_X509_CDISTRIBUTIONPOINTHELPER_H__
#define __ORG_APACHE_HARMONY_SECURITY_X509_CDISTRIBUTIONPOINTHELPER_H__

#include "_CDistributionPointHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CDistributionPointHelper)
{
public:
    CARAPI GetASN1(
        /* [out] */ Org::Apache::Harmony::Security::Asn1::IASN1Sequence ** ppAsn1);

    CARAPI SetASN1(
        /* [in] */ Org::Apache::Harmony::Security::Asn1::IASN1Sequence * pAsn1);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_X509_CDISTRIBUTIONPOINTHELPER_H__
