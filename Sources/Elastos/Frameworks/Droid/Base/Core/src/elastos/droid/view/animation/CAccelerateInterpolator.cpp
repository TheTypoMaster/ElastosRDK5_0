
#include "view/animation/CAccelerateInterpolator.h"
#include "ext/frameworkext.h"
#include <elastos/core/Math.h>

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CAR_OBJECT_IMPL(CAccelerateInterpolator);
CAR_INTERFACE_IMPL_4(CAccelerateInterpolator, Object, IAccelerateInterpolator,INativeInterpolatorFactory,IInterpolator,ITimeInterpolator);

ECode CAccelerateInterpolator::constructor()
{
    mFactor = 1.0f;
    mDoubleFactor = 2.0;

    return NOERROR;
}

ECode CAccelerateInterpolator::constructor(
    /* [in] */ Float factor)
{
    mFactor = factor;
    mDoubleFactor = 2 * mFactor;

    return NOERROR;
}

ECode CAccelerateInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IResourcesTheme> theme;
    context->GetTheme((IResourcesTheme**)&theme);
    return constructor(res, theme, attrs);
}

ECode CAccelerateInterpolator::constructor(
    /* [in] */ IResources* res,
    /* [in] */ IResourcesTheme* theme,
    /* [in] */ IAttributeSet* attrs)
{
    AutoPtr<ITypedArray> a;
    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::AccelerateInterpolator),
            ARRAY_SIZE(R::styleable::AccelerateInterpolator));

    if (theme != NULL) {
        theme->ObtainStyledAttributes(attrs, attrIds, 0, 0, (ITypedArray**)&a);
    } else {
        res->ObtainAttributes(attrs, attrIds, (ITypedArray**)&a);
    }

    a->GetFloat(R::styleable::AccelerateInterpolator_factor, 1.0f, &mFactor);
    mDoubleFactor = 2 * mFactor;

    return a->Recycle();
}

ECode CAccelerateInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);

    if (mFactor == 1.0f) {
        *output = input * input;
    }
    else {
        *output =  (Float)Elastos::Core::Math::Pow(input, mDoubleFactor);
    }

    return NOERROR;
}

ECode CAccelerateInterpolator::CreateNativeInterpolator(
    /* [out] */ Int64* interpolator)
{
    VALIDATE_NOT_NULL(interpolator);
    *interpolator = NativeInterpolatorFactoryHelper::CreateAccelerateInterpolator(mFactor);
    return NOERROR;
}

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos
