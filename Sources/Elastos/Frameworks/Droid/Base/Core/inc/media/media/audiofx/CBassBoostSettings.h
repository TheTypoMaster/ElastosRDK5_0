
#ifndef __ELASTOS_DROID_MEDIA_AUDIOFX_CBASSBOOSTSETTINGS_H__
#define __ELASTOS_DROID_MEDIA_AUDIOFX_CBASSBOOSTSETTINGS_H__

#include "_CBassBoostSettings.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Media {
namespace Audiofx {

/**
 * The Settings class regroups all bass boost parameters. It is used in
 * conjuntion with getProperties() and setProperties() methods to backup and restore
 * all parameters in a single call.
 */
CarClass(CBassBoostSettings)
{
public:
    CBassBoostSettings();

    CARAPI constructor();

    /**
     * Settings class constructor from a key=value; pairs formatted string. The string is
     * typically returned by Settings.toString() method.
     * @throws IllegalArgumentException if the string is not correctly formatted.
     */
    CARAPI constructor(
        /* [in] */ const String& settings);

    CARAPI GetStrength(
        /* [out] */ Int16* result);

    CARAPI SetStrength(
        /* [in] */ Int16 result);

    CARAPI ToString(
        /* [out] */ String* result);

public:
    Int16 mStrength;
};

} // namespace Audiofx
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_MEDIA_AUDIOFX_CBASSBOOSTSETTINGS_H__
