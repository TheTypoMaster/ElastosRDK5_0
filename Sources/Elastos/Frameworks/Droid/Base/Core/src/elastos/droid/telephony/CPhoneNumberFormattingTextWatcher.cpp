
#include "CPhoneNumberFormattingTextWatcher.h"
#include "text/Selection.h"
#include "CPhoneNumberUtils.h"
//#include "CString.h"

using Elastos::Core::CString;
using Elastos::Droid::Text::Selection;
using Libcore::ICU::CLocaleHelper;
using Libcore::ICU::ILocale;
using Libcore::ICU::ILocaleHelper;

namespace Elastos {
namespace Droid {
namespace Telephony {


ECode CPhoneNumberFormattingTextWatcher::BeforeTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 count,
    /* [in] */ Int32 after)
{
    if (mSelfChange || mStopFormatting) {
        return NOERROR;
    }
    // If the user manually deleted any non-dialable characters, stop formatting
    if (count > 0 && HasSeparator(s, start, count)) {
        StopFormatting();
    }
    return NOERROR;
}

ECode CPhoneNumberFormattingTextWatcher::OnTextChanged(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 start,
    /* [in] */ Int32 before,
    /* [in] */ Int32 count)
{
    if (mSelfChange || mStopFormatting) {
        return NOERROR;
    }
    // If the user manually deleted any non-dialable characters, stop formatting
    if (count > 0 && HasSeparator(s, start, count)) {
        StopFormatting();
    }
    return NOERROR;
}

ECode CPhoneNumberFormattingTextWatcher::AfterTextChanged(
    /* [in] */ IEditable* s)
{
    if (mStopFormatting) {
        // Restart the formatting when all texts were clear.
        Int32 len;
        mStopFormatting = !((s->GetLength(&len), len) == 0);
        return NOERROR;
    }
    if (mSelfChange) {
        // Ignore the change caused by s.replace().
        return NOERROR;
    }
    String formatted = Reformat(s, Selection::GetSelectionEnd(s));
    if (!formatted.IsNull()) {
        Int32 rememberedPos;/* = mFormatter.getRememberedPosition()*/;
        mSelfChange = TRUE;
        Int32 len1, len2;
        s->GetLength(&len1);
        AutoPtr<ICharSequence> cs;
        CString::New(formatted, (ICharSequence**)&cs);
        cs->GetLength(&len2);
        s->Replace(0, len1, cs, 0, len2);
        // The text could be changed by other TextWatcher after we changed it. If we found the
        // text is not the one we were expecting, just give up calling setSelection().
        String str;
        s->ToString(&str);
        if (formatted.Equals(str)) {
            Selection::SetSelection(s, rememberedPos);
        }
        mSelfChange = FALSE;
    }

    return NOERROR;
}

ECode CPhoneNumberFormattingTextWatcher::constructor()
{
    AutoPtr<ILocale> deloc;
    AutoPtr<ILocaleHelper> helper;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&helper);
    helper->GetDefault((ILocale**)&deloc);
    String countryCode;
    deloc->GetCountry(&countryCode);
    return Init(countryCode);
}

ECode CPhoneNumberFormattingTextWatcher::constructor(
    /* [in] */ const String& countryCode)
{
    return Init(countryCode);
}

ECode CPhoneNumberFormattingTextWatcher::Init(
        /* [in] */ const String& countryCode)
{
    if (countryCode.IsNull()) return E_ILLEGAL_ARGUMENT_EXCEPTION;
    //mFormatter = PhoneNumberUtil.getInstance().getAsYouTypeFormatter(countryCode);
    mSelfChange = FALSE;
    assert(0);

    return NOERROR;
}

String CPhoneNumberFormattingTextWatcher::Reformat(
    /* [in] */ ICharSequence* s,
    /* [in] */ Int32 cursor)
{
    // The index of Char32 to the leftward of the cursor.
    Int32 curIndex = cursor - 1;
    String formatted;
    //mFormatter.clear();
    Char32 lastNonSeparator = 0;
    Boolean hasCursor = FALSE, tmpRes;
    Int32 len;
    s->GetLength(&len);
    for (Int32 i = 0; i < len; i++) {
        Char32 c;
        s->GetCharAt(i, &c);
        if (CPhoneNumberUtils::IsNonSeparator(c, &tmpRes), tmpRes) {
            if (lastNonSeparator != 0) {
                formatted = GetFormattedNumber(lastNonSeparator, hasCursor);
                hasCursor = FALSE;
            }
            lastNonSeparator = c;
        }
        if (i == curIndex) {
            hasCursor = TRUE;
        }
    }
    if (lastNonSeparator != 0) {
        formatted = GetFormattedNumber(lastNonSeparator, hasCursor);
    }
    return formatted;

}

String CPhoneNumberFormattingTextWatcher::GetFormattedNumber(
    /* [in] */ Char32 lastNonSeparator,
    /* [in] */ Boolean hasCursor)
{
    /*return hasCursor ? mFormatter.inputDigitAndRememberPosition(lastNonSeparator)
            : mFormatter.inputDigit(lastNonSeparator);*/
    assert(0);
    return String(NULL);
}

ECode CPhoneNumberFormattingTextWatcher::StopFormatting()
{
    mStopFormatting = TRUE;
    //mFormatter.clear();
    return NOERROR;
}

Boolean CPhoneNumberFormattingTextWatcher::HasSeparator(
    /* [in] */ ICharSequence* s,
    /* [in] */ const Int32 start,
    /* [in] */ const Int32 count)
{
    for (Int32 i = start; i < start + count; i++) {
        Char32 c;
        s->GetCharAt(i, &c);
        Boolean res;
        if (!CPhoneNumberUtils::IsNonSeparator(c, &res)) {
            return TRUE;
        }
    }
    return FALSE;

}

}
}
}

