#ifndef __CHARSETICU_H__
#define __CHARSETICU_H__

#include "Charset.h"

namespace Elastos {
namespace IO {
namespace Charset {

class CharsetICU
    : public Charset
{
public:
    CharsetICU();

    CARAPI Init(
        /* [in] */ const String& canonicalName,
        /* [in] */ const String& icuCanonName,
        /* [in] */ ArrayOf<String>* aliases);

    CARAPI NewEncoder(
        /* [out] */ ICharsetEncoder** charsetEncoder);

    CARAPI NewDecoder(
        /* [out] */ ICharsetDecoder** charsetDecoder);

    CARAPI Contains(
        /* [in] */ ICharset* charset,
        /* [out] */ Boolean* result);

    CARAPI GetName(
        /* [out] */ String* name);

private:
    String mIcuCanonicalName;
};

} // namespace Charset
} // namespace IO
} // namespace Elastos

#endif //__CHARSETICU_H__