#include "os/HandlerCaller.h"
#include "os/Looper.h"
#include "os/SomeArgs.h"

using Elastos::Droid::Os::SomeArgs;
using Elastos::Droid::Os::Looper;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Os {

//==========================================================================
// HandlerCaller::MyHandler
//==========================================================================
HandlerCaller::MyHandler::MyHandler(
    /* [in] */ ILooper* looper,
    /* [in] */ IWeakReference* host)
    : Handler(looper)
    , mWeakHost(host)
{
}

ECode HandlerCaller::MyHandler::HandleMessage(
    /* [in] */ IMessage* msg)
{
    AutoPtr<IWeakReferenceSource> wrs;
    mWeakHost->Resolve(EIID_IWeakReferenceSource, (IInterface**)&wrs);
    if (wrs != NULL) {
        HandlerCaller* hc = (HandlerCaller*)wrs.Get();
        AutoPtr<IHandlerCallerCallback> cb = hc->GetCallback();
        if (cb) {
            return cb->ExecuteMessage(msg);
        }
    }

    return NOERROR;
}

//==========================================================================
// HandlerCaller
//==========================================================================
CAR_INTERFACE_IMPL(HandlerCaller, Object, IHandlerCaller)

ECode HandlerCaller::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(
        (IInterface*)(IWeakReferenceSource*)this, CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

HandlerCaller::HandlerCaller(
    /* [in] */ IContext* context,
    /* [in] */ IHandlerCallerCallback* callback,
    /* [in] */ Boolean isStrong)
    : mContext(context)
{
    constructor(context, callback, isStrong);
}

HandlerCaller::HandlerCaller(
    /* [in] */ IContext* context,
    /* [in] */ ILooper* looper,
    /* [in] */ IHandlerCallerCallback* callback,
    /* [in] */ Boolean isStrong)
    : mContext(context)
{
    constructor(context, looper, callback, isStrong);
}

HandlerCaller::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IHandlerCallerCallback* callback,
    /* [in] */ Boolean isStrong)
{
    assert(context != NULL);
    context->GetMainLooper((ILooper**)&mMainLooper);
    AutoPtr<IWeakReference> weakReference = new WeakReferenceImpl(
        (IInterface*)(IWeakReferenceSource*)this, CreateWeak(this));
    mH = new MyHandler(mMainLooper, weakReference);
    if (isStrong) {
        mCallback = callback;
    }
    else if (IWeakReferenceSource::Probe(callback) != NULL) {
        IWeakReferenceSource::Probe(callback)->GetWeakReference((IWeakReference**)&mWeakCallback);
    }
    else {
        assert(0 && "IWeakReferenceSource::Probe(callback) == NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode HandlerCaller::constructor(
    /* [in] */ IContext* context,
    /* [in] */ ILooper* looper,
    /* [in] */ IHandlerCallerCallback* callback,
    /* [in] */ Boolean isStrong)
{
    mMainLooper = looper;
    AutoPtr<IWeakReference> weakReference = new WeakReferenceImpl(
        (IInterface*)(IWeakReferenceSource*)this, CreateWeak(this));
    mH = new MyHandler(mMainLooper, weakReference);
    if (isStrong) {
        mCallback = callback;
    }
    else if (IWeakReferenceSource::Probe(callback) != NULL) {
        IWeakReferenceSource::Probe(callback)->GetWeakReference((IWeakReference**)&mWeakCallback);
    }
    else {
        assert(0 && "IWeakReferenceSource::Probe(callback) == NULL");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode HandlerCaller::GetContext(
    /* [out] */ IContext** ctx)
{
    VALIDATE_NOT_NULL(ctx);
    *ctx = mContext;
    REFCOUNT_ADD(*ctx);
    return NOERROR;
}

ECode HandlerCaller::SetContext(
    /* [in] */ IContext* ctx)
{
    mContext = ctx;
    return NOERROR;
}

ECode HandlerCaller::ExecuteOrSendMessage(
    /* [in] */ IMessage* msg)
{
    // If we are calling this from the main thread, then we can call
    // right through.  Otherwise, we need to send the message to the
    // main thread.
    AutoPtr<ILooper> myLooper = Looper::GetMyLooper();
    if (myLooper == mMainLooper) {
        ECode ec = NOERROR;
        AutoPtr<IHandlerCallerCallback> cb = GetCallback();
        if (cb) {
            ec = cb->ExecuteMessage(msg);
        }

        msg->Recycle();
        return ec;
    }

    Boolean result;
    return mH->SendMessage(msg, &result);
}

ECode HandlerCaller::HasMessages(
    /* [in] */ Int32 what,
    /* [out] */ Boolean* result)
{
    return mH->HasMessages(what, result);
}

ECode HandlerCaller::RemoveMessages(
    /* [in] */ Int32 what)
{
    return mH->RemoveMessages(what);
}

ECode HandlerCaller::RemoveMessages(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* obj)
{
    return mH->RemoveMessages(what, obj);
}

ECode HandlerCaller::SendMessage(
    /* [in] */ IMessage* msg)
{
    Boolean result;
    return mH->SendMessage(msg, &result);
}

ECode HandlerCaller::ObtainMessage(
    /* [in] */ Int32 what,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);

    AutoPtr<IMessage> message;
    mH->ObtainMessage(what, (IMessage**)&message);
    *msg = message;
    REFCOUNT_ADD(*msg);
    return NOERROR;
}

ECode HandlerCaller::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ Boolean arg1,
    /* [in] */ IInterface* arg2,
    /* [out] */ IMessage** msg)
{
    return ObtainMessage(what, arg1 ? 1 : 0, 0, arg2, msg);
}

ECode HandlerCaller::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ Boolean arg1,
    /* [in] */ IInterface* arg2,
    /* [in] */ IInterface* arg3,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);

    AutoPtr<SomeArgs> args = SomeArgs::Obtain();
    args->mArg1 = arg2;
    args->mArg2 = arg3;

    return ObtainMessage(what, arg1 ? 1 : 0, 0, TO_IINTERFACE(args), msg);
}

ECode HandlerCaller::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* arg1,
    /* [out] */ IMessage** msg)
{
    return ObtainMessage(what, 0, 0, arg1, msg);
}

