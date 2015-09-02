
#ifndef __ELASTOS_DROID_CONTENT_CCLIPDESCRIPTIONHELPER_H__
#define __ELASTOS_DROID_CONTENT_CCLIPDESCRIPTIONHELPER_H__

#include "_Elastos_Droid_Content_CClipDescriptionHelper.h"
#include <elastos/core/Singleton.h>

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CClipDescriptionHelper)
    , public Singleton
    , public IClipDescriptionHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    /**
     * Helper to compare two MIME types, where one may be a pattern.
     * @param concreteType A fully-specified MIME type.
     * @param desiredType A desired MIME type that may be a pattern such as * \/ *.
     * @return Returns true if the two MIME types match.
     */
    CARAPI CompareMimeTypes(
        /* [in] */ const String& concreteType,
        /* [in] */ const String& desiredType,
        /* [out] */ Boolean* isMatch);
};

}
}
}

#endif // __ELASTOS_DROID_CONTENT_CCLIPDESCRIPTIONHELPER_H__
