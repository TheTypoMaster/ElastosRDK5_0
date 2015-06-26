#ifndef __ELASTOS_NET_CCOOKIEHANDLERHELPER_H__
#define __ELASTOS_NET_CCOOKIEHANDLERHELPER_H__

#include "_ELASTOS_NET_CCookieHandlerHelper.h"
#include "Singleton.h"

namespace Elastos {
namespace Net {

CarClass(CCookieHandlerHelper)
    , public Singleton
    , public ICookieHandlerHelper
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI GetDefault(
        /* [out] */ ICookieHandler** handler);

    CARAPI SetDefault(
        /* [in] */ ICookieHandler* handler);

private:
   AutoPtr<ICookieHandler> mSystemWideCookieHandler;
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_CCOOKIEHANDLERHELPER_H__
