
#ifndef __ELASTOS_DROID_ACCOUNTS_CAUTHENTICATORDESCRIPTIONHELPER_H__
#define __ELASTOS_DROID_ACCOUNTS_CAUTHENTICATORDESCRIPTIONHELPER_H__

#include "_CAuthenticatorDescriptionHelper.h"

namespace Elastos {
namespace Droid {
namespace Accounts {

CarClass(CAuthenticatorDescriptionHelper)
{
public:
    CARAPI NewKey(
        /* [in] */ const String& type,
        /* [out] */ IAuthenticatorDescription** description);
};

} // namespace Accounts
} // namespace Droid
} // namespace Elastos

#endif //__CAUTHENTICATORDESCRIPTION_H__
