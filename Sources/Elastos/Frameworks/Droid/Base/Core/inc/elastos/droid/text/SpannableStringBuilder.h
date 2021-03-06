#ifndef __ELASTOS_DROID_TEXT_SPANNABLESTRINGBUILDER_H__
#define __ELASTOS_DROID_TEXT_SPANNABLESTRINGBUILDER_H__

#include "ext/frameworkext.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IPaint;
//using Elastos::Utility::IObjectContainer;

namespace Elastos {
namespace Droid {
namespace Text {

/**
 * This is the class for text whose content and markup can both be changed.
 */
class SpannableStringBuilder
{
public:
    /**
     * Create a new SpannableStringBuilder with empty contents
     */
    SpannableStringBuilder();

    /**
     * Create a new SpannableStringBuilder containing a copy of the
     * specified text, including its spans if any.
     */
    SpannableStringBuilder(
        /* [in] */ ICharSequence* text);

    /**
     * Create a new SpannableStringBuilder containing a copy of the
     * specified slice of the specified text, including its spans if any.
     */
    SpannableStringBuilder(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    virtual ~SpannableStringBuilder();

    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;

public:

    static AutoPtr<ISpannableStringBuilder> ValueOf(
        /* [in] */ ICharSequence* source);

    /**
     * Return the char at the specified offset within the buffer.
     */
    virtual CARAPI GetCharAt(
        /* [in] */ Int32 where,
        /* [out] */ Char32* c);

    /**
     * Return the number of chars in the buffer.
     */
    virtual CARAPI_(Int32) GetLength();

    // Documentation from interface
    virtual CARAPI Insert(
        /* [in] */ Int32 where,
        /* [in] */ ICharSequence* tb,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    // Documentation from interface
    virtual CARAPI Insert(
        /* [in] */ Int32 where,
        /* [in] */ ICharSequence* tb);

    // Documentation from interface
    virtual CARAPI Delete(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    // Documentation from interface
    virtual CARAPI Clear();

    // Documentation from interface
    virtual CARAPI ClearSpans();

    // Documentation from interface
    virtual CARAPI Append(
        /* [in] */ ICharSequence* text);

    // Documentation from interface
    virtual CARAPI Append(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    // Documentation from interface
    virtual CARAPI Append(
        /* [in] */ Char32 text);

    // Documentation from interface
    virtual CARAPI Replace(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ICharSequence* tb);

    // Documentation from interface
    virtual CARAPI Replace(
        /* [in] */ const Int32 start,
        /* [in] */ const Int32 end,
        /* [in] */ ICharSequence* tb,
        /* [in] */ Int32 tbstart,
        /* [in] */ Int32 tbend);

    /**
     * Mark the specified range of text with the specified object.
     * The flags determine how the span will behave when text is
     * inserted at the start or end of the span's range.
     */
    virtual CARAPI SetSpan(
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 flags);

    /**
     * Remove the specified markup object from the buffer.
     */
    virtual CARAPI RemoveSpan(
        /* [in] */ IInterface* what);

    /**
     * Return the buffer offset of the beginning of the specified
     * markup object, or -1 if it is not attached to this buffer.
     */
    virtual CARAPI_(Int32) GetSpanStart(
        /* [in] */ IInterface* what);

    /**
     * Return the buffer offset of the end of the specified
     * markup object, or -1 if it is not attached to this buffer.
     */
    virtual CARAPI_(Int32) GetSpanEnd(
        /* [in] */ IInterface* what);

    /**
     * Return the flags of the end of the specified
     * markup object, or 0 if it is not attached to this buffer.
     */
    virtual CARAPI_(Int32) GetSpanFlags(
        /* [in] */ IInterface* what);

    /**
     * Return an array of the spans of the specified type that overlap
     * the specified range of the buffer.  The kind may be Object.class to get
     * a list of all the spans regardless of type.
     */
    //@SuppressWarnings("unchecked")
    virtual CARAPI_(AutoPtr< ArrayOf<IInterface*> >) GetSpans(
        /* [in] */ Int32 queryStart,
        /* [in] */ Int32 queryEnd,
        /* [in] */ const InterfaceID& kind);

    /**
     * Return the next offset after <code>start</code> but less than or
     * equal to <code>limit</code> where a span of the specified type
     * begins or ends.
     */
    virtual CARAPI_(Int32) NextSpanTransition(
        /* [in] */ Int32 start,
        /* [in] */ Int32 limit,
        /* [in] */ const InterfaceID& kind);

    /**
     * Return a new CharSequence containing a copy of the specified
     * range of this buffer, including the overlapping spans.
     */
    virtual CARAPI SubSequence(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ ICharSequence** cs);

    /**
     * Copy the specified range of chars from this buffer into the
     * specified array, beginning at the specified offset.
     */
    virtual CARAPI GetChars(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ArrayOf<Char32>* dest,
        /* [in] */ Int32 destoff);

    /**
     * Return a String containing a copy of the chars in this buffer.
     */
    //@Override
    virtual CARAPI_(String) ToString();

    /**
     * Return a String containing a copy of the chars in this buffer, limited to the
     * [start, end[ range.
     * @hide
     */
    virtual CARAPI_(String) Substring(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    /**
     * Don't call this yourself -- exists for Canvas to use internally.
     * {@hide}
     */
    virtual CARAPI DrawText(
        /* [in] */ ICanvas* c,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ IPaint* p);

    /**
     * Don't call this yourself -- exists for Canvas to use internally.
     * {@hide}
     */
    virtual CARAPI DrawTextRun(
        /* [in] */ ICanvas* c,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int32 flags,
        /* [in] */ IPaint* p);

    /**
     * Don't call this yourself -- exists for Paint to use internally.
     * {@hide}
     */
    virtual CARAPI MeasureText(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ IPaint* p,
        /* [out] */ Float* width);

    /**
     * Don't call this yourself -- exists for Paint to use internally.
     * {@hide}
     */
    virtual CARAPI GetTextWidths(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ArrayOf<Float>* widths,
        /* [in] */ IPaint* p,
        /* [out] */ Int32* count);

    /**
     * Don't call this yourself -- exists for Paint to use internally.
     * {@hide}
     */
    virtual CARAPI_(Float) GetTextRunAdvances(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Float>* advances,
        /* [in] */ Int32 advancesPos,
        /* [in] */ IPaint* p);

    /**
     * Don't call this yourself -- exists for Paint to use internally.
     * {@hide}
     */
    virtual CARAPI_(Float) GetTextRunAdvances(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Int32 flags,
        /* [in] */ ArrayOf<Float>* advances,
        /* [in] */ Int32 advancesPos,
        /* [in] */ IPaint* p,
        /* [in] */ Int32 reserved);

    /**
     * Returns the next cursor position in the run.  This avoids placing the cursor between
     * surrogates, between characters that form conjuncts, between base characters and combining
     * marks, or within a reordering cluster.
     *
     * <p>The context is the shaping context for cursor movement, generally the bounds of the metric
     * span enclosing the cursor in the direction of movement.
     * <code>contextStart</code>, <code>contextEnd</code> and <code>offset</code> are relative to
     * the start of the string.</p>
     *
     * <p>If cursorOpt is CURSOR_AT and the offset is not a valid cursor position,
     * this returns -1.  Otherwise this will never return a value before contextStart or after
     * contextEnd.</p>
     *
     * @param contextStart the start index of the context
     * @param contextEnd the (non-inclusive) end index of the context
     * @param flags either DIRECTION_RTL or DIRECTION_LTR
     * @param offset the cursor position to move from
     * @param cursorOpt how to move the cursor, one of CURSOR_AFTER,
     * CURSOR_AT_OR_AFTER, CURSOR_BEFORE,
     * CURSOR_AT_OR_BEFORE, or CURSOR_AT
     * @param p the Paint object that is requesting this information
     * @return the offset of the next position, or -1
     * @deprecated This is an internal method, refrain from using it in your code
     */
    //@Deprecated
    virtual CARAPI_(Int32) GetTextRunCursor(
        /* [in] */ Int32 contextStart,
        /* [in] */ Int32 contextEnd,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 cursorOpt,
        /* [in] */ IPaint* p);

    // Documentation from interface
    virtual CARAPI SetFilters(
        /* [in] */ ArrayOf<IInputFilter*>* filters);

    // Documentation from interface
    virtual CARAPI_(AutoPtr< ArrayOf<IInputFilter*> >) GetFilters();

protected:
    /**
     * Create a new SpannableStringBuilder with empty contents
     */
    virtual CARAPI Init();

    /**
     * Create a new SpannableStringBuilder containing a copy of the
     * specified text, including its spans if any.
     */
    virtual CARAPI Init(
        /* [in] */ ICharSequence* text);

    /**
     * Create a new SpannableStringBuilder containing a copy of the
     * specified slice of the specified text, including its spans if any.
     */
    virtual CARAPI Init(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(Boolean) IsSpanEquals(
        /* [in] */ IInterface* lhs,
        /* [in] */ IInterface* rhs);

private:
    CARAPI ResizeFor(
        /* [in] */ Int32 size);

    CARAPI_(void) MoveGapTo(
        /* [in] */ Int32 where);

    CARAPI Change(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ ICharSequence* cs,
        /* [in] */ Int32 csStart,
        /* [in] */ Int32 csEnd);

    CARAPI_(Int32) UpdatedIntervalBound(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 start,
        /* [in] */ Int32 nbNewChars,
        /* [in] */ Int32 flag,
        /* [in] */ Boolean atEnd,
        /* [in] */ Boolean textIsRemoved);

    CARAPI_(void) RemoveSpan(
        /* [in] */ Int32 i);

    static CARAPI_(Boolean) HasNonExclusiveExclusiveSpanAt(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 offset);

    CARAPI_(void) SendToSpanWatchers(
        /* [in] */ Int32 replaceStart,
        /* [in] */ Int32 replaceEnd,
        /* [in] */ Int32 nbNewChars);

    CARAPI_(void) SendBeforeTextChanged(
        /* [in] */ ArrayOf<ITextWatcher*>* watchers,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 after);

    CARAPI_(void) SendTextChanged(
        /* [in] */ ArrayOf<ITextWatcher*>* watchers,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 after);

    CARAPI_(void) SendAfterTextChanged(
        /* [in] */ ArrayOf<ITextWatcher*>* watchers);

    CARAPI_(void) SendSpanAdded(
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(void) SendSpanRemoved(
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI_(void) SendSpanChanged(
        /* [in] */ IInterface* what,
        /* [in] */ Int32 oldStart,
        /* [in] */ Int32 oldEnd,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI_(String) Region(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI CheckRange(
        /* [in] */ const String& operation,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI SetSpan(
        /* [in] */ Boolean send,
        /* [in] */ IInterface* what,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 flags);

    CARAPI CheckRange(
        /* [in] */ ICharSequence* tb,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

private:

    static AutoPtr< ArrayOf<IInputFilter*> > NO_FILTERS;
    AutoPtr< ArrayOf<IInputFilter*> > mFilters;

    AutoPtr< ArrayOf<Char32> > mText;
    Int32 mGapStart;
    Int32 mGapLength;

    AutoPtr< ArrayOf<IInterface*> > mSpans;
    AutoPtr< ArrayOf<Int32> > mSpanStarts;
    AutoPtr< ArrayOf<Int32> > mSpanEnds;
    AutoPtr< ArrayOf<Int32> > mSpanFlags;
    Int32 mSpanCount;
    Int32 mSpanCountBeforeAdd;

    // TODO These value are tightly related to the public SPAN_MARK/POINT values in {@link Spanned}
    static const Int32 MARK = 1;
    static const Int32 POINT = 2;
    static const Int32 PARAGRAPH = 3;

    static const Int32 START_MASK = 0xF0;
    static const Int32 END_MASK = 0x0F;
    static const Int32 START_SHIFT = 4;

    // These bits are not (currently) used by SPANNED flags
    static const Int32 SPAN_START_AT_START = 0x1000;
    static const Int32 SPAN_START_AT_END = 0x2000;
    static const Int32 SPAN_END_AT_START = 0x4000;
    static const Int32 SPAN_END_AT_END = 0x8000;
    static const Int32 SPAN_START_END_MASK = 0xF000;

    static AutoPtr<ICharSequence> EMPTY_CS;
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TEXT_SPANNABLESTRINGBUILDER_H__
