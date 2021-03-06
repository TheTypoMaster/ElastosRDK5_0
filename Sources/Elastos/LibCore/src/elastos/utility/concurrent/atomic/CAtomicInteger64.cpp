
#include "CAtomicInteger64.h"
#include <cutils/atomic.h>
#include <cutils/atomic-inline.h>

using Elastos::IO::EIID_ISerializable;

namespace Elastos {
namespace Utility {
namespace Concurrent {
namespace Atomic {

CAR_INTERFACE_IMPL_2(CAtomicInteger64, Object, IAtomicInteger64, ISerializable)

CAR_OBJECT_IMPL(CAtomicInteger64)
/**
 * Creates a new AtomicInteger64 with the given initial value.
 *
 * @param initialValue the initial value
 */
ECode CAtomicInteger64::constructor(
    /* [in] */ Int64 initialValue)
{
    mValue = initialValue;
    return NOERROR;
}

/**
 * Creates a new AtomicInteger64 with initial value {@code 0}.
 */
ECode CAtomicInteger64::constructor()
{
    mValue = 0;
    return NOERROR;
}

/**
 * Gets the current value.
 *
 * @return the current value
 */
ECode CAtomicInteger64::Get(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    *value = mValue;
    return NOERROR;
}

/**
 * Sets to the given value.
 *
 * @param newValue the new value
 */
ECode CAtomicInteger64::Set(
    /* [in] */ Int64 newValue)
{
    mValue = newValue;
    return NOERROR;
}

/**
 * Eventually sets to the given value.
 *
 * @param newValue the new value
 * @since 1.6
 */
ECode CAtomicInteger64::LazySet(
    /* [in] */ Int64 newValue)
{
    volatile Int64* address = &mValue;

    // ANDROID_MEMBAR_STORE();
    *address = newValue;
    return NOERROR;
}

/**
 * Atomically sets to the given value and returns the old value.
 *
 * @param newValue the new value
 * @return the previous value
 */
ECode CAtomicInteger64::GetAndSet(
    /* [in] */ Int64 newValue,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    for (;;) {
        Int64 current;
        Get(&current);
        Boolean result;
        if (CompareAndSet(current, newValue, &result), result) {
            *value = current;
            return NOERROR;
        }
    }
}

/**
 * Atomically sets the value to the given updated value
 * if the current value {@code ==} the expected value.
 *
 * @param expect the expected value
 * @param update the new value
 * @return true if successful. False return indicates that
 * the actual value was not equal to the expected value.
 */
ECode CAtomicInteger64::CompareAndSet(
    /* [in] */ Int64 expect,
    /* [in] */ Int64 update,
    /* [out] */ Boolean* result)
{
    volatile int64_t* address = (volatile int64_t*)&mValue;

    assert(0 && "TODO");
    // Note: android_atomic_cmpxchg() returns 0 on success, not failure.
//    int ret = dvmQuasiAtomicCas64(expect, update, address);
//
//    *result = (ret == 0);
    return NOERROR;
}

/**
 * Atomically sets the value to the given updated value
 * if the current value {@code ==} the expected value.
 *
 * <p>May <a href="package-summary.html#Spurious">fail spuriously</a>
 * and does not provide ordering guarantees, so is only rarely an
 * appropriate alternative to {@code compareAndSet}.
 *
 * @param expect the expected value
 * @param update the new value
 * @return true if successful.
 */
ECode CAtomicInteger64::WeakCompareAndSet(
    /* [in] */ Int64 expect,
    /* [in] */ Int64 update,
    /* [out] */ Boolean* result)
{
    volatile int64_t* address = (volatile int64_t*)&mValue;

    assert(0 && "TODO");
    // Note: android_atomic_cmpxchg() returns 0 on success, not failure.
//    int ret = dvmQuasiAtomicCas64(expect, update, address);
//
//    *result = (ret == 0);
    return NOERROR;
}

/**
 * Atomically increments by one the current value.
 *
 * @return the previous value
 */
ECode CAtomicInteger64::GetAndIncrement(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    for (;;) {
        Int64 current;
        Get(&current);
        Int64 next = current + 1;
        Boolean result;
        if (CompareAndSet(current, next, &result), result) {
            *value = current;
            return NOERROR;
        }
    }
}

/**
 * Atomically decrements by one the current value.
 *
 * @return the previous value
 */
ECode CAtomicInteger64::GetAndDecrement(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    for (;;) {
        Int64 current;
        Get(&current);
        Int64 next = current - 1;
        Boolean result;
        if (CompareAndSet(current, next, &result), result) {
            *value = current;
            return NOERROR;
        }
    }
}

/**
 * Atomically adds the given value to the current value.
 *
 * @param delta the value to add
 * @return the previous value
 */
ECode CAtomicInteger64::GetAndAdd(
    /* [in] */ Int64 delta,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    for (;;) {
        Int64 current;
        Get(&current);
        Int64 next = current + delta;
        Boolean result;
        if (CompareAndSet(current, next, &result), result) {
            *value = current;
            return NOERROR;
        }
    }
}

/**
 * Atomically increments by one the current value.
 *
 * @return the updated value
 */
ECode CAtomicInteger64::IncrementAndGet(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    for (;;) {
        Int64 current;
        Get(&current);
        Int64 next = current + 1;
        Boolean result;
        if (CompareAndSet(current, next, &result), result) {
            *value = next;
            return NOERROR;
        }
    }
}

/**
 * Atomically decrements by one the current value.
 *
 * @return the updated value
 */
ECode CAtomicInteger64::DecrementAndGet(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    for (;;) {
        Int64 current;
        Get(&current);
        Int64 next = current - 1;
        Boolean result;
        if (CompareAndSet(current, next, &result), result) {
            *value = next;
            return NOERROR;
        }
    }
}

/**
 * Atomically adds the given value to the current value.
 *
 * @param delta the value to add
 * @return the updated value
 */
ECode CAtomicInteger64::AddAndGet(
    /* [in] */ Int64 delta,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    for (;;) {
        Int64 current;
        Get(&current);
        Int64 next = current + delta;
        Boolean result;
        if (CompareAndSet(current, next, &result), result) {
            *value = next;
            return NOERROR;
        }
    }
}

/**
 * Returns the value of this {@code AtomicInteger64} as an {@code int}.
 */
ECode CAtomicInteger64::Int32Value(
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    Int64 current;
    ECode ec = Get(&current);
    *value = (Int32)current;
    return ec;
}

/**
 * Returns the value of this {@code AtomicInteger64} as a {@code long}
 * after a widening primitive conversion.
 */
ECode CAtomicInteger64::Int64Value(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);

    return Get(value);
}

/**
 * Returns the value of this {@code AtomicInteger64} as a {@code float}
 * after a widening primitive conversion.
 */
ECode CAtomicInteger64::FloatValue(
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);

    Int64 current;
    Get(&current);
    *value = (Float)current;
    return NOERROR;
}

/**
 * Returns the value of this {@code AtomicInteger64} as a {@code double}
 * after a widening primitive conversion.
 */
ECode CAtomicInteger64::DoubleValue(
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);

    Int64 current;
    Get(&current);
    *value = (Double)current;
    return NOERROR;
}

} // namespace Atomic
} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
