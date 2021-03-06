
#include "NetScapeDraftSpec.h"
#include "BasicPathHandler.h"
#include "NetscapeDomainHandler.h"
#include "BasicSecureHandler.h"
#include "BasicMaxAgeHandler.h"
#include "BasicCommentHandler.h"
#include "BasicExpiresHandler.h"
#include "NetscapeDraftHeaderParser.h"
#include "CParserCursor.h"
#include "CBufferedHeader.h"
#include "CCharArrayBuffer.h"
#include <elastos/Logger.h>

using Elastos::Utility::IIterator;
using Elastos::Utility::CList;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IHeaderElement;
using Org::Apache::Http::IFormattedHeader;
using Org::Apache::Http::Cookie::IClientCookie;
using Org::Apache::Http::Cookie::ISM;
using Org::Apache::Http::Message::IParserCursor;
using Org::Apache::Http::Message::CParserCursor;
using Org::Apache::Http::Message::CBufferedHeader;
using Org::Apache::Http::Utility::ICharArrayBuffer;
using Org::Apache::Http::Utility::CCharArrayBuffer;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Cookie {

const String NetScapeDraftSpec::EXPIRES_PATTERN("EEE, dd-MMM-yyyy HH:mm:ss z");

NetscapeDraftSpec::NetScapeDraftSpec(
    /* [in] */ ArrayOf<String>* datepatterns)
    : CookieSpecBase()
{
    Init(datepatterns);
}

NetscapeDraftSpec::NetscapeDraftSpec()
    : CookieSpecBase()
{
    Init(NULL);
}

void NetscapeDraftSpec::Init(
    /* [in] */ ArrayOf<String>* datepatterns)
{
    if (datepatterns != NULL) {
        mDatepatterns = datepatterns->Clone();
    }
    else {
        mDatepatterns = ArrayOf<String>::Alloc(1);
        (*mDatepatterns)[0] = EXPIRES_PATTERN;
    }
    AutoPtr<ICookieAttributeHandler> pathHandler = (ICookieAttributeHandler*)new BasicPathHandler();
    AutoPtr<ICookieAttributeHandler> domainHandler = (ICookieAttributeHandler*)new NetscapeDomainHandler();
    AutoPtr<ICookieAttributeHandler> maxAgeHandler = (ICookieAttributeHandler*)new BasicMaxAgeHandler();
    AutoPtr<ICookieAttributeHandler> secureHandler = (ICookieAttributeHandler*)new BasicSecureHandler();
    AutoPtr<ICookieAttributeHandler> commentHandler = (ICookieAttributeHandler*)new BasicCommentHandler();
    AutoPtr<ICookieAttributeHandler> expiresHandler = (ICookieAttributeHandler*)new BasicExpiresHandler(mDatepatterns);
    RegisterAttribHandler(IClientCookie::PATH_ATTR, pathHandler);
    RegisterAttribHandler(IClientCookie::DOMAIN_ATTR, domainHandler);
    RegisterAttribHandler(IClientCookie::MAX_AGE_ATTR, maxAgeHandler);
    RegisterAttribHandler(IClientCookie::SECURE_ATTR, secureHandler);
    RegisterAttribHandler(IClientCookie::COMMENT_ATTR, commentHandler);
    RegisterAttribHandler(IClientCookie::EXPIRES_ATTR, expiresHandler);
}

ECode NetScapeDraftSpec::Parse(
    /* [in] */ IHeader* header,
    /* [in] */ ICookieOrigin* origin,
    /* [out] */ IList** cookies)
{
    VALIDATE_NOT_NULL(cookies)
    *cookies = NULL;

    if (header == NULL) {
        Logger::E("NetscapeDraftSpec", "Header may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (origin == NULL) {
        Logger::E("NetscapeDraftSpec", "Cookie origin may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<NetscapeDraftHeaderParser> parser = NetscapeDraftHeaderParser::DEFAULT;
    AutoPtr<ICharArrayBuffer> buffer;
    AutoPtr<IParserCursor> cursor;
    AutoPtr<IFormattedHeader> formattedHeader = IFormattedHeader::Probe(header);
    if (formattedHeader != NULL) {
        formattedHeader->GetBuffer((ICharArrayBuffer**)&buffer);
        Int32 pos;
        formattedHeader->GetValuePos(&pos);
        Int32 len;
        buffer->GetLength(&len);
        CParserCursor::New(pos, len, (IParserCursor**)&cursor);
    }
    else {
        String s;
        header->GetValue(&s);
        if (s.IsNull()) {
            Logger::E("NetScapeDraftSpec", "Header value is null");
            return E_MALFORMED_COOKIE_EXCEPTION;
        }
        CCharArrayBuffer::New(s.GetLength(), (ICharArrayBuffer**)&buffer);
        buffer->Append(s);
        Int32 len;
        buffer->GetLength(&len);
        CParserCursor::New(0, len, (IParserCursor**)&cursor);
    }
    AutoPtr<IHeaderElement> elem;
    parser->ParseHeader(buffer, cursor, (IHeaderElement**)&elem);
    AutoPtr< ArrayOf<IHeaderElement*> > elems = ArrayOf<IHeaderElement*>::Alloc(1);
    elems->Set(0, elem);
    return Parse(elems, origin, cookies);
}

ECode NetScapeDraftSpec::FormatCookies(
    /* [in] */ IList* cookies,
    /* [out] */ IList** _headers)
{
    VALIDATE_NOT_NULL(_headers)
    *_headers = NULL;

    if (cookies == NULL) {
        Logger::E("NetScapeDraftSpec", "List of cookie may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Boolean isEmpty;
    if (cookies->IsEmpty(&isEmpty), isEmpty) {
        Logger::E("NetScapeDraftSpec", "List of cookie may not be empty");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 size;
    cookies->GetSize(&size);
    AutoPtr<ICharArrayBuffer> buffer;
    CCharArrayBuffer::New(20 * size, (ICharArrayBuffer**)&buffer);
    buffer->Append(ISM::COOKIE);
    buffer->Append(String(": "));
    AutoPtr<IIterator> it;
    cookies->GetIterator((IIterator**)&it);
    Int32 i = 0;
    Boolean hasNext;
    while (it->HasNext(&hasNext), hasNext) {
        AutoPtr<IInterface> value;
        it->GetNext((IInterface**)&value);
        AutoPtr<ICookie> cookie = ICookie::Probe(value);
        if (i > 0) {
            buffer->Append(String("; "));
        }
        String name;
        cookie->GetName(&name);
        buffer->Append(name);
        String s;
        cookie->GetValue(&s);
        if (!s.IsNull()) {
            buffer->Append(String("="));
            buffer->Append(s);
        }
        i++;
    }
    AutoPtr<IList> headers;
    CList::New(1, (IList**)&headers);
    AutoPtr<IBufferedHeader> header;
    CBufferedHeader::New(buffer, (IBufferedHeader**)&header);
    headers->Add(header);
    *_headers = headers;
    REFCOUNT_ADD(*_headers)
    return NOERROR;
}

ECode NetScapeDraftSpec::GetVersion(
    /* [out] */ Int32* version)
{
    VALIDATE_NOT_NULL(version)
    *version = 0;
    return NOERROR;
}

ECode NetScapeDraftSpec::GetVersionHeader(
    /* [out] */ IHeader* header)
{
    VALIDATE_NOT_NULL(header)
    *header = NULL;
    return NOERROR;
}

} // namespace Cookie
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org