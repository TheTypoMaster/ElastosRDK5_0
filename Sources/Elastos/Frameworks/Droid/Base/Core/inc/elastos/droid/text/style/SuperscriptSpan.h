#ifndef __ELASTOS_DROID_TEXT_STYLE_SuperscriptSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_SuperscriptSpan_H__

#include "text/style/MetricAffectingSpan.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

class SuperscriptSpan : public MetricAffectingSpan
{
public:
    SuperscriptSpan();

    SuperscriptSpan(
        /* [in] */ IParcel* src);

    CARAPI_(void) Init();

    CARAPI_(void) Init(
        /* [in] */ IParcel* src);

    CARAPI_(Int32) GetSpanTypeId();

    CARAPI_(Int32) DescribeContents();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    //@Override
    CARAPI UpdateDrawState(
        /* [in] */ ITextPaint* tp);

    //@Override
    CARAPI UpdateMeasureState(
        /* [in] */ ITextPaint* tp);
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_SuperscriptSpan_H__
