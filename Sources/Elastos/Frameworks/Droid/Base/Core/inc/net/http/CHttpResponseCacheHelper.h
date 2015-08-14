
#ifndef __ELASTOS_DROID_NET_HTTP_CHTTPRESPONSECACHEHELPER_H__
#define __ELASTOS_DROID_NET_HTTP_CHTTPRESPONSECACHEHELPER_H__

#include "_CHttpResponseCacheHelper.h"

using Elastos::Droid::Net::Http::IHttpResponseCache;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Http {

CarClass(CHttpResponseCacheHelper)
{
public:
    /**
     * Returns the currently-installed {@code HttpResponseCache}, or null if
     * there is no cache installed or it is not a {@code HttpResponseCache}.
     */
    CARAPI GetInstalled(
        /* [out] */ IHttpResponseCache** cache);

    CARAPI Install(
        /* [in] */ Elastos::IO::IFile* directory,
        /* [in] */ Int64 maxSize,
        /* [out] */ IHttpResponseCache** cache);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}

#endif // __ELASTOS_DROID_NET_HTTP_CHTTPRESPONSECACHEHELPER_H__
