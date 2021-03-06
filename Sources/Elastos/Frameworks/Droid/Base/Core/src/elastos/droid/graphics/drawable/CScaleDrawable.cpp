
#include "ext/frameworkext.h"
#include "graphics/drawable/CScaleDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CAR_OBJECT_IMPL(CScaleDrawable);
ECode CScaleDrawable::constructor()
{
    return ScaleDrawable::constructor();
}

ECode CScaleDrawable::constructor(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 gravity,
    /* [in] */ Float scaleWidth,
    /* [in] */ Float scaleHeight)
{
    return ScaleDrawable::constructor(
            drawable, gravity, scaleWidth, scaleHeight);
}

ECode CScaleDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ IResources* res)
{
    return ScaleDrawable::constructor((ScaleState*)state, res);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
