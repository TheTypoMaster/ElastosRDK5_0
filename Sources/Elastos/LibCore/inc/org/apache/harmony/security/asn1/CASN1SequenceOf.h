
#ifndef __CASN1SEQUENCEOF_H__
#define __CASN1SEQUENCEOF_H__

#include "_CASN1SequenceOf.h"
#include <Asn1TypeMacro.h>
#include <ASN1ValueCollection.h>

using Elastos::Utility::ICollection;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

CarClass(CASN1SequenceOf), public ASN1ValueCollection
{
public:
    ASN1TYPE_METHODS_DECL()

    CARAPI SetType(
        /* [in] */ IASN1Type* type);

    CARAPI GetType(
        /* [out] */ IASN1Type** type);

    CARAPI GetValues(
        /* [in] */ IInterface* object,
        /* [out] */ ICollection** values);

    CARAPI constructor(
        /* [in] */ IASN1Type* asn1Type);

private:
    // TODO: Add your private member variables here.
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CASN1SEQUENCEOF_H__
