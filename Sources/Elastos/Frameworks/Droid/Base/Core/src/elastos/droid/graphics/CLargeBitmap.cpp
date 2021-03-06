
#include "ext/frameworkext.h"
#include "graphics/CLargeBitmap.h"
#include "graphics/CRect.h"
#include "graphics/CBitmapFactoryOptions.h"
#include "graphics/CBitmapFactory.h"
#include "graphics/AutoDecoderCancel.h"
#include "graphics/GraphicsNative.h"
#include "graphics/NBitmapFactory.h"
#include <skia/core/SkTemplates.h>
#include <skia/core/SkPixelRef.h>
#include <skia/images/SkBitmapRegionDecoder.h>
#include <skia/core/SkTScopedPtr.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

CAR_OBJECT_IMPL(CLargeBitmap);
CAR_INTERFACE_IMPL(CLargeBitmap, Object, ILargeBitmap);
CLargeBitmap::CLargeBitmap()
    : mNativeLargeBitmap(0)
    , mRecycled(FALSE)
{}

CLargeBitmap::~CLargeBitmap()
{
    Recycle();
}

ECode CLargeBitmap::constructor(
    /* [in] */ Int32 lbm)
{
    mNativeLargeBitmap = lbm;
    mRecycled = FALSE;
    return NOERROR;
}

ECode CLargeBitmap::DecodeRegion(
    /* [in] */ IRect* _rect,
    /* [in] */ IBitmapFactoryOptions* options,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    FAIL_RETURN(CheckRecycled("decodeRegion called on recycled large bitmap"));

    CRect* rect = (CRect*)_rect;
    Int32 width;
    Int32 height;
    GetWidth(&width);
    GetHeight(&height);
    if (rect->mLeft < 0 || rect->mTop < 0 ||
        rect->mRight > width || rect->mBottom > height) {
        // throw new IllegalArgumentException("rectangle is not inside the image");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NativeDecodeRegion(mNativeLargeBitmap,
            rect->mLeft, rect->mTop, rect->mRight - rect->mLeft,
            rect->mBottom - rect->mTop, options, bitmap);
}

ECode CLargeBitmap::GetWidth(
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length);

    FAIL_RETURN(CheckRecycled("getWidth called on recycled large bitmap"));
    *length = NativeGetWidth(mNativeLargeBitmap);
    return NOERROR;
}

ECode CLargeBitmap::GetHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);

    FAIL_RETURN(CheckRecycled("getHeight called on recycled large bitmap"));
    *height =  NativeGetHeight(mNativeLargeBitmap);
    return NOERROR;
}

ECode CLargeBitmap::Recycle()
{
    if (!mRecycled) {
        NativeClean(mNativeLargeBitmap);
        mRecycled = TRUE;
    }
    return NOERROR;
}

ECode CLargeBitmap::IsRecycled(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    *result = mRecycled;
    return NOERROR;
}

ECode CLargeBitmap::CheckRecycled(
    /* [in] */ CString errorMessage)
{
    if (mRecycled) {
        // throw new IllegalStateException(errorMessage);
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode CLargeBitmap::NativeDecodeRegion(
    /* [in] */ Int32 lbm,
    /* [in] */ Int32 startX,
    /* [in] */ Int32 startY,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ IBitmapFactoryOptions* options,
    /* [out] */ IBitmap** bitmap)
{
    VALIDATE_NOT_NULL(bitmap);

    AutoPtr<IBitmap> tileBitmap;
    SkImageDecoder* decoder = ((SkBitmapRegionDecoder*)lbm)->getDecoder();
    Int32 sampleSize = 1;
    SkBitmap::Config prefConfig = SkBitmap::kNo_Config;
    bool doDither = true;
    bool preferQualityOverSpeed = false;
    CBitmapFactoryOptions* optObj = (CBitmapFactoryOptions*)options;

    if (NULL != optObj) {
        sampleSize = optObj->mInSampleSize;
        // initialize these, in case we fail later on
        optObj->mOutWidth = -1;
        optObj->mOutHeight = -1;
        optObj->mOutMimeType = NULL;

        prefConfig = GraphicsNative::GetNativeBitmapConfig(optObj->mInPreferredConfig);
        doDither = (bool)optObj->mInDither;
        preferQualityOverSpeed = (bool)optObj->mInPreferQualityOverSpeed;
        // Get the bitmap for re-use if it exists.
        tileBitmap = optObj->mInBitmap;
    }

    decoder->setDitherImage(doDither);
    decoder->setPreferQualityOverSpeed(preferQualityOverSpeed);
    AutoDecoderCancel adc(options, decoder);

    // To fix the race condition in case "requestCancelDecode"
    // happens earlier than AutoDecoderCancel object is added
    // to the gAutoDecoderCancelMutex linked list.
    if (NULL != optObj && optObj->mCancel) {
        *bitmap = NULL;
        return NOERROR;
    }

    SkIRect region;
    region.fLeft = startX;
    region.fTop = startY;
    region.fRight = startX + width;
    region.fBottom = startY + height;
    SkBitmap* nbitmap = NULL;
    SkTScopedPtr<SkBitmap> adb;

    if (tileBitmap != NULL) {
        // Re-use bitmap.
        nbitmap = GraphicsNative::GetNativeBitmap(tileBitmap);
    }
    if (nbitmap == NULL) {
        nbitmap = new SkBitmap;
        adb.reset(nbitmap);
    }

    if (!((SkBitmapRegionDecoder*)lbm)->decodeRegion(nbitmap, region, prefConfig, sampleSize)) {
        *bitmap = NULL;
        return NOERROR;
    }

    // update options (if any)
    if (NULL != optObj) {
        optObj->mOutWidth = nbitmap->width();
        optObj->mOutHeight = nbitmap->height();
        // TODO: set the mimeType field with the data from the codec.
        // but how to reuse a set of strings, rather than allocating new one
        // each time?
        optObj->mOutMimeType = NBitmapFactory::GetMimeTypeString(decoder->getFormat());
    }

    if (tileBitmap != NULL) {
        *bitmap = tileBitmap;
        REFCOUNT_ADD(*bitmap);
        return NOERROR;
    }

    // detach bitmap from its autotdeleter, since we want to own it now
    adb.release();

    GraphicsNative::DroidPixelAllocator* allocator = (GraphicsNative::DroidPixelAllocator*)decoder->getAllocator();
    AutoPtr< ArrayOf<Byte> > buff = allocator->getStorageObjAndReset();
    AutoPtr<CBitmap> bitmapObj;
    ECode ec = GraphicsNative::CreateBitmap(nbitmap, buff, FALSE, NULL, NULL, -1, (CBitmap**)&bitmapObj);
    *bitmap = (IBitmap*)bitmapObj.Get();
    REFCOUNT_ADD(*bitmap);
    return NOERROR;
}

Int32 CLargeBitmap::NativeGetWidth(
    /* [in] */ Int32 lbm)
{
    return ((SkBitmapRegionDecoder*)lbm)->getWidth();
}

Int32 CLargeBitmap::NativeGetHeight(
    /* [in] */ Int32 lbm)
{
    return ((SkBitmapRegionDecoder*)lbm)->getHeight();
}

void CLargeBitmap::NativeClean(
    /* [in] */ Int32 lbm)
{
    delete (SkBitmapRegionDecoder*)lbm;
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
