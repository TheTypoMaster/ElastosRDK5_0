
#ifndef __BEROUTPUTSTREAM_H__
#define __BEROUTPUTSTREAM_H__

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

class BerOutputStream
{
public:
    CARAPI SetEncoded(
        /* [in] */ ArrayOf<Byte>* encoded);

    CARAPI GetEncoded(
        /* [out, callee] */ ArrayOf<Byte>** encoded);

    CARAPI SetLength(
        /* [in] */ Int32 length);

    CARAPI GetLength(
        /* [out] */ Int32* length);

    CARAPI SetContent(
        /* [in] */ ArrayOf<Byte>* content);

    CARAPI GetContent(
        /* [out] */ IInterface** content);

    CARAPI EncodeTag(
        /* [in] */ Int32 tag);

    CARAPI EncodeANY();

    CARAPI EncodeBitString();

    CARAPI EncodeBoolean();

    CARAPI EncodeChoice(
        /* [in] */ IASN1Choice* choice);

    CARAPI EncodeExplicit(
        /* [in] */ IASN1Type* xplicit);

    CARAPI EncodeGeneralizedTime();

    CARAPI EncodeUTCTime();

    CARAPI EncodeInteger();

    CARAPI EncodeOctetString();

    CARAPI EncodeOID();

    CARAPI EncodeSequence(
        /* [in] */ IASN1Sequence* sequence);

    CARAPI EncodeSequenceOf(
        /* [in] */ IASN1SequenceOf* sequenceOf);

    CARAPI EncodeSet(
        /* [in] */ IASN1Set* set);

    CARAPI EncodeSetOf(
        /* [in] */ IASN1SetOf* setOf);

    CARAPI EncodeString();

    CARAPI GetChoiceLength(
        /* [in] */ IASN1Choice* choice);

    CARAPI GetExplicitLength(
        /* [in] */ IASN1Type* sequence);

    CARAPI GetSequenceLength(
        /* [in] */ IASN1Sequence* sequence);

    CARAPI GetSequenceOfLength(
        /* [in] */ IASN1SequenceOf* sequence);

    CARAPI GetSetLength(
        /* [in] */ IASN1Set* set);

    CARAPI GetSetOfLength(
        /* [in] */ IASN1SetOf* setOf);

public:
    /** Encoded byte array */
    AutoPtr<ArrayOf<Byte> > mEncoded;

    /** Current encoded length */
    Int32 mLength;

    /** Current encoded content */
    AutoPtr<IInterface> mContent;

protected:
    /** current offset */
    Int32 mOffset;
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __BEROUTPUTSTREAM_H__