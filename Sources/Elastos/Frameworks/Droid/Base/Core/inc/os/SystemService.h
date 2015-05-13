#ifndef __SYSTEMSERVICE_H__
#define __SYSTEMSERVICE_H__

#include "ext/frameworkdef.h"
#include "Elastos.Droid.Core_server.h"
#include <elastos/HashMap.h>
#include <elastos/Object.h>

using Elastos::Utility::HashMap;
using Elastos::Core::Threading::Object;

namespace Elastos {
namespace Droid {
namespace Os {

/**
 * Controls and utilities for low-level {@code init} services.
 *
 * @hide
 */
class SystemService
{
public:
    /** Request that the init daemon start a named service. */
    static CARAPI Start(
        /* [in] */ const String& name);

    /** Request that the init daemon stop a named service. */
    static CARAPI Stop(
        /* [in] */ const String& name);

    /** Request that the init daemon restart a named service. */
    static CARAPI Restart(
        /* [in] */ const String& name);

    /**
     * Return current state of given service.
     */
    static CARAPI_(SystemServiceState) GetState(
        /* [in] */ const String& service);

    /**
     * Check if given service is {@link State#STOPPED}.
     */
    static CARAPI_(Boolean) IsStopped(
        /* [in] */ const String& service);

    /**
     * Check if given service is {@link State#RUNNING}.
     */
    static CARAPI_(Boolean) IsRunning(
        /* [in] */ const String& service);

    /**
     * Wait until given service has entered specific state.
     */
    static CARAPI WaitForState(
        /* [in] */ const String& service,
        /* [in] */ SystemServiceState state,
        /* [in] */ Int64 timeoutMillis);

    /**
     * Wait until any of given services enters {@link State#STOPPED}.
     */
    static CARAPI WaitForAnyStopped(
        /* [in] */ ArrayOf<String>* services);

private:
    static CARAPI_(Boolean) Init();

private:
    static HashMap<String, SystemServiceState> sStates;
    static Object sPropertyLock;
    static Boolean sInitialized;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__SYSTEMSERVICE_H__
