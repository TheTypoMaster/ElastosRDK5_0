
#ifndef __ELASTOS_DROID_WEBKIT_CONTENT_PUBLIC_REFERRER_H__
#define __ELASTOS_DROID_WEBKIT_CONTENT_PUBLIC_REFERRER_H__

#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content_Public {

/**
 * Container that holds together a referrer URL along with the referrer policy set on the
 * originating frame. This corresponds to native content/public/common/referrer.h.
 */
class Referrer
{
public:
    Referrer(String url, int policy);

    CARAPI_(String) GetUrl();

    CARAPI_(Int32) GetPolicy();

private:
    const String mUrl;
    const Int32 mPolicy;
};

} // namespace Content_Public
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_CONTENT_PUBLIC_REFERRER_H__
