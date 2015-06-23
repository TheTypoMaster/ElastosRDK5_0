#ifndef __CNOTIFICATIONICONAREAICONLAYOUT_H_
#define __CNOTIFICATIONICONAREAICONLAYOUT_H_

#include "_CNotificationIconAreaIconLayout.h"



#include "systemui/statusbar/tablet/NotificationIconAreaIconLayout.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Tablet {


CarClass(CNotificationIconAreaIconLayout), public NotificationIconAreaIconLayout
{
public:
    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IVIEWPARENT_METHODS_DECL()
    IVIEWMANAGER_METHODS_DECL()
    IDRAWABLECALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()
    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()
    ILINEARLAYOUT_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);
};


}// namespace Tablet
}// namespace StatusBar
}// namespace SystemUI
}// namespace Droid
}// namespace Elastos

#endif //__CNOTIFICATIONICONAREAICONLAYOUT_H_
