
#ifndef __ELASTOS_UTILITY_CATOMICBOOLEAN_H__
#define __ELASTOS_UTILITY_CATOMICBOOLEAN_H__

#include "_Elastos_Utility_Concurrent_CAtomicBoolean.h"
#include "Object.h"

using Elastos::Core::Object;

namespace Elastos {
namespace Utility {
namespace Concurrent {
namespace Atomic {

CarClass(CAtomicBoolean)
    , public Object
    , public IAtomicBoolean
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()
    /**
     * Creates a new AtomicInteger with the given initial value.
     *
     * @param initialValue the initial value
     */
    CARAPI constructor(
        /* [in] */ Boolean initialValue);

    /**
     * Creates a new AtomicInteger with initial value {@code 0}.
     */
    CARAPI constructor();

    /**
     * Gets the current value.
     *
     * @return the current value
     */
    CARAPI Get(
        /* [out] */ Boolean* value);

    /**
     * Sets to the given value.
     *
     * @param newValue the new value
     */
    CARAPI Set(
        /* [in] */ Boolean newValue);

    /**
     * Eventually sets to the given value.
     *
     * @param newValue the new value
     * @since 1.6
     */
    CARAPI LazySet(
        /* [in] */ Boolean newValue);

    /**
     * Atomically sets to the given value and returns the old value.
     *
     * @param newValue the new value
     * @return the previous value
     */
    CARAPI GetAndSet(
        /* [in] */ Boolean newValue,
        /* [out] */ Boolean* value);

    /**
     * Atomically sets the value to the given updated value
     * if the current value {@code ==} the expected value.
     *
     * @param expect the expected value
     * @param update the new value
     * @return true if successful. False return indicates that
     * the actual value was not equal to the expected value.
     */
    CARAPI CompareAndSet(
        /* [in] */ Boolean expect,
        /* [in] */ Boolean update,
        /* [out] */ Boolean* result);

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
    CARAPI WeakCompareAndSet(
        /* [in] */ Boolean expect,
        /* [in] */ Boolean update,
        /* [out] */ Boolean* result);

private:
    volatile Int32 mValue;
};

} // namespace Atomic
} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CATOMICBOOLEAN_H__
