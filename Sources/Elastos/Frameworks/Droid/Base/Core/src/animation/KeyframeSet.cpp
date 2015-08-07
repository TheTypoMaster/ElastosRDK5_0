
#include "animation/KeyframeSet.h"
#include "animation/Int32KeyframeSet.h"
#include "animation/FloatKeyframeSet.h"
#include <elastos/core/Math.h>

namespace Elastos {
namespace Droid {
namespace Animation {

CAR_INTERFACE_IMPL(KeyframeSet, Object, IKeyframeSet)

KeyframeSet::KeyframeSet(
    /* [in] */ ArrayOf<IKeyframe*>* keyframes)
{
    assert(keyframes != NULL);
    mNumKeyframes = keyframes->GetLength();
    mKeyframes = keyframes->Clone();

    mFirstKeyframe = (*mKeyframes)[0];
    mLastKeyframe = (*mKeyframes)[mNumKeyframes - 1];
    mLastKeyframe->GetInterpolator((ITimeInterpolator**)&mInterpolator);
}

KeyframeSet::~KeyframeSet()
{}

AutoPtr<IKeyframeSet> KeyframeSet::OfInt32(
    /* [in] */ ArrayOf<Int32>* values)
{
    assert(values != NULL);
    Int32 numKeyframes = values->GetLength();
    AutoPtr<ArrayOf<IInt32Keyframe*> > keyframes =
            ArrayOf<IInt32Keyframe*>::Alloc(Elastos::Core::Math::Max(numKeyframes, 2));

    if (numKeyframes == 1) {
        AutoPtr<IInt32Keyframe> temp;
        Keyframe::OfInt32(0.0f, (IKeyframe**)&temp);
        keyframes->Set(0, temp); temp = NULL;
        Keyframe::OfInt32(1.0f, (*values)[0], (IKeyframe**)&temp);
        keyframes->Set(1, temp);
    } else {
        AutoPtr<IInt32Keyframe> temp;
        Keyframe::OfInt32(0.0f, (*values)[0], (IKeyframe**)&temp);
        keyframes->Set(0, temp);
        for (Int32 i = 1; i < numKeyframes; ++i) {
            temp = NULL;
            Keyframe::OfInt32((Float) i / (numKeyframes - 1), (*values)[i], (IKeyframe**)&temp);
            keyframes->Set(i, temp);
        }
    }

    return new Int32KeyframeSet(keyframes);
}

AutoPtr<IKeyframeSet> KeyframeSet::OfFloat(
    /* [in] */ ArrayOf<Float>* values)
{
    assert(values != NULL);

    Int32 numKeyframes = values->GetLength();
    AutoPtr<ArrayOf<IFloatKeyframe*> > keyframes =
            ArrayOf<IFloatKeyframe*>::Alloc(Elastos::Core::Math::Max(numKeyframes, 2));

    if (numKeyframes == 1) {
        AutoPtr<IFloatKeyframe> temp;
        Keyframe::OfFloat(0.0f, (IKeyframe**)&temp);
        keyframes->Set(0, temp); temp = NULL;
        Keyframe::OfFloat(1.0f, (*values)[0], (IKeyframe**)&temp);
        keyframes->Set(1, temp);
    } else {
        AutoPtr<IFloatKeyframe> temp;
        Keyframe::OfFloat(0.0f, (*values)[0], (IKeyframe**)&temp);
        keyframes->Set(0, temp);
        for (Int32 i = 1; i < numKeyframes; ++i) {
            temp = NULL;
            Keyframe::OfFloat((Float) i / (numKeyframes - 1), (*values)[i], (IKeyframe**)&temp);
            keyframes->Set(i, temp);
        }
    }

    return new FloatKeyframeSet(keyframes);
}

AutoPtr<IKeyframeSet> KeyframeSet::OfKeyframe(
    /* [in] */ ArrayOf<IKeyframe*>* keyframes)
{
    assert(keyframes != NULL);
    // if all keyframes of same primitive type, create the appropriate KeyframeSet
    Int32 numKeyframes = keyframes->GetLength();
    Boolean hasFloat = FALSE;
    Boolean hasInt = FALSE;
    Boolean hasOther = FALSE;
    for (Int32 i = 0; i < numKeyframes; ++i) {
        if (IFloatKeyframe::Probe((*keyframes)[i])) {
            hasFloat = TRUE;
        } else if (IInt32Keyframe::Probe((*keyframes)[i])) {
            hasInt = TRUE;
        } else {
            hasOther = TRUE;
        }
    }
    if (hasFloat && !hasInt && !hasOther) {
        AutoPtr<ArrayOf<IFloatKeyframe*> > floatKeyframes =
                ArrayOf<IFloatKeyframe*>::Alloc(numKeyframes);

        for (Int32 i = 0; i < numKeyframes; ++i) {
            AutoPtr<IFloatKeyframe> temp = IFloatKeyframe::Probe((*keyframes)[i]);
            floatKeyframes->Set(i, temp);
        }

        return new FloatKeyframeSet(floatKeyframes);
    } else if (hasInt && !hasFloat && !hasOther) {
        AutoPtr<ArrayOf<IInt32Keyframe*> > intKeyframes =
                ArrayOf<IInt32Keyframe*>::Alloc(numKeyframes);

        for (Int32 i = 0; i < numKeyframes; ++i) {
            AutoPtr<IInt32Keyframe> temp = IInt32Keyframe::Probe((*keyframes)[i]);
            intKeyframes->Set(i, temp);
        }

        return new Int32KeyframeSet(intKeyframes);
    } else {
        return new KeyframeSet(keyframes);
    }
}

AutoPtr<IKeyframeSet> KeyframeSet::OfObject(
    /* [in] */ ArrayOf<IInterface*>* values)
{
    assert(values != NULL);

    Int32 numKeyframes = values->GetLength();
    AutoPtr<ArrayOf<IObjectKeyframe*> > keyframes =
            ArrayOf<IObjectKeyframe*>::Alloc(Elastos::Core::Math::Max(numKeyframes, 2));

    if (numKeyframes == 1) {
        AutoPtr<IObjectKeyframe> temp;
        Keyframe::OfObject(0.0f, (IKeyframe**)&temp);
        keyframes->Set(0, temp); temp = NULL;
        Keyframe::OfObject(1.0f, (*values)[0], (IKeyframe**)&temp);
        keyframes->Set(1, temp);
    } else {
        AutoPtr<IObjectKeyframe> temp;
        Keyframe::OfObject(0.0f, (*values)[0], (IKeyframe**)&temp);
        keyframes->Set(0, temp);
        for (Int32 i = 1; i < numKeyframes; ++i) {
            temp = NULL;
            Keyframe::OfObject((Float) i / (numKeyframes - 1), (*values)[i], (IKeyframe**)&temp);
            keyframes->Set(i, temp);
        }
    }

    return new KeyframeSet((ArrayOf<IKeyframe*>*)(keyframes.Get()));
}

ECode KeyframeSet::SetEvaluator(
    /* [in] */ ITypeEvaluator* evaluator)
{
    mEvaluator = evaluator;
    return NOERROR;
}

ECode KeyframeSet::GetValue(
    /* [in] */ Float fraction,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value);

