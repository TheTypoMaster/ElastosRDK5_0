
#include "ConnRouteParams.h"
#include "CHttpHost.h"
#include "routing/CHttpRoute.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::CHttpHost;
using Org::Apache::Http::Conn::Routing::CHttpRoute;

namespace Org {
namespace Apache {
namespace Http {
namespace Conn {
namespace Params {

static AutoPtr<IHttpHost> InitHttpHost()
{
    AutoPtr<CHttpHost> host;
    CHttpHost::NewByFriend(String("127.0.0.255"), 0, String("no-host"), (CHttpHost**)&host);
    return (IHttpHost*)host.Get();
}
const AutoPtr<IHttpHost> ConnRouteParams::NO_HOST = InitHttpHost();

static AutoPtr<IHttpRoute> InitHttpRoute()
{
    AutoPtr<CHttpRoute> route;
    CHttpRoute::NewByFriend(ConnRouteParams::NO_HOST, (CHttpRoute**)&route);
    return (IHttpRoute*)route.Get();
}
const AutoPtr<IHttpRoute> ConnRouteParams::NO_ROUTE = InitHttpRoute();

CAR_INTERFACE_IMPL(ConnRouteParams, Object, IConnRoutePNames)

CAR_OBJECT_IMPL(ConnRouteParams)

ECode ConnRouteParams::GetDefaultProxy(
    /* [in] */ IHttpParams* params,
    /* [out] */ IHttpHost** proxy)
{
    VALIDATE_NOT_NULL(proxy)
    *proxy = NULL;

    if (params == NULL) {
        Logger::E("ConnRouteParams", "Parameters must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IObject> o;
    params->GetParameter(DEFAULT_PROXY, (IObject**)&o);
    AutoPtr<IHttpHost> host = IHttpHost::Probe(o);
    if ((host != NULL) && NO_HOST->Equals(host)) {
        // value is explicitly unset
        host = NULL;
    }
    *proxy = host;
    REFCOUNT_ADD(*proxy)
    return NOERROR;
}

ECode ConnRouteParams::SetDefaultProxy(
    /* [in] */ IHttpParams* params,
    /* [in] */ IHttpHost* proxy)
{
    if (params == NULL) {
        Logger::E("ConnRouteParams", "Parameters must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return params->SetParameter(DEFAULT_PROXY, IObject::Probe(proxy));
}

ECode ConnRouteParams::GetForcedRoute(
    /* [in] */ IHttpParams* params,
    /* [out] */ IHttpRoute** route)
{
    VALIDATE_NOT_NULL(route)
    *route = NULL;

    if (params == NULL) {
        Logger::E("ConnRouteParams", "Parameters must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IObject> o;
    params->GetParameter(FORCED_ROUTE, (IObject**)&o);
    AutoPtr<IHttpRoute> httpRoute = IHttpRoute::Probe(o);
    if ((httpRoute != NULL) && NO_ROUTE->Equals(httpRoute)) {
        // value is explicitly unset
        httpRoute = NULL;
    }
    *route = httpRoute;
    REFCOUNT_ADD(*route)
    return NOERROR;
}

ECode ConnRouteParams::SetForcedRoute(
    /* [in] */ IHttpParams* params,
    /* [in] */ IHttpRoute* route)
{
    if (params == NULL) {
        Logger::E("ConnRouteParams", "Parameters must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return params->SetParameter(FORCED_ROUTE, IObject::Probe(route));
}

ECode ConnRouteParams::GetLocalAddress(
    /* [in] */ IHttpParams* params,
    /* [out] */ IInetAddress** local)
{
    VALIDATE_NOT_NULL(local)
    *local = NULL;

    if (params == NULL) {
        Logger::E("ConnRouteParams", "Parameters must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IObject> o;
    params->GetParameter(LOCAL_ADDRESS, (IObject**)&o);
    *local = IInetAddress::Probe(o);
    REFCOUNT_ADD(*local)
    return NOERROR;
}

ECode ConnRouteParams::SetLocalAddress(
    /* [in] */ IHttpParams* params,
    /* [in] */ IInetAddress* local)
{
    if (params == NULL) {
        Logger::E("ConnRouteParams", "Parameters must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return params->SetParameter(LOCAL_ADDRESS, IObject::Probe(local));
}

} // namespace Params
} // namespace Conn
} // namespace Http
} // namespace Apache
} // namespace Org