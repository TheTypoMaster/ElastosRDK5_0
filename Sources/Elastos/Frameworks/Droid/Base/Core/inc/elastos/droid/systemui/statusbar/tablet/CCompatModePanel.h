#ifndef __ELASTOS_DROID_SYSTEMUI_STATUSBAR_TABLET_CCOMPATMODEPANEL_H__
#define __ELASTOS_DROID_SYSTEMUI_STATUSBAR_TABLET_CCOMPATMODEPANEL_H__

#include "_Elastos_Droid_SystemUI_StatusBar_Tablet_CCompatModePanel.h"



#include "systemui/statusbar/tablet/CompatModePanel.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Tablet {

CarClass(CCompatModePanel), public CompatModePanel
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
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI SetTrigger(
        /* [in] */  IView* view);

    CARAPI OpenPanel();

    CARAPI ClosePanel();

    CARAPI OnClick(
        /* [in] */ IView* v);

    CARAPI IsInContentArea(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [out] */ Boolean* result);
};

}// namespace Tablet
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_SYSTEMUI_STATUSBAR_TABLET_CCOMPATMODEPANEL_H__
