
#ifndef __ELASTOS_DROID_CONTENT_CSYNCADAPTERTYPEHELPER_H__
#define __ELASTOS_DROID_CONTENT_CSYNCADAPTERTYPEHELPER_H__

#include "_Elastos_Droid_Content_CSyncAdapterTypeHelper.h"

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CSyncAdapterTypeHelper)
{
public:
    CARAPI NewKey(
        /* [in] */ const String& authority,
        /* [in] */ const String& accountType,
        /* [out] */ ISyncAdapterType** syncAdapterType);

private:
};

}
}
}

#endif // __ELASTOS_DROID_CONTENT_CSYNCADAPTERTYPEHELPER_H__
