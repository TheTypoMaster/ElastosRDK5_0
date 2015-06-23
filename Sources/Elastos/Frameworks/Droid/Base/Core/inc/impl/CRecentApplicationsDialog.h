#ifndef __CRECENTAPPLICATIONSDIALOG_H__
#define __CRECENTAPPLICATIONSDIALOG_H__

#include "_CRecentApplicationsDialog.h"
#include "impl/RecentApplicationsDialog.h"




namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

CarClass(CRecentApplicationsDialog), public RecentApplicationsDialog
{
public:
    IDIALOG_METHODS_DECL()
    IWINDOWCALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI OnCreateContextMenu(
        /* [in] */ IContextMenu* menu,
        /* [in] */ IView* v,
        /* [in] */ IContextMenuInfo* menuInfo);

    /**
     * Dismiss the dialog and switch to the selected application.
     */
    CARAPI DismissAndSwitch();

    CARAPI OnClick(
        /* [in] */ IView* view);
};

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namepsace Droid
} // namespace Elastos

#endif // __CRECENTAPPLICATIONSDIALOG_H__
