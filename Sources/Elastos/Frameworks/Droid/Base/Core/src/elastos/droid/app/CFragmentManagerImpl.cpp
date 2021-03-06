
#include "ext/frameworkdef.h"
#include "app/Fragment.h"
#include "app/CFragmentSavedState.h"
#include "app/CFragmentManagerState.h"
#include "app/CFragmentManagerImpl.h"
#include "app/Activity.h"
#include "app/CBackStackState.h"
#include "os/CBundle.h"
#include "os/Looper.h"
#include "animation/AnimatorInflater.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Logger.h>
#include "R.h"

using Elastos::Droid::R;
using Elastos::Core::CInteger32;
using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Utility::CObjectInt32Map;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Os::Looper;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::App::Fragment;
using Elastos::Droid::App::CFragmentSavedState;
using Elastos::Droid::View::IWindow;
using Elastos::Droid::View::IViewManager;
using Elastos::Droid::Animation::AnimatorInflater;
using Elastos::Droid::Content::Res::ITypedArray;


namespace Elastos {
namespace Droid {
namespace App {

ECode ExecCommitRunnable::Run()
{
    Boolean executed;
    return mHost->ExecPendingActions(&executed);
}

ECode Runnable1::Run()
{
    Boolean hasPopped;
    AutoPtr<Activity> activity = reinterpret_cast<Activity*>(mHost->mActivity->Probe(EIID_Activity));
    return mHost->PopBackStackState(activity->mHandler, String(NULL), -1, 0, &hasPopped);
}

ECode Runnable2::Run()
{
    Boolean hasPopped;
    AutoPtr<Activity> activity = reinterpret_cast<Activity*>(mHost->mActivity->Probe(EIID_Activity));
    return mHost->PopBackStackState(activity->mHandler, mName, -1, mFlags, &hasPopped);
}

ECode Runnable3::Run()
{
    Boolean hasPopped;
    AutoPtr<Activity> activity = reinterpret_cast<Activity*>(mHost->mActivity->Probe(EIID_Activity));
    return mHost->PopBackStackState(activity->mHandler, String(NULL), mId, mFlags, &hasPopped);
}

AnimatorListener::AnimatorListener(
    /* [in] */ IViewGroup* container,
    /* [in] */ IView* view,
    /* [in] */ IFragment* fragment,
    /* [in] */ CFragmentManagerImpl* host)
{
    mContainer = container;
    mView = view;
    mFragment = fragment;
    mHost = host;
}

ECode AnimatorListener::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    mContainer->EndViewTransition(mView);
    AutoPtr<IAnimator> anim;
    mFragment->GetAnimatingAway((IAnimator**)&anim);
    if(anim) {
        mFragment->SetAnimatingAway(NULL);
        Int32 state;
        mFragment->GetStateAfterAnimating(&state);
        mHost->MoveToState(mFragment, state, 0, 0, FALSE);
    }
    return NOERROR;
}

AnimatorListenerEx::AnimatorListener(
    /* [in] */ IFragment* fragment)
{
    mFragment = fragment;
}

ECode AnimatorListenerEx::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    if (mFragment) {
        AutoPtr<IView> v;
        mFragment->GetView((IView**)&v);
        v->SetVisibility(IView::GONE);
    }
    return NOERROR;
}

Boolean CFragmentManagerImpl::DEBUG = FALSE;

CFragmentManagerImpl::CFragmentManagerImpl()
    : mTmpActions(NULL)
    , mExecutingActions(FALSE)
    , mCurState(IFragment::INITIALIZING)
    , mNeedMenuInvalidate(FALSE)
    , mStateSaved(FALSE)
    , mDestroyed(FALSE)
    , mNoTransactionsBecause(String(NULL))
    , mHavePendingDeferredStart(FALSE)
{
    mExecCommit = new ExecCommitRunnable(this);
}

CFragmentManagerImpl::~CFragmentManagerImpl()
{
    mPendingActions.Clear();
    mActive.Clear();
    mAdded.Clear();
    mAvailIndices.Clear();
    mBackStack.Clear();
    mCreatedMenus.Clear();
    mBackStackIndices.Clear();
    mAvailBackStackIndices.Clear();
    mBackStackChangeListeners.Clear();
}

ECode CFragmentManagerImpl::BeginTransaction(
    /* [out] */ IFragmentTransaction** transaction)
{
    VALIDATE_NOT_NULL(transaction);

    *transaction = new BackStackRecord(this);
    REFCOUNT_ADD(*transaction);
    return NOERROR;
}

ECode CFragmentManagerImpl::OpenTransaction(
    /* [out] */ IFragmentTransaction** transaction)
{
   VALIDATE_NOT_NULL(transaction);
   return BeginTransaction(transaction);
}

ECode CFragmentManagerImpl::ExecutePendingTransactions(
    /* [out] */ Boolean* executed)
{
   VALIDATE_NOT_NULL(executed);
   return ExecPendingActions(executed);
}

ECode CFragmentManagerImpl::PopBackStack()
{
   AutoPtr<IRunnable> runnable = new Runnable1(this);
   return EnqueueAction(runnable, FALSE);
}

ECode CFragmentManagerImpl::PopBackStackImmediate(
/* [out] */ Boolean* hasPopped)
{
   VALIDATE_NOT_NULL(hasPopped);
   FAIL_RETURN(CheckStateLoss());
   Boolean executed;
   ExecutePendingTransactions(&executed);
   AutoPtr<Activity> activity = reinterpret_cast<Activity*>(mActivity->Probe(EIID_Activity));
   return PopBackStackState(activity->mHandler, String(NULL), -1, 0, hasPopped);
}

ECode CFragmentManagerImpl::PopBackStack(
    /* [in] */ const String& name,
    /* [in] */ Int32 flags)
{
   AutoPtr<IRunnable> runnable = new Runnable2(this, name, flags);
   return EnqueueAction(runnable, FALSE);
}

ECode CFragmentManagerImpl::PopBackStackImmediate(
    /* [in] */ const String& name,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* hasPopped)
{
    VALIDATE_NOT_NULL(hasPopped);
    FAIL_RETURN(CheckStateLoss());
    Boolean executed;
    ExecutePendingTransactions(&executed);
    AutoPtr<Activity> activity = reinterpret_cast<Activity*>(mActivity->Probe(EIID_Activity));
    return PopBackStackState(activity->mHandler, name, -1, flags, hasPopped);
}

ECode CFragmentManagerImpl::PopBackStack(
    /* [in] */ Int32 id,
    /* [in] */ Int32 flags)
{
    if (id < 0) {
//         throw new IllegalArgumentException("Bad id: " + id);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IRunnable> runnable = new Runnable3(this, id, flags);
    EnqueueAction(runnable, FALSE);
    return NOERROR;
}

ECode CFragmentManagerImpl::PopBackStackImmediate(
    /* [in] */ Int32 id,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* hasPopped)
{
    FAIL_RETURN(CheckStateLoss());
    Boolean executed;
    ExecutePendingTransactions(&executed);
    if (id < 0) {
//         throw new IllegalArgumentException("Bad id: " + id);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<Activity> activity = reinterpret_cast<Activity*>(mActivity->Probe(EIID_Activity));
    return PopBackStackState(activity->mHandler, String(NULL), id, flags, hasPopped);
}

ECode CFragmentManagerImpl::GetBackStackEntryCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mBackStack.GetSize();
    return NOERROR;
}

ECode CFragmentManagerImpl::GetBackStackEntryAt(
    /* [in] */ Int32 index,
    /* [out] */ IFragmentManagerBackStackEntry** bsEntry)
{
    VALIDATE_NOT_NULL(bsEntry);
//     return mBackStack.get(index);
    return E_NOT_IMPLEMENTED;
}

ECode CFragmentManagerImpl::AddOnBackStackChangedListener(
    /* [in] */ IFragmentManagerOnBackStackChangedListener* listener)
{
    mBackStackChangeListeners.PushBack(listener);
    return NOERROR;
}

ECode CFragmentManagerImpl::RemoveOnBackStackChangedListener(
    /* [in] */ IFragmentManagerOnBackStackChangedListener* listener)
{
    if (listener != NULL) {
        mBackStackChangeListeners.Remove(listener);
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::PutFragment(
    /* [in] */ IBundle* bundle,
    /* [in] */ const String& key,
    /* [in] */ IFragment* fragment)
{
    Int32 index;
    fragment->GetIndex(&index);
    if (index < 0) {
//         throwException(new IllegalStateException("Fragment " + fragment
//                 + " is not currently in the FragmentManager"));
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    bundle->PutInt32(key, index);
    return NOERROR;
}

ECode CFragmentManagerImpl::GetFragment(
    /* [in] */ IBundle* bundle,
    /* [in] */ const String& key,
    /* [out] */ IFragment** fragment)
{
    VALIDATE_NOT_NULL(fragment);
    Int32 index;
    bundle->GetInt32(key, -1, &index);
    if (index == -1) {
        *fragment = NULL;
    }
    if (index >= (Int32)mActive.GetSize()) {
//         throwException(new IllegalStateException("Fragement no longer exists for key "
//                 + key + ": index " + index));
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    AutoPtr<IFragment> f = mActive[index];
    if (f == NULL) {
//         throwException(new IllegalStateException("Fragement no longer exists for key "
//                 + key + ": index " + index));
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    *fragment = f;
    REFCOUNT_ADD(*fragment);
    return NOERROR;
}

ECode CFragmentManagerImpl::SaveFragmentInstanceState(
    /* [in] */ IFragment* fragment,
    /* [out] */ IFragmentSavedState** state)
{
    VALIDATE_NOT_NULL(state);
    *state = NULL;

    Int32 index;
    fragment->GetIndex(&index);
    if (index < 0) {
//         throwException(new IllegalStateException("Fragment " + fragment
//                 + " is not currently in the FragmentManager"));
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    Int32 fstate;
    fragment->GetState(&fstate);
    if (fstate > IFragment::INITIALIZING) {
        AutoPtr<IBundle> result;
        SaveFragmentBasicState(fragment, (IBundle**)&result);
        if (result != NULL) {
            return CFragmentSavedState::New(result, state);
        }
    }

    return NOERROR;
}

ECode CFragmentManagerImpl::IsDestroyed(
    /* [out] */ Boolean* destroyed)
{
    VALIDATE_NOT_NULL(destroyed);
    *destroyed = mDestroyed;
    return NOERROR;
}

ECode CFragmentManagerImpl::ToString(
    /* [out] */ String* s)
{
    VALIDATE_NOT_NULL(s);
    StringBuilder sb(128);
    sb += "FragmentManager{";
    sb += StringUtils::Int32ToHexString((Int32)this);
    sb += " in ";
    if (mParent != NULL) {
//         DebugUtils.buildShortClassTag(mParent, sb);
    } else {
//         DebugUtils.buildShortClassTag(mActivity, sb);
    }
    sb += "}}";
    *s = sb.ToString();
    return NOERROR;
}

ECode CFragmentManagerImpl::Dump(
    /* [in] */ const String& prefix,
    /* [in] */ IFileDescriptor* fd,
    /* [in] */ IPrintWriter* writer,
    /* [in] */ ArrayOf<String>* args)
{
    String innerPrefix = prefix + "    ";

    Int32 N;
    if (!mActive.IsEmpty()) {
        writer->PrintString(prefix);
        writer->PrintString(String("Active Fragments in "));
        writer->PrintString(StringUtils::Int32ToHexString((Int32)this));
        writer->PrintStringln(String(":"));
        Int32 i = 0;
        List<AutoPtr<IFragment> >::Iterator it;
        for (it = mActive.Begin(); it != mActive.End(); ++it) {
            AutoPtr<IFragment> f = *it;
            writer->PrintString(prefix);
            writer->PrintString(String("  #"));
            writer->PrintInt32(i);
            writer->PrintString(String(": "));
            writer->PrintObjectln(f->Probe(EIID_IInterface));
            if (f != NULL) {
                f->Dump(innerPrefix, fd, writer, args);
            }
            ++i;
        }
    }

    if (!mAdded.IsEmpty()) {
        writer->PrintString(prefix);
        writer->PrintStringln(String("Added Fragments:"));
        Int32 i = 0;
        List<AutoPtr<IFragment> >::Iterator it;
        for (it = mAdded.Begin(); it != mAdded.End(); ++it) {
            AutoPtr<IFragment> f = *it;
            writer->PrintString(prefix);
            writer->PrintString(String("  #"));
            writer->PrintInt32(i);
            writer->PrintString(String(": "));
            String fstring;
            f->ToString(&fstring);
            writer->PrintStringln(fstring);
        }
    }

   if (!mCreatedMenus.IsEmpty()) {
        writer->PrintString(prefix);
        writer->PrintStringln(String("Fragments Created Menus:"));
        Int32 i = 0;
        List<AutoPtr<IFragment> >::Iterator it;
        for (it = mCreatedMenus.Begin(); it != mCreatedMenus.End(); ++it) {
            AutoPtr<IFragment> f = *it;
            writer->PrintString(prefix);
            writer->PrintString(String("  #"));
            writer->PrintInt32(i);
            writer->PrintString(String(": "));
            String fstring;
            f->ToString(&fstring);
            writer->PrintStringln(fstring);
        }
    }

//TODO:
//     if (mBackStack != NULL) {
//         if (mBackStack->Begin() != mBackStack->End()) {
//             writer->PrintString(prefix);
//             writer->PrintStringln("Back Stack:");
//             List<IBackStackRecord* >::Iterator it;
//             for (it = mBackStack->Begin(); it != mBackStack->End(); ++it) {
//                 AutoPtr<IBackStackRecord> bs = *it;
//                 writer->PrintString(prefix);
//                 writer->PrintString("  #");
//                 writer->PrintInt32(i);
//                 writer->PrintString(": ");
//                 String bsString;
//                 bs->ToString(&bsString);
//                 writer->PrintStringln(bsString);
//                 bs->Dump(innerPrefix, fd, writer, args);
//             }
//         }
//     }
//TODO:
//     {
//         Object::AutoLock lock(mThisLock);
//         if (mBackStackIndices != NULL) {
//             if (mBackStackIndices->Begin() > mBackStackIndices->End()) {
//                 writer->PrintString(prefix);
//                 writer->PrintStringln("Back Stack Indices:");
//                 List<IBackStackRecord* >::Iterator it;
//                 for (it = mBackStackIndices->Begin(); it != mBackStackIndices->End(); ++it) {
// //                 for (int i=0; i<N; i++) {
//                     AutoPtr<IBackStackRecord> bs = *it;
//                     writer->PrintString(prefix);
//                     writer->PrintString("  #");
//                     writer->PrintInt32(i);
//                     writer->PrintString(": ");
//                     writer->PrintObjectln(bs->Probe(EIID_IInterface);
//                 }
//             }
//         }

//         if (mAvailBackStackIndices != NULL && mAvailBackStackIndices->Begin() != mAvailBackStackIndices->End()) {
//             writer->PrintString(prefix);
//             writer->PrintString("mAvailBackStackIndices: ");
// //            writer->PrintStringln(Arrays.toString(mAvailBackStackIndices.toArray()));
//         }
//     }

    if (!mPendingActions.IsEmpty()) {
        writer->PrintString(prefix);
        writer->PrintStringln(String("Pending Actions:"));
        Int32 i = 0;
        List<AutoPtr<IRunnable> >::Iterator it;
        for (it = mPendingActions.Begin(); it != mPendingActions.End(); ++it) {
            AutoPtr<IRunnable> r = *it;
            writer->PrintString(prefix);
            writer->PrintString(String("  #"));
            writer->PrintInt32(i);
            writer->PrintString(String(": "));
            writer->PrintObjectln(r->Probe(EIID_IInterface));
        }
    }

    writer->PrintString(prefix);
    writer->PrintStringln(String("FragmentManager misc state:"));
    writer->PrintString(prefix);
    writer->PrintString(String("  mActivity="));
    writer->PrintObjectln(mActivity->Probe(EIID_IInterface));
    writer->PrintString(prefix);
    writer->PrintString(String("  mContainer="));
    writer->PrintObjectln(mContainer->Probe(EIID_IInterface));
    if (mParent != NULL) {
        writer->PrintString(prefix);
        writer->PrintString(String("  mParent="));
        writer->PrintObjectln(mParent->Probe(EIID_IInterface));
    }
    writer->PrintString(prefix);
    writer->PrintString(String("  mCurState="));
    writer->PrintInt32(mCurState);
    writer->PrintString(String(" mStateSaved="));
    writer->PrintBoolean(mStateSaved);
    writer->PrintString(String(" mDestroyed="));
    writer->PrintBooleanln(mDestroyed);
    if (mNeedMenuInvalidate) {
        writer->PrintString(prefix);
        writer->PrintString(String("  mNeedMenuInvalidate="));
        writer->PrintBooleanln(mNeedMenuInvalidate);
    }
    if (mNoTransactionsBecause != NULL) {
        writer->PrintString(prefix);
        writer->PrintString(String("  mNoTransactionsBecause="));
        writer->PrintStringln(mNoTransactionsBecause);
    }
    if (!mAvailIndices.IsEmpty()) {
        writer->PrintString(prefix);
        writer->PrintString(String("  mAvailIndices: "));
//        writer.println(Arrays.toString(mAvailIndices.toArray()));
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::LoadAnimator(
    /* [in] */ IFragment* fragment,
    /* [in] */ Int32 transit,
    /* [in] */ Boolean enter,
    /* [in] */ Int32 transitionStyle,
    /* [out] */ IAnimator** animator)
{
    VALIDATE_NOT_NULL(animator);
    Int32 fanim;
    fragment->GetNextAnim(&fanim);
    AutoPtr<IAnimator> animObj;
    fragment->OnCreateAnimator(transit, enter,
            fanim, (IAnimator**)&animObj);
    if (animObj != NULL) {
        *animator = animObj;
        REFCOUNT_ADD(*animator);
        return NOERROR;
    }

    if (fanim != 0) {
        AutoPtr<IAnimator> anim = AnimatorInflater::LoadAnimator(mActivity, fanim);
        if (anim != NULL) {
            *animator = anim;
            REFCOUNT_ADD(*animator);
            return NOERROR;
        }
    }

    if (transit == 0) {
        *animator = NULL;
        return NOERROR;
    }

    Int32 styleIndex;
    TransitToStyleIndex(transit, enter, &styleIndex);
    if (styleIndex < 0) {
        *animator = NULL;
        return NOERROR;
    }

    AutoPtr<IWindow> window;
    mActivity->GetWindow((IWindow**)&window);
    if (transitionStyle == 0 && window != NULL) {
        AutoPtr<IWindowManagerLayoutParams> attr;
        window->GetAttributes((IWindowManagerLayoutParams**)&attr);
        Int32 wAnimations;
        attr->GetWindowAnimations(&wAnimations);
        transitionStyle = wAnimations;
    }
    if (transitionStyle == 0) {
        *animator = NULL;
        return NOERROR;
    }
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::FragmentAnimation),
            ARRAY_SIZE(R::styleable::FragmentAnimation));
    AutoPtr<ITypedArray> attrs;
    mActivity->ObtainStyledAttributes(transitionStyle, attrIds, (ITypedArray**)&attrs);
    Int32 anim;
    attrs->GetResourceId(styleIndex, 0, &anim);
    attrs->Recycle();

    if (anim == 0) {
        *animator = NULL;
        return NOERROR;
    }
    AutoPtr<IAnimator> a = AnimatorInflater::LoadAnimator(mActivity, anim);
    *animator = a;
    REFCOUNT_ADD(*animator)
    return NOERROR;
}

ECode CFragmentManagerImpl::PerformPendingDeferredStart(
    /* [in] */ IFragment* f)
{
    Boolean start;
    f->GetDeferStart(&start);
    if (start) {
        if (mExecutingActions) {
            // Wait until we're done executing our pending transactions
            mHavePendingDeferredStart = TRUE;
            return NOERROR;
        }
        f->SetDeferStart(FALSE);
        MoveToState(f, mCurState, 0, 0, FALSE);
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::MoveToState(
    /* [in] */ IFragment* f,
    /* [in] */ Int32 newState,
    /* [in] */ Int32 transit,
    /* [in] */ Int32 transitionStyle,
    /* [in] */ Boolean keepActive)
{
    Int32 state;
    f->GetState(&state);
    Boolean removing;
    f->IsRemoving(&removing);
    if (DEBUG && FALSE) Logger::V(TAG, "moveToState: %p oldState=%d newState=%d mRemoving=%d", f
        , state, newState, removing /*" Callers=" + *Debug.getCallers(5)*/);

    // Fragments that are not currently added will sit in the onCreate() state.
    Boolean added;
    f->GetAdded(&added);
    Boolean detached;
    f->IsDetached(&detached);
    if ((!added || detached) && newState > IFragment::CREATED) {
        newState = IFragment::CREATED;
    }
    if (removing && newState > state) {
        // While removing a fragment, we can't change it to a higher state.
        newState = state;
    }
    // Defer start if requested; don't allow it to move to STARTED or higher
    // if it's not already started.
    Boolean deferStart;
    f->GetDeferStart(&deferStart);
    if (deferStart && state < IFragment::STARTED && newState > IFragment::STOPPED) {
        newState = IFragment::STOPPED;
    }
    Boolean fromLayout;
    f->GetFromLayout(&fromLayout);
    Boolean inLayout;
    f->IsInLayout(&inLayout);
    AutoPtr<IAnimator> animatingAway;
    f->GetAnimatingAway((IAnimator**)&animatingAway);
    Int32 stateAfterAnimating;
    f->GetStateAfterAnimating(&stateAfterAnimating);
    AutoPtr<IBundle> savedFragmentState;
    f->GetSavedFragmentState((IBundle**)&savedFragmentState);
    if (state < newState) {
        // For fragments that are created from a layout, when restoring from
        // state we don't want to allow them to be created until they are
        // being reloaded from the layout.
        if (fromLayout && !inLayout) {
            return NOERROR;
        }
        if (animatingAway != NULL) {
            // The fragment is currently being animated...  but!  Now we
            // want to move our state back up.  Give up on waiting for the
            // animation, move to whatever the final state should be once
            // the animation is done, and then we can proceed from there.
            animatingAway = NULL;
            MoveToState(f, stateAfterAnimating, 0, 0, TRUE);
        }
        switch (state) {
            case IFragment::INITIALIZING:
            {
                if (DEBUG) Logger::V(TAG, "moveto CREATED: %p", f);
                if (savedFragmentState != NULL) {
                   AutoPtr<IInterface> parcelableMap;
                   savedFragmentState->GetParcelableMap(
                            IFragmentManagerImpl::VIEW_STATE_TAG, (IInterface**)&parcelableMap);
                    f->SetSavedViewState(IObjectInt32Map::Probe(parcelableMap));
                    AutoPtr<IFragment> fragment;
                    GetFragment(savedFragmentState,
                            IFragmentManagerImpl::TARGET_STATE_TAG, (IFragment**)&fragment);
                    f->SetTarget(fragment);
                    if (fragment != NULL) {
                        Int32 value;
                        savedFragmentState->GetInt32(
                                IFragmentManagerImpl::TARGET_REQUEST_CODE_STATE_TAG, 0, &value);
                        f->SetTargetRequestCode(value);
                    }
                    Boolean value;
                    savedFragmentState->GetBoolean(
                            IFragmentManagerImpl::USER_VISIBLE_HINT_TAG, TRUE, &value);
                    f->SetUserVisibleHint(value);
                    if (!value) {
                        f->SetDeferStart(TRUE);
                        if (newState > IFragment::STOPPED) {
                            newState = IFragment::STOPPED;
                        }
                    }
                }
                f->SetActivity(mActivity);
                f->SetParentFragment(mParent);
                if (mParent != NULL) {
                    AutoPtr<IFragmentManagerImpl> pFragmentManagerImpl;
                    mParent->GetChildFragmentManagerValue((IFragmentManagerImpl**)&pFragmentManagerImpl);
                    f->SetFragmentManager(pFragmentManagerImpl);
                }
                else {
                    AutoPtr<Activity> activity = reinterpret_cast<Activity*>(mActivity->Probe(EIID_Activity));
                    f->SetFragmentManager(activity->mFragments);
                }
                f->SetCalled(FALSE);
                f->OnAttach(mActivity);
                Boolean called;
                f->GetCalled(&called);
                if (!called) {
//                     throw new SuperNotCalledException("Fragment " + f
//                             + " did not call through to super.onAttach()");
                    return E_SUPER_NOT_CALLED_EXCEPTION;
                }
                AutoPtr<IFragment> parentFragment;
                f->GetParentFragment((IFragment**)&parentFragment);
                if (parentFragment == NULL) {
                    mActivity->OnAttachFragment(f);
                }
                Boolean retaining;
                f->GetRetaining(&retaining);
                if (!retaining) {
                    f->PerformCreate(savedFragmentState);
                }
                f->SetRetaining(FALSE);
                if (fromLayout) {
                    // For fragments that are part of the content view
                    // layout, we need to instantiate the view immediately
                    // and the inflater will take care of adding it.
                    AutoPtr<ILayoutInflater> layoutInflater;
                    f->GetLayoutInflater(savedFragmentState, (ILayoutInflater**)&layoutInflater);
                    AutoPtr<IView> createView;
                    f->PerformCreateView(layoutInflater, NULL, savedFragmentState, (IView**)&createView);
                    f->SetView(createView);
                    AutoPtr<IView> view;
                    f->GetView((IView**)&view);
                    if (view != NULL) {
                        view->SetSaveFromParentEnabled(FALSE);
                        Boolean hidden;
                        f->IsHidden(&hidden);
                        if (hidden) view->SetVisibility(IView::GONE);
                        f->OnViewCreated(view, savedFragmentState);
                    }
                }
            }
            case IFragment::CREATED:
                if (newState > IFragment::CREATED) {
                    if (DEBUG) Logger::V(TAG, "moveto ACTIVITY_CREATED: %p", f);
                    if (!fromLayout) {
                        AutoPtr<IViewGroup> container = NULL;
                        Int32 containerId;
                        f->GetContainerId(&containerId);
                        if (containerId != 0) {
                            AutoPtr<IView> findView;
                            mContainer->FindViewById(containerId, (IView**)&findView);
                            container = IViewGroup::Probe(findView);
                            Boolean restored;
                            f->GetRestored(&restored);
                            if (container == NULL && !restored) {
//                                 throwException(new IllegalArgumentException(
//                                         "No view found for id 0x"
//                                         + Integer.toHexString(f.mContainerId) + " ("
//                                         + f.getResources().getResourceName(f.mContainerId)
//                                         + ") for fragment " + f));
                                return E_ILLEGAL_ARGUMENT_EXCEPTION;
                            }
                        }
                        f->SetContainer(container);
                        AutoPtr<ILayoutInflater> lInflater;
                        f->GetLayoutInflater(savedFragmentState, (ILayoutInflater**)&lInflater);
                        AutoPtr<IView> cView;
                        f->PerformCreateView(lInflater, container, savedFragmentState, (IView**)&cView);
                        f->SetView(cView);
                        AutoPtr<IView> view;
                        f->GetView((IView**)&view);
                        if (view != NULL) {
                            view->SetSaveFromParentEnabled(FALSE);
                            if (container != NULL) {
                                AutoPtr<IAnimator> anim;
                                LoadAnimator(f, transit, TRUE, transitionStyle, (IAnimator**)&anim);
                                if (anim != NULL) {
                                    anim->SetTarget(view);
                                    anim->Start();
                                }
                                container->AddView(view);
                            }
                            Boolean hidden;
                            f->IsHidden(&hidden);
                            if (hidden) view->SetVisibility(IView::GONE);
                            f->OnViewCreated(view, savedFragmentState);
                        }
                    }

                    f->PerformActivityCreated(savedFragmentState);
                    AutoPtr<IView> view;
                    f->GetView((IView**)&view);
                    if (view != NULL) {
                        f->RestoreViewState(savedFragmentState);
                    }
                    f->SetSavedFragmentState(NULL);
                }
            case IFragment::ACTIVITY_CREATED:
            case IFragment::STOPPED:
                if (newState > IFragment::STOPPED) {
                    if (DEBUG) Logger::V(TAG, "moveto STARTED: %p", f);
                    f->PerformStart();
                }
            case IFragment::STARTED:
                if (newState > IFragment::STARTED) {
                    if (DEBUG) Logger::V(TAG, "moveto RESUMED: %p", f);
                    f->SetResumed(TRUE);
                    f->PerformResume();
                    // Get rid of this in case we saved it and never needed it.
                    f->SetSavedFragmentState(NULL);
                    f->SetSavedViewState(NULL);
                }
        }
    } else if (state > newState) {
        switch (state) {
            case IFragment::RESUMED:
                if (newState < IFragment::RESUMED) {
                    if (DEBUG) Logger::V(TAG, "movefrom RESUMED: %p", f);
                    f->PerformPause();
                    f->SetResumed(FALSE);
                }
            case IFragment::STARTED:
                if (newState < IFragment::STARTED) {
                    if (DEBUG) Logger::V(TAG, "movefrom STARTED: %p", f);
                    f->PerformStop();
                }
            case IFragment::STOPPED:
            case IFragment::ACTIVITY_CREATED:
                if (newState < IFragment::ACTIVITY_CREATED) {
                    if (DEBUG) Logger::V(TAG, "movefrom ACTIVITY_CREATED: %p", f);
                    AutoPtr<IView> view;
                    f->GetView((IView**)&view);
                    if (view != NULL) {
                        // Need to save the current view state if not
                        // done already.
                        Boolean finishing;
                        mActivity->IsFinishing(&finishing);
                        AutoPtr<IObjectInt32Map> savedViewState;
                        f->GetSavedViewState((IObjectInt32Map**)&savedViewState);
                        if (!finishing && savedViewState == NULL) {
                            SaveFragmentViewState(f);
                        }
                    }
                    f->PerformDestroyView();
                    AutoPtr<IViewGroup> container;
                    f->GetContainer((IViewGroup**)&container);
                    if (view != NULL && container != NULL) {
                        AutoPtr<IAnimator> anim;
                        if (mCurState > IFragment::INITIALIZING && !mDestroyed) {
                            LoadAnimator(f, transit, FALSE,
                                    transitionStyle, (IAnimator**)&anim);
                        }
                        if (anim != NULL) {
                            AutoPtr<IView> view;
                            f->GetView((IView**)&view);
                            AutoPtr<IFragment> fragment = f;
                            container->StartViewTransition(view);
                            f->SetAnimatingAway(anim);
                            f->SetStateAfterAnimating(newState);
                            AutoPtr<AnimatorListenerAdapter> l = new AnimatorListener(container, view, fragment, this);
                            anim->AddListener(l);
                            anim->SetTarget(view);
                            anim->Start();

                        }
                        IViewManager::Probe(container)->RemoveView(view);
                    }
                    f->SetContainer(NULL);
                    f->SetView(NULL);
                }
            case IFragment::CREATED:
                if (newState < IFragment::CREATED) {
                    if (mDestroyed) {
                        if (animatingAway != NULL) {
                            // The fragment's containing activity is
                            // being destroyed, but this fragment is
                            // currently animating away.  Stop the
                            // animation right now -- it is not needed,
                            // and we can't wait any more on destroying
                            // the fragment.
                            AutoPtr<IAnimator> anim = animatingAway;
                            f->SetAnimatingAway(NULL);
                            anim->Cancel();
                        }
                    }
                    if (animatingAway != NULL) {
                        // We are waiting for the fragment's view to finish
                        // animating away.  Just make a note of the state
                        // the fragment now should move to once the animation
                        // is done.
                        f->SetStateAfterAnimating(newState);
                        newState = IFragment::CREATED;
                    } else {
                        if (DEBUG) Logger::V(TAG, "movefrom CREATED: %p", f);
                        Boolean retaining;
                        f->GetRetaining(&retaining);
                        if (!retaining) {
                            f->PerformDestroy();
                        }

                        f->SetCalled(FALSE);
                        f->OnDetach();
                        Boolean called;
                        f->GetCalled(&called);
                        if (!called) {
//                             throw new SuperNotCalledException("Fragment " + f
//                                     + " did not call through to super.onDetach()");
                            return E_SUPER_NOT_CALLED_EXCEPTION;
                        }
                        if (!keepActive) {
                            if (!retaining) {
                                MakeInactive(f);
                            } else {
                                f->SetActivity(NULL);
                                f->SetParentFragment(NULL);
                                f->SetFragmentManager(NULL);
                            }
                        }
                    }
               }
        }
    }

    f->SetState(newState);
    return NOERROR;
}

ECode CFragmentManagerImpl::MoveToState(
    /* [in] */ IFragment* f)
{
    return MoveToState(f, mCurState, 0, 0, FALSE);
}

ECode CFragmentManagerImpl::MoveToState(
    /* [in] */ Int32 newState,
    /* [in] */ Boolean always)
{
    return MoveToState(newState, 0, 0, always);
}

ECode CFragmentManagerImpl::MoveToState(
    /* [in] */ Int32 newState,
    /* [in] */ Int32 transit,
    /* [in] */ Int32 transitStyle,
    /* [in] */ Boolean always)
{
    if (mActivity == NULL && newState != IFragment::INITIALIZING) {
//         throw new IllegalStateException("No activity");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    if (!always && mCurState == newState) {
        return NOERROR;
    }

    mCurState = newState;
    if (!mActive.IsEmpty()) {
        Boolean loadersRunning = FALSE;
        List<AutoPtr<IFragment> >::Iterator it;
        for (it = mActive.Begin(); it != mActive.End(); ++it) {
            AutoPtr<IFragment> f = *it;
            if (f != NULL) {
                MoveToState(f, newState, transit, transitStyle, FALSE);
                AutoPtr<ILoaderManagerImpl> loaderManager;
                f->GetLoaderManagerValue((ILoaderManagerImpl**)&loaderManager);
                if (loaderManager != NULL) {
                    Boolean hasLoaders;
                    loaderManager->HasRunningLoaders(&hasLoaders);
                    loadersRunning |= hasLoaders;
                }
            }
        }

        if (!loadersRunning) {
            StartPendingDeferredFragments();
        }

        if (mNeedMenuInvalidate && mActivity != NULL && mCurState == IFragment::RESUMED) {
            mActivity->InvalidateOptionsMenu();
            mNeedMenuInvalidate = FALSE;
        }
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::StartPendingDeferredFragments()
{
    if (mActive.IsEmpty()) return NOERROR;

    List<AutoPtr<IFragment> >::Iterator it;
    for (it = mActive.Begin(); it != mActive.End(); ++it) {
        AutoPtr<IFragment> f = *it;
        if (f != NULL) {
            PerformPendingDeferredStart(f);
        }
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::MakeActive(
    /* [in] */ IFragment* f)
{
    Int32 index;
    f->GetIndex(&index);
    if (index >= 0) {
        return NOERROR;
    }

    if (mAvailIndices.IsEmpty()) {
        f->SetIndex(mActive.GetSize(), mParent);
        mActive.PushBack(f);
    }
    else {
        Int32 idx = mAvailIndices.GetBack();
        mAvailIndices.PopBack();

        f->SetIndex(idx, mParent);
        mActive[index] = f;
    }
    if (DEBUG) Logger::V(TAG, "Allocated fragment index %p", f);
    return NOERROR;
}

ECode CFragmentManagerImpl::MakeInactive(
    /* [in] */ IFragment* f)
{
    Int32 index;
    f->GetIndex(&index);
    if (index < 0) {
        return NOERROR;
    }

    if (DEBUG) Logger::V(TAG, "Freeing fragment index %p", f);
    mActive[index] = NULL;
    mAvailIndices.PushBack(index);
    String who;
    f->GetWho(&who);
    AutoPtr<Activity> activity = reinterpret_cast<Activity*>(mActivity->Probe(EIID_Activity));
    activity->InvalidateFragment(who);
    f->InitState();
    return NOERROR;
}

ECode CFragmentManagerImpl::AddFragment(
    /* [in] */ IFragment* fragment,
    /* [in] */ Boolean moveToStateNow)
{
    if (DEBUG) Logger::V(TAG, "add: %p", fragment);
    MakeActive(fragment);
    Boolean detached;
    fragment->IsDetached(&detached);
    if (!detached) {
        Boolean contains = FALSE;
        AutoPtr<IFragment> f = fragment;
        List<AutoPtr<IFragment> >::Iterator it = Find(mAdded.Begin(), mAdded.End(), f);
        if (it != mAdded.End()) contains = TRUE;
        if (contains) {
//             throw new IllegalStateException("Fragment already added: " + fragment);
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        mAdded.PushBack(f);
        fragment->SetAdded(TRUE);
        fragment->SetRemoving(FALSE);
        Boolean hasMenu;
        fragment->GetHasMenu(&hasMenu);
        Boolean menuVisible;
        fragment->GetMenuVisible(&menuVisible);
        if (hasMenu && menuVisible) {
            mNeedMenuInvalidate = TRUE;
        }
        if (moveToStateNow) {
            MoveToState(fragment);
        }
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::RemoveFragment(
    /* [in] */ IFragment* fragment,
    /* [in] */ Int32 transition,
    /* [in] */ Int32 transitionStyle)
{
    Int32 backStackNesting;
    fragment->GetBackStackNesting(&backStackNesting);
    if (DEBUG) Logger::V(TAG, "remove: %p nesting=", fragment, backStackNesting);
    Boolean inbs;
    fragment->IsInBackStack(&inbs);
    Boolean inactive = !inbs;
    Boolean detached;
    fragment->IsDetached(&detached);
    if (!detached || inactive) {
        AutoPtr<IFragment> f = fragment;
        if (FALSE) {
            // Would be nice to catch a bad remove here, but we need
            // time to test this to make sure we aren't crashes cases
            // where it is not a problem.
            Boolean contains = FALSE;
            List<AutoPtr<IFragment> >::Iterator it = Find(mAdded.Begin(), mAdded.End(), f);
            if (it != mAdded.End()) contains = TRUE;
            if (!contains) {
//                 throw new IllegalStateException("Fragment not added: " + fragment);
                return E_ILLEGAL_STATE_EXCEPTION;
            }
        }

        mAdded.Remove(f);

        Boolean hasMenu;
        fragment->GetHasMenu(&hasMenu);
        Boolean menuVisible;
        fragment->GetMenuVisible(&menuVisible);
        if (hasMenu && menuVisible) {
            mNeedMenuInvalidate = TRUE;
        }
        fragment->SetAdded(FALSE);
        fragment->SetRemoving(TRUE);
        MoveToState(fragment, inactive ? IFragment::INITIALIZING : IFragment::CREATED,
                transition, transitionStyle, FALSE);
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::HideFragment(
    /* [in] */ IFragment* fragment,
    /* [in] */ Int32 transition,
    /* [in] */ Int32 transitionStyle)
{
    if (DEBUG) Logger::V(TAG, "hide: %p", fragment);
    Boolean hidden;
    fragment->IsHidden(&hidden);
    if (!hidden) {
        fragment->SetHidden(TRUE);
        AutoPtr<IView> view;
        fragment->GetView((IView**)&view);
        if (view != NULL) {
            AutoPtr<IAnimator> anim;
            LoadAnimator(fragment, transition, TRUE,
                    transitionStyle, (IAnimator**)&anim);
            if (anim != NULL) {
                anim->SetTarget(view);
                // Delay the actual hide operation until the animation finishes, otherwise
                // the fragment will just immediately disappear
                AutoPtr<IFragment> finalFragment = fragment;
                AutoPtr<AnimatorListenerAdapter> l = new AnimatorListener(finalFragment);
                anim->AddListener(l);
                anim->Start();
            } else {
                view->SetVisibility(IView::GONE);
            }
        }
        Boolean added;
        fragment->GetAdded(&added);
        Boolean hasMenu;
        fragment->GetHasMenu(&hasMenu);
        Boolean menuVisible;
        fragment->GetMenuVisible(&menuVisible);
        if (added && hasMenu && menuVisible) {
            mNeedMenuInvalidate = TRUE;
        }
        fragment->OnHiddenChanged(TRUE);
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::ShowFragment(
    /* [in] */ IFragment* fragment,
    /* [in] */ Int32 transition,
    /* [in] */ Int32 transitionStyle)
{
    if (DEBUG) Logger::V(TAG, "show: %p", fragment);
    Boolean hidden;
    fragment->IsHidden(&hidden);
    if (hidden) {
        fragment->SetHidden(FALSE);
        AutoPtr<IView> view;
        fragment->GetView((IView**)&view);
        if (view != NULL) {
            AutoPtr<IAnimator> anim;
            LoadAnimator(fragment, transition, TRUE,
                    transitionStyle, (IAnimator**)&anim);
            if (anim != NULL) {
                anim->SetTarget(view);
                anim->Start();
            }
            view->SetVisibility(IView::VISIBLE);
        }
        Boolean added;
        fragment->GetAdded(&added);
        Boolean hasMenu;
        fragment->GetHasMenu(&hasMenu);
        Boolean menuVisible;
        fragment->GetMenuVisible(&menuVisible);
        if (added && hasMenu && menuVisible) {
            mNeedMenuInvalidate = TRUE;
        }
        fragment->OnHiddenChanged(FALSE);
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::DetachFragment(
    /* [in] */ IFragment* fragment,
    /* [in] */ Int32 transition,
    /* [in] */ Int32 transitionStyle)
{
    if (DEBUG) Logger::V(TAG, "detach: %p", fragment);
    Boolean detached;
    fragment->IsDetached(&detached);
    if (!detached) {
        fragment->SetDetached(TRUE);
        Boolean added;
        fragment->GetAdded(&added);
        if (added) {
            // We are not already in back stack, so need to remove the fragment.
            if (!mAdded.IsEmpty()) {
                if (DEBUG) Logger::V(TAG, "remove from detach: %p", fragment);
                mAdded.Remove(fragment);
            }
            Boolean hasMenu;
            fragment->GetHasMenu(&hasMenu);
            Boolean menuVisible;
            fragment->GetMenuVisible(&menuVisible);
            if (hasMenu && menuVisible) {
                mNeedMenuInvalidate = TRUE;
            }
            fragment->SetAdded(FALSE);
            MoveToState(fragment, IFragment::CREATED, transition, transitionStyle, FALSE);
        }
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::AttachFragment(
    /* [in] */ IFragment* fragment,
    /* [in] */ Int32 transition,
    /* [in] */ Int32 transitionStyle)
{
    if (DEBUG) Logger::V(TAG, "attach: %p", fragment);
    Boolean detached;
    fragment->IsDetached(&detached);
    if (detached) {
        fragment->SetDetached(FALSE);
        Boolean added;
        fragment->GetAdded(&added);
        if (!added) {
            Boolean contains = FALSE;
            AutoPtr<IFragment> f = fragment;
            List<AutoPtr<IFragment> >::Iterator it = Find(mAdded.Begin(), mAdded.End(), f);
            if (it != mAdded.End()) contains = TRUE;
            if (contains) {
//                 throw new IllegalStateException("Fragment already added: " + fragment);
                return E_ILLEGAL_STATE_EXCEPTION;
            }
            if (DEBUG) Logger::V(TAG, "add from attach: %p", fragment);
            mAdded.PushBack(f);
            fragment->SetAdded(TRUE);
            Boolean hasMenu;
            fragment->GetHasMenu(&hasMenu);
            Boolean menuVisible;
            fragment->GetMenuVisible(&menuVisible);
            if (hasMenu && menuVisible) {
                mNeedMenuInvalidate = TRUE;
            }
            MoveToState(fragment, mCurState, transition, transitionStyle, FALSE);
        }
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::FindFragmentById(
    /* [in] */ Int32 id,
    /* [out] */ IFragment** fragment)
{
    VALIDATE_NOT_NULL(fragment);
    if (!mAdded.IsEmpty()) {
        // First look through added fragments.
        List<AutoPtr<IFragment> >::ReverseIterator rit;
        for (rit = mAdded.RBegin(); rit != mAdded.REnd(); ++rit) {
            AutoPtr<IFragment> f = *rit;
            Int32 fId;
            f->GetFragmentId(&fId);
            if (f != NULL && fId == id) {
                *fragment = f;
                REFCOUNT_ADD(*fragment);
                return NOERROR;
            }
        }
    }
    if (!mActive.IsEmpty()) {
        // Now for any known fragment.
        List<AutoPtr<IFragment> >::ReverseIterator rit;
        for (rit = mAdded.RBegin(); rit != mAdded.REnd(); ++rit) {
            AutoPtr<IFragment> f = *rit;
            Int32 fId;
            f->GetFragmentId(&fId);
            if (f != NULL && fId == id) {
                *fragment = f;
                REFCOUNT_ADD(*fragment);
                return NOERROR;
            }
        }
    }
    *fragment = NULL;
    return NOERROR;
}

ECode CFragmentManagerImpl::FindFragmentByTag(
    /* [in] */ const String& tag,
    /* [out] */ IFragment** fragment)
{
    VALIDATE_NOT_NULL(fragment);
    if (!mAdded.IsEmpty() && tag != NULL) {
        // First look through added fragments.
        List<AutoPtr<IFragment> >::ReverseIterator rit;
        for (rit = mAdded.RBegin(); rit != mAdded.REnd(); ++rit) {
            AutoPtr<IFragment> f = *rit;
            String ftag;
            f->GetTag(&ftag);
            if (f != NULL && tag.Equals(ftag)) {
                *fragment = f;
                REFCOUNT_ADD(*fragment);
                return NOERROR;
            }
        }
    }
    if (!mActive.IsEmpty() && tag != NULL) {
        // Now for any known fragment.
        List<AutoPtr<IFragment> >::ReverseIterator rit;
        for (rit = mAdded.RBegin(); rit != mAdded.REnd(); ++rit) {
            AutoPtr<IFragment> f = *rit;
            String ftag;
            f->GetTag(&ftag);
            if (f != NULL && tag.Equals(ftag)) {
                *fragment = f;
                REFCOUNT_ADD(*fragment);
                return NOERROR;
            }
        }
    }
    *fragment = NULL;
    return NOERROR;
}

ECode CFragmentManagerImpl::FindFragmentByWho(
    /* [in] */ const String& who,
    /* [out] */ IFragment** fragment)
{
    VALIDATE_NOT_NULL(fragment);
    if (!mActive.IsEmpty() && who != NULL) {
        List<AutoPtr<IFragment> >::ReverseIterator rit;
        for (rit = mAdded.RBegin(); rit != mAdded.REnd(); ++rit) {
            AutoPtr<IFragment> f = *rit;
            AutoPtr<IFragment> fwho;
            f->FindFragmentByWho(who, (IFragment**)&fwho);
            if (f != NULL && (f=fwho) != NULL) {
                *fragment = f;
                REFCOUNT_ADD(*fragment);
                return NOERROR;
            }
        }
    }
    *fragment = NULL;
    return NOERROR;
}

ECode CFragmentManagerImpl::CheckStateLoss()
{
    if (mStateSaved) {
        Logger::V(TAG, "Can not perform this action after onSaveInstanceState");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    if (!mNoTransactionsBecause.IsNull()) {
        Logger::V(TAG, "Can not perform this action inside of %s", mNoTransactionsBecause.string());
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::EnqueueAction(
    /* [in] */ IRunnable* action,
    /* [in] */ Boolean allowStateLoss)
{
    if (!allowStateLoss) {
        FAIL_RETURN(CheckStateLoss());
    }
    {
        AutoLock lock(mThisLock);
        if (mActivity == NULL) {
//             throw new IllegalStateException("Activity has been destroyed");
            return E_ILLEGAL_STATE_EXCEPTION;
        }

        mPendingActions.PushBack(action);
        if (mPendingActions.GetSize() == 1) {
            AutoPtr<Activity> activity = reinterpret_cast<Activity*>(mActivity->Probe(EIID_Activity));
            activity->mHandler->RemoveCallbacks(IRunnable::Probe(mExecCommit));
            Boolean result;
            FAIL_RETURN(activity->mHandler->Post(IRunnable::Probe(mExecCommit), &result) );
        }
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::AllocBackStackIndex(
    /* [in] */ IBackStackRecord* bse,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    AutoLock lock(mThisLock);

    if (mAvailBackStackIndices.IsEmpty()) {
        Int32 size = mBackStackIndices.GetSize();
        if (DEBUG) Logger::V(TAG, "Setting back stack index %d to %p", size, bse);
        mBackStackIndices.PushBack(bse);
        *index = size;
        return NOERROR;
    }
    else {
        Int32 idx = mAvailBackStackIndices.GetBack();
        mAvailBackStackIndices.PopBack();
        if (DEBUG) Logger::V(TAG, "Adding back stack index %d with %p", idx, bse);
        mBackStackIndices.PushBack(bse);
        *index = idx;
        return NOERROR;
    }
}

ECode CFragmentManagerImpl::SetBackStackIndex(
    /* [in] */ Int32 index,
    /* [in] */ IBackStackRecord* bse)
{
    AutoLock lock(mThisLock);

    Int32 N = mBackStackIndices.GetSize();
    if (index < N) {
        if (DEBUG) Logger::V(TAG, "Setting back stack index %d to %p", index, bse);
        mBackStackIndices[index] = bse;
    }
    else {
        while (N < index) {
            mBackStackIndices.PushBack(NULL);
            if (DEBUG) Logger::V(TAG, "Adding available back stack index %d", N);
            mAvailBackStackIndices.PushBack(N);
            N++;
        }

        if (DEBUG) Logger::V(TAG, "Adding back stack index %d with %p", index, bse);
        mBackStackIndices.PushBack(bse);
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::FreeBackStackIndex(
    /* [in] */ Int32 index)
{
    AutoLock look(mThisLock);
//TODO:
//    (*mBackStackIndices)[index] = NULL;

    if (DEBUG) Logger::V(TAG, "Freeing back stack index %d", index);
    mAvailBackStackIndices.PushBack(index);
    return NOERROR;
}

ECode CFragmentManagerImpl::ExecPendingActions(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (mExecutingActions) {
//         throw new IllegalStateException("Recursive entry to executePendingTransactions");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr<Activity> activity = reinterpret_cast<Activity*>(mActivity->Probe(EIID_Activity));
    AutoPtr<ILooper> myLooper = Looper::GetMyLooper();
    AutoPtr<ILooper> aLooper;
    activity->mHandler->GetLooper((ILooper**)&aLooper);
    if (myLooper.Get() != aLooper.Get()) {
        return E_ILLEGAL_STATE_EXCEPTION;
        // throw new IllegalStateException("Must be called from main thread of process");
    }

    Boolean didSomething = FALSE;
    Int32 numActions, index;
    List<AutoPtr<IRunnable> >::Iterator it;
    while (TRUE) {
        {
            AutoLock lock(mThisLock);
            numActions = mPendingActions.GetSize();
            if (numActions == 0) {
                break;
            }

            if (mTmpActions == NULL || mTmpActions->GetLength() < numActions) {
                mTmpActions = NULL;
                mTmpActions = ArrayOf<IRunnable*>::Alloc(numActions);
            }

            index = 0;
            for (it = mPendingActions.Begin(); it != mPendingActions.End(); ++it, ++index) {
                mTmpActions->Set(index, *it);
            }
            if (mTmpActions->GetLength() > numActions) {
                mTmpActions->Set(numActions, NULL);
            }
            mPendingActions.Clear();
            activity->mHandler->RemoveCallbacks(mExecCommit);
        }

        mExecutingActions = TRUE;
        for (Int32 i = 0; i < numActions; i++) {
            (*mTmpActions)[i]->Run();
            mTmpActions->Set(i, NULL);
        }
        mExecutingActions = FALSE;
        didSomething = TRUE;
    }

    if (mHavePendingDeferredStart) {
        Boolean loadersRunning = FALSE;
        List<AutoPtr<IFragment> >::Iterator it;
        for (it = mActive.Begin(); it != mActive.End(); ++it) {
            AutoPtr<IFragment> f = *it;
            AutoPtr<ILoaderManagerImpl> loaderManager;
            f->GetLoaderManagerValue((ILoaderManagerImpl**)&loaderManager);
            if (f != NULL && loaderManager != NULL) {
                Boolean hasRLoaders;
                loaderManager->HasRunningLoaders(&hasRLoaders);
                loadersRunning |= hasRLoaders;
            }
        }
        if (!loadersRunning) {
            mHavePendingDeferredStart = FALSE;
            StartPendingDeferredFragments();
        }
    }
    *result = didSomething;
    return NOERROR;
}

ECode CFragmentManagerImpl::ReportBackStackChanged()
{
    List<AutoPtr<IFragmentManagerOnBackStackChangedListener> >::Iterator it;
    for (it = mBackStackChangeListeners.Begin(); it != mBackStackChangeListeners.End(); ++it) {
        (*it)->OnBackStackChanged();
    }

    return NOERROR;
}

ECode CFragmentManagerImpl::AddBackStackState(
    /* [in] */ IBackStackRecord* state)
{
    mBackStack.PushBack(state);
    ReportBackStackChanged();
    return NOERROR;
}

ECode CFragmentManagerImpl::PopBackStackState(
    /* [in] */ IHandler* handler,
    /* [in] */ const String& name,
    /* [in] */ Int32 id,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (name.IsNull() && id < 0 && (flags&POP_BACK_STACK_INCLUSIVE) == 0) {
        if (mBackStack.IsEmpty()) {
            *result = FALSE;
            return NOERROR;
        }
        List<AutoPtr<IBackStackRecord> >::Iterator bss = mBackStack.Erase(--mBackStack.End());
        (*bss)->PopFromBackStack(TRUE);
        ReportBackStackChanged();
    }
    else {
        List<AutoPtr<IBackStackRecord> >::ReverseIterator index;
        if (!name.IsNull() || id >= 0) {
            // If a name or ID is specified, look for that place in
            // the stack.
            index = mBackStack.RBegin();
            while (*index != NULL && index != mBackStack.REnd()) {
                AutoPtr<IBackStackRecord> bss = *index;
                String bssName;
                bss->GetName(&bssName);
                if (!name.IsNull() && name.Equals(bssName)) {
                    break;
                }
                Int32 bssIndex;
                bss->GetId(&bssIndex);
                if (id >= 0 && id == bssIndex) {
                    break;
                }
                ++index;
            }
            if (*index == NULL) {
                *result = FALSE;
                return NOERROR;
            }
            if ((flags&POP_BACK_STACK_INCLUSIVE) != 0) {
                index++;
                // Consume all following entries that match.
                while (index != mBackStack.REnd()) {
                    AutoPtr<IBackStackRecord> bss = *index;
                    String bssName;
                    Int32 bssIndex;
                    bss->GetName(&bssName);
                    bss->GetId(&bssIndex);
                    if ((!name.IsNull() && name.Equals(bssName))
                            || (id >= 0 && id == bssIndex)) {
                        index++;
                        continue;
                    }
                    break;
                }
            }
        }
        if (index == mBackStack.RBegin()) {
            *result = FALSE;
            return NOERROR;
        }
        List<AutoPtr<IBackStackRecord> > states;
        List<AutoPtr<IBackStackRecord> >::ReverseIterator rIt;
        for (rIt = mBackStack.RBegin(); rIt != mBackStack.REnd();) {
            states.PushBack(*rIt);
            rIt = List<AutoPtr<IBackStackRecord> >::ReverseIterator(mBackStack.Erase(--(rIt.GetBase())));
        }
        Int32 LAST = states.GetSize()-1;
        for (Int32 i = 0; i <= LAST; i++) {
            if (DEBUG) Logger::V(TAG, "Popping back stack state: %p", states[i].Get());
            states[i]->PopFromBackStack(i == LAST);
        }
        ReportBackStackChanged();
    }
    *result = TRUE;
    return NOERROR;
}

ECode CFragmentManagerImpl::RetainNonConfig(
    /* [out] */ IObjectContainer** retains)
{
    VALIDATE_NOT_NULL(retains);
    List<AutoPtr<IFragment> > fragments;
    if (!mActive.IsEmpty()) {
        List<AutoPtr<IFragment> >::Iterator it;
        for (it = mActive.Begin(); it != mActive.End(); ++it) {
            AutoPtr<IFragment> f = *it;
            Boolean retainInstance;
            f->GetRetainInstance(&retainInstance);
            if (f != NULL && retainInstance) {
                fragments.PushBack(f);
                f->SetRetaining(TRUE);
                AutoPtr<IFragment> target;
                f->GetTarget((IFragment**)&target);
                Int32 index;
                target->GetIndex(&index);
                if (target != NULL) f->SetTargetIndex(index);
                else f->SetTargetIndex(-1);
                if (DEBUG) Logger::V(TAG, "retainNonConfig: keeping retained %p", f.Get());
            }
        }
    }
    List<AutoPtr<IFragment> >::Iterator fit;
    Int32 i = 0;
    AutoPtr<IObjectContainer> tempRetains = NULL;
    for (fit = fragments.Begin(); fit != fragments.End(); ++fit, ++i) {
        tempRetains->Add((*fit)->Probe(EIID_IInterface));
    }
    *retains = tempRetains;
    REFCOUNT_ADD(*retains);
    return NOERROR;
}

ECode CFragmentManagerImpl::SaveFragmentViewState(
    /* [in] */ IFragment* f)
{
    AutoPtr<IView> view;
    f->GetView((IView**)&view);
    if (view == NULL) {
        return NOERROR;
    }
    if (mStateArray == NULL) {
        CObjectInt32Map::New((IObjectInt32Map**)&mStateArray);
    }
    else {
        mStateArray = NULL;
    }

    view->SaveHierarchyState(mStateArray);
    Int32 size;
    mStateArray->GetSize(&size);
    if (size > 0) {
        f->SetSavedViewState(mStateArray);
        mStateArray = NULL;
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::SaveFragmentBasicState(
    /* [in] */ IFragment* f,
    /* [out] */ IBundle** rst)
{
    VALIDATE_NOT_NULL(rst);
    AutoPtr<IBundle> result = NULL;

    if (mStateBundle == NULL) {
        CBundle::New((IBundle**)&mStateBundle);
    }
    f->PerformSaveInstanceState(mStateBundle);
    Boolean isEmpty;
    mStateBundle->IsEmpty(&isEmpty);
    if (!isEmpty) {
        result = mStateBundle;
        mStateBundle = NULL;
    }

    AutoPtr<IView> view;
    f->GetView((IView**)&view);
    if (view != NULL) {
        SaveFragmentViewState(f);
    }
    AutoPtr<IObjectInt32Map> savedViewState;
    f->GetSavedViewState((IObjectInt32Map**)&savedViewState);
    if (savedViewState != NULL) {
        if (result == NULL) {
            CBundle::New((IBundle**)&result);
        }
        result->PutParcelableMap(
                IFragmentManagerImpl::VIEW_STATE_TAG, savedViewState->Probe(EIID_IInterface));
    }
    Boolean userVisibleHint;
    f->GetUserVisibleHint(&userVisibleHint);
    if (!userVisibleHint) {
        if (result == NULL) {
            CBundle::New((IBundle**)&result);
        }
        // Only add this if it's not the default value
        result->PutBoolean(IFragmentManagerImpl::USER_VISIBLE_HINT_TAG, userVisibleHint);
    }

    *rst = result;
    REFCOUNT_ADD(*rst);
    return NOERROR;
}

ECode CFragmentManagerImpl::SaveAllState(
    /* [out] */ IParcelable** state)
{
    VALIDATE_NOT_NULL(state);
    // Make sure all pending operations have now been executed to get
    // our state update-to-date.
    Boolean executed;
    ExecPendingActions(&executed);

    mStateSaved = TRUE;

    if (mActive.IsEmpty()) {
        *state = NULL;
        return NOERROR;
    }

    // First collect all active fragments.
    Int32 N = mActive.GetSize();
    AutoPtr<ArrayOf<FragmentState*> > active = ArrayOf<FragmentState*>::Alloc(N);
    Boolean haveFragments = FALSE;
    List<AutoPtr<IFragment> >::Iterator it = mActive.Begin();
    for (Int32 i = 0; i < N && it != mActive.End(); ++it, i++) {
        AutoPtr<IFragment> f = *it;
        if (f != NULL) {
            Int32 index;
            f->GetIndex(&index);
            if (index < 0) {
                Logger::V(TAG, "Failure saving state: active %p has cleared index: %d", f.Get(), index);
                return E_ILLEGAL_STATE_EXCEPTION;
            }

            haveFragments = TRUE;

            AutoPtr<FragmentState> fs = new FragmentState(f);
            active->Set(i, fs);

            Int32 fstate;
            f->GetState(&fstate);
            if (fstate > IFragment::INITIALIZING && fs->mSavedFragmentState == NULL) {
                SaveFragmentBasicState(f, (IBundle**)&fs->mSavedFragmentState);

                AutoPtr<IFragment> t;
                f->GetTarget((IFragment**)&t);
                if (t != NULL) {
                    Int32 tIndex = 0;
                    t->GetIndex(&tIndex);
                    if (tIndex < 0) {
                        Logger::V(TAG, "Failure saving state: %p has target not in fragment manager: %p", f.Get(), t.Get());
                        return E_ILLEGAL_STATE_EXCEPTION;
                    }
                    if (fs->mSavedFragmentState == NULL) {
                        CBundle::New((IBundle**)&fs->mSavedFragmentState);
                    }

                    PutFragment(fs->mSavedFragmentState,
                            IFragmentManagerImpl::TARGET_STATE_TAG, t);

                    Int32 code = 0;
                    f->GetTargetRequestCode(&code);
                    if (code != 0) {
                        fs->mSavedFragmentState->PutInt32(
                                IFragmentManagerImpl::TARGET_REQUEST_CODE_STATE_TAG, code);
                    }
                }

            } else {
                fs->mSavedFragmentState = NULL;
                f->GetSavedFragmentState((IBundle**)&fs->mSavedFragmentState);
            }

            if (DEBUG) Logger::V(TAG, "Saved state of %p:", f.Get(), fs->mSavedFragmentState.Get());
        }
    }

    if (!haveFragments) {
        if (DEBUG) Logger::V(TAG, "saveAllState: no fragments!");
        *state = NULL;
        return NOERROR;
    }

    AutoPtr<ArrayOf<Int32> > added;
    AutoPtr<ArrayOf<IBackStackState*> > backStack;

    // Build list of currently added fragments.
    if (!mAdded.IsEmpty()) {
        N = mAdded.GetSize();
        if (N > 0) {
            added = ArrayOf<Int32>::Alloc(N);
            List<AutoPtr<IFragment> >::Iterator ator = mAdded.Begin();
            Int32 index = 0;
            for (Int32 i = 0; i < N && ator != mAdded.End(); i++, ++ator) {
                (*ator)->GetIndex(&index);
                (*added)[i] = index;
                if ((*added)[i] < 0) {
                    Logger::V(TAG, "Failure saving state: active %p has cleared index: %d", (*ator).Get(), index);
                    return E_ILLEGAL_STATE_EXCEPTION;
                }
                if (DEBUG) Logger::V(TAG, "saveAllState: adding fragment #%d: %p", i, (*ator).Get());
            }
        }
    }

    // Now save back stack.
    N = mBackStack.GetSize();
    if (N > 0) {
        backStack = ArrayOf<IBackStackState*>::Alloc(N);
        List<AutoPtr<IBackStackRecord> >::Iterator ator = mBackStack.Begin();
        for (Int32 i = 0; i < N && ator != mBackStack.End(); i++, ++ator) {
            AutoPtr<IBackStackState> value;
            CBackStackState::New(this, *ator, (IBackStackState**)&value);
            backStack->Set(i, value);
            if (DEBUG) Logger::V(TAG, "saveAllState: adding back stack #%d: %p", i, (*ator).Get());
        }
    }

    AutoPtr<CFragmentManagerState> fms;
    CFragmentManagerState::NewByFriend((CFragmentManagerState**)&fms);
    fms->mActive = active;
    fms->mAdded = added;
    fms->mBackStack = backStack;
    *state = (IParcelable*)fms->Probe(EIID_IParcelable);
    REFCOUNT_ADD(*state);
    return NOERROR;
}

ECode CFragmentManagerImpl::RestoreAllState(
    /* [in] */ IParcelable* state,
    /* [in] */ IObjectContainer* nonConfig)
{
    // If there is no saved state at all, then there can not be
    // any nonConfig fragments either, so that is that.
    if (state == NULL) return NOERROR;
    CFragmentManagerState* fms = (CFragmentManagerState*)state;
    if (fms->mActive == NULL) return NOERROR;

    // First re-attach any non-config instances we are retaining back
    // to their saved state, so we don't try to instantiate them again.
    if (nonConfig != NULL) {
        AutoPtr<IObjectEnumerator> ator;
        nonConfig->GetObjectEnumerator((IObjectEnumerator**)&ator);
        Boolean hasNext = FALSE;
        while(ator->MoveNext(&hasNext), hasNext) {
            AutoPtr<IInterface> data;
            ator->Current((IInterface**)&data);
            AutoPtr<IFragment> f = IFragment::Probe(data);
            assert(f != NULL);
            if (DEBUG) Logger::V(TAG, "restoreAllState: re-attaching retained %p", f.Get());
            Int32 index = 0;
            f->GetIndex(&index);
            AutoPtr<FragmentState> fs = (*fms->mActive)[index];
            fs->mInstance = f;
            f->SetSavedViewState(NULL);
            f->SetBackStackNesting(0);
            f->SetInLayout(FALSE);
            f->SetAdded(FALSE);
            f->SetTarget(NULL);
            if (fs->mSavedFragmentState != NULL) {
                AutoPtr<IClassLoader> classLoader;
                mActivity->GetClassLoader((IClassLoader**)&classLoader);
                fs->mSavedFragmentState->SetClassLoader(classLoader);

                AutoPtr<IObjectInt32Map> map;
                fs->mSavedFragmentState->GetSparseParcelableArray(
                        IFragmentManagerImpl::VIEW_STATE_TAG, (IObjectInt32Map**)&map);
                f->SetSavedViewState(map);
            }
        }
    }

    // Build the full list of active fragments, instantiating them from
    // their saved state.
    mActive.Clear();
    if (!mAvailIndices.IsEmpty()) {
        mAvailIndices.Clear();
    }
    for (Int32 i = 0; i < fms->mActive->GetLength(); i++) {
        AutoPtr<FragmentState> fs = (*fms->mActive)[i];
        if (fs != NULL) {
            AutoPtr<IFragment> f;
            fs->Instantiate(mActivity, mParent, (IFragment**)&f);
            if (DEBUG) Logger::V(TAG, "restoreAllState: active #%d: %p", i , f.Get());
            mActive.PushBack(f);
            // Now that the fragment is instantiated (or came from being
            // retained above), clear mInstance in case we end up re-restoring
            // from this FragmentState again.
            fs->mInstance = NULL;
        } else {
            mActive.PushBack(NULL);
            if (DEBUG) Logger::V(TAG, "restoreAllState: avail #%d", i);
            mAvailIndices.PushBack(i);
        }
    }

    // Update the target of all retained fragments.
    if (nonConfig != NULL) {
        AutoPtr<IObjectEnumerator> ator;
        nonConfig->GetObjectEnumerator((IObjectEnumerator**)&ator);
        Boolean hasNext = FALSE;
        while(ator->MoveNext(&hasNext), hasNext) {
            AutoPtr<IInterface> data;
            ator->Current((IInterface**)&data);
            AutoPtr<IFragment> f = IFragment::Probe(data);
            Int32 targetIndex = 0;
            f->GetTargetIndex(&targetIndex);
            if (targetIndex >= 0) {
                if (targetIndex < (Int32)mActive.GetSize()) {
                    f->SetTarget(mActive[targetIndex]);
                } else {
                    Logger::W(TAG, "Re-attaching retained fragment %p target no longer exists: %d", f.Get(), targetIndex);
                    f->SetTarget(NULL);
                }
            }
        }
    }

    // Build the list of currently added fragments.
    if (fms->mAdded != NULL) {
        mAdded.Clear();
        for (Int32 i = 0; i < fms->mAdded->GetLength(); i++) {
            AutoPtr<IFragment> f = mActive[(*fms->mAdded)[i]];
            if (f == NULL) {
                Logger::W(TAG, "No instantiated fragment for index #%d", (*fms->mAdded)[i]);
                return E_ILLEGAL_STATE_EXCEPTION;
            }
            f->SetAdded(TRUE);
            if (DEBUG) Logger::V(TAG, "restoreAllState: added #%d: %p", i, f.Get());

            List<AutoPtr<IFragment> >::Iterator ator = mAdded.Begin();
            for (; ator != mAdded.End(); ++ator) {
                if (*ator == f) {
                    Logger::W(TAG, "Already added!");
                    return E_ILLEGAL_STATE_EXCEPTION;
                }
            }
            mAdded.PushBack(f);
        }
    } else {
        mAdded.Clear();
    }

    // Build the back stack.
    if (fms->mBackStack != NULL) {
        mBackStack.Clear();
        for (Int32 i = 0; i < fms->mBackStack->GetLength(); i++) {
            AutoPtr<IBackStackRecord> bse;
            (*fms->mBackStack)[i]->Instantiate(this, (IBackStackRecord**)&bse);
            if (DEBUG) {
                Logger::V(TAG, "restoreAllState: back stack #%d (index %d): %p",
                    i, ((BackStackRecord*)bse.Get())->mIndex, bse.Get());
                // TODO
                // LogWriter logw = new LogWriter(Log.VERBOSE, TAG);
                // PrintWriter pw = new PrintWriter(logw);
                // bse.dump("  ", pw, FALSE);
            }
            mBackStack.PushBack(bse);
            if (((BackStackRecord*)bse.Get())->mIndex >= 0) {
                SetBackStackIndex(((BackStackRecord*)bse.Get())->mIndex, bse);
            }
        }
    } else {
        mBackStack.Clear();
    }

    return NOERROR;
}

ECode CFragmentManagerImpl::AttachActivity(
    /* [in] */ IActivity* activity,
    /* [in] */ IFragmentContainer* container,
    /* [in] */ IFragment* parent)
{
    if (mActivity != NULL) //throw new IllegalStateException("Already attached");
        return E_ILLEGAL_STATE_EXCEPTION;
    mActivity = activity;
    mContainer = container;
    mParent = parent;
    return NOERROR;
}

ECode CFragmentManagerImpl::NoteStateNotSaved()
{
    mStateSaved = FALSE;
    return NOERROR;
}

ECode CFragmentManagerImpl::DispatchCreate()
{
    mStateSaved = FALSE;
    return MoveToState(IFragment::CREATED, FALSE);
}

ECode CFragmentManagerImpl::DispatchActivityCreated()
{
    mStateSaved = FALSE;
    return MoveToState(IFragment::ACTIVITY_CREATED, FALSE);
}

ECode CFragmentManagerImpl::DispatchStart()
{
    mStateSaved = FALSE;
    return MoveToState(IFragment::STARTED, FALSE);
}

ECode CFragmentManagerImpl::DispatchResume()
{
    mStateSaved = FALSE;
    return MoveToState(IFragment::RESUMED, FALSE);
}

ECode CFragmentManagerImpl::DispatchPause()
{
    return MoveToState(IFragment::STARTED, FALSE);
}

ECode CFragmentManagerImpl::DispatchStop()
{
    return MoveToState(IFragment::STOPPED, FALSE);
}

ECode CFragmentManagerImpl::DispatchDestroyView()
{
    return MoveToState(IFragment::CREATED, FALSE);
}

ECode CFragmentManagerImpl::DispatchDestroy()
{
    mDestroyed = TRUE;
    Boolean executed;
    ExecPendingActions(&executed);
    MoveToState(IFragment::INITIALIZING, FALSE);
    mActivity = NULL;
    mContainer = NULL;
    mParent = NULL;
    return NOERROR;
}

ECode CFragmentManagerImpl::DispatchConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    if (!mAdded.IsEmpty()) {
        List<AutoPtr<IFragment> >::Iterator it;
        for (it = mAdded.Begin(); it != mAdded.End(); ++it) {
            AutoPtr<IFragment> f = *it;
            if (f != NULL) {
                f->PerformConfigurationChanged(newConfig);
            }
        }
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::DispatchLowMemory()
{
    if (!mAdded.IsEmpty()) {
        List<AutoPtr<IFragment> >::Iterator it;
        for (it = mAdded.Begin(); it != mAdded.End(); ++it) {
            AutoPtr<IFragment> f = *it;
            if (f != NULL) {
                f->PerformLowMemory();
            }
        }
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::DispatchTrimMemory(
    /* [in] */ Int32 level)
{
    if (!mAdded.IsEmpty()) {
        List<AutoPtr<IFragment> >::Iterator it;
        for (it = mAdded.Begin(); it != mAdded.End(); ++it) {
            AutoPtr<IFragment> f = *it;
            if (f != NULL) {
                f->PerformTrimMemory(level);
            }
        }
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::DispatchCreateOptionsMenu(
    /* [in] */ IMenu* menu,
    /* [in] */ IMenuInflater* inflater,
    /* [out] */ Boolean* rst)
{
    VALIDATE_NOT_NULL(rst);
    Boolean show = FALSE;
    List<AutoPtr<IFragment> > newMenus;
    if (!mAdded.IsEmpty()) {
        List<AutoPtr<IFragment> >::Iterator it;
        for (it = mAdded.Begin(); it != mAdded.End(); ++it) {
            AutoPtr<IFragment> f = *it;
            if (f != NULL) {
                Boolean created;
                f->PerformCreateOptionsMenu(menu, inflater, &created);
                if (created) {
                    show = TRUE;
                    newMenus.PushBack(f);
                }
            }
        }
    }

    if (!mCreatedMenus.IsEmpty()) {
        List<AutoPtr<IFragment> >::Iterator it;
        for (it = mCreatedMenus.Begin(); it != mCreatedMenus.End(); ++it) {
            AutoPtr<IFragment> f = *it;
            Boolean contains = FALSE;
            List<AutoPtr<IFragment> >::Iterator mIt = Find(newMenus.Begin(), newMenus.End(), f);
            if (mIt != newMenus.End()) contains = TRUE;
            if (newMenus.IsEmpty() || !contains) {
                f->OnDestroyOptionsMenu();
            }
        }
    }

    mCreatedMenus.Clear();
    Copy(newMenus.Begin(), newMenus.End(), mCreatedMenus.Begin());

    *rst = show;
    return NOERROR;
}

ECode CFragmentManagerImpl::DispatchPrepareOptionsMenu(
    /* [in] */ IMenu* menu,
    /* [out] */ Boolean* rst)
{
    VALIDATE_NOT_NULL(rst);
    Boolean show = FALSE;
    if (!mAdded.IsEmpty()) {
        List<AutoPtr<IFragment> >::Iterator it;
        for (it = mAdded.Begin(); it != mAdded.End(); ++it) {
            AutoPtr<IFragment> f = *it;
            if (f != NULL) {
                Boolean prepared;
                f->PerformPrepareOptionsMenu(menu, &prepared);
                if (prepared) {
                    show = TRUE;
                }
            }
        }
    }
    *rst = show;
    return NOERROR;
}

ECode CFragmentManagerImpl::DispatchOptionsItemSelected(
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* rst)
{
    VALIDATE_NOT_NULL(rst);
    if (!mAdded.IsEmpty()) {
        List<AutoPtr<IFragment> >::Iterator it;
        for (it = mAdded.Begin(); it != mAdded.End(); ++it) {
            AutoPtr<IFragment> f = *it;
            if (f != NULL) {
                Boolean selected;
                f->PerformOptionsItemSelected(item, &selected);
                if (selected) {
                    *rst = TRUE;
                    return NOERROR;
                }
            }
        }
    }
    *rst = FALSE;
    return NOERROR;
}

ECode CFragmentManagerImpl::DispatchContextItemSelected(
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* rst)
{
    if (!mAdded.IsEmpty()) {
        List<AutoPtr<IFragment> >::Iterator it;
        for (it = mAdded.Begin(); it != mAdded.End(); ++it) {
            AutoPtr<IFragment> f = *it;
            if (f != NULL) {
                Boolean selected;
                f->PerformContextItemSelected(item, &selected);
                if (selected) {
                    *rst = TRUE;
                    return NOERROR;
                }
            }
        }
    }
    *rst = FALSE;
    return NOERROR;
}

ECode CFragmentManagerImpl::DispatchOptionsMenuClosed(
    /* [in] */ IMenu* menu)
{
    if (!mAdded.IsEmpty()) {
        List<AutoPtr<IFragment> >::Iterator it;
        for (it = mAdded.Begin(); it != mAdded.End(); ++it) {
            AutoPtr<IFragment> f = *it;
            if (f != NULL) {
                f->PerformOptionsMenuClosed(menu);
            }
        }
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::InvalidateOptionsMenu()
{
    if (mActivity != NULL && mCurState == IFragment::RESUMED) {
        mActivity->InvalidateOptionsMenu();
    } else {
        mNeedMenuInvalidate = TRUE;
    }
    return NOERROR;
}

ECode CFragmentManagerImpl::ReverseTransit(
    /* [in] */ Int32 transit,
    /* [out] */ Int32* reverse)
{
    VALIDATE_NOT_NULL(reverse);
    Int32 rev = 0;
    switch (transit) {
        case IFragmentTransaction::TRANSIT_FRAGMENT_OPEN:
            rev = IFragmentTransaction::TRANSIT_FRAGMENT_CLOSE;
            break;
        case IFragmentTransaction::TRANSIT_FRAGMENT_CLOSE:
            rev = IFragmentTransaction::TRANSIT_FRAGMENT_OPEN;
            break;
        case IFragmentTransaction::TRANSIT_FRAGMENT_FADE:
            rev = IFragmentTransaction::TRANSIT_FRAGMENT_FADE;
            break;
    }
    *reverse = rev;
    return NOERROR;

}

ECode CFragmentManagerImpl::TransitToStyleIndex(
    /* [in] */ Int32 transit,
    /* [in] */ Boolean enter,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    Int32 animAttr = -1;
    switch (transit) {
        case IFragmentTransaction::TRANSIT_FRAGMENT_OPEN:
            animAttr = enter
                ? R::styleable::FragmentAnimation_fragmentOpenEnterAnimation
                : R::styleable::FragmentAnimation_fragmentOpenExitAnimation;
            break;
        case IFragmentTransaction::TRANSIT_FRAGMENT_CLOSE:
            animAttr = enter
                ? R::styleable::FragmentAnimation_fragmentCloseEnterAnimation
                : R::styleable::FragmentAnimation_fragmentCloseExitAnimation;
            break;
        case IFragmentTransaction::TRANSIT_FRAGMENT_FADE:
            animAttr = enter
                ? R::styleable::FragmentAnimation_fragmentFadeEnterAnimation
                : R::styleable::FragmentAnimation_fragmentFadeExitAnimation;
            break;
    }
    *index = animAttr;
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
