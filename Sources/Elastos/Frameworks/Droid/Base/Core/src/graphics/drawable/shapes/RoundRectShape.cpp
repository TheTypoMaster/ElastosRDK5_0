
#include "ext/frameworkext.h"
#include "graphics/drawable/shapes/RoundRectShape.h"
// #include "graphics/CPath.h"

namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {
namespace Shapes {

CAR_INTERFACE_IMPL(RoundRectShape, RectShape, IRoundRectShape);
RoundRectShape::RoundRectShape()
{}

RoundRectShape::RoundRectShape(
    /* [in] */ ArrayOf<Float>* outerRadii,
    /* [in] */ IRectF* inset,
    /* [in] */ ArrayOf<Float>* innerRadii)
{
    if (outerRadii != NULL && outerRadii->GetLength() < 8) {
        // throw new ArrayIndexOutOfBoundsException(
        //                             "outer radii must have >= 8 values");
        assert(0);
    }
    if (innerRadii != NULL && innerRadii->GetLength() < 8) {
        // throw new ArrayIndexOutOfBoundsException(
        //                             "inner radii must have >= 8 values");
        assert(0);
    }
    mOuterRadii = outerRadii;
    mInnerRadii = innerRadii;

    assert(0 && "TODO");
    // if (inset != NULL) {
    //     CRectF::New((IRectF**)&mInnerRect);
    // }
    // CPath::New((IPath**)&mPath);
}

ECode RoundRectShape::constructor(
    /* [in] */ ArrayOf<Float>* outerRadii,
    /* [in] */ IRectF* inset,
    /* [in] */ ArrayOf<Float>* innerRadii)
{
    if (outerRadii != NULL && outerRadii->GetLength() < 8) {
        // throw new ArrayIndexOutOfBoundsException(
        //                             "outer radii must have >= 8 values");
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    if (innerRadii != NULL && innerRadii->GetLength() < 8) {
        // throw new ArrayIndexOutOfBoundsException(
        //                             "inner radii must have >= 8 values");
        return E_ARRAY_INDEX_OUT_OF_BOUNDS_EXCEPTION;
    }
    mOuterRadii = outerRadii;
    mInnerRadii = innerRadii;

    assert(0 && "TODO");
    // if (inset != NULL) {
    //     CRectF::NewByFriend((CRectF**)&mInnerRect);
    // }
    // CPath::New((IPath**)&mPath);
    return NOERROR;
}

ECode RoundRectShape::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IPaint* paint)
{
    return canvas->DrawPath(mPath, paint);
}

void RoundRectShape::OnResize(
    /* [in] */ Float w,
    /* [in] */ Float h)
{
    RectShape::OnResize(w, h);

    AutoPtr<IRectF> rf = Rect();
    mPath->Reset();

    if (mOuterRadii != NULL) {
        mPath->AddRoundRect(rf, mOuterRadii, PathDirection_CW);
    }
    else {
        mPath->AddRect(rf, PathDirection_CW);
    }
    if (mInnerRect != NULL) {
        Float l = 0.f, r = 0.f, t = 0.f, b = 0.f;
        Float l2 = 0.f, r2 = 0.f, t2 = 0.f, b2 = 0.f;
        rf->Get(&l, &t, &r, &b);
        mInset->Get(&l2, &t2, &r2, &b2);
        mInnerRect->Set(l + l2, t + t2, r - r2, b - b2);
        Float ww, hh;
        mInnerRect->GetWidth(&ww);
        mInnerRect->GetHeight(&hh);
        if (ww < w && hh < h) {
            if (mInnerRadii != NULL) {
                mPath->AddRoundRect(mInnerRect, mInnerRadii, PathDirection_CCW);
            }
            else {
                mPath->AddRect(mInnerRect, PathDirection_CCW);
            }
        }
    }
}

ECode RoundRectShape::CloneImpl(
    /* [in] */ IRoundRectShape* _other)
{
    RoundRectShape* other = (RoundRectShape*)_other;
    RectShape::CloneImpl(IRectShape::Probe(other));
    other->mOuterRadii = mOuterRadii != NULL ? mOuterRadii->Clone() : NULL;
    other->mInnerRadii = mInnerRadii != NULL ? mInnerRadii->Clone() : NULL;
    assert(0 && "TODO");
    // CRectF::New((IRectF*)mInset.Get(), (IRectF**)&other->mInset);
    // CRectF::New((IRectF*)mInnerRect.Get(), (IRectF**)&other->mInnerRect);
    // return CPath::New(mPath, (IPath**)&other->mPath);
    return NOERROR;
}

} // namespace Shapes
} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos
