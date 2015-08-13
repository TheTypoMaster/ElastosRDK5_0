// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_DEVICEFORMFACTOR_H_
#define _ELASTOS_DROID_WEBKIT_UI_DEVICEFORMFACTOR_H_

// package org.chromium.ui.base;
// import android.content.Context;
// import org.chromium.base.CalledByNative;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

/**
 * UI utilities for accessing form factor information.
 */
class DeviceFormFactor
{
public:
    /**
     * @param context Android's context
     * @return        Whether the app is should treat the device as a tablet for layout.
     */
    //@CalledByNative
    static CARAPI_(Boolean) IsTablet(
        /* in */ IContext* context);

private:
    /**
     * The minimum width that would classify the device as a tablet.
     */
    static const Int32 MINIMUM_TABLET_WIDTH_DP = 600;
    static Boolean sIsTablet;
};

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_DEVICEFORMFACTOR_H_
