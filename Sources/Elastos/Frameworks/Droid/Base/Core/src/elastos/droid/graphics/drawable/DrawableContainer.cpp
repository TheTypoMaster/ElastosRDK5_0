
#include "ext/frameworkext.h"
#include "graphics/drawable/DrawableContainer.h"
#include "graphics/CRect.h"
#include "graphics/Insets.h"
#include "os/SystemClock.h"

using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Utility::ILayoutDirection;
using Elastos::Core::EIID_IRunnable;

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

const Boolean DrawableContainer::DEFAULT_DITHER;
const String DrawableContainer::TAG("DrawableContainer");

CAR_INTERFACE_IMPL_2(DrawableContainer, Drawable, IDrawableContainer, IDrawableCallback)
DrawableContainer::DrawableContainer()
    : mAlpha(0xFF)
    , mHasAlpha(FALSE)
    , mCurIndex(-1)
    , mMutated(FALSE)
    , mEnterAnimationEnd(0)
    , mExitAnimationEnd(0)
{
}

ECode DrawableContainer::Draw(
    /* [in] */ ICanvas* canvas)
{
    if (mCurrDrawable != NULL) {
        mCurrDrawable->Draw(canvas);
    }
    if (mLastDrawable != NULL) {
        mLastDrawable->Draw(canvas);
    }
    return NOERROR;
}

ECode DrawableContainer::GetChangingConfigurations(
    /* [out] */ Int32* configuration)
{
    VALIDATE_NOT_NULL(configuration);
    Drawable::GetChangingConfigurations(configuration);
    *configuration = (*configuration)
            | mDrawableContainerState->mChangingConfigurations
            | mDrawableContainerState->mChildrenChangingConfigurations;
    return NOERROR;
}

Boolean DrawableContainer::NeedsMirroring()
{
    Int32 dir = 0;
    Boolean mirrored = FALSE;
    IsAutoMirrored(&mirrored);
    return mirrored && (GetLayoutDirection(&dir), dir) == ILayoutDirection::RTL;
}

ECode DrawableContainer::GetPadding(
    /* [in] */ IRect* padding,
    /* [out] */ Boolean* isPadding)
{
    VALIDATE_NOT_NULL(isPadding);
    AutoPtr<IRect> r = mDrawableContainerState->GetConstantPadding();
    Boolean result = FALSE;
    if (r != NULL) {
        padding->Set(r);
        Int32 left = 0, top = 0, bottom = 0, right = 0;
        r->Get(&left, &top, &right, &bottom);
        result = (left | top | bottom | right) != 0;
    } else {
        if (mCurrDrawable != NULL) {
            mCurrDrawable->GetPadding(padding, &result);
        } else {
            Drawable::GetPadding(padding, &result);
        }
    }
    if (NeedsMirroring()) {
        Int32 left = 0;
        padding->GetLeft(&left);
        Int32 right = 0;
        padding->GetRight(&right);
        padding->SetLeft(right);
        padding->SetRight(left);
    }
    *isPadding = result;
    return NOERROR;
}

ECode DrawableContainer::GetOpticalInsets(
    /* [out] */ IInsets** sets)
{
    VALIDATE_NOT_NULL(sets);
    if (mCurrDrawable != NULL) {
        return ((Drawable*)mCurrDrawable.Get())->GetOpticalInsets(sets);
    }

    assert(0 && "TODO");
    // *sets = Insets::NONE;
    REFCOUNT_ADD(*sets);
    return NOERROR;
}

ECode DrawableContainer::GetOutline(
    /* [in] */ /*@NonNull*/ IOutline* outline)
{
    if (mCurrDrawable != NULL) {
        mCurrDrawable->GetOutline(outline);
    }
    return NOERROR;
}

ECode DrawableContainer::SetAlpha(
    /* [in] */ Int32 alpha)
{
    if (!mHasAlpha || mAlpha != alpha) {
        mHasAlpha = TRUE;
        mAlpha = alpha;
        if (mCurrDrawable != NULL) {
            if (mEnterAnimationEnd == 0) {
                AutoPtr<IDrawable> dr;
                mCurrDrawable->Mutate((IDrawable**)&dr);
                dr->SetAlpha(alpha);
            }
            else {
                Animate(FALSE);
            }
        }
    }
    return NOERROR;
}

ECode DrawableContainer::GetAlpha(
    /* [out] */ Int32* alpha)
{
    VALIDATE_NOT_NULL(alpha);
    *alpha = mAlpha;
    return NOERROR;
}

ECode DrawableContainer::SetDither(
    /* [in] */ Boolean dither)
{
    if (mDrawableContainerState->mDither != dither) {
        mDrawableContainerState->mDither = dither;
        if (mCurrDrawable != NULL) {
            AutoPtr<IDrawable> dr;
            mCurrDrawable->Mutate((IDrawable**)&dr);
            dr->SetDither(mDrawableContainerState->mDither);
        }
    }
    return NOERROR;
}

