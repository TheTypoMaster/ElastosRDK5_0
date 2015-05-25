
#ifndef __CCERTIFICATIONREQUEST_H__
#define __CCERTIFICATIONREQUEST_H__

#include "_CCertificationRequest.h"
#include <ASN1Sequence.h>

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Pkcs10 {

CarClass(CCertificationRequest)
{
public:
    class ASN1SequenceDerived
        : public ASN1Sequence
        , public IASN1Sequence
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL()
        ASN1TYPE_METHODS_DECL()
        CARAPI SetType(
        /* [in] */ ArrayOf<IASN1Type *>* type);

        CARAPI GetType(
            /* [out, callee] */ ArrayOf<IASN1Type *>** type);

        CARAPI SetOptional(
            /* [in] */ ArrayOf<Boolean>* optional);

        CARAPI GetOptional(
            /* [out, callee] */ ArrayOf<Boolean>** optional);

        CARAPI SetDefault(
            /* [in] */ ArrayOf<IInterface *>* def);

        CARAPI GetDefault(
            /* [out, callee] */ ArrayOf<IInterface *>** def);

        CARAPI GetValues(
            /* [in] */ IInterface* object,
            /* [in] */ ArrayOf<IInterface*>* values);
        ASN1SequenceDerived(
            /* [in] */ ArrayOf<IASN1Type *>* type);
    };

    static AutoPtr<IASN1Sequence> InitStatic();

    CCertificationRequest();

    CCertificationRequest(
        /* [in] */ ICertificationRequestInfo* info,
        /* [in] */ IAlgorithmIdentifier* algId,
        /* [in] */ ArrayOf<Byte>* signature,
        /* [in] */ ArrayOf<Byte>* encoding);

    CARAPI GetInfo(
        /* [out] */ ICertificationRequestInfo** info);

    CARAPI GetSignature(
        /* [out, callee] */ ArrayOf<Byte>** signature);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte>** encoded);

    CARAPI constructor(
        /* [in] */ ICertificationRequestInfo* info,
        /* [in] */ IAlgorithmIdentifier* algId,
        /* [in] */ ArrayOf<Byte>* signature);

    CARAPI GetAlgorithmIdentifier(
        /* [out] */ IAlgorithmIdentifier** algId);

public:
    static AutoPtr<IASN1Sequence> ASN1;

private:
        /** the value of certificationRequestInfo field of the structure */
    AutoPtr<ICertificationRequestInfo> mInfo;

    /** the value of signatureAlgorithm field of the structure */
    AutoPtr<IAlgorithmIdentifier> mAlgId;

    /** the value of signature field of the structure */
    AutoPtr<ArrayOf<Byte> > mSignature;

    /** the ASN.1 encoded form of CertificationRequest */
    AutoPtr<ArrayOf<Byte> > mEncoding;
};

} // namespace Pkcs10
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CCERTIFICATIONREQUEST_H__
