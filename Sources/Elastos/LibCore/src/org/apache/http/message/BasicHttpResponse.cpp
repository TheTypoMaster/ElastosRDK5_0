
#include "BasicHttpResponse.h"
#include "CBasicStatusLine.h"
#include <elastos/Logger.h>

using Elastos::Utility::ILocaleHelper;
using Elastos::Utility::CLocaleHelper;
using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Message {

CAR_INTERFACE_IMPL_2(BasicHttpResponse, AbstractHttpMessage, IBasicHttpResponse, IHttpResponse)

ECode BasicHttpResponse::Init(
    /* [in] */ IStatusLine* statusline,
    /* [in] */ IReasonPhraseCatalog* catalog,
    /* [in] */ ILocale* locale)
{
    AbstractHttpMessage::Init();

    if (statusline == NULL) {
        Logger::E("BasicHttpResponse", "Status line may not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mStatusline    = statusline;
    mReasonCatalog = catalog;
    if (locale != NULL) {
        mLocale = locale;
    }
    else {
        mLocale = NULL;
        AutoPtr<ILocaleHelper> helper;
        CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
        helper->GetDefault((ILocale**)&mLocale);
    }
    return NOERROR;
}

ECode BasicHttpResponse::Init(
    /* [in] */ IStatusLine* statusline)
{
    return Init(statusline, NULL, NULL);
}

ECode BasicHttpResponse::Init(
    /* [in] */ IProtocolVersion* ver,
    /* [in] */ Int32 code,
    /* [in] */ const String& reason)
{
    AutoPtr<IStatusLine> line;
    CBasicStatusLine(ver, code, reason, (IStatusLine**)&line);
    return Init(line, NULL, NULL);
}

ECode BasicHttpResponse::GetProtocolVersion(
    /* [out] */ IProtocolVersion** protocolVersion)
{
    VALIDATE_NOT_NULL(ProtocolVersion)
    return mStatusline->GetProtocolVersion(ProtocolVersion);
}

ECode BasicHttpResponse::GetStatusLine(
    /* [out] */ IStatusLine** statusLine)
{
    VALIDATE_NOT_NULL(statusLine)
    *statusLine = mStatusline;
    REFCOUNT_ADD(*statusLine)
    return NOERROR;
}

ECode BasicHttpResponse::GetEntity(
    /* [out] */ IHttpEntity** entity)
{
    VALIDATE_NOT_NULL(entity)
    *entity = mEntity;
    REFCOUNT_ADD(*entity)
    return NOERROR;
}

ECode BasicHttpResponse::GetLocale(
    /* [out] */ ILocale** loc)
{
    VALIDATE_NOT_NULL(loc)
    *loc = mLocale;
    REFCOUNT_ADD(*loc)
    return NOERROR;
}

ECode BasicHttpResponse::SetStatusLine(
    /* [in] */ IStatusLine* statusLine)
{
    if (statusline == NULL) {
        Logger::E("BasicHttpResponse", "Status line may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mStatusline = statusline;
    return NOERROR;
}

ECode BasicHttpResponse::SetStatusLine(
    /* [in] */ IProtocolVersion* ver,
    /* [in] */ Int32 code)
{
    // arguments checked in BasicStatusLine constructor
    String reason;
    GetReason(code, &reason);
    mStatusline = NULL;
    return CBasicStatusLine::New(ver, code, reason, (IStatusLine**)&mStatusline);
}

ECode BasicHttpResponse::SetStatusLine(
    /* [in] */ IProtocolVersion* ver,
    /* [in] */ Int32 code,
    /* [in] */ const String& reason)
{
    // arguments checked in BasicStatusLine constructor
    mStatusline = NULL;
    return CBasicStatusLine::New(ver, code, reason, (IStatusLine**)&,mStatusline);
}

ECode BasicHttpResponse::SetStatusCode(
    /* [in] */ Int32 code)
{
    // argument checked in BasicStatusLine constructor
    AutoPtr<IProtocolVersion> ver;
    mStatusline->GetProtocolVersion((IProtocolVersion**)&ver);
    String reason;
    GetReason(code, &reason);
    mStatusline = NULL;
    return CBasicStatusLine::New(ver, code, reason, (IStatusLine**)&mStatusline);
}

ECode BasicHttpResponse::SetReasonPhrase(
    /* [in] */ const String& reason)
{
    if ((!reason.IsNull()) && ((reason.IndexOf('\n') >= 0) ||
            (reason.IndexOf('\r') >= 0))) {
        Logger::E("BasicHttpResponse", "Line break in reason phrase.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IProtocolVersion> ver;
    mStatusline->GetProtocolVersion((IProtocolVersion**)&ver);
    Int32 code;
    mStatusline->GetStatusCode(&code);
    mStatusline = NULL;
    return CBasicStatusLine::New(ver, code, reason, (IStatusLine**)&mStatusline);
}

ECode BasicHttpResponse::SetEntity(
    /* [in] */ IHttpEntity* entity)
{
    mEntity = entity;
    return NOERROR;
}

ECode BasicHttpResponse::SetLocale(
    /* [in] */ ILocale* loc)
{
    if (loc == NULL) {
        Logger::E("BasicHttpResponse", "Locale may not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mLocale = loc;
    Int32 code;
    mStatusline->GetStatusCode(&code);
    AutoPtr<IProtocolVersion> ver;
    mStatusline->GetProtocolVersion((IProtocolVersion**)&ver);
    String reason;
    GetReason(code, &reason);
    mStatusline = NULL;
    return CBasicStatusLine::New(ver, code, reason, (IStatusLine**)&mStatusline);
}

ECode BasicHttpResponse::GetReason(
    /* [in] */ Int32 code,
    /* [out] */ String* reason)
{
    VALIDATE_NOT_NULL(reason)
    if (mReasonCatalog) {
        return mReasonCatalog->GetReason(code, mLocale, reason);
    }
    else {
        *reason = String(NULL);
        return NOERROR;
    }
}

void BasicHttpResponse::CloneImpl(
    /* [in] */ BasicHttpResponse* obj)
{
    CloneImpl((AbstractHttpMessage*)obj);
    obj->mStatusline = mStatusline;
    obj->mEntity = mEntity;
    obj->mReasonCatalog = mReasonCatalog;
    obj->mLocale = mLocale;
}

} // namespace Message
} // namespace Http
} // namespace Apache
} // namespace Org
