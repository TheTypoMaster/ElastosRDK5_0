
#include "CBasicHttpRequest.h"

namespace Org {
namespace Apache {
namespace Http {
namespace Message {

CAR_OBJECT_IMPL(CBasicHttpRequest)

ECode CBasicHttpRequest::Clone(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj)
    AutoPtr<CBasicHttpRequest> request;
    CBasicHttpRequest::NewByFriend((CBasicHttpRequest**)&request);
    CloneImpl((BasicHttpRequest*)request.Get());
    return NOERROR;
}

ECode CBasicHttpRequest::constructor(
    /* [in] */ const String& method,
    /* [in] */ const String& uri)
{
    return Init(method, uri);
}

ECode CBasicHttpRequest::constructor(
    /* [in] */ const String& method,
    /* [in] */ const String& uri
    /* [in] */ IProtocolVersion* ver)
{
    return Init(method, uri, ver);
}

ECode CBasicHttpRequest::constructor(
    /* [in] */ IRequestLine* requestline)
{
    return Init(requestline);
}

ECode CBasicHttpRequest::constructor()
{
    return NOERROR;
}

} // namespace Message
} // namespace Http
} // namespace Apache
} // namespace Org
