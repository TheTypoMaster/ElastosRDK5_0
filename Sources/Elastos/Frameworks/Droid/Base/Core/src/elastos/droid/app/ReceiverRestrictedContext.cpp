
#include "app/ReceiverRestrictedContext.h"

namespace Elastos {
namespace Droid {
namespace App {

ReceiverRestrictedContext::ReceiverRestrictedContext(
    /* [in] */ IContext* base)
    : ContextWrapper(base)
{}

PInterface ReceiverRestrictedContext::Probe(
    /* [in]  */ REIID riid)
{
    assert(0);
    return NULL;
}

ECode ReceiverRestrictedContext::RegisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IIntentFilter* filter,
    /* [out] */ IIntent** intent)
{
   return RegisterReceiver(receiver, filter, String(NULL), NULL, intent);
}

ECode ReceiverRestrictedContext::RegisterReceiver(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ const String& broadcastPermission,
    /* [in] */ IHandler* scheduler,
    /* [out] */ IIntent** intent)
{
    if (receiver == NULL) {
        // Allow retrieving current sticky broadcast; this is safe since we
        // aren't actually registering a receiver.
        return ContextWrapper::RegisterReceiver(NULL, filter, broadcastPermission
                , scheduler, intent);
    } else {
//         throw new ReceiverCallNotAllowedException(
//                 "BroadcastReceiver components are not allowed to register to receive intents");
        return E_RECEIVER_CALL_NOT_ALLOWED_EXCEPTION;
    }
}

ECode ReceiverRestrictedContext::RegisterReceiverAsUser(
    /* [in] */ IBroadcastReceiver* receiver,
    /* [in] */ IUserHandle* user,
    /* [in] */ IIntentFilter* filter,
    /* [in] */ const String& broadcastPermission,
    /* [in] */ IHandler* scheduler,
    /* [out] */ IIntent** intent)
{
    if (receiver == NULL) {
        // Allow retrieving current sticky broadcast; this is safe since we
        // aren't actually registering a receiver.
        return ContextWrapper::RegisterReceiverAsUser(NULL, user, filter
                , broadcastPermission, scheduler, intent);
    } else {
//         throw new ReceiverCallNotAllowedException(
//                 "BroadcastReceiver components are not allowed to register to receive intents");
        return E_RECEIVER_CALL_NOT_ALLOWED_EXCEPTION;
    }
}

ECode ReceiverRestrictedContext::BindService(
    /* [in] */ IIntent* service,
    /* [in] */ IServiceConnection* conn,
    /* [in] */ Int32 flags,
    /* [out] */ Boolean* succeeded)
{
//    throw new ReceiverCallNotAllowedException(
//            "BroadcastReceiver components are not allowed to bind to services");
    return E_RECEIVER_CALL_NOT_ALLOWED_EXCEPTION;
}

ReceiverRestrictedContext::ReceiverRestrictedContext()
{
}

ECode ReceiverRestrictedContext::Init(
    /* [in] */ IContext* context)
{
    return ContextWrapper::Init(context);
}

} // namespace App
} // namespace Droid
} // namespace Elastos