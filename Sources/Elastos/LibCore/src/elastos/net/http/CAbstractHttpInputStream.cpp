
#include "CAbstractHttpInputStream.h"
#include <cmdef.h>

namespace Elastos {
namespace Net {
namespace Http {

ECode CAbstractHttpInputStream::constructor(
    /* [in] */ IInputStream* in,
    /* [in] */ IHttpEngine* httpEngine,
    /* [in] */ ICacheRequest* cacheRequest)
{
    return AbstractHttpInputStream::Init(in, httpEngine, cacheRequest);
}

ECode CAbstractHttpInputStream::Available(
    /* [out] */ Int32* number)
{
    return AbstractHttpInputStream::Available(number);
}

ECode CAbstractHttpInputStream::Mark(
    /* [in] */ Int32 readLimit)
{
    return AbstractHttpInputStream::Mark(readLimit);
}

ECode CAbstractHttpInputStream::IsMarkSupported(
    /* [out] */ Boolean* supported)
{
    return AbstractHttpInputStream::IsMarkSupported(supported);
}

ECode CAbstractHttpInputStream::Read(
    /* [out] */ Int32* value)
{
    return AbstractHttpInputStream::Read(value);
}

ECode CAbstractHttpInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [out] */ Int32* number)
{
    return AbstractHttpInputStream::ReadBytes(buffer, number);
}

ECode CAbstractHttpInputStream::ReadBytes(
    /* [out] */ ArrayOf<Byte>* buffer,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [out] */ Int32* number)
{
    return AbstractHttpInputStream::ReadBytes(buffer, offset, length, number);
}

ECode CAbstractHttpInputStream::Reset()
{
    return AbstractHttpInputStream::Reset();
}

ECode CAbstractHttpInputStream::Skip(
    /* [in] */ Int64 byteCount,
    /* [out] */ Int64* number)
{
    return AbstractHttpInputStream::Skip(byteCount, number);
}

ECode CAbstractHttpInputStream::Close()
{
    return AbstractHttpInputStream::Close();
}


} // namespace Http
} // namespace Net
} // namespace Elastos
