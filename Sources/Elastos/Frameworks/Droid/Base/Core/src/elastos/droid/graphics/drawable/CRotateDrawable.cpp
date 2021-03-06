
#include "graphics/drawable/CRotateDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CAR_OBJECT_IMPL(CRotateDrawable);
ECode CRotateDrawable::constructor()
{
    return RotateDrawable::constructor();
}

ECode CRotateDrawable::constructor(
    /* [in] */ Handle32 state,
    /* [in] */ IResources* res)
{
    return RotateDrawable::constructor((RotateState*)state, res);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
