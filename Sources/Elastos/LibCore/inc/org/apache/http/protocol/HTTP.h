
#ifndef __ORG_APACHE_HTTP_PROTOCOL_HTTP_H_
#define __ORG_APACHE_HTTP_PROTOCOL_HTTP_H_

#include <Org.Apache.Http_server.h>
#include <elastos/core/Object.h>

namespace Org {
namespace Apache {
namespace Http {
namespace Protocol {

/**
 * Constants and static helpers related to the HTTP protocol.
 *
 * @author <a href="mailto:oleg at ural.ru">Oleg Kalnichevski</a>
 *
 * @version $Revision: 555989 $
 *
 * @since 4.0
 */
class HTTP : public IHTTP
{
public:
    CAR_INTERFACE_DECL()

    CARAPI_(Boolean) IsWhitespace(
        /* [in] */ Char32 ch);

private:
    HTTP() {}
};

} // namespace Protocol
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_PROTOCOL_HTTP_H_
