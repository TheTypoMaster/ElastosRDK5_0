
#ifndef __ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACTDISPLAYPHOTO_H__
#define __ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACTDISPLAYPHOTO_H__

#include "_Elastos_Droid_Provider_CContactsContractDisplayPhoto.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CContactsContractDisplayPhoto)
{
public:
    /**
     * no public constructor since this is a utility class
     */
    CARAPI constructor();

    /**
     * The content:// style URI for this class, which allows access to full-size photos,
     * given a key.
     */
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);

    /**
     * This URI allows the caller to query for the maximum dimensions of a display photo
     * or thumbnail.  Requests to this URI can be performed on the UI thread because
     * they are always unblocking.
     */
    CARAPI GetCONTENTMAXDIMENSIONSURI(
        /* [out] */ IUri** uri);
};

} //Provider
} //Droid
} //Elastos

#endif //__ELASTOS_DROID_PROVIDER_CCONTACTSCONTRACTDISPLAYPHOTO_H__
