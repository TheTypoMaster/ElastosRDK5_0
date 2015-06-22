//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CCallbackContext.h"
#include "CCallbackParcel.h"
#include <pthread.h>
#if !defined(_android) && !defined(_linux)
#include <e_time.h>
#endif
#include <errno.h>
#include <unistd.h>

#define MillisecPerSec 1000
#define MicrosecPerSec 1000000

#define NanoSecPerMicrosec 1000
#define NanoSecPerMilliSec 1000000
#define NanoSecPerSec 1000000000

#define ForEachDLinkNode(t, p, h) \
        for (p = (t)((h)->m_pNext); p != (t)h; p = (t)(p->m_pNext))

#define ForEachDLinkNodeReversely(t, p, h) \
        for (p = (t)((h)->m_pPrev); p != (t)h; p = (t)(p->m_pPrev))

EXTERN_C int CDECL printf(const char *fmt,...);
EXTERN_C int invokeCoalescer(void* pFunc, void* pOldParam, void* pNewParam, int size);
EXTERN_C int invokeCallback(CallbackEventFlags flags, void* sender, void* pThis, void* pFunc, void* pParam, int size);

ELAPI_(PCallbackEvent) _Impl_CallbackSink_AllocCallbackEvent(Elastos::MemorySize size)
{
    // TODO: optimize
    return (PCallbackEvent)malloc(size);
}

ELAPI_(void) _Impl_CallbackSink_FreeCallbackEvent(Elastos::PVoid callbackEvent)
{
    // TODO: optimize

    ((PCallbackEvent)callbackEvent)->~_EzCallbackEvent();
    free(callbackEvent);
}

_ELASTOS_NAMESPACE_BEGIN

CCallbackContext::CCallbackContext()
    : mExitRequested(FALSE)
    , mRequestToQuit(FALSE)
    , mStatus(CallbackContextStatus_Created)
    , mEventsCount(0)
{
    mEventQueue.Initialize();
    mCurrentEvents.Initialize();
}

CCallbackContext::~CCallbackContext()
{
    assert(mCurrentEvents.IsEmpty());

    CancelAllCallbackEvents();

    pthread_mutex_destroy(&mWorkingLock);
    pthread_mutex_destroy(&mQueueLock);

    sem_destroy(&mThreadEvent);
}

//
//  class CCallbackContext
//
UInt32 CCallbackContext::AddRef()
{
    return ElLightRefBase::AddRef();
}

UInt32 CCallbackContext::Release()
{
    return ElLightRefBase::Release();
}

PInterface CCallbackContext::Probe(REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    return NULL;
}