ECode DrawableContainer::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    mDrawableContainerState->mHasColorFilter = (cf != NULL);

    if (mDrawableContainerState->mColorFilter.Get() != cf) {
        mDrawableContainerState->mColorFilter = cf;
        if (mCurrDrawable != NULL) {
            AutoPtr<IDrawable> dr;
            mCurrDrawable->Mutate((IDrawable**)&dr);
            dr->SetColorFilter(cf);
        }
    }
    return NOERROR;
}

ECode DrawableContainer::SetTintList(
    /* [in] */ IColorStateList* tint)
{
    mDrawableContainerState->mHasTintList = TRUE;

    if (mDrawableContainerState->mTintList.Get() != tint) {
        mDrawableContainerState->mTintList = tint;

        if (mCurrDrawable != NULL) {
            AutoPtr<IDrawable> drawable;
            mCurrDrawable->Mutate((IDrawable**)&drawable);
            drawable->SetTintList(tint);
        }
    }
    return NOERROR;
}

ECode DrawableContainer::SetTintMode(
    /* [in] */ PorterDuffMode tintMode)
{
    mDrawableContainerState->mHasTintMode = TRUE;

    if (mDrawableContainerState->mTintMode != tintMode) {
        mDrawableContainerState->mTintMode = tintMode;

        if (mCurrDrawable != NULL) {
            AutoPtr<IDrawable> drawable;
            mCurrDrawable->Mutate((IDrawable**)&drawable);
            drawable->SetTintMode(tintMode);
        }
    }
    return NOERROR;
}

ECode DrawableContainer::SetEnterFadeDuration(
    /* [in] */ Int32 ms)
{
    mDrawableContainerState->mEnterFadeDuration = ms;
    return NOERROR;
}

ECode DrawableContainer::SetExitFadeDuration(
    /* [in] */ Int32 ms)
{
    mDrawableContainerState->mExitFadeDuration = ms;
    return NOERROR;
}

void DrawableContainer::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    if (mLastDrawable != NULL) {
        mLastDrawable->SetBounds(bounds);
    }
    if (mCurrDrawable != NULL) {
        mCurrDrawable->SetBounds(bounds);
    }
}

ECode DrawableContainer::IsStateful(
    /* [out] */ Boolean* isStateful)
{
    VALIDATE_NOT_NULL(isStateful);
    *isStateful = mDrawableContainerState->IsStateful();
    return NOERROR;
}

ECode DrawableContainer::SetAutoMirrored(
    /* [in] */ Boolean mirrored)
{
    if (mDrawableContainerState->mAutoMirrored != mirrored) {
        mDrawableContainerState->mAutoMirrored = mirrored;
        if (mCurrDrawable != NULL) {
            AutoPtr<IDrawable> drawable;
            mCurrDrawable->Mutate((IDrawable**)&drawable);
            drawable->SetAutoMirrored(mDrawableContainerState->mAutoMirrored);
        }
    }
    return NOERROR;
}

ECode DrawableContainer::IsAutoMirrored(
    /* [out] */ Boolean* mirrored)
{
    VALIDATE_NOT_NULL(mirrored);
    *mirrored = mDrawableContainerState->mAutoMirrored;
    return NOERROR;
}

ECode DrawableContainer::JumpToCurrentState()
{
    Boolean changed = FALSE;
    if (mLastDrawable != NULL) {
        mLastDrawable->JumpToCurrentState();
        mLastDrawable = NULL;
        changed = TRUE;
    }
    if (mCurrDrawable != NULL) {
        mCurrDrawable->JumpToCurrentState();
        if (mHasAlpha) {
            AutoPtr<IDrawable> dr;
            mCurrDrawable->Mutate((IDrawable**)&dr);
            dr->SetAlpha(mAlpha);
        }
    }
    if (mExitAnimationEnd != 0) {
        mExitAnimationEnd = 0;
        changed = TRUE;
    }
    if (mEnterAnimationEnd != 0) {
        mEnterAnimationEnd = 0;
        changed = TRUE;
    }
    if (changed) {
        InvalidateSelf();
    }
    return NOERROR;
}

ECode DrawableContainer::SetHotspot(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    if (mCurrDrawable != NULL) {
        mCurrDrawable->SetHotspot(x, y);
    }
    return NOERROR;
}

ECode DrawableContainer::SetHotspotBounds(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    if (mHotspotBounds == NULL) {
        CRect::New(left, top, bottom, right, (IRect**)&mHotspotBounds);
    } else {
        mHotspotBounds->Set(left, top, bottom, right);
    }

    if (mCurrDrawable != NULL) {
        mCurrDrawable->SetHotspotBounds(left, top, right, bottom);
    }
    return NOERROR;
}

ECode DrawableContainer::GetHotspotBounds(
    /* [in] */ IRect* outRect)
{
    if (mHotspotBounds != NULL) {
        outRect->Set(mHotspotBounds);
    } else {
        Drawable::GetHotspotBounds(outRect);
    }
    return NOERROR;
}

Boolean DrawableContainer::OnStateChange(
    /* [in] */ const ArrayOf<Int32>* state)
{
    if (mLastDrawable != NULL) {
        Boolean isStateful;
        mLastDrawable->SetState(const_cast<ArrayOf<Int32>*>(state), &isStateful);
        return isStateful;
    }
    if (mCurrDrawable != NULL) {
        Boolean isStateful;
        mCurrDrawable->SetState(const_cast<ArrayOf<Int32>*>(state), &isStateful);
        return isStateful;
    }
    return FALSE;
}

Boolean DrawableContainer::OnLevelChange(
    /* [in] */ Int32 level)
{
    if (mLastDrawable != NULL) {
        Boolean changed;
        mLastDrawable->SetLevel(level, &changed);
        return changed;
    }
    if (mCurrDrawable != NULL) {
        Boolean changed;
        mCurrDrawable->SetLevel(level, &changed);
        return changed;
    }
    return FALSE;
}

ECode DrawableContainer::GetIntrinsicWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    if (mDrawableContainerState->IsConstantSize()) {
        *width = mDrawableContainerState->GetConstantWidth();
        return NOERROR;
    }
    *width = -1;
    if (mCurrDrawable != NULL) {
        mCurrDrawable->GetIntrinsicWidth(width);
    }
    return NOERROR;
}

ECode DrawableContainer::GetIntrinsicHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    if (mDrawableContainerState->IsConstantSize()) {
        *height = mDrawableContainerState->GetConstantHeight();
        return NOERROR;
    }
    *height = -1;
    if (mCurrDrawable != NULL) {
        mCurrDrawable->GetIntrinsicHeight(height);
    }
    return NOERROR;
}

ECode DrawableContainer::GetMinimumWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    if (mDrawableContainerState->IsConstantSize()) {
        *width = mDrawableContainerState->GetConstantMinimumWidth();
        return NOERROR;
    }
    *width = 0;
    if (mCurrDrawable != NULL) {
        mCurrDrawable->GetMinimumWidth(width);
    }
    return NOERROR;
}

ECode DrawableContainer::GetMinimumHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    if (mDrawableContainerState->IsConstantSize()) {
        *height = mDrawableContainerState->GetConstantMinimumHeight();
        return NOERROR;
    }
    *height = 0;
    if (mCurrDrawable != NULL) {
        mCurrDrawable->GetMinimumHeight(height);
    }
    return NOERROR;
}

ECode DrawableContainer::InvalidateDrawable(
    /* [in] */ IDrawable* who)
{
    AutoPtr<IDrawableCallback> callback;
    GetCallback((IDrawableCallback**)&callback);
    if (who == mCurrDrawable.Get() && callback != NULL) {
        callback->InvalidateDrawable((IDrawable*)this->Probe(EIID_IDrawable));
    }
    return NOERROR;
}

ECode DrawableContainer::ScheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what,
    /* [in] */ Int64 when)
{
    AutoPtr<IDrawableCallback> callback;
    GetCallback((IDrawableCallback**)&callback);
    if (who == mCurrDrawable.Get() && callback != NULL) {
        callback->ScheduleDrawable((IDrawable*)this->Probe(EIID_IDrawable), what, when);
    }
    return NOERROR;
}

ECode DrawableContainer::UnscheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what)
{
    AutoPtr<IDrawableCallback> callback;
    GetCallback((IDrawableCallback**)&callback);
    if (who == mCurrDrawable && callback != NULL) {
        callback->UnscheduleDrawable((IDrawable*)this->Probe(EIID_IDrawable), what);
    }
    return NOERROR;
}

ECode DrawableContainer::SetVisible(
    /* [in] */ Boolean visible,
    /* [in] */ Boolean restart,
    /* [out] */ Boolean* isDifferent)
{
    VALIDATE_NOT_NULL(isDifferent);
    Boolean changed = FALSE;
    Drawable::SetVisible(visible, restart, &changed);
    if (mLastDrawable != NULL) {
        Boolean tmp;
        mLastDrawable->SetVisible(visible, restart, &tmp);
    }
    if (mCurrDrawable != NULL) {
        Boolean tmp;
        mCurrDrawable->SetVisible(visible, restart, &tmp);
    }
    *isDifferent = changed;
    return NOERROR;
}

ECode DrawableContainer::GetOpacity(
    /* [out] */ Int32* opacity)
{
    VALIDATE_NOT_NULL(opacity);
    Boolean visible = FALSE;
    *opacity = (mCurrDrawable == NULL || (mCurrDrawable->IsVisible(&visible), !visible)) ? IPixelFormat::TRANSPARENT :
            mDrawableContainerState->GetOpacity();
    return NOERROR;
}

