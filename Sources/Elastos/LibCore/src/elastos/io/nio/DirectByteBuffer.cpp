
#include "DirectByteBuffer.h"
#include "CByteOrderHelper.h"
#include "Math.h"
#include "ByteBufferAsCharBuffer.h"
#include "ByteBufferAsDoubleBuffer.h"
#include "ByteBufferAsFloatBuffer.h"
#include "ByteBufferAsInt16Buffer.h"
#include "ByteBufferAsInt32Buffer.h"
#include "ByteBufferAsInt64Buffer.h"
#include "Memory.h"

using Elastos::Core::Math;
using Libcore::IO::ISizeOf;
using Libcore::IO::Memory;

namespace Elastos {
namespace IO {

// {A2A31377-84A4-30B4-30D9-55F3369DD50F}
extern "C" const InterfaceID EIID_DirectByteBuffer =
    { 0xa2a31377, 0x84a4, 0x30b4, { 0x30, 0xd9, 0x55, 0xf3, 0x36, 0x9d, 0xd5, 0x0f } };

DirectByteBuffer::DirectByteBuffer(
    /* [in] */ Int64 address,
    /* [in] */ Int32 capacity)
    : MappedByteBuffer(MemoryBlock::WrapFromNative(address, capacity), capacity, NULL, MemoryBlock::WrapFromNative(address, capacity)->ToInt64())
{
    Int64 baseSize = MemoryBlock::WrapFromNative(address, capacity)->GetSize();
    Int32 offset = 0;
    // We're throwing this exception after we passed a bogus value
    // to the superclass constructor, but it doesn't make any
    // difference in this case.
    if (baseSize >= 0 && (capacity + offset) > baseSize) {
        assert(0);
        // throw new IllegalArgumentException("capacity + offset > baseSize");
    }

    mOffset = offset;
    mIsReadOnly = FALSE;
}

DirectByteBuffer::DirectByteBuffer(
    /* [in] */ MemoryBlock* block,
    /* [in] */ Int32 capacity,
    /* [in] */ Int32 offset,
    /* [in] */ Boolean mIsReadOnly,
    /* [in] */ FileChannelMapMode mapMode)
    : MappedByteBuffer(block, capacity, mapMode, block->ToInt64() + offset)
{
    Int64 baseSize = block->GetSize();
    // We're throwing this exception after we passed a bogus value
    // to the superclass constructor, but it doesn't make any
    // difference in this case.
    if (baseSize >= 0 && (capacity + offset) > baseSize) {
        assert(0);
        // throw new IllegalArgumentException("capacity + offset > baseSize");
    }

    mOffset = offset;
    mIsReadOnly = mIsReadOnly;
}

ECode DirectByteBuffer::Get(
    /* [out] */ ArrayOf<Byte>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 byteCount)
{
    VALIDATE_NOT_NULL(dst);
    Int32 bounds;
    FAIL_RETURN(CheckGetBounds(1, dst->GetLength(), dstOffset, byteCount, &bounds));
    mBlock->PeekByteArray(mOffset + mPosition, dst, dstOffset, byteCount);
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::GetChars(
    /* [out] */ ArrayOf<Char32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 charCount)
{
    VALIDATE_NOT_NULL(dst);
    Int32 byteCount = 0;
    FAIL_RETURN(CheckGetBounds(sizeof(Char32), dst->GetLength(), dstOffset, charCount, &byteCount));
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    mBlock->PeekCharArray(mOffset + mPosition, dst, dstOffset, charCount, needsSwap);
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::GetDoubles(
    /* [out] */ ArrayOf<Double>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 doubleCount)
{
    VALIDATE_NOT_NULL(dst);
    Int32 byteCount = 0;
    FAIL_RETURN(CheckGetBounds(sizeof(Double), dst->GetLength(), dstOffset, doubleCount, &byteCount))
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    mBlock->PeekDoubleArray(mOffset + mPosition, dst, dstOffset, doubleCount, needsSwap);
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::GetFloats(
    /* [out] */ ArrayOf<Float>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 floatCount)
{
    VALIDATE_NOT_NULL(dst);
    Int32 byteCount = 0;
    FAIL_RETURN(CheckGetBounds(sizeof(Float), dst->GetLength(), dstOffset, floatCount, &byteCount))
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    mBlock->PeekFloatArray(mOffset + mPosition, dst, dstOffset, floatCount, needsSwap);
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::GetInt32s(
    /* [out] */ ArrayOf<Int32>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int32Count)
{
    VALIDATE_NOT_NULL(dst);
    Int32 byteCount = 0;
    FAIL_RETURN(CheckGetBounds(sizeof(Int32), dst->GetLength(), dstOffset, int32Count, &byteCount))
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    mBlock->PeekInt32Array(mOffset + mPosition, dst, dstOffset, int32Count, needsSwap);
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::GetInt64s(
    /* [out] */ ArrayOf<Int64>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int64Count)
{
    VALIDATE_NOT_NULL(dst);
    Int32 byteCount = 0;
    FAIL_RETURN(CheckGetBounds(ISizeOf::LONG, dst->GetLength(), dstOffset, int64Count, &byteCount))
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    mBlock->PeekInt64Array(mOffset + mPosition, dst, dstOffset, int64Count, needsSwap);
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::GetInt16s(
    /* [out] */ ArrayOf<Int16>* dst,
    /* [in] */ Int32 dstOffset,
    /* [in] */ Int32 int16Count)
{
    VALIDATE_NOT_NULL(dst);
    Int32 byteCount = 0;
    FAIL_RETURN(CheckGetBounds(sizeof(Int16), dst->GetLength(), dstOffset, int16Count, &byteCount))
    AutoPtr<IByteOrderHelper> helper;
    ASSERT_SUCCEEDED(CByteOrderHelper::AcquireSingleton((IByteOrderHelper**)&helper))
    Boolean needsSwap = FALSE;
    ASSERT_SUCCEEDED(helper->IsNeedsSwap(mOrder, &needsSwap))
    mBlock->PeekInt16Array(mOffset + mPosition, dst, dstOffset, int16Count, needsSwap);
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::Get(
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);
    if (mPosition == mLimit) {
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = mBlock->PeekByte(mOffset + mPosition++);
    return NOERROR;
}

ECode DirectByteBuffer::Get(
    /* [in] */ Int32 index,
    /* [out] */ Byte* value)
{
    VALIDATE_NOT_NULL(value);
    FAIL_RETURN(CheckIndex(index))
    *value = mBlock->PeekByte(mOffset + index);
    return NOERROR;
}

ECode DirectByteBuffer::GetChar(
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value)
    Int32 newPosition = mPosition + sizeof(Char32);
    if (newPosition > mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = (Char32) mBlock->PeekInt32(mOffset + mPosition, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode DirectByteBuffer::GetChar(
    /* [in] */ Int32 index,
    /* [out] */ Char32* value)
{
    VALIDATE_NOT_NULL(value)
    FAIL_RETURN(CheckIndex(index, sizeof(Char32)))
    *value = (Char32) mBlock->PeekInt32(mOffset + index, mOrder);
    return NOERROR;
}

ECode DirectByteBuffer::GetDouble(
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 newPosition = mPosition + sizeof(Double);
    if (newPosition > mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = Elastos::Core::Math::Int64BitsToDouble(mBlock->PeekInt64(mOffset + mPosition, mOrder));
    mPosition = newPosition;
    return NOERROR;
}

ECode DirectByteBuffer::GetDouble(
    /* [in] */ Int32 index,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    FAIL_RETURN(CheckIndex(index, sizeof(Double)))
    *value = Elastos::Core::Math::Int64BitsToDouble(mBlock->PeekInt64(mOffset + index, mOrder));
    return NOERROR;
}

ECode DirectByteBuffer::GetFloat(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 newPosition = mPosition + sizeof(Float);
    if (newPosition > mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = Elastos::Core::Math::Int32BitsToFloat(mBlock->PeekInt32(mOffset + mPosition, mOrder));
    mPosition = newPosition;
    return NOERROR;
}

ECode DirectByteBuffer::GetFloat(
    /* [in] */ Int32 index,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    FAIL_RETURN(CheckIndex(index, sizeof(Float)))
    *value = Elastos::Core::Math::Int32BitsToFloat(mBlock->PeekInt32(mOffset + index, mOrder));
    return NOERROR;
}

ECode DirectByteBuffer::GetInt32(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 newPosition = mPosition + sizeof(Int32);
    if (newPosition > mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = mBlock->PeekInt32(mOffset + mPosition, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode DirectByteBuffer::GetInt32(
    /* [in] */ Int32 index,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    FAIL_RETURN(CheckIndex(index, sizeof(Int32)))
    *value = mBlock->PeekInt32(mOffset + index, mOrder);
    return NOERROR;
}

ECode DirectByteBuffer::GetInt64(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 newPosition = mPosition + ISizeOf::LONG;
    if (newPosition > mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = mBlock->PeekInt64(mOffset + mPosition, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode DirectByteBuffer::GetInt64(
    /* [in] */ Int32 index,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    FAIL_RETURN(CheckIndex(index, ISizeOf::LONG))
    *value = mBlock->PeekInt64(mOffset + index, mOrder);
    return NOERROR;
}

ECode DirectByteBuffer::GetInt16(
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    Int32 newPosition = mPosition + sizeof(Int16);
    if (newPosition > mLimit) {
        // throw new BufferUnderflowException();
        return E_BUFFER_UNDER_FLOW_EXCEPTION;
    }
    *value = mBlock->PeekInt16(mOffset + mPosition, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode DirectByteBuffer::GetInt16(
    /* [in] */ Int32 index,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    FAIL_RETURN(CheckIndex(index, sizeof(Int16)))
    *value = mBlock->PeekInt16(mOffset + index, mOrder);
    return NOERROR;
}

ECode DirectByteBuffer::IsDirect(
    /* [out] */ Boolean* isDirect)
{
    VALIDATE_NOT_NULL(isDirect);
    *isDirect = TRUE;
    return NOERROR;
}

ECode DirectByteBuffer::Free()
{
    mBlock->Free();
    return NOERROR;
}

ECode DirectByteBuffer::ProtectedArray(
    /* [out, callee] */ ArrayOf<Byte>** array)
{
    VALIDATE_NOT_NULL(array);
    AutoPtr< ArrayOf<Byte> > arr = mBlock->GetArray();
    if (arr == NULL) {
        // throw new UnsupportedOperationException();
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    }
    *array = arr;
    REFCOUNT_ADD(*array);
    return NOERROR;
}

ECode DirectByteBuffer::ProtectedArrayOffset(
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);
    AutoPtr< ArrayOf<Byte> > array;
    FAIL_RETURN(ProtectedArray((ArrayOf<Byte>**)&array)); // Throw if we don't have an array.
    *offset = mOffset;
    return NOERROR;
}

ECode DirectByteBuffer::ProtectedHasArray(
    /* [out] */ Boolean* hasArray)
{
    VALIDATE_NOT_NULL(hasArray);
    *hasArray = mBlock->GetArray() != NULL;
    return NOERROR;
}

ECode DirectByteBuffer::Put(
    /* [in] */ Byte b)
{
    FAIL_RETURN(CheckIsAccessible());
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    if (mPosition == mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVERFLOW_EXCEPTION;
    }
    mBlock->PokeByte(mOffset + mPosition++, b);
    return NOERROR;
}

ECode DirectByteBuffer::Put(
    /* [in] */ Int32 index,
    /* [in] */ Byte b)
{
    FAIL_RETURN(CheckIsAccessible());
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    FAIL_RETURN(CheckIndex(index));
    mBlock->PokeByte(mOffset + index, b);
    return NOERROR;
}

ECode DirectByteBuffer::Put(
    /* [in] */ ArrayOf<Byte>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 byteCount)
{
    FAIL_RETURN(CheckIsAccessible());
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    Int32 outvalue = 0;
    FAIL_RETURN(CheckPutBounds(1, src->GetLength(), srcOffset, byteCount, &outvalue));
    mBlock->PokeByteArray(mOffset + mPosition, src, srcOffset, byteCount);
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::PutChar(
    /* [in] */ Char32 value)
{
    FAIL_RETURN(CheckIsAccessible());
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    Int32 newPosition = mPosition + ISizeOf::CHAR;
    if (newPosition > mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVERFLOW_EXCEPTION;
    }
    mBlock->PokeInt16(mOffset + mPosition, (Int16) value, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode DirectByteBuffer::PutChar(
    /* [in] */ Int32 index,
    /* [in] */ Char32 value)
{
    FAIL_RETURN(CheckIsAccessible());
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    FAIL_RETURN(CheckIndex(index, ISizeOf::CHAR));
    mBlock->PokeInt16(mOffset + index, (Int16) value, mOrder);
    return NOERROR;
}

ECode DirectByteBuffer::PutDouble(
    /* [in] */ Double value)
{
    FAIL_RETURN(CheckIsAccessible());
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    Int32 newPosition = mPosition + ISizeOf::DOUBLE;
    if (newPosition > mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVERFLOW_EXCEPTION;
    }
    assert(0 && "TODO");
    // mBlock->PokeInt64(mOffset + mPosition, Double.doubleToRawLongBits(value), mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode DirectByteBuffer::PutDouble(
    /* [in] */ Int32 index,
    /* [in] */ Double value)
{
    FAIL_RETURN(CheckIsAccessible());
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    FAIL_RETURN(CheckIndex(index, ISizeOf::DOUBLE));
    assert(0 && "TODO");
    // mBlock->PokeInt64(mOffset + index, Double.doubleToRawLongBits(value), mOrder);
    return NOERROR;
}

ECode DirectByteBuffer::PutFloat(
    /* [in] */ Float value)
{
    FAIL_RETURN(CheckIsAccessible());
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    Int32 newPosition = mPosition + ISizeOf::FLOAT;
    if (newPosition > mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVERFLOW_EXCEPTION;
    }
    assert(0 && "TODO");
    // mBlock->PokeInt32(mOffset + mPosition, Float.floatToRawIntBits(value), mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode DirectByteBuffer::PutFloat(
    /* [in] */ Int32 index,
    /* [in] */ Float value)
{
    FAIL_RETURN(CheckIsAccessible());
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    FAIL_RETURN(CheckIndex(index, ISizeOf::FLOAT));
    assert(0 && "TODO");
    // mBlock->PokeInt32(mOffset + index, Float.floatToRawIntBits(value), mOrder);
    return NOERROR;
}

ECode DirectByteBuffer::PutInt16(
    /* [in] */ Int16 value)
{
    FAIL_RETURN(CheckIsAccessible());
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    Int32 newPosition = mPosition + ISizeOf::SHORT;
    if (newPosition > mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVERFLOW_EXCEPTION;
    }
    mBlock->PokeInt16(mOffset + mPosition, value, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode DirectByteBuffer::PutInt16(
    /* [in] */ Int32 index,
    /* [in] */ Int16 value)
{
    FAIL_RETURN(CheckIsAccessible());
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    FAIL_RETURN(CheckIndex(index, ISizeOf::SHORT));
    mBlock->PokeInt16(mOffset + index, value, mOrder);
    return NOERROR;
}

ECode DirectByteBuffer::PutInt32(
    /* [in] */ Int32 value)
{
    FAIL_RETURN(CheckIsAccessible());
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    Int32 newPosition = mPosition + ISizeOf::INT;
    if (newPosition > mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVERFLOW_EXCEPTION;
    }
    mBlock->PokeInt32(mOffset + mPosition, value, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode DirectByteBuffer::PutInt32(
    /* [in] */ Int32 index,
    /* [in] */ Int32 value)
{
    FAIL_RETURN(CheckIsAccessible());
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    FAIL_RETURN(CheckIndex(index, ISizeOf::INT));
    mBlock->PokeInt32(mOffset + index, value, mOrder);
    return NOERROR;
}

ECode DirectByteBuffer::PutInt64(
    /* [in] */ Int64 value)
{
    FAIL_RETURN(CheckIsAccessible());
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    Int32 newPosition = mPosition + ISizeOf::LONG;
    if (newPosition > mLimit) {
        // throw new BufferOverflowException();
        return E_BUFFER_OVERFLOW_EXCEPTION;
    }
    mBlock->PokeInt64(mOffset + mPosition, value, mOrder);
    mPosition = newPosition;
    return NOERROR;
}

ECode DirectByteBuffer::PutInt64(
    /* [in] */ Int32 index,
    /* [in] */ Int64 value)
{
    FAIL_RETURN(CheckIsAccessible());
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    FAIL_RETURN(CheckIndex(index, ISizeOf::LONG));
    mBlock->PokeInt64(mOffset + index, value, mOrder);
    return NOERROR;
}

ECode DirectByteBuffer::Slice(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    FAIL_RETURN(CheckNotFreed());
    Int32 remainvalue = 0;
    GetRemaining(&remainvalue);
    AutoPtr<IByteBuffer> res = (IByteBuffer*) new DirectByteBuffer(mBlock, remainvalue, mOffset + mPosition, mIsReadOnly, mMapMode);
    *buffer = res;
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode DirectByteBuffer::AsCharBuffer(
    /* [out] */ ICharBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    FAIL_RETURN(CheckNotFreed());
    *buffer = ByteBufferAsCharBuffer::AsCharBuffer(this);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode DirectByteBuffer::AsDoubleBuffer(
    /* [out] */ IDoubleBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    FAIL_RETURN(CheckNotFreed());
    *buffer = ByteBufferAsDoubleBuffer::AsDoubleBuffer(this);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode DirectByteBuffer::AsFloatBuffer(
    /* [out] */ IFloatBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    FAIL_RETURN(CheckNotFreed());
    *buffer = ByteBufferAsFloatBuffer::AsFloatBuffer(this);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode DirectByteBuffer::AsInt16Buffer(
    /* [out] */ IInt16Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    FAIL_RETURN(CheckNotFreed());
    *buffer = ByteBufferAsInt16Buffer::AsInt16Buffer(this);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode DirectByteBuffer::AsInt32Buffer(
    /* [out] */ IInt32Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    FAIL_RETURN(CheckNotFreed());
    *buffer = ByteBufferAsInt32Buffer::AsInt32Buffer(this);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode DirectByteBuffer::AsInt64Buffer(
    /* [out] */ IInt64Buffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    FAIL_RETURN(CheckNotFreed());
    *buffer = ByteBufferAsInt64Buffer::AsInt64Buffer(this);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode DirectByteBuffer::AsReadOnlyBuffer(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    *buffer = Copy(this, mMark, TRUE);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode DirectByteBuffer::Compact()
{
    FAIL_RETURN(CheckIsAccessible());
    if (mIsReadOnly) {
        // throw new ReadOnlyBufferException();
        return E_READ_ONLY_BUFFER_EXCEPTION;
    }
    Int32 remainvalue = 0;
    GetRemaining(&remainvalue);
    assert(0 && "TODO");
    // Memory::Memmove(this, 0, this, mPosition, remainvalue);
    mPosition = mLimit - mPosition;
    mLimit = mCapacity;
    mMark = UNSET_MARK;
    return NOERROR;
}

ECode DirectByteBuffer::Duplicate(
    /* [out] */ IByteBuffer** buffer)
{
    VALIDATE_NOT_NULL(buffer)

    *buffer = Copy(this, mMark, mIsReadOnly);
    REFCOUNT_ADD(*buffer)
    return NOERROR;
}

ECode DirectByteBuffer::IsReadOnly(
    /* [out] */ Boolean* value)
{
    *value = mIsReadOnly;
    return NOERROR;
}

ECode DirectByteBuffer::PutChars(
    /* [in] */ ArrayOf<Char32>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 charCount)
{
    FAIL_RETURN(CheckIsAccessible());
    Int32 byteCount = 0;
    CheckPutBounds(ISizeOf::CHAR, src->GetLength(), srcOffset, charCount, &byteCount);
    mBlock->PokeCharArray(mOffset + mPosition, src, srcOffset, charCount, CByteOrderHelper::_IsNeedsSwap(mOrder));
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::PutDoubles(
    /* [in] */ ArrayOf<Double>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 doubleCount)
{
    FAIL_RETURN(CheckIsAccessible());
    Int32 byteCount = 0;
    FAIL_RETURN(CheckPutBounds(ISizeOf::DOUBLE, src->GetLength(), srcOffset, doubleCount, &byteCount));
    mBlock->PokeDoubleArray(mOffset + mPosition, src, srcOffset, doubleCount, CByteOrderHelper::_IsNeedsSwap(mOrder));
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::PutFloats(
    /* [in] */ ArrayOf<Float>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 floatCount)
{
    FAIL_RETURN(CheckIsAccessible());
    Int32 byteCount = 0;
    FAIL_RETURN(CheckPutBounds(ISizeOf::FLOAT, src->GetLength(), srcOffset, floatCount, &byteCount));
    mBlock->PokeFloatArray(mOffset + mPosition, src, srcOffset, floatCount, CByteOrderHelper::_IsNeedsSwap(mOrder));
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::PutInt32s(
    /* [in] */ ArrayOf<Int32>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 intCount)
{
    FAIL_RETURN(CheckIsAccessible());
    Int32 byteCount = 0;
    FAIL_RETURN(CheckPutBounds(ISizeOf::INT, src->GetLength(), srcOffset, intCount, &byteCount));
    mBlock->PokeInt32Array(mOffset + mPosition, src, srcOffset, intCount, CByteOrderHelper::_IsNeedsSwap(mOrder));
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::PutInt64s(
    /* [in] */ ArrayOf<Int64>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 longCount)
{
    FAIL_RETURN(CheckIsAccessible());
    Int32 byteCount = 0;
    FAIL_RETURN(CheckPutBounds(ISizeOf::LONG, src->GetLength(), srcOffset, longCount, &byteCount));
    mBlock->PokeInt64Array(mOffset + mPosition, src, srcOffset, longCount, CByteOrderHelper::_IsNeedsSwap(mOrder));
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::PutInt16s(
    /* [in] */ ArrayOf<Int16>* src,
    /* [in] */ Int32 srcOffset,
    /* [in] */ Int32 shortCount)
{
    FAIL_RETURN(CheckIsAccessible());
    Int32 byteCount = 0;
    FAIL_RETURN(CheckPutBounds(ISizeOf::SHORT, src->GetLength(), srcOffset, shortCount, &byteCount));
    mBlock->PokeInt16Array(mOffset + mPosition, src, srcOffset, shortCount, CByteOrderHelper::_IsNeedsSwap(mOrder));
    mPosition += byteCount;
    return NOERROR;
}

ECode DirectByteBuffer::CheckIsAccessible()
{
    FAIL_RETURN(CheckNotFreed());
    if (!mBlock->IsAccessible()) {
        // throw new IllegalStateException("buffer is inaccessible");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode DirectByteBuffer::CheckNotFreed()
{
    if (mBlock->IsFreed()) {
        // throw new IllegalStateException("buffer was freed");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

AutoPtr<DirectByteBuffer> DirectByteBuffer::Copy(
    /* [in] */ DirectByteBuffer* other,
    /* [in] */ Int32 markOfOther,
    /* [in] */ Boolean isReadOnly)
{
    FAIL_RETURN_NULL(other->CheckNotFreed());
    Int32 capvalue = 0;
    other->GetCapacity(&capvalue);
    AutoPtr<DirectByteBuffer> buf = new DirectByteBuffer(other->mBlock, capvalue, other->mOffset, isReadOnly, other->mMapMode);
    buf->mLimit = other->mLimit;
    other->GetPosition(&buf->mPosition);
    buf->mMark = markOfOther;
    return buf;
}

} // namespace IO
} // namespace Elastos
