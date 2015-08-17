
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_CLINEARINTERPOLATOR_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_CLINEARINTERPOLATOR_H__

#include "_Elastos_Droid_View_Animation_CLinearInterpolator.h"

using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CLinearInterpolator)
    : public Object
    , public ILinearInterpolator
    , public INativeInterpolatorFactory
    , public IHasNativeInterpolator
    , public IInterpolator
    , public ITimeInterpolator
{
public:
    CAR_INTERFACE_DECL();

    CAR_OBJECT_DECL();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI GetInterpolation(
        /* [in] */ Float input,
        /* [out] */ Float* output);
};

}   //namespace Animation
}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif //__ELASTOS_DROID_VIEW_ANIMATION_CLINEARINTERPOLATOR_H__
