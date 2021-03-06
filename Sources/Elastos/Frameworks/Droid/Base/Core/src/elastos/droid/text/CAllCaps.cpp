
#include "ext/frameworkdef.h"
#include "text/CAllCaps.h"
#include "text/TextUtils.h"
#include "text/CSpannableString.h"
#include <elastos/core/Character.h>

using Elastos::Core::Character;
using Elastos::Core::CString;

namespace Elastos {
namespace Droid {
namespace Text {

ECode CAllCaps::Filter(
    /* [in] */ ICharSequence* source,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ ISpanned* dest,
    /* [in] */ Int32 dstart,
    /* [in] */ Int32 dend,
    /* [out] */ ICharSequence** cs)
{
    VALIDATE_NOT_NULL(cs);

    String s;
    Char32 c;
    for (Int32 i = start; i < end; i++) {
        source->GetCharAt(i, &c);
        if (Character::IsLowerCase(c)) {
            AutoPtr< ArrayOf<Char32> > v = ArrayOf<Char32>::Alloc(end - start);
            TextUtils::GetChars(source, start, end, v, 0);
            s = String(*v, 0, end - start);
            s = s.ToUpperCase();

            AutoPtr<ICharSequence> charS;
            FAIL_RETURN(CString::New(s, (ICharSequence**)&charS));
            if (ISpanned::Probe(source) != NULL) {
                AutoPtr<ISpannableString> sp;
                FAIL_RETURN(CSpannableString::New(charS, (ISpannableString**)&sp));
                TextUtils::CopySpansFrom(ISpanned::Probe(source),
                        start, end, EIID_IInterface, sp, 0);
                *cs = ICharSequence::Probe(sp);
                REFCOUNT_ADD(*cs);
                return NOERROR;
            }
            else {
                *cs = charS;
                REFCOUNT_ADD(*cs);
                return NOERROR;
            }
        }
    }

    *cs = NULL;
    return NOERROR;
}

}//namespace Text
}//namespace Droid
}//namespace Elastos
