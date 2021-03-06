#include "text/style/ClickableSpan.h"
#include "text/CTextPaint.h"
//#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

ECode ClickableSpan::UpdateDrawState(
    /* [in] */ ITextPaint* ds)
{
    VALIDATE_NOT_NULL(ds);
    Int32 linkColor;
    ds->SetColor((ds->GetLinkColor(&linkColor), linkColor));
    ds->SetUnderlineText(TRUE);
    return NOERROR;
}

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos