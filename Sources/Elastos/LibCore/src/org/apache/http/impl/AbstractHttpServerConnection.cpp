
#include "AbstractHttpServerConnection.h"
#include "DefaultHttpRequestFactory.h"
#include "LaxContentLengthStrategy.h"
#include "StrictContentLengthStrategy.h"
#include "HttpRequestParser.h"
#include "HttpResponseWriter.h"
#include "Logger.h"

using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IHttpMessage;
using Org::Apache::Http::IStatusLine;
using Org::Apache::Http::Impl::Entity::LaxContentLengthStrategy;
using Org::Apache::Http::Impl::Entity::StrictContentLengthStrategy;
using Org::Apache::Http::Impl::IO::HttpRequestParser;
using Org::Apache::Http::Impl::IO::HttpResponseWriter;
using Org::Apache::Http::IO::IHttpTransportMetrics;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {

AbstractHttpServerConnection::AbstractHttpServerConnection()
{
    mEntityserializer = CreateEntitySerializer();
    mEntitydeserializer = CreateEntityDeserializer();
}

CAR_INTERFACE_IMPL_2(AbstractHttpServerConnection, Object, IHttpServerConnection, IHttpConnection)

AutoPtr<EntityDeserializer> AbstractHttpServerConnection::CreateEntityDeserializer()
{
    AutoPtr<IContentLengthStrategy> strategy = (IContentLengthStrategy*)new LaxContentLengthStrategy();
    AutoPtr<EntityDeserializer> deserializer = new EntityDeserializer(strategy);
    return deserializer;
}

AutoPtr<EntitySerializer> AbstractHttpServerConnection::CreateEntitySerializer()
{
    AutoPtr<IContentLengthStrategy> strategy = (IContentLengthStrategy*)new StrictContentLengthStrategy();
    AutoPtr<EntitySerializer> serializer = new EntitySerializer(strategy);
    return serializer;
}

AutoPtr<IHttpRequestFactory> AbstractHttpServerConnection::CreateHttpRequestFactory()
{
    AutoPtr<IHttpRequestFactory> factory = (IHttpRequestFactory*)new DefaultHttpRequestFactory();
    return factory;
}

AutoPtr<IHttpMessageParser> AbstractHttpServerConnection::CreateRequestParser(
    /* [in] */ ISessionInputBuffer* buffer,
    /* [in] */ IHttpRequestFactory* requestFactory,
    /* [in] */ IHttpParams* params)
{
    // override in derived class to specify a line parser
    AutoPtr<IHttpMessageParser> parser
            = (IHttpMessageParser*)new HttpRequestParser(buffer, NULL, requestFactory, params);
    return parser;
}

AutoPtr<IHttpMessageWriter> AbstractHttpServerConnection::CreateResponseWriter(
    /* [in] */ ISessionOutputBuffer* buffer,
    /* [in] */ IHttpParams* params)
{
    // override in derived class to specify a line formatter
    AutoPtr<IHttpMessageWriter> writer
            = (IHttpMessageWriter*)new HttpResponseWriter(buffer, NULL, params);
    return writer;
}

ECode AbstractHttpServerConnection::Init(
    /* [in] */ ISessionInputBuffer* inbuffer,
    /* [in] */ ISessionOutputBuffer* outbuffer,
    /* [in] */ IHttpParams* params)
{
    if (inbuffer == NULL) {
        Logger::E("AbstractHttpServerConnection", "Input session buffer may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (outbuffer == NULL) {
        Logger::E("AbstractHttpServerConnection", "Output session buffer may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mInbuffer = inbuffer;
    mOutbuffer = outbuffer;
    AutoPtr<IHttpRequestFactory> factory = CreateHttpRequestFactory();
    mRequestParser = CreateRequestParser(inbuffer, factory, params);
    mResponseWriter = CreateResponseWriter(outbuffer, params);
    AutoPtr<IHttpTransportMetrics> inMetrics, outMetrics;
    inbuffer->GetMetrics((IHttpTransportMetrics**)&inMetrics);
    outbuffer->GetMetrics((IHttpTransportMetrics**)&outMetrics);
    mMetrics = new HttpConnectionMetricsImpl(inMetrics, outMetrics);
    return NOERROR;
}

ECode AbstractHttpServerConnection::ReceiveRequestHeader(
    /* [out] */ IHttpRequest** _request)
{
    VALIDATE_NOT_NULL(_request)
    *_request = NULL;
    FAIL_RETURN(AssertOpen())
    AutoPtr<IHttpMessage> message;
    mRequestParser->Parse((IHttpMessage**)&message);
    AutoPtr<IHttpRequest> request = IHttpRequest::Probe(message);
    mMetrics->IncrementRequestCount();
    *_request = request;
    REFCOUNT_ADD(*_request)
    return NOERROR;
}

ECode AbstractHttpServerConnection::ReceiveRequestEntity(
    /* [in] */ IHttpEntityEnclosingRequest* request)
{
    if (request == NULL) {
        Logger::E("AbstractHttpServerConnection", "HTTP request may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    FAIL_RETURN(AssertOpen())
    AutoPtr<IHttpEntity> entity;
    mEntitydeserializer->Deserialize(mInbuffer, IHttpMessage::Probe(request)
            , (IHttpEntity**)&entity);
    request->SetEntity(entity);
    return NOERROR;
}

ECode AbstractHttpServerConnection::DoFlush()
{
    return mOutbuffer->Flush();
}

ECode AbstractHttpServerConnection::Flush()
{
    FAIL_RETURN(AssertOpen())
    return DoFlush();
}

ECode AbstractHttpServerConnection::SendResponseHeader(
    /* [in] */ IHttpResponse* response)
{
    if (response == NULL) {
        Logger::E("AbstractHttpServerConnection", "HTTP response may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    FAIL_RETURN(AssertOpen())
    mResponseWriter->Write(IHttpMessage::Probe(response));
    AutoPtr<IStatusLine> statusLine;
    response->GetStatusLine((IStatusLine**)&statusLine);
    Int32 code;
    if (statusLine->GetStatusCode(&code), code >= 200) {
        mMetrics->IncrementResponseCount();
    }
    return NOERROR;
}

ECode AbstractHttpServerConnection::SendResponseEntity(
    /* [in] */ IHttpResponse* response)
{
    AutoPtr<IHttpEntity> entity;
    if (response->GetEntity((IHttpEntity**)&entity), entity == NULL) {
        return NOERROR;
    }
    mEntityserializer->Serialize(mOutbuffer, IHttpMessage::Probe(response), entity);
    return NOERROR;
}

ECode AbstractHttpServerConnection::IsStale(
    /* [out] */ Boolean* isStale)
{
    VALIDATE_NOT_NULL(isStale)
    FAIL_RETURN(AssertOpen())
    // try {
    Boolean result;
    if (FAILED(mInbuffer->IsDataAvailable(1, &result))) {
        *isStale = TRUE;
        return NOERROR;
    }
    *isStale = FALSE;
    return NOERROR;
    // } catch (IOException ex) {
    //     return true;
    // }
}

ECode AbstractHttpServerConnection::GetMetrics(
    /* [out] */ IHttpConnectionMetrics** metrics)
{
    VALIDATE_NOT_NULL(metrics)
    *metrics = mMetrics;
    REFCOUNT_ADD(*metrics)
    return NOERROR;
}

} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org