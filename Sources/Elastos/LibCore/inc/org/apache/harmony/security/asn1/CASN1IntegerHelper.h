
#ifndef __CASN1INTEGERHELPER_H__
#define __CASN1INTEGERHELPER_H__

#include "_CASN1IntegerHelper.h"

using Elastos::Math::IBigInteger;

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Asn1 {

CarClass(CASN1IntegerHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ IASN1Integer** instance);

    CARAPI ToIntValue(
        /* [in] */ IInterface* decoded,
        /* [out] */ Int32* intValue);

    CARAPI ToBigIntegerValue(
        /* [in] */ IInterface *decoded,
        /* [out] */ IBigInteger** bigInteger);

    CARAPI FromIntValue(
        /* [in] */ Int32 value,
        /* [out] */ IInterface** fromIntValue);

private:
    // TODO: Add your private member variables here.
};

} // namespace Asn1
} // namespace Security
} // namespace Harmony
} // namespace Apache
} // namespace Org

#endif // __CASN1INTEGERHELPER_H__
