
#include "SoftKeyToggle.h"
#include "SkbTemplate.h"

namespace Elastos {
namespace Droid {
namespace Inputmethods {
namespace PinyinIME {

const Int32 SoftKeyToggle::KEYMASK_TOGGLE_STATE = 0x000000ff;

SoftKeyToggle::ToggleState::ToggleState()
    : mIdAndFlags(0)
    , mKeyCode(0)
{
}

void SoftKeyToggle::ToggleState::SetStateId(
    /* [in] */ Int32 stateId)
{
    mIdAndFlags |= (stateId & KEYMASK_TOGGLE_STATE);
}

void SoftKeyToggle::ToggleState::SetStateFlags(
    /* [in] */ Boolean repeat,
    /* [in] */ Boolean balloon)
{
    if (repeat) {
        mIdAndFlags |= KEYMASK_REPEAT;
    } else {
        mIdAndFlags &= (~KEYMASK_REPEAT);
    }

    if (balloon) {
        mIdAndFlags |= KEYMASK_BALLOON;
    } else {
        mIdAndFlags &= (~KEYMASK_BALLOON);
    }
}

UInt32 SoftKeyToggle::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 SoftKeyToggle::Release()
{
    return ElRefBase::Release();
}

PInterface SoftKeyToggle::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (ISoftKeyToggle*)this;
    }
    else if (riid == EIID_ISoftKeyToggle) {
        return (ISoftKeyToggle*)this;
    }
    else if (riid == EIID_ISoftKey) {
        return (ISoftKeyToggle*)this;
    }

