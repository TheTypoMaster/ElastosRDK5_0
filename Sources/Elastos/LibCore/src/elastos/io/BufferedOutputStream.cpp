
#include "BufferedOutputStream.h"
#include "Autolock.h"

namespace Elastos {
namespace IO {

CAR_INTERFACE_IMPL(BufferedOutputStream, FilterOutputStream, IBufferedOutputStream)

BufferedOutputStream::BufferedOutputStream()
    : mCount(0)
{
}

BufferedOutputStream::~BufferedOutputStream()
{
}

ECode BufferedOutputStream::constructor(
    /* [in] */ IOutputStream* outs)
{
    constructor(outs, 8192);
}

ECode BufferedOutputStream::constructor(
    /* [in] */ IOutputStream* outs,
    /* [in] */ Int32 size)
{
    VALIDATE_NOT_NULL(outs);
    FAIL_RETURN(FilterOutputStream::constructor(outs));
    if (size <= 0) {
//      throw new IllegalArgumentException("size <= 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mBuf = ArrayOf<Byte>::Alloc(size);
    if (mBuf == NULL) {
        return E_OUT_OF_MEMORY_ERROR;
    }
    return NOERROR;
}

ECode BufferedOutputStream::Close()
{
    Autolock lock(this);
    if (mBuf != NULL) {
        FilterOutputStream::Close();
        mBuf = NULL;
    }
    return NOERROR;
}

ECode BufferedOutputStream::Flush()
{
    Autolock lock(this);
    FAIL_RETURN(CheckNotClosed());
    FAIL_RETURN(FlushInternal());
    return IFlushable::Probe(mOut)->Flush();
}

ECode BufferedOutputStream::Write(
    /* [in] */ Int32 oneByte)
{
    Autolock lock(this);
    FAIL_RETURN(CheckNotClosed());

    if (mCount == mBuf->GetLength()) {
        mOut->Write(mBuf, 0, mCount);
        mCount = 0;
    }
    (*mBuf)[mCount++] = (Byte)oneByte;
    return NOERROR;
}

ECode BufferedOutputStream::Write(
    /* [in] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 count)
{
    VALIDATE_NOT_NULL(buffer)

    Autolock lock(this);
    FAIL_RETURN(CheckNotClosed());

    AutoPtr<ArrayOf<Byte> > localBuf = mBuf;
    if (count >= localBuf->GetLength()) {
        FAIL_RETURN(FlushInternal());
        return mOut->Write(buffer, offset, count);
    }

    if (offset < 0 || offset > buffer->GetLength() - count) {
//      throw new ArrayIndexOutOfBoundsException("Offset out of bounds: " + offset);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (count < 0) {
//      throw new ArrayIndexOutOfBoundsException("Length out of bounds: " + length);
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    // flush the internal buffer first if we have not enough space left
    if (count > (localBuf->GetLength() - mCount)) {
        FAIL_RETURN(FlushInternal());
    }

    // the length is always less than (internalBuffer.length - count) here so arraycopy is safe
    localBuf->Copy(mCount, buffer, offset, count);
    mCount += count;

    return NOERROR;
}

ECode BufferedOutputStream::FlushInternal()
{
    if (mCount > 0) {
        FAIL_RETURN(mOut->Write(mBuf, 0, mCount));
        mCount = 0;
    }
    return NOERROR;
}

ECode BufferedOutputStream::CheckNotClosed()
{
    if (mBuf == NULL)
        return E_IO_EXCEPTION;
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
