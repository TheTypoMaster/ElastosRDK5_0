
#include "CLDAPCertStoreParameters.h"
#include "cmdef.h"
#include "StringBuilder.h"

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Security {
namespace Cert {

const String CLDAPCertStoreParameters::DEFAULT_LDAP_SERVER_NAME("localhost");

ECode CLDAPCertStoreParameters::Clone(
    /* [out] */ IInterface **object)
{
    VALIDATE_NOT_NULL(object)
    AutoPtr<ILDAPCertStoreParameters> clonedObj;
    FAIL_RETURN(CLDAPCertStoreParameters::New((ILDAPCertStoreParameters**)&clonedObj))
    *object = clonedObj;
    INTERFACE_ADDREF(*object)
    return NOERROR;
}

ECode CLDAPCertStoreParameters::GetPort(
    /* [out] */ Int32 *port)
{
    VALIDATE_NOT_NULL(port)
    *port = mPort;
    return NOERROR;
}

ECode CLDAPCertStoreParameters::GetServerName(
    /* [out] */ String *name)
{
    VALIDATE_NOT_NULL(name)
    *name = mServerName;
    return NOERROR;
}

ECode CLDAPCertStoreParameters::ToString(
    /* [out] */ String *str)
{
    StringBuilder sb("LDAPCertStoreParameters: [\n serverName: ");
    String sn;
    GetServerName(&sn);
    sb.AppendString(sn);
    sb.AppendCStr("\n port: ");
    Int32 port;
    GetPort(&port);
    sb.AppendInt32(port);
    sb.AppendCStr("\n]");
    return sb.ToString(str);
}

ECode CLDAPCertStoreParameters::constructor(
    /* [in] */ const String& serverName,
    /* [in] */ Int32 port)
{
    if (serverName.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mPort = port;
    mServerName = serverName;
    return NOERROR;
}

ECode CLDAPCertStoreParameters::constructor()
{
    mPort = DEFAULT_LDAP_PORT;
    mServerName = DEFAULT_LDAP_SERVER_NAME;
    return NOERROR;
}

ECode CLDAPCertStoreParameters::constructor(
    /* [in] */ const String& serverName)
{
    if (serverName.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    mPort = DEFAULT_LDAP_PORT;
    mServerName = serverName;
    return NOERROR;
}

}
}
}

