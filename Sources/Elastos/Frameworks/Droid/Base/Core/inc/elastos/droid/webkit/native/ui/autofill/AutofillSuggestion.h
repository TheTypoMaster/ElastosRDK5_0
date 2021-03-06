// wuweizuo automatic build .h file from .java file.
// Copyright 2013 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_UI_AUTOFILL_AUTOFILLSUGGESTION_H_
#define _ELASTOS_DROID_WEBKIT_UI_AUTOFILL_AUTOFILLSUGGESTION_H_

#include "elatypes.h"
#include "elautoptr.h"
#include "ext/frameworkext.h"
#include "webkit/native/ui/DropdownItem.h"

// package org.chromium.ui.autofill;
// import org.chromium.ui.DropdownItem;

using Elastos::Droid::Webkit::Ui::DropdownItem;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {
namespace Autofill {

/**
  * Autofill suggestion container used to store information needed for each Autofill popup entry.
  */
class AutofillSuggestion
    : public Object
    , public DropdownItem
{
public:
    /**
      * Constructs a Autofill suggestion container.
      * @param name The name of the Autofill suggestion.
      * @param label The describing label of the Autofill suggestion.
      * @param uniqueId The unique id used to identify the Autofill suggestion.
      */
    AutofillSuggestion(
        /* [in] */ String name,
        /* [in] */ String label,
        /* [in] */ Int32 uniqueId);

    // @Override
    CARAPI_(String) GetLabel();

    // @Override
    CARAPI_(String) GetSublabel();

    // @Override
    CARAPI_(Boolean) IsEnabled();

    // @Override
    CARAPI_(Boolean) IsGroupHeader();

    virtual CARAPI_(Int32) GetUniqueId();

public:
    /*const*/ String mLabel;
    /*const*/ String mSublabel;
    /*const*/ Int32 mUniqueId;
};

} // namespace Autofill
} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_UI_AUTOFILL_AUTOFILLSUGGESTION_H_

