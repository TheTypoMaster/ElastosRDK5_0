#ifndef __ELASTOS_DROID_ACCESSIBILITYSERVICE_CACCESSIBILITYSERVICEINFO_H__
#define __ELASTOS_DROID_ACCESSIBILITYSERVICE_CACCESSIBILITYSERVICEINFO_H__

#include "_Elastos_Droid_AccessibilityService_CAccessibilityServiceInfo.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace AccessibilityService {

CarClass(CAccessibilityServiceInfo)
{
public:
    CAccessibilityServiceInfo();

    ~CAccessibilityServiceInfo();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IResolveInfo * resolveInfo,
        /* [in] */ IContext * context);

    CARAPI SetEventTypes (
        /* [in] */ Int32 types);

    CARAPI GetEventTypes (
        /* [out] */ Int32* types);

    CARAPI SetPackageNames (
        /* [in] */ ArrayOf<String>* names);

    CARAPI GetPackageNames (
        /* [out, callee] */ ArrayOf<String>** names);

    CARAPI SetFeedbackType (
        /* [in] */ Int32 type);

    CARAPI GetFeedbackType (
        /* [out] */ Int32* type);

    CARAPI SetNotificationTimeout (
        /* [in] */ Int64 timeout);

    CARAPI GetNotificationTimeout (
        /* [out] */ Int64* timeout);

    CARAPI SetFlags (
        /* [in] */ Int32 flags);

    CARAPI GetFlags (
        /* [out] */ Int32* flags);

    /**
     * Updates the properties that an AccessibilitySerivice can change dynamically.
     *
     * @param other The info from which to update the properties.
     *
     * @hide
     */
    CARAPI UpdateDynamicallyConfigurableProperties(
        /* [in] */ IAccessibilityServiceInfo* other);

    /**
     * The accessibility service id.
     * <p>
     *   <strong>Generated by the system.</strong>
     * </p>
     * @return The id.
     */
    CARAPI GetId(
        /* [out] */ String* str);

    /**
     * The service {@link ResolveInfo}.
     * <p>
     *   <strong>Generated by the system.</strong>
     * </p>
     * @return The info.
     */
    CARAPI GetResolveInfo(
        /* [out] */ IResolveInfo** info);

    /**
     * The settings activity name.
     * <p>
     *    <strong>Statically set from
     *    {@link AccessibilityService#SERVICE_META_DATA meta-data}.</strong>
     * </p>
     * @return The settings activity name.
     */
    CARAPI GetSettingsActivityName(
        /* [out] */ String* str);

    /**
     * Whether this service can retrieve the current window's content.
     * <p>
     *    <strong>Statically set from
     *    {@link AccessibilityService#SERVICE_META_DATA meta-data}.</strong>
     * </p>
     * @return True if window content can be retrieved.
     */
    CARAPI GetCanRetrieveWindowContent(
        /* [out] */ Boolean* result);

    /**
     * Gets the non-localized description of the accessibility service.
     * <p>
     *    <strong>Statically set from
     *    {@link AccessibilityService#SERVICE_META_DATA meta-data}.</strong>
     * </p>
     * @return The description.
     *
     * @deprecated Use {@link #loadDescription(PackageManager)}.
     */
    CARAPI GetDescription(
        /* [out] */ String* str);

    /**
     * The localized description of the accessibility service.
     * <p>
     *    <strong>Statically set from
     *    {@link AccessibilityService#SERVICE_META_DATA meta-data}.</strong>
     * </p>
     * @return The localized description.
     */
    CARAPI LoadDescription(
        /* [in] */ IPackageManager* packageManager,
        /* [out] */ String* str);

    /**
     * {@inheritDoc}
     */
    CARAPI DescribeContents(
        /* [out] */ Int32* val);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    /**
     * Returns the string representation of a feedback type. For example,
     * {@link #FEEDBACK_SPOKEN} is represented by the string FEEDBACK_SPOKEN.
     *
     * @param feedbackType The feedback type.
     * @return The string representation.
     */
    static CARAPI FeedbackTypeToString(
        /* [in] */ Int32 feedbackType,
        /* [out] */ String* str);

    /**
     * Returns the string representation of a flag. For example,
     * {@link #DEFAULT} is represented by the string DEFAULT.
     *
     * @param flag The flag.
     * @return The string representation.
     */
    static CARAPI FlagToString(
        /* [in] */ Int32 flag,
        /* [out] */ String* str);

