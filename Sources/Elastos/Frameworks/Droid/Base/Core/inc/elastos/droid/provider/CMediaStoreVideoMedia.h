
#ifndef __ELASTOS_DROID_PROVIDER_CMEDIASTOREVIDEOMEDIA_H__
#define __ELASTOS_DROID_PROVIDER_CMEDIASTOREVIDEOMEDIA_H__

#include "_Elastos_Droid_Provider_CMediaStoreVideoMedia.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CMediaStoreVideoMedia)
{
public:
    /**
     * The content:// style URI for the internal storage.
     */
    CARAPI GetINTERNAL_CONTENT_URI(
        /* [out] */ IUri** uri);

    /**
     * The content:// style URI for the "primary" external storage
     * volume.
     */
    CARAPI GetEXTERNAL_CONTENT_URI(
        /* [out] */ IUri** uri);

    /**
     * Get the content:// style URI for the video media table on the
     * given volume.
     *
     * @param volumeName the name of the volume to get the URI for
     * @return the URI to the video media table on the given volume
     */
    CARAPI GetContentUri(
        /* [in] */ const String& volumeName,
        /* [out] */ IUri** uri);
};

} //namespace Provider
} //namespace Droid
} //namespace Elastos

#endif //__ELASTOS_DROID_PROVIDER_CMEDIASTOREVIDEOMEDIA_H__
