
#include "CUsernamePasswordCredentials.h"
#include "CBasicUserPrincipal.h"
#include "LangUtils.h"
#include <StringBuilder.h>
#include <elastos/Logger.h>

using Elastos::Core::StringBuilder;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::Util::ILangUtils;
using Org::Apache::Http::Util::LangUtils;

namespace Org {
namespace Apache {
namespace Http {
namespace Auth {

CAR_INTERFACE_IMPL_2(CUsernamePasswordCredentials, Object, IUsernamePasswordCredentials, ICredentials)

CAR_OBJECT_IMPL(CUsernamePasswordCredentials)

ECode CUsernamePasswordCredentials::GetUserPrincipal(
    /* [out] */ IPrincipal** principal)
{
    VALIDATE_NOT_NULL(principal)
    *principal = IPrincipal::Probe(mPrincipal);
    REFCOUNT_ADD(*principal)
    return NOERROR;
}

ECode CUsernamePasswordCredentials::GetUserName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name)
    return mPrincipal->GetUserName(name);
}

ECode CUsernamePasswordCredentials::GetPassword(
    /* [out] */ String* password)
{
    VALIDATE_NOT_NULL(password)
    *password = mPassword;
    return NOERROR;
}

ECode CUsernamePasswordCredentials::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    AutoPtr<IObject> o = IObject::Probe(mPrincipal);
    return o->GetHashCode(hashCode);
}

ECode CUsernamePasswordCredentials::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* equals)
{
    VALIDATE_NOT_NULL(equals)
    if (o == NULL) {
        *equals = FALSE
        return NOERROR;
    }
    if (o == this) {
        *equals = TRUE;
        return NOERROR;
    }
    if (IUsernamePasswordCredentials::Probe(o) != NULL) {
        AutoPtr<IUsernamePasswordCredentials> that = IUsernamePasswordCredentials::Probe(o);
        AutoPtr<IPrincipal> thatPrincipal;
        that->GetUserPrincipal((IPrincipal**)&thatPrincipal);
        if (LangUtils::Equals(mPrincipal, thatPrincipal)) {
            *equals = TRUE;
            return NOERROR;
        }
    }
    *equals = FALSE;
    return NOERROR;
}

ECode CUsernamePasswordCredentials::ToString(
    /* [out] */ String* string)
{
    VALIDATE_NOT_NULL(string)
    AutoPtr<IObject> o = IObject::Probe(mPrincipal);
    return o->ToString(string);
}

ECode CUsernamePasswordCredentials::constructor(
    /* [in] */ const String& usernamePassword)
{
    if (usernamePassword.IsNull()) {
        Logger::E("CUsernamePasswordCredentials", "Username:password string may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 atColon = usernamePassword.indexOf(':');
    if (atColon >= 0) {
        CBasicUserPrincipal::New(usernamePassword.Substring(0, atColon), (IBasicUserPrincipal**)&mPrincipal);
        mPassword = usernamePassword.Substring(atColon + 1);
    }
    else {
        CBasicUserPrincipal::New(usernamePassword, (IBasicUserPrincipal**)&mPrincipal);
        mPassword = String(NULL);
    }
    return NOERROR;
}

ECode CUsernamePasswordCredentials::constructor(
    /* [in] */ const String& userName,
    /* [in] */ const String& password)
{
    if (userName.IsNull()) {
        Logger::E("CUsernamePasswordCredentials", "User name may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    CBasicUserPrincipal::New(userName, (IBasicUserPrincipal**)&mPrincipal);
    mPassword = password;
    return NOERROR;
}

} // namespace Auth
} // namespace Http
} // namespace Apache
} // namespace Org