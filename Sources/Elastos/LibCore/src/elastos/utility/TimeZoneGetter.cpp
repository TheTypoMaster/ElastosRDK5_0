
#include "TimeZoneGetter.h"
#include "CSystem.h"

using Elastos::Core::ISystem;

namespace Elastos {
namespace Utility {

CAR_INTERFACE_IMPL(TimeZoneGetter::DefaultTimeZoneGetter, Object, ITimeZoneGetter);

ECode TimeZoneGetter::DefaultTimeZoneGetter::GetId(
    /* [out] */ String* id)
{
    VALIDATE_NOT_NULL(id);

    AutoPtr<ISystem> system;
    AutoPtr<Elastos::Core::CSystem> cs;
    Elastos::Core::CSystem::AcquireSingletonByFriend((Elastos::Core::CSystem**)&cs);
    system = (ISystem*)cs.Get();

    system->GetEnv(String("persist.sys.timezone"), id);
    if ((*id).IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

AutoPtr<ITimeZoneGetter> TimeZoneGetter::sInstance;

AutoPtr<ITimeZoneGetter> TimeZoneGetter::GetInstance()
{
    if (sInstance == NULL) {
        sInstance = new DefaultTimeZoneGetter();
    }

    return sInstance;
}

ECode TimeZoneGetter::SetInstance(
    /* [in] */ ITimeZoneGetter* instance)
{
    if (instance == NULL) {
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    }

    sInstance = instance;
    return NOERROR;
}

} // namespace Utility
} // namespace Elastos
