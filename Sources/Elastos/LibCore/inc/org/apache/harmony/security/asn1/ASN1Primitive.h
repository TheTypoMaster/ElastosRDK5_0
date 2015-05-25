
#ifndef __ASN1PRIMITIVE_H__
#define __ASN1PRIMITIVE_H__

#include <elastos.h>
#include "ASN1Type.h"

_ELASTOS_NAMESPACE_USING

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

/**
 * This abstract class is the super class for all primitive ASN.1 types
 *
 * @see <a href="http://asn1.elibel.tm.fr/en/standards/index.htm">ASN.1</a>
 */
class ASN1Primitive
    : public ASN1Type {
public:
    ASN1Primitive(
        /* [in] */ Int32 tagNumber);

    CARAPI Init(
        /* [in] */ Int32 tagNumber);

    /**
     * Tests provided identifier.
     *
     * @param identifier identifier to be verified
     * @return true if identifier correspond to primitive identifier of this
     *     ASN.1 type, otherwise false
     */
    virtual CARAPI CheckTag(
        /* [in] */ Int32 identifier,
        /* [ou] */ Boolean* result);

    virtual CARAPI EncodeASN(
        /* [in] */ IBerOutputStream* out);

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __ASN1PRIMITIVE_H__
