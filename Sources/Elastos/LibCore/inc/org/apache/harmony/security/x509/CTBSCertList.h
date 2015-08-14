
#ifndef __ORG_APACHE_HARMONY_SECURITY_X509_CTBSCERTLIST_H__
#define __ORG_APACHE_HARMONY_SECURITY_X509_CTBSCERTLIST_H__

#include "_CTBSCertList.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CTBSCertList)
{
public:
    CARAPI GetVersion(
        /* [out] */ Int32 * pVer);

    CARAPI GetSignature(
        /* [out] */ Org::Apache::Harmony::Security::X509::IAlgorithmIdentifier ** ppSignature);

    CARAPI GetIssuer(
        /* [out] */ Org::Apache::Harmony::Security::X501::IName ** ppIssuer);

    CARAPI GetThisUpdate(
        /* [out] */ Elastos::Utility::IDate ** ppDate);

    CARAPI GetNextUpdate(
        /* [out] */ Elastos::Utility::IDate ** ppDate);

    CARAPI GetRevokedCertificates(
        /* [out] */ Elastos::Utility::IList ** ppCertificates);

    CARAPI GetCrlExtensions(
        /* [out] */ Org::Apache::Harmony::Security::X509::IExtensions ** ppExtensions);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncoded);

    CARAPI Equals(
        /* [in] */ IInterface * pRc,
        /* [out] */ Boolean * pResult);

    CARAPI GetHashCode(
        /* [out] */ Int32 * pHashCode);

    CARAPI DumpValue(
        /* [in] */ Elastos::Core::IStringBuilder * pSb);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_X509_CTBSCERTLIST_H__