ECode DrawableContainer::SetCurrentIndex(
    /* [in] */ Int32 index)
{
    Boolean res = FALSE;
    return SelectDrawable(index, &res);
}

ECode DrawableContainer::GetCurrentIndex(
    /* [in] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = mCurIndex;
    return NOERROR;
}

ECode DrawableContainer::SelectDrawable(
    /* [in] */ Int32 idx,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
    *res = FALSE;
    if (idx == mCurIndex) {
        return NOERROR;
    }

    Int64 now = SystemClock::GetUptimeMillis();

    // if (DEBUG) android.util.Log.i(TAG, toString() + " from " + mCurIndex + " to " + idx
    //         + ": exit=" + mDrawableContainerState.mExitFadeDuration
    //         + " enter=" + mDrawableContainerState.mEnterFadeDuration);

    if (mDrawableContainerState->mExitFadeDuration > 0) {
        if (mLastDrawable != NULL) {
            Boolean isDifferent;
            mLastDrawable->SetVisible(FALSE, FALSE, &isDifferent);
        }
        if (mCurrDrawable != NULL) {
            mLastDrawable = mCurrDrawable;
            mExitAnimationEnd = now + mDrawableContainerState->mExitFadeDuration;
        }
        else {
            mLastDrawable = NULL;
            mExitAnimationEnd = 0;
        }
    }
    else if (mCurrDrawable != NULL) {
        Boolean isDifferent;
        mCurrDrawable->SetVisible(FALSE, FALSE, &isDifferent);
    }

    Boolean isDifferent;
    if (idx >= 0 && idx < mDrawableContainerState->mNumChildren) {
        AutoPtr<IDrawable> d = mDrawableContainerState->GetChild(idx);
        mCurrDrawable = d;
        mCurIndex = idx;
        if (d != NULL) {
            AutoPtr<IDrawable> dr;
            d->Mutate((IDrawable**)&dr);
            if (mDrawableContainerState->mEnterFadeDuration > 0) {
                mEnterAnimationEnd = now + mDrawableContainerState->mEnterFadeDuration;
            }
            else if (mHasAlpha) {
                d->SetAlpha(mAlpha);
            }

            if (mDrawableContainerState->mHasColorFilter) {
                // Color filter always overrides tint.
                d->SetColorFilter(mDrawableContainerState->mColorFilter);
            } else {
                if (mDrawableContainerState->mHasTintList) {
                    d->SetTintList(mDrawableContainerState->mTintList);
                }
                if (mDrawableContainerState->mHasTintMode) {
                    d->SetTintMode(mDrawableContainerState->mTintMode);
                }
            }

            Boolean visible = FALSE;
            d->SetVisible((IsVisible(&visible), visible), TRUE, &isDifferent);
            d->SetDither(mDrawableContainerState->mDither);

            AutoPtr<ArrayOf<Int32> > states;
            GetState((ArrayOf<Int32>**)&states);
            d->SetState(states, &isDifferent);
            Int32 v = 0;
            d->SetLevel((GetLevel(&v), v), &isDifferent);

            AutoPtr<IRect> r;
            GetBounds((IRect**)&r);
            d->SetBounds(r);

            Int32 dir = 0;
            GetLayoutDirection(&dir);
            d->SetLayoutDirection(dir);
            d->SetAutoMirrored(mDrawableContainerState->mAutoMirrored);

            AutoPtr<IRect> hotspotBounds = mHotspotBounds;
            if (hotspotBounds != NULL) {
                Int32 left = 0, top = 0, bottom = 0, right = 0;
                hotspotBounds->Get(&left, &top, &right, &bottom);
                d->SetHotspotBounds(left, top, right, bottom);
            }
        }
    }
    else {
        mCurrDrawable = NULL;
        mCurIndex = -1;
    }

    if (mEnterAnimationEnd != 0 || mExitAnimationEnd != 0) {
        if (mAnimationRunnable == NULL) {
            mAnimationRunnable = new _Runnable(this);
        }
        else {
            UnscheduleSelf(mAnimationRunnable);
        }
        // Compute first frame and schedule next animation.
        Animate(true);
    }

    InvalidateSelf();
    *res = TRUE;
    return NOERROR;
}

