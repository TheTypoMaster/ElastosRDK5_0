
#include "URLDecoder.h"
#include "UriCodec.h"
#include "CCharsetHelper.h"

namespace Elastos {
namespace Net {

using Elastos::IO::Charset::ICharsetHelper;
using Elastos::IO::Charset::CCharsetHelper;
using Elastos::IO::Charset::ICharset;

ECode URLDecoder::Decode(
    /* [in] */ const String& s,
    /* [out] */ String* decodedS)
{
    VALIDATE_NOT_NULL(decodedS);
    AutoPtr<ICharset> charset;
    AutoPtr<ICharsetHelper> charsetHelper;
    CCharsetHelper::AcquireSingleton((ICharsetHelper**)&charsetHelper);
    charsetHelper->DefaultCharset((ICharset**)&charset);
    return UriCodec::DecodeEx(s, TRUE, charset, TRUE, decodedS);
}

ECode URLDecoder::Decode(
    /* [in] */ const String& s,
    /* [in] */  const String& encoding,
    /* [out] */ String* decodedS)
{
    VALIDATE_NOT_NULL(decodedS);
    AutoPtr<ICharset> charset;
    AutoPtr<ICharsetHelper> charsetHelper;
    CCharsetHelper::AcquireSingleton((ICharsetHelper**)&charsetHelper);
    charsetHelper->ForName(encoding, (ICharset**)&charset);
    return UriCodec::DecodeEx(s, TRUE, charset, TRUE, decodedS);
}

} // namespace Net
} // namespace Elastos
