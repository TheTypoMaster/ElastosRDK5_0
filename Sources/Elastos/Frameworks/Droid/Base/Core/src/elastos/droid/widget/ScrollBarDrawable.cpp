
#include "widget/ScrollBarDrawable.h"
#include <elastos/core/Math.h>
#include "graphics/CRect.h"

using Elastos::Droid::Graphics::IPixelFormat;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Graphics::CanvasEdgeType_AA;
using Elastos::Droid::Graphics::Drawable::EIID_IDrawable;
using Elastos::Droid::Graphics::Drawable::IDrawableCallback;

namespace Elastos {
namespace Droid {
namespace Widget {

ScrollBarDrawable::ScrollBarDrawable()
    : mRange(0)
    , mOffset(0)
    , mExtent(0)
    , mVertical(FALSE)
    , mChanged(FALSE)
    , mRangeChanged(FALSE)
    , mAlwaysDrawHorizontalTrack(FALSE)
    , mAlwaysDrawVerticalTrack(FALSE)
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mTempBounds));
}

ECode ScrollBarDrawable::SetAlwaysDrawHorizontalTrack(
    /* [in] */ Boolean alwaysDrawTrack)
{
    mAlwaysDrawHorizontalTrack = alwaysDrawTrack;
    return NOERROR;
}

ECode ScrollBarDrawable::SetAlwaysDrawVerticalTrack(
    /* [in] */ Boolean alwaysDrawTrack)
{
    mAlwaysDrawVerticalTrack = alwaysDrawTrack;
    return NOERROR;
}

Boolean ScrollBarDrawable::GetAlwaysDrawHorizontalTrack()
{
    return mAlwaysDrawHorizontalTrack;
}

Boolean ScrollBarDrawable::GetAlwaysDrawVerticalTrack()
{
    return mAlwaysDrawVerticalTrack;
}

ECode ScrollBarDrawable::SetParameters(
    /* [in] */ Int32 range,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 extent,
    /* [in] */ Boolean vertical)
{
    if (mVertical != vertical) {
        mChanged = TRUE;
    }

    if (mRange != range || mOffset != offset || mExtent != extent) {
        mRangeChanged = TRUE;
    }

    mRange = range;
    mOffset = offset;
    mExtent = extent;
    mVertical = vertical;
    return NOERROR;
}

ECode ScrollBarDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    const Boolean vertical = mVertical;
    const Int32 extent = mExtent;
    const Int32 range = mRange;

    Boolean drawTrack = TRUE;
    Boolean drawThumb = TRUE;
    if (extent <= 0 || range <= extent) {
        drawTrack = vertical ? mAlwaysDrawVerticalTrack : mAlwaysDrawHorizontalTrack;
        drawThumb = FALSE;
    }

    AutoPtr<IRect> rect = GetBounds();
    Int32 l, t, r, b;
    rect->Get(&l, &t, &r, &b);
    Boolean notIntersected;
    FAIL_RETURN(canvas->QuickReject(l, t, r, b, CanvasEdgeType_AA, &notIntersected));
    if (notIntersected) {
        return NOERROR;
    }
    if (drawTrack) {
        DrawTrack(canvas, rect, vertical);
    }

    if (drawThumb) {
        Int32 height, width;
        rect->GetHeight(&height);
        rect->GetWidth(&width);
        Int32 size = vertical ? height : width;
        Int32 thickness = vertical ? width : height;
        Int32 length = Elastos::Core::Math::Round((Float)size * extent / range);
        Int32 offset = Elastos::Core::Math::Round((Float)(size - length) * mOffset / (range - extent));

        // avoid the tiny thumb
        Int32 minLength = thickness * 2;
        if (length < minLength) {
            length = minLength;
        }
        // avoid the too-big thumb
        if (offset + length > size) {
            offset = size - length;
        }

        DrawThumb(canvas, rect, offset, length, vertical);
    }
    return NOERROR;
}

void ScrollBarDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    Drawable::OnBoundsChange(bounds);
    mChanged = TRUE;
}

void ScrollBarDrawable::DrawTrack(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* bounds,
    /* [in] */ Boolean vertical)
{
    AutoPtr<IDrawable> track;
    if (vertical) {
        track = mVerticalTrack;
    } else {
        track = mHorizontalTrack;
    }
    if (track != NULL) {
        if (mChanged) {
            track->SetBounds(bounds);
        }
        track->Draw(canvas);
    }
}

