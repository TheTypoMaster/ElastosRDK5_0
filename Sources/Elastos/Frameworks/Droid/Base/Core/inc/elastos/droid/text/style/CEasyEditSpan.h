
#ifndef __ELASTOS_DROID_TEXT_STYLE_CEASYEDITSPAN_H__
#define __ELASTOS_DROID_TEXT_STYLE_CEASYEDITSPAN_H__

#include "_Elastos_Droid_Text_Style_CEasyEditSpan.h"
#include "text/style/EasyEditSpan.h"


namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

CarClass(CEasyEditSpan), public EasyEditSpan
{
public:
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI constructor();
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_CEASYEDITSPAN_H__
