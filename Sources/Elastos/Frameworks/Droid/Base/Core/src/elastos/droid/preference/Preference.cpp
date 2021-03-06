
#include "Preference.h"
#include "os/CBundle.h"
#include "text/TextUtils.h"
#include "view/CAbsSavedStateHelper.h"
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/Math.h>
#include <elastos/core/StringBuilder.h>

using Elastos::Core::EIID_IComparable;
using Elastos::Core::StringBuilder;
using Elastos::Core::Math;
using Elastos::Core::CString;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::View::IAbsSavedState;
using Elastos::Droid::View::CAbsSavedStateHelper;
using Elastos::Droid::View::IAbsSavedStateHelper;

namespace Elastos {
namespace Droid {
namespace Preference {

// {373C1451-FE12-4D41-883B-CC603F142FAD}
extern "C" const InterfaceID EIID_Preference =
        { 0x373c1451, 0xfe12, 0x4d41, { 0x88, 0x3b, 0xcc, 0x60, 0x3f, 0x14, 0x2f, 0xad } };

Preference::Preference()
    : mId(0)
    , mOrder(IPreference::DEFAULT_ORDER)
    , mTitleRes(0)
    , mIconResId(0)
    , mEnabled(TRUE)
    , mSelectable(TRUE)
    , mRequiresKey(FALSE)
    , mPersistent(TRUE)
    , mDependencyMet(TRUE)
    , mShouldDisableView(TRUE)
    , mLayoutResId(R::layout::preference)
    , mWidgetLayoutResId(0)
    , mHasSpecifiedLayout(FALSE)
    , mBaseMethodCalled(FALSE)
{}

Preference::~Preference()
{
    if (mDependents != NULL) {
        mDependents->Clear();
        mDependents = NULL;
    }
}

PInterface Preference::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)(IPreference*)this;
    }
    else if (riid == EIID_IObject) {
        return (IObject*)this;
    }
    else if (riid == EIID_IPreference) {
        return (IPreference*)this;
    }
    else if (riid == EIID_IComparable) {
        return (IComparable*)this;
    }
    else if (riid == EIID_IOnDependencyChangeListener) {
        return (IOnDependencyChangeListener*)this;
    }
    else if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource*)this;
    }
    else if (riid == EIID_Preference) {
        return reinterpret_cast<PInterface>(this);
    }
    return NULL;
}

UInt32 Preference::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 Preference::Release()
{
    return ElRefBase::Release();
}

ECode Preference::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    VALIDATE_NOT_NULL(pIID)

    if (pObject == (IInterface *)(IPreference *)this) {
        *pIID = EIID_IPreference;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IObject *)this) {
        *pIID = EIID_IObject;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IComparable *)this) {
        *pIID = EIID_IComparable;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IOnDependencyChangeListener *)this) {
        *pIID = EIID_IOnDependencyChangeListener;
        return NOERROR;
    }
    else if (pObject == (IInterface *)(IWeakReferenceSource *)this) {
        *pIID = EIID_IWeakReferenceSource;
        return NOERROR;
    }

    return E_INVALID_ARGUMENT;
}

ECode Preference::Initialize()
{
    return NOERROR;
}

ECode Preference::Aggregate(
    /* [in] */ AggrType aggrType,
    /* [in] */ PInterface pObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode Preference::GetDomain(
    /* [out] */ PInterface *ppObject)
{
    return E_NOT_IMPLEMENTED;
}

ECode Preference::GetClassID(
    /* [out] */ ClassID *pCLSID)
{
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode Preference::Equals(
    /* [in] */ IInterface* other,
    /* [out] */ Boolean * result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    VALIDATE_NOT_NULL(other)

    IPreference* o = IPreference::Probe(other);
    if (o != NULL) {
        *result = (o == THIS_PROBE(IPreference));
    }
    return NOERROR;
}

ECode Preference::GetHashCode(
    /* [out] */ Int32* hash)
{
    VALIDATE_NOT_NULL(hash);
    *hash = (Int32)THIS_PROBE(IPreference);
    return NOERROR;
}

ECode Preference::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(THIS_PROBE(IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

void Preference::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    mContext = context;

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::Preference),
            ARRAY_SIZE(R::styleable::Preference));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, defStyle, 0, (ITypedArray**)&a);

    Int32 indexCount;
    a->GetIndexCount(&indexCount);
    for (Int32 i = indexCount; i >= 0; i--) {
        Int32 attr;
        a->GetIndex(i, &attr);
        switch (attr) {
            case R::styleable::Preference_icon:
                a->GetResourceId(attr, 0, &mIconResId);
                break;

            case R::styleable::Preference_key:
                a->GetString(attr, &mKey);
                break;

            case R::styleable::Preference_title:
            {
                a->GetResourceId(attr, 0, &mTitleRes);
                String str;
                a->GetString(attr, &str);
                CString::New(str, (ICharSequence**)&mTitle);
                break;
            }
            case R::styleable::Preference_summary:
            {
                String str;
                a->GetString(attr, &str);
                CString::New(str, (ICharSequence**)&mSummary);
                break;
            }
            case R::styleable::Preference_order:
                a->GetInt32(attr, mOrder, &mOrder);
                break;

            case R::styleable::Preference_fragment:
                a->GetString(attr, &mFragment);
                break;

            case R::styleable::Preference_layout:
                a->GetResourceId(attr, mLayoutResId, &mLayoutResId);
                break;

            case R::styleable::Preference_widgetLayout:
                a->GetResourceId(attr, mWidgetLayoutResId, &mWidgetLayoutResId);
                break;

            case R::styleable::Preference_enabled:
                a->GetBoolean(attr, TRUE, &mEnabled);
                break;

            case R::styleable::Preference_selectable:
                a->GetBoolean(attr, TRUE, &mSelectable);
                break;

            case R::styleable::Preference_persistent:
                a->GetBoolean(attr, mPersistent, &mPersistent);
                break;

            case R::styleable::Preference_dependency:
                a->GetString(attr, &mDependencyKey);
                break;

            case R::styleable::Preference_defaultValue:
                OnGetDefaultValue(a, attr, (IInterface**)&mDefaultValue);
                break;

            case R::styleable::Preference_shouldDisableView:
                a->GetBoolean(attr, mShouldDisableView, &mShouldDisableView);
                break;
        }
    }

    a->Recycle();

    // ClassID clsId;
    // GetClassID(&clsId);
    // String clsName(clsId.pUunm);
    // if (!clsName.StartWith("Elastos.Droid.Core.eco")) {
    //     // For subclasses not in this package, assume the worst and don't cache views
    //     mHasSpecifiedLayout = TRUE;
    // }
}

