
#ifndef __ELASTOS_DROID_PREFERENCE_CEDITTEXTPREFERENCESAVEDSTATE_H__
#define __ELASTOS_DROID_PREFERENCE_CEDITTEXTPREFERENCESAVEDSTATE_H__

#include "_Elastos_Droid_Preference_CEditTextPreferenceSavedState.h"
#include "PreferenceBaseSavedState.h"

namespace Elastos {
namespace Droid {
namespace Preference {

CarClass(CEditTextPreferenceSavedState), public PreferenceBaseSavedState
{
public:
    CARAPI GetText(
        /* [out] */ String* text);

    CARAPI SetText(
        /* [in] */ const String& text);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI GetSuperState(
        /* [out] */ IParcelable** superState);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcelable* source);

private:
    String mText;
};

}
}
}

#endif // __ELASTOS_DROID_PREFERENCE_CEDITTEXTPREFERENCESAVEDSTATE_H__
