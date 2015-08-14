
#ifndef __ORG_APACHE_HARMONY_SECURITY_X509_CGENERALNAMES_H__
#define __ORG_APACHE_HARMONY_SECURITY_X509_CGENERALNAMES_H__

#include "_CGeneralNames.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CGeneralNames)
{
public:
    CARAPI GetNames(
        /* [out] */ Elastos::Utility::IList ** ppNames);

    CARAPI GetPairsList(
        /* [out] */ Elastos::Utility::ICollection ** ppPairslist);

    CARAPI AddName(
        /* [in] */ Org::Apache::Harmony::Security::X509::IGeneralName * pName);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncoded);

    CARAPI DumpValue(
        /* [in] */ Elastos::Core::IStringBuilder * pSb,
        /* [in] */ const String& prefix);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Elastos::Utility::IList * pGeneralNames);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_X509_CGENERALNAMES_H__
