
#include "CModifiedUtf8.h"
#include <coredef.h>
#include <stdio.h>

namespace Elastos {
namespace IO {
namespace Charset {

ECode CModifiedUtf8::Decode(
    /* [in] */ ArrayOf<Byte>* inBytes,
    /* [in] */ ArrayOf<Char8>* outChars,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 utfSize,
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(inBytes);
    VALIDATE_NOT_NULL(outChars);
    VALIDATE_NOT_NULL(string);
    if (utfSize <= 0)
        return E_ILLEGAL_ARGUMENT_EXCEPTION;

    Int32 count = 0;
    Int32 s = 0;
    Int32 a;

    Byte*  inPtr    = inBytes->GetPayload();
    Char8* outPtr   = outChars->GetPayload();
    while (count < utfSize) {
        if ((outPtr[s] = inPtr[offset + count++]) < 128) {
            s++;
        } else if ((a = outPtr[s] & 0xe0) == 0xc0) {
            if (count >= utfSize) {
                printf("bad second byte at: %d \n", count);
                return E_UTF_DATA_FORMAT_EXCEPTION;
            }
            Int32 b = inPtr[offset + count++];
            if ((b & 0xC0) != 0x80) {
                // throw new UTFDataFormatException("bad second byte at " + (count - 1));
                return E_UTF_DATA_FORMAT_EXCEPTION;
            }
            outPtr[s++] = (((a & 0x1F) << 6) | (b & 0x3F));
        } else if ((a & 0xf0) == 0xe0) {
            if (count + 1 >= utfSize) {
                // throw new UTFDataFormatException("bad third byte at " + (count + 1));
                return E_UTF_DATA_FORMAT_EXCEPTION;
            }
            Int32 b = inPtr[offset + count++];
            Int32 c = inPtr[offset + count++];
            if (((b & 0xC0) != 0x80) || ((c & 0xC0) != 0x80)) {
                // throw new UTFDataFormatException("bad second or third byte at " + (count - 2));
                return E_UTF_DATA_FORMAT_EXCEPTION;
            }
            outPtr[s++] = (((a & 0x0F) << 12) | ((b & 0x3F) << 6) | (c & 0x3F));
        } else {
            // throw new UTFDataFormatException("bad byte at " + (count - 1));
            return E_UTF_DATA_FORMAT_EXCEPTION;
        }
    }

    *string = String((const char*)(inPtr + offset), utfSize);
    return NOERROR;
}

ECode CModifiedUtf8::CountBytes(
    /* [in] */  String* s,
    /* [in] */  Boolean shortLength,
    /* [out] */ Int64* num)
{
    VALIDATE_NOT_NULL(s);
    VALIDATE_NOT_NULL(num);
    *num = s->GetByteLength();
    return NOERROR;
}

ECode CModifiedUtf8::Encode(
    /* [in] */ ArrayOf<Byte>* dst,
    /* [in] */ Int32 offset,
    /* [in] */ String* s)
{
    VALIDATE_NOT_NULL(dst);
    VALIDATE_NOT_NULL(s);

    Int32 dstLen = dst->GetLength();
    Int32 srcLen = s->GetByteLength();
    if ((0 > offset) || (offset > dstLen)) {
        return E_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }

    if ((srcLen + offset) > dstLen) {
        return E_BUFFER_OVER_FLOW_EXCEPTION;
    }

    Byte* dstPtr = dst->GetPayload() + offset;
    const Byte* srcPtr = reinterpret_cast<const Byte*>(s->string());
    memcpy(dstPtr, srcPtr, srcLen);

    return NOERROR;
}

ECode CModifiedUtf8::Encode(
    /* [in] */ String* s,
    /* [out, callee] */ ArrayOf<Byte>** bytes)
{
    VALIDATE_NOT_NULL(s);
    VALIDATE_NOT_NULL(bytes);

    Int32 size = s->GetByteLength();
    AutoPtr<ArrayOf<Byte> > buf = ArrayOf<Byte>::Alloc(sizeof(Int16) + size);
    if (NULL == buf)
        return E_BUFFER_OVER_FLOW_EXCEPTION;

    Byte* dstPtr = buf->GetPayload();
    dstPtr[0] = ((Int16)size >> 8) & 0X00FF;
    dstPtr[1] = (Int16)size & 0X00FF;
    Byte* srcPtr = (Byte*)(s->string());
    dstPtr += sizeof(Int16);
    memcpy(dstPtr, srcPtr, size);

    *bytes = buf;
    INTERFACE_ADDREF(*bytes);
    return NOERROR;
}

} // namespace Charset
} // namespace IO
} // namespace Elastos
