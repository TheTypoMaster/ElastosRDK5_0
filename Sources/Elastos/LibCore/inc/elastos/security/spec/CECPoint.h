
#ifndef __ELASTOS_SECURITY_SPEC_CECPOINT_H__
#define __ELASTOS_SECURITY_SPEC_CECPOINT_H__

#include "_CECPoint.h"

using Elastos::Math::IBigInteger;

namespace Elastos {
namespace Security {
namespace Spec {

CarClass(CECPoint)
{
public:
    static CARAPI_(const AutoPtr<IECPoint>) InitStatic();

    CARAPI GetAffineX(
        /* [out] */ IBigInteger **affineX);

    CARAPI GetAffineY(
        /* [out] */ IBigInteger **affineY);

    CARAPI Equals(
        /* [in] */ IInterface *other,
        /* [out] */ Boolean *result);

    CARAPI GetHashCode(
        /* [out] */ Int32 *hashCode);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IBigInteger *affineX,
        /* [in] */ IBigInteger *affineY);

    /**
     * The point on an Elliptic Curve at infinity.
    */
    static const AutoPtr<IECPoint> POINT_INFINITY;

private:
    // affine X coordinate of this point
    const AutoPtr<IBigInteger> mAffineX;
    // affine Y coordinate of this point
    const AutoPtr<IBigInteger> mAffineY;
};

}
}
}

#endif // __ELASTOS_SECURITY_SPEC_CECPOINT_H__
