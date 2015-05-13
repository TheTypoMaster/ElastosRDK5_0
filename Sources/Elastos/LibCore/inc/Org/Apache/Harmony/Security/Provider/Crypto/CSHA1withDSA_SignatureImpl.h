
#ifndef __CSHA1WITHDSA_SIGNATUREIMPL_H__
#define __CSHA1WITHDSA_SIGNATUREIMPL_H__

#include "_CSHA1withDSA_SignatureImpl.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Provider {
namespace Crypto {

CarClass(CSHA1withDSA_SignatureImpl)
{
public:
    CARAPI GetProvider(
        /* [out] */ Elastos::Security::IProvider ** ppProvider);

    CARAPI GetAlgorithm(
        /* [out] */ String * pAlgo);

    CARAPI InitVerify(
        /* [in] */ Elastos::Security::IPublicKey * pPublicKey);

    CARAPI InitVerifyEx(
        /* [in] */ Elastos::Security::ICertificate * pCertificate);

    CARAPI InitSign(
        /* [in] */ Elastos::Security::IPrivateKey * pPrivateKey);

    CARAPI InitSignEx(
        /* [in] */ Elastos::Security::IPrivateKey * pPrivateKey,
        /* [in] */ Elastos::Security::ISecureRandom * pRandom);

    CARAPI Sign(
        /* [out, callee] */ ArrayOf<Byte> ** ppSign);

    CARAPI SignEx(
        /* [in] */ const ArrayOf<Byte> & outbuf,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 len,
        /* [out] */ Int32 * pNum);

    CARAPI Verify(
        /* [in] */ const ArrayOf<Byte> & signature,
        /* [out] */ Boolean * pIsVerified);

    CARAPI VerifyEx(
        /* [in] */ const ArrayOf<Byte> & signature,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Boolean * pIsVerified);

    CARAPI Update(
        /* [in] */ Byte b);

    CARAPI UpdateEx(
        /* [in] */ const ArrayOf<Byte> & data);

    CARAPI UpdateEx2(
        /* [in] */ const ArrayOf<Byte> & data,
        /* [in] */ Int32 off,
        /* [in] */ Int32 len);

    CARAPI UpdateEx3(
        /* [in] */ Elastos::IO::IByteBuffer * pData);

    CARAPI ToString(
        /* [out] */ String * pStr);

    CARAPI SetParameter(
        /* [in] */ const String& param,
        /* [in] */ IInterface * pValue);

    CARAPI SetParameterEx(
        /* [in] */ Elastos::Security::Spec::IAlgorithmParameterSpec * pParams);

    CARAPI GetParameters(
        /* [out] */ Elastos::Security::IAlgorithmParameters ** ppParam);

    CARAPI GetParameter(
        /* [in] */ const String& name,
        /* [out] */ IInterface ** ppParam);

    CARAPI Clone(
        /* [out] */ IInterface ** ppClone);

    CARAPI constructor();

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CSHA1WITHDSA_SIGNATUREIMPL_H__
