
#ifndef __ORG_APACHE_HTTP_IMPL_COOKIE_BESTMATCHSPECFACTORY_H__
#define __ORG_APACHE_HTTP_IMPL_COOKIE_BESTMATCHSPECFACTORY_H__

#include <Org.Apache.Http_server.h>
#include <elastos/core/Object.h>

using Elastos::Core::Object;
using Org::Apache::Http::Cookie::ICookieSpec;
using Org::Apache::Http::Cookie::ICookieSpecFactory;
using Org::Apache::Http::Params::IHttpParams;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Cookie {

class BestMatchSpecFactory
    : public Object
    , public ICookieSpecFactory
{
public:
    CAR_INTERFACE_DECL()

    CARAPI NewInstance(
        /* [in] */ IHttpParams* params,
        /* [out] */ ICookieSpec** spec);
};

} // namespace Cookie
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_IMPL_COOKIE_BESTMATCHSPECFACTORY_H__
