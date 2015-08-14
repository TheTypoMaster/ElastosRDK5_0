
#ifndef __ORG_APACHE_HARMONY_SECURITY_X509_CNAMECONSTRAINTS_H__
#define __ORG_APACHE_HARMONY_SECURITY_X509_CNAMECONSTRAINTS_H__

#include "_CNameConstraints.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CNameConstraints)
{
public:
    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncode);

    CARAPI DumpValue(
        /* [in] */ Elastos::Core::IStringBuilder * pSb,
        /* [in] */ const String& prefix);

    CARAPI DumpValueEx(
        /* [in] */ Elastos::Core::IStringBuilder * pSb);

    CARAPI IsAcceptable(
        /* [in] */ Elastos::Security::Cert::IX509Certificate * pCert,
        /* [out] */ Boolean * pIsAcceptable);

    CARAPI IsAcceptableEx(
        /* [in] */ Elastos::Utility::IList * pNames,
        /* [out] */ Boolean * pIsAcceptable);

    CARAPI constructor(
        /* [in] */ Org::Apache::Harmony::Security::X509::IGeneralSubtrees * pPermittedSubtrees,
        /* [in] */ Org::Apache::Harmony::Security::X509::IGeneralSubtrees * pExcludedSubtrees);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_X509_CNAMECONSTRAINTS_H__
