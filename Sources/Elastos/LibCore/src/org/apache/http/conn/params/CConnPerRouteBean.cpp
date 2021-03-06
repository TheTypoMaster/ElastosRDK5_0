
#include "CConnPerRouteBean.h"
#include "CInteger32.h"
#include "Logger.h"

using Elastos::Core::CInteger32;
using Elastos::Utility::IIterable;
using Elastos::Utility::IIterator;
using Elastos::Utility::ISet;
using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Conn {
namespace Params {

CConnPerRouteBean::CConnPerRouteBean()
    : mDefaultMax(0)
{}

CAR_INTERFACE_IMPL_2(CConnPerRouteBean, Object, IConnPerRouteBean, IConnPerRoute)

CAR_OBJECT_IMPL(CConnPerRouteBean)

ECode CConnPerRouteBean::GetDefaultMax(
    /* [out] */ Int32* max)
{
    VALIDATE_NOT_NULL(max)
    *max = mDefaultMax;
    return NOERROR;
}

ECode CConnPerRouteBean::SetDefaultMaxPerRoute(
    /* [in] */ Int32 max)
{
    if (max < 1) {
        Logger::E("CConnPerRouteBean", "The maximum must be greater than 0.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mDefaultMax = max;
    return NOERROR;
}

ECode CConnPerRouteBean::SetMaxForRoute(
    /* [in] */ IHttpRoute* route,
    /* [in] */ Int32 max)
{
    if (route == NULL) {
        Logger::E("CConnPerRouteBean", "HTTP route may not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (max < 1) {
        Logger::E("CConnPerRouteBean", "The maximum must be greater than 0.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IInteger32> integer;
    CInteger32::New(max, (IInteger32**)&integer);
    mMaxPerHostMap[route] = integer;
    return NOERROR;
}

ECode CConnPerRouteBean::GetMaxForRoute(
    /* [in] */ IHttpRoute* route,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number)
    *number = 0;

    if (route == NULL) {
        Logger::E("CConnPerRouteBean", "HTTP route may not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    HashMap<AutoPtr<IHttpRoute>, AutoPtr<IInteger32> >::Iterator it = mMaxPerHostMap.Find(route);
    AutoPtr<IInteger32> max;
    if (it != mMaxPerHostMap.End()) {
        max = it->mSecond;
        return max->GetValue(number);
    }
    else {
        *number = mDefaultMax;
    }
    return NOERROR;
}

ECode CConnPerRouteBean::SetMaxForRoutes(
    /* [in] */ IMap* map)
{
    if (map == NULL) {
        return NOERROR;
    }
    mMaxPerHostMap.Clear();
    AutoPtr<ISet> keys;
    map->GetKeySet((ISet**)&keys);
    AutoPtr<IIterator> it;
    keys->GetIterator((IIterator**)&it);
    Boolean hasNext;
    while (it->HasNext(&hasNext), hasNext) {
        AutoPtr<IHttpRoute> key;
        it->GetNext((IInterface**)&key);
        AutoPtr<IInteger32> value;
        map->Get(key, (IInterface**)&value);
        mMaxPerHostMap[key] = value;
    }
    return NOERROR;
}

ECode CConnPerRouteBean::constructor(
    /* [in] */ Int32 defaultMax)
{
    return SetDefaultMaxPerRoute(defaultMax);
}

ECode CConnPerRouteBean::constructor()
{
    return constructor(DEFAULT_MAX_CONNECTIONS_PER_ROUTE);
}

} // namespace Params
} // namespace Conn
} // namespace Http
} // namespace Apache
} // namespace Org
