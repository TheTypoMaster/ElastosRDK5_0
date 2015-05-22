
#ifndef __X509CRLENTRY_H__
#define __X509CRLENTRY_H__

#ifdef ELASTOS_CORE
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif
#include "cmdef.h"

using Elastos::Math::IBigInteger;
using Elastos::Utility::IDate;
using Elastosx::Security::Auth::X500::IX500Principal;

namespace Elastos {
namespace Security {
namespace Cert {

extern "C" const InterfaceID EIID_X509CRLEntry;

class X509CRLEntry
    : public ElLightRefBase
    , public IX509Extension {
public:
    CAR_INTERFACE_DECL()
    /**
     * Creates a new {@code X509CRLEntry} instance.
     */
    X509CRLEntry();

    /**
     * Returns whether the specified object equals to this instance.
     *
     * @param other
     *            the object to compare.
     * @return {@code true} if the specified object equals to this instance,
     *         otherwise {@code false}.
     */
    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* isEqual);

    /**
     * Returns the hashcode of this instance.
     *
     * @return the hashcode of this instance.
     */
    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

        /**
     * Returns this entry in ASN.1 DER encoded form.
     *
     * @return the encoded form of this entry.
     * @throws CRLException
     *             if encoding fails.
     */
    virtual CARAPI GetEncoded(
        /* [out] */ ArrayOf<Byte>** encode) = 0;

    /**
     * Returns the serial number of the revoked certificate.
     *
     * @return the serial number of the revoked certificate.
     */
    virtual CARAPI GetSerialNumber(
        /* [out] */ IBigInteger** number) = 0;

    /**
     * Returns the issuer of the revoked certificate.
     *
     * @return the issuer of the revoked certificate, or {@code null} if the
     *         issuer is equal to the CRL issuer.
     */
    CARAPI GetCertificateIssuer(
        /* [out] */ IX500Principal** issuer);

    /**
     * Returns the date when the certificate is revoked.
     *
     * @return the date when the certificate is revoked.
     */
    virtual CARAPI GetRevocationDate(
        /* [out] */ IDate** date) = 0;

    /**
     * Returns whether this CRL entry has extensions.
     *
     * @return {@code true} is this CRL entry has extensions, otherwise {@code
     *         false}.
     */
    virtual CARAPI HasExtensions(
        /* [out] */ Boolean* hasExtensions) = 0;

    /**
     * Returns a string representation of this instance.
     *
     * @return a string representation of this instance.
     */
    virtual CARAPI ToString(
        /* [out] */ String* str) = 0;

protected:
private:
};

} // end Cert
} // end Security
} // end Elastos

#endif // __X509CRLENTRY_H__