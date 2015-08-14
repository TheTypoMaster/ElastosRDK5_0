
#ifndef __ORG_APACHE_HARMONY_SECURITY_X509_CAUTHORITYKEYIDENTIFIER_H__
#define __ORG_APACHE_HARMONY_SECURITY_X509_CAUTHORITYKEYIDENTIFIER_H__

#include "_CAuthorityKeyIdentifier.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CAuthorityKeyIdentifier)
{
public:
    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncode);

    CARAPI DumpValue(
        /* [in] */ Elastos::Core::IStringBuilder * pSb,
        /* [in] */ const String& prefix);

    CARAPI DumpValueEx(
        /* [in] */ Elastos::Core::IStringBuilder * pSb);

    CARAPI GetKeyIdentifier(
        /* [out, callee] */ ArrayOf<Byte> ** ppKeyIdentifier);

    CARAPI GetAuthorityCertIssuer(
        /* [out] */ Org::Apache::Harmony::Security::X509::IGeneralNames ** ppIssuer);

    CARAPI GetAuthorityCertSerialNumber(
        /* [out] */ Elastos::Math::IBigInteger ** ppNumber);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte> * pKeyIdentifier,
        /* [in] */ Org::Apache::Harmony::Security::X509::IGeneralNames * pAuthorityCertIssuer,
        /* [in] */ Elastos::Math::IBigInteger * pAuthorityCertSerialNumber);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_X509_CAUTHORITYKEYIDENTIFIER_H__
