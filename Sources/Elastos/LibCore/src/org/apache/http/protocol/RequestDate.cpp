
#include "RequestDate.h"
#include "CHttpDateGenerator.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IHttpStatus;
using Org::Apache::Http::IHttpMessage;
using Org::Apache::Http::IStatusLine;
using Org::Apache::Http::IProtocolVersion;

namespace Org {
namespace Apache {
namespace Http {
namespace Protocol {

static AutoPtr<IHttpDateGenerator> InitDateGenerator()
{
    AutoPtr<IHttpDateGenerator> generator;
    CHttpDateGenerator::NewByFriend((CHttpDateGenerator**)&generator);
    return (IHttpDateGenerator)generator.Get();
}
const AutoPtr<IHttpDateGenerator> RequestDate::DATE_GENERATOR = InitDateGenerator();

CAR_INTERFACE_IMPL(RequestDate, Object, IHttpRequestInterceptor)

ECode RequestDate::Process(
    /* [in] */ IHttpRequest* request,
    /* [in] */ IHttpContext* context)
{
    if (Request == NULL) {
        Logger::E("RequestDate", "HTTP request may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IHttpEntityEnclosingRequest> enclosingRequest = IHttpEntityEnclosingRequest::Probe(request);
    Boolean contains;
    if (enclosingRequest != NULL && (request->ContainsHeader(IHTTP::DATE_HEADER, &contains), !contains)) {
        String httpdate;
        DATE_GENERATOR->GetCurrentDate(&httpdate);
        request->SetHeader(IHTTP::DATE_HEADER, httpdate);
    }
    return NOERROR;
}

} // namespace Protocol
} // namespace Http
} // namespace Apache
} // namespace Org
