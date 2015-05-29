
#ifndef __ELASTOS_CORE_CDOUBLE_H__
#define __ELASTOS_CORE_CDOUBLE_H__

#include "_Elastos_Core_CDouble.h"
#include "Object.h"

using Elastos::Core::INumber;
using Elastos::Core::IComparable;
using Elastos::IO::ISerializable;

namespace Elastos {
namespace Core {

CarClass(CDouble)
    , public Object
    , public INumber
    , public ISerializable
    , public IComparable
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()


    CARAPI constructor(
        /* [in] */ Double value);

    CARAPI GetValue(
        /* [out] */ Double* result);

    /**
     * Indicates whether this object represents an infinite value.
     *
     * @return {@code true} if the value of this double is positive or negative
     *         infinity; {@code false} otherwise.
     */
    CARAPI IsInfinite(
        /* [out] */ Boolean* result);

    /**
     * Indicates whether this object is a <em>Not-a-Number (NaN)</em> value.
     *
     * @return {@code true} if this double is <em>Not-a-Number</em>;
     *         {@code false} if it is a (potentially infinite) double number.
     */
    CARAPI IsNaN(
        /* [out] */ Boolean* result);

    /**
     * Returns this object's value as a byte. Might involve rounding and/or
     * truncating the value, so it fits into a byte.
     *
     * @return the primitive byte value of this object.
     */
    CARAPI ByteValue(
        /* [out] */ Byte* result);

    /**
     * Returns this object's value as a short. Might involve rounding and/or
     * truncating the value, so it fits into a short.
     *
     * @return the primitive short value of this object.
     */
    CARAPI Int16Value(
        /* [out] */ Int16* result);

    /**
     * Returns this object's value as an int. Might involve rounding and/or
     * truncating the value, so it fits into an int.
     *
     * @return the primitive int value of this object.
     */
    CARAPI Int32Value(
        /* [out] */ Int32* result);

    /**
     * Returns this object's value as a long. Might involve rounding and/or
     * truncating the value, so it fits into a long.
     *
     * @return the primitive long value of this object.
     */
    CARAPI Int64Value(
        /* [out] */ Int64* result);

    /**
     * Returns this object's value as a float. Might involve rounding.
     *
     * @return the primitive float value of this object.
     */
    CARAPI FloatValue(
        /* [out] */ Float* result);

    /**
     * Returns this object's value as a double. Might involve rounding.
     *
     * @return the primitive double value of this object.
     */
    CARAPI DoubleValue(
        /* [out] */ Double* result);

    CARAPI CompareTo(
        /* [in] */ IInterface* other,
        /* [out] */ Int32* result);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI ToString(
        /* [out] */ String* result);

private:
    Double mValue;
};

} // namespace Core
} // namespace Elastos

#endif //__ELASTOS_CORE_CDOUBLE_H__
