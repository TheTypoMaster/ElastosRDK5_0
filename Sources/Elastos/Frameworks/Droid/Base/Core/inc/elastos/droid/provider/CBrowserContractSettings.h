
#ifndef __ELASTOS_DROID_PROVIDER_CBROWSERCONTRACTSETTINGS_H__
#define __ELASTOS_DROID_PROVIDER_CBROWSERCONTRACTSETTINGS_H__

#include "_Elastos_Droid_Provider_CBrowserContractSettings.h"

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CBrowserContractSettings)
{
public:
    /**
     * This utility class cannot be instantiated
     */
    CARAPI constructor();

    /**
     * The content:// style URI for this table
     */
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);

    /**
     * Returns true if bookmark sync is enabled
     */
    CARAPI IsSyncEnabled(
        /* [in] */ IContext* context,
        /* [out] */ Boolean* res);

    /**
     * Sets the bookmark sync enabled setting.
     */
    CARAPI SetSyncEnabled(
        /* [in] */ IContext* context,
        /* [in] */ Boolean enabled);
};

}
}
}

#endif //__ELASTOS_DROID_PROVIDER_CBROWSERCONTRACTSETTINGS_H__