ECode Preference::OnGetDefaultValue(
    /* [in] */ ITypedArray* a,
    /* [in] */ Int32 index,
    /* [out] */ IInterface** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;
    return NOERROR;
}

ECode Preference::SetIntent(
    /* [in] */ IIntent* intent)
{
    mIntent = intent;
    return NOERROR;
}

ECode Preference::GetIntent(
    /* [out] */ IIntent** intent)
{
    VALIDATE_NOT_NULL(intent)
    *intent = mIntent;
    REFCOUNT_ADD(*intent)
    return NOERROR;
}

ECode Preference::SetFragment(
    /* [in] */ const String& fragment)
{
    mFragment = fragment;
    return NOERROR;
}

ECode Preference::GetFragment(
    /* [out] */ String* fragment)
{
    VALIDATE_NOT_NULL(fragment)
    *fragment = mFragment;
    return NOERROR;
}

ECode Preference::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras)

    if (mExtras == NULL) {
        CBundle::New((IBundle**)&mExtras);
    }
    *extras = mExtras;
    REFCOUNT_ADD(*extras)
    return NOERROR;
}

ECode Preference::PeekExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras)
    *extras = mExtras;
    REFCOUNT_ADD(*extras)
    return NOERROR;
}

ECode Preference::SetLayoutResource(
    /* [in] */ Int32 layoutResId)
{
    if (layoutResId != mLayoutResId) {
        // Layout changed
        mHasSpecifiedLayout = TRUE;
    }

    mLayoutResId = layoutResId;
    return NOERROR;
}

ECode Preference::GetLayoutResource(
    /* [out] */ Int32* layoutResId)
{
    VALIDATE_NOT_NULL(layoutResId)
    *layoutResId = mLayoutResId;
    return NOERROR;
}

ECode Preference::SetWidgetLayoutResource(
    /* [in] */ Int32 widgetLayoutResId)
{
    if (widgetLayoutResId != mWidgetLayoutResId) {
        // Layout changed
        mHasSpecifiedLayout = TRUE;
    }
    mWidgetLayoutResId = widgetLayoutResId;
    return NOERROR;
}

ECode Preference::GetWidgetLayoutResource(
    /* [out] */ Int32* widgetLayoutResId)
{
    VALIDATE_NOT_NULL(widgetLayoutResId)
    *widgetLayoutResId = mWidgetLayoutResId;
    return NOERROR;
}

ECode Preference::GetView(
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view)
    if (convertView == NULL) {
        OnCreateView(parent, (IView**)&convertView);
    }
    OnBindView(convertView);
    *view = convertView;
    REFCOUNT_ADD(*view)
    return NOERROR;
}

ECode Preference::OnCreateView(
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view)
    AutoPtr<ILayoutInflater> layoutInflater;
    mContext->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&layoutInflater);

    AutoPtr<IView> layout;
    layoutInflater->Inflate(mLayoutResId, parent, FALSE, (IView**)&layout);

    AutoPtr<IView> v;
    layout->FindViewById(R::id::widget_frame, (IView**)&v);
    AutoPtr<IViewGroup> widgetFrame = IViewGroup::Probe(v);
    if (widgetFrame != NULL) {
        if (mWidgetLayoutResId != 0) {
            AutoPtr<IView> t;
            layoutInflater->Inflate(mWidgetLayoutResId, widgetFrame, (IView**)&t);
        }
        else {
            widgetFrame->SetVisibility(IView::GONE);
        }
    }
    *view = layout;
    REFCOUNT_ADD(*view)
    return NOERROR;
}

