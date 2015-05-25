
#include "CX509EncodedKeySpec.h"
#include "cmdef.h"

namespace Elastos {
namespace Security {
namespace Spec {


ECode CX509EncodedKeySpec::GetEncoded(
    /* [out, callee] */ ArrayOf<Byte> **encoded)
{
    return EncodedKeySpec::GetEncoded(encoded);
}

ECode CX509EncodedKeySpec::GetFormat(
    /* [out] */ String *format)
{
    VALIDATE_NOT_NULL(format)
    *format = String("X.509");
    return NOERROR;
}

ECode CX509EncodedKeySpec::constructor(
    /* [in] */ ArrayOf<Byte> *encodedKey)
{
    return EncodedKeySpec::Init(encodedKey);
}

}
}
}