    return NULL;
}

ECode SoftKeyToggle::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(ISoftKeyToggle *)this) {
        *pIID = EIID_ISoftKeyToggle;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

Int32 SoftKeyToggle::GetToggleStateId()
{
    return (mKeyMask & KEYMASK_TOGGLE_STATE);
}

Boolean SoftKeyToggle::EnableToggleState(
    /* [in] */ Int32 stateId,
    /* [in] */ Boolean resetIfNotFound)
{
    Int32 oldStateId = (mKeyMask & KEYMASK_TOGGLE_STATE);
    if (oldStateId == stateId) return FALSE;

    mKeyMask &= (~KEYMASK_TOGGLE_STATE);
    if (stateId > 0) {
        mKeyMask |= (KEYMASK_TOGGLE_STATE & stateId);
        if (GetToggleState() == NULL) {
            mKeyMask &= (~KEYMASK_TOGGLE_STATE);
            if (!resetIfNotFound && oldStateId > 0) {
                mKeyMask |= (KEYMASK_TOGGLE_STATE & oldStateId);
            }
            return resetIfNotFound;
        } else {
            return TRUE;
        }
    } else {
        return TRUE;
    }
}

Boolean SoftKeyToggle::DisableToggleState(
    /* [in] */ Int32 stateId,
    /* [in] */ Boolean resetIfNotFound)
{
    Int32 oldStateId = (mKeyMask & KEYMASK_TOGGLE_STATE);
    if (oldStateId == stateId) {
        mKeyMask &= (~KEYMASK_TOGGLE_STATE);
        return stateId != 0;
    }

    if (resetIfNotFound) {
        mKeyMask &= (~KEYMASK_TOGGLE_STATE);
        return oldStateId != 0;
    }
    return FALSE;
}

Boolean SoftKeyToggle::DisableAllToggleStates()
{
    Int32 oldStateId = (mKeyMask & KEYMASK_TOGGLE_STATE);
    mKeyMask &= (~KEYMASK_TOGGLE_STATE);
    return oldStateId != 0;
}

AutoPtr<IDrawable> SoftKeyToggle::GetKeyIcon()
{
    AutoPtr<ToggleState> state = GetToggleState();
    if (NULL != state) return state->mKeyIcon;
    return SoftKey::GetKeyIcon();
}

AutoPtr<IDrawable> SoftKeyToggle::GetKeyIconPopup()
{
    AutoPtr<ToggleState> state = GetToggleState();
    if (NULL != state) {
        if (NULL != state->mKeyIconPopup) {
            return state->mKeyIconPopup;
        } else {
            return state->mKeyIcon;
        }
    }
    return SoftKey::GetKeyIconPopup();
}

ECode SoftKeyToggle::GetKeyCode(
    /* [out] */ Int32* code)
{
    VALIDATE_NOT_NULL(code);
    AutoPtr<ToggleState> state = GetToggleState();
    if (NULL != state) {
        *code = state->mKeyCode;
        return NOERROR;
    }
    *code = mKeyCode;
    return NOERROR;
}

ECode SoftKeyToggle::GetKeyLabel(
    /* [out] */ String* label)
{
    VALIDATE_NOT_NULL(label);
    AutoPtr<ToggleState> state = GetToggleState();
    if (NULL != state) {
        *label = state->mKeyLabel;
        return NOERROR;
    }
    *label = mKeyLabel;
    return NOERROR;
}

AutoPtr<IDrawable> SoftKeyToggle::GetKeyBg()
{
    AutoPtr<ToggleState> state = GetToggleState();
    if (NULL != state && NULL != state->mKeyType) {
        return state->mKeyType->mKeyBg;
    }
    return mKeyType->mKeyBg;
}

AutoPtr<IDrawable> SoftKeyToggle::GetKeyHlBg()
{
    AutoPtr<ToggleState> state = GetToggleState();
    if (NULL != state && NULL != state->mKeyType) {
        return state->mKeyType->mKeyHlBg;
    }
    return mKeyType->mKeyHlBg;
}

Int32 SoftKeyToggle::GetColor()
{
    AutoPtr<ToggleState> state = GetToggleState();
    if (NULL != state && NULL != state->mKeyType) {
        return state->mKeyType->mColor;
    }
    return mKeyType->mColor;
}

Int32 SoftKeyToggle::GetColorHl()
{
    AutoPtr<ToggleState> state = GetToggleState();
    if (NULL != state && NULL != state->mKeyType) {
        return state->mKeyType->mColorHl;
    }
    return mKeyType->mColorHl;
}

Int32 SoftKeyToggle::GetColorBalloon()
{
    AutoPtr<ToggleState> state = GetToggleState();
    if (NULL != state && NULL != state->mKeyType) {
        return state->mKeyType->mColorBalloon;
    }
    return mKeyType->mColorBalloon;
}

ECode SoftKeyToggle::IsKeyCodeKey(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ToggleState> state = GetToggleState();
    if (NULL != state) {
        if (state->mKeyCode > 0) {
            *result = TRUE;
            return NOERROR;
        }
        *result = FALSE;
        return NOERROR;
    }
    return SoftKey::IsKeyCodeKey(result);
}

ECode SoftKeyToggle::IsUserDefKey(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ToggleState> state = GetToggleState();
    if (NULL != state) {
        if (state->mKeyCode < 0) {
            *result = TRUE;
            return NOERROR;
        }
        *result = FALSE;
        return NOERROR;
    }
    return SoftKey::IsUserDefKey(result);
}

ECode SoftKeyToggle::IsUniStrKey(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<ToggleState> state = GetToggleState();
    if (NULL != state) {
        if (NULL != state->mKeyLabel && state->mKeyCode == 0) {
            *result = TRUE;
            return NOERROR;
        }
        *result = FALSE;
        return NOERROR;
    }
    return SoftKey::IsUniStrKey(result);
}

Boolean SoftKeyToggle::NeedBalloon()
{
    AutoPtr<ToggleState> state = GetToggleState();
    if (NULL != state) {
        return (state->mIdAndFlags & KEYMASK_BALLOON) != 0;
    }
    return SoftKey::NeedBalloon();
}

Boolean SoftKeyToggle::Repeatable()
{
    AutoPtr<ToggleState> state = GetToggleState();
    if (NULL != state) {
        return (state->mIdAndFlags & KEYMASK_REPEAT) != 0;
    }
    return SoftKey::Repeatable();
}

void SoftKeyToggle::ChangeCase(
    /* [in] */ Boolean lowerCase)
{
    AutoPtr<ToggleState> state = GetToggleState();
    if (NULL != state && NULL != state->mKeyLabel) {
        if (lowerCase)
            state->mKeyLabel = state->mKeyLabel.ToLowerCase();
        else
            state->mKeyLabel = state->mKeyLabel.ToUpperCase();
    }
}

AutoPtr<SoftKeyToggle::ToggleState> SoftKeyToggle::CreateToggleState()
{
    return new ToggleState();
}

Boolean SoftKeyToggle::SetToggleStates(
    /* [in] */ ToggleState* rootState)
{
    if (NULL == rootState) return FALSE;
    mToggleState = rootState;
    return TRUE;
}

AutoPtr<SoftKeyToggle::ToggleState> SoftKeyToggle::GetToggleState()
{
    Int32 stateId = (mKeyMask & KEYMASK_TOGGLE_STATE);
    if (0 == stateId) return NULL;

    AutoPtr<ToggleState> state = mToggleState;
    while ((NULL != state)
            && (state->mIdAndFlags & KEYMASK_TOGGLE_STATE) != stateId) {
        state = state->mNextState;
    }
    return state;
}


} // namespace PinyinIME
} // namespace Inputmethods
} // namespace Droid
} // namespace Elastos
