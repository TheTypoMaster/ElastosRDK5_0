
#include "view/animation/CLinearInterpolator.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CAR_OBJECT_IMPL(CLinearInterpolator);
CAR_INTERFACE_IMPL_5(CLinearInterpolator, Object, ILinearInterpolator,INativeInterpolatorFactory,IHasNativeInterpolator,IInterpolator,ITimeInterpolator);

ECode CLinearInterpolator::constructor()
{
    return NOERROR;
}

ECode CLinearInterpolator::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return NOERROR;
}

ECode CLinearInterpolator::GetInterpolation(
    /* [in] */ Float input,
    /* [out] */ Float* output)
{
    VALIDATE_NOT_NULL(output);
    *output = input;

    return NOERROR;
}

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos
