
#ifndef __ORG_APACHE_HTTP_PROTOCOL_URIPATTERNMATCHER_H_
#define __ORG_APACHE_HTTP_PROTOCOL_URIPATTERNMATCHER_H_

#include <Org.Apache.Http_server.h>
#include <elastos/core/Object.h>

using Elastos::Utility::IMap;

namespace Org {
namespace Apache {
namespace Http {
namespace Protocol {

/**
 * Maintains a map of objects keyed by a request URI pattern.
 * Instances can be looked up by request URI.<br/>
 * Patterns may have three formats:
 * <ul>
 *   <li><code>*</code></li>
 *   <li><code>*&lt;uri&gt;</code></li>
 *   <li><code>&lt;uri&gt;*</code></li>
 * </ul>
 *
 * @author <a href="mailto:oleg at ural.ru">Oleg Kalnichevski</a>
 *
 * @version $Revision: 630662 $
 */
class UriPatternMatcher
    : public Object
    , public IUriPatternMatcher
{
public:
    CAR_INTERFACE_DECL()

    virtual ~UriPatternMatcher() {}

    CARAPI Register(
        /* [in] */ String pattern,
        /* [in] */ IInterface* handler);

    CARAPI Unregister(
        /* [in] */ String pattern);

    CARAPI SetHandlers(
        /* [in] */ IMap* map);

    CARAPI Lookup(
        /* [in] */ String requestURI,
        /* [out] */ IInterface** value);

protected:
    CARAPI MatchUriRequestPattern(
        /* [in] */ String pattern,
        /* [in] */ String requestUri,
        /* [out] */ Boolean* isMatch);

    CARAPI Init();

private:
    AutoPtr<IMap> mHandlerMap;
};

} // namespace Protocol
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_PROTOCOL_URIPATTERNMATCHER_H_