    // Special-case optimization for the common case of only two keyframes
    Float fractionTemp = fraction;
    if (mNumKeyframes == 2) {
        if (mInterpolator != NULL) {
            mInterpolator->GetInterpolation(fractionTemp, &fractionTemp);
        }
        AutoPtr<IInterface> fvTemp;
        mFirstKeyframe->GetValue((IInterface**)&fvTemp);
        AutoPtr<IInterface> lvTemp;
        mLastKeyframe->GetValue((IInterface**)&lvTemp);
        return mEvaluator->Evaluate(fractionTemp, fvTemp, lvTemp, value);
    }

    if (fractionTemp <= 0.0f) {
        AutoPtr<IKeyframe> nextKeyframe = (*mKeyframes)[1];
        AutoPtr<ITimeInterpolator> interpolator;
        nextKeyframe->GetInterpolator((ITimeInterpolator**)&interpolator);
        if (interpolator != NULL) {
            interpolator->GetInterpolation(fractionTemp, &fractionTemp);
        }

        Float prevFraction;
        mFirstKeyframe->GetFraction(&prevFraction);
        Float nextFraction;
        nextKeyframe->GetFraction(&nextFraction);
        Float intervalFraction = (fractionTemp - prevFraction) /
            (nextFraction - prevFraction);

        AutoPtr<IInterface> vTemp;
        mFirstKeyframe->GetValue((IInterface**)&vTemp);
        AutoPtr<IInterface> nvTemp;
        nextKeyframe->GetValue((IInterface**)&nvTemp);
        return mEvaluator->Evaluate(intervalFraction, vTemp, nvTemp, value);
    }
    else if (fractionTemp >= 1.0f) {
        AutoPtr<IKeyframe> prevKeyframe = (*mKeyframes)[mNumKeyframes - 2];
        AutoPtr<ITimeInterpolator> interpolator;
        mLastKeyframe->GetInterpolator((ITimeInterpolator**)&interpolator);
        if (interpolator != NULL) {
            interpolator->GetInterpolation(fractionTemp, &fractionTemp);
        }

        Float prevFraction;
        prevKeyframe->GetFraction(&prevFraction);
        Float lastFraction;
        mLastKeyframe->GetFraction(&lastFraction);
        Float intervalFraction = (fractionTemp - prevFraction) /
            (lastFraction - prevFraction);
        AutoPtr<IInterface> vTemp;
        prevKeyframe->GetValue((IInterface**)&vTemp);
        AutoPtr<IInterface> lvTemp;
        mLastKeyframe->GetValue((IInterface**)&lvTemp);
        return mEvaluator->Evaluate(intervalFraction, vTemp, lvTemp, value);
    }

