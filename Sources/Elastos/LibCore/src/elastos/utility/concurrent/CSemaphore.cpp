
#include "CSemaphore.h"

using Elastos::IO::EIID_ISerializable;
using namespace Elastos::Utility::Concurrent::Locks;

namespace Elastos {
namespace Utility {
namespace Concurrent {

//====================================================================
// CSemaphore::Sync
//====================================================================
CSemaphore::Sync::Sync(
    /* [in] */ Int32 permits)
{
    SetState(permits);
}

ECode CSemaphore::Sync::GetPermits(
    /* [out] */ Int32* out)
{
    VALIDATE_NOT_NULL(out)
    *out = GetState();
    return NOERROR;
}

ECode CSemaphore::Sync::NonfairTryAcquireShared(
    /* [in] */ Int32 acquires,
    /* [out] */ Int32* out)
{
    VALIDATE_NOT_NULL(out)
    for (;;) {
        Int32 available = GetState();
        Int32 remaining = available - acquires;
        if (remaining < 0 ||
            CompareAndSetState(available, remaining)) {
            *out = remaining;
            return NOERROR;
        }
    }
}

ECode CSemaphore::Sync::TryReleaseShared(
    /* [in] */ Int32 releases,
    /* [out] */ Boolean* out)
{
    VALIDATE_NOT_NULL(out)
    for (;;) {
        Int32 current = GetState();
        Int32 next = current + releases;
        if (next < current) {
//            throw new Error("Maximum permit count exceeded");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        if (CompareAndSetState(current, next)) {
            *out = TRUE;
            return NOERROR;
        }
    }
}

ECode CSemaphore::Sync::ReducePermits(
    /* [in] */ Int32 reductions)
{
    for (;;) {
        Int32 current = GetState();
        Int32 next = current - reductions;
        if (next > current) {
//            throw new Error("Permit count underflow");
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        if (CompareAndSetState(current, next)) {
            return NOERROR;
        }
    }
}

ECode CSemaphore::Sync::DrainPermits(
    /* [out] */ Int32* out)
{
    VALIDATE_NOT_NULL(out)
    for (;;) {
        Int32 current = GetState();
        if (current == 0 || CompareAndSetState(current, 0)) {
            *out = current;
            return NOERROR;
        }
    }
}


//====================================================================
// CSemaphore::NonfairSync
//====================================================================
CSemaphore::NonfairSync::NonfairSync(
    /* [in] */ Int32 permits)
    : Sync(permits)
{
}

ECode CSemaphore::NonfairSync::TryAcquireShared(
    /* [in] */ Int32 acquires,
    /* [out] */ Int32* out)
{
    return NonfairTryAcquireShared(acquires, out);
}


//====================================================================
// CSemaphore::FairSync
//====================================================================
CSemaphore::FairSync::FairSync(
    /* [in] */ Int32 permits)
    : Sync(permits)
{
}

ECode CSemaphore::FairSync::TryAcquireShared(
    /* [in] */ Int32 acquires,
    /* [out] */ Int32* out)
{
    VALIDATE_NOT_NULL(out)
    for (;;) {
        Boolean b = FALSE;
        if ((HasQueuedPredecessors(&b), b)) {
            *out = -1;
            return NOERROR;
        }
        Int32 available = GetState();
        Int32 remaining = available - acquires;
        if (remaining < 0 ||
            CompareAndSetState(available, remaining)) {
            *out = remaining;
            return NOERROR;
        }
    }
}


//====================================================================
// CSemaphore::
//====================================================================
CAR_INTERFACE_IMPL_2(CSemaphore, Object, ISemaphore, ISerializable)

CAR_OBJECT_IMPL(CSemaphore);

CSemaphore::constructor(
    /* [in] */ Int32 permits)
{
    mSync = new NonfairSync(permits);
    return NOERROR;
}

CSemaphore::constructor(
    /* [in] */ Int32 permits,
    /* [in] */ Boolean fair)
{
    mSync = fair ? (Sync*)new FairSync(permits) : (Sync*)new NonfairSync(permits);
    return NOERROR;
}

ECode CSemaphore::Acquire()
{
    return mSync->AcquireSharedInterruptibly(1);
}

ECode CSemaphore::AcquireUninterruptibly()
{
    return mSync->AcquireShared(1);
}

ECode CSemaphore::TryAcquire(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    Int32 res;
    mSync->NonfairTryAcquireShared(1, &res);
    *value = res >= 0;
    return NOERROR;
}

ECode CSemaphore::TryAcquire(
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    Int64 res;
    unit->ToNanos(timeout, &res);
    return mSync->TryAcquireSharedNanos(1, res, value);
}

ECode CSemaphore::ReleasePermit()
{
    Boolean b = FALSE;
    return mSync->ReleaseShared(1, &b);
}

ECode CSemaphore::Acquire(
    /* [in] */ Int32 permits)
{
    if (permits < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return mSync->AcquireSharedInterruptibly(permits);
}

ECode CSemaphore::AcquireUninterruptibly(
    /* [in] */ Int32 permits)
{
    if (permits < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return mSync->AcquireShared(permits);
}

ECode CSemaphore::TryAcquire(
    /* [in] */ Int32 permits,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    if (permits < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 res;
    mSync->NonfairTryAcquireShared(permits, &res);
    *value = res >= 0;
    return NOERROR;
}

ECode CSemaphore::TryAcquire(
    /* [in] */ Int32 permits,
    /* [in] */ Int64 timeout,
    /* [in] */ ITimeUnit* unit,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    if (permits < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int64 res;
    unit->ToNanos(timeout, &res);
    return mSync->TryAcquireSharedNanos(permits, res, value);
}

ECode CSemaphore::ReleasePermit(
    /* [in] */ Int32 permits)
{
    if (permits < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Boolean b = FALSE;
    return mSync->ReleaseShared(permits, &b);
}

ECode CSemaphore::AvailablePermits(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    return mSync->GetPermits(value);
}

ECode CSemaphore::DrainPermits(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    return mSync->DrainPermits(value);
}

ECode CSemaphore::ReducePermits(
    /* [in] */ Int32 reduction)
{
    if (reduction < 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return mSync->ReducePermits(reduction);
}

ECode CSemaphore::IsFair(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    *value = mSync->GetSyncID() == SyncID_FairSync;
    return NOERROR;
}

ECode CSemaphore::HasQueuedThreads(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)
    return mSync->HasQueuedThreads(value);
}

ECode CSemaphore::GetQueueLength(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    return mSync->GetQueueLength(value);
}

ECode CSemaphore::GetQueuedThreads(
    /* [out] */ ICollection** out)
{
    VALIDATE_NOT_NULL(out)
    return mSync->GetQueuedThreads(out);
}

ECode CSemaphore::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    Int32 mits;
    mSync->GetPermits(&mits);
//    *str = super.toString() + "[Permits = " + mits + "]";
    String res("[Permits = ");
    res += mits;
    res += "]";
    *str = res;
    return NOERROR;
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
