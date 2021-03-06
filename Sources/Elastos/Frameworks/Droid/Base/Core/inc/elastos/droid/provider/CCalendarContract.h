
#ifndef __ELASTOS_DROID_PROVIDER_CCALENDARCONTRACT_H__
#define __ELASTOS_DROID_PROVIDER_CCALENDARCONTRACT_H__

#include "_Elastos_Droid_Provider_CCalendarContract.h"
#include "CalendarContract.h"

using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CCalendarContract)
{
public:
    CARAPI constructor();

    /**
     * The content:// style URL for the top-level calendar authority
     */
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);
};

} //Provider
} //Droid
} //Elastos

#endif //__ELASTOS_DROID_PROVIDER_CCALENDARCONTRACT_H__
