#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_CACTIONBARCONTAINER_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_CACTIONBARCONTAINER_H__

#include "_Elastos_Droid_Widget_Internal_CActionBarContainer.h"


#include "widget/internal/ActionBarContainer.h"


namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CActionBarContainer), public ActionBarContainer
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    IFRAMELAYOUT_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetPrimaryBackground(
        /* [in] */ IDrawable* bg);

    CARAPI SetStackedBackground(
        /* [in] */ IDrawable* bg);

    CARAPI SetSplitBackground(
        /* [in] */ IDrawable* bg);

    CARAPI SetTransitioning(
        /* [in] */ Boolean isTransitioning);

    CARAPI SetTabContainer(
        /* [in] */ IScrollingTabContainerView* tabView);

    CARAPI GetTabContainer(
        /* [out] */ IView** container);

};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_INTERNAL_CACTIONBARCONTAINER_H__