ECode Preference::OnBindView(
    /* [in] */ IView* view)
{
    Slogger::E("Preference", "~~~~~~~~~~~~~~~~~~~~~OnBindView");
    AutoPtr<ITextView> titleView;
    view->FindViewById(R::id::title, (IView**)&titleView);
    if (titleView != NULL) {
        AutoPtr<ICharSequence> title;
        GetTitle((ICharSequence**)&title);
        if (!TextUtils::IsEmpty(title)) {
            titleView->SetText(title);
            titleView->SetVisibility(IView::VISIBLE);
        }
        else {
            titleView->SetVisibility(IView::GONE);
        }
    }

    AutoPtr<ITextView> summaryView;
    view->FindViewById(R::id::summary, (IView**)&summaryView);
    if (summaryView != NULL) {
        AutoPtr<ICharSequence> summary;
        GetSummary((ICharSequence**)&summary);
        if (!TextUtils::IsEmpty(summary)) {
            summaryView->SetText(summary);
            summaryView->SetVisibility(IView::VISIBLE);
        }
        else {
            summaryView->SetVisibility(IView::GONE);
        }
    }

    AutoPtr<IImageView> imageView;
    view->FindViewById(R::id::icon, (IView**)&imageView);
    if (imageView != NULL) {
        if (mIconResId != 0 || mIcon != NULL) {
            if (mIcon == NULL) {
                AutoPtr<IContext> ctx;
                GetContext((IContext**)&ctx);
                AutoPtr<IResources> res;
                ctx->GetResources((IResources**)&res);
                res->GetDrawable(mIconResId, (IDrawable**)&mIcon);
            }
            if (mIcon != NULL) {
                imageView->SetImageDrawable(mIcon);
            }
        }
        imageView->SetVisibility(mIcon != NULL ? IView::VISIBLE : IView::GONE);
    }

    if (mShouldDisableView) {
        Slogger::E("Preference", "~~~~~~~~~~~~~~~~~~~~~OnBindView, oh no");
        Boolean enabled;
        IsEnabled(&enabled);
        SetEnabledStateOnViews(view, enabled);
    }

    return NOERROR;
}

void Preference::SetEnabledStateOnViews(
    /* [in] */ IView* v,
    /* [in] */ Boolean enabled)
{
    v->SetEnabled(enabled);

    AutoPtr<IViewGroup> viewGroup= IViewGroup::Probe(v);
    if (viewGroup != NULL) {
        Int32 childCount;
        viewGroup->GetChildCount(&childCount);
        for (Int32 i = childCount -1; i >= 0; i--) {
            AutoPtr<IView> childView;
            viewGroup->GetChildAt(i, (IView**)&childView);
            SetEnabledStateOnViews(childView, enabled);
        }
    }
}

ECode Preference::SetOrder(
    /* [in] */ Int32 order)
{
    if (order != mOrder) {
        mOrder = order;

        // Reorder the list
        NotifyHierarchyChanged();
    }
    return NOERROR;
}

ECode Preference::GetOrder(
    /* [out] */ Int32* order)
{
    VALIDATE_NOT_NULL(order)
    *order = mOrder;
    return NOERROR;
}

ECode Preference::SetTitle(
    /* [in] */ ICharSequence* title)
{
    if ((title == NULL && mTitle != NULL) || (title != NULL && mTitle == NULL)) {
        mTitleRes = 0;
        mTitle = title;
        NotifyChanged();
    }
    else {
        AutoPtr<IObject> obj = IObject::Probe(title);
        Boolean isEquals;
        if (obj != NULL && (obj->Equals(mTitle, &isEquals), !isEquals)) {
            mTitleRes = 0;
            mTitle = title;
            NotifyChanged();
        }
    }
    return NOERROR;
}

ECode Preference::SetTitle(
    /* [in] */ Int32 titleResId)
{
    String title;
    mContext->GetString(titleResId, &title);
    AutoPtr<ICharSequence> cs;
    CString::New(title, (ICharSequence**)&cs);
    SetTitle(cs);
    mTitleRes = titleResId;
    return NOERROR;
}

ECode Preference::GetTitleRes(
    /* [out] */ Int32* resId)
{
    VALIDATE_NOT_NULL(resId)
    *resId = mTitleRes;
    return NOERROR;
}

ECode Preference::GetTitle(
    /* [out] */ ICharSequence** title)
{
    VALIDATE_NOT_NULL(title)
    *title = mTitle;
    REFCOUNT_ADD(*title)
    return NOERROR;
}

ECode Preference::SetIcon(
    /* [in] */ IDrawable* icon)
{
    if ((icon == NULL && mIcon != NULL) || (icon != NULL && mIcon.Get() != icon)) {
        mIcon = icon;
        NotifyChanged();
    }
    return NOERROR;
}

ECode Preference::SetIcon(
    /* [in] */ Int32 iconResId)
{
    mIconResId = iconResId;
    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);
    AutoPtr<IDrawable> icon;
    resources->GetDrawable(iconResId, (IDrawable**)&icon);
    return SetIcon(icon);
}

ECode Preference::GetIcon(
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(drawable)
    *drawable = mIcon;
    REFCOUNT_ADD(*drawable)
    return NOERROR;
}

ECode Preference::GetSummary(
    /* [out] */ ICharSequence** summary)
{
    VALIDATE_NOT_NULL(summary)
    *summary = mSummary;
    REFCOUNT_ADD(*summary)
    return NOERROR;
}

ECode Preference::SetSummary(
    /* [in] */ ICharSequence* summary)
{
    if ((summary == NULL && mSummary != NULL) || (summary != NULL && mSummary == NULL)) {
        mSummary = summary;
        NotifyChanged();
    }
    else {
        AutoPtr<IObject> obj = IObject::Probe(summary);
        Boolean isEquals;
        if (obj != NULL && (obj->Equals(mSummary, &isEquals), !isEquals)) {
            mSummary = summary;
            NotifyChanged();
        }
    }
    return NOERROR;
}

ECode Preference::SetSummary(
    /* [in] */ Int32 summaryResId)
{
    String summary;
    mContext->GetString(summaryResId, &summary);
    AutoPtr<ICharSequence> cs;
    CString::New(summary, (ICharSequence**)&cs);
    return SetSummary(cs);
}

ECode Preference::SetEnabled(
    /* [in] */ Boolean enabled)
{
    if (mEnabled != enabled) {
        mEnabled = enabled;

        // Enabled state can change dependent preferences' states, so notify
        Boolean shouldDisableDependents;
        ShouldDisableDependents(&shouldDisableDependents);
        NotifyDependencyChange(shouldDisableDependents);
        NotifyChanged();
    }
    return NOERROR;
}

