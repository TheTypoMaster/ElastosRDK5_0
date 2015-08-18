
#ifndef __ELASTOS_SECURITY_CKEYSTOREPASSWORDPROTECTION_H__
#define __ELASTOS_SECURITY_CKEYSTOREPASSWORDPROTECTION_H__

#include "_Elastos_Security_CKeyStorePasswordProtection.h"
#include "elastos/Object.h"


namespace Elastos {
namespace Security {

CarClass(CKeyStorePasswordProtection)
{
public:
    CKeyStorePasswordProtection();

    CARAPI GetPassword(
        /* [out, callee] */ ArrayOf<Char32> **pwd);

    CARAPI Destroy();

    CARAPI IsDestroyed(
        /* [out] */ Boolean *isDestroyed);

    CARAPI constructor(
        /* [in] */ ArrayOf<Char32> *password);

private:
    // Store password
    AutoPtr<ArrayOf<Char32> > mPassword;

    Boolean mIsDestroyed;

    Object mLock;
};

}
}

#endif // __ELASTOS_SECURITY_CKEYSTOREPASSWORDPROTECTION_H__
