
#include "SequenceInputStream.h"
// #include "CVector.h"

using Elastos::Utility::IVector;
// using Elastos::Utility::CVector;

namespace Elastos {
namespace IO {

CAR_INTERFACE_IMPL(SequenceInputStream, InputStream, ISequenceInputStream)

ECode SequenceInputStream::constructor(
    /* [in] */ IInputStream* s1,
    /* [in] */ IInputStream* s2)
{
    if (s1 == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }

    AutoPtr<IVector> inVector;
    // CVector::New(1, (IVector**)&inVector);
    inVector->AddElement(s2);
    inVector->GetElements((IEnumeration**)&mEnum);
    mIn = s1;
    return NOERROR;
}

ECode SequenceInputStream::constructor(
    /* [in] */ IEnumeration* e)
{
    VALIDATE_NOT_NULL(e)
    mEnum = e;
    Boolean hasMore = FALSE;
    e->HasMoreElements(&hasMore);
    if (hasMore) {
        AutoPtr<IInterface> obj;
        e->GetNextElement((IInterface**)&obj);
        mIn = IInputStream::Probe(obj);
        if (mIn == NULL) {
            return E_NULL_POINTER_EXCEPTION;
        }
    }
    return NOERROR;
}

ECode SequenceInputStream::Available(
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)
    if (mEnum != NULL && mIn != NULL) {
        return mIn->Available(number);
    }
    *number = 0;

    return NOERROR;
}

ECode SequenceInputStream::Close()
{
    while (mIn != NULL) {
        NextStream();
    }
    mEnum = NULL;
    return NOERROR;
}

ECode SequenceInputStream::NextStream()
{
    if (mIn != NULL) {
        ICloseable::Probe(mIn)->Close();
        mIn = NULL;
    }
    Boolean succeeded = FALSE;
    mEnum->HasMoreElements(&succeeded);
    if (succeeded) {
        AutoPtr<IInterface> obj;
        mEnum->GetNextElement((IInterface**)&obj);
        mIn = IInputStream::Probe(obj);
        if (mIn == NULL) {
            return E_NULL_POINTER_EXCEPTION;
        }
    }

    return NOERROR;
}

ECode SequenceInputStream::Read(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    while (mIn != NULL) {
        mIn->Read(value);
        if ((*value) >= 0) {
            return NOERROR;
        }
        NextStream();
    }
    return -1;
}

ECode SequenceInputStream::Read(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 byteOffset,
    /* [in] */ Int32 byteCount,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)
    *number = 0;
    VALIDATE_NOT_NULL(buffer)
    if (mIn == NULL) {
        *number = -1;
        return NOERROR;
    }
    if (buffer == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    if ((byteOffset | byteCount) < 0 || byteOffset > buffer->GetLength() - byteCount) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    while (mIn != NULL) {
        mIn->Read(buffer, byteOffset, byteCount, number);
        if ((*number) >= 0) {
            return NOERROR;
        }
        NextStream();
    }
    *number = -1;

    return NOERROR;
}

} // namespace IO
} // namespace Elastos
