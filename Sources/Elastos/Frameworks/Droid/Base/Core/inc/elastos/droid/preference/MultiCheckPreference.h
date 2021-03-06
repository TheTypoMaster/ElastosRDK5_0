
#ifndef __ELASTOS_DROID_PREFERENCE_CMULTICHECKPREFERENCE_H__
#define __ELASTOS_DROID_PREFERENCE_CMULTICHECKPREFERENCE_H__

#include "DialogPreference.h"

using Elastos::Droid::Content::IDialogInterfaceOnMultiChoiceClickListener;

namespace Elastos {
namespace Droid {
namespace Preference {

/**
 * @hide
* A {@link Preference} that displays a list of entries as
* a dialog which allow the user to toggle each individually on and off.
 *
* @attr ref android.R.styleable#ListPreference_entries
* @attr ref android.R.styleable#ListPreference_entryValues
 */
class MultiCheckPreference
    : public DialogPreference
    , public IDialogPreference
{
private:
    class MultiChoiceClickListener
        : public ElRefBase
        , public IDialogInterfaceOnMultiChoiceClickListener
    {
    public:
        MultiChoiceClickListener(
            /* [in] */ MultiCheckPreference* host);

        CAR_INTERFACE_DECL()

        CARAPI OnClick(
            /* [in] */ IDialogInterface* dialog,
            /* [in] */ Int32 which,
            /* [in] */ Boolean isChecked);

    private:
        MultiCheckPreference* mHost;
    };

public:
    MultiCheckPreference(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    MultiCheckPreference(
        /* [in] */ IContext* context);

    CAR_INTERFACE_DECL()

    /**
     * Sets the human-readable entries to be shown in the list. This will be
     * shown in subsequent dialogs.
     * <p>
     * Each entry must have a corresponding index in
     * {@link #setEntryValues(CharSequence[])}.
     *
     * @param entries The entries.
     * @see #setEntryValues(CharSequence[])
     */
    CARAPI_(void) SetEntries(
        /* [in] */ ArrayOf<ICharSequence*>* entries);

    /**
     * @see #setEntries(CharSequence[])
     * @param entriesResId The entries array as a resource.
     */
    CARAPI_(void) SetEntries(
        /* [in] */ Int32 entriesResId);

    /**
     * The list of entries to be shown in the list in subsequent dialogs.
     *
     * @return The list as an array.
     */
    CARAPI_(AutoPtr< ArrayOf<ICharSequence*> >) GetEntries();

    /**
     * The array to find the value to save for a preference when an entry from
     * entries is selected. If a user clicks on the second item in entries, the
     * second item in this array will be saved to the preference.
     *
     * @param entryValues The array to be used as values to save for the preference.
     */
    CARAPI_(void) SetEntryValues(
        /* [in] */ ArrayOf<String>* entryValues);

    /**
     * @see #setEntryValues(CharSequence[])
     * @param entryValuesResId The entry values array as a resource.
     */
    CARAPI_(void) SetEntryValues(
        /* [in] */ Int32 entryValuesResId);

    /**
     * Returns the array of values to be saved for the preference.
     *
     * @return The array of values.
     */
    CARAPI_(AutoPtr< ArrayOf<String> >) GetEntryValues();

    /**
     * Get the boolean state of a given value.
     */
    CARAPI_(Boolean) GetValue(
        /* [in] */ Int32 index);

    /**
     * Set the boolean state of a given value.
     */
    CARAPI_(void) SetValue(
        /* [in] */ Int32 index,
        /* [in] */ Boolean state);

    /**
     * Sets the current values.
     */
    CARAPI_(void) SetValues(
        /* [in] */ ArrayOf<Boolean>* values);

    /**
     * Returns the summary of this ListPreference. If the summary
     * has a {@linkplain java.lang.String#format String formatting}
     * marker in it (i.e. "%s" or "%1$s"), then the current entry
     * value will be substituted in its place.
     *
     * @return the summary with appropriate string substitution
     */
    CARAPI GetSummary(
        /* [out] */ ICharSequence** summary);

    /**
     * Sets the summary for this Preference with a CharSequence.
     * If the summary has a
     * {@linkplain java.lang.String#format String formatting}
     * marker in it (i.e. "%s" or "%1$s"), then the current entry
     * value will be substituted in its place when it's retrieved.
     *
     * @param summary The summary for the preference.
     */
    CARAPI SetSummary(
        /* [in] */ ICharSequence* summary);

    /**
     * Returns the currently selected values.
     */
    CARAPI_(AutoPtr< ArrayOf<Boolean> >) GetValues();

    /**
     * Returns the index of the given value (in the entry values array).
     *
     * @param value The value whose index should be returned.
     * @return The index of the value, or -1 if not found.
     */
    CARAPI_(Int32) FindIndexOfValue(
        /* [in] */ const String& value);

    CARAPI SetDialogTitle(
        /* [in] */ ICharSequence* dialogTitle);

    CARAPI SetDialogTitle(
        /* [in] */ Int32 dialogTitleResId);

    CARAPI GetDialogTitle(
        /* [out] */ ICharSequence** title);

    CARAPI SetDialogMessage(
        /* [in] */ ICharSequence* dialogMessage);

    CARAPI SetDialogMessage(
        /* [in] */ Int32 dialogMessageResId);

    CARAPI GetDialogMessage(
        /* [out] */ ICharSequence** message);

    CARAPI SetDialogIcon(
        /* [in] */ IDrawable* dialogIcon);

    CARAPI SetDialogIcon(
        /* [in] */ Int32 dialogIconRes);

    CARAPI GetDialogIcon(
        /* [out] */ IDrawable** icon);

    CARAPI SetPositiveButtonText(
        /* [in] */ ICharSequence* positiveButtonText);

    CARAPI SetPositiveButtonText(
        /* [in] */ Int32 positiveButtonTextResId);

    CARAPI GetPositiveButtonText(
        /* [out] */ ICharSequence** text);

    CARAPI SetNegativeButtonText(
        /* [in] */ ICharSequence* negativeButtonText);

    CARAPI SetNegativeButtonText(
        /* [in] */ Int32 negativeButtonTextResId);

    CARAPI GetNegativeButtonText(
        /* [out] */ ICharSequence** text);

    CARAPI SetDialogLayoutResource(
        /* [in] */ Int32 dialogLayoutResId);

    CARAPI GetDialogLayoutResource(
        /* [out] */ Int32* layoutResId);

    CARAPI ShowDialog(
        /* [in] */ IBundle* state);

    CARAPI NeedInputMethod(
        /* [out] */ Boolean* isNeed);

    CARAPI OnCreateDialogView(
        /* [out] */ IView** view);

    CARAPI GetDialog(
        /* [out] */ IDialog** dialog);

protected:
    CARAPI OnPrepareDialogBuilder(
        /* [in] */ IAlertDialogBuilder* builder);

    CARAPI OnDialogClosed(
        /* [in] */ Boolean positiveResult);

    CARAPI OnGetDefaultValue(
        /* [in] */ ITypedArray* a,
        /* [in] */ Int32 index,
        /* [out] */ IInterface** value);

    CARAPI OnSetInitialValue(
        /* [in] */ Boolean restorePersistedValue,
        /* [in] */ IInterface* defaultValue);

    CARAPI OnSaveInstanceState(
        /* [out] */ IParcelable** state);

    CARAPI OnRestoreInstanceState(
        /* [in] */ IParcelable* state);

private:
    CARAPI_(void) Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(void) SetEntryValuesCS(
        /* [in] */ ArrayOf<ICharSequence*>* values);

private:
    AutoPtr< ArrayOf<ICharSequence*> > mEntries;
    AutoPtr< ArrayOf<String> > mEntryValues;
    AutoPtr< ArrayOf<Boolean> > mSetValues;
    AutoPtr< ArrayOf<Boolean> > mOrigValues;
    String mSummary;
};

}
}
}

#endif // __ELASTOS_DROID_PREFERENCE_CMULTICHECKPREFERENCE_H__