void ScrollBarDrawable::DrawThumb(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IRect* bounds,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length,
    /* [in] */ Boolean vertical)
{
    const AutoPtr<IRect> thumbRect = mTempBounds;
    const Boolean changed = mRangeChanged || mChanged;
    if (changed) {
        Int32 l, t, r, b;
        bounds->Get(&l, &t, &r, &b);
        if (vertical) {
            thumbRect->Set(l,  t + offset, r, t + offset + length);
        } else {
            thumbRect->Set(l + offset, t, l + offset + length, b);
        }
    }

    if (vertical) {
        if (mVerticalThumb != NULL) {
            if (changed) {
                mVerticalThumb->SetBounds(thumbRect);
            }
            mVerticalThumb->Draw(canvas);
        }
    }
    else {
        if (mHorizontalThumb != NULL) {
            if (changed) {
                mHorizontalThumb->SetBounds(thumbRect);
            }
            mHorizontalThumb->Draw(canvas);
        }
    }
}

ECode ScrollBarDrawable::SetVerticalThumbDrawable(
    /* [in] */ IDrawable* thumb)
{
    if (thumb != NULL) {
        mVerticalThumb = thumb;
    }
    return NOERROR;
}

ECode ScrollBarDrawable::SetVerticalTrackDrawable(
    /* [in] */ IDrawable* track)
{
    mVerticalTrack = track;
    return NOERROR;
}

ECode ScrollBarDrawable::SetHorizontalThumbDrawable(
    /* [in] */ IDrawable* thumb)
{
    if (thumb != NULL) {
        mHorizontalThumb = thumb;
    }
    return NOERROR;
}

ECode ScrollBarDrawable::SetHorizontalTrackDrawable(
    /* [in] */ IDrawable* track)
{
    mHorizontalTrack = track;
    return NOERROR;
}

Int32 ScrollBarDrawable::GetSize(
    /* [in] */ Boolean vertical)
{
    Int32 value = 0;
    if (vertical) {
        if (mVerticalTrack != NULL) {
            mVerticalTrack->GetIntrinsicWidth(&value);
        }
        else if (mVerticalThumb != NULL) {
            mVerticalThumb->GetIntrinsicWidth(&value);
        }
    }
    else {
        if (mHorizontalTrack != NULL) {
            mHorizontalTrack->GetIntrinsicHeight(&value);
        }
        else if (mHorizontalThumb != NULL) {
            mHorizontalThumb->GetIntrinsicHeight(&value);
        }
    }
    return value;
}

ECode ScrollBarDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    if (mVerticalTrack != NULL) {
        mVerticalTrack->SetAlpha(alpha);
    }

    if (mVerticalThumb != NULL) {
        mVerticalThumb->SetAlpha(alpha);
    }

    if (mHorizontalTrack != NULL) {
        mHorizontalTrack->SetAlpha(alpha);
    }

    if (mHorizontalThumb != NULL) {
        mHorizontalThumb->SetAlpha(alpha);
    }

    return NOERROR;
}

ECode ScrollBarDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    if (mVerticalTrack != NULL) {
        mVerticalTrack->SetColorFilter(cf);
    }

    if (mVerticalThumb != NULL) {
        mVerticalThumb->SetColorFilter(cf);
    }

    if (mHorizontalTrack != NULL) {
        mHorizontalTrack->SetColorFilter(cf);
    }

    if (mHorizontalThumb != NULL) {
        mHorizontalThumb->SetColorFilter(cf);
    }

    return NOERROR;
}

Int32 ScrollBarDrawable::GetOpacity()
{
    return IPixelFormat::TRANSLUCENT;
}

ECode ScrollBarDrawable::InvalidateDrawable(
    /* [in] */ IDrawable* who)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->InvalidateDrawable(
                (IDrawable*)this->Probe(EIID_IDrawable));
    }
    return NOERROR;
}

ECode ScrollBarDrawable::ScheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what,
    /* [in] */ Int64 when)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->ScheduleDrawable(
                (IDrawable*)this->Probe(EIID_IDrawable), what, when);
    }
    return NOERROR;
}

ECode ScrollBarDrawable::UnscheduleDrawable(
    /* [in] */ IDrawable* who,
    /* [in] */ IRunnable* what)
{
    AutoPtr<IDrawableCallback> callback = GetCallback();
    if (callback != NULL) {
        callback->UnscheduleDrawable(
                (IDrawable*)this->Probe(EIID_IDrawable), what);
    }
    return NOERROR;
}


}// namespace Widget
}// namespace Droid
}// namespace Elastos