
#ifndef __ORG_APACHE_HARMONY_SECURITY_X509_CISSUINGDISTRIBUTIONPOINTHELPER_H__
#define __ORG_APACHE_HARMONY_SECURITY_X509_CISSUINGDISTRIBUTIONPOINTHELPER_H__

#include "_CIssuingDistributionPointHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CIssuingDistributionPointHelper)
{
public:
    CARAPI Decode(
        /* [in] */ ArrayOf<Byte> * pEncoding,
        /* [out] */ Org::Apache::Harmony::Security::X509::IIssuingDistributionPoint ** ppObject);

    CARAPI GetASN1(
        /* [out] */ Org::Apache::Harmony::Security::Asn1::IASN1Type ** ppAsn1);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_X509_CISSUINGDISTRIBUTIONPOINTHELPER_H__
