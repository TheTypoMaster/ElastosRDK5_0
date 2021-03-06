
#include "ext/frameworkext.h"
#include "graphics/drawable/shapes/CArcShape.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {

CAR_OBJECT_IMPL(CArcShape);
ECode CArcShape::constructor(
    /* [in] */ Float startAngle,
    /* [in] */ Float sweepAngle)
{
    return ArcShape::constructor(startAngle, sweepAngle);
}

ECode CArcShape::Clone(
    /* [out] */ IInterface** shape)
{
    VALIDATE_NOT_NULL(shape);
    AutoPtr<IArcShape> s;
    CArcShape::New(0, 0, (IArcShape**)&s);
    ArcShape::CloneImpl(s);
    *shape = (IShape*)s.Get();
    REFCOUNT_ADD(*shape);
    return NOERROR;
}

} // namespace Shapes
} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
