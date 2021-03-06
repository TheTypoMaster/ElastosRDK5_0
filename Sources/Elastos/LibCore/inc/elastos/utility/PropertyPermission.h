#ifndef __ELASTOS_UTILITY_PROPERTYPERMISSION_H__
#define __ELASTOS_UTILITY_PROPERTYPERMISSION_H__

#include "security/BasicPermission.h"

using Elastos::Security::BasicPermission;
using Elastos::Security::IPermissionCollection;

namespace Elastos {
namespace Utility {

class PropertyPermission
    : public BasicPermission
    , public IPropertyPermission
{
public:
    CAR_INTERFACE_DECL()

    PropertyPermission(
        /* [in] */ String name,
        /* [in] */ String actions);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI NewPermissionCollection(
        /* [out] */ IPermissionCollection** permissions);

    CARAPI GetActions(
        /* [out] */ String* actions);

    CARAPI Implies(
        /* [in] */ IPermission* permission,
        /* [out] */ Boolean* result);
};

} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_PROPERTYPERMISSION_H__