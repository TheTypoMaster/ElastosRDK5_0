
#ifndef __CKEYMANAGERFACTORYIMPL_H__
#define __CKEYMANAGERFACTORYIMPL_H__

#include "_CKeyManagerFactoryImpl.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CKeyManagerFactoryImpl)
{
public:
    CARAPI EngineInit(
        /* [in] */ Elastos::Security::IKeyStore * pKs,
        /* [in] */ ArrayOf<Char32> * pPassword);

    CARAPI EngineInitEx(
        /* [in] */ Elastosx::Net::Ssl::IManagerFactoryParameters * pSpec);

    CARAPI EngineGetKeyManagers(
        /* [out, callee] */ ArrayOf<Elastosx::Net::Ssl::IKeyManager *> ** ppManagers);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CKEYMANAGERFACTORYIMPL_H__
