
#include "HttpClientParams.h"
#include <elastos/Logger.h>

using Elastos::Core::CStringWrapper;
using Elastos::Core::ICharSequence;
using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Client {
namespace Params {

ECode HttpClientParams::IsRedirecting(
    /* [in] */ IHttpParams* params,
    /* [out] */ Boolean* isRedirecting)
{
    VALIDATE_NOT_NULL(isRedirecting)
    *isRedirecting = FALSE;

    if (params == NULL) {
        Logger::E("HttpClientParams", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return params->GetBooleanParameter(IClientPNames::HANDLE_REDIRECTS, TRUE, isRedirecting);
}

ECode HttpClientParams::SetRedirecting(
    /* [in] */ IHttpParams* params,
    /* [in] */ Boolean value)
{
    if (params == NULL) {
        Logger::E("HttpClientParams", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return params->SetBooleanParameter(IClientPNames::HANDLE_REDIRECTS, value);
}

ECode HttpClientParams::IsAuthenticating(
    /* [in] */ IHttpParams* params,
    /* [out] */ Boolean* isAuthenticating)
{
    VALIDATE_NOT_NULL(isAuthenticating)
    *isAuthenticating = FALSE;

    if (params == NULL) {
        Logger::E("HttpClientParams", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return params->GetBooleanParameter(IClientPNames::HANDLE_AUTHENTICATION, TRUE, isAuthenticating);
}

ECode HttpClientParams::SetAuthenticating(
    /* [in] */ IHttpParams* params,
    /* [in] */ Boolean value)
{
    if (params == NULL) {
        Logger::E("HttpClientParams", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return params->SetBooleanParameter(IClientPNames::HANDLE_AUTHENTICATION, value);
}

ECode HttpClientParams::GetCookiePolicy(
    /* [in] */ IHttpParams* params,
    /* [out] */ String* cookiePolicy)
{
    VALIDATE_NOT_NULL(cookiePolicy)
    *cookiePolicy = String(NULL);

    if (params == NULL) {
        Logger::E("HttpClientParams", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IObject> o;
    params->GetParameter(IClientPNames::COOKIE_POLICY, (IObject**)&o);
    AutoPtr<ICharSequence> cs = ICharSequence::Probe(o);
    if (cs == NULL) {
        *cookiePolicy = ICookiePolicy::BEST_MATCH;
        return NOERROR;
    }
    return cs->ToString(cookiePolicy);
}

ECode HttpClientParams::SetCookiePolicy(
    /* [in] */ IHttpParams* params,
    /* [in] */ const String& cookiePolicy)
{
    if (params == NULL) {
        Logger::E("HttpClientParams", "HTTP parameters may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<ICharSequence> cs;
    CStringWrapper::New(cookiePolicy, (ICharSequence**)&cs);
    AutoPtr<IObject> o = IObject::Probe(cs);
    return params->SetParameter(IClientPNames::COOKIE_POLICY, o);
}

} // namespace Params
} // namespace Client
} // namespace Http
} // namespace Apache
} // namespace Org