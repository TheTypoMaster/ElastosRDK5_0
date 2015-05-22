
#ifndef __PKIXCERTPATHVALIDATORRESULT_H__
#define __PKIXCERTPATHVALIDATORRESULT_H__

#ifdef ELASTOS_CORE
#include "Elastos.CoreLibrary_server.h"
#else
#include "Elastos.CoreLibrary.h"
#endif

using Elastos::Security::IPublicKey;

namespace Elastos {
namespace Security {
namespace Cert {

class PKIXCertPathValidatorResult {
public:
    /**
     * Creates a new {@code PKIXCertPathValidatorResult} with the specified
     * trust anchor, the valid policy tree and the subject public key.
     *
     * @param trustAnchor
     *            the trust anchor describing the certification authority (CA)
     *            that served as trust anchor for the certification path.
     * @param policyTree
     *            the valid policy tree from the validation.
     * @param subjectPublicKey
     *            the subject public key from the validation.
     */
    CARAPI Init(
        /* [in] */ ITrustAnchor *trustAnchor,
        /* [in] */ IPolicyNode *policyTree,
        /* [in] */ IPublicKey *subjectPublicKey);

    /**
     * Returns the valid policy tree from the validation.
     *
     * @return the valid policy tree from the validation.
     */
    CARAPI GetPolicyTree(
        /* [out] */ IPolicyNode **node);

    /**
     * Returns the subject public key from the validation.
     *
     * @return the subject public key from the validation.
     */
    CARAPI GetPublicKey(
        /* [out] */ IPublicKey **key);

    /**
     * Returns the trust anchor describing the certification authority (CA) that
     * served as trust anchor for this certification path.
     *
     * @return the trust anchor.
     */
    CARAPI GetTrustAnchor(
        /* [out] */ ITrustAnchor **anchor);

    /**
     * Clones this {@code PKIXCertPathValidatorResult} instance.
     *
     * @return the cloned instance.
     */
    CARAPI Clone(
        /* [out] */ IInterface **obj);

    /**
     * Returns a string representation for this {@code
     * PKIXCertPathValidatorResult} instance.
     *
     * @return a string representation for this {@code
     *         PKIXCertPathValidatorResult} instance.
     */
    CARAPI ToString(
        /* [out] */ String *str);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

protected:
private:
    // A trust anchor used during validation of certification path
    AutoPtr<ITrustAnchor> mTrustAnchor;
    // Valid policy tree resulting from PKIX
    // certification path validation algorithm
    AutoPtr<IPolicyNode> mPolicyTree;
    // Public key of the subject (target) certificate
    AutoPtr<IPublicKey> mSubjectPublicKey;
};

} // end Cert
} // end Security
} // end Elastos

#endif // __PKIXCERTPATHVALIDATORRESULT_H__