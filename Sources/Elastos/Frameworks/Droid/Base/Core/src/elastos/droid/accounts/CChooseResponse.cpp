
#include "accounts/CChooseResponse.h"
#include "accounts/CAccount.h"

namespace Elastos {
namespace Droid {
namespace Accounts {

ECode CChooseResponse::OnResult(
    /* [in] */ IBundle* value)
{
    AutoPtr<IAccount> account;
    String name, type;
    value->GetString(IAccountManager::KEY_ACCOUNT_NAME, &name);
    value->GetString(IAccountManager::KEY_ACCOUNT_TYPE, &type);
    CAccount::New(name, type, (IAccount**)&account);
    mHost->mFuture = NULL;
    return mHost->mHost->GetAuthToken(account, mHost->mAuthTokenType, mHost->mLoginOptions,
            mHost->mActivity, mHost->mMyCallback, mHost->mHandler,
            (IAccountManagerFuture**)&mHost->mFuture);
}

ECode CChooseResponse::OnError(
    /* [in] */ Int32 errorCode,
    /* [in] */ const String& errorMessage)
{
    return mHost->mResponse->OnError(errorCode, errorMessage);
}

ECode CChooseResponse::constructor (
    /* [in] */ Handle32 host)
{
    mHost = (CAccountManager::GetAuthTokenByTypeAndFeaturesTask*)host;
    return NOERROR;
}

} // namespace Accounts
} // namespace Droid
} // namespace Elastos