ECode HandlerCaller::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [out] */ IMessage** msg)
{
    return ObtainMessage(what, arg1, 0, NULL, msg);
}

ECode HandlerCaller::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [out] */ IMessage** msg)
{
    return ObtainMessage(what, arg1, arg2, NULL, msg);
}

ECode HandlerCaller::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ IInterface* arg2,
    /* [out] */ IMessage** msg)
{
    return ObtainMessage(what, arg1, 0, arg2, msg);
}

ECode HandlerCaller::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IInterface* arg3,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);

    AutoPtr<IMessage> message;
    mH->ObtainMessage(what, arg1, arg2, arg3, (IMessage**)&message);
    *msg = message;
    REFCOUNT_ADD(*msg);
    return NOERROR;
}

ECode HandlerCaller::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ IInterface* arg3,
    /* [in] */ IInterface* arg4,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);

    AutoPtr<SomeArgs> args = SomeArgs::Obtain();
    args->mArg1 = arg3;
    args->mArg2 = arg4;

    return ObtainMessage(what, arg1, arg2, TO_IINTERFACE(args), msg);
}

ECode HandlerCaller::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ IInterface* arg2,
    /* [in] */ IInterface* arg3,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);

    AutoPtr<SomeArgs> args = SomeArgs::Obtain();
    args->mArg1 = arg2;
    args->mArg2 = arg3;

    return ObtainMessage(what, arg1, 0, TO_IINTERFACE(args), msg);
}

