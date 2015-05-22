#ifndef __MATH_DIVISION_H__
#define __MATH_DIVISION_H__

#include <elastos.h>
#include <elquintet.h>
#include "Object.h"

namespace Elastos {
namespace Math {

/**
 * Static library that provides all operations related with division and modular
 * arithmetic to {@link BigInteger}. Some methods are provided in both mutable
 * and immutable way. There are several variants provided listed below:
 *
 * <ul type="circle">
 * <li> <b>Division</b>
 * <ul type="circle">
 * <li>{@link BigInteger} division and remainder by {@link BigInteger}.</li>
 * <li>{@link BigInteger} division and remainder by {@code Int32}.</li>
 * <li><i>gcd</i> between {@link BigInteger} numbers.</li>
 * </ul>
 * </li>
 * <li> <b>Modular arithmetic </b>
 * <ul type="circle">
 * <li>Modular exponentiation between {@link BigInteger} numbers.</li>
 * <li>Modular inverse of a {@link BigInteger} numbers.</li>
 * </ul>
 * </li>
 *</ul>
 */
class Division : public Object
{
public:
    /**
     * Divides an array by an integer value. Implements the Knuth's division
     * algorithm. See D. Knuth, The Art of Computer Programming, vol. 2.
     *
     * @return remainder
     */
    static Int32 DivideArrayByInt(
        /* [in] */ ArrayOf<Int32>& quotient,
        /* [in] */ ArrayOf<Int32>& dividend,
        /* [in] */ Int32 dividendLength,
        /* [in] */ Int32 divisor);

private:
    /** Just to denote that this class can't be instantiated. */
    Division();

    Division(
        /* [in] */ const Division&);

    Division& operator = (
        /* [in] */ const Division&);
};

} // namespace Math
} // namespace Elastos

#endif // __MATH_DIVISION_H__

