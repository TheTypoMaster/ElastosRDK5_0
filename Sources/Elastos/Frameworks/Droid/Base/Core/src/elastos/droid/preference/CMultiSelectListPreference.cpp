
#include "CMultiSelectListPreference.h"
#include "CMultiSelectListPreferenceSavedState.h"
#include "R.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::CString;
using Elastos::Utility::CHashSet;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Content::EIID_IDialogInterfaceOnMultiChoiceClickListener;
using Elastos::Droid::Content::Res::IResources;


namespace Elastos {
namespace Droid {
namespace Preference {

/////////////////////////////////////////////////////
// CMultiSelectListPreference::MultiChoiceClickListener
/////////////////////////////////////////////////////

CMultiSelectListPreference::MultiChoiceClickListener::MultiChoiceClickListener(
    /* [in] */ CMultiSelectListPreference* host)
    : mHost(host)
{}

CAR_INTERFACE_IMPL(CMultiSelectListPreference::MultiChoiceClickListener, IDialogInterfaceOnMultiChoiceClickListener)

ECode CMultiSelectListPreference::MultiChoiceClickListener::OnClick(
    /* [in] */ IDialogInterface* dialog,
    /* [in] */ Int32 which,
    /* [in] */ Boolean isChecked)
{
    String entryValue;
    ((*mHost->mEntryValues)[which])->ToString(&entryValue);
    if (isChecked) {
        mHost->mNewValues.Insert(entryValue);
        mHost->mPreferenceChanged |= TRUE;
        return NOERROR;
    }
    else {
        Set<String>::Iterator itr = mHost->mNewValues.Find(entryValue);
        if (itr !=  mHost->mNewValues.End()) {
            mHost->mNewValues.Erase(itr);
            mHost->mPreferenceChanged |= TRUE;
            return NOERROR;
        }
        mHost->mPreferenceChanged |= FALSE;
        return NOERROR;
    }
}


/////////////////////////////////////////////////////
// CMultiSelectListPreference
/////////////////////////////////////////////////////

CMultiSelectListPreference::CMultiSelectListPreference()
    : mPreferenceChanged(FALSE)
{}

PInterface CMultiSelectListPreference::Probe(
    /* [in] */REIID riid)
{
    if (riid == EIID_IMultiSelectListPreference) {
        return (IMultiSelectListPreference*)this;
    }
    else if (riid == EIID_IDialogPreference) {
        return (IDialogPreference*)this;
    }

    return DialogPreference::Probe(riid);
}

UInt32 CMultiSelectListPreference::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 CMultiSelectListPreference::Release()
{
    return ElRefBase::Release();
}

ECode CMultiSelectListPreference::GetInterfaceID(
    /* [in] */ IInterface* pObject,
    /* [out] */ InterfaceID* pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface *)(IMultiSelectListPreference*)this) {
        *pIID = EIID_IMultiSelectListPreference;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IDialogPreference*)this) {
        *pIID = EIID_IDialogPreference;
        return NOERROR;
    }
    return DialogPreference::GetInterfaceID(pObject, pIID);
}

ECode CMultiSelectListPreference::SetEntries(
    /* [in] */ ArrayOf<ICharSequence*>* entries)
{
    mEntries = entries;
    return NOERROR;
}

ECode CMultiSelectListPreference::SetEntries(
    /* [in] */ Int32 entriesResId)
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    AutoPtr< ArrayOf<ICharSequence*> > entries;
    resources->GetTextArray(entriesResId, (ArrayOf<ICharSequence*>**)&entries);
    return NOERROR;
}

ECode CMultiSelectListPreference::GetEntries(
    /* [out, callee] */ ArrayOf<ICharSequence*>** entries)
{
    VALIDATE_NOT_NULL(entries)
    *entries = mEntries;
    REFCOUNT_ADD(*entries)
    return NOERROR;
}

ECode CMultiSelectListPreference::SetEntryValues(
    /* [in] */ ArrayOf<ICharSequence*>* entryValues)
{
    mEntryValues = entryValues;
    return NOERROR;
}

ECode CMultiSelectListPreference::SetEntryValues(
    /* [in] */ Int32 entryValuesResId)
{
    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    AutoPtr<ArrayOf<ICharSequence*> > entries;
    resources->GetTextArray(entryValuesResId, (ArrayOf<ICharSequence*>**)&entries);
    return SetEntryValues(entries);
}

ECode CMultiSelectListPreference::GetEntryValues(
    /* [out, callee] */ ArrayOf<ICharSequence*>** entryValues)
{
    VALIDATE_NOT_NULL(entryValues)
    *entryValues = mEntryValues;
    REFCOUNT_ADD(*entryValues)
    return NOERROR;
}

ECode CMultiSelectListPreference::SetValues(
    /* [in] */ ISet* values)
{
    mValues.Clear();
    AutoPtr< ArrayOf<IInterface*> > valuesArray;
    values->ToArray((ArrayOf<IInterface*>**)&valuesArray);
    for (Int32 i = 0; i < valuesArray->GetLength(); ++i) {
        AutoPtr<ICharSequence> cs = ICharSequence::Probe((*valuesArray)[i]);
        String str;
        cs->ToString(&str);
        mValues.Insert(str);
    }

    Boolean result;
    return PersistStringSet(values, &result);
}

ECode CMultiSelectListPreference::GetValues(
    /* [out] */ ISet** result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<ISet> v;
    CHashSet::New((ISet**)&v);
    Set<String>::Iterator it = mValues.Begin();
    for (; it != mValues.End(); ++it) {
        AutoPtr<ICharSequence> cs;
        CString::New(*it, (ICharSequence**)&cs);
        Boolean isSuccess;
        v->Add(cs, &isSuccess);
    }
    *result = v;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

ECode CMultiSelectListPreference::FindIndexOfValue(
    /* [in] */ const String& value,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)

    if (!value.IsNull() && mEntryValues != NULL) {
        for (Int32 i = mEntryValues->GetLength() - 1; i >= 0; i--) {
            String entryValue;
            ((*mEntryValues)[i])->ToString(&entryValue);
            if (entryValue.Equals(value)) {
                *index = i;
                return NOERROR;
            }
        }
    }
    *index = -1;
    return NOERROR;
}

ECode CMultiSelectListPreference::OnPrepareDialogBuilder(
    /* [in] */ IAlertDialogBuilder* builder)
{
    FAIL_RETURN(DialogPreference::OnPrepareDialogBuilder(builder))

    if (mEntries == NULL || mEntryValues == NULL) {
        Logger::E("CMultiSelectListPreference", "MultiSelectListPreference requires an entries array and an entryValues array.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    AutoPtr< ArrayOf<Boolean> > checkedItems = GetSelectedItems();

    AutoPtr<IDialogInterfaceOnMultiChoiceClickListener> listener = new MultiChoiceClickListener(this);
    builder->SetMultiChoiceItems(mEntries, checkedItems, listener);

    mNewValues.Clear();
    mNewValues.Insert(mValues.Begin(), mValues.End());
    return NOERROR;
}

AutoPtr< ArrayOf<Boolean> > CMultiSelectListPreference::GetSelectedItems()
{
    AutoPtr< ArrayOf<ICharSequence*> > entries = mEntryValues;
    Int32 entryCount = mEntryValues->GetLength();
    Set<String> values(mValues);
    AutoPtr< ArrayOf<Boolean> > result = ArrayOf<Boolean>::Alloc(entryCount);

    for (Int32 i = 0; i < entryCount; i++) {
        String entryValue;
        ((*entries)[i])->ToString(&entryValue);
        Set<String>::Iterator itr = values.Find(entryValue);
        (*result)[i] = itr != values.End() ? TRUE : FALSE;
    }

    return result;
}

ECode CMultiSelectListPreference::OnDialogClosed(
    /* [in] */ Boolean positiveResult)
{
    FAIL_RETURN(DialogPreference::OnDialogClosed(positiveResult))

    if (positiveResult && mPreferenceChanged) {
        AutoPtr<ISet> values;
        CHashSet::New((ISet**)&values);
        Set<String>::Iterator it = mNewValues.Begin();
        for (; it != mNewValues.End(); ++it) {
            AutoPtr<ICharSequence> cs;
            CString::New(*it, (ICharSequence**)&cs);
            Boolean isSuccess;
            values->Add(cs, &isSuccess);
        }
        Boolean result;
        if (CallChangeListener(values, &result), result) {
            SetValues(values);
        }
    }
    mPreferenceChanged = FALSE;
    return NOERROR;
}

ECode CMultiSelectListPreference::OnGetDefaultValue(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 index,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value)

    AutoPtr< ArrayOf<ICharSequence*> > defaultValues;
    a->GetTextArray(index, (ArrayOf<ICharSequence*>**)&defaultValues);
    Int32 valueCount = defaultValues->GetLength();
    AutoPtr<ISet> result;
    CHashSet::New((ISet**)&result);
    for (Int32 i = 0; i < valueCount; i++) {
        Boolean isSuccess;
        result->Add((*defaultValues)[i], &isSuccess);
    }
    *value = result;
    REFCOUNT_ADD(*value)
    return NOERROR;
}

ECode CMultiSelectListPreference::OnSetInitialValue(
    /* [in] */ Boolean restoreValue,
    /* [in] */ IInterface* defaultValue)
{
    AutoPtr<ISet> values;
    if (restoreValue) {
        AutoPtr<ISet> valuesSet;
        CHashSet::New((ISet**)&valuesSet);
        Set<String>::Iterator it = mValues.Begin();
        for (; it != mValues.End(); ++it) {
            AutoPtr<ICharSequence> cs;
            CString::New(*it, (ICharSequence**)&cs);
            Boolean isSuccess;
            valuesSet->Add(cs, &isSuccess);
        }
        GetPersistedStringSet(valuesSet, (ISet**)&values);
    }
    else {
        values = ISet::Probe(defaultValue);
    }
    return SetValues(values);
}

ECode CMultiSelectListPreference::OnSaveInstanceState(
    /* [out] */ IParcelable** state)
{
    VALIDATE_NOT_NULL(state)

    AutoPtr<IParcelable> superState;
    DialogPreference::OnSaveInstanceState((IParcelable**)&superState);
    Boolean isPersistent;
    if (IsPersistent(&isPersistent), isPersistent) {
        // No need to save instance state
        *state = superState;
        REFCOUNT_ADD(*state)
        return NOERROR;
    }

    AutoPtr<IMultiSelectListPreferenceSavedState> myState;
    CMultiSelectListPreferenceSavedState::New(superState, (IMultiSelectListPreferenceSavedState**)&myState);
    AutoPtr<ISet> values;
    GetValues((ISet**)&values);
    myState->SetValues(values);
    *state = IParcelable::Probe(myState);
    REFCOUNT_ADD(*state)
    return NOERROR;
}

ECode CMultiSelectListPreference::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    DialogPreference::Init(context, attrs);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::MultiSelectListPreference),
            ARRAY_SIZE(R::styleable::MultiSelectListPreference));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, 0, 0, (ITypedArray**)&a);
    a->GetTextArray(R::styleable::MultiSelectListPreference_entries, (ArrayOf<ICharSequence*>**)&mEntries);
    a->GetTextArray(R::styleable::MultiSelectListPreference_entryValues, (ArrayOf<ICharSequence*>**)&mEntryValues);
    a->Recycle();

    return NOERROR;
}

ECode CMultiSelectListPreference::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode CMultiSelectListPreference::SetDialogTitle(
    /* [in] */ ICharSequence* dialogTitle)
{
    return DialogPreference::SetDialogTitle(dialogTitle);
}

ECode CMultiSelectListPreference::SetDialogTitle(
    /* [in] */ Int32 dialogTitleResId)
{
    return DialogPreference::SetDialogTitle(dialogTitleResId);
}

ECode CMultiSelectListPreference::GetDialogTitle(
    /* [out] */ ICharSequence** title)
{
    return DialogPreference::GetDialogTitle(title);
}

ECode CMultiSelectListPreference::SetDialogMessage(
    /* [in] */ ICharSequence* dialogMessage)
{
    return DialogPreference::SetDialogMessage(dialogMessage);
}

ECode CMultiSelectListPreference::SetDialogMessage(
    /* [in] */ Int32 dialogMessageResId)
{
    return DialogPreference::SetDialogMessage(dialogMessageResId);
}

ECode CMultiSelectListPreference::GetDialogMessage(
    /* [out] */ ICharSequence** message)
{
    return DialogPreference::GetDialogMessage(message);
}

ECode CMultiSelectListPreference::SetDialogIcon(
    /* [in] */ IDrawable* dialogIcon)
{
    return DialogPreference::SetDialogIcon(dialogIcon);
}

ECode CMultiSelectListPreference::SetDialogIcon(
    /* [in] */ Int32 dialogIconRes)
{
    return DialogPreference::SetDialogIcon(dialogIconRes);
}

ECode CMultiSelectListPreference::GetDialogIcon(
    /* [out] */ IDrawable** icon)
{
    return DialogPreference::GetDialogIcon(icon);
}

ECode CMultiSelectListPreference::SetPositiveButtonText(
    /* [in] */ ICharSequence* positiveButtonText)
{
    return DialogPreference::SetPositiveButtonText(positiveButtonText);
}

ECode CMultiSelectListPreference::SetPositiveButtonText(
    /* [in] */ Int32 positiveButtonTextResId)
{
    return DialogPreference::SetPositiveButtonText(positiveButtonTextResId);
}

ECode CMultiSelectListPreference::GetPositiveButtonText(
    /* [out] */ ICharSequence** text)
{
    return DialogPreference::GetPositiveButtonText(text);
}

ECode CMultiSelectListPreference::SetNegativeButtonText(
    /* [in] */ ICharSequence* negativeButtonText)
{
    return DialogPreference::SetNegativeButtonText(negativeButtonText);
}

ECode CMultiSelectListPreference::SetNegativeButtonText(
    /* [in] */ Int32 negativeButtonTextResId)
{
    return DialogPreference::SetNegativeButtonText(negativeButtonTextResId);
}

ECode CMultiSelectListPreference::GetNegativeButtonText(
    /* [out] */ ICharSequence** text)
{
    return DialogPreference::GetNegativeButtonText(text);
}

ECode CMultiSelectListPreference::SetDialogLayoutResource(
    /* [in] */ Int32 dialogLayoutResId)
{
    return DialogPreference::SetDialogLayoutResource(dialogLayoutResId);
}

ECode CMultiSelectListPreference::GetDialogLayoutResource(
    /* [out] */ Int32* layoutResId)
{
    return DialogPreference::GetDialogLayoutResource(layoutResId);
}

ECode CMultiSelectListPreference::ShowDialog(
    /* [in] */ IBundle* state)
{
    return DialogPreference::ShowDialog(state);
}

ECode CMultiSelectListPreference::NeedInputMethod(
    /* [out] */ Boolean* isNeed)
{
    return DialogPreference::NeedInputMethod(isNeed);
}

ECode CMultiSelectListPreference::OnCreateDialogView(
    /* [out] */ IView** view)
{
    return DialogPreference::OnCreateDialogView(view);
}

ECode CMultiSelectListPreference::OnBindDialogView(
    /* [in] */ IView* view)
{
    return DialogPreference::OnBindDialogView(view);
}

ECode CMultiSelectListPreference::GetDialog(
    /* [out] */ IDialog** dialog)
{
    return DialogPreference::GetDialog(dialog);
}

}
}
}

