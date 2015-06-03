
#include "CCertificationRequest.h"
#include <cmdef.h>
#include <CBitString.h>

using Org::Apache::Harmony::Security::Asn1::CBitString;
using Org::Apache::Harmony::Security::Asn1::IBitString;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Pkcs10 {

CAR_INTERFACE_IMPL(CCertificationRequest::ASN1SequenceDerived, IASN1Sequence)

ECode CCertificationRequest::ASN1SequenceDerived::GetId(
    /* [out] */ Int32* id)
{
    return ASN1Sequence::GetId(id);
}

ECode CCertificationRequest::ASN1SequenceDerived::GetConstrId(
    /* [out] */ Int32* constrId)
{
    return ASN1Sequence::GetConstrId(constrId);
}

ECode CCertificationRequest::ASN1SequenceDerived::Decode(
    /* [in] */ ArrayOf<Byte>* encoded,
    /* [out] */ IInterface** object)
{
    return ASN1Sequence::Decode(encoded, object);
}

ECode CCertificationRequest::ASN1SequenceDerived::DecodeEx(
    /* [in] */ ArrayOf<Byte>* encoded,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 encodingLen,
    /* [out] */ IInterface** object)
{
    return ASN1Sequence::DecodeEx(encoded, offset, encodingLen, object);
}

ECode CCertificationRequest::ASN1SequenceDerived::DecodeEx2(
    /* [in] */ IInputStream* is,
    /* [out] */ IInterface** object)
{
    return ASN1Sequence::DecodeEx2(is, object);
}

ECode CCertificationRequest::ASN1SequenceDerived::Verify(
    /* [in] */ ArrayOf<Byte>* encoded)
{
    return ASN1Sequence::Verify(encoded);
}

ECode CCertificationRequest::ASN1SequenceDerived::VerifyEx(
    /* [in] */ IInputStream* is)
{
    return ASN1Sequence::VerifyEx(is);
}

ECode CCertificationRequest::ASN1SequenceDerived::Encode(
    /* [in] */ IInterface* object,
    /* [out, callee] */ ArrayOf<Byte>** encode)
{
    return ASN1Sequence::Encode(object, encode);
}

ECode CCertificationRequest::ASN1SequenceDerived::DecodeEx3(
    /* [in] */ IBerInputStream* bis,
    /* [out] */ IInterface** object)
{
    bis->ReadSequence(this);

    if (((CBerInputStream*)bis)->mIsVerify) {
        return NOERROR;
    }
    return GetDecodedObject(bis, object);
}

ECode CCertificationRequest::ASN1SequenceDerived::CheckTag(
    /* [in] */ Int32 identifier,
    /* [out] */ Boolean* checkTag)
{
    return ASN1Sequence::CheckTag(identifier, checkTag);
}

ECode CCertificationRequest::ASN1SequenceDerived::EncodeASN(
    /* [in] */ IBerOutputStream* bos)
{
    return ASN1Sequence::EncodeASN(bos);
}

ECode CCertificationRequest::ASN1SequenceDerived::EncodeContent(
    /* [in] */ IBerOutputStream* bos)
{
    return bos->EncodeSequence(this);
}

ECode CCertificationRequest::ASN1SequenceDerived::SetEncodingContent(
    /* [in] */ IBerOutputStream* bos)
{
    return bos->GetSequenceLength(this);
}

ECode CCertificationRequest::ASN1SequenceDerived::GetEncodedLength(
    /* [in] */ IBerOutputStream* bos,
    /* [out] */ Int32* length)
{
    return ASN1Sequence::GetEncodedLength(bos, length);
}

ECode CCertificationRequest::ASN1SequenceDerived::ToString(
    /* [out] */ String* result)
{
    return ASN1Sequence::ToString(result);
}

ECode CCertificationRequest::ASN1SequenceDerived::SetType(
/* [in] */ ArrayOf<IASN1Type *>* type)
{
    return ASN1Sequence::SetType(type);
}

ECode CCertificationRequest::ASN1SequenceDerived::GetType(
    /* [out, callee] */ ArrayOf<IASN1Type *>** type)
{
    return ASN1Sequence::GetType(type);
}

ECode CCertificationRequest::ASN1SequenceDerived::SetOptional(
    /* [in] */ ArrayOf<Boolean>* optional)
{
    return ASN1Sequence::SetOptional(optional);
}

ECode CCertificationRequest::ASN1SequenceDerived::GetOptional(
    /* [out, callee] */ ArrayOf<Boolean>** optional)
{
    return ASN1Sequence::GetOptional(optional);
}

ECode CCertificationRequest::ASN1SequenceDerived::SetDefault(
    /* [in] */ ArrayOf<IInterface *>* def)
{
    return ASN1Sequence::SetDefault(def);
}

ECode CCertificationRequest::ASN1SequenceDerived::GetDefault(
    /* [out, callee] */ ArrayOf<IInterface *>** def)
{
    return ASN1Sequence::GetDefault(def);
}

ECode CCertificationRequest::ASN1SequenceDerived::GetDecodedObject(
    /* [in] */ IBerInputStream* bis,
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    AutoPtr<IInterface> content;
    bis->GetContent((IInterface**)&content);
    AutoPtr<IArrayOf> values = IArrayOf::Probe(content);
    AutoPtr<IInterface> tmp;
    values->Get(0, (IInterface**)&tmp);
    AutoPtr<ICertificationRequestInfo> arg1 = ICertificationRequestInfo::Probe(tmp);
    tmp = NULL;
    values->Get(1, (IInterface**)&tmp);
    AutoPtr<IAlgorithmIdentifier> arg2 = IAlgorithmIdentifier::Probe(tmp);
    tmp = NULL;
    values->Get(2, (IInterface**)&tmp);
    AutoPtr<IBitString> arg3 = IBitString::Probe(tmp);
    AutoPtr<ArrayOf<Byte> > bytes;
    arg3->GetBytes((ArrayOf<Byte>**)&bytes);
    AutoPtr<ArrayOf<Byte> > arg4;
    bis->GetEncoded((ArrayOf<Byte>**)&arg4);
    *object = new CCertificationRequest(arg1, arg2, bytes, arg4);
    REFCOUNT_ADD(*object)
    return NOERROR;
}

ECode CCertificationRequest::ASN1SequenceDerived::GetValues(
    /* [in] */ IInterface* object,
    /* [in] */ ArrayOf<IInterface*>* values)
{
    AutoPtr<ICertificationRequest> certReq = ICertificationRequest::Probe(object);
    AutoPtr<ICertificationRequestInfo> cri;
    certReq->GetInfo((ICertificationRequestInfo**)&cri);
    values->Set(0, cri.Get());
    AutoPtr<IAlgorithmIdentifier> ai;
    ((CCertificationRequest*)certReq.Get())->GetAlgorithmIdentifier((IAlgorithmIdentifier**)&ai);
    values->Set(1, ai.Get());
    AutoPtr<ArrayOf<Byte> > sig;
    certReq->GetSignature((ArrayOf<Byte>**)&sig);
    AutoPtr<IBitString> bs;
    CBitString::New(sig, 0, (IBitString**)&bs);
    return values->Set(2, bs.Get());
}

CCertificationRequest::ASN1SequenceDerived::ASN1SequenceDerived(
    /* [in] */ ArrayOf<IASN1Type *>* type)
{
    ASN1Sequence::Init(type);
}

AutoPtr<IASN1Sequence> CCertificationRequest::ASN1 = InitStatic();

AutoPtr<IASN1Sequence> CCertificationRequest::InitStatic()
{
    AutoPtr<IASN1Sequence> ret;
    AutoPtr<ArrayOf<IASN1Type*> > arg = ArrayOf<IASN1Type*>::Alloc(3);
    AutoPtr<ICertificationRequestInfoHelper> hlp;
    CCertificationRequestInfoHelper::AcquireSingleton((ICertificationRequestInfoHelper**)&hlp);
    AutoPtr<IASN1Sequence> as;
    hlp->GetASN1((IASN1Sequence**)&as);
    arg->Set(0, as.Get());
    AutoPtr<IAlgorithmIdentifier> ai;
    AutoPtr<IAlgorithmIdentifierHelper> aih;
    CAlgorithmIdentifierHelper::AcquireSingleton((IAlgorithmIdentifierHelper**)&aih);
    as = NULL;
    aih->GetASN1((IASN1Sequence**)&as);
    arg->Set(1, as.Get());
    AutoPtr<IASN1BitStringHelper> absh;
    CASN1BitStringHelper::AcquireSingleton((IASN1BitStringHelper**)&absh);
    AutoPtr<IASN1Type> at;
    absh->GetInstance((IASN1Type**)&at);
    arg->Set(2, at.Get());
    ret = new ASN1SequenceDerived(arg);
    return ret;
}

CCertificationRequest::CCertificationRequest()
{}

CCertificationRequest::CCertificationRequest(
    /* [in] */ ICertificationRequestInfo* info,
    /* [in] */ IAlgorithmIdentifier* algId,
    /* [in] */ ArrayOf<Byte>* signature,
    /* [in] */ ArrayOf<Byte>* encoding)
{
    constructor(info, algId, signature);
    mEncoding = encoding;
}

ECode CCertificationRequest::GetInfo(
    /* [out] */ ICertificationRequestInfo** info)
{
    VALIDATE_NOT_NULL(info)
    *info = mInfo;
    return NOERROR;
}

ECode CCertificationRequest::GetSignature(
    /* [out, callee] */ ArrayOf<Byte>** signature)
{
    VALIDATE_NOT_NULL(signature)
    AutoPtr<ArrayOf<Byte> > result = ArrayOf<Byte>::Alloc(mSignature->GetLength());
    result->Copy(0, mSignature, 0, mSignature->GetLength());
    *signature = result;
    REFCOUNT_ADD(*signature)
    return NOERROR;
}

ECode CCertificationRequest::GetEncoded(
    /* [out, callee] */ ArrayOf<Byte>** encoded)
{
    VALIDATE_NOT_NULL(encoded)
    if (mEncoding == NULL) {
        AutoPtr<ICertificationRequestHelper> helper;
        CCertificationRequestHelper::AcquireSingleton((ICertificationRequestHelper**)*helper);
        AutoPtr<IASN1Sequence> seq;
        helper->GetASN1((IASN1Sequence**)&seq);
        seq->Encode(this, (ArrayOf<Byte>**)&mEncoding);
    }
    *encoded = mEncoding;
    REFCOUNT_ADD(*encoded)
    return NOERROR;
}

ECode CCertificationRequest::GetAlgorithmIdentifier(
    /* [out] */ IAlgorithmIdentifier** algId)
{
    VALIDATE_NOT_NULL(algId)
    *algId = mAlgId;
    REFCOUNT_ADD(algId);
    return NOERROR;
}

ECode CCertificationRequest::constructor(
    /* [in] */ ICertificationRequestInfo* info,
    /* [in] */ IAlgorithmIdentifier* algId,
    /* [in] */ ArrayOf<Byte>* signature)
{
    mInfo = info;
    mAlgId = algId;
    mSignature = signature->Clone();
    return NOERROR;
}

} // namespace Pkcs10
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

