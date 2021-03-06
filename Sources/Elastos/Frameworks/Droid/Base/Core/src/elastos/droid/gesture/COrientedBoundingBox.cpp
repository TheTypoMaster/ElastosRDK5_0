
#include "ext/frameworkext.h"
#include "gesture/COrientedBoundingBox.h"
#include "graphics/CPath.h"
#include "graphics/CMatrix.h"

using Elastos::Droid::Graphics::CPath;
using Elastos::Droid::Graphics::IMatrix;
using Elastos::Droid::Graphics::CMatrix;

namespace Elastos {
namespace Droid {
namespace Gesture {

COrientedBoundingBox::constructor(
    /* [in] */ Float angle,
    /* [in] */ Float cx,
    /* [in] */ Float cy,
    /* [in] */ Float w,
    /* [in] */ Float h)
{
    mOrientation = angle;
    mWidth = w;
    mHeight = h;
    mCenterX = cx;
    mCenterY = cy;
    Float ratio = w / h;
    if (ratio > 1) {
        mSquareness = 1 / ratio;
    } else {
        mSquareness = ratio;
    }
}

ECode COrientedBoundingBox::ToPath(
    /* [out] */ IPath **outPath)
{
    VALIDATE_NOT_NULL(outPath);

    AutoPtr<IPath> path;
    CPath::New((IPath**)&path);
    AutoPtr<ArrayOf<Float> > point = ArrayOf<Float>::Alloc(2);
    (*point)[0] = -mWidth / 2;
    (*point)[1] = mHeight / 2;
    AutoPtr<IMatrix> matrix;
    CMatrix::New((IMatrix**)&matrix);
    matrix->SetRotate(mOrientation);
    Boolean isPostTranslated;
    matrix->PostTranslate(mCenterX, mCenterY, &isPostTranslated);
    matrix->MapPoints(point);
    path->MoveTo((*point)[0], (*point)[1]);

    (*point)[0] = -mWidth / 2;
    (*point)[1] = -mHeight / 2;
    matrix->MapPoints(point);
    path->LineTo((*point)[0], (*point)[1]);

    (*point)[0] = mWidth / 2;
    (*point)[1] = -mHeight / 2;
    matrix->MapPoints(point);
    path->LineTo((*point)[0], (*point)[1]);

    (*point)[0] = mWidth / 2;
    (*point)[1] = mHeight / 2;
    matrix->MapPoints(point);
    path->LineTo((*point)[0], (*point)[1]);

    path->Close();

    *outPath = path;
    REFCOUNT_ADD(*outPath);
    return NOERROR;
}

ECode COrientedBoundingBox::GetSquareness(
    /* [out] */ Float* squareness)
{
    VALIDATE_NOT_NULL(squareness);

    *squareness = mSquareness;
    return NOERROR;
}

ECode COrientedBoundingBox::GetWidth(
    /* [out] */ Float *width)
{
    VALIDATE_NOT_NULL(width);
    *width = mWidth;
    return NOERROR;
}

ECode COrientedBoundingBox::GetHeight(
    /* [out] */ Float *height)
{
    VALIDATE_NOT_NULL(height);
    *height = mHeight;
    return NOERROR;
}

ECode COrientedBoundingBox::GetOrientation(
    /* [out] */ Float *orientation)
{
    VALIDATE_NOT_NULL(orientation);
    *orientation = mOrientation;
    return NOERROR;
}

ECode COrientedBoundingBox::GetCenterX(
    /* [out] */ Float *centerX)
{
    VALIDATE_NOT_NULL(centerX);
    *centerX = mCenterX;
    return NOERROR;
}

ECode COrientedBoundingBox::GetCenterY(
    /* [out] */ Float *centerY)
{
    VALIDATE_NOT_NULL(centerY);
    *centerY = mCenterY;
    return NOERROR;
}

}//namespace Gesture
}//namespace Droid
}//namespace Elastos
