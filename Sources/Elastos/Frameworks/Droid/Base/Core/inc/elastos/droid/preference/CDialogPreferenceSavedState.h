
#ifndef __ELASTOS_DROID_PREFERENCE_CDIALOGPREFERENCESAVEDSTATE_H__
#define __ELASTOS_DROID_PREFERENCE_CDIALOGPREFERENCESAVEDSTATE_H__

#include "_Elastos_Droid_Preference_CDialogPreferenceSavedState.h"
#include "PreferenceBaseSavedState.h"

using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Preference {

CarClass(CDialogPreferenceSavedState), public PreferenceBaseSavedState
{
public:
    CDialogPreferenceSavedState();

    CARAPI IsDialogShowing(
        /* [out] */ Boolean* isShow);

    CARAPI SetDialogShowing(
        /* [in] */ Boolean isShow);

    CARAPI GetDialogBundle(
        /* [out] */ IBundle** bundle);

    CARAPI SetDialogBundle(
        /* [in] */ IBundle* bundle);

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
    Boolean mIsDialogShowing;
    AutoPtr<IBundle> mDialogBundle;
};

}
}
}

#endif // __ELASTOS_DROID_PREFERENCE_CDIALOGPREFERENCESAVEDSTATE_H__
