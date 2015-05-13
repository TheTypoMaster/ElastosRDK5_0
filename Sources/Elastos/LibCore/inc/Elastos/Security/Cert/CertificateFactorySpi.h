
#ifndef __CERTIFICATEFACTORYSPI_H__
#define __CERTIFICATEFACTORYSPI_H__

#ifdef ELASTOS_CORE
#include "Elastos.Core_server.h"
#else
#include "Elastos.Core.h"
#endif

using Elastos::IO::IInputStream;
using Elastos::Utility::ICollection;
using Elastos::Utility::IList;

namespace Elastos {
namespace Security {
namespace Cert {

/**
 * This class defines the <i>Service Provider Interface</i> (<b>SPI</b>) for the
 * {@code CertificateFactory} class. This SPI must be implemented for each
 * certificate type a security provider wishes to support.
 */
class CertificateFactorySpi {
public:
    /**
     * Constructs a new instance of this class.
     */
    CertificateFactorySpi();

    /**
     * Generates and initializes a {@code Certificate} from the provided input
     * stream.
     *
     * @param inStream
     *            the stream from which the data is read to create the
     *            certificate.
     * @return an initialized certificate.
     * @exception CertificateException
     *                if parsing problems are detected.
     */
    virtual CARAPI EngineGenerateCertificate(
        /* [in] */ IInputStream* inStream,
        /* [out] */ ICertificate** cert) = 0;

    /**
     * Generates and initializes a collection of certificates from the provided
     * input stream.
     *
     * @param inStream
     *            the stream from where data is read to create the certificates.
     * @return a collection of certificates.
     * @exception CertificateException
     *                if parsing problems are detected.
     */
    virtual CARAPI EngineGenerateCertificates(
        /* [in] */ IInputStream* inStream,
        /* [out] */ ICollection** certs) = 0;

    /**
     * Generates and initializes a <i>Certificate Revocation List</i> (CRL) from
     * the provided input stream.
     *
     * @param inStream
     *            the stream from where data is read to create the CRL.
     * @return an CRL instance.
     * @exception CRLException
     *                if parsing problems are detected.
     */
    virtual CARAPI EngineGenerateCRL(
        /* [in] */ IInputStream* inStream,
        /* [out] */ ICRL** crl) = 0;

    /**
     * Generates and initializes a collection of <i>Certificate Revocation
     * List</i> (CRL) from the provided input stream.
     *
     * @param inStream
     *            the stream from which the data is read to create the CRLs.
     * @return a collection of CRLs.
     * @exception CRLException
     *                if parsing problems are detected.
     */
    virtual CARAPI EngineGenerateCRLs(
        /* [in] */ IInputStream* inStream,
        /* [out] */ ICollection** crls) = 0;

    /**
     * Generates a {@code CertPath} from the provided {@code InputStream}. The
     * default encoding scheme is applied.
     *
     * @param inStream
     *            an input stream with encoded data.
     * @return a {@code CertPath} initialized from the provided data.
     * @throws CertificateException
     *             if parsing problems are detected.
     */
    virtual CARAPI EngineGenerateCertPath(
        /* [in] */ IInputStream* inStream,
        /* [out] */ ICertPath** certPath);

    /**
     * Generates a {@code CertPath} from the provided {@code
     * InputStream} in the specified encoding.
     *
     * @param inStream
     *            an input stream containing certificate path data in specified
     *            encoding.
     * @param encoding
     *            the encoding of the data in the input stream.
     * @return a {@code CertPath} initialized from the provided data
     * @throws CertificateException
     *             if parsing problems are detected.
     * @throws UnsupportedOperationException
     *             if the provider does not implement this method.
     */
    virtual CARAPI EngineGenerateCertPath(
        /* [in] */ IInputStream* inStream,
        /* [in] */ const String& encoding,
        /* [out] */ ICertPath** certPath);

    /**
     * Generates a {@code CertPath} from the provided list of certificates. The
     * encoding is the default encoding.
     *
     * @param certificates
     *            the list containing certificates in a format supported by the
     *            {@code CertificateFactory}.
     * @return a {@code CertPath} initialized from the provided data.
     * @throws CertificateException
     *             if parsing problems are detected.
     * @throws UnsupportedOperationException
     *             if the provider does not implement this method.
     */
    virtual CARAPI EngineGenerateCertPath(
        /* [in] */ IList** certificates,
        /* [out] */ ICertPath** certPath);

    /**
     * Returns an {@code Iterator} over the supported {@code CertPath} encodings
     * (as Strings). The first element is the default encoding.
     *
     * @return an iterator over supported {@code CertPath} encodings (as
     *         Strings).
     */
     virtual CARAPI EngineGetCertPathEncodings(
        /* [out] */ ICertPath** IIterator);
};

} // end Cert
} // end Security
} // end Elastos

#endif // __CERTIFICATEFACTORYSPI_H__