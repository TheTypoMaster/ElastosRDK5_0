#ifndef __ELASTOS_MATH_CMATHCONTEXTHELPER_H__
#define __ELASTOS_MATH_CMATHCONTEXTHELPER_H__

#include "_Elastos_Math_CMathContextHelper.h"
#include <elastos/core/Singleton.h>

using Elastos::Core::Singleton;

namespace Elastos {
namespace Math {

CarClass(CMathContextHelper)
    , public Singleton
    , public IMathContextHelper
{
public:
    CAR_SINGLETON_DECL()

    CAR_INTERFACE_DECL()

    CARAPI GetDECIMAL32(
        /* [out] */ IMathContext** context);

    CARAPI GetDECIMAL64(
        /* [out] */ IMathContext** context);

    CARAPI GetDECIMAL128(
        /* [out] */ IMathContext** context);

    CARAPI GetUNLIMITED(
        /* [out] */ IMathContext** context);
};

} // namespace Math
} // namespace Elastos

#endif // __ELASTOS_MATH_CMATHCONTEXTHELPER_H__
