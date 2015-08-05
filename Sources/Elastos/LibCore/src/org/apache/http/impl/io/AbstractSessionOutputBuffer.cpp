
#include "AbstractSessionOutputBuffer.h"
#include "CCharArrayBuffer.h"
#include "CByteArrayBuffer.h"
#include "HttpProtocolParams.h"
#include <elastos/Logger.h>
#include <elastos/core/Math.h>

using Elastos::Core::Math;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::Params::HttpProtocolParams;
using Org::Apache::Http::Params::ICoreConnectionPNames;
using Org::Apache::Http::Protocol::IHTTP;
using Org::Apache::Http::Utility::CCharArrayBuffer;
using Org::Apache::Http::Utility::IByteArrayBuffer;
using Org::Apache::Http::Utility::CByteArrayBuffer;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace IO {

static AutoPtr< ArrayOf<Byte> > InitCrlf()
{
    AutoPtr< ArrayOf<Byte> > bytes = ArrayOf<Byte>::Alloc(2);
    (*bytes)[0] = IHTTP::CR;
    (*bytes)[1] = IHTTP::LF;
}
const AutoPtr< ArrayOf<Byte> > AbstractSessionOutputBuffer::CRLF = InitCrlf();
const Int32 AbstractSessionOutputBuffer::MAX_CHUNK;

AbstractSessionOutputBuffer::AbstractSessionOutputBuffer()
    : mCharset(IHTTP::US_ASCII)
    , mAscii(TRUE)
{}

CAR_INTERFACE_IMPL(AbstractSessionOutputBuffer, Object, ISessionOutputBuffer)

ECode AbstractSessionOutputBuffer::Init(
    /* [in] */ IOutputStream* outstream,
    /* [in] */ Int32 buffersize,
    /* [in] */ IHttpParams* params)
{
    if (outstream == NULL) {
        Logger::E("AbstractSessionOutputBuffer", "Output stream may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (buffersize <= 0) {
        Logger::E("AbstractSessionOutputBuffer", "Buffer size may not be negative or zero");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        Logger::E("AbstractSessionOutputBuffer", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mOutstream = outstream;
    CByteArrayBuffer::New(buffersize, (IByteArrayBuffer**)&mBuffer);
    HttpProtocolParams::GetHttpElementCharset(params, &mCharset);
    mAscii = mCharset.EqualsIgnoreCase(IHTTP::US_ASCII) || mCharset.EqualsIgnoreCase(IHTTP::ASCII);
    params->GetInt32Parameter(ICoreConnectionPNames::MAX_LINE_LENGTH, -1, &mMaxLineLen);
    mMetrics = new HttpTransportMetricsImpl();
}

ECode AbstractSessionOutputBuffer::FlushBuffer()
{
    Int32 len;
    if (mBuffer->GetLength(&len), len > 0) {
        AutoPtr< ArrayOf<Byte> > buffer;
        mBuffer->GetBuffer((ArrayOf<Byte>**)&buffer);
        mOutstream->Write(buffer, 0, len);
        mBuffer->Clear();
        mMetrics->IncrementBytesTransferred(len);
    }
    return NOERROR;
}

ECode AbstractSessionOutputBuffer::Flush()
{
    FlushBuffer();
    return mOutstream->Flush();
}

ECode AbstractSessionOutputBuffer::Write(
    /* [in] */ ArrayOf<Byte>* b,
    /* [in] */ Int32 off,
    /* [in] */ Int32 len)
{
    if (b == NULL) {
        return NOERROR;
    }
    // Do not want to buffer largish chunks
    // if the byte array is larger then MAX_CHUNK
    // write it directly to the output stream
    Int32 capacity;
    if (len > MAX_CHUNK || (mBuffer->GetCapacity(&capacity), len > capacity)) {
        // flush the buffer
        FlushBuffer();
        // write directly to the out stream
        mOutstream->Write(b, off, len);
        mMetrics->IncrementBytesTransferred(len);
    }
    else {
        // Do not let the buffer grow unnecessarily
        mBuffer->GetCapacity(&capacity);
        Int32 length;
        mBuffer->GetLength(&length);
        Int32 freecapacity = capacity - length;
        if (len > freecapacity) {
            // flush the buffer
            FlushBuffer();
        }
        // buffer
        mBuffer->Append(b, off, len);
    }
    return NOERROR
}

ECode AbstractSessionOutputBuffer::Write(
    /* [in] */ ArrayOf<Byte>* b)
{
    if (b == NULL) {
        return NOERROR;
    }
    return Write(b, 0, b->GetLength());
}

ECode AbstractSessionOutputBuffer::Write(
    /* [in] */ Int32 b)
{
    Boolean isFull;
    if (mBuffer->IsFull(&isFull), isFull) {
        FlushBuffer();
    }
    mBuffer->Append(b);
    return NOERROR;
}

ECode AbstractSessionOutputBuffer::WriteLine(
    /* [in] */ const String& s)
{
    if (s.IsNull()) {
        return NOERROR;
    }
    if (s.GetLength() > 0) {
        return Write(s.GetBytes(/*mCharset*/));
    }
    return Write(CRLF);
}

ECode AbstractSessionOutputBuffer::WriteLine(
    /* [in] */ ICharArrayBuffer* s)
{
    Boolean isNull;
    if (s->IsNull(&isNull), isNull) {
        return NOERROR;
    }
    if (mAscii) {
        Int32 off = 0;
        Int32 remaining = s.GetLength();
        while (remaining > 0) {
            Int32 capacity, len;
            mBuffer->GetCapacity(&capacity);
            mBuffer->GetLength(&len);
            Int32 chunk = capacity - len;
            chunk = Math::Min(chunk, remaining);
            if (chunk > 0) {
                mBuffer->Append(s, off, chunk);
            }
            Boolean isFull
            if (mBuffer->IsFull(&isFull), isFull) {
                FlushBuffer();
            }
            off += chunk;
            remaining -= chunk;
        }
    }
    else {
        // This is VERY memory inefficient, BUT since non-ASCII charsets are
        // NOT meant to be used anyway, there's no point optimizing it
        String str;
        IObject::Probe(s)->ToString(&str);
        AutoPtr< ArrayOf<Byte> > tmp = str.GetBytes(/*this.charset*/);
        return Write(tmp);
    }
    return Write(CRLF);
}

ECode AbstractSessionOutputBuffer::GetMetrics(
    /* [out] */ IHttpTransportMetrics** metrics)
{
    VALIDATE_NOT_NULL(metrics)
    *metrics = (IHttpTransportMetrics*)mMetrics;
    REFCOUNT_ADD(*metrics)
    return NOERROR;
}

} // namespace IO
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org