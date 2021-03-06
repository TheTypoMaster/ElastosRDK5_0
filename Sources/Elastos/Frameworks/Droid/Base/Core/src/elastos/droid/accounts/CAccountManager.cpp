
#include "accounts/CAccountManager.h"
#include "accounts/AccountManagerResponse.h"
#include "accounts/CChooseResponse.h"
#include "accounts/CAccountManagerAmsResponse.h"
#include "accounts/ChooseTypeAndAccountActivity.h"
#include "accounts/CAccountManagerFutureResponse.h"
#include "accounts/CAccount.h"
#include "os/CBundle.h"
#include "os/Looper.h"
#include "os/Process.h"
#include "os/CUserHandleHelper.h"
#include "os/CBundle.h"
#include "os/CHandler.h"
#include "text/TextUtils.h"
#include "content/CIntent.h"
#include "content/CIntentFilter.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Core::CObjectContainer;
using Elastos::Utility::Logging::Slogger;
using Elastos::Core::CInteger64;
using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::Core::EIID_IRunnable;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::Looper;
using Elastos::Droid::Os::CHandler;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::EIID_IBroadcastReceiver;

namespace Elastos {
namespace Droid {
namespace Accounts {

const String CAccountManager::TAG("AccountManager");


CAccountManager::AmsTask::AmsTask(
    /* [in] */ IActivity* activity,
    /* [in] */ IHandler* handler,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ CAccountManager* host)
    : mHandler(handler)
    , mCallback(cb)
    , mActivity(activity)
    , mHost(host)
{
    // super(new Callable<Bundle>() {
    //     public Bundle call() throws Exception {
    //         throw new IllegalStateException("this should never be called");
    //     }
    // });
    ASSERT_SUCCEEDED(CAccountManagerAmsResponse::New((Handle32)this,
            (IAccountManagerResponse**)&mResponse));
}

PInterface CAccountManager::AmsTask::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IAccountManagerFuture*)this;
    }
    else if (riid == EIID_IAccountManagerFuture) {
        return (IAccountManagerFuture*)this;
    }
    // todo: when amstask extends from futuretask, add the interface of futuretask
    // else if (riid == ) {
    //     return
    // }

    return NULL;
}

UInt32 CAccountManager::AmsTask::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAccountManager::AmsTask::Release()
{
    return ElRefBase::Release();
}

ECode CAccountManager::AmsTask::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IAccountManagerFuture*)this) {
        *pIID = EIID_IAccountManagerFuture;
    }
    // todo: when amstask extends from futuretask, add the interface of futuretask
    // else if (pObject == ) {
    //     *pIID =
    // }
    else {
        return E_INVALID_ARGUMENT;
    }

    return  NOERROR;
}

ECode CAccountManager::AmsTask::Start(
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    //try {
    ECode ec = DoWork();
    if (FAILED(ec)) {
        *accountManagerFuture = NULL;
        // SetException(e);
        return ec;
    }
    // } catch (RemoteException e) {
    //     setException(e);
    // }
    *accountManagerFuture = THIS_PROBE(IAccountManagerFuture);
    REFCOUNT_ADD(*accountManagerFuture);
    return NOERROR;
}

ECode CAccountManager::AmsTask::Set(
    /* [in] */ IBundle* bundle)
{
    //TODO: somehow a null is being set as the result of the Future. Log this
    // case to help debug where this is occurring. When this bug is fixed this
    // condition statement should be removed.
    if (bundle == NULL) {
        Slogger::E(TAG, "the bundle must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    //super.set(bundle);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::AmsTask::InternalGetResult(
    /* [in] */ IInteger64* timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ IBundle** result)
{
    VALIDATE_NOT_NULL(result);

    Boolean isDone;
    IsDone(&isDone);
    if (!isDone) {
        FAIL_RETURN(mHost->EnsureNotOnMainThread());
    }
    // try {
    // ECode ec = NOERROR;
    if (timeout == NULL) {
        assert(0);
        // ec = Get();
    }
    else {
        assert(0);
        // ec = Get(timeout, unit);
    }
    // } catch (CancellationException e) {
    //     throw new OperationCanceledException();
    // } catch (TimeoutException e) {
    //     // fall through and cancel
    // } catch (InterruptedException e) {
    //     // fall through and cancel
    // } catch (ExecutionException e) {
    //     final Throwable cause = e.getCause();
    //     if (cause instanceof IOException) {
    //         throw (IOException) cause;
    //     } else if (cause instanceof UnsupportedOperationException) {
    //         throw new AuthenticatorException(cause);
    //     } else if (cause instanceof AuthenticatorException) {
    //         throw (AuthenticatorException) cause;
    //     } else if (cause instanceof RuntimeException) {
    //         throw (RuntimeException) cause;
    //     } else if (cause instanceof Error) {
    //         throw (Error) cause;
    //     } else {
    //         throw new IllegalStateException(cause);
    //     }
    // } finally {
    Boolean value;
    Cancel(TRUE /* interrupt if running */, &value);
    *result = NULL;
    // }
    return E_OPERATION_CANCELED_EXCEPTION;
}

ECode CAccountManager::AmsTask::GetResult(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    return InternalGetResult(NULL, NULL, (IBundle**)result);
}

ECode CAccountManager::AmsTask::GetResult(
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IInteger64> integer64;
    CInteger64::New(timeout, (IInteger64**)&integer64);
    return InternalGetResult(integer64, unit, (IBundle**)result);
}

void CAccountManager::AmsTask::Done()
{
    if (mCallback != NULL) {
        mHost->PostToHandler(mHandler, mCallback, (IAccountManagerFuture*)this);
    }
}

ECode CAccountManager::AmsTask::Cancel(
    /* [in] */ Boolean mayInterruptIfRunning,
    /* [out] */ Boolean* result)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::AmsTask::IsCancelled(
    /* [out] */ Boolean* result)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::AmsTask::IsDone(
    /* [out] */ Boolean* result)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

CAccountManager::BaseFutureTask::BaseFutureTask(
    /* [in] */ IHandler* handler,
    /* [in] */ CAccountManager* host)
    : mHandler(handler)
    , mHost(host)
{
    // super(new Callable<T>() {
    //     public T call() throws Exception {
    //         throw new IllegalStateException("this should never be called");
    //     }
    // });
    ASSERT_SUCCEEDED(CAccountManagerFutureResponse::New((Handle32)this,
            (IAccountManagerResponse**)&mResponse));
}

void CAccountManager::BaseFutureTask::PostRunnableToHandler(
    /* [in] */ IRunnable* runnable)
{
    AutoPtr<IHandler> handler = (mHandler == NULL) ? mHost->mMainHandler : mHandler;
    Boolean result;
    handler->Post(runnable, &result);
}

ECode CAccountManager::BaseFutureTask::StartTask()
{
    //try {
    return DoWork();
    // } catch (RemoteException e) {
    //     setException(e);
    // }
}

CAccountManager::Future2Task::CallbackRunnable::CallbackRunnable(
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ Future2Task* host)
    : mCallback(cb)
    , mHost(host)
{}

CAR_INTERFACE_IMPL(CAccountManager::Future2Task::CallbackRunnable, IRunnable);

ECode CAccountManager::Future2Task::CallbackRunnable::Run()
{
    return mCallback->Run((IAccountManagerFuture*)mHost);
}

CAccountManager::Future2Task::Future2Task(
    /* [in] */ IHandler* handler,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ CAccountManager* host)
    : BaseFutureTask(handler, host)
    , mCallback(cb)
{}

PInterface CAccountManager::Future2Task::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IAccountManagerFuture*)this;
    }
    else if (riid == EIID_IAccountManagerFuture) {
        return (IAccountManagerFuture*)this;
    }
    // todo: when BaseFutureTask extends from futuretask
    // return BaseFutureTask::Probe(riid);

    return NULL;
}

