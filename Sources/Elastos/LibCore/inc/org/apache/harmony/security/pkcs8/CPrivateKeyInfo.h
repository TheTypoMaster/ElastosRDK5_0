
#ifndef __ORG_APACHE_HARMONY_SECURITY_PKCS8_CPRIVATEKEYINFO_H__
#define __ORG_APACHE_HARMONY_SECURITY_PKCS8_CPRIVATEKEYINFO_H__

#include "_CPrivateKeyInfo.h"

using Org::Apache::Harmony::Security::X509::IAlgorithmIdentifier;
using Elastos::Utility::IList;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Pkcs8 {

CarClass(CPrivateKeyInfo)
{
private:
    friend class ASN1SequenceDerived;
    class ASN1SequenceDerived
        : public ASN1Sequence
        , public IASN1Sequence
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()

        ASN1SEQUENCE_METHODS_DECL()

        ASN1SequenceDerived(
            /* [in] */ ArrayOf<IASN1Type *>* type);
    };

public:
    static AutoPtr<IASN1Sequence> InitStatic();

    CPrivateKeyInfo();

    CARAPI GetVersion(
        /* [out] */ Int32* ver);

    CARAPI GetAlgorithmIdentifier(
        /* [out] */ IAlgorithmIdentifier** identifier);

    CARAPI GetAttributes(
        /* [out] */ IList** attributes);

    CARAPI GetPrivateKey(
        /* [out, callee] */ ArrayOf<Byte>** privateKey);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte>** encoded);

    CARAPI constructor(
        /* [in] */ Int32 version,
        /* [in] */ IAlgorithmIdentifier* privateKeyAlgorithm,
        /* [in] */ ArrayOf<Byte>* privateKey,
        /* [in] */ IList* attributes);

private:
    CPrivateKeyInfo(
        /* [in] */ Int32 version,
        /* [in] */ IAlgorithmIdentifier* privateKeyAlgorithm,
        /* [in] */ ArrayOf<Byte>* privateKey,
        /* [in] */ IList* attributes,
        /* [in] */ ArrayOf<Byte>* encoding);

public:
    static AutoPtr<IASN1Sequence> ASN1;

private:
    Int32 mVersion;
    AutoPtr<IAlgorithmIdentifier> mPrivateKeyAlgorithm;
    AutoPtr<ArrayOf<Byte> > mPrivateKey;
    AutoPtr<IList> mAttributes;
    AutoPtr<ArrayOf<Byte> > mEncoding;
};

} // namespace Pkcs8
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HARMONY_SECURITY_PKCS8_CPRIVATEKEYINFO_H__
