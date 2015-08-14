
#ifndef __ORG_APACHE_HARMONY_SECURITY_X509_CINVALIDITYDATE_H__
#define __ORG_APACHE_HARMONY_SECURITY_X509_CINVALIDITYDATE_H__

#include "_CInvalidityDate.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CInvalidityDate)
{
public:
    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncode);

    CARAPI DumpValue(
        /* [in] */ Elastos::Core::IStringBuilder * pSb,
        /* [in] */ const String& prefix);

    CARAPI DumpValueEx(
        /* [in] */ Elastos::Core::IStringBuilder * pSb);

    CARAPI GetDate(
        /* [out] */ Elastos::Utility::IDate ** ppDate);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte> * pEncoding);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_X509_CINVALIDITYDATE_H__
