
#ifndef __ORG_APACHE_HARMONY_SECURITY_X509_CTBSCERTIFICATEHELPER_H__
#define __ORG_APACHE_HARMONY_SECURITY_X509_CTBSCERTIFICATEHELPER_H__

#include "_Org_Apache_Harmony_Security_X509_CTBSCertificateHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CTBSCertificateHelper)
{
public:
    CARAPI GetASN1(
        /* [out] */ Org::Apache::Harmony::Security::Asn1::IASN1Sequence ** ppAsn1);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_X509_CTBSCERTIFICATEHELPER_H__
