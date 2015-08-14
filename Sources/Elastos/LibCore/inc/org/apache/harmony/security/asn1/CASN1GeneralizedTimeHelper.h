
#ifndef __ORG_APACHE_HARMONY_SECURITY_ASN1_CASN1GENERALIZEDTIMEHELPER_H__
#define __ORG_APACHE_HARMONY_SECURITY_ASN1_CASN1GENERALIZEDTIMEHELPER_H__

#include "_CASN1GeneralizedTimeHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

CarClass(CASN1GeneralizedTimeHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IASN1GeneralizedTime** instance);

private:
    // TODO: Add your private member variables here.
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HARMONY_SECURITY_ASN1_CASN1GENERALIZEDTIMEHELPER_H__