ECode HandlerCaller::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* arg1,
    /* [in] */ IInterface* arg2,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);

    AutoPtr<SomeArgs> args = SomeArgs::Obtain();
    args->mArg1 = arg1;
    args->mArg2 = arg2;

    return ObtainMessage(what, 0, 0, TO_IINTERFACE(args), msg);
}

ECode HandlerCaller::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* arg1,
    /* [in] */ IInterface* arg2,
    /* [in] */ IInterface* arg3,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);

    AutoPtr<SomeArgs> args = SomeArgs::Obtain();
    args->mArg1 = arg1;
    args->mArg2 = arg2;
    args->mArg3 = arg3;

    return ObtainMessage(what, 0, 0, TO_IINTERFACE(args), msg);
}

ECode HandlerCaller::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ IInterface* arg1,
    /* [in] */ IInterface* arg2,
    /* [in] */ IInterface* arg3,
    /* [in] */ IInterface* arg4,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);

    AutoPtr<SomeArgs> args = SomeArgs::Obtain();
    args->mArg1 = arg1;
    args->mArg2 = arg2;
    args->mArg3 = arg3;
    args->mArg4 = arg4;

    return ObtainMessage(what, 0, 0, TO_IINTERFACE(args), msg);
}

ECode HandlerCaller::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ Int32 arg3,
    /* [in] */ Int32 arg4,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);

    AutoPtr<SomeArgs> args = SomeArgs::Obtain();
    args->mArgi1 = arg1;
    args->mArgi2 = arg2;
    args->mArgi3 = arg3;
    args->mArgi4 = arg4;

    return ObtainMessage(what, 0, 0, TO_IINTERFACE(args), msg);
}

ECode HandlerCaller::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ Int32 arg3,
    /* [in] */ Int32 arg4,
    /* [in] */ Int32 arg5,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);

    AutoPtr<SomeArgs> args = SomeArgs::Obtain();
    args->mArgi1 = arg1;
    args->mArgi2 = arg2;
    args->mArgi3 = arg3;
    args->mArgi4 = arg4;
    args->mArgi5 = arg5;

    return ObtainMessage(what, 0, 0, TO_IINTERFACE(args), msg);
}

ECode HandlerCaller::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ Int32 arg3,
    /* [in] */ Int32 arg4,
    /* [in] */ Int32 arg5,
    /* [in] */ Int32 arg6,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);

    AutoPtr<SomeArgs> args = SomeArgs::Obtain();
    args->mArgi1 = arg1;
    args->mArgi2 = arg2;
    args->mArgi3 = arg3;
    args->mArgi4 = arg4;
    args->mArgi5 = arg5;
    args->mArgi6 = arg6;

    return ObtainMessage(what, 0, 0, TO_IINTERFACE(args), msg);
}

ECode HandlerCaller::ObtainMessage(
    /* [in] */ Int32 what,
    /* [in] */ Int32 arg1,
    /* [in] */ Int32 arg2,
    /* [in] */ Int32 arg3,
    /* [in] */ Int32 arg4,
    /* [in] */ IInterface* arg5,
    /* [out] */ IMessage** msg)
{
    VALIDATE_NOT_NULL(msg);

    AutoPtr<SomeArgs> args = SomeArgs::Obtain();
    args->mArgi1 = arg1;
    args->mArgi2 = arg2;
    args->mArgi3 = arg3;
    args->mArgi4 = arg4;
    args->mArg1 = arg5;

    return ObtainMessage(what, 0, 0, TO_IINTERFACE(args), msg);
}

AutoPtr<IHandlerCallerCallback> HandlerCaller::GetCallback()
{
    if (mCallback != NULL)
        return mCallback;

    AutoPtr<IHandlerCallerCallback> callback;
    mWeakCallback->Resolve(EIID_IHandlerCallerCallback, (IInterface**)&callback);
    return callback;
}

} // namespace Os
} // namespace Internal
} // namespace Droid
} // namespace Elastos
