
#ifndef __CALGORITHMIDENTIFIER_H__
#define __CALGORITHMIDENTIFIER_H__

#include "_CAlgorithmIdentifier.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {

CarClass(CAlgorithmIdentifier)
{
public:
    CARAPI GetAlgorithm(
        /* [out] */ String * pAlgorithm);

    CARAPI GetAlgorithmName(
        /* [out] */ String * pAlgorithmName);

    CARAPI GetParameters(
        /* [out, callee] */ ArrayOf<Byte> ** ppParameters);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte> ** ppEncoded);

    CARAPI Equals(
        /* [in] */ IInterface * pAi,
        /* [out] */ Boolean * pResult);

    CARAPI GetHashCode(
        /* [out] */ Int32 * pResult);

    CARAPI DumpValue(
        /* [in] */ Elastos::Core::IStringBuilder * pSb);

    CARAPI constructor(
        /* [in] */ const String& algorithm);

    CARAPI constructor(
        /* [in] */ const String& algorithm,
        /* [in] */ ArrayOf<Byte> * pParameters);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __CALGORITHMIDENTIFIER_H__