ECode Preference::IsEnabled(
    /* [out] */ Boolean* isEnable)
{
    VALIDATE_NOT_NULL(isEnable)
    *isEnable = mEnabled && mDependencyMet;
    return NOERROR;
}

ECode Preference::SetSelectable(
    /* [in] */ Boolean selectable)
{
    if (mSelectable != selectable) {
        mSelectable = selectable;
        NotifyChanged();
    }
    return NOERROR;
}

ECode Preference::IsSelectable(
    /* [out] */ Boolean* isSelectable)
{
    VALIDATE_NOT_NULL(isSelectable)
    *isSelectable = mSelectable;
    return NOERROR;
}

ECode Preference::SetShouldDisableView(
    /* [in] */ Boolean shouldDisableView)
{
    mShouldDisableView = shouldDisableView;
    NotifyChanged();
    return NOERROR;
}

ECode Preference::GetShouldDisableView(
    /* [out] */ Boolean* shouldDisableView)
{
    VALIDATE_NOT_NULL(shouldDisableView)
    *shouldDisableView = mShouldDisableView;
    return NOERROR;
}

Int64 Preference::GetId()
{
    return mId;
}

ECode Preference::OnClick()
{
    return NOERROR;
}

ECode Preference::SetKey(
    /* [in] */ const String& key)
{
    mKey = key;
    Boolean hasKey;
    if (mRequiresKey && (HasKey(&hasKey), !hasKey)) {
        return RequireKey();
    }
    return NOERROR;
}

ECode Preference::GetKey(
    /* [out] */ String* key)
{
    VALIDATE_NOT_NULL(key)
    *key = mKey;
    return NOERROR;
}