void DrawableContainer::Animate(
    /* [in] */ Boolean schedule)
{
    mHasAlpha = TRUE;
    Int64 now = SystemClock::GetUptimeMillis();
    Boolean animating = FALSE;
    if (mCurrDrawable != NULL) {
        if (mEnterAnimationEnd != 0) {
            if (mEnterAnimationEnd <= now) {
                AutoPtr<IDrawable> dr;
                mCurrDrawable->Mutate((IDrawable**)&dr);
                dr->SetAlpha(mAlpha);
                mEnterAnimationEnd = 0;
            }
            else {
                Int32 animAlpha = (Int32)((mEnterAnimationEnd - now) * 255)
                        / mDrawableContainerState->mEnterFadeDuration;
                //if (DEBUG) android.util.Log.i(TAG, toString() + " cur alpha " + animAlpha);
                AutoPtr<IDrawable> dr;
                mCurrDrawable->Mutate((IDrawable**)&dr);
                dr->SetAlpha(((255 - animAlpha) * mAlpha) / 255);
                animating = TRUE;
            }
        }
    }
    else {
        mEnterAnimationEnd = 0;
    }

    if (mLastDrawable != NULL) {
        if (mExitAnimationEnd != 0) {
            if (mExitAnimationEnd <= now) {
                Boolean isDifferent;
                mLastDrawable->SetVisible(FALSE, FALSE, &isDifferent);
                mLastDrawable = NULL;
                mExitAnimationEnd = 0;
            }
            else {
                Int32 animAlpha = (Int32)((mExitAnimationEnd - now) * 255)
                        / mDrawableContainerState->mExitFadeDuration;
                //if (DEBUG) android.util.Log.i(TAG, toString() + " last alpha " + animAlpha);
                AutoPtr<IDrawable> dr;
                mLastDrawable->Mutate((IDrawable**)&dr);
                dr->SetAlpha((animAlpha * mAlpha) / 255);
                animating = TRUE;
            }
        }
    }
    else {
        mExitAnimationEnd = 0;
    }

    if (schedule && animating) {
        ScheduleSelf(mAnimationRunnable, now + 1000 / 60);
    }
}

