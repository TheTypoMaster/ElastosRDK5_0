
#include "view/animation/CTransformation.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Animation {

CTransformation::CTransformation()
    : mHasClipRect(FALSE)
{
    CRect::New((IRect**)&mClipRect);
}

ECode CTransformation::constructor()
{
    return Clear();
}

ECode CTransformation::Clear()
{
    if (mMatrix == NULL) {
        CMatrix::New((IMatrix**)&mMatrix);
    }
    else {
        mMatrix->Reset();
    }
    mClipRect->SetEmpty();
    mHasClipRect = FALSE;
    mAlpha = 1.0f;
    mTransformationType = ITransformation::TYPE_BOTH;

    return NOERROR;
}

ECode CTransformation::GetTransformationType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type)
    *type = mTransformationType;

    return NOERROR;
}

ECode CTransformation::SetTransformationType(
    /* [in] */ Int32 transformationType)
{
    mTransformationType = transformationType;

    return NOERROR;
}

ECode CTransformation::Set(
    /* [in] */ ITransformation* t)
{
    t->GetAlpha(&mAlpha);
    AutoPtr<IMatrix> matrix;
    t->GetMatrix((IMatrix**)&matrix);
    mMatrix->Set(matrix);
    if (((CTransformation*)t)->mHasClipRect) {
        AutoPtr<IRect> cr;
        t->GetClipRect((IRect**)&cr);
        SetClipRect(cr);
    } else {
        mHasClipRect = FALSE;
        mClipRect->SetEmpty();
    }
    t->GetTransformationType(&mTransformationType);

    return NOERROR;
}

ECode CTransformation::Compose(
    /* [in] */ ITransformation* t)
{
    Float alpha;
    t->GetAlpha(&alpha);
    mAlpha *= alpha;

    AutoPtr<IMatrix> matrix;
    t->GetMatrix((IMatrix**)&matrix);
    Boolean res;
    mMatrix->PreConcat(matrix, &res);
    if (((CTransformation*)t)->mHasClipRect) {
        AutoPtr<IRect> cr;
        t->GetClipRect((IRect**)&cr);
        SetClipRect(cr);
    }
    return NOERROR;
}

ECode CTransformation::PostCompose(
    /* [in] */ ITransformation* t)
{
    assert(t != NULL);
    Float a = 0.f;
    t->GetAlpha(&a);

    mAlpha *= a;
    AutoPtr<IMatrix> mx;
    t->GetMatrix((IMatrix**)&mx);
    Boolean result;
    mMatrix->PostConcat(mx, &result);

    if (((CTransformation*)t)->mHasClipRect) {
        AutoPtr<IRect> cr;
        t->GetClipRect((IRect**)&cr);
        SetClipRect(cr);
    }
    return NOERROR;
}

ECode CTransformation::GetMatrix(
    /* [out] */ IMatrix** matrix)
{
    VALIDATE_NOT_NULL(matrix)
    *matrix = mMatrix;
    REFCOUNT_ADD(*matrix);


    return NOERROR;
}

ECode CTransformation::SetAlpha(
    /* [in] */ Float alpha)
{
    mAlpha = alpha;
    return NOERROR;
}

ECode CTransformation::SetClipRect(
    /* [in] */ IRect* r)
{
    Int32 left = 0, top = 0, right = 0, bottom = 0;
    r->Get(&left, &top, &right, &bottom);
    return SetClipRect(left, top, right, bottom);
}

ECode CTransformation::SetClipRect(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 b)
{
    mClipRect->Set(l, t, r, b);
    mHasClipRect = TRUE;
    return NOERROR;
}

ECode CTransformation::GetClipRect(
    /* [out] */ IRect** cr)
{
    VALIDATE_NOT_NULL(cr);
    *cr = mClipRect;
    REFCOUNT_ADD(*cr);
    return NOERROR;
}

ECode CTransformation::HasClipRect(
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has);
    *has = mHasClipRect;
    return NOERROR;
}

ECode CTransformation::GetAlpha(
    /* [out] */ Float* alpha)
{
    VALIDATE_NOT_NULL(alpha);
    *alpha = mAlpha;

    return NOERROR;
}

}// namespace Animation
}// namespace View
}// namespace Droid
}// namespace Elastos