ECode Preference::RequireKey()
{
    if (mKey.IsNull()) {
//        throw new IllegalStateException("Preference does not have a key assigned.");
        Slogger::E("Preference", "Preference does not have a key assigned.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mRequiresKey = TRUE;
    return NOERROR;
}

ECode Preference::HasKey(
    /* [out] */ Boolean* hasKey)
{
    VALIDATE_NOT_NULL(hasKey)
    *hasKey = !mKey.IsNullOrEmpty();
    return NOERROR;
}

ECode Preference::IsPersistent(
    /* [out] */ Boolean* isPersistent)
{
    VALIDATE_NOT_NULL(isPersistent)
    *isPersistent = mPersistent;
    return NOERROR;
}

ECode Preference::ShouldPersist(
    /* [out] */ Boolean* shouldPersist)
{
    VALIDATE_NOT_NULL(shouldPersist)
    Boolean isPersistent, hasKey;
    *shouldPersist = mPreferenceManager != NULL && (IsPersistent(&isPersistent), isPersistent) && (HasKey(&hasKey), hasKey);
    return NOERROR;
}

ECode Preference::SetPersistent(
    /* [in] */ Boolean persistent)
{
    mPersistent = persistent;
    return NOERROR;
}

ECode Preference::CallChangeListener(
    /* [in] */ IInterface* newValue,
    /* [out] */ Boolean* shouldSetValue)
{
    VALIDATE_NOT_NULL(shouldSetValue)

    if (mOnChangeListener == NULL) {
        *shouldSetValue = TRUE;
        return NOERROR;
    }
    else {
        return mOnChangeListener->OnPreferenceChange(
                THIS_PROBE(IPreference), newValue, shouldSetValue);
    }
}

ECode Preference::SetOnPreferenceChangeListener(
    /* [in] */ IPreferenceOnPreferenceChangeListener* onPreferenceChangeListener)
{
    mOnChangeListener = onPreferenceChangeListener;
    return NOERROR;
}

ECode Preference::GetOnPreferenceChangeListener(
    /* [out] */ IPreferenceOnPreferenceChangeListener** listener)
{
    VALIDATE_NOT_NULL(listener)
    *listener = mOnChangeListener;
    REFCOUNT_ADD(*listener)
    return NOERROR;
}

ECode Preference::SetOnPreferenceClickListener(
    /* [in] */ IPreferenceOnPreferenceClickListener* onPreferenceClickListener)
{
    mOnClickListener = onPreferenceClickListener;
    return NOERROR;
}

ECode Preference::GetOnPreferenceClickListener(
    /* [out] */ IPreferenceOnPreferenceClickListener** listener)
{
    VALIDATE_NOT_NULL(listener)
    *listener = mOnClickListener;
    REFCOUNT_ADD(*listener)
    return NOERROR;
}

ECode Preference::PerformClick(
    /* [in] */ IPreferenceScreen* preferenceScreen)
{
    Boolean isEnable;
    if (IsEnabled(&isEnable), !isEnable) {
        return NOERROR;
    }

    OnClick();

    Boolean clickResult;
    if (mOnClickListener != NULL &&
            (mOnClickListener->OnPreferenceClick(THIS_PROBE(IPreference), &clickResult), clickResult)) {
        return NOERROR;
    }

    AutoPtr<IPreferenceManager> preferenceManager;
    GetPreferenceManager((IPreferenceManager**)&preferenceManager);
    if (preferenceManager != NULL) {
        AutoPtr<IPreferenceManagerOnPreferenceTreeClickListener> listener;
        preferenceManager->GetOnPreferenceTreeClickListener((IPreferenceManagerOnPreferenceTreeClickListener**)&listener);
        if (preferenceScreen != NULL && listener != NULL
                && (listener->OnPreferenceTreeClick(preferenceScreen, THIS_PROBE(IPreference), &clickResult), clickResult)) {
            return NOERROR;
        }
    }

    if (mIntent != NULL) {
        AutoPtr<IContext> context;
        GetContext((IContext**)&context);
        context->StartActivity(mIntent);
    }
    return NOERROR;
}

ECode Preference::OnKey(
    /* [in] */ IView* v,
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode Preference::GetContext(
    /* [out] */ IContext** context)
{
    VALIDATE_NOT_NULL(context)
    *context = mContext;
    REFCOUNT_ADD(*context)
    return NOERROR;
}

ECode Preference::GetSharedPreferences(
    /* [out] */ ISharedPreferences** preference)
{
    VALIDATE_NOT_NULL(preference)
    if (mPreferenceManager == NULL) {
        *preference = NULL;
        return NOERROR;
    }
    return mPreferenceManager->GetSharedPreferences(preference);
}

ECode Preference::GetEditor(
    /* [out] */ ISharedPreferencesEditor** editor)
{
    VALIDATE_NOT_NULL(editor)
    if (mPreferenceManager == NULL) {
        *editor = NULL;
        return NOERROR;
    }
    return mPreferenceManager->GetEditor(editor);
}

ECode Preference::ShouldCommit(
    /* [out] */ Boolean* shouldCommit)
{
    VALIDATE_NOT_NULL(shouldCommit)
    if (mPreferenceManager == NULL) {
        *shouldCommit = FALSE;
    }
    return mPreferenceManager->ShouldCommit(shouldCommit);
}

ECode Preference::CompareTo(
    /* [in] */ IInterface* _another,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    AutoPtr<IPreference> another = THIS_PROBE(IPreference);
    Int32 anotherOrder;
    another->GetOrder(&anotherOrder);
    if (mOrder != IPreference::DEFAULT_ORDER
            || (mOrder == IPreference::DEFAULT_ORDER && anotherOrder != IPreference::DEFAULT_ORDER)) {
        // Do order comparison
        *result = mOrder - anotherOrder;
    }
    else if (mTitle == NULL) {
        *result = 1;
    }
    else {
        AutoPtr<ICharSequence> anotherTitle;
        another->GetTitle((ICharSequence**)&anotherTitle);
        if (anotherTitle == NULL) {
            *result = -1;
        }
        else {
            // Do name comparison
            String titleStr, anotherTitleStr;
            mTitle->ToString(&titleStr);
            anotherTitle->ToString(&anotherTitleStr);
            *result = titleStr.CompareIgnoreCase(anotherTitleStr);
        }
    }
    return NOERROR;
}

void Preference::SetOnPreferenceChangeInternalListener(
    /* [in] */ IPreferenceOnPreferenceChangeInternalListener* listener)
{
    mListener = listener;
}

ECode Preference::NotifyChanged()
{
    if (mListener != NULL) {
        return mListener->OnPreferenceChange(THIS_PROBE(IPreference));
    }
    return NOERROR;
}

ECode Preference::NotifyHierarchyChanged()
{
    if (mListener != NULL) {
        return mListener->OnPreferenceHierarchyChange(THIS_PROBE(IPreference));
    }
    return NOERROR;
}

ECode Preference::GetPreferenceManager(
    /* [out] */ IPreferenceManager** manager)
{
    VALIDATE_NOT_NULL(manager)
    *manager = mPreferenceManager;
    REFCOUNT_ADD(*manager)
    return NOERROR;
}

ECode Preference::OnAttachedToHierarchy(
    /* [in] */ IPreferenceManager* preferenceManager)
{
    mPreferenceManager = preferenceManager;
    mPreferenceManager->GetNextId(&mId);
    DispatchSetInitialValue();
    return NOERROR;
}

ECode Preference::OnAttachedToActivity()
{
    // At this point, the hierarchy that this preference is in is connected
    // with all other preferences.
    return RegisterDependency();
}

ECode Preference::RegisterDependency()
{
    if (TextUtils::IsEmpty(mDependencyKey)) return NOERROR;

    AutoPtr<IPreference> preference;
    FindPreferenceInHierarchy(mDependencyKey, (IPreference**)&preference);
    if (preference != NULL) {
        AutoPtr<Preference> p = reinterpret_cast<Preference*>(preference->Probe(EIID_Preference));
        return p->RegisterDependent(this);
    }
    else {
//        throw new IllegalStateException("Dependency \"" + mDependencyKey
//                + "\" not found for preference \"" + mKey + "\" (title: \"" + mTitle + "\"");
        String str;
        mTitle->ToString(&str);
        Slogger::E("Preference", "Dependency \"%s\" not found for preference \"%s\" (title: \"%s\""
                , mDependencyKey.string(), mKey.string(), str.string());
        return E_ILLEGAL_STATE_EXCEPTION;
    }
}

void Preference::UnregisterDependency()
{
    if (mDependencyKey != NULL) {
        AutoPtr<IPreference> oldDependency;
        FindPreferenceInHierarchy(mDependencyKey, (IPreference**)&oldDependency);
        if (oldDependency != NULL) {
            AutoPtr<Preference> preference = reinterpret_cast<Preference*>(oldDependency->Probe(EIID_Preference));
            return preference->UnRegisterDependent(this);
        }
    }
}

ECode Preference::FindPreferenceInHierarchy(
    /* [in] */ const String& key,
    /* [out] */ IPreference** preference)
{
    VALIDATE_NOT_NULL(preference)
    if (TextUtils::IsEmpty(key) || mPreferenceManager == NULL) {
        *preference = NULL;
        return NOERROR;
    }
    AutoPtr<ICharSequence> cs;
    CString::New(key, (ICharSequence**)&cs);
    return mPreferenceManager->FindPreference(cs, preference);
}

ECode Preference::RegisterDependent(
    /* [in] */ Preference* dependent)
{
    if (mDependents == NULL) {
        mDependents = new List<AutoPtr<IPreference> >();
    }
    mDependents->PushBack(dependent);

    Boolean shouldDisableDependents;
    ShouldDisableDependents(&shouldDisableDependents);
    return dependent->OnDependencyChanged(THIS_PROBE(IPreference), shouldDisableDependents);
}

void Preference::UnRegisterDependent(
    /* [in] */ Preference* dependent)
{
    if (mDependents != NULL) {
        mDependents->Remove(dependent);
    }
}

ECode Preference::NotifyDependencyChange(
    /* [in] */ Boolean disableDependents)
{
    AutoPtr< List<AutoPtr<IPreference> > > dependents = mDependents;

    if (dependents == NULL) return NOERROR;

    List<AutoPtr<IPreference> >::Iterator it;
    for (it = dependents->Begin(); it != dependents->End(); ++it) {
        IOnDependencyChangeListener::Probe(*it)->OnDependencyChanged(THIS_PROBE(IPreference), disableDependents);
    }
    return NOERROR;
}

ECode Preference::OnDependencyChanged(
    /* [in] */ IPreference* dependency,
    /* [in] */ Boolean disableDependent)
{
    if (mDependencyMet == disableDependent) {
        mDependencyMet = !disableDependent;

        // Enabled state can change dependent preferences' states, so notify
        Boolean shouldDisableDependents;
        ShouldDisableDependents(&shouldDisableDependents);
        NotifyDependencyChange(shouldDisableDependents);

        NotifyChanged();
    }
    return NOERROR;
}

ECode Preference::ShouldDisableDependents(
    /* [out] */ Boolean* shouldDisableDependents)
{
    VALIDATE_NOT_NULL(shouldDisableDependents)
    Boolean isEnable;
    IsEnabled(&isEnable);
    *shouldDisableDependents = !isEnable;
    return NOERROR;
}

ECode Preference::SetDependency(
    /* [in] */ const String& dependencyKey)
{
    // Unregister the old dependency, if we had one
    UnregisterDependency();

    // Register the new
    mDependencyKey = dependencyKey;
    return RegisterDependency();
}

ECode Preference::GetDependency(
    /* [out] */ String* dependencyKey)
{
    VALIDATE_NOT_NULL(dependencyKey)
    *dependencyKey = mDependencyKey;
    return NOERROR;
}

ECode Preference::OnPrepareForRemoval()
{
    UnregisterDependency();
    return NOERROR;
}

ECode Preference::SetDefaultValue(
    /* [in] */ IInterface* defaultValue)
{
    mDefaultValue = defaultValue;
    return NOERROR;
}

ECode Preference::DispatchSetInitialValue()
{
    // By now, we know if we are persistent.
    Boolean shouldPersist;
    if (ShouldPersist(&shouldPersist), !shouldPersist) {
        if (mDefaultValue != NULL) {
            return OnSetInitialValue(FALSE, mDefaultValue);
        }
    }
    else {
        AutoPtr<ISharedPreferences> sp;
        GetSharedPreferences((ISharedPreferences**)&sp);
        Boolean contains;
        sp->Contains(mKey, &contains);
        if (!contains && mDefaultValue != NULL) {
            return OnSetInitialValue(FALSE, mDefaultValue);
        }
        return OnSetInitialValue(TRUE, NULL);
    }
    return NOERROR;
}

ECode Preference::OnSetInitialValue(
    /* [in] */ Boolean restorePersistedValue,
    /* [in] */ IInterface* defaultValue)
{
    return NOERROR;
}

void Preference::TryCommit(
    /* [in] */ ISharedPreferencesEditor* editor)
{
    Boolean shouldCommit;
    if (mPreferenceManager->ShouldCommit(&shouldCommit), shouldCommit) {
        if(FAILED(editor->Apply())){
            Boolean commitResult;
            editor->Commit(&commitResult);
        }
    }
}

ECode Preference::PersistString(
    /* [in] */ const String& value,
    /* [out] */ Boolean* isPersistString)
{
    VALIDATE_NOT_NULL(isPersistString)
    Boolean shouldPersist;
    if (ShouldPersist(&shouldPersist), shouldPersist) {
        // Shouldn't store null
        String persistedString;
        GetPersistedString(String(NULL), &persistedString);
        if (persistedString.Equals(value)) {
            // It's already there, so the same as persisting
            *isPersistString = TRUE;
            return NOERROR;
        }

        AutoPtr<ISharedPreferencesEditor> editor;
        mPreferenceManager->GetEditor((ISharedPreferencesEditor**)&editor);
        editor->PutString(mKey, value);
        TryCommit(editor);
        *isPersistString = TRUE;
        return NOERROR;
    }
    *isPersistString = FALSE;
    return NOERROR;
}

ECode Preference::GetPersistedString(
    /* [in] */ const String& defaultReturnValue,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    Boolean shouldPersist;
    if (ShouldPersist(&shouldPersist), !shouldPersist) {
        *str = defaultReturnValue;
        return NOERROR;
    }

    AutoPtr<ISharedPreferences> sp;
    mPreferenceManager->GetSharedPreferences((ISharedPreferences**)&sp);
    return sp->GetString(mKey, defaultReturnValue, str);
}

ECode Preference::PersistStringSet(
    /* [in] */ ISet* values,
    /* [out] */ Boolean* isPersistStringSet)
{
    VALIDATE_NOT_NULL(isPersistStringSet)
    Boolean shouldPersist;
    if (ShouldPersist(&shouldPersist), shouldPersist) {
        // Shouldn't store null
        AutoPtr<ISet> persistStringSet;
        GetPersistedStringSet(NULL, (ISet**)&persistStringSet);
        Boolean isEquals;
        if (values->Equals(persistStringSet, &isEquals), isEquals) {
            // It's already there, so the same as persisting
            *isPersistStringSet = TRUE;
            return NOERROR;
        }

        AutoPtr<ISharedPreferencesEditor> editor;
        mPreferenceManager->GetEditor((ISharedPreferencesEditor**)&editor);
        editor->PutStringSet(mKey, values);
        TryCommit(editor);
        *isPersistStringSet = TRUE;
        return NOERROR;
    }
    *isPersistStringSet = FALSE;
    return NOERROR;
}

ECode Preference::GetPersistedStringSet(
    /* [in] */ ISet* defaultReturnValue,
    /* [out] */ ISet** value)
{
    VALIDATE_NOT_NULL(value)
    Boolean shouldPersist;
    if (ShouldPersist(&shouldPersist), !shouldPersist) {
        *value = defaultReturnValue;
        REFCOUNT_ADD(*value)
        return NOERROR;
    }

    AutoPtr<ISharedPreferences> sp;
    mPreferenceManager->GetSharedPreferences((ISharedPreferences**)&sp);
    return sp->GetStringSet(mKey, defaultReturnValue, value);
}

ECode Preference::PersistInt32(
    /* [in] */ Int32 value,
    /* [out] */ Boolean* isPersist)
{
    VALIDATE_NOT_NULL(isPersist)

    Boolean shouldPersist;
    if (ShouldPersist(&shouldPersist), shouldPersist) {
        Int32 persistedValue;
        if (GetPersistedInt32(~value, &persistedValue), value == persistedValue) {
            // It's already there, so the same as persisting
            *isPersist = TRUE;
            return NOERROR;
        }

        AutoPtr<ISharedPreferencesEditor> editor;
        mPreferenceManager->GetEditor((ISharedPreferencesEditor**)&editor);
        editor->PutInt32(mKey, value);
        TryCommit(editor);
        *isPersist = TRUE;
        return NOERROR;
    }
    *isPersist = FALSE;
    return NOERROR;
}

ECode Preference::GetPersistedInt32(
    /* [in] */ Int32 defaultReturnValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    Boolean shouldPersist;
    if (ShouldPersist(&shouldPersist), !shouldPersist) {
        *value = defaultReturnValue;
        return NOERROR;
    }

    AutoPtr<ISharedPreferences> sp;
    mPreferenceManager->GetSharedPreferences((ISharedPreferences**)&sp);
    return sp->GetInt32(mKey, defaultReturnValue, value);
}

ECode Preference::PersistFloat(
    /* [in] */ Float value,
    /* [out] */ Boolean* isPersist)
{
    VALIDATE_NOT_NULL(isPersist)

    Boolean shouldPersist;
    if (ShouldPersist(&shouldPersist), shouldPersist) {
        Float persistValue;
        if (GetPersistedFloat(Elastos::Core::Math::FLOAT_NAN, &persistValue), value == persistValue) {
            // It's already there, so the same as persisting
            *isPersist = TRUE;
            return NOERROR;
        }

        AutoPtr<ISharedPreferencesEditor> editor;
        mPreferenceManager->GetEditor((ISharedPreferencesEditor**)&editor);
        editor->PutFloat(mKey, value);
        TryCommit(editor);
        *isPersist = TRUE;
        return NOERROR;
    }
    *isPersist = FALSE;
    return NOERROR;
}

ECode Preference::GetPersistedFloat(
    /* [in] */ Float defaultReturnValue,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value)

    Boolean shouldPersist;
    if (ShouldPersist(&shouldPersist), !shouldPersist) {
        *value = defaultReturnValue;
        return NOERROR;
    }

    AutoPtr<ISharedPreferences> sp;
    mPreferenceManager->GetSharedPreferences((ISharedPreferences**)&sp);
    return sp->GetFloat(mKey, defaultReturnValue, value);
}

ECode Preference::PersistInt64(
    /* [in] */ Int64 value,
    /* [out] */ Boolean* isPersist)
{
    VALIDATE_NOT_NULL(isPersist)

    Boolean shouldPersist;
    if (ShouldPersist(&shouldPersist), shouldPersist) {
        Int64 persistedLong;
        if (GetPersistedInt64(~value, &persistedLong), value == persistedLong) {
            // It's already there, so the same as persisting
            *isPersist = TRUE;
            return NOERROR;
        }

        AutoPtr<ISharedPreferencesEditor> editor;
        mPreferenceManager->GetEditor((ISharedPreferencesEditor**)&editor);
        editor->PutInt64(mKey, value);
        TryCommit(editor);
        *isPersist = TRUE;
        return NOERROR;
    }
    *isPersist = FALSE;
    return NOERROR;
}

ECode Preference::GetPersistedInt64(
    /* [in] */ Int64 defaultReturnValue,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)

    Boolean shouldPersist;
    if (ShouldPersist(&shouldPersist), !shouldPersist) {
        *value = defaultReturnValue;
        return NOERROR;
    }

    AutoPtr<ISharedPreferences> sp;
    mPreferenceManager->GetSharedPreferences((ISharedPreferences**)&sp);
    return sp->GetInt64(mKey, defaultReturnValue, value);
}

ECode Preference::PersistBoolean(
    /* [in] */ Boolean value,
    /* [out] */ Boolean* isPersist)
{
    VALIDATE_NOT_NULL(isPersist)

    Boolean shouldPersist;
    if (ShouldPersist(&shouldPersist), shouldPersist) {
        Boolean persistedBoolean;
        if (GetPersistedBoolean(!value, &persistedBoolean), value == persistedBoolean) {
            // It's already there, so the same as persisting
            *isPersist = TRUE;
            return NOERROR;
        }

        AutoPtr<ISharedPreferencesEditor> editor;
        mPreferenceManager->GetEditor((ISharedPreferencesEditor**)&editor);
        editor->PutBoolean(mKey, value);
        TryCommit(editor);
        *isPersist = TRUE;
        return NOERROR;
    }

    *isPersist = FALSE;
    return NOERROR;
}

ECode Preference::GetPersistedBoolean(
    /* [in] */ Boolean defaultReturnValue,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    Boolean shouldPersist;
    if (ShouldPersist(&shouldPersist), !shouldPersist) {
        *value = defaultReturnValue;
        return NOERROR;
    }

    AutoPtr<ISharedPreferences> sp;
    mPreferenceManager->GetSharedPreferences((ISharedPreferences**)&sp);
    return sp->GetBoolean(mKey, defaultReturnValue, value);
}

ECode Preference::HasSpecifiedLayout(
    /* [out] */ Boolean* hasSpecifiedLayout)
{
    VALIDATE_NOT_NULL(hasSpecifiedLayout)
    *hasSpecifiedLayout = mHasSpecifiedLayout;
    return NOERROR;
}

ECode Preference::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    *str = GetFilterableString();
    return NOERROR;
}