ECode DrawableContainer::GetCurrent(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    *drawable = mCurrDrawable;
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

ECode DrawableContainer::ApplyTheme(
    /* [in] */ IResourcesTheme* theme)
{
    return mDrawableContainerState->ApplyTheme(theme);
}

ECode DrawableContainer::CanApplyTheme(
    /* [out] */ Boolean* can)
{
    return mDrawableContainerState->CanApplyTheme(can);
}

ECode DrawableContainer::GetConstantState(
    /* [out] */ IDrawableConstantState** state)
{
    VALIDATE_NOT_NULL(state);
    if (mDrawableContainerState->CanConstantState()) {
        GetChangingConfigurations(&mDrawableContainerState->mChangingConfigurations);
        *state = (IDrawableConstantState*)mDrawableContainerState->Probe(EIID_IDrawableConstantState);
        REFCOUNT_ADD(*state);
        return NOERROR;
    }
    *state = NULL;
    return NOERROR;
}

ECode DrawableContainer::Mutate(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> d;
    if (!mMutated && (Drawable::Mutate((IDrawable**)&d), d.Get()) == (IDrawable*)this->Probe(EIID_IDrawable)) {
        mDrawableContainerState->Mutate();
        mMutated = TRUE;
    }
    *drawable = (IDrawable*)this->Probe(EIID_IDrawable);
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

void DrawableContainer::SetConstantState(
    /* [in] */ DrawableContainerState* state)
{
    mDrawableContainerState = state;
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
DrawableContainer::DrawableContainerState::DrawableContainerState(
    /* [in] */ DrawableContainerState* orig,
    /* [in] */ DrawableContainer* owner,
    /* [in] */ IResources* res)
    : mOwner(owner)
    , mChangingConfigurations(0)
    , mChildrenChangingConfigurations(0)
    , mNumChildren(0)
    , mVariablePadding(FALSE)
    , mPaddingChecked(FALSE)
    , mConstantSize(FALSE)
    , mComputedConstantSize(FALSE)
    , mConstantWidth(0)
    , mConstantHeight(0)
    , mConstantMinimumWidth(0)
    , mConstantMinimumHeight(0)
    , mCheckedOpacity(FALSE)
    , mOpacity(0)
    , mCheckedStateful(FALSE)
    , mStateful(FALSE)
    , mCheckedConstantState(FALSE)
    , mCanConstantState(FALSE)
    , mDither(DEFAULT_DITHER)
    , mMutated(FALSE)
    , mLayoutDirection(0)
    , mEnterFadeDuration(0)
    , mExitFadeDuration(0)
    , mAutoMirrored(FALSE)
    , mHasColorFilter(FALSE)
    , mTintMode(-1)
    , mHasTintList(FALSE)
    , mHasTintMode(FALSE)
{
    mOwner = owner;
    mRes = res;

    if (orig != NULL) {
        mChangingConfigurations = orig->mChangingConfigurations;
        mChildrenChangingConfigurations = orig->mChildrenChangingConfigurations;

        mCheckedConstantState = TRUE;
        mCanConstantState = TRUE;

        mVariablePadding = orig->mVariablePadding;
        mConstantSize = orig->mConstantSize;
        mDither = orig->mDither;
        mMutated = orig->mMutated;
        mLayoutDirection = orig->mLayoutDirection;
        mEnterFadeDuration = orig->mEnterFadeDuration;
        mExitFadeDuration = orig->mExitFadeDuration;
        mAutoMirrored = orig->mAutoMirrored;
        mColorFilter = orig->mColorFilter;
        mHasColorFilter = orig->mHasColorFilter;
        mTintList = orig->mTintList;
        mTintMode = orig->mTintMode;
        mHasTintList = orig->mHasTintList;
        mHasTintMode = orig->mHasTintMode;

        // Cloning the following values may require creating futures.
        mConstantPadding = orig->GetConstantPadding();
        mPaddingChecked = TRUE;

        mConstantWidth = orig->GetConstantWidth();
        mConstantHeight = orig->GetConstantHeight();
        mConstantMinimumWidth = orig->GetConstantMinimumWidth();
        mConstantMinimumHeight = orig->GetConstantMinimumHeight();
        mComputedConstantSize = TRUE;

        mOpacity = orig->GetOpacity();
        mCheckedOpacity = TRUE;

        mStateful = orig->IsStateful();
        mCheckedStateful = TRUE;

        // Postpone cloning children and futures until we're absolutely
        // sure that we're done computing values for the original state.
        AutoPtr<ArrayOf<IDrawable*> > origDr = orig->mDrawables;
        mDrawables = ArrayOf<IDrawable*>::Alloc(origDr->GetLength());
        mNumChildren = orig->mNumChildren;

        assert(0 && "TODO");
        // final SparseArray<ConstantStateFuture> origDf = orig->mDrawableFutures;
        // if (origDf != NULL) {
        //     mDrawableFutures = origDf->Clone();
        // } else {
        //     mDrawableFutures = new SparseArray<ConstantStateFuture>(mNumChildren);
        // }

        // const Int32 N = mNumChildren;
        // for (Int32 i = 0; i < N; i++) {
        //     if (origDr[i] != NULL) {
        //         mDrawableFutures.put(i, new ConstantStateFuture(origDr[i]));
        //     }
        // }
    } else {
        mDrawables = ArrayOf<IDrawable*>::Alloc(10);
        mNumChildren = 0;
    }
}

DrawableContainer::DrawableContainerState::~DrawableContainerState()
{
    mDrawables = NULL;
}

ECode DrawableContainer::DrawableContainerState::GetChangingConfigurations(
    /* [out] */ Int32* configurations)
{
    VALIDATE_NOT_NULL(configurations);

    *configurations = mChangingConfigurations | mChildrenChangingConfigurations;
    return NOERROR;
}

Int32 DrawableContainer::DrawableContainerState::AddChild(
    /* [in] */ IDrawable* dr)
{
    Int32 pos = mNumChildren;

    if (pos >= mDrawables->GetLength()) {
        GrowArray(pos, pos + 10);
    }

    Boolean isDifferent;
    dr->SetVisible(FALSE, TRUE, &isDifferent);
    dr->SetCallback(mOwner != NULL ? (IDrawableCallback*)mOwner->Probe(EIID_IDrawableCallback) : NULL);

    mDrawables->Set(pos, dr);
    mNumChildren++;
    Int32 config;
    dr->GetChangingConfigurations(&config);
    mChildrenChangingConfigurations |= config;
    mCheckedStateful = FALSE;
    mCheckedOpacity = FALSE;

    mConstantPadding = NULL;
    mPaddingChecked = FALSE;
    mComputedConstantSize = FALSE;

    return pos;
}

Int32 DrawableContainer::DrawableContainerState::GetCapacity()
{
    return mDrawables->GetLength();
}

void DrawableContainer::DrawableContainerState::CreateAllFutures()
{
    assert(0 && "TODO");
    // if (mDrawableFutures != NULL) {
    //     final int futureCount = mDrawableFutures.size();
    //     for (int keyIndex = 0; keyIndex < futureCount; keyIndex++) {
    //         final int index = mDrawableFutures.keyAt(keyIndex);
    //         mDrawables[index] = mDrawableFutures.valueAt(keyIndex).get(this);
    //     }

    //     mDrawableFutures = null;
    // }
}

Int32 DrawableContainer::DrawableContainerState::GetChildCount()
{
    return mNumChildren;
}

AutoPtr< ArrayOf<IDrawable*> > DrawableContainer::DrawableContainerState::GetChildren()
{
    // Create all futures for backwards compatibility.
    CreateAllFutures();
    return mDrawables;
}

AutoPtr<IDrawable> DrawableContainer::DrawableContainerState::GetChild(
    /* [in] */ Int32 index)
{
    assert(0 && "TODO");
    // final Drawable result = mDrawables[index];
    // if (result != null) {
    //     return result;
    // }

    // // Prepare future drawable if necessary.
    // if (mDrawableFutures != null) {
    //     final int keyIndex = mDrawableFutures.indexOfKey(index);
    //     if (keyIndex >= 0) {
    //         final Drawable prepared = mDrawableFutures.valueAt(keyIndex).get(this);
    //         mDrawables[index] = prepared;
    //         mDrawableFutures.removeAt(keyIndex);
    //         return prepared;
    //     }
    // }

    return NULL;
}

ECode DrawableContainer::DrawableContainerState::SetLayoutDirection(
    /* [in] */ Int32 layoutDirection)
{
    assert(0 && "TODO");
    // No need to call createAllFutures, since future drawables will
    // change layout direction when they are prepared.
    // final int N = mNumChildren;
    // final Drawable[] drawables = mDrawables;
    // for (int i = 0; i < N; i++) {
    //     if (drawables[i] != null) {
    //         drawables[i].setLayoutDirection(layoutDirection);
    //     }
    // }

    mLayoutDirection = layoutDirection;
    return NOERROR;
}

ECode DrawableContainer::DrawableContainerState::ApplyTheme(
    /* [in] */ IResourcesTheme* theme)
{
    assert(0 && "TODO");
    // No need to call createAllFutures, since future drawables will
    // apply the theme when they are prepared.
    // final int N = mNumChildren;
    // final Drawable[] drawables = mDrawables;
    // for (int i = 0; i < N; i++) {
    //     if (drawables[i] != null) {
    //         drawables[i].applyTheme(theme);
    //     }
    // }

    mTheme = theme;
    return NOERROR;
}

ECode DrawableContainer::DrawableContainerState::CanApplyTheme(
    /* [out] */ Boolean* can)
{
    assert(0 && "TODO");
    VALIDATE_NOT_NULL(can);
    // final int N = mNumChildren;
    // final Drawable[] drawables = mDrawables;
    // for (int i = 0; i < N; i++) {
    //     final Drawable d = drawables[i];
    //     if (d != null) {
    //         if (d.canApplyTheme()) {
    //             return true;
    //         }
    //     } else {
    //         final ConstantStateFuture future = mDrawableFutures.get(i);
    //         if (future != null && future.canApplyTheme()) {
    //             return true;
    //         }
    //     }
    // }

    *can = FALSE;
    return NOERROR;
}

ECode DrawableContainer::DrawableContainerState::Mutate()
{
    assert(0 && "TODO");
    // No need to call createAllFutures, since future drawables will
    // mutate when they are prepared.
    // final int N = mNumChildren;
    // final Drawable[] drawables = mDrawables;
    // for (int i = 0; i < N; i++) {
    //     if (drawables[i] != null) {
    //         drawables[i].mutate();
    //     }
    // }

    mMutated = TRUE;
    return NOERROR;
}

void DrawableContainer::DrawableContainerState::SetVariablePadding(
    /* [in] */ Boolean variable)
{
    mVariablePadding = variable;
}

AutoPtr<IRect> DrawableContainer::DrawableContainerState::GetConstantPadding()
{
    if (mVariablePadding) {
        return NULL;
    }
    if (mConstantPadding != NULL || mPaddingChecked) {
        return mConstantPadding;
    }

    CreateAllFutures();

    AutoPtr<IRect> r;
    AutoPtr<IRect> t;
    ASSERT_SUCCEEDED(CRect::New((IRect**)&t));
    Int32 N = mNumChildren;
    for (Int32 i = 0; i < N; i++) {
        Boolean isPadding;
        (*mDrawables)[i]->GetPadding(t, &isPadding);
        if (isPadding) {
            if (r == NULL) ASSERT_SUCCEEDED(CRect::New(0, 0, 0, 0, (IRect**)&r));
            CRect* t_ = (CRect*)t.Get();
            CRect* r_ = (CRect*)r.Get();
            if (t_->mLeft > r_->mLeft) r_->mLeft = t_->mLeft;
            if (t_->mTop > r_->mTop) r_->mTop = t_->mTop;
            if (t_->mRight > r_->mRight) r_->mRight = t_->mRight;
            if (t_->mBottom > r_->mBottom) r_->mBottom = t_->mBottom;
        }
    }
    mPaddingChecked = TRUE;
    mConstantPadding = r;
    return mConstantPadding;
}

void DrawableContainer::DrawableContainerState::SetConstantSize(
    /* [in] */ Boolean constant)
{
    mConstantSize = constant;
}

Boolean DrawableContainer::DrawableContainerState::IsConstantSize()
{
    return mConstantSize;
}

Int32 DrawableContainer::DrawableContainerState::GetConstantWidth()
{
    if (!mComputedConstantSize) {
        ComputeConstantSize();
    }

    return mConstantWidth;
}

Int32 DrawableContainer::DrawableContainerState::GetConstantHeight()
{
    if (!mComputedConstantSize) {
        ComputeConstantSize();
    }

    return mConstantHeight;
}

Int32 DrawableContainer::DrawableContainerState::GetConstantMinimumWidth()
{
    if (!mComputedConstantSize) {
        ComputeConstantSize();
    }

    return mConstantMinimumWidth;
}

Int32 DrawableContainer::DrawableContainerState::GetConstantMinimumHeight()
{
    if (!mComputedConstantSize) {
        ComputeConstantSize();
    }

    return mConstantMinimumHeight;
}

void DrawableContainer::DrawableContainerState::ComputeConstantSize()
{
    mComputedConstantSize = TRUE;

    CreateAllFutures();

    Int32 N = mNumChildren;
    mConstantWidth = mConstantHeight = -1;
    mConstantMinimumWidth = mConstantMinimumHeight = 0;
    for (Int32 i = 0; i < N; i++) {
        AutoPtr<IDrawable> dr = (*mDrawables)[i];
        Int32 s;
        dr->GetIntrinsicWidth(&s);
        if (s > mConstantWidth) mConstantWidth = s;
        dr->GetIntrinsicHeight(&s);
        if (s > mConstantHeight) mConstantHeight = s;
        dr->GetMinimumWidth(&s);
        if (s > mConstantMinimumWidth) mConstantMinimumWidth = s;
        dr->GetMinimumHeight(&s);
        if (s > mConstantMinimumHeight) mConstantMinimumHeight = s;
    }
}

void DrawableContainer::DrawableContainerState::SetEnterFadeDuration(
    /* [in] */ Int32 duration)
{
    mEnterFadeDuration = duration;
}

Int32 DrawableContainer::DrawableContainerState::GetEnterFadeDuration()
{
    return mEnterFadeDuration;
}

void DrawableContainer::DrawableContainerState::SetExitFadeDuration(
    /* [in] */ Int32 duration)
{
    mExitFadeDuration = duration;
}

Int32 DrawableContainer::DrawableContainerState::GetExitFadeDuration()
{
    return mExitFadeDuration;
}

Int32 DrawableContainer::DrawableContainerState::GetOpacity()
{
    if (mCheckedOpacity) {
        return mOpacity;
    }

    CreateAllFutures();

    mCheckedOpacity = TRUE;

    Int32 N = mNumChildren;
    Int32 op;
    if (N > 0) {
        Int32 opacity;
        (*mDrawables)[0]->GetOpacity(&opacity);
        op = opacity;
    }
    else op = IPixelFormat::TRANSPARENT;
    for (Int32 i = 1; i < N; i++) {
        Int32 opacity;
        (*mDrawables)[i]->GetOpacity(&opacity);
        Drawable::ResolveOpacity(op, opacity, &op);
    }
    mOpacity = op;
    return op;
}

Boolean DrawableContainer::DrawableContainerState::IsStateful()
{
    if (mCheckedStateful) {
        return mStateful;
    }

    CreateAllFutures();

    mCheckedStateful = TRUE;

    Int32 N = mNumChildren;
    AutoPtr<ArrayOf<IDrawable*> > drawables = mDrawables;
    Boolean state = FALSE;
    for (Int32 i = 0; i < N; i++) {
        if (((*drawables)[i]->IsStateful(&state), state)) {
            mStateful = TRUE;
            return TRUE;
        }
    }

    mStateful = FALSE;
    return FALSE;
}

void DrawableContainer::DrawableContainerState::GrowArray(
    /* [in] */ Int32 oldSize,
    /* [in] */ Int32 newSize)
{
    AutoPtr<ArrayOf<IDrawable*> > newDrawables = ArrayOf<IDrawable*>::Alloc(newSize);
    for (Int32 i = 0; i < oldSize; i++) {
        newDrawables->Set(i, (*mDrawables)[i]);
    }
    mDrawables = newDrawables;
}

Boolean DrawableContainer::DrawableContainerState::CanConstantState()
{
    if (mCheckedConstantState) {
        return mCanConstantState;
    }

    CreateAllFutures();

    mCheckedConstantState = TRUE;

    Int32 N = mNumChildren;
    AutoPtr<ArrayOf<IDrawable*> > drawables = mDrawables;
    for (Int32 i = 0; i < N; i++) {
        AutoPtr<IDrawableConstantState> state;
        (*drawables)[i]->GetConstantState((IDrawableConstantState**)&state);
        if (state.Get() == NULL) {
            mCanConstantState = FALSE;
            return FALSE;
        }
    }

    mCanConstantState = TRUE;
    return TRUE;
}

CAR_INTERFACE_IMPL(DrawableContainer::_Runnable, Object, IRunnable)
DrawableContainer::_Runnable::_Runnable(
    /* [in] */ DrawableContainer* host)
    : mHost(host)
{}

ECode DrawableContainer::_Runnable::Run()
{
    mHost->Animate(TRUE);
    mHost->InvalidateSelf();
    return NOERROR;
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
