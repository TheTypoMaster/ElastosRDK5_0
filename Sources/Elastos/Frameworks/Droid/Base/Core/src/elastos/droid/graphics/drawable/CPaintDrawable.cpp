
#include "ext/frameworkext.h"
#include "graphics/drawable/CPaintDrawable.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CAR_OBJECT_IMPL(CPaintDrawable);
ECode CPaintDrawable::constructor()
{
    return PaintDrawable::constructor();
}

ECode CPaintDrawable::constructor(
    /* [in] */ Int32 color)
{
    return PaintDrawable::constructor(color);
}

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
