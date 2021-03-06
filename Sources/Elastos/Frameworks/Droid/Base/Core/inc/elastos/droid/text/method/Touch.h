#ifndef __ELASTOS_DROID_TEXT_METHOD_Touch_H__
#define __ELASTOS_DROID_TEXT_METHOD_Touch_H__

#include "Elastos.Droid.Core_server.h"

using Elastos::Droid::Text::Method::ITouchDragState;
using Elastos::Droid::View::IMotionEvent;
using Elastos::Droid::View::IView;
using Elastos::Droid::Widget::ITextView;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

    #define Integer_MAX_VALUE 0x7fffffff
#endif
    #define Integer_MIN_VALUE 0x80000000
#endif

//public
class Touch
    : public Object
    , public ITouch
{
private:
    //static
    class DragState
        : public object
        , public ITouchDragState
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* object,
            /* [in] */ InterfaceID* iid);
    public:
        DragState(
            /* [in] */ Float x,
            /* [in] */ Float y,
            /* [in] */ Int32 scrollX,
            /* [in] */ Int32 scrollY);
    public:
        Float mX;
        Float mY;
        Int32 mScrollX;
        Int32 mScrollY;
        Boolean mFarEnough;
        Boolean mUsed;
    };

public:
    CAR_INTERFACE_DECL()

    /**
     * Scrolls the specified widget to the specified coordinates, except
     * constrains the X scrolling position to the horizontal regions of
     * the text that will be visible after scrolling to the specified
     * Y position.
     */
     static CARAPI_(void) ScrollTo(
        /* [in] */ ITextView* widget,
        /* [in] */ ILayout* layout,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    /**
     * Handles touch events for dragging.  You may want to do other actions
     * like moving the cursor on touch as well.
     */
    static CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer,
        /* [in] */ IMotionEvent* ev ent);

    /**
     * @param widget The text view.
     * @param buffer The text buffer.
     */
    static CARAPI_(Int32) GetInitialScrollX(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    /**
     * @param widget The text view.
     * @param buffer The text buffer.
     */
    static CARAPI_(Int32) GetInitialScrollY(
        /* [in] */ ITextView* widget,
        /* [in] */ ISpannable* buffer);

    static CARAPI_(Boolean) IsActivelySelecting(
        /* [in] */ ISpannable* buffer);

    static CARAPI_(Boolean) IsSelectionStarted(
        /* [in] */ ISpannable* buffer);

private:
    Touch();
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_Touch_H__
