
#ifndef __ELASTOS_DROID_APPWIDGET_CAPPWIDGETMANAGERHELPER_H__
#define __ELASTOS_DROID_APPWIDGET_CAPPWIDGETMANAGERHELPER_H__

#include "_Elastos_Droid_AppWidget_CAppWidgetManagerHelper.h"

using Elastos::Droid::Content::IContext;

namespace Elastos {
namespace Droid {
namespace AppWidget {

CarClass(CAppWidgetManagerHelper)
{
public:
    /**
     * Get the AppWidgetManager instance to use for the supplied {@link android.content.Context
     * Context} object.
     */
    CARAPI GetInstance(
        /* [in] */ IContext* context,
        /* [out] */ IAppWidgetManager** appWidgetManager);
};

} // namespace AppWidget
} // namespace Droid
} // namespace Elastos

#endif  // __ELASTOS_DROID_APPWIDGET_CAPPWIDGETMANAGERHELPER_H__
