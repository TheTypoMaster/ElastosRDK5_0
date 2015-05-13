
#ifndef __CSUBJECTPUBLICKEYINFO_H__
#define __CSUBJECTPUBLICKEYINFO_H__

#include "_CSubjectPublicKeyInfo.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CSubjectPublicKeyInfo)
{
public:
    CARAPI GetAlgorithmIdentifier(
        /* [out] */ Org::Apache::Harmony::Security::X509::IAlgorithmIdentifier ** ppAlgorithmIdentifier);

    CARAPI GetSubjectPublicKey(
        /* [out, callee] */ ArrayOf<Byte> ** ppPublicKey);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncoded);

    CARAPI GetPublicKey(
        /* [out] */ Elastos::Security::IPublicKey ** ppPublicKey);

    CARAPI constructor(
        /* [in] */ Org::Apache::Harmony::Security::X509::IAlgorithmIdentifier * pAlgID,
        /* [in] */ ArrayOf<Byte> * pSubjectPublicKey);

    CARAPI constructor(
        /* [in] */ Org::Apache::Harmony::Security::X509::IAlgorithmIdentifier * pAlgID,
        /* [in] */ ArrayOf<Byte> * pSubjectPublicKey,
        /* [in] */ Int32 unused);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CSUBJECTPUBLICKEYINFO_H__