private:
    CARAPI_(void) InitFromParcel(
        /* [in] */ IParcel* parcel);

    static CARAPI_(void) AppendFeedbackTypes(
        /* [in] */ StringBuilder* stringBuilder,
        /* [in] */ Int32 feedbackTypes);

    static CARAPI_(void) AppendPackageNames(
        /* [in] */ StringBuilder* stringBuilder,
        /* [in] */ ArrayOf<String>* packageNames);

    static CARAPI_(void) AppendEventTypes(
        /* [in] */ StringBuilder* stringBuilder,
        /* [in] */ Int32 eventTypes);

    static CARAPI_(void) AppendFlags(
        /* [in] */ StringBuilder* stringBuilder,
        /* [in] */ Int32 flags);

public:
    static const String TAG_ACCESSIBILITY_SERVICE;

    /**
     * The event types an {@link AccessibilityService} is interested in.
     * <p>
     *   <strong>Can be dynamically set at runtime.</strong>
     * </p>
     * @see android.view.accessibility.AccessibilityEvent#TYPE_VIEW_CLICKED
     * @see android.view.accessibility.AccessibilityEvent#TYPE_VIEW_LONG_CLICKED
     * @see android.view.accessibility.AccessibilityEvent#TYPE_VIEW_FOCUSED
     * @see android.view.accessibility.AccessibilityEvent#TYPE_VIEW_SELECTED
     * @see android.view.accessibility.AccessibilityEvent#TYPE_VIEW_TEXT_CHANGED
     * @see android.view.accessibility.AccessibilityEvent#TYPE_WINDOW_STATE_CHANGED
     * @see android.view.accessibility.AccessibilityEvent#TYPE_NOTIFICATION_STATE_CHANGED
     * @see android.view.accessibility.AccessibilityEvent#TYPE_TOUCH_EXPLORATION_GESTURE_START
     * @see android.view.accessibility.AccessibilityEvent#TYPE_TOUCH_EXPLORATION_GESTURE_END
     * @see android.view.accessibility.AccessibilityEvent#TYPE_VIEW_HOVER_ENTER
     * @see android.view.accessibility.AccessibilityEvent#TYPE_VIEW_HOVER_EXIT
     * @see android.view.accessibility.AccessibilityEvent#TYPE_VIEW_SCROLLED
     * @see android.view.accessibility.AccessibilityEvent#TYPE_VIEW_TEXT_SELECTION_CHANGED
     * @see android.view.accessibility.AccessibilityEvent#TYPE_WINDOW_CONTENT_CHANGED
     */
    Int32 mEventTypes;

    /**
     * The package names an {@link AccessibilityService} is interested in. Setting
     * to <code>null</code> is equivalent to all packages.
     * <p>
     *   <strong>Can be dynamically set at runtime.</strong>
     * </p>
     */
    AutoPtr<ArrayOf<String> > mPackageNames;

    /**
     * The feedback type an {@link AccessibilityService} provides.
     * <p>
     *   <strong>Can be dynamically set at runtime.</strong>
     * </p>
     * @see #FEEDBACK_AUDIBLE
     * @see #FEEDBACK_GENERIC
     * @see #FEEDBACK_HAPTIC
     * @see #FEEDBACK_SPOKEN
     * @see #FEEDBACK_VISUAL
     * @see #FEEDBACK_BRAILLE
     */
    Int32 mFeedbackType;

    /**
     * The timeout after the most recent event of a given type before an
     * {@link AccessibilityService} is notified.
     * <p>
     *   <strong>Can be dynamically set at runtime.</strong>.
     * </p>
     * <p>
     * <strong>Note:</strong> The event notification timeout is useful to avoid propagating
     *       events to the client too frequently since this is accomplished via an expensive
     *       interprocess call. One can think of the timeout as a criteria to determine when
     *       event generation has settled down.
     */
    Int64 mNotificationTimeout;

    /**
     * This field represents a set of flags used for configuring an
     * {@link AccessibilityService}.
     * <p>
     *   <strong>Can be dynamically set at runtime.</strong>
     * </p>
     * @see #DEFAULT
     * @see #FLAG_INCLUDE_NOT_IMPORTANT_VIEWS
     * @see #FLAG_REQUEST_TOUCH_EXPLORATION_MODE
     */
    Int32 mFlags;

    /**
     * The unique string Id to identify the accessibility service.
     */
    String mId;

    /**
     * The Service that implements this accessibility service component.
     */
    AutoPtr<IResolveInfo> mResolveInfo;

    /**
     * The accessibility service setting activity's name, used by the system
     * settings to launch the setting activity of this accessibility service.
     */
    String mSettingsActivityName;

    /**
     * Flag whether this accessibility service can retrieve window content.
     */
    Boolean mCanRetrieveWindowContent;

    /**
     * Resource id of the description of the accessibility service.
     */
    Int32 mDescriptionResId;

    /**
     * Non localized description of the accessibility service.
     */
    String mNonLocalizedDescription;
};

}
}
}

#endif // __ELASTOS_DROID_ACCESSIBILITYSERVICE_CACCESSIBILITYSERVICEINFO_H__
