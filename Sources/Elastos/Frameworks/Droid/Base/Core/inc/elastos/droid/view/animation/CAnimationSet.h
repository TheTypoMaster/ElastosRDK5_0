
#ifndef __ELASTOS_DROID_VIEW_ANIMATION_CANIMATIONSET_H__
#define __ELASTOS_DROID_VIEW_ANIMATION_CANIMATIONSET_H__

#include "_Elastos_Droid_View_Animation_CAnimationSet.h"
#include "view/animation/AnimationSet.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CarClass(CAnimationSet)
    , public AnimationSet
{
public:
    CAR_INTERFACE_DECL();

    CAR_OBJECT_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Boolean shareInterpolator);

private:
};

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos

#endif // __ELASTOS_DROID_VIEW_ANIMATION_CANIMATIONSET_H__
