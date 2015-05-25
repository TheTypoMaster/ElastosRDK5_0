
#include "cmdef.h"
#include "CAtomicStampedReference.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {
namespace Atomic {

ECode CAtomicStampedReference::constructor(
    /* [in] */ IInterface* initialRef,
    /* [in] */ Int32 initialStamp)
{
    mPair = Pair::Of(initialRef, initialStamp);
    return NOERROR;
}

ECode CAtomicStampedReference::GetReference(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    *outface = mPair->mReference;
    INTERFACE_ADDREF(*outface)
    return NOERROR;
}

ECode CAtomicStampedReference::GetStamp(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mPair->mStamp;
    return NOERROR;
}

ECode CAtomicStampedReference::Get(
    /* [in] */ ArrayOf<Int32>* stampHolder,
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    Pair* pair = mPair;
    (*stampHolder)[0] = pair->mStamp;
    *outface = mPair->mReference;
    INTERFACE_ADDREF(*outface)
    return NOERROR;
}

ECode CAtomicStampedReference::WeakCompareAndSet(
    /* [in] */ IInterface* expectedReference,
    /* [in] */ IInterface* newReference,
    /* [in] */ Int32 expectedStamp,
    /* [in] */ Int32 newStamp,
    /* [out] */ Boolean* value)
{
    return CompareAndSet(expectedReference, newReference, expectedStamp, newStamp, value);
}

ECode CAtomicStampedReference::CompareAndSet(
    /* [in] */ IInterface* expectedReference,
    /* [in] */ IInterface* newReference,
    /* [in] */ Int32 expectedStamp,
    /* [in] */ Int32 newStamp,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    Pair* current = mPair;
    *value = expectedReference == current->mReference &&
           expectedStamp == current->mStamp &&
           ((newReference == current->mReference &&
           newStamp == current->mStamp) ||
           CasPair(current, Pair::Of(newReference, newStamp)));
    return NOERROR;
}

ECode CAtomicStampedReference::Set(
    /* [in] */ IInterface* newReference,
    /* [in] */ Int32 newStamp)
{
    Pair* current = mPair;
    if (newReference != current->mReference || newStamp != current->mStamp)
        mPair = Pair::Of(newReference, newStamp);
    return NOERROR;
}

ECode CAtomicStampedReference::AttemptStamp(
    /* [in] */ IInterface* expectedReference,
    /* [in] */ Int32 newStamp,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    Pair* current = mPair;
    return expectedReference == current->mReference &&
           (newStamp == current->mStamp ||
           CasPair(current, Pair::Of(expectedReference, newStamp)));
    return NOERROR;
}

Boolean CAtomicStampedReference::CasPair(
    /* [in] */ Pair* cmp,
    /* [in] */ Pair* val)
{
    assert(0 && "TODO");
    // return UNSAFE.compareAndSwapObject(this, pairOffset, cmp, val);
    return FALSE;
}

} // namespace Atomic
} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
