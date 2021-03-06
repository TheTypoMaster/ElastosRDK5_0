#include "coredef.h"
#include "StringUtils.h"
#include "Character.h"
#include "StringToIntegral.h"
#include "StringToReal.h"
#include "IntegralToString.h"
#include "RealToString.h"
#include "CString.h"
#include "CPatternHelper.h"
#include "CSplitter.h"
#include "Math.h"
#include "StringBuilder.h"

using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Core::Character;
using Elastos::Core::StringBuilder;
using Elastos::Utility::Regex::IPatternHelper;
using Elastos::Utility::Regex::CPatternHelper;
using Elastos::Utility::Regex::ISplitter;
using Elastos::Utility::Regex::CSplitter;
using Elastos::Utility::Regex::IPattern;
using Elastos::Utility::Regex::IMatcher;

namespace Elastos {
namespace Core {

Int16 StringUtils::ParseInt16(
    /* [in] */ const String& input,
    /* [in] */ Int32 radix,
    /* [in] */ Int16 defValue)
{
    Int16 value;
    ECode ec = StringToIntegral::Parse(input, radix, &value);
    if (FAILED(ec)) {
#ifdef DEBUG
        prInt32f("Warnning: StringUtils::ParseInt16 NumberFormatException, input [%s]\n", input.string());
#endif
        return defValue;
    }

    return value;
}

Int32 StringUtils::ParseInt32(
    /* [in] */ const String& input,
    /* [in] */ Int32 radix,
    /* [in] */ Int32 defValue)
{
    Int32 value;
    ECode ec = StringToIntegral::Parse(input, radix, &value);
    if (FAILED(ec)) {
#ifdef DEBUG
        prInt32f("Warnning: StringUtils::ParseInt32 NumberFormatException, input [%s]\n", input.string());
#endif
        return defValue;
    }

    return value;
}

Int64 StringUtils::ParseInt64(
    /* [in] */ const String& input,
    /* [in] */ Int32 radix,
    /* [in] */ Int64 defValue)
{
    Int64 value;
    ECode ec = StringToIntegral::Parse(input, radix, &value);
    if (FAILED(ec)) {
#ifdef DEBUG
        prInt32f("Warnning: StringUtils::ParseInt64 NumberFormatException, input [%s]\n", input.string());
#endif
        return defValue;
    }

    return value;
}

Float StringUtils::ParseFloat(
    /* [in] */ const String& input,
    /* [in] */ Float defValue)
{
    Float value;
    ECode ec = StringToReal::Parse(input, &value);
    if (FAILED(ec)) {
#ifdef DEBUG
        prInt32f("Warnning: StringUtils::ParseFloat NumberFormatException, input [%s]\n", input.string());
#endif
        return defValue;
    }

    return value;
}

Double StringUtils::ParseDouble(
    /* [in] */ const String& input,
    /* [in] */ Double defValue)
{
    Double value;
    ECode ec = StringToReal::Parse(input, &value);
    if (FAILED(ec)) {
#ifdef DEBUG
        prInt32f("Warnning: StringUtils::ParseDouble NumberFormatException, input [%s]\n", input.string());
#endif
        return defValue;
    }

    return value;
}

ECode StringUtils::Parse(
    /* [in] */ const String& input,
    /* [out] */ Int16* result)
{
    return StringToIntegral::Parse(input, 10, result);
}

ECode StringUtils::Parse(
    /* [in] */ const String& input,
    /* [out] */ Int32* result)
{
    return StringToIntegral::Parse(input, 10, result);
}

ECode StringUtils::Parse(
    /* [in] */ const String& input,
    /* [out] */ Int64* result)
{
    return StringToIntegral::Parse(input, 10, result);
}

ECode StringUtils::Parse(
    /* [in] */ const String& input,
    /* [in] */ Int32 radix,
    /* [out] */ Int16* result)
{
    return StringToIntegral::Parse(input, radix, result);
}

ECode StringUtils::Parse(
    /* [in] */ const String& input,
    /* [in] */ Int32 radix,
    /* [out] */ Int32* result)
{
    return StringToIntegral::Parse(input, radix, result);
}

ECode StringUtils::Parse(
    /* [in] */ const String& input,
    /* [in] */ Int32 radix,
    /* [out] */ Int64* result)
{
    return StringToIntegral::Parse(input, radix, result);
}

ECode StringUtils::Parse(
    /* [in] */ const String& input,
    /* [out] */ Float* result)
{
    return StringToReal::Parse(input, result);
}

ECode StringUtils::Parse(
    /* [in] */ const String& input,
    /* [out] */ Double* result)
{
    return StringToReal::Parse(input, result);
}

String StringUtils::BooleanToString(
    /* [in] */ Boolean b)
{
    return b ? String("TRUE", 4) : String("FALSE", 5);
}

/**
 * Equivalent to Integer.toString(i, radix).
 */
String StringUtils::ToString(
    /* [in] */ Int32 i,
    /* [in] */ Int32 radix)
{
    return IntegralToString::ToString(i, radix);
}

/**
 * Equivalent to Long.toString(v, radix).
 */
String StringUtils::ToString(
    /* [in] */ Int64 v,
    /* [in] */ Int32 radix)
{
    return IntegralToString::ToString(v, radix);
}

String StringUtils::ToBinaryString(
    /* [in] */ Int32 i)
{
    return IntegralToString::ToBinaryString(i);
}

String StringUtils::ToBinaryString(
    /* [in] */ Int64 v)
{
    return IntegralToString::ToBinaryString(v);
}

String StringUtils::ToHexString(
    /* [in] */ Byte b,
    /* [in] */ Boolean upperCase)
{
    return IntegralToString::ToHexString(b, upperCase);
}

String StringUtils::ToHexString(
    /* [in] */ ArrayOf<Byte>& bytes,
    /* [in] */ Boolean upperCase)
{
    return IntegralToString::ToHexString(bytes, upperCase);
}

String StringUtils::ToHexString(
    /* [in] */ Int32 i,
    /* [in] */ Boolean upperCase,
    /* [in] */ Int32 minWidth)
{
    return IntegralToString::ToHexString(i, upperCase, minWidth);
}

String StringUtils::ToHexString(
    /* [in] */ Int64 v,
    /* [in] */ Boolean upperCase)
{
    return IntegralToString::ToHexString(v, upperCase);
}

String StringUtils::ToHexString(
    /* [in] */ Float f)
{
    using Elastos::Core::Math;

    /*
     * Reference: http://en.wikipedia.org/wiki/IEEE_754-1985
     */
    if (f != f) {
        return String("NaN");
    }
    if (f == Math::FLOAT_POSITIVE_INFINITY) {
        return String("Infinity");
    }
    if (f == Math::FLOAT_NEGATIVE_INFINITY) {
        return String("-Infinity");
    }

    Int32 bitValue = Math::FloatToInt32Bits(f);

    Boolean negative = (bitValue & 0x80000000) != 0;
    // mask exponent bits and shift down
    Int32 exponent = ((Int64)bitValue & 0x7f800000) >> 23;
    // mask significand bits and shift up
    // significand is 23-bits, so we shift to treat it like 24-bits
    Int32 significand = (bitValue & 0x007FFFFF) << 1;

    if (exponent == 0 && significand == 0) {
        return (negative ? String("-0x0.0p0") : String("0x0.0p0"));
    }

    StringBuilder hexString(10);
    if (negative) {
        hexString.Append("-0x");
    } else {
        hexString.Append("0x");
    }

    if (exponent == 0) { // denormal (subnormal) value
        hexString.Append("0.");
        // significand is 23-bits, so there can be 6 hex digits
        Int32 fractionDigits = 6;
        // remove trailing hex zeros, so Integer.toHexString() won't prInt32
        // them
        while ((significand != 0) && ((significand & 0xF) == 0)) {
            significand = ((Int64)significand >> 4);
            fractionDigits--;
        }
        // this assumes Integer.toHexString() returns lowercase characters
        String hexSignificand = StringUtils::ToHexString(significand);

        // if there are digits left, then insert some '0' chars first
        if (significand != 0 && fractionDigits > hexSignificand.GetLength()) {
            Int32 digitDiff = fractionDigits - hexSignificand.GetLength();
            while (digitDiff-- != 0) {
                hexString.AppendChar('0');
            }
        }
        hexString.Append(hexSignificand);
        hexString.Append("p-126");
    }
    else { // normal value
        hexString.Append("1.");
        // significand is 23-bits, so there can be 6 hex digits
        Int32 fractionDigits = 6;
        // remove trailing hex zeros, so Integer.toHexString() won't prInt32
        // them
        while ((significand != 0) && ((significand & 0xF) == 0)) {
            significand = ((Int64)significand >> 4);
            fractionDigits--;
        }
        // this assumes Integer.toHexString() returns lowercase characters
        String hexSignificand = StringUtils::ToHexString(significand);

        // if there are digits left, then insert some '0' chars first
        if (significand != 0 && fractionDigits > hexSignificand.GetLength()) {
            Int32 digitDiff = fractionDigits - hexSignificand.GetLength();
            while (digitDiff-- != 0) {
                hexString.AppendChar('0');
            }
        }
        hexString.Append(hexSignificand);
        hexString.AppendChar('p');
        // remove exponent's 'bias' and convert to a string
        hexString.Append(exponent - 127);
    }
    return hexString.ToString();
}

String StringUtils::ToHexString(
    /* [in] */ Double d)
{
    using Elastos::Core::Math;

    /*
     * Reference: http://en.wikipedia.org/wiki/IEEE_754-1985
     */
    if (d != d) {
        return String("NaN");
    }
    if (d == Math::DOUBLE_POSITIVE_INFINITY) {
        return String("Infinity");
    }
    if (d == Math::DOUBLE_NEGATIVE_INFINITY) {
        return String("-Infinity");
    }

    Int64 bitValue = Math::DoubleToInt64Bits(d);

    Boolean negative = (bitValue & 0x8000000000000000L) != 0;
    // mask exponent bits and shift down
    Int64 exponent = ((Int64)bitValue & 0x7FF0000000000000L) >> 52;
    // mask significand bits and shift up
    Int64 significand = bitValue & 0x000FFFFFFFFFFFFFL;

    if (exponent == 0 && significand == 0) {
        return (negative ? String("-0x0.0p0") : String("0x0.0p0"));
    }

    StringBuilder hexString(10);
    if (negative) {
        hexString.Append("-0x");
    } else {
        hexString.Append("0x");
    }

    if (exponent == 0) { // denormal (subnormal) value
        hexString.Append("0.");
        // significand is 52-bits, so there can be 13 hex digits
        Int32 fractionDigits = 13;
        // remove trailing hex zeros, so Integer.toHexString() won't prInt32
        // them
        while ((significand != 0) && ((significand & 0xF) == 0)) {
            significand >>= (Int64)significand >> 4;
            fractionDigits--;
        }
        // this assumes Integer.toHexString() returns lowercase characters
        String hexSignificand = StringUtils::ToHexString(significand);

        // if there are digits left, then insert some '0' chars first
        if (significand != 0 && fractionDigits > hexSignificand.GetLength()) {
            Int32 digitDiff = fractionDigits - hexSignificand.GetLength();
            while (digitDiff-- != 0) {
                hexString.AppendChar('0');
            }
        }
        hexString.Append(hexSignificand);
        hexString.Append("p-1022");
    } else { // normal value
        hexString.Append("1.");
        // significand is 52-bits, so there can be 13 hex digits
        Int32 fractionDigits = 13;
        // remove trailing hex zeros, so Integer.toHexString() won't prInt32
        // them
        while ((significand != 0) && ((significand & 0xF) == 0)) {
            significand >>= (Int64)significand >> 4;
            fractionDigits--;
        }
        // this assumes Integer.toHexString() returns lowercase characters
        String hexSignificand = StringUtils::ToHexString(significand);

        // if there are digits left, then insert some '0' chars first
        if (significand != 0 && fractionDigits > hexSignificand.GetLength()) {
            Int32 digitDiff = fractionDigits - hexSignificand.GetLength();
            while (digitDiff-- != 0) {
                hexString.AppendChar('0');
            }
        }

        hexString.Append(hexSignificand);
        hexString.Append('p');
        // remove exponent's 'bias' and convert to a string
        String str = ToString(exponent - 1023);
        hexString.Append(str);
    }
    return hexString.ToString();
}

String StringUtils::ToOctalString(
    /* [in] */ Int32 i)
{
    return IntegralToString::ToOctalString(i);
}

String StringUtils::ToOctalString(
    /* [in] */ Int64 v)
{
    return IntegralToString::ToOctalString(v);
}

String StringUtils::ToString(
    /* [in] */ Double d)
{
    return RealToString::GetInstance()->ToString(d);
}

String StringUtils::ToString(
    /* [in] */ Float f)
{
    return RealToString::GetInstance()->ToString(f);
}

ECode StringUtils::Split(
    /* [in] */ const String& input,
    /* [in] */ const String& regularExpression,
    /* [out, callee] */ ArrayOf<String>** array)
{
    return Split(input, regularExpression, 0, array);
}

ECode StringUtils::Split(
    /* [in] */ const String& input,
    /* [in] */ const String& regularExpression,
    /* [in] */ Int32 limit,
    /* [out, callee] */ ArrayOf<String>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = NULL;

    AutoPtr<ISplitter> splitter;
    CSplitter::AcquireSingleton((ISplitter**)&splitter);
    splitter->FastSplit(regularExpression, input, limit, array);
    if (*array != NULL){
        return NOERROR;
    }
    else{
        AutoPtr<IPatternHelper> helper;
        FAIL_RETURN(CPatternHelper::AcquireSingleton((IPatternHelper**)&helper));
        AutoPtr<IPattern> pattern;
        FAIL_RETURN(helper->Compile(regularExpression, (IPattern**)&pattern));
        AutoPtr<ICharSequence> seq;
        FAIL_RETURN(CString::New(input, (ICharSequence**)&seq));
        return pattern->Split(seq, limit, array);
    }
}

ECode StringUtils::ReplaceFirst(
    /* [in] */ const String& input,
    /* [in] */ const String& regularExpression,
    /* [in] */ const String& replacement,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = String(NULL);

    AutoPtr<IPatternHelper> helper;
    FAIL_RETURN(CPatternHelper::AcquireSingleton((IPatternHelper**)&helper));
    AutoPtr<IPattern> pattern;
    FAIL_RETURN(helper->Compile(regularExpression, (IPattern**)&pattern));

    AutoPtr<IMatcher> matcher;
    FAIL_RETURN(pattern->Matcher(input, (IMatcher**)&matcher));
    return matcher->ReplaceFirst(replacement, result);
}

ECode StringUtils::ReplaceAll(
    /* [in] */ const String& input,
    /* [in] */ const String& regularExpression,
    /* [in] */ const String& replacement,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = String(NULL);

    AutoPtr<IPatternHelper> helper;
    FAIL_RETURN(CPatternHelper::AcquireSingleton((IPatternHelper**)&helper));
    AutoPtr<IPattern> pattern;
    FAIL_RETURN(helper->Compile(regularExpression, (IPattern**)&pattern));

    AutoPtr<IMatcher> matcher;
    FAIL_RETURN(pattern->Matcher(input, (IMatcher**)&matcher));
    return matcher->ReplaceAll(replacement, result);
}

ECode StringUtils::Matches(
    /* [in] */ const String& input,
    /* [in] */ const String& regularExpression,
    /*[out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IPatternHelper> helper;
    FAIL_RETURN(CPatternHelper::AcquireSingleton((IPatternHelper**)&helper));
    return helper->Matches(regularExpression, input, result);
}

ECode StringUtils::Split(
    /* [in] */ const String& input,
    /* [in] */ const char* regularExpression,
    /* [out, callee] */ ArrayOf<String>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = NULL;
    VALIDATE_NOT_NULL(regularExpression);

    return Split(input, String(regularExpression), 0, array);
}

ECode StringUtils::Split(
    /* [in] */ const String& input,
    /* [in] */ const char* regularExpression,
    /* [in] */ Int32 limit,
    /* [out, callee] */ ArrayOf<String>** array)
{
    VALIDATE_NOT_NULL(array);
    *array = NULL;
    VALIDATE_NOT_NULL(regularExpression);

    return Split(input, String(regularExpression), limit, array);
}

ECode StringUtils::ReplaceFirst(
    /* [in] */ const String& input,
    /* [in] */ const char* regularExpression,
    /* [in] */ const char* replacement,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = String(NULL);
    VALIDATE_NOT_NULL(regularExpression);
    VALIDATE_NOT_NULL(replacement);

    return ReplaceFirst(input, String(regularExpression), String(replacement), result);
}

ECode StringUtils::ReplaceAll(
    /* [in] */ const String& input,
    /* [in] */ const char* regularExpression,
    /* [in] */ const char* replacement,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = String(NULL);
    VALIDATE_NOT_NULL(regularExpression);
    VALIDATE_NOT_NULL(replacement);

    return ReplaceAll(input, String(regularExpression), String(replacement), result);
}

ECode StringUtils::Matches(
    /* [in] */ const String& input,
    /* [in] */ const char* regularExpression,
    /*[out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(regularExpression);

    return Matches(input, String(regularExpression), result);
}

ECode StringUtils::ParsePositiveInt64(
    /* [in] */ const String& input,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    return ParsePositiveInt64(input, 10, result);
}

ECode StringUtils::ParsePositiveInt64(
    /* [in] */ const String& input,
    /* [in] */ const Int32 radix,
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    if (radix < Character::MIN_RADIX || Character::MAX_RADIX) {
        return E_NUMBER_FORMAT_EXCEPTION;
    }
    if (input.IsEmpty() || input.GetLength() == 0) {
        return E_NUMBER_FORMAT_EXCEPTION;
    }
    return StringToIntegral::Parse(input, 0, radix, FALSE, result);
}

} // namespace Core
} // namespace Elastos
