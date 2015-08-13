// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_AUTOFILLSUGGESTION_H_
#define _ELASTOS_DROID_WEBKIT_UI_AUTOFILLSUGGESTION_H_

//package org.chromium.ui.autofill;

//import org.chromium.ui.DropdownItem;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

/**
 * Autofill suggestion container used to store information needed for each Autofill popup entry.
 */
class AutofillSuggestion : public DropdownItem
{
public:
    /**
     * Constructs a Autofill suggestion container.
     * @param name The name of the Autofill suggestion.
     * @param label The describing label of the Autofill suggestion.
     * @param uniqueId The unique id used to identify the Autofill suggestion.
     */
    AutofillSuggestion(
        /* in */ String name,
        /* in */ String label,
        /* in */ Int32 uniqueId);

    //@Override
    CARAPI_(String) GetLabel();

    //@Override
    CARAPI_(String) GetSublabel();

    //@Override
    CARAPI_(Boolean) IsEnabled();

    //@Override
    CARAPI_(Boolean) IsGroupHeader();

    virtual CARAPI_(Int32) GetUniqueId();

private:
    const String mLabel;
    const String mSublabel;
    const Int32 mUniqueId;
};

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_AUTOFILLSUGGESTION_H_

