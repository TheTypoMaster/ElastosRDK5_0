#ifndef __ELASTOS_DROID_TEXT_STYLE_MetricAffectingSpan_H__
#define __ELASTOS_DROID_TEXT_STYLE_MetricAffectingSpan_H__

#include "text/style/CharacterStyle.h"
#include "Elastos.Droid.Core_server.h"

using Elastos::Droid::Text::Style::IMetricAffectingSpan;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Style {

/**
 * The classes that affect character-level text formatting in a way that
 * changes the width or height of characters extend this class.
 */
//public abstract
class MetricAffectingSpan : public CharacterStyle
{
public:
    /**
     * A Passthrough MetricAffectingSpan is one that
     * passes {@link #updateDrawState} and {@link #updateMeasureState}
     * calls through to the specified MetricAffectingSpan
     * while still being a distinct object,
     * and is therefore able to be attached to the same Spannable
     * to which the specified MetricAffectingSpan is already attached.
     */
    /* package */
    //static
    class Passthrough
        : public IMetricAffectingSpan
        , public ElRefBase
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* object,
            /* [in] */ InterfaceID* iid);

    public:
        /**
         * Creates a new Passthrough of the specfied MetricAffectingSpan.
         */
        Passthrough(
            /* [in] */ IMetricAffectingSpan* cs);

        /**
         * Passes updateDrawState through to the underlying MetricAffectingSpan.
         */
        //@Override
        CARAPI UpdateDrawState(
            /* [in] */ ITextPaint* tp);

        /**
         * Passes updateMeasureState through to the underlying MetricAffectingSpan.
         */
        //@Override
        CARAPI UpdateMeasureState(
            /* [in] */ ITextPaint* tp);

        /**
         * Returns the MetricAffectingSpan underlying this one, or the one
         * underlying it if it too is a Passthrough.
         */
        //@Override
        CARAPI GetUnderlying(
            /* [out] */ /*IMetricAffectingSpan*/ICharacterStyle** result);

    private:
        AutoPtr<IMetricAffectingSpan> mStyle;
    };

public:
    virtual CARAPI UpdateMeasureState(
        /* [in] */ ITextPaint* p) = 0;

    /**
     * Returns "this" for most MetricAffectingSpans, but for
     * MetricAffectingSpans that were generated by {@link #wrap},
     * returns the underlying MetricAffectingSpan.
     */
    //@Override
    CARAPI_(AutoPtr<IMetricAffectingSpan>) GetUnderlying();
};

} // namespace Style
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_STYLE_MetricAffectingSpan_H__
