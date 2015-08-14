
#include "animation/Int32KeyframeSet.h"

using Elastos::Core::INumber;

namespace Elastos {
namespace Droid {
namespace Animation {

CAR_INTERFACE_IMPL_2(Int32KeyframeSet, KeyframeSet, IInt32KeyframeSet)

Int32KeyframeSet::Int32KeyframeSet(
    /* [in] */ ArrayOf<IInt32Keyframe*>* keyframes)
    : KeyframeSet((ArrayOf<IKeyframe*>*)keyframes)
    , mFirstValue(0)
    , mLastValue(0)
    , mDeltaValue(0)
    , mFirstTime(TRUE)
{}

ECode Int32KeyframeSet::GetValue(
    /* [in] */ Float fraction,
    /* [out] */ IInterface** value)
{
    Int32 vTemp;
    GetInt32Value(fraction, &vTemp);
    AutoPtr<IInterface> iv;
    CInteger32::New(vTemp, (IInteger32**)&iv);
    *value = iv;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode Int32KeyframeSet::GetInt32Value(
    /* [in] */ Float frac,
    /* [out] */ Int32* value)
{
    Float fraction = frac;
    if (mNumKeyframes == 2) {
        if (mFirstTime) {
            mFirstTime = FALSE;
            AutoPtr<IInt32Keyframe> frameTmp = (IInt32Keyframe*)((*mKeyframes)[0]->Probe(EIID_IInt32Keyframe));
            frameTmp->GetInt32Value(&mFirstValue);

            frameTmp = (IInt32Keyframe*)((*mKeyframes)[1]->Probe(EIID_IInt32Keyframe));
            frameTmp->GetInt32Value(&mLastValue);;
            mDeltaValue = mLastValue - mFirstValue;
        }

        if (mInterpolator != NULL) {
            mInterpolator->GetInterpolation(fraction, &fraction);
        }

        if (mEvaluator == NULL) {
            *value = mFirstValue + (Int32)(fraction * mDeltaValue);
            return NOERROR;
        } else {
            AutoPtr<IInteger32> obj, fValue, lValue;
            CInteger32::New(mFirstValue, (IInteger32**)&fValue);
            CInteger32::New(mLastValue, (IInteger32**)&lValue);
            mEvaluator->Evaluate(fraction, fValue, lValue, (IInterface**)&obj);
            assert(obj != NULL);
            obj->GetValue(value);
            return NOERROR;
        }
    }

    if (fraction <= 0.0f) {
        AutoPtr<IInt32Keyframe> prevKeyframe = (IInt32Keyframe*)((*mKeyframes)[0]->Probe(EIID_IInt32Keyframe));
        AutoPtr<IInt32Keyframe> nextKeyframe = (IInt32Keyframe*)((*mKeyframes)[1]->Probe(EIID_IInt32Keyframe));
        Int32 prevValue;
        prevKeyframe->GetInt32Value(&prevValue);
        Int32 nextValue;
        nextKeyframe->GetInt32Value(&nextValue);
        Float prevFraction;
        prevKeyframe->GetFraction(&prevFraction);
        Float nextFraction;
        nextKeyframe->GetFraction(&nextFraction);
        AutoPtr<ITimeInterpolator> interpolator;
        nextKeyframe->GetInterpolator((ITimeInterpolator**)&interpolator);
        if (interpolator != NULL) {
            interpolator->GetInterpolation(fraction, &fraction);
        }

        Float intervalFraction = (fraction - prevFraction) / (nextFraction - prevFraction);
        if (mEvaluator == NULL) {
            *value = prevValue + (Int32)(intervalFraction * (nextValue - prevValue)) ;
            return NOERROR;
        }

        AutoPtr<IInteger32> obj, pValue, nValue;
        CInteger32::New(prevValue, (IInteger32**)&pValue);
        CInteger32::New(nextValue, (IInteger32**)&nValue);
        mEvaluator->Evaluate(intervalFraction, pValue, nValue, (IInterface**)&obj);

        assert(obj != NULL);
        obj->GetValue(value);
        return NOERROR;
    } else if (fraction >= 1.0f) {
        AutoPtr<IInt32Keyframe> prevKeyframe =
                (IInt32Keyframe*)((*mKeyframes)[mNumKeyframes - 2]->Probe(EIID_IInt32Keyframe));
        AutoPtr<IInt32Keyframe> nextKeyframe =
                (IInt32Keyframe*)((*mKeyframes)[mNumKeyframes - 1]->Probe(EIID_IInt32Keyframe));
        Int32 prevValue;
        prevKeyframe->GetInt32Value(&prevValue);
        Int32 nextValue;
        nextKeyframe->GetInt32Value(&nextValue);
        Float prevFraction;
        prevKeyframe->GetFraction(&prevFraction);
        Float nextFraction;
        nextKeyframe->GetFraction(&nextFraction);
        AutoPtr<ITimeInterpolator> interpolator;
        nextKeyframe->GetInterpolator((ITimeInterpolator**)&interpolator);
        if (interpolator != NULL) {
            interpolator->GetInterpolation(fraction, &fraction);
        }

        Float intervalFraction = (fraction - prevFraction) / (nextFraction - prevFraction);
        if (mEvaluator == NULL) {
            *value = prevValue + (Int32)(intervalFraction * (nextValue - prevValue));
            return NOERROR;
        }

        AutoPtr<IInteger32> obj, pValue, nValue;
        CInteger32::New(prevValue, (IInteger32**)&pValue);
        CInteger32::New(nextValue, (IInteger32**)&nValue);
        mEvaluator->Evaluate(intervalFraction, pValue, nValue, (IInterface**)&obj);

        assert(obj != NULL);
        obj->GetValue(value);
        return NOERROR;
    }

    AutoPtr<IInt32Keyframe> prevKeyframe = (IInt32Keyframe*)((*mKeyframes)[0]->Probe(EIID_IInt32Keyframe));
    for (Int32 i = 1; i < mNumKeyframes; ++i) {
        AutoPtr<IInt32Keyframe> nextKeyframe = (IInt32Keyframe*)((*mKeyframes)[i]->Probe(EIID_IInt32Keyframe));
        Float nFraction;
        nextKeyframe->GetFraction(&nFraction);
        if (fraction < nFraction) {
            AutoPtr<ITimeInterpolator> interpolator;
            nextKeyframe->GetInterpolator((ITimeInterpolator**)&interpolator);
            if (interpolator != NULL) {
                interpolator->GetInterpolation(fraction, &fraction);
            }

            Float pFraction, nFraction;
            prevKeyframe->GetFraction(&pFraction);
            nextKeyframe->GetFraction(&nFraction);
            Float intervalFraction = (fraction - pFraction) / (nFraction - pFraction);
            Int32 prevValue;
            prevKeyframe->GetInt32Value(&prevValue);
            Int32 nextValue;
            nextKeyframe->GetInt32Value(&nextValue);

            if (mEvaluator == NULL) {
                *value = prevValue + (Int32)(intervalFraction * (nextValue - prevValue));
                return NOERROR;
            }

            AutoPtr<IInteger32> obj, pValue, nValue;
            CInteger32::New(prevValue, (IInteger32**)&pValue);
            CInteger32::New(nextValue, (IInteger32**)&nValue);
            mEvaluator->Evaluate(intervalFraction, pValue, nValue, (IInterface**)&obj);

            assert(obj != NULL);
            obj->GetValue(value);
            return NOERROR;
        }

        prevKeyframe = nextKeyframe;
    }

    // shouldn't get here
    AutoPtr<IInt32Keyframe> obj = (IInt32Keyframe*)((*mKeyframes)[mNumKeyframes - 1]->Probe(EIID_IInt32Keyframe));
    AutoPtr<INumber> rst;
    obj->GetValue((IInterface**)&rst);
    rst->Int32Value(value);
    return NOERROR;
}

ECode Int32KeyframeSet::InvalidateCache()
{
    mFirstTime = TRUE;
    return NOERROR;
}

ECode Int32KeyframeSet::Clone(
    /* [out] */ IKeyframeSet** obj)
{
    Int32 numKeyframes = mKeyframes->GetLength();
    AutoPtr<ArrayOf<IInt32Keyframe*> > newKeyframes = ArrayOf<IInt32Keyframe*>::Alloc(numKeyframes);
    for (Int32 i = 0; i < numKeyframes; ++i) {
        AutoPtr<IInt32Keyframe> newone;
        (*mKeyframes)[i]->Clone((IKeyframe**)&newone);
        newKeyframes->Set(i, newone);
    }
    AutoPtr<IInt32KeyframeSet> newSet = new Int32KeyframeSet(newKeyframes);
    *obj = newSet;
    REFCOUNT_ADD(*obj);
    return NOERROR;
}

ECode Int32KeyframeSet::GetKeyframes(
    /* [out, callee] */ ArrayOf<IKeyframe*>** frames)
{
    VALIDATE_NOT_NULL(frames);
    *frames = mKeyframes;
    REFCOUNT_ADD(*frames);
    return NOERROR;
}

ECode Int32KeyframeSet::GetType(
    /* [out] */ ClassID* type)
{
    VALIDATE_NOT_NULL(type);
    *type = ECLSID_CInteger32;
    return NOERROR;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
