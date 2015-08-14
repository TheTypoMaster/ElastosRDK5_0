
#include "StateListAnimator.h"

namespace Elastos {
namespace Droid {
namespace Animation {

StateListAnimator::Tuple::Tuple(
    /* [in] */ ArrayOf<Int32>* specs,
    /* [in] */ IAnimator* animator)
    : mSpecs(specs)
    , mAnimator(animator)
{
}

AutoPtr<ArrayOf<Int32> >  StateListAnimator::Tuple::GetSpecs()
{
    return mSpecs;
}

AutoPtr<IAnimator> StateListAnimator::Tuple::GetAnimator()
{
    return mAnimator;
}

StateListAnimator::AnimatorListener::AnimatorListener(
    /* [in] */ StateListAnimator* host)
    : mHost(host)
{}

StateListAnimator::AnimatorListener::OnAnimationEnd(
    /* [in] */ IAnimator* animation)
{
    animation->SetTarget(NULL);
    if (mHost->mRunningAnimator == animation) {
        mHost->mRunningAnimator = NULL;
    }
    return NOERROR;
}


CAR_INTERFACE_IMPL(StateListAnimator, Object, IStateListAnimator);
ECode StateListAnimator::AddState(
    /* [in] */ ArrayOf<Int32>* specs,
    /* [in] */ IAnimator* animator)
{
    AutoPtr<Tuple> tuple = new Tuple(specs, animator);
    tuple->mAnimator->AddListener(mAnimatorListener);
    mTuples.PushBack(tuple);
    return NOERROR;
}

ECode StateListAnimator::GetRunningAnimator(
    /* [out] */ IAnimator** animator)
{
    VALIDATE_NOT_NULL(animator);
    *animator = mRunningAnimator;
    REFCOUNT_ADD(*animator);
    return NOERROR;
}

ECode StateListAnimator::GetTarget(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = NULL;
    if (mViewRef != NULL) {
        mViewRef->Resolve(EIID_IView, view);
    }
    return NOERROR;
}

ECode StateListAnimator::SetTarget(
    /* [in] */ IView* view)
{
    AutoPtr<IView> current;
    GetTarget((IView**)&view);
    if (current == view) {
        return NOERROR;
    }
    if (current != NULL) {
        ClearTarget();
    }
    if (view != NULL) {
        mViewRef = IWeakReference::Probe(view);
    }

    return NOERROR;
}

void StateListAnimator::ClearTarget()
{
    List<AutoPtr<Tuple> >::Iterator ator = mTuples.Begin();
    for (; ator != mTuples.End(); ++ator) {
        (*ator)->mAnimator->SetTarget(NULL);
    }

    mViewRef = NULL;
    mLastMatch = NULL;
    mRunningAnimator = NULL;
}

ECode StateListAnimator::SetState(
    /* [in] */ ArrayOf<Int32>* state)
{
    AutoPtr<Tuple> match;
    List<AutoPtr<Tuple> >::Iterator ator = mTuples.Begin();
    for (; ator != mTuples.End(); ++ator) {
        AutoPtr<Tuple> tuple = *ator;
        if (StateSet::StateSetMatches(tuple->mSpecs, state)) {
            match = tuple;
            break;
        }
    }

    if (match == mLastMatch) {
        return NOERROR;
    }
    if (mLastMatch != NULL) {
        Cancel();
    }
    mLastMatch = match;
    if (match != null) {
        Start(match);
    }
    return NOERROR;
}

void StateListAnimator::Start(
    /* [in] */ Tuple* match)
{
    AutoPtr<IView> view;
    GetTarget((IView**)&view)
    match->mAnimator->SetTarget(view);
    mRunningAnimator = match.mAnimator;
    mRunningAnimator->Start();
}

void StateListAnimator::Cancel()
{
    if (mRunningAnimator != NULL) {
        mRunningAnimator->Cancel();
        mRunningAnimator = NULL;
    }
}

ECode StateListAnimator::GetTuples(
    /* [out] */ IArrayList** list)
{
    assert(list != NULL && *list != NULL);
    List<AutoPtr<Tuple> >::Iterator ator = mTuples.Begin();
    for (; ator != mTuples.End(); ++ator) {
        (*list)->Add((*ator)->Probe(EIID_IInterface));
    }
    return NOERROR;
}

ECode StateListAnimator::JumpToCurrentState()
{
    if (mRunningAnimator != NULL) {
        mRunningAnimator->End();
    }
    return NOERROR;
}

} // namespace Animation
} // namepsace Droid
} // namespace Elastos