UInt32 CAccountManager::Future2Task::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAccountManager::Future2Task::Release()
{
    return ElRefBase::Release();
}

ECode CAccountManager::Future2Task::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IAccountManagerFuture*)this) {
        *pIID = EIID_IAccountManagerFuture;
    }
    // todo: when BaseFutureTask extends from futuretask
    // return BaseFutureTask::GetInterfaceID(pObject, pIID);
    else {
        return E_INVALID_ARGUMENT;
    }

    return  NOERROR;
}

void CAccountManager::Future2Task::Done()
{
    if (mCallback != NULL) {
        AutoPtr<IRunnable> action = (IRunnable*)new CallbackRunnable(mCallback, this);
        PostRunnableToHandler(action);
    }
}

ECode CAccountManager::Future2Task::Start(
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    StartTask();
    *accountManagerFuture = THIS_PROBE(IAccountManagerFuture);
    REFCOUNT_ADD(*accountManagerFuture);
    return NOERROR;
}

ECode CAccountManager::Future2Task::InternalGetResult(
    /* [in] */ IInteger64* timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ IInterface** result)
{
    Boolean isDone;
    IsDone(&isDone);
    if (!isDone) {
        FAIL_RETURN(mHost->EnsureNotOnMainThread());
    }
    //try {
    // if (timeout == null) {
    //     return get();
    // } else {
    //     return get(timeout, unit);
    // }
    // } catch (InterruptedException e) {
    //     // fall through and cancel
    // } catch (TimeoutException e) {
    //     // fall through and cancel
    // } catch (CancellationException e) {
    //     // fall through and cancel
    // } catch (ExecutionException e) {
    //     final Throwable cause = e.getCause();
    //     if (cause instanceof IOException) {
    //         throw (IOException) cause;
    //     } else if (cause instanceof UnsupportedOperationException) {
    //         throw new AuthenticatorException(cause);
    //     } else if (cause instanceof AuthenticatorException) {
    //         throw (AuthenticatorException) cause;
    //     } else if (cause instanceof RuntimeException) {
    //         throw (RuntimeException) cause;
    //     } else if (cause instanceof Error) {
    //         throw (Error) cause;
    //     } else {
    //         throw new IllegalStateException(cause);
    //     }
    // } finally {
    //     cancel(true /* interrupt if running */);
    // }
    //throw new OperationCanceledException();
    assert(0);

    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::Future2Task::GetResult(
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    return InternalGetResult(NULL, NULL, result);
}

ECode CAccountManager::Future2Task::GetResult(
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IInteger64> integer64;
    CInteger64::New(timeout, (IInteger64**)&integer64);
    return InternalGetResult(integer64, NULL, result);
}

ECode CAccountManager::Future2Task::Cancel(
    /* [in] */ Boolean mayInterruptIfRunning,
    /* [out] */ Boolean* result)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::Future2Task::IsCancelled(
    /* [out] */ Boolean* result)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode CAccountManager::Future2Task::IsDone(
    /* [out] */ Boolean* result)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

CAccountManager::GetAuthTokenByTypeAndFeaturesTask::GetAccountsCallback::GetAccountsCallback(
    /* [in] */ GetAuthTokenByTypeAndFeaturesTask* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(CAccountManager::GetAuthTokenByTypeAndFeaturesTask::GetAccountsCallback,
        IAccountManagerCallback);

ECode CAccountManager::GetAuthTokenByTypeAndFeaturesTask::GetAccountsCallback::Run(
    /* [in] */ IAccountManagerFuture* future)
{
    AutoPtr<IObjectContainer> accounts;//Account[] accounts;
    //try {
    FAIL_RETURN(future->GetResult((IInterface**)&accounts));
    // } catch (OperationCanceledException e) {
    //     setException(e);
    //     return;
    // } catch (IOException e) {
    //     setException(e);
    //     return;
    // } catch (AuthenticatorException e) {
    //     setException(e);
    //     return;
    // }

    AutoPtr<IObjectEnumerator> enumerator;
    accounts->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
    Boolean hasNext = FALSE;
    enumerator->MoveNext(&hasNext);
    if (!hasNext) {
        mHost->mNumAccounts = 0;
        if (mHost->mActivity != NULL) {
            // no accounts, add one now. pretend that the user directly
            // made this request
            mHost->mFuture = NULL;
            FAIL_RETURN(mHost->mHost->AddAccount(mHost->mAccountType, mHost->mAuthTokenType,
                    *mHost->mFeatures, mHost->mAddAccountOptions,
                    mHost->mActivity, mHost->mMyCallback, mHost->mHandler,
                    (IAccountManagerFuture**)&mHost->mFuture));
        }
        else {
            // send result since we can't prompt to add an account
            AutoPtr<IBundle> result;
            CBundle::New((IBundle**)&result);
            result->PutString(IAccountManager::KEY_ACCOUNT_NAME, String(NULL));
            result->PutString(IAccountManager::KEY_ACCOUNT_TYPE, String(NULL));
            result->PutString(IAccountManager::KEY_AUTHTOKEN, String(NULL));
            //try {
            mHost->mResponse->OnResult(result);
            // } catch (RemoteException e) {
            //     // this will never happen
            // }
            // we are done
        }
    }
    else {
        AutoPtr<IAccount> account;
        enumerator->Current((IInterface**)&account);
        enumerator->MoveNext(&hasNext);
        if (!hasNext) {
            mHost->mNumAccounts = 1;
            // have a single account, return an authtoken for it
            mHost->mFuture = NULL;
            if (mHost->mActivity == NULL) {
                mHost->mHost->GetAuthToken(account, mHost->mAuthTokenType,
                        FALSE /* notifyAuthFailure */, mHost->mMyCallback,
                        mHost->mHandler, (IAccountManagerFuture**)&(mHost->mFuture));
            }
            else {
                mHost->mHost->GetAuthToken(account, mHost->mAuthTokenType,
                        mHost->mLoginOptions, mHost->mActivity, mHost->mMyCallback,
                        mHost->mHandler, (IAccountManagerFuture**)&(mHost->mFuture));
            }
        }
        else {
            accounts->GetObjectCount(&mHost->mNumAccounts);
            if (mHost->mActivity != NULL) {
                AutoPtr<IAccountManagerResponse> chooseResponse;
                FAIL_RETURN(CChooseResponse::New((Handle32)this,
                        (IAccountManagerResponse**)&chooseResponse));
                // have many accounts, launch the chooser
                AutoPtr<IIntent> intent;
                FAIL_RETURN(CIntent::New((IIntent**)&intent));
                intent->SetClassName(String("elastos"),
                        String("elastos.accounts.ChooseAccountActivity"));
                intent->PutExtra(IAccountManager::KEY_ACCOUNTS, (Int32)accounts.Get());
                AutoPtr<AccountManagerResponse> response = new AccountManagerResponse(
                        chooseResponse);
                intent->PutExtra(IAccountManager::KEY_ACCOUNT_MANAGER_RESPONSE,
                        (IParcelable*)response->Probe(EIID_IParcelable));
                mHost->mActivity->StartActivity(intent);
                // the result will arrive via the IAccountManagerResponse
            }
            else {
                // send result since we can't prompt to select an account
                AutoPtr<IBundle> result;
                CBundle::New((IBundle**)&result);
                result->PutString(IAccountManager::KEY_ACCOUNTS, String(NULL));
                //try {
                mHost->mResponse->OnResult(result);
                // } catch (RemoteException e) {
                //     // this will never happen
                // }
                // we are done
            }
        }
    }

    return NOERROR;
}

CAccountManager::GetAuthTokenByTypeAndFeaturesTask::GetAuthTokenByTypeAndFeaturesTask(
    /* [in] */ const String& accountType,
    /* [in] */ const String& authTokenType,
    /* [in] */ const ArrayOf<String>& features,
    /* [in] */ IActivity* activityForPrompting,
    /* [in] */ IBundle* addAccountOptions,
    /* [in] */ IBundle* loginOptions,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ IHandler* handler,
    /* [in] */ CAccountManager* host)
    : AmsTask(activityForPrompting, handler, cb, host)
    , mAccountType(accountType)
    , mAuthTokenType(authTokenType)
    , mAddAccountOptions(addAccountOptions)
    , mNumAccounts(0)
{
    mFeatures = features.Clone();
    assert(!accountType.IsNull());
    mMyCallback = (IAccountManagerCallback*)this;
}

PInterface CAccountManager::GetAuthTokenByTypeAndFeaturesTask::Probe(
    /* [in]  */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IAccountManagerCallback*)this;
    }
    return AmsTask::Probe(riid);
}

UInt32 CAccountManager::GetAuthTokenByTypeAndFeaturesTask::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CAccountManager::GetAuthTokenByTypeAndFeaturesTask::Release()
{
    return ElRefBase::Release();
}

ECode CAccountManager::GetAuthTokenByTypeAndFeaturesTask::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID);

    if (pObject == (IInterface*)(IAccountManagerCallback*)this) {
        *pIID = EIID_IAccountManagerCallback;
    }
    return AmsTask::GetInterfaceID(pObject, pIID);
}

ECode CAccountManager::GetAuthTokenByTypeAndFeaturesTask::DoWork()
{
    AutoPtr<IAccountManagerFuture> future;
    AutoPtr<IAccountManagerCallback> callback = new GetAccountsCallback(this);
    return mHost->GetAccountsByTypeAndFeatures(mAccountType, *mFeatures,
            callback, mHandler,
            (IAccountManagerFuture**)&future);
}

ECode CAccountManager::GetAuthTokenByTypeAndFeaturesTask::Run(
    /* [in] */ IAccountManagerFuture* future)
{
    //try {
    AutoPtr<IBundle> result;
    FAIL_RETURN(future->GetResult((IInterface**)&result));
    if (mNumAccounts == 0) {
        String accountName, accountType;
        result->GetString(IAccountManager::KEY_ACCOUNT_NAME, &accountName);
        result->GetString(IAccountManager::KEY_ACCOUNT_TYPE, &accountType);
        AutoPtr<ICharSequence> cs1, cs2;
        CString::New(accountName, (ICharSequence**)&cs1);
        CString::New(accountType, (ICharSequence**)&cs2);
        if (TextUtils::IsEmpty(cs1) || TextUtils::IsEmpty(cs2)) {
            Slogger::E(TAG, "account not in result");
            return E_AUTHENTICATOR_EXCEPTION;
//            setException(new AuthenticatorException("account not in result"));
        }
        AutoPtr<IAccount> account;
        CAccount::New(accountName, accountType, (IAccount**)&account);
        mNumAccounts = 1;
        AutoPtr<IAccountManagerFuture> f;
        return mHost->GetAuthToken(account, mAuthTokenType, NULL /* options */, mActivity,
                mMyCallback, mHandler, (IAccountManagerFuture**)&f);
    }
    ECode ec = Set(result);
    if (ec == (ECode)E_OPERATION_CANCELED_EXCEPTION) {
        Boolean result;
        Cancel(TRUE /* mayInterruptIfRUnning */, &result);
    }

    return ec;
    // } catch (OperationCanceledException e) {
    //     cancel(true /* mayInterruptIfRUnning */);
    // } catch (IOException e) {
    //     setException(e);
    // } catch (AuthenticatorException e) {
    //     setException(e);
    // }
}

CAccountManager::AccountsChangedBroadcastReceiver::AccountsChangedBroadcastReceiver(
    /* [in] */ CAccountManager* host)
    : mHost(host)
{}

ECode CAccountManager::AccountsChangedBroadcastReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    AutoPtr< ArrayOf<IAccount*> > accounts;
    FAIL_RETURN(mHost->GetAccounts((ArrayOf<IAccount*>**)&accounts));
    // send the result to the listeners
    AutoLock lock(mHost->mAccountsUpdatedListenersLock);
    HashMap<AutoPtr<IOnAccountsUpdateListener>, AutoPtr<IHandler> >::Iterator it
            = mHost->mAccountsUpdatedListeners.Begin();
    for (; it != mHost->mAccountsUpdatedListeners.End(); ++it) {
        mHost->PostToHandler(it->mSecond, it->mFirst, *accounts);
    }

    return NOERROR;
}


CAccountManager::CAccountManager()
{
    mAccountsChangedBroadcastReceiver = new AccountsChangedBroadcastReceiver(this);
}

ECode CAccountManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IIAccountManager* service)
{
    mContext = context;
    mService = service;
    AutoPtr<ILooper> looper;
    ASSERT_SUCCEEDED(mContext->GetMainLooper((ILooper**)&looper));
    return CHandler::New(looper, (IHandler**)&mMainHandler);
}

ECode CAccountManager::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IIAccountManager* service,
    /* [in] */ IHandler* handler)
{
    mContext = context;
    mService = service;
    mMainHandler = handler;
    return NOERROR;
}

AutoPtr<IBundle> CAccountManager::SanitizeResult(
    /* [in] */ IBundle* result)
{
    if (result != NULL) {
        Boolean isContains;
        result->ContainsKey(KEY_AUTHTOKEN, &isContains);
        if (isContains) {
            String value;
            result->GetString(KEY_AUTHTOKEN, &value);
            AutoPtr<ICharSequence> cs;
            CString::New(value, (ICharSequence**)&cs);
            if (!TextUtils::IsEmpty(cs)) {
                AutoPtr<CBundle> newResult;
                CBundle::NewByFriend((CBundle**)&newResult);
                newResult->PutString(KEY_AUTHTOKEN,
                        String("<omitted for logging purposes>"));
                return newResult;
            }
        }
    }
    return result;
}