ECode CCallbackContext::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [in] */ InterfaceID* iid)
{
    if (NULL == iid) return E_INVALID_ARGUMENT;

    if (object == (IInterface*)this) {
        *iid = EIID_IInterface;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CCallbackContext::Initialize()
{
    pthread_mutexattr_t recursiveAttr;

    pthread_mutexattr_init(&recursiveAttr);
    pthread_mutexattr_settype(&recursiveAttr, PTHREAD_MUTEX_RECURSIVE);
    ECode ec = sem_init(&mThreadEvent, 0, 0);

    if (FAILED(ec)) return ec;
    if (pthread_mutex_init(&mWorkingLock, &recursiveAttr)) goto Exit;
    if (pthread_mutex_init(&mQueueLock, &recursiveAttr)) goto Exit;
Exit:
    pthread_mutexattr_destroy(&recursiveAttr);
    return NOERROR;
}

void CCallbackContext::PushEvent(
    /* [in] */ PCallbackEvent event)
{
    pthread_mutex_lock(&mQueueLock);
    mCurrentEvents.InsertNext(event);
    pthread_mutex_unlock(&mQueueLock);
}

PCallbackEvent CCallbackContext::PopEvent()
{
    pthread_mutex_lock(&mQueueLock);
    PCallbackEvent callbackEvent = mCurrentEvents.First();
    callbackEvent->Detach();
    pthread_mutex_unlock(&mQueueLock);
    return callbackEvent;
}

Boolean CCallbackContext::IsExistEvent(
    /* [in] */ PCallbackEvent event)
{
    return IsExistEvent(event->m_pSender, event->m_id,
            event->m_pHandlerThis, event->m_pHandlerFunc);
}

Boolean CCallbackContext::IsExistEvent(
    /* [in] */ PInterface sender,
    /* [in] */ CallbackEventId id,
    /* [in] */ PVoid handlerThis,
    /* [in] */ PVoid handlerFunc)
{
    PCallbackEvent curtEvent;
    pthread_mutex_lock(&mQueueLock);
    ForEachDLinkNode(PCallbackEvent, curtEvent, &mCurrentEvents) {
        if ( (sender == curtEvent->m_pSender || sender == (PInterface)-1)
            && (id == curtEvent->m_id || id == (CallbackEventId)-1)
            && (handlerThis == curtEvent->m_pHandlerThis || handlerThis == (PVoid)-1)
            && (handlerFunc == curtEvent->m_pHandlerFunc || handlerFunc == (PVoid)-1)) {
            pthread_mutex_unlock(&mQueueLock);
            return TRUE;
        }
    }

    pthread_mutex_unlock(&mQueueLock);
    return FALSE;
}

ECode CCallbackContext::PostCallbackEvent(
    /* [in] */ PCallbackEvent callbackEvent)
{
    assert(callbackEvent);

    pthread_mutex_lock(&mQueueLock);

    callbackEvent->AddRef();
    if (mExitRequested ||
            mRequestToQuit ||
            mStatus >= CallbackContextStatus_Finishing) {

        pthread_mutex_unlock(&mQueueLock);

        callbackEvent->m_Status = CallingStatus_Cancelled;
        callbackEvent->m_bCompleted = TRUE;
        sem_post(&callbackEvent->m_pSyncEvent);
        callbackEvent->Release();

        return NOERROR;
    }

    Boolean needNotify = mEventQueue.IsEmpty();

    ECode ec;
    PCallbackEvent prevCBEvent;
    PCallbackEvent cancelingCBEvent;

    if (callbackEvent->m_flags & CallbackEventFlag_Duplicated) {
        CallbackEventFlags highestFlag =
            callbackEvent->m_flags & CallbackEventFlag_PriorityMask;
        ForEachDLinkNodeReversely(PCallbackEvent, prevCBEvent, &mEventQueue) {
            if (callbackEvent->m_id == prevCBEvent->m_id
              && callbackEvent->m_pSender == prevCBEvent->m_pSender
              && callbackEvent->m_pHandlerThis == prevCBEvent->m_pHandlerThis
              && callbackEvent->m_pHandlerFunc == prevCBEvent->m_pHandlerFunc) {
                PVoid userFunc = callbackEvent->m_pCoalesceFunc;
                if (userFunc) {
                    Int32 size = 0;
                    Byte* newBuf = NULL;
                    Byte* oldBuf = NULL;

                    if ((NULL != callbackEvent->m_pParameters)
                        && (NULL != prevCBEvent->m_pParameters)) {
                        callbackEvent->m_pParameters->GetElementSize(&size);
                        callbackEvent->m_pParameters->GetElementPayload((Handle32*)&newBuf);
                        prevCBEvent->m_pParameters->GetElementPayload((Handle32*)&oldBuf);
                    }

                    ec = invokeCoalescer(userFunc, oldBuf, newBuf, size);
                    if (FAILED(ec)) {
                        if (ec == E_CANCLE_BOTH_EVENTS) {
                            callbackEvent->Release();
                            prevCBEvent->Detach();
                            prevCBEvent->Release();
                            mEventsCount--;
                            needNotify = FALSE;
                            goto Next;
                        }
                        continue;
                    }
                }
                highestFlag = prevCBEvent->m_flags & CallbackEventFlag_PriorityMask;
                cancelingCBEvent = prevCBEvent;
                prevCBEvent = (PCallbackEvent)prevCBEvent->Next();
                cancelingCBEvent->Detach();
                mEventsCount--;
                cancelingCBEvent->Release();
            }
        }
        if ((callbackEvent->m_flags & CallbackEventFlag_PriorityMask)
                > highestFlag) {
            callbackEvent->m_flags &= ~CallbackEventFlag_PriorityMask;
            callbackEvent->m_flags |= highestFlag;
        }
    }

    ForEachDLinkNodeReversely(PCallbackEvent, prevCBEvent, &mEventQueue) {
        CallbackEventFlags prevPri = prevCBEvent->m_flags & CallbackEventFlag_PriorityMask;
        CallbackEventFlags currPri = callbackEvent->m_flags & CallbackEventFlag_PriorityMask;
        if ((prevPri < currPri) || ((prevPri == currPri) &&
            (prevCBEvent->m_when <= callbackEvent->m_when))) {
            prevCBEvent->InsertNext(callbackEvent);
            mEventsCount++;
            goto Next;
        }
    }

    mEventQueue.InsertFirst(callbackEvent);
    mEventsCount++;

Next:
    pthread_mutex_unlock(&mQueueLock);

    if (needNotify) {
        sem_post(&mThreadEvent);
    }

    return NOERROR;
}

ECode CCallbackContext::SendCallbackEvent(
    /* [in] */ PCallbackEvent callbackEvent,
    /* [in] */ Millisecond32 timeOut)
{
    ECode ec;
    Int32 CCStatus = 0;
    WaitResult wr = WaitResult_OK;
    PInterface orgCallbackContext;
    Boolean cancelled = FALSE;
    struct timespec ts;
    struct timeval tp;

    if (INFINITE != timeOut) {
        long long temp1, temp2;
        gettimeofday(&tp, NULL);
        ts.tv_sec  = tp.tv_sec;
        temp1 = (long long)tp.tv_usec * NanoSecPerMicrosec;
        temp2 = (long long)timeOut * NanoSecPerMilliSec;
        ts.tv_nsec = (long)((temp1 + temp2) % NanoSecPerSec);
        ts.tv_sec += (long)((temp1 + temp2) / NanoSecPerSec);
    }

    orgCallbackContext = (PInterface)pthread_getspecific(TL_ORG_CALLBACK_SLOT);

    if (orgCallbackContext) {
        CCStatus = _Impl_CallbackSink_GetStatus(orgCallbackContext);
    }

    if (orgCallbackContext && CCStatus > CallbackContextStatus_Created) {
        ec = _Impl_CallbackSink_GetThreadEvent(
                orgCallbackContext, &callbackEvent->m_pSyncEvent);
        if (NOERROR == ec) {
            ec = _Impl_CallbackSink_PostCallbackEvent(
                    this, callbackEvent);
            if (NOERROR == ec) {
                ec = _Impl_CallbackSink_WaitForCallbackEvent(
                        orgCallbackContext, timeOut, &wr,
                        (Boolean*)&callbackEvent->m_bCompleted,
                        CallbackEventFlag_SyncCall);
                if (NOERROR == ec) {
                    if (WaitResult_OK == wr) ec = NOERROR;
                    else if (WaitResult_Interrupted == wr) {
                        ec = E_INTERRUPTED;
                        cancelled = TRUE;
                    }
                    else if (WaitResult_TimedOut == wr) {
                        ec = E_TIMED_OUT;
                        cancelled = TRUE;
                    }
                }
            }
        }
    }
    else {
        ec = sem_init(&callbackEvent->m_pSyncEvent, 0, 0);
        if (NOERROR == ec) {
            ec = _Impl_CallbackSink_PostCallbackEvent(this, callbackEvent);
            if (NOERROR == ec) {
                if (INFINITE == timeOut) {
                    ec = sem_wait(&callbackEvent->m_pSyncEvent);
                }
                else {
                    ec = sem_timedwait(&callbackEvent->m_pSyncEvent, &ts);
                }

                if ( ec == -1) {
                    if (errno == ETIMEDOUT) {
                        ec = E_TIMED_OUT;
                        cancelled = TRUE;
                    }
                    else if (errno == EINTR) {
                        ec = E_INTERRUPTED;
                        cancelled = TRUE;
                    }
                }
                else {
                    ec = NOERROR;
                }
            }
        }
    }

    if (callbackEvent->m_Status == CallingStatus_Cancelled) {
        ec = E_CALLBACK_CANCELED;
    }

    if (cancelled) {
        callbackEvent->m_Status = CallingStatus_Cancelled;
    }

    return ec;
}

ECode CCallbackContext::RequestToFinish(
    /* [in] */ Int32 flag)
{
    if (flag == CallbackContextFinish_Nice) {
        mRequestToQuit = TRUE;
    }
    else if (flag == CallbackContextFinish_ASAP) {
        mRequestToQuit = FALSE;
    }
    else return E_NOT_IMPLEMENTED;

    mExitRequested = TRUE;
    sem_post(&mThreadEvent);

    return NOERROR;
}

ECode CCallbackContext::CancelAllPendingCallbacks(
    /* [in] */ PInterface sender)
{
    register PCallbackEvent cbEvent;
    PCallbackEvent cancelingCBEvent;

    pthread_mutex_lock(&mQueueLock);
    ForEachDLinkNode(PCallbackEvent, cbEvent, &mEventQueue) {
        if (cbEvent->m_pSender == sender) {
            cancelingCBEvent = cbEvent;
            cbEvent = (PCallbackEvent)cancelingCBEvent->Prev();
            cancelingCBEvent->Detach();
            --mEventsCount;

            cancelingCBEvent->m_Status = CallingStatus_Cancelled;
            cancelingCBEvent->m_bCompleted = TRUE;
            sem_post(&cancelingCBEvent->m_pSyncEvent);
            cancelingCBEvent->Release();
        }
    }

    pthread_mutex_unlock(&mQueueLock);

    return NOERROR;
}

ECode CCallbackContext::CancelPendingCallback(
    /* [in] */ PInterface sender,
    /* [in] */ CallbackEventId id)
{
    register PCallbackEvent cbEvent;
    PCallbackEvent cancelingCBEvent;

    pthread_mutex_lock(&mQueueLock);
    ForEachDLinkNode(PCallbackEvent, cbEvent, &mEventQueue) {
        if (cbEvent->m_pSender == sender
            && cbEvent->m_id == id) {
            cancelingCBEvent = cbEvent;
            cbEvent = (PCallbackEvent)cancelingCBEvent->Prev();
            cancelingCBEvent->Detach();
            --mEventsCount;

            cancelingCBEvent->m_Status = CallingStatus_Cancelled;
            cancelingCBEvent->m_bCompleted = TRUE;
            sem_post(&cancelingCBEvent->m_pSyncEvent);
            cancelingCBEvent->Release();
        }
    }

    pthread_mutex_unlock(&mQueueLock);

    return NOERROR;
}

ECode CCallbackContext::CancelCallbackEvents(
    /* [in] */ PInterface sender,
    /* [in] */ CallbackEventId id,
    /* [in] */ PVoid handlerThis,
    /* [in] */ PVoid handlerFunc)
{
    register PCallbackEvent cbEvent;
    PCallbackEvent cancelingCBEvent;

    pthread_mutex_lock(&mQueueLock);
    ForEachDLinkNode(PCallbackEvent, cbEvent, &mEventQueue) {
        if (cbEvent->m_pSender == sender
            && cbEvent->m_id == id
            && cbEvent->m_pHandlerThis == handlerThis
            && cbEvent->m_pHandlerFunc == handlerFunc) {
            cancelingCBEvent = cbEvent;
            cbEvent = (PCallbackEvent)cancelingCBEvent->Prev();
            cancelingCBEvent->Detach();
            --mEventsCount;

            cancelingCBEvent->m_Status = CallingStatus_Cancelled;
            cancelingCBEvent->m_bCompleted = TRUE;
            sem_post(&cancelingCBEvent->m_pSyncEvent);
            cancelingCBEvent->Release();
        }
    }
    if (!IsExistEvent(sender, id, handlerThis, handlerFunc)) {
        pthread_mutex_unlock(&mQueueLock);
        return NOERROR;
    }
    pthread_mutex_unlock(&mQueueLock);

    // 'm_workingLock' be used waiting for msg loop idled
    if (!pthread_mutex_trylock(&mWorkingLock)) {
        pthread_mutex_unlock(&mWorkingLock);
    }
    else {
        return E_CALLBACK_IS_BUSY;
    }

    return NOERROR;
}

void CCallbackContext::CancelAllCallbackEvents()
{
    pthread_mutex_lock(&mQueueLock);
    while (!mEventQueue.IsEmpty()) {
        PCallbackEvent callbackEvent = (PCallbackEvent)mEventQueue.First();

        callbackEvent->Detach();
        --mEventsCount;

        callbackEvent->m_Status = CallingStatus_Cancelled;
        callbackEvent->m_bCompleted = TRUE;
        sem_post(&callbackEvent->m_pSyncEvent);
        callbackEvent->Release();

    }

    pthread_mutex_unlock(&mQueueLock);

}

PCallbackEvent CCallbackContext::GetEvent(
    /* [in] */ UInt32 priority)
{
again:
    struct timeval tp;
    long long now;
    gettimeofday(&tp, NULL);
    now = (long long)tp.tv_sec * MillisecPerSec;
    now += (long long)tp.tv_usec / (MicrosecPerSec / MillisecPerSec);

    PCallbackEvent callbackEvent = mEventQueue.First();
    if (priority) {
        while (callbackEvent != &mEventQueue) {
            if ((priority & CallbackEventFlag_PriorityMask) <
                ((UInt32)callbackEvent->m_flags & CallbackEventFlag_PriorityMask)) {
                if ((priority & ~CallbackEventFlag_PriorityMask) == 0
                        || ((priority & ~CallbackEventFlag_PriorityMask)
                        & callbackEvent->m_flags)) {
                    if (now < callbackEvent->m_when) {
                        pthread_mutex_unlock(&mQueueLock);
                        usleep((callbackEvent->m_when - now) *
                                (MicrosecPerSec / MillisecPerSec));
                        pthread_mutex_lock(&mQueueLock);
                        goto again;
                    }

                    callbackEvent->Detach();
                    return callbackEvent;
                }
            }

            callbackEvent = callbackEvent->Next();
        }

        return NULL;
    }
    else {
        if (now < callbackEvent->m_when) {
            pthread_mutex_unlock(&mQueueLock);
            usleep((callbackEvent->m_when - now) *
                    (MicrosecPerSec / MillisecPerSec));
            pthread_mutex_lock(&mQueueLock);
            goto again;
        }

        callbackEvent->Detach();
        return callbackEvent;
    }
}

Int32 CCallbackContext::HandleCallbackEvents(
    /* [in] */ Millisecond32 msTimeOut,
    /* [in] */ WaitResult* result,
    /* [in] */ Boolean* occured,
    /* [in] */ UInt32 priority)
{
    Boolean empty = FALSE;
    PCallbackEvent callbackEvent = NULL;
    IObject* clientObj = NULL;
    PVoid lock = NULL;
    struct timespec ts;
    struct timeval tp;

    Millisecond32 msBegPoint = clock();
    Millisecond32 msLapsedTime = 0;

    PInterface callbackContext;
    callbackContext = (PInterface)pthread_getspecific(TL_CALLBACK_SLOT);
    this->AddRef();
    pthread_setspecific(TL_CALLBACK_SLOT, (PInterface)this);

    if (INFINITE != msTimeOut) {
        long long temp1, temp2;
        gettimeofday(&tp, NULL);
        ts.tv_sec = tp.tv_sec;
        temp1 = (long long)tp.tv_usec * NanoSecPerMicrosec;
        temp2 = (long long)(msTimeOut - msLapsedTime) * NanoSecPerMilliSec;
        ts.tv_nsec = (long)((temp1 + temp2) % NanoSecPerSec);
        ts.tv_sec += (long)((temp1 + temp2) / NanoSecPerSec);
    }

    while (TRUE) {
        pthread_mutex_lock(&mQueueLock);

        if (mExitRequested) {
            if (!mRequestToQuit) {
                pthread_mutex_unlock(&mQueueLock);
                if (result) *result = WaitResult_Interrupted;
                goto Exit;
            }
        }
        if (mEventQueue.IsEmpty()) {
            empty = TRUE;
        }
        else {
            callbackEvent = GetEvent(priority);
            if (callbackEvent == NULL) {
                empty = TRUE;
            }
        }
        if (empty) {
            empty = FALSE;
            if (mRequestToQuit) {
                pthread_mutex_unlock(&mQueueLock);
                if (result) *result = WaitResult_Interrupted;
                goto Exit;
            }
            mStatus = CallbackContextStatus_Idling;
            pthread_mutex_unlock(&mQueueLock);
            if (INFINITE == msTimeOut) {
                sem_wait(&mThreadEvent);
            }
            else {
                sem_timedwait(&mThreadEvent, &ts);
            }

            if (occured) {
                if (*occured) {
                    if (result) *result = WaitResult_OK;
                    *occured = FALSE;
                    goto Exit;
                }
            }
        }
        else {
            --mEventsCount;
            assert(callbackEvent);
            PushEvent(callbackEvent);
            pthread_mutex_lock(&mWorkingLock);
            mStatus = CallbackContextStatus_Working;
            pthread_mutex_unlock(&mQueueLock);

            if (NULL != callbackEvent->m_pHandlerFunc) {
                clientObj = NULL;
                if (CallbackType_CAR == callbackEvent->m_typeOfFunc) {
                    clientObj = (IObject*)callbackEvent->m_pReceiver->Probe(EIID_IObject);
                    assert(clientObj);
                    if (clientObj->AddRef() == 1) goto Cancel;
                    clientObj->Aggregate(AggrType_FriendEnter, (PInterface)&lock);
                }

                if (callbackEvent->m_flags & CallbackEventFlag_SyncCall) {
                    if (CallingStatus_Cancelled == callbackEvent->m_Status) {
                        callbackEvent->m_bCompleted = TRUE;
                        sem_post(&callbackEvent->m_pSyncEvent);
                        goto CallingCompleted;
                    }
                    callbackEvent->m_Status = CallingStatus_Running;
                }

                if (callbackEvent->m_flags & CallbackEventFlag_DirectCall) {
                        _Impl_SetHelperInfoFlag(HELPER_ASYNC_CALLING, TRUE);
                }
                {
                    Int32 size = 0;
                    Byte* buf = NULL;

                    if (callbackEvent->m_pParameters != NULL) {
                        callbackEvent->m_pParameters->GetElementPayload((Handle32*)&buf);
                        callbackEvent->m_pParameters->GetElementSize(&size);
                    }

                    CallbackEventFlags flags = callbackEvent->m_flags;
                    PVoid sender = callbackEvent->m_pSender;
                    PVoid pThis = callbackEvent->m_pHandlerThis;
                    PVoid pFunc = callbackEvent->m_pHandlerFunc;

                    callbackEvent->m_ecRet = invokeCallback(flags, sender, pThis, pFunc, buf, size);
                }
                if (callbackEvent->m_flags & CallbackEventFlag_DirectCall) {
                    _Impl_SetHelperInfoFlag(HELPER_ASYNC_CALLING, FALSE);
                }

                if (callbackEvent->m_flags & CallbackEventFlag_SyncCall) {
                    callbackEvent->m_Status = CallingStatus_Completed;
                    callbackEvent->m_bCompleted = TRUE;
                    sem_post(&callbackEvent->m_pSyncEvent);
                }

CallingCompleted:
                if ((CallbackType_CAR == callbackEvent->m_typeOfFunc) && (clientObj)) {
                    clientObj->Aggregate(AggrType_FriendLeave, (PInterface)lock);
                    clientObj->Release();
                }
            }
Cancel:
            pthread_mutex_unlock(&mWorkingLock);

            PopEvent();
            callbackEvent->Release();

            if (occured) {
                if (*occured) {
                    if (result) *result = WaitResult_OK;
                    *occured = FALSE;
                    goto Exit;
                }
            }
        }

        if (msTimeOut != INFINITE) {
            msLapsedTime = clock() - msBegPoint;
            if (msLapsedTime > msTimeOut) {
                if (result) *result = WaitResult_TimedOut;
                goto Exit;
            }
        }
    }


Exit:
    this->Release();
    pthread_setspecific(TL_CALLBACK_SLOT, callbackContext);
    return 0;
}

_ELASTOS_NAMESPACE_END
