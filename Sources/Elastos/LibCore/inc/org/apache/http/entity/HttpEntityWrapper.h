
#ifndef __ORG_APACHE_HTTP_ENTITY_HTTPENTITYWRAPPER_H__
#define __ORG_APACHE_HTTP_ENTITY_HTTPENTITYWRAPPER_H__

#include <Org.Apache.Http_server.h>

using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;
using Org::Apache::Http::Entity::IHeader;

namespace Org {
namespace Apache {
namespace Http {
namespace Entity {

/**
 * Base class for wrapping entities.
 * Keeps a {@link #wrappedEntity wrappedEntity} and delegates all
 * calls to it. Implementations of wrapping entities can derive
 * from this class and need to override only those methods that
 * should not be delegated to the wrapped entity.
 *
 * @version $Revision: 496070 $
 *
 * @since 4.0
 */
class HttpEntityWrapper
    : public Object
    , public IHttpEntity
{
public:
    CAR_INTERFACE_DECL()

    CARAPI IsRepeatable(
        /* [out] */ Boolean* isRepeatable);

    CARAPI IsChunked(
        /* [out] */ Boolean* isChunked);

    CARAPI GetContentLength(
        /* [out] */ Int64* length);

    CARAPI GetContentType(
        /* [out] */ IHeader** type);

    CARAPI GetContentEncoding(
        /* [out] */ IHeader** encoding);

    CARAPI GetContent(
        /* [out] */ IInputStream** inputStream);

    CARAPI WriteTo(
        /* [in] */ IOutputStream* outstream);

    CARAPI IsStreaming(
        /* [out] */ Boolean* isStreaming);

    CARAPI ConsumeContent();

protected:
    CARAPI Init(
        /* [in] */ IHttpEntity* entity);

protected:
    /** The wrapped entity. */
    AutoPtr<IHttpEntity> wrappedEntity;
};

} // namespace Entity
} // namespace Http
} // namespace Apache
} // namespace Org

#endif // __ORG_APACHE_HTTP_ENTITY_HTTPENTITYWRAPPER_H__
