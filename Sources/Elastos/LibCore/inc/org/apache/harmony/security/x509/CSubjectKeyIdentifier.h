
#ifndef __ORG_APACHE_HARMONY_SECURITY_X509_CSUBJECTKEYIDENTIFIER_H__
#define __ORG_APACHE_HARMONY_SECURITY_X509_CSUBJECTKEYIDENTIFIER_H__

#include "_CSubjectKeyIdentifier.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CSubjectKeyIdentifier)
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

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte> * pKeyIdentifier);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_X509_CSUBJECTKEYIDENTIFIER_H__
