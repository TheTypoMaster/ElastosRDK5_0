#include "PropertyPermission.h"

namespace Elastos {
namespace Utility {

CAR_INTERFACE_IMPL(PropertyPermission, BasicPermission, IPropertyPermission)

PropertyPermission::PropertyPermission(
    /* [in] */ String name,
    /* [in] */ String actions)
    : BasicPermission(String(""))
{}

ECode PropertyPermission::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    return BasicPermission::GetName(name);
}

ECode PropertyPermission::NewPermissionCollection(
    /* [out] */ IPermissionCollection** permissions)
{
    VALIDATE_NOT_NULL(permissions)
    return BasicPermission::NewPermissionCollection(permissions);
}

ECode PropertyPermission::GetActions(
    /* [out] */ String* actions)
{
    VALIDATE_NOT_NULL(actions)
    *actions = NULL;
    return NOERROR;
}

ECode PropertyPermission::Implies(
    /* [in] */ IPermission* permission,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;
    return NOERROR;
}

} // namespace Utility
} // namespace Elastos