#ifndef __ELASTOS_DROID_OS_HANDLER_H__
#define __ELASTOS_DROID_OS_HANDLER_H__

#include <ext/frameworkext.h>
#include <elastos/Core/Object.h>

using Elastos::Core::IRunnable;
using Elastos::Core::Mutex;
using Elastos::Core::ISynchronize;
using Elastos::Core::Object;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IMessageQueue;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::IIMessenger;

namespace Elastos {
namespace Droid {
namespace Os {

class Handler
{
private:
    class BlockingRunnable
        : public ElRefBase
        , public Object
        , public IRunnable
        , public ISynchronize
    {
    public:
        BlockingRunnable(
            /* [in] */ IRunnable* task);

        CARAPI_(PInterface) Probe(
        /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);

        CARAPI Lock();

        CARAPI Unlock();

        CARAPI Notify();

        CARAPI NotifyAll();

        CARAPI Wait();

        CARAPI Wait(
            /* [in] */ Int64 millis);

        CARAPI Wait(
            /* [in] */ Int64 millis,
            /* [in] */ Int32 nanos);

        CARAPI Run();

        CARAPI_(Boolean) PostAndWait(
            /* [in] */ Handler* handler,
            /* [in] */ Int64 timeout);

    private:
        AutoPtr<IRunnable> mTask;
        Boolean mDone;
    };

public:
    Handler();

    Handler(
        /* [in] */ Boolean async);

    Handler(
        /* [in] */ ILooper* looper,
        /* [in] */ Boolean async = FALSE);

    Handler(
        /* [in] */ IHandlerCallback* callback,
        /* [in] */ Boolean takeStrongRefOfCallback,
        /* [in] */ Boolean async = FALSE);

    Handler(
        /* [in] */ ILooper* looper,
        /* [in] */ IHandlerCallback* callback,
        /* [in] */ Boolean takeStrongRefOfCallback,
        /* [in] */ Boolean async = FALSE);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

    CARAPI RunWithScissors(
        /* [in] */ IRunnable* r,
        /* [in] */ Int64 timeout,
        /* [out] */ Boolean* result);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [out] */ IMessage** msg);

    CARAPI ObtainMessage(
        /* [in] */ Int32 what,
        /* [in] */ Int32 arg1,
        /* [in] */ Int32 arg2,
        /* [in] */ IInterface* obj,
        /* [out] */ IMessage** msg);

    CARAPI Post(
        /* [in] */ IRunnable* action,
        /* [out] */ Boolean* result);

    CARAPI PostAtTime(
        /* [in] */ IRunnable* action,
        /* [in] */ Int64 uptimeMillis,
        /* [out] */ Boolean* result);

    CARAPI PostAtTime(
        /* [in] */ IRunnable* action,
        /* [in] */ IInterface* token,
        /* [in] */ Int64 uptimeMillis,
        /* [out] */ Boolean* result);

    CARAPI PostDelayed(
        /* [in] */ IRunnable* action,
        /* [in] */ Int64 delayMillis,
        /* [out] */ Boolean* result);

    CARAPI PostAtFrontOfQueue(
        /* [in] */ IRunnable* action,
        /* [out] */ Boolean* result);

    CARAPI RemoveCallbacks(
        /* [in] */ IRunnable* action);

    CARAPI RemoveCallbacks(
        /* [in] */ IRunnable* action,
        /* [in] */ IInterface* token);

    CARAPI GetIMessenger(
        /* [out] */ IIMessenger** mgr);

    CARAPI SendEmptyMessage(
        /* [in] */ Int32 what,
        /* [out] */ Boolean* result);

    CARAPI SendEmptyMessageAtTime(
        /* [in] */ Int32 what,
        /* [in] */ Int64 uptimeMillis,
        /* [out] */ Boolean* result);

    CARAPI SendEmptyMessageDelayed(
        /* [in] */ Int32 what,
        /* [in] */ Int64 delayMillis,
        /* [out] */ Boolean* result);

    CARAPI SendMessage(
        /* [in] */ IMessage* msg,
        /* [out] */ Boolean* result);

    CARAPI SendMessageAtTime(
        /* [in] */ IMessage* msg,
        /* [in] */ Int64 uptimeMillis,
        /* [out] */ Boolean* result);

    CARAPI SendMessageDelayed(
        /* [in] */ IMessage* msg,
        /* [in] */ Int64 delayMillis,
        /* [out] */ Boolean* result);

    CARAPI SendMessageAtFrontOfQueue(
        /* [in] */ IMessage* msg,
        /* [out] */ Boolean* result);

    CARAPI HasCallbacks(
        /* [in] */ IRunnable* action,
        /* [out] */ Boolean* result);

    CARAPI HasMessages(
        /* [in] */ Int32 what,
        /* [out] */ Boolean* result);

    CARAPI HasMessages(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* result);

    CARAPI HasMessages(
        /* [in] */ Int32 id,
        /* [in] */ Int32 what,
        /* [out] */ Boolean* result);

    CARAPI HasMessages(
        /* [in] */ Int32 id,
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* result);

    CARAPI RemoveMessages(
        /* [in] */ Int32 what);

    CARAPI RemoveMessages(
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj);

    CARAPI RemoveMessages(
        /* [in] */ Int32 id,
        /* [in] */ Int32 what,
        /* [in] */ IInterface* obj);

    CARAPI RemoveCallbacksAndMessages(
        /* [in] */ IInterface* obj);

    CARAPI DispatchMessage(
        /* [in] */ IMessage* msg);

    /**
     * Returns a string representing the name of the specified message.
     * The default implementation will either return the class name of the
     * message callback if any, or the hexadecimal representation of the
     * message "what" field.
     *
     * @param message The message whose name is being queried
     */
    CARAPI GetMessageName(
        /* [in] */ IMessage* msg,
        /* [out] */ String* name);

    virtual CARAPI_(String) GetMessageNameImpl(
        /* [in] */ IMessage* msg);
    /**
     * Subclasses must implement this to receive messages.
     */
    virtual CARAPI HandleMessage(
        /* [in] */ IMessage* msg);

protected:
    CARAPI Init(
        /* [in] */ Boolean async = FALSE);

    CARAPI Init(
        /* [in] */ IHandlerCallback* callback,
        /* [in] */ Boolean takeStrongRefOfCallback,
        /* [in] */ Boolean async = FALSE);

    CARAPI Init(
        /* [in] */ ILooper* looper,
        /* [in] */ Boolean async = FALSE);

    CARAPI Init(
        /* [in] */ ILooper* looper,
        /* [in] */ IHandlerCallback* callback,
        /* [in] */ Boolean takeStrongRefOfCallback,
        /* [in] */ Boolean async = FALSE);

private:
    CARAPI_(Boolean) EnqueueMessage(
        /* [in] */ IMessageQueue* queue,
        /* [in] */ IMessage* msg,
        /* [in] */ Int64 uptimeMillis);

    CARAPI_(AutoPtr<IMessage>) GetPostMessage(
        /* [in] */ IRunnable* r);

    CARAPI_(AutoPtr<IMessage>) GetPostMessage(
        /* [in] */ IRunnable* r,
        /* [in] */ IInterface* token);

public:
    Mutex mQueueLock;
    AutoPtr<IMessageQueue> mQueue;
    AutoPtr<ILooper> mLooper;
    AutoPtr<IIMessenger> mMessenger;
    AutoPtr<IHandlerCallback> mCallback;
    AutoPtr<IWeakReference> mWeakCallback;
    Boolean mTakeStrongRefOfCallback;
    Boolean mAsynchronous;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_HANDLER_H__
