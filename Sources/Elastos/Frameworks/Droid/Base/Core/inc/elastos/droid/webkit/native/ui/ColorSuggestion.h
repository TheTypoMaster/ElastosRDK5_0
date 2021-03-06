// wuweizuo automatic build .h file from .java file.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_COLORSUGGESTION_H_
#define _ELASTOS_DROID_WEBKIT_UI_COLORSUGGESTION_H_

#include "elatypes.h"
#include "elautoptr.h"
#include "ext/frameworkext.h"

// package org.chromium.ui;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

/**
  * Color suggestion container used to store information for each color button that will be shown in
  * the simple color picker.
  */
class ColorSuggestion : public Object
{
public:
    /**
      * Constructs a color suggestion container.
      * @param color The suggested color.
      * @param label The label for the suggestion.
      */
    ColorSuggestion(
        /* [in] */ Int32 color,
        /* [in] */ String label);

public:
    /*const*/ Int32 mColor;
    /*const*/ String mLabel;
};

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_COLORSUGGESTION_H_

