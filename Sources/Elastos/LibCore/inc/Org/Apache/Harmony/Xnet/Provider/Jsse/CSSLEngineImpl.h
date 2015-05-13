
#ifndef __CSSLENGINEIMPL_H__
#define __CSSLENGINEIMPL_H__

#include "_CSSLEngineImpl.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CSSLEngineImpl)
{
public:
    CARAPI GetPeerHost(
        /* [out] */ String * pHost);

    CARAPI GetPeerPort(
        /* [out] */ Int32 * pPort);

    CARAPI BeginHandshake();

    CARAPI CloseInbound();

    CARAPI CloseOutbound();

    CARAPI GetDelegatedTask(
        /* [out] */ Elastos::Core::IRunnable ** ppTask);

    CARAPI GetEnabledCipherSuites(
        /* [out, callee] */ ArrayOf<String> ** ppSuites);

    CARAPI GetEnabledProtocols(
        /* [out, callee] */ ArrayOf<String> ** ppProtocols);

    CARAPI GetEnableSessionCreation(
        /* [out] */ Boolean * pEnabled);

    CARAPI GetHandshakeStatus(
        /* [out] */ Elastosx::Net::Ssl::SSLEngineResult_HandshakeStatus * pStatus);

    CARAPI GetNeedClientAuth(
        /* [out] */ Boolean * pAuth);

    CARAPI GetSession(
        /* [out] */ Elastosx::Net::Ssl::ISSLSession ** ppSession);

    CARAPI GetSupportedCipherSuites(
        /* [out, callee] */ ArrayOf<String> ** ppSuites);

    CARAPI GetSupportedProtocols(
        /* [out, callee] */ ArrayOf<String> ** ppProtocols);

    CARAPI GetUseClientMode(
        /* [out] */ Boolean * pMode);

    CARAPI GetWantClientAuth(
        /* [out] */ Boolean * pAuth);

    CARAPI IsInboundDone(
        /* [out] */ Boolean * pDone);

    CARAPI IsOutboundDone(
        /* [out] */ Boolean * pDone);

    CARAPI SetEnabledCipherSuites(
        /* [in] */ ArrayOf<String> * pSuites);

    CARAPI SetEnabledProtocols(
        /* [in] */ ArrayOf<String> * pProtocols);

    CARAPI SetEnableSessionCreation(
        /* [in] */ Boolean flag);

    CARAPI SetNeedClientAuth(
        /* [in] */ Boolean need);

    CARAPI SetUseClientMode(
        /* [in] */ Boolean mode);

    CARAPI SetWantClientAuth(
        /* [in] */ Boolean want);

    CARAPI Unwrap(
        /* [in] */ Elastos::IO::IByteBuffer * pSrc,
        /* [in] */ ArrayOf<Elastos::IO::IByteBuffer *> * pDsts,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [out] */ Elastosx::Net::Ssl::ISSLEngineResult ** ppResult);

    CARAPI Wrap(
        /* [in] */ ArrayOf<Elastos::IO::IByteBuffer *> * pSrcs,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 length,
        /* [in] */ Elastos::IO::IByteBuffer * pDst,
        /* [out] */ Elastosx::Net::Ssl::ISSLEngineResult ** ppResult);

    CARAPI UnwrapEx(
        /* [in] */ Elastos::IO::IByteBuffer * pSrc,
        /* [in] */ Elastos::IO::IByteBuffer * pDst,
        /* [out] */ Elastosx::Net::Ssl::ISSLEngineResult ** ppResult);

    CARAPI UnwrapEx2(
        /* [in] */ Elastos::IO::IByteBuffer * pSrc,
        /* [in] */ ArrayOf<Elastos::IO::IByteBuffer *> * pDsts,
        /* [out] */ Elastosx::Net::Ssl::ISSLEngineResult ** ppResult);

    CARAPI WrapEx(
        /* [in] */ ArrayOf<Elastos::IO::IByteBuffer *> * pSrcs,
        /* [in] */ Elastos::IO::IByteBuffer * pDst,
        /* [out] */ Elastosx::Net::Ssl::ISSLEngineResult ** ppResult);

    CARAPI WrapEx2(
        /* [in] */ Elastos::IO::IByteBuffer * pSrc,
        /* [in] */ Elastos::IO::IByteBuffer * pDst,
        /* [out] */ Elastosx::Net::Ssl::ISSLEngineResult ** ppResult);

    CARAPI GetSSLParameters(
        /* [out] */ Elastosx::Net::Ssl::ISSLParameters ** ppParameters);

    CARAPI SetSSLParameters(
        /* [in] */ Elastosx::Net::Ssl::ISSLParameters * pP);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CSSLENGINEIMPL_H__
