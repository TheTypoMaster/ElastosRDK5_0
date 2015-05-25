
#ifndef __CREVOKEDCERTIFICATE_H__
#define __CREVOKEDCERTIFICATE_H__

#include "_CRevokedCertificate.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CRevokedCertificate)
{
public:
    CARAPI GetCrlEntryExtensions(
        /* [out] */ Org::Apache::Harmony::Security::X509::IExtensions ** ppExtensions);

    CARAPI GetUserCertificate(
        /* [out] */ Elastos::Math::IBigInteger ** ppCertificate);

    CARAPI GetRevocationDate(
        /* [out] */ Elastos::Utility::IDate ** ppDate);

    CARAPI GetIssuer(
        /* [out] */ Elastosx::Security::Auth::X500::IX500Principal ** ppIssuer);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncoded);

    CARAPI Equals(
        /* [in] */ IInterface * pRc,
        /* [out] */ Boolean * pResult);

    CARAPI GetHashCode(
        /* [out] */ Int32 * pHashCode);

    CARAPI DumpValue(
        /* [in] */ Elastos::Core::IStringBuilder * pSb,
        /* [in] */ const String& prefix);

    CARAPI constructor(
        /* [in] */ Elastos::Math::IBigInteger * pUserCertificate,
        /* [in] */ Elastos::Utility::IDate * pRevocationDate,
        /* [in] */ Org::Apache::Harmony::Security::X509::IExtensions * pCrlEntryExtensions);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CREVOKEDCERTIFICATE_H__