ECode CAccountManager::Get(
    /* [in] */ IContext* context,
    /* [out] */ IAccountManager** accountManager)
{
    VALIDATE_NOT_NULL(accountManager);
    if (context == NULL) {
        Slogger::E(TAG, "context is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        //throw new IllegalArgumentException("context is null");
    }
    return context->GetSystemService(IContext::ACCOUNT_SERVICE,
            (IInterface**)accountManager);
}

ECode CAccountManager::GetPassword(
    /* [in] */ IAccount* account,
    /* [out] */ String* password)
{
    VALIDATE_NOT_NULL(password);
    if (account == NULL) {
        Slogger::E(TAG, "account is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    //try {
    return mService->GetPassword(account, password);
    // } catch (RemoteException e) {
    //     // will never happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::GetUserData(
    /* [in] */ IAccount* account,
    /* [in] */ const String& key,
    /* [out] */ String* userData)
{
    VALIDATE_NOT_NULL(userData);
    if (account == NULL) {
        Slogger::E(TAG, "account is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    if (key.IsNull()) {
        Slogger::E(TAG, "key is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("key is null");
    }
    //try {
    return mService->GetUserData(account, key, userData);
    // } catch (RemoteException e) {
    //     // will never happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::GetAuthenticatorTypes(
    /* [out, callee] */ ArrayOf<IAuthenticatorDescription*>** authenticators)
{
    VALIDATE_NOT_NULL(authenticators);
    //try {
    return mService->GetAuthenticatorTypes(authenticators);
    // } catch (RemoteException e) {
    //     // will never happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::GetAccounts(
    /* [out, callee] */ ArrayOf<IAccount*>** accounts)
{
    VALIDATE_NOT_NULL(accounts);
    //try {
    return mService->GetAccounts(String(NULL), accounts);
    // } catch (RemoteException e) {
    //     // won't ever happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::GetAccountsByType(
    /* [in] */ const String& type,
    /* [out, callee] */ ArrayOf<IAccount*>** accounts)
{
    VALIDATE_NOT_NULL(accounts);
    AutoPtr<IUserHandle> userH;
    Process::MyUserHandle((IUserHandle**)&userH);
    return GetAccountsByTypeAsUser(type, userH, accounts);
}

ECode CAccountManager::GetAccountsByTypeAsUser(
    /* [in] */ const String& type,
    /* [in] */ IUserHandle* userHandle,
    /* [out, callee] */ ArrayOf<IAccount*>** accounts)

{
    VALIDATE_NOT_NULL(accounts);
    // try {
    Int32 id;
    userHandle->GetIdentifier(&id);
    return mService->GetAccountsAsUser(type, id, accounts);
    // } catch (RemoteException e) {
    //     // won't ever happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::UpdateAppPermission(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ Int32 uid,
    /* [in] */ Boolean value)
{
    // try {
    return mService->UpdateAppPermission(account, authTokenType, uid, value);
    // } catch (RemoteException e) {
    //     // won't ever happen
    //     throw new RuntimeException(e);
    // }
}

CAccountManager::Future2Task_GetAuthTokenLabel::Future2Task_GetAuthTokenLabel(
    /* [in] */ IHandler* handler,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ CAccountManager* host,
    /* [in] */ const String& accountType,
    /* [in] */ const String& authTokenType)
    : Future2Task(handler, cb, host)
    , mAccountType(accountType)
    , mAuthTokenType(authTokenType)
{}

ECode CAccountManager::Future2Task_GetAuthTokenLabel::DoWork()
{
    return mHost->mService->GetAuthTokenLabel(mResponse, mAccountType, mAuthTokenType);
}

ECode CAccountManager::Future2Task_GetAuthTokenLabel::BundleToResult(
    /* [in] */ IBundle* bundle,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);

    Boolean isContains;
    bundle->ContainsKey(IAccountManager::KEY_AUTH_TOKEN_LABEL, &isContains);
    if (!isContains) {
        Slogger::E(TAG, "no result in response");
        return E_AUTHENTICATOR_EXCEPTION;
        // throw new AuthenticatorException("no result in response");
    }
    String s;
    bundle->GetString(IAccountManager::KEY_AUTH_TOKEN_LABEL, &s);
    AutoPtr<ICharSequence> cs;
    CString::New(s, (ICharSequence**)&cs);
    *result = (IInterface*)cs;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CAccountManager::GetAuthTokenLabel(
    /* [in] */ const String& accountType,
    /* [in] */ const String& authTokenType,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ IHandler* handler,
    /* [out] */ IAccountManagerFuture** future)
{
    VALIDATE_NOT_NULL(future);
    *future = NULL;

    if (accountType.IsNull()) {
        Slogger::E(TAG, "accountType is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalArgumentException("accountType is null");
    }
    if (authTokenType.IsNull()) {
        Slogger::E(TAG, "authTokenType is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalArgumentException("authTokenType is null");
    }
    AutoPtr<Future2Task_GetAuthTokenLabel> f = new Future2Task_GetAuthTokenLabel(
            handler, cb, this, accountType, authTokenType);
    return f->Start(future);
}

CAccountManager::Future2Task_HasFeatures::Future2Task_HasFeatures(
    /* [in] */ IHandler* handler,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ CAccountManager* host,
    /* [in] */ IAccount* account,
    /* [in] */ const ArrayOf<String>& features)
    : Future2Task(handler, cb, host)
    , mAccount(account)
{
    mFeatures = features.Clone();
}

ECode CAccountManager::Future2Task_HasFeatures::DoWork()
{
    return mHost->mService->HasFeatures(mResponse, mAccount, *mFeatures);
}

ECode CAccountManager::Future2Task_HasFeatures::BundleToResult(
    /* [in] */ IBundle* bundle,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    Boolean isContains;
    bundle->ContainsKey(IAccountManager::KEY_BOOLEAN_RESULT, &isContains);
    if (!isContains) {
        Slogger::E(TAG, "no result in response");
        return E_AUTHENTICATOR_EXCEPTION;
//        throw new AuthenticatorException("no result in response");
    }
    Boolean res;
    bundle->GetBoolean(IAccountManager::KEY_BOOLEAN_RESULT, &res);
    AutoPtr<IBoolean> b;
    CBoolean::New(res, (IBoolean**)&b);
    *result = (IInterface*)b;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CAccountManager::HasFeatures(
    /* [in] */ IAccount* account,
    /* [in] */ const ArrayOf<String>& features,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ IHandler* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    if (account == NULL) {
        Slogger::E(TAG, "account is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    //if (features == null) throw new IllegalArgumentException("features is null");
    AutoPtr<Future2Task_HasFeatures> future = new Future2Task_HasFeatures(
            handler, cb, this, account, features);
    return future->Start(accountManagerFuture);
}

CAccountManager::Future2Task_GetAccountsByTypeAndFeatures::Future2Task_GetAccountsByTypeAndFeatures(
    /* [in] */ IHandler* handler,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ CAccountManager* host,
    /* [in] */ const String& type,
    /* [in] */ const ArrayOf<String>& features)
    : Future2Task(handler, cb, host)
    , mType(type)
{
    mFeatures = features.Clone();
}

ECode CAccountManager::Future2Task_GetAccountsByTypeAndFeatures::DoWork()
{
    return mHost->mService->GetAccountsByFeatures(mResponse, mType, mFeatures);
}

ECode CAccountManager::Future2Task_GetAccountsByTypeAndFeatures::BundleToResult(
    /* [in] */ IBundle* bundle,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);

    Boolean isContains;
    bundle->ContainsKey(String(IAccountManager::KEY_BOOLEAN_RESULT), &isContains);
    if (!isContains) {
        Slogger::E(TAG, "no result in response");
        return E_AUTHENTICATOR_EXCEPTION;
//        throw new AuthenticatorException("no result in response");
    }
    AutoPtr< ArrayOf<IParcelable*> > parcelables;
    bundle->GetParcelableArray(IAccountManager::KEY_ACCOUNTS,
            (ArrayOf<IParcelable*>**)&parcelables);
    AutoPtr<IObjectContainer> descs;
    CObjectContainer::New((IObjectContainer**)&descs);
    for (Int32 i = 0; i < parcelables->GetLength(); i++) {
        AutoPtr<IAccount> account;
        if ((*parcelables)[i] != NULL &&
                (*parcelables)[i]->Probe(EIID_IAccount) != NULL){
            account = (IAccount*)(*parcelables)[i]->Probe(EIID_IAccount);
        }
        descs->Add((IInterface*)account);
    }
    *result = (IInterface*)descs;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CAccountManager::GetAccountsByTypeAndFeatures(
    /* [in] */ const String& type,
    /* [in] */ const ArrayOf<String>& features,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ IHandler* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    if (type.IsNull()) {
        Slogger::E(TAG, "type is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("type is null");
    }
    AutoPtr<Future2Task_GetAccountsByTypeAndFeatures> future = new Future2Task_GetAccountsByTypeAndFeatures(
            handler, cb, this, type, features);
    return future->Start(accountManagerFuture);
}

ECode CAccountManager::AddAccountExplicitly(
    /* [in] */ IAccount* account,
    /* [in] */ const String& password,
    /* [in] */ IBundle* userdata,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (account == NULL) {
        Slogger::E(TAG, "account is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    //try {
    return mService->AddAccount(account, password, userdata, result);
    // } catch (RemoteException e) {
    //     // won't ever happen
    //     throw new RuntimeException(e);
    // }
}

CAccountManager::Future2Task_RemoveAccount::Future2Task_RemoveAccount(
    /* [in] */ IHandler* handler,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ CAccountManager* host,
    /* [in] */ IAccount* account)
    : Future2Task(handler, cb, host)
    , mAccount(account)
{}

ECode CAccountManager::Future2Task_RemoveAccount::DoWork()
{
    return mHost->mService->RemoveAccount(mResponse, mAccount);
}

ECode CAccountManager::Future2Task_RemoveAccount::BundleToResult(
    /* [in] */ IBundle* bundle,
    /* [out] */ IInterface** result)
{
    VALIDATE_NOT_NULL(result);

    Boolean isContains;
    bundle->ContainsKey(IAccountManager::KEY_BOOLEAN_RESULT, &isContains);
    if (!isContains) {
        Slogger::E(TAG, "no result in response");
        return E_AUTHENTICATOR_EXCEPTION;
//        throw new AuthenticatorException("no result in response");
    }
    Boolean res;
    bundle->GetBoolean(IAccountManager::KEY_BOOLEAN_RESULT, &res);
    AutoPtr<IBoolean> b;
    CBoolean::New(res, (IBoolean**)&b);
    *result = (IInterface*)b;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CAccountManager::RemoveAccount(
    /* [in] */ IAccount* account,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ IHandler* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    if (account == NULL) {
        Slogger::E(TAG, "account is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    AutoPtr<Future2Task_RemoveAccount> future = new Future2Task_RemoveAccount(
            handler, cb, this, account);
    return future->Start(accountManagerFuture);
}

ECode CAccountManager::InvalidateAuthToken(
    /* [in] */ const String& accountType,
    /* [in] */ const String& authToken)
{
    if (accountType.IsNull()) {
        Slogger::E(TAG, "accountType is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("accountType is null");
    }
    //try {
    if (!authToken.IsNull()) {
        return mService->InvalidateAuthToken(accountType, authToken);
    }

    return NOERROR;
    // } catch (RemoteException e) {
    //     // won't ever happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::PeekAuthToken(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [out] */ String* token)
{
    VALIDATE_NOT_NULL(token);
    if (account == NULL) {
        Slogger::E(TAG, "account is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    if (authTokenType.IsNull()) {
        Slogger::E(TAG, "authTokenType is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("authTokenType is null");
    }
    //try {
    return mService->PeekAuthToken(account, authTokenType, token);
    // } catch (RemoteException e) {
    //     // won't ever happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::SetPassword(
    /* [in] */ IAccount* account,
    /* [in] */ const String& password)
{
    if (account == NULL) {
        Slogger::E(TAG, "account is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    //try {
    return mService->SetPassword(account, password);
    // } catch (RemoteException e) {
    //     // won't ever happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::ClearPassword(
    /* [in] */ IAccount* account)
{
    if (account == NULL) {
        Slogger::E(TAG, "account is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    //try {
    return mService->ClearPassword(account);
    // } catch (RemoteException e) {
    //     // won't ever happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::SetUserData(
    /* [in] */ IAccount* account,
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    if (account == NULL) {
        Slogger::E(TAG, "account is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    if (key.IsNull()) {
        Slogger::E(TAG, "key is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("key is null");
    }
    //try {
    return mService->SetUserData(account, key, value);
    // } catch (RemoteException e) {
    //     // won't ever happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::SetAuthToken(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ const String& authToken)
{
    if (account == NULL) {
        Slogger::E(TAG, "account is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    if (authTokenType.IsNull()) {
        Slogger::E(TAG, "authTokenType is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("authTokenType is null");
    }
    //try {
    return mService->SetAuthToken(account, authTokenType, authToken);
    // } catch (RemoteException e) {
    //     // won't ever happen
    //     throw new RuntimeException(e);
    // }
}

ECode CAccountManager::BlockingGetAuthToken(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ Boolean notifyAuthFailure,
    /* [out] */ String* token)
{
    VALIDATE_NOT_NULL(token);
    if (account == NULL) {
        Slogger::E(TAG, "account is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    if (authTokenType.IsNull()) {
        Slogger::E(TAG, "authTokenType is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("authTokenType is null");
    }

    AutoPtr<IAccountManagerFuture> accountManagerFuture;
    GetAuthToken(account, authTokenType, notifyAuthFailure, NULL /* callback */,
            NULL /* handler */, (IAccountManagerFuture**)&accountManagerFuture);
    AutoPtr<IBundle> bundle;
    accountManagerFuture->GetResult((IInterface**)&bundle);
    if (bundle == NULL) {
        // This should never happen, but it does, occasionally. If it does return null to
        // signify that we were not able to get the authtoken.
        // TODO: remove this when the bug is found that sometimes causes a null bundle to be
        // returned
        Slogger::E(TAG, "blockingGetAuthToken: null was returned from getResult() for %p, authTokenType %s"
                , account, (const char*)authTokenType);
        *token = String(NULL);
        return NOERROR;
    }
    return bundle->GetString(KEY_AUTHTOKEN, token);
}

CAccountManager::AmsTask_GetAuthToken::AmsTask_GetAuthToken(
    /* [in] */ IActivity* activity,
    /* [in] */ IHandler* handler,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ CAccountManager* host,
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ Boolean notifyAuthFailure,
    /* [in] */ Boolean expectActivityLaunch,
    /* [in] */ IBundle* options)
    : AmsTask(activity, handler, cb, host)
    , mAccount(account)
    , mAuthTokenType(authTokenType)
    , mNotifyAuthFailure(notifyAuthFailure)
    , mExpectActivityLaunch(expectActivityLaunch)
    , mOptions(options)
{}

ECode CAccountManager::AmsTask_GetAuthToken::DoWork()
{
    return mHost->mService->GetAuthToken(mResponse, mAccount, mAuthTokenType,
            mNotifyAuthFailure, mExpectActivityLaunch, mOptions);
}

ECode CAccountManager::GetAuthToken(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ IBundle* options,
    /* [in] */ IActivity* activity,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ IHandler* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    *accountManagerFuture = NULL;

    if (account == NULL) {
        Slogger::E(TAG, "account is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    if (authTokenType.IsNull()) {
        Slogger::E(TAG, "authTokenType is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("authTokenType is null");
    }
    AutoPtr<IBundle> optionsIn;
    FAIL_RETURN(CBundle::New((IBundle**)&optionsIn));
    if (options != NULL) {
        optionsIn->PutAll(options);
    }
    String packageName;
    mContext->GetPackageName(&packageName);
    optionsIn->PutString(KEY_ELASTOS_PACKAGE_NAME, packageName);
    AutoPtr<AmsTask_GetAuthToken> task = new AmsTask_GetAuthToken(
            activity, handler, cb, this, account, authTokenType,
            FALSE /* notifyOnAuthFailure */, TRUE /* expectActivityLaunch */, optionsIn);
    return task->Start(accountManagerFuture);
}

ECode CAccountManager::GetAuthToken(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ Boolean notifyAuthFailure,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ IHandler* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    return GetAuthToken(account, authTokenType, NULL, notifyAuthFailure, cb,
                handler, accountManagerFuture);
}

ECode CAccountManager::GetAuthToken(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ IBundle* options,
    /* [in] */ Boolean notifyAuthFailure,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ IHandler* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    if (account == NULL) {
        Slogger::E(TAG, "account is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalArgumentException("account is null");
    }
    if (authTokenType.IsNull()) {
        Slogger::E(TAG, "authTokenType is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalArgumentException("authTokenType is null");
    }
    AutoPtr<IBundle> optionsIn;
    FAIL_RETURN(CBundle::New((IBundle**)&optionsIn));
    if (options != NULL) {
        optionsIn->PutAll(options);
    }
    String packageName;
    mContext->GetPackageName(&packageName);
    optionsIn->PutString(KEY_ELASTOS_PACKAGE_NAME, packageName);
    AutoPtr<AmsTask_GetAuthToken> task = new AmsTask_GetAuthToken(
            NULL, handler, cb, this, account, authTokenType,
            notifyAuthFailure, FALSE /* expectActivityLaunch */, optionsIn);
    return task->Start(accountManagerFuture);
}

CAccountManager::AmsTask_AddAccount::AmsTask_AddAccount(
    /* [in] */ IActivity* activity,
    /* [in] */ IHandler* handler,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ CAccountManager* host,
    /* [in] */ const String& accountType,
    /* [in] */ const String& authTokenType,
    /* [in] */ const ArrayOf<String>& requiredFeatures,
    /* [in] */ IBundle* options)
    : AmsTask(activity, handler, cb, host)
    , mAccountType(accountType)
    , mAuthTokenType(authTokenType)
    , mOptions(options)
{
    mRequiredFeatures = requiredFeatures.Clone();
}

ECode CAccountManager::AmsTask_AddAccount::DoWork()
{
    return mHost->mService->AddAcount(mResponse, mAccountType, mAuthTokenType,
            *mRequiredFeatures, mActivity != NULL, mOptions);
}

ECode CAccountManager::AddAccount(
    /* [in] */ const String& accountType,
    /* [in] */ const String& authTokenType,
    /* [in] */ const ArrayOf<String>& requiredFeatures,
    /* [in] */ IBundle* addAccountOptions,
    /* [in] */ IActivity* activity,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ IHandler* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    if (accountType.IsNull()) {
        Slogger::E(TAG, "accountType is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("accountType is null");
    }
    AutoPtr<IBundle> optionsIn;
    FAIL_RETURN(CBundle::New((IBundle**)&optionsIn));
    if (addAccountOptions != NULL) {
        optionsIn->PutAll(addAccountOptions);
    }
    String packageName;
    mContext->GetPackageName(&packageName);
    optionsIn->PutString(KEY_ELASTOS_PACKAGE_NAME, packageName);
    AutoPtr<AmsTask_AddAccount> task = new AmsTask_AddAccount(
            activity, handler, cb, this, accountType, authTokenType,
            requiredFeatures, optionsIn);
    return task->Start(accountManagerFuture);
}

ECode CAccountManager::ConfirmCredentials(
    /* [in] */ IAccount* account,
    /* [in] */ IBundle* options,
    /* [in] */ IActivity* activity,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ IHandler* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    AutoPtr<IUserHandle> userHandle;
    Process::MyUserHandle((IUserHandle**)&userHandle);
    return ConfirmCredentialsAsUser(account, options, activity, cb, handler,
            userHandle, accountManagerFuture);
}

CAccountManager::AmsTask_ConfirmCredentialsAsUser::AmsTask_ConfirmCredentialsAsUser(
    /* [in] */ IActivity* activity,
    /* [in] */ IHandler* handler,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ CAccountManager* host,
    /* [in] */ IAccount* account,
    /* [in] */ IBundle* options,
    /* [in] */ Int32 userId)
    : AmsTask(activity, handler, cb, host)
    , mAccount(account)
    , mOptions(options)
    , mUserId(userId)
{}

ECode CAccountManager::AmsTask_ConfirmCredentialsAsUser::DoWork()
{
    return mHost->mService->ConfirmCredentialsAsUser(mResponse, mAccount,
            mOptions, mActivity != NULL, mUserId);
}

ECode CAccountManager::ConfirmCredentialsAsUser(
    /* [in] */ IAccount* account,
    /* [in] */ IBundle* options,
    /* [in] */ IActivity* activity,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ IHandler* handler,
    /* [in] */ IUserHandle* userHandle,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    if (account == NULL) {
        Slogger::E(TAG, "account is null");
        return E_AUTHENTICATOR_EXCEPTION;
        // throw new IllegalArgumentException("account is null");
    }
    Int32 userId;
    userHandle->GetIdentifier(&userId);
    AutoPtr<AmsTask_ConfirmCredentialsAsUser> task = new AmsTask_ConfirmCredentialsAsUser(
        activity, handler, cb, this, account, options, userId);
    return task->Start(accountManagerFuture);
}

CAccountManager::AmsTask_UpdateCredentials::AmsTask_UpdateCredentials(
    /* [in] */ IActivity* activity,
    /* [in] */ IHandler* handler,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ CAccountManager* host,
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ IBundle* options)
    : AmsTask(activity, handler, cb, host)
    , mAccount(account)
    , mAuthTokenType(authTokenType)
    , mOptions(options)
{}

ECode CAccountManager::AmsTask_UpdateCredentials::DoWork()
{
    return mHost->mService->UpdateCredentials(mResponse, mAccount,
            mAuthTokenType, mActivity != NULL, mOptions);
}

ECode CAccountManager::UpdateCredentials(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authTokenType,
    /* [in] */ IBundle* options,
    /* [in] */ IActivity* activity,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ IHandler* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    if (account == NULL) {
        Slogger::E(TAG, "account is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account is null");
    }
    AutoPtr<AmsTask_UpdateCredentials> task = new AmsTask_UpdateCredentials(
            activity, handler, cb, this, account, authTokenType, options);
    return task->Start(accountManagerFuture);
}

CAccountManager::AmsTask_EditProperties::AmsTask_EditProperties(
    /* [in] */ IActivity* activity,
    /* [in] */ IHandler* handler,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ CAccountManager* host,
    /* [in] */ const String& authTokenType)
    : AmsTask(activity, handler, cb, host)
    , mAuthTokenType(authTokenType)
{}

ECode CAccountManager::AmsTask_EditProperties::DoWork()
{
    return mHost->mService->EditProperties(mResponse,
            mAuthTokenType, mActivity != NULL);
}

ECode CAccountManager::EditProperties(
    /* [in] */ const String& accountType,
    /* [in] */ IActivity* activity,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ IHandler* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    if (accountType.IsNull()) {
        Slogger::E(TAG, "accountType is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("accountType is null");
    }
    AutoPtr<AmsTask_EditProperties> task = new AmsTask_EditProperties(
            activity, handler, cb, this, accountType);
    return task->Start(accountManagerFuture);
}

ECode CAccountManager::EnsureNotOnMainThread()
{
    AutoPtr<ILooper> looper = Looper::GetMyLooper();
    AutoPtr<ILooper> mainLooper;
    if (looper != NULL &&
            (mContext->GetMainLooper((ILooper**)&mainLooper), looper == mainLooper)) {
        // final IllegalStateException exception = new IllegalStateException(
        //         "calling this from your main thread can lead to deadlock");
        Slogger::E(TAG, "calling this from your main thread can lead to deadlock and/or ANRs");
        AutoPtr<IApplicationInfo> info;
        ASSERT_SUCCEEDED(mContext->GetApplicationInfo((IApplicationInfo**)&info));
        Int32 version;
        info->GetTargetSdkVersion(&version);
        if (version >= 8/*Build.VERSION_CODES.FROYO*/) {
            return E_ILLEGAL_STATE_EXCEPTION;
        }
    }
    return NOERROR;
}

CAccountManager::CallbackAction::CallbackAction(
    /* [in] */ IAccountManagerCallback* accountManagerCb,
    /* [in] */ IAccountManagerFuture* future)
    : mCallback(accountManagerCb)
    , mFuture(future)
{}

CAR_INTERFACE_IMPL(CAccountManager::CallbackAction, IRunnable);

ECode CAccountManager::CallbackAction::Run()
{
    return mCallback->Run(mFuture);
}

void CAccountManager::PostToHandler(
    /* [in] */ IHandler* handler,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ IAccountManagerFuture* future)
{
    AutoPtr<IRunnable> action = new CallbackAction(cb, future);
    AutoPtr<IHandler> h = handler == NULL ? mMainHandler.Get() : handler;
    Boolean result;
    h->Post(action, &result);
}

CAccountManager::AccountUpdateAction::AccountUpdateAction(
    /* [in] */ IOnAccountsUpdateListener* listener,
    /* [in] */ ArrayOf<IAccount*>* accounts)
    : mListener(listener)
    , mAccounts(accounts)
{}

CAR_INTERFACE_IMPL(CAccountManager::AccountUpdateAction, IRunnable);

ECode CAccountManager::AccountUpdateAction::Run()
{
    //try {
    return mListener->OnAccountsUpdated(*mAccounts);
    // } catch (SQLException e) {
    //     // Better luck next time.  If the problem was disk-full,
    //     // the STORAGE_OK intent will re-trigger the update.
    //     Log.e(TAG, "Can't update accounts", e);
    // }
}

void CAccountManager::PostToHandler(
    /* [in] */ IHandler* handler,
    /* [in] */ IOnAccountsUpdateListener* listener,
    /* [in] */ const ArrayOf<IAccount*>& accounts)
{
    // send a copy to make sure that one doesn't
    // change what another sees
    AutoPtr< ArrayOf<IAccount*> > accountsCopy = ArrayOf<IAccount*>::Alloc(accounts.GetLength());
    for (Int32 i = 0; i < accounts.GetLength(); ++i) {
        accountsCopy->Set(i, accounts[i]);
    }

    AutoPtr<IHandler> h = (handler == NULL) ? mMainHandler.Get() : handler;
    AutoPtr<IRunnable> action = new AccountUpdateAction(listener, accountsCopy);
    Boolean result;
    h->Post(action, &result);
}

ECode CAccountManager::ConvertErrorToException(
    /* [in] */ Int32 code,
    /* [in] */ const String& message)
{
    if (code == ERROR_CODE_NETWORK_ERROR) {
        Slogger::E(TAG, "%s", (const char*)message);
        return E_IO_EXCEPTION;
//        return new IOException(message);
    }

    if (code == ERROR_CODE_UNSUPPORTED_OPERATION) {
        Slogger::E(TAG, "%s", (const char*)message);
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
//        return new UnsupportedOperationException(message);
    }

    if (code == ERROR_CODE_INVALID_RESPONSE) {
        Slogger::E(TAG, "%s", (const char*)message);
        return E_AUTHENTICATOR_EXCEPTION;
//        return new AuthenticatorException(message);
    }

    if (code == ERROR_CODE_BAD_ARGUMENTS) {
        Slogger::E(TAG, "%s", (const char*)message);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        return new IllegalArgumentException(message);
    }

    Slogger::E(TAG, "%s", (const char*)message);
    return E_AUTHENTICATOR_EXCEPTION;
//    return new AuthenticatorException(message);
}

ECode CAccountManager::GetAuthTokenByFeatures(
    /* [in] */ const String& accountType,
    /* [in] */ const String& authTokenType,
    /* [in] */ const ArrayOf<String>& features,
    /* [in] */ IActivity* activity,
    /* [in] */ IBundle* addAccountOptions,
    /* [in] */ IBundle* getAuthTokenOptions,
    /* [in] */ IAccountManagerCallback* cb,
    /* [in] */ IHandler* handler,
    /* [out] */ IAccountManagerFuture** accountManagerFuture)
{
    VALIDATE_NOT_NULL(accountManagerFuture);
    if (accountType.IsNull()) {
        Slogger::E(TAG, "account type is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("account type is null");
    }
    if (authTokenType.IsNull()) {
        Slogger::E(TAG, "authTokenType type is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("authTokenType is null");
    }
    AutoPtr<GetAuthTokenByTypeAndFeaturesTask> task =
            new GetAuthTokenByTypeAndFeaturesTask(accountType, authTokenType, features,
            activity, addAccountOptions, getAuthTokenOptions, cb, handler, this);
    AutoPtr<IAccountManagerFuture> future;
    task->Start((IAccountManagerFuture**)&future);
    *accountManagerFuture = (IAccountManagerFuture*)task;
    REFCOUNT_ADD(*accountManagerFuture);
    return NOERROR;
}

ECode CAccountManager::NewChooseAccountIntent(
    /* [in] */ IAccount* selectedAccount,
    /* [in] */ const ArrayOf<IAccount*>& allowableAccounts,
    /* [in] */ const ArrayOf<String>& allowableAccountTypes,
    /* [in] */ Boolean alwaysPromptForAccount,
    /* [in] */ const String& descriptionOverrideText,
    /* [in] */ const String& addAccountAuthTokenType,
    /* [in] */ const ArrayOf<String>& addAccountRequiredFeatures,
    /* [in] */ IBundle* addAccountOptions,
    /* [out] */ IIntent** _intent)
{
    VALIDATE_NOT_NULL(_intent);
    AutoPtr<IIntent> intent;
    FAIL_RETURN(CIntent::New((IIntent**)&intent));
    intent->SetClassName(String("elastos"),
            String("elastos.accounts.ChooseTypeAndAccountActivity"));
    AutoPtr< ArrayOf<IParcelable*> > parcelables = ArrayOf<IParcelable*>::Alloc(
            allowableAccounts.GetLength());
    for (Int32 i = 0; i < allowableAccounts.GetLength(); ++i) {
        AutoPtr<IParcelable> p = (IParcelable*)allowableAccounts[i]->Probe(EIID_IParcelable);
        parcelables->Set(i, p);
    }
    intent->PutExtra(ChooseTypeAndAccountActivity::EXTRA_ALLOWABLE_ACCOUNTS_ARRAYLIST,
            parcelables);
    intent->PutExtra(ChooseTypeAndAccountActivity::EXTRA_ALLOWABLE_ACCOUNT_TYPES_STRING_ARRAY,
            const_cast<ArrayOf<String>*>(&allowableAccountTypes));
    intent->PutExtra(ChooseTypeAndAccountActivity::EXTRA_ADD_ACCOUNT_OPTIONS_BUNDLE,
            addAccountOptions);
    intent->PutExtra(ChooseTypeAndAccountActivity::EXTRA_SELECTED_ACCOUNT,
            (Int32)selectedAccount);
    intent->PutBooleanExtra(ChooseTypeAndAccountActivity::EXTRA_ALWAYS_PROMPT_FOR_ACCOUNT,
            alwaysPromptForAccount);
    intent->PutExtra(ChooseTypeAndAccountActivity::EXTRA_DESCRIPTION_TEXT_OVERRIDE,
            descriptionOverrideText);
    intent->PutExtra(ChooseTypeAndAccountActivity::EXTRA_ADD_ACCOUNT_AUTH_TOKEN_TYPE_STRING,
            addAccountAuthTokenType);
    intent->PutExtra(
            ChooseTypeAndAccountActivity::EXTRA_ADD_ACCOUNT_REQUIRED_FEATURES_STRING_ARRAY,
            const_cast<ArrayOf<String>*>(&addAccountRequiredFeatures));
    *_intent = intent;
    REFCOUNT_ADD(*_intent);
    return NOERROR;
}

ECode CAccountManager::AddOnAccountsUpdatedListener(
    /* [in] */ IOnAccountsUpdateListener* listener,
    /* [in] */ IHandler* handler,
    /* [in] */ Boolean updateImmediately)
{
    if (listener == NULL) {
        Slogger::E(TAG, "the listener is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("the listener is null");
    }

    {
        AutoLock lock(mAccountsUpdatedListenersLock);
        HashMap<AutoPtr<IOnAccountsUpdateListener>, AutoPtr<IHandler> >::Iterator it;
        AutoPtr<IOnAccountsUpdateListener> key;
        for (it = mAccountsUpdatedListeners.Begin(); it != mAccountsUpdatedListeners.End(); ++it) {
            key = it->mFirst;
            if (key.Get() == listener) {
                Slogger::E(TAG, "this listener is already added");
                return E_ILLEGAL_STATE_EXCEPTION;
                //throw new IllegalStateException("this listener is already added");
            }
        }

        Boolean wasEmpty = mAccountsUpdatedListeners.Begin() == mAccountsUpdatedListeners.End();

        mAccountsUpdatedListeners[listener] = handler;

        if (wasEmpty) {
            // Register a broadcast receiver to monitor account changes
            AutoPtr<IIntentFilter> intentFilter;
            FAIL_RETURN(CIntentFilter::New((IIntentFilter**)&intentFilter));
            intentFilter->AddAction(LOGIN_ACCOUNTS_CHANGED_ACTION);
            // To recover from disk-full.
            intentFilter->AddAction(IIntent::ACTION_DEVICE_STORAGE_OK);
            AutoPtr<IIntent> intent;
            FAIL_RETURN(mContext->RegisterReceiver(
                    (IBroadcastReceiver*)mAccountsChangedBroadcastReceiver,
                    intentFilter, (IIntent**)&intent));
        }
    }

    if (updateImmediately) {
        AutoPtr< ArrayOf<IAccount*> > accounts;
        GetAccounts((ArrayOf<IAccount*>**)&accounts);
        PostToHandler(handler, listener, *accounts);
    }
    return NOERROR;
}

ECode CAccountManager::RemoveOnAccountsUpdatedListener(
    /* [in] */ IOnAccountsUpdateListener* listener)
{
    if (listener == NULL) {
        Slogger::E(TAG, "listener is null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
//        throw new IllegalArgumentException("listener is null");
    }
    AutoLock lock(mAccountsUpdatedListenersLock);
    HashMap<AutoPtr<IOnAccountsUpdateListener>, AutoPtr<IHandler> >::Iterator it;
    AutoPtr<IOnAccountsUpdateListener> key;
    for (it = mAccountsUpdatedListeners.Begin(); it != mAccountsUpdatedListeners.End(); ++it) {
        key = it->mFirst;
        if (key.Get() == listener) {
            break;
        }
    }
    if (it == mAccountsUpdatedListeners.End()) {
        Slogger::E(TAG, "Listener was not previously added");
        return NOERROR;
    }

    mAccountsUpdatedListeners.Erase(it);
    if (mAccountsUpdatedListeners.Begin() == mAccountsUpdatedListeners.End()) {
        return mContext->UnregisterReceiver(
            (IBroadcastReceiver*)mAccountsChangedBroadcastReceiver->Probe(EIID_IBroadcastReceiver));
    }

    return NOERROR;
}

} // namespace Accounts
} // namespace Droid
} // namespace Elastos
