
#ifndef __ORG_APACHE_HARMONY_SECURITY_X509_CBASICCONSTRAINTSHELPER_H__
#define __ORG_APACHE_HARMONY_SECURITY_X509_CBASICCONSTRAINTSHELPER_H__

#include "_CBasicConstraintsHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CBasicConstraintsHelper)
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

#endif // __ORG_APACHE_HARMONY_SECURITY_X509_CBASICCONSTRAINTSHELPER_H__