String Preference::GetFilterableString()
{
    StringBuilder sb;
    AutoPtr<ICharSequence> title;
    GetTitle((ICharSequence**)&title);
    if (!TextUtils::IsEmpty(title)) {
        sb += title;
        sb += ' ';
    }
    AutoPtr<ICharSequence> summary;
    GetSummary((ICharSequence**)&summary);
    if (!TextUtils::IsEmpty(summary)) {
        sb += summary;
        sb += ' ';
    }
    Int32 len = sb.GetLength();
    if (len > 0) {
        // Drop the last space
        return sb.Substring(0, len - 2);
    }
    return sb.ToString();
}

ECode Preference::SaveHierarchyState(
    /* [in] */ IBundle* container)
{
    return DispatchSaveInstanceState(container);
}

ECode Preference::DispatchSaveInstanceState(
    /* [in] */ IBundle* container)
{
    Boolean hasKey;
    if (HasKey(&hasKey), hasKey) {
        mBaseMethodCalled = FALSE;
        AutoPtr<IParcelable> state;
        OnSaveInstanceState((IParcelable**)&state);
        if (!mBaseMethodCalled) {
//            throw new IllegalStateException(
//                    "Derived class did not call super.onSaveInstanceState()");
            Slogger::E("preference", "Derived class did not call super.onSaveInstanceState()");
            return E_ILLEGAL_STATE_EXCEPTION;
        }
        if (state != NULL) {
            container->PutParcelable(mKey, state);
        }
    }

    return NOERROR;
}

