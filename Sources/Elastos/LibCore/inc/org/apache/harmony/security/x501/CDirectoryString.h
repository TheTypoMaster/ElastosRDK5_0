
#ifndef __ORG_APACHE_HARMONY_SECURITY_X501_CDIRECTORYSTRING_H__
#define __ORG_APACHE_HARMONY_SECURITY_X501_CDIRECTORYSTRING_H__

#include "_CDirectoryString.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X501 {

CarClass(CDirectoryString)
{
public:
    CARAPI GetASN1(
        /* [out] */ Org::Apache::Harmony::Security::Asn1::IASN1Choice ** ppAsn1);

    CARAPI SetASN1(
        /* [in] */ Org::Apache::Harmony::Security::Asn1::IASN1Choice * pAsn1);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_X501_CDIRECTORYSTRING_H__
