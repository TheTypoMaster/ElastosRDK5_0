
#ifndef __ELASTOS_DROID_MEDIA_CMETADATAHELPER_H__
#define __ELASTOS_DROID_MEDIA_CMETADATAHELPER_H__

#include "_Elastos_Droid_Media_CMetadataHelper.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Media {

CarClass(CMetadataHelper)
{
public:
        /**
         * @return the last available system metadata id. Ids are
         *         1-indexed.
         * {@hide}
         */
        CARAPI LastSytemId(
            /* [out] */ Int32* result);

        /**
         * @return the first available cutom metadata id.
         * {@hide}
         */
        CARAPI FirstCustomId(
            /* [out] */ Int32* result);

        /**
         * @return the last value of known type. Types are 1-indexed.
         * {@hide}
         */
        CARAPI LastType(
            /* [out] */ Int32* result);
};

} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_MEDIA_CMETADATAHELPER_H__