ECode Preference::OnSaveInstanceState(
    /* [out] */ IParcelable** state)
{
    VALIDATE_NOT_NULL(state)
    mBaseMethodCalled = TRUE;
    AutoPtr<IAbsSavedStateHelper> helper;
    CAbsSavedStateHelper::AcquireSingleton((IAbsSavedStateHelper**)&helper);
    AutoPtr<IAbsSavedState> emptyState;
    helper->GetEMPTY_STATE((IAbsSavedState**)&emptyState);
    *state = IParcelable::Probe(emptyState);
    REFCOUNT_ADD(*state);
    return NOERROR;
}

ECode Preference::RestoreHierarchyState(
    /* [in] */ IBundle* container)
{
    return DispatchRestoreInstanceState(container);
}

ECode Preference::DispatchRestoreInstanceState(
    /* [in] */ IBundle* container)
{
    Boolean hasKey;
    if (HasKey(&hasKey), hasKey) {
        AutoPtr<IParcelable> state;
        container->GetParcelable(mKey, (IParcelable**)&state);
        if (state != NULL) {
            mBaseMethodCalled = FALSE;
            OnRestoreInstanceState(state);
            if (!mBaseMethodCalled) {
//                throw new IllegalStateException(
//                        "Derived class did not call super.onRestoreInstanceState()");
                Slogger::E("preference", "Derived class did not call super.onRestoreInstanceState()");
                return E_ILLEGAL_STATE_EXCEPTION;
            }
        }
    }
    return NOERROR;
}

ECode Preference::OnRestoreInstanceState(
    /* [in] */ IParcelable* state)
{
    mBaseMethodCalled = TRUE;
    AutoPtr<IAbsSavedStateHelper> helper;
    CAbsSavedStateHelper::AcquireSingleton((IAbsSavedStateHelper**)&helper);
    AutoPtr<IAbsSavedState> emptyState;
    helper->GetEMPTY_STATE((IAbsSavedState**)&emptyState);
    if (state != IParcelable::Probe(emptyState) && state != NULL) {
    //        throw new IllegalArgumentException("Wrong state class -- expecting Preference State");
        Slogger::E("preference", "Wrong state class -- expecting Preference State");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

} // Preference
} // Droid
} // Elastos
