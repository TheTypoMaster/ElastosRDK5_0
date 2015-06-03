
#include "CECParameterSpec.h"
#include "CBigIntegerHelper.h"

using Elastos::Math::IBigIntegerHelper;
using Elastos::Math::CBigIntegerHelper;

namespace Elastos {
namespace Security {
namespace Spec {

ECode CECParameterSpec::GetCofactor(
    /* [out] */ Int32 *cofactor)
{
    VALIDATE_NOT_NULL(cofactor)
    *cofactor = mCofactor;
    return NOERROR;
}

ECode CECParameterSpec::GetCurve(
    /* [out] */ IEllipticCurve **curve)
{
    VALIDATE_NOT_NULL(curve)
    *curve = mCurve;
    return NOERROR;
}

ECode CECParameterSpec::GetGenerator(
    /* [out] */ IECPoint **generator)
{
    VALIDATE_NOT_NULL(generator)
    *generator = mGenerator;
    return NOERROR;
}

ECode CECParameterSpec::GetOrder(
    /* [out] */ IBigInteger **order)
{
    VALIDATE_NOT_NULL(order)
    *order = mOrder;
    return NOERROR;
}

ECode CECParameterSpec::constructor(
    /* [in] */ IEllipticCurve *curve,
    /* [in] */ IECPoint *generator,
    /* [in] */ IBigInteger *order,
    /* [in] */ Int32 cofactor)
{
    mCurve = curve;
    mGenerator = generator;
    mOrder = order;
    mCofactor = cofactor;
    // throw NullPointerException if curve, generator or order is null
    if (mCurve == NULL || mGenerator == NULL || mOrder == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    // throw IllegalArgumentException if order or cofactor is not positive
    AutoPtr<IBigIntegerHelper> helper;
    CBigIntegerHelper::AcquireSingleton((IBigIntegerHelper**)&helper);
    AutoPtr<IBigInteger> zero;
    helper->GetZERO((IBigInteger**)&zero);
    Int32 result;
    mOrder->CompareTo(zero.Get(), &result);
    if (result <= 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (mCofactor <= 0) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

}
}
}

