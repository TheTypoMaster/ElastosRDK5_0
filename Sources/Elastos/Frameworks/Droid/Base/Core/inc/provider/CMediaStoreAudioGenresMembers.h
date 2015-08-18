
#ifndef __ELASTOS_DROID_PROVIDER_CMEDIASTOREAUDIOGENRESMEMBERS_H__
#define __ELASTOS_DROID_PROVIDER_CMEDIASTOREAUDIOGENRESMEMBERS_H__

#include "_Elastos_Droid_Provider_CMediaStoreAudioGenresMembers.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CMediaStoreAudioGenresMembers)
{
public:
    CARAPI GetContentUri(
        /* [in] */ const String& volumeName,
        /* [in] */ Int64 genreId,
        /* [out] */ IUri** uri);
};

}
}
}

#endif //__ELASTOS_DROID_PROVIDER_CMEDIASTOREAUDIOGENRESMEMBERS_H__