    AutoPtr<IKeyframe> prevKeyframe = mFirstKeyframe;
    for (Int32 i = 1; i < mNumKeyframes; ++i) {
        AutoPtr<IKeyframe> nextKeyframe = (*mKeyframes)[i];
        Float nextFraction;
        nextKeyframe->GetFraction(&nextFraction);
        if (fractionTemp < nextFraction) {
            AutoPtr<ITimeInterpolator> interpolator;
            nextKeyframe->GetInterpolator((ITimeInterpolator**)&interpolator);
            if (interpolator != NULL) {
                interpolator->GetInterpolation(fractionTemp, &fractionTemp);
            }

            Float prevFraction;
            prevKeyframe->GetFraction(&prevFraction);
            Float nFraction;
            prevKeyframe->GetFraction(&nFraction);
            Float intervalFraction = (fractionTemp - prevFraction) /
                (nFraction - prevFraction);
            AutoPtr<IInterface> vTemp;
            prevKeyframe->GetValue((IInterface**)&vTemp);
            AutoPtr<IInterface> nvTemp;
            nextKeyframe->GetValue((IInterface**)&nvTemp);
            return mEvaluator->Evaluate(intervalFraction, vTemp, nvTemp, value);
        }
        prevKeyframe = nextKeyframe;
    }

    // shouldn't reach here
    return mLastKeyframe->GetValue(value);
}

ECode KeyframeSet::Clone(
    /* [out] */ IKeyframeSet** object)
{
    Int32 numKeyframes = mKeyframes->GetLength();
    AutoPtr<ArrayOf<IKeyframe*> > newKeyframes = ArrayOf<IKeyframe*>::Alloc(numKeyframes);
    for (Int32 i = 0; i < numKeyframes; ++i) {
        AutoPtr<IKeyframe> temp;
        (*mKeyframes)[i]->Clone((IKeyframe**)&temp);
        newKeyframes->Set(i, temp);
    }
    AutoPtr<IKeyframeSet> newSet = new KeyframeSet(newKeyframes);
    *object = newSet;
    REFCOUNT_ADD(*object)
    return NOERROR;
}

ECode KeyframeSet::GetKeyframes(
    /* [out, callee] */ ArrayOf<IKeyframe*>** frames)
{
    VALIDATE_NOT_NULL(frames);
    *frames = mKeyframes;
    REFCOUNT_ADD(*frames);
    return NOERROR;
}

ECode KeyframeSet::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    String returnVal(" "), tmp;
    for (Int32 i = 0; i < mNumKeyframes; ++i) {
        AutoPtr<IInterface> obj;
        (*mKeyframes)[i]->GetValue((IInterface**)&obj);
        IObject::Probe(obj)->ToString(&tmp);
        returnVal += tmp + String("  ");
    }
    *str = returnVal;
    return NOERROR;
}

}   //namespace Animation
}   //namespace Droid
}   //namespace Elastos
