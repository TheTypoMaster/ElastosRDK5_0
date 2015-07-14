
#include "FloatArrayBuffer.h"
#include "CByteOrderHelper.h"

namespace Elastos {
namespace IO {

FloatArrayBuffer::FloatArrayBuffer(
    /* [in] */ ArrayOf<Float>* array)
    : FloatBuffer(array->GetLength(), 0)
    , mBackingArray(array)
    , mArrayOffset(0)
    , mIsReadOnly(FALSE)
{}

FloatArrayBuffer::FloatArrayBuffer(
    /* [in] */ Int32 capacity,
    /* [in] */ ArrayOf<Float>* backingArray,
    /* [in] */ Int32 offset,
    /* [in] */ Boolean isReadOnly)
    : FloatBuffer(capacity, 0)
    , mBackingArray(backingArray)
    , mArrayOffset(offset)
    , mIsReadOnly(isReadOnly)
{}

ECode FloatArrayBuffer::Get(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    if (mPosition == mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = (*mBackingArray)[mArrayOffset + mPosition++];
    return NOERROR;
}

ECode FloatArrayBuffer::Get(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    FAIL_RETURN(CheckIndex(index))
    *value = (*mBackingArray)[mArrayOffset + index];
    return NOERROR;
}

ECode FloatArrayBuffer::Get(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 floatCount)
{
    VALIDATE_NOT_NULL(dst);
    Int32 remaining = 0;
    GetRemaining(&remaining);
    if (floatCount > remaining) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }

    dst->Copy(dstOffset, mBackingArray, mArrayOffset + mPosition, floatCount);
    mPosition += floatCount;
    return NOERROR;
}

ECode FloatArrayBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    *isDirect = FALSE;
    return NOERROR;
}

ECode FloatArrayBuffer::GetOrder(
   /* [out] */ ByteOrder* byteOrder)
{
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    return helper->GetNativeOrder(byteOrder);
}

ECode FloatArrayBuffer::AsReadOnlyBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    *buffer = (IFloatBuffer*) Copy(this, mMark, TRUE);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode FloatArrayBuffer::Compact()
{
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    // System.arraycopy(backingArray, mPosition + mArrayOffset, backingArray, mArrayOffset, remaining());
    Int32 reaminvalue = 0;
    GetRemaining(&reaminvalue);
    mBackingArray->Copy(mArrayOffset, mBackingArray, mPosition + mArrayOffset, reaminvalue);
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = UNSET_MARK;
    return NOERROR;
}

ECode FloatArrayBuffer::Duplicate(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    *buffer = (IFloatBuffer*) Copy(this, mMark, mIsReadOnly);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode FloatArrayBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Float>** array)
{
    VALIDATE_NOT_NULL(array)

    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    *array = mBackingArray;
    return NOERROR;
}

ECode FloatArrayBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset)

    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    *offset = mArrayOffset;
    return NOERROR;
}

ECode FloatArrayBuffer::ProtectedHasArray(
    /* [out] */ Boolean* hasArray)
{
    VALIDATE_NOT_NULL(hasArray)

    if (mIsReadOnly) {
        *hasArray = FALSE;
    }
    else {
        *hasArray = TRUE;
    }
    return NOERROR;
}

ECode FloatArrayBuffer::Put(
    /* [in] */ Float d)
{
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    (*mBackingArray)[mArrayOffset + mPosition++] = d;
    return NOERROR;
}

ECode FloatArrayBuffer::Put(
    /* [in] */ Int32 index,
    /* [in] */ Float d)
{
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    FAIL_RETURN(CheckIndex(index));
    (*mBackingArray)[mArrayOffset + index] = d;
    return NOERROR;
}

ECode FloatArrayBuffer::Put(
    /* [in] */ const ArrayOf<Float>& src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 doubleCount)
{
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    Int32 remainvalue = 0;
    GetRemaining(&remainvalue);
    if (doubleCount > remainvalue) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVERFLOW_EXCEPTION;
    }
    // System.arraycopy(src, srcOffset, backingArray, mArrayOffset + mPosition, doubleCount);
    mBackingArray->Copy(mArrayOffset + mPosition, &src, srcOffset, doubleCount);
    mPosition += doubleCount;
    return NOERROR;
}

ECode FloatArrayBuffer::Slice(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    Int32 remainvalue = 0;
    GetRemaining(&remainvalue);
    *buffer = (IFloatBuffer*) new FloatArrayBuffer(remainvalue, mBackingArray, mArrayOffset + mPosition, mIsReadOnly);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode FloatArrayBuffer::IsReadOnly(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mIsReadOnly;
    return NOERROR;
}

AutoPtr<FloatArrayBuffer> FloatArrayBuffer::Copy(
    /* [in] */ FloatArrayBuffer* other,
    /* [in] */ Int32 mMarkOfOther,
    /* [in] */ Boolean mIsReadOnly)
{
    Int32 capvalue = 0;
    other->GetCapacity(&capvalue);
    AutoPtr<FloatArrayBuffer> buf = new FloatArrayBuffer(capvalue, other->mBackingArray, other->mArrayOffset, mIsReadOnly);
    buf->mLimit = other->mLimit;
    other->GetPosition(&buf->mPosition);
    buf->mMark = mMarkOfOther;
    return buf;
}

} // namespace IO
} // namespace Elastos
