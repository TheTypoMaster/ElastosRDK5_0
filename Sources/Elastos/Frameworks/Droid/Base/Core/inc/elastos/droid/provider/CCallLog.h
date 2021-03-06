
#ifndef __ELASTOS_DROID_PROVIDER_CCALLLOG_H__
#define __ELASTOS_DROID_PROVIDER_CCALLLOG_H__

#include "_Elastos_Droid_Provider_CCallLog.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CCallLog)
{
public:
    /**
     * The content:// style URL for this provider
     */
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);

public:
    static AutoPtr<IUri> CONTENT_URI;
};

} //Provider
} //Droid
} //Elastos

#endif //__ELASTOS_DROID_PROVIDER_CCALLLOG_H__
