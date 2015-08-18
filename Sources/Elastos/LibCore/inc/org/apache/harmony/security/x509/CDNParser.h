
#ifndef __ORG_APACHE_HARMONY_SECURITY_X509_CDNPARSER_H__
#define __ORG_APACHE_HARMONY_SECURITY_X509_CDNPARSER_H__

#include "_Org_Apache_Harmony_Security_X509_CDNParser.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CDNParser)
{
public:
    CARAPI Parse(
        /* [out] */ Elastos::Utility::IList ** ppList);

    CARAPI constructor(
        /* [in] */ const String& dn);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_X509_CDNPARSER_H__
