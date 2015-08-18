#ifndef __ELASTOS_DROID_TEXT_METHOD_CTOUCH_H__
#define __ELASTOS_DROID_TEXT_METHOD_CTOUCH_H__

#include "_Elastos_Droid_Text_Method_CTouch.h"
#include "text/method/Touch.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CTouch)
{
public:
    CARAPI ScrollTo(
        /* [in] */ ITextView* widget,
        /* [in] */ ILayout* layout,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI OnTouchEvent(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* ret);

    CARAPI GetInitialScrollX(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [out] */ Int32* ret);

    CARAPI GetInitialScrollY(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [out] */ Int32* ret);
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_CTOUCH_H__
