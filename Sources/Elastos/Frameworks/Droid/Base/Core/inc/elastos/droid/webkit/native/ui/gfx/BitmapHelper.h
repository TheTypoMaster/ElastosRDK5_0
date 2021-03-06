// wuweizuo automatic build .h file from .java file.
// Copyright 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_GFX_BITMAPHELPER_H_
#define _ELASTOS_DROID_WEBKIT_UI_GFX_BITMAPHELPER_H_

#include "elatypes.h"
#include "elautoptr.h"
#include "ext/frameworkext.h"
#include "content/res/CResources.h"
#include "graphics/CBitmap.h"
#include "graphics/CBitmapFactory.h"

// package org.chromium.ui.gfx;
// import android.content.res.Resources;
// import android.graphics.Bitmap;
// import android.graphics.BitmapFactory;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::IBitmapFactory;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Gfx {

/**
  * Helper class to decode and sample down bitmap resources.
  */
// @JNINamespace("gfx")
class BitmapHelper
{
private:
    // @CalledByNative
    static CARAPI_(AutoPtr<IBitmap>) CreateBitmap(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 bitmapFormatValue);

    /**
      * Decode and sample down a bitmap resource to the requested width and height.
      *
      * @param name The resource name of the bitmap to decode.
      * @param reqWidth The requested width of the resulting bitmap.
      * @param reqHeight The requested height of the resulting bitmap.
      * @return A bitmap sampled down from the original with the same aspect ratio and dimensions.
      *         that are equal to or greater than the requested width and height.
      */
    // @CalledByNative
    static CARAPI_(AutoPtr<IBitmap>) DecodeDrawableResource(
        /* [in] */ String name,
        /* [in] */ Int32 reqWidth,
        /* [in] */ Int32 reqHeight);

    // http://developer.android.com/training/displaying-bitmaps/load-bitmap.html
    static CARAPI_(Int32) CalculateInSampleSize(
        /* [in] */ IBitmapFactory* ::Options* options,
        /* [in] */ Int32 reqWidth,
        /* [in] */ Int32 reqHeight);

    /**
      * Provides a matching integer constant for the Bitmap.Config value passed.
      *
      * @param bitmapConfig The Bitmap Configuration value.
      * @return Matching integer constant for the Bitmap.Config value passed.
      */
    // @CalledByNative
    static CARAPI_(Int32) GetBitmapFormatForConfig(
        /* [in] */  Bitmap);

    /**
      * Provides a matching Bitmap.Config for the enum config value passed.
      *
      * @param bitmapFormatValue The Bitmap Configuration enum value.
      * @return Matching Bitmap.Config  for the enum value passed.
      */
    static CARAPI_(AutoPtr<IBitmap>) .Config getBitmapConfigForFormat(
        /* [in] */ Int32 bitmapFormatValue);
};

} // namespace Gfx
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_GFX_BITMAPHELPER_H_

