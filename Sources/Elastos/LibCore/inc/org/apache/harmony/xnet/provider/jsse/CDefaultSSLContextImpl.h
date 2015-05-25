
#ifndef __CDEFAULTSSLCONTEXTIMPL_H__
#define __CDEFAULTSSLCONTEXTIMPL_H__

#include "_CDefaultSSLContextImpl.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CDefaultSSLContextImpl)
{
public:
    CARAPI EngineInit(
        /* [in] */ ArrayOf<Elastosx::Net::Ssl::IKeyManager *> * pKm,
        /* [in] */ ArrayOf<Elastosx::Net::Ssl::ITrustManager *> * pTm,
        /* [in] */ ArrayOf<Elastos::Security::ISecureRandom *> * pSr);

    CARAPI EngineGetSocketFactory(
        /* [out] */ Elastosx::Net::Ssl::ISSLSocketFactory ** ppFactory);

    CARAPI EngineGetServerSocketFactory(
        /* [out] */ Elastosx::Net::Ssl::ISSLServerSocketFactory ** ppFactory);

    CARAPI EngineCreateSSLEngine(
        /* [in] */ const String& host,
        /* [in] */ Int32 port,
        /* [out] */ Elastosx::Net::Ssl::ISSLEngine ** ppEngine);

    CARAPI EngineCreateSSLEngineEx(
        /* [out] */ Elastosx::Net::Ssl::ISSLEngine ** ppEngine);

    CARAPI EngineGetServerSessionContext(
        /* [out] */ Elastosx::Net::Ssl::ISSLSessionContext ** ppContext);

    CARAPI EngineGetClientSessionContext(
        /* [out] */ Elastosx::Net::Ssl::ISSLSessionContext ** ppContext);

    CARAPI EngineGetDefaultSSLParameters(
        /* [out] */ Elastosx::Net::Ssl::ISSLParameters ** ppParam);

    CARAPI EngineGetSupportedSSLParameters(
        /* [out] */ Elastosx::Net::Ssl::ISSLParameters ** ppParam);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CDEFAULTSSLCONTEXTIMPL_H__
