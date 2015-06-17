
#ifndef __ELASTOS_UTILITY_CLOCKSUPPORT_H__
#define __ELASTOS_UTILITY_CLOCKSUPPORT_H__

#include "_Elastos_Utility_Concurrent_Locks_CLockSupport.h"
#include "LockSupport.h"
#include "Singleton.h"

using Elastos::Core::Singleton;

namespace Elastos {
namespace Utility {
namespace Concurrent {
namespace Locks {

CarClass(CLockSupport)
    , public Singleton
    , public ILockSupport
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI Unpark(
        /* [in] */ IThread* thread);

    CARAPI Park(
        /* [in] */ IInterface* blocker);

    CARAPI ParkNanos(
        /* [in] */ IInterface* blocker,
        /* [in] */ Int64 nanos);

    CARAPI ParkUntil(
        /* [in] */ IInterface* blocker,
        /* [in] */ Int64 deadline);

    CARAPI Park();

    CARAPI ParkNanos(
        /* [in] */ Int64 nanos);

    CARAPI ParkUntil(
        /* [in] */ Int64 deadline);
};

} // namespace Locks
} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CLOCKSUPPORT_H__
