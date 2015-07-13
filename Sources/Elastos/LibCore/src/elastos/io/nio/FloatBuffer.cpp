
#include "FloatBuffer.h"
#include "CoreUtils.h"
#include "CArrayOf.h"

using Elastos::Core::CArrayOf;
using Elastos::Core::CoreUtils;

namespace Elastos {
namespace IO {

FloatBuffer::FloatBuffer()
{}

FloatBuffer::FloatBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ Int64 effectiveDirectAddress)
    : Buffer(2, capacity, effectiveDirectAddress)
{}

CAR_INTERFACE_IMPL_2(FloatBuffer, Object, IFloatBuffer, IBuffer)

ECode FloatBuffer::Allocate(
    /* [in] */ Int32 capacity,
    /* [out] */ IFloatBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    assert(0 && "TODO");
    // if (capacity < 0) {
    //     throw new IllegalArgumentException("capacity < 0: " + capacity);
    // }
    // return new FloatArrayBuffer(new float[capacity]);
    return NOERROR;
}

ECode FloatBuffer::Wrap(
    /* [in] */ ArrayOf<Float>* array,
    /* [out] */ IFloatBuffer** buf)
{
    return Wrap(array, 0, array->GetLength(), buf);
}

ECode FloatBuffer::Wrap(
    /* [in] */ ArrayOf<Float>* array,
    /* [in] */ Int32 start,
    /* [in] */ Int32 floatCount,
    /* [out] */ IFloatBuffer** buf)
{
    VALIDATE_NOT_NULL(buf);

    // Arrays.checkOffsetAndCount(array.length, start, floatCount);
    // FloatBuffer buf = new FloatArrayBuffer(array);
    // buf.position = start;
    // buf.limit = start + floatCount;
    // return buf;
    return NOERROR;
}

ECode FloatBuffer::GetArray(
    /* [out] */ IArrayOf** array)
{
    VALIDATE_NOT_NULL(array)

    AutoPtr< ArrayOf<Float> > res;
    GetArray((ArrayOf<Float>**)&res);
    AutoPtr<IArrayOf> iarr;
    CArrayOf::New(res->GetLength(), (IArrayOf**)&iarr);
    for (Int32 i = 0; i < res->GetLength(); ++i) {
        iarr->Set(i, CoreUtils::Convert((*res)[i]));
    }
    *array = iarr;
    REFCOUNT_ADD(*array)
    return NOERROR;
}

ECode FloatBuffer::GetArray(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    return ProtectedArray(array);
}

ECode FloatBuffer::GetArrayOffset(
    /* [out] */ Int32* offset)
{
    return ProtectedArrayOffset(offset);
}

ECode FloatBuffer::CompareTo(
    /* [in] */ IFloatBuffer* otherBuffer,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 remaining = 0;
    Int32 otherRemaining = 0;
    GetRemaining(&remaining);
    IBuffer::Probe(otherBuffer)->GetRemaining(&otherRemaining);
    Int32 compareRemaining = (remaining < otherRemaining) ? remaining : otherRemaining;
    Int32 thisPos = mPosition;
    Int32 otherPos = 0;
    IBuffer::Probe(otherBuffer)->GetPosition(&otherPos);
    Float thisFloat = 0.0f;
    Float otherFloat = 0.0f;
    while (compareRemaining > 0) {
        Get(thisPos, &thisFloat);
        otherBuffer->Get(otherPos, &otherFloat);
        // checks for Float and NaN inequality
        if ((thisFloat != otherFloat)
                && ((thisFloat == thisFloat) || (otherFloat == otherFloat))) {
            *result = thisFloat < otherFloat ? -1 : 1;
            return NOERROR;
        }
        thisPos++;
        otherPos++;
        compareRemaining--;
    }
    *result = remaining - otherRemaining;
    return NOERROR;
}

ECode FloatBuffer::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean* rst)
{
    VALIDATE_NOT_NULL(rst)

    if (IFloatBuffer::Probe(other) == NULL) {
        *rst = FALSE;
        return NOERROR;
    }

    AutoPtr<IFloatBuffer> otherBuffer = (IFloatBuffer*) other;
    Int32 thisRemaining = 0;
    Int32 otherRemaining = 0;
    GetRemaining(&thisRemaining);
    IBuffer::Probe(otherBuffer)->GetRemaining(&otherRemaining);
    if (thisRemaining != otherRemaining) {
        *rst = FALSE;
        return NOERROR;
    }

    Int32 myPosition = mPosition;
    Int32 otherPosition = 0;
    IBuffer::Probe(otherBuffer)->GetPosition(&otherPosition);
    Boolean equalSoFar = TRUE;
    Float thisValue = 0.0f;
    Float otherValue = 0.0f;
    while (equalSoFar && (myPosition < mLimit)) {
        FAIL_RETURN(Get(myPosition++, &thisValue))
        FAIL_RETURN(otherBuffer->Get(otherPosition++, &otherValue))
        equalSoFar = thisValue == otherValue || (thisValue != thisValue && otherValue != otherValue);
    }
    *rst = equalSoFar;
    return NOERROR;
}

ECode FloatBuffer::Get(
    /* [out] */ ArrayOf<Float>* dst)
{
    return Get(dst, 0, dst->GetLength());
}

ECode FloatBuffer::Get(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 floatCount)
{
    VALIDATE_NOT_NULL(dst);
    Int32 arrayLength = dst->GetLength();
    if ((dstOffset | floatCount) < 0 || dstOffset > arrayLength || arrayLength - dstOffset < floatCount) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
        //         count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (floatCount > remaining) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }

    for (Int32 i = dstOffset; i < dstOffset + floatCount; ++i) {
        Get(&(*dst)[i]);
    }
    return NOERROR;
}

ECode FloatBuffer::HasArray(
    /* [out] */ Boolean* hasArray)
{
    return ProtectedHasArray(hasArray);
}

ECode FloatBuffer::Put(
    /* [in] */ const ArrayOf<Float>& src)
{
    return Put(src, 0, src.GetLength());
}

ECode FloatBuffer::Put(
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 floatCount)
{
    Int32 arrayLength = src.GetLength();
    if ((srcOffset | floatCount) < 0 || srcOffset > arrayLength || arrayLength - srcOffset < floatCount) {
        // throw new ArrayIndexOutOfBoundsException(arrayLength, offset,
        //         count);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (floatCount > remaining) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }

    for (Int32 i = srcOffset; i < srcOffset + floatCount; ++i) {
        Put(src[i]);
    }
    return NOERROR;
}

ECode FloatBuffer::Put(
    /* [in] */ IFloatBuffer* src)
{
    if (src == (IFloatBuffer*)this->Probe(EIID_IFloatBuffer)) {
        // throw new IllegalArgumentException("src == this");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 srcRemaining = 0;
    Int32 remaining = 0;
    IBuffer::Probe(src)->GetRemaining(&srcRemaining);
    GetRemaining(&remaining);
    if (srcRemaining > remaining) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }

    AutoPtr< ArrayOf<Float> > contents = ArrayOf<Float>::Alloc(srcRemaining);
    FAIL_RETURN(src->Get(contents))
    return Put(*contents);
}

} // namespace IO
} // namespace Elastos
