
#ifndef __ELASTOS_DROID_WEBKIT_BASE_APPLICATIONSTATUS_H__
#define __ELASTOS_DROID_WEBKIT_BASE_APPLICATIONSTATUS_H__

// import android.app.Activity;
// import android.app.Application;
// import android.app.Application.ActivityLifecycleCallbacks;
// import android.content.Context;
// import android.os.Bundle;

// import java.lang.ref.WeakReference;
// import java.util.ArrayList;
// import java.util.List;
// import java.util.Map;
// import java.util.concurrent.ConcurrentHashMap;

/**
 * Provides information about the current activity's status, and a way
 * to register / unregister listeners for state changes.
 */
//@JNINamespace("base::android")

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

class ApplicationStatus
{
public:
    /**
     * Interface to be implemented by listeners.
     */
    class ApplicationStateListener
    {
    public:
        /**
         * Called when the application's state changes.
         * @param newState The application state.
         */
        virtual CARAPI_(void) OnApplicationStateChange(
            /* [in] */ Int32 newState) = 0;
    };

    /**
     * Interface to be implemented by listeners.
     */
    class ActivityStateListener
    {
    public:
        /**
         * Called when the activity's state changes.
         * @param activity The activity that had a state change.
         * @param newState New activity state.
         */
        virtual CARAPI_(void) OnActivityStateChange(
            /* [in] */ IActivity* activity,
            /* [in] */ Int32 newState) = 0;
    };

private:
    class ActivityInfo
    {
    public:
        ActivityInfo();

       /**
         * @return The current {@link ActivityState} of the activity.
         */
        virtual CARAPI_(Int32) GetStatus();

        /**
         * @param status The new {@link ActivityState} of the activity.
         */
        virtual CARAPI_(void) SetStatus(
            /* [in] */ Int32 status);

        /**
         * @return A list of {@link ActivityStateListener}s listening to this activity.
         */
        virtual CARAPI_(ObserverList<ActivityStateListener>) GetListeners();

    private:
        Int32 mStatus;// = ActivityState.DESTROYED;
        ObserverList<ActivityStateListener> mListeners;// = new ObserverList<ActivityStateListener>();
    };

    class InnerWindowFocusChangedListener : public BaseChromiumApplication::WindowFocusChangedListener
    {
    public:
        InnerWindowFocusChangedListener(
            /* [in] */ ApplicationStatus* owner);

        virtual CARAPI_(void) OnWindowFocusChanged(
            /* [in] */ IActivity* activity,
            /* [in] */ Boolean hasFocus);

    private:
        ApplicationStatus* mOwner;
    };

    class InnerActivityLifecycleCallbacks
        : public Object,
        , public IActivityLifecycleCallbacks
    {
    public:
        InnerActivityLifecycleCallbacks(
            /* [in] */ ApplicationStatus* owner);

        virtual CARAPI_(void) OnActivityCreated(
            /* [in] */ IActivity* activity,
            /* [in] */ IBundle* savedInstanceState);

        virtual CARAPI_(void) OnActivityDestroyed(
            /* [in] */ IActivity* activity);

        virtual CARAPI_(void) OnActivityPaused(
            /* [in] */ IActivity* activity);

        virtual CARAPI_(void) OnActivityResumed(
            /* [in] */ IActivity* activity);

        virtual CARAPI_(void) OnActivitySaveInstanceState(
            /* [in] */ IActivity* activity,
            /* [in] */ IBundle* outState);

        virtual CARAPI_(void) OnActivityStarted(
            /* [in] */ IActivity* activity);

        virtual CARAPI_(void) OnActivityStopped(
            /* [in] */ IActivity* activity);

    private:
        ApplicationStatus* mOwner;
    }

    class InnerRunnable
        : public Object
        , public IRunnable
    {
    public:
        InnerRunnable(
            /* [in] */ ApplicationStatus* owner);

        CARAPI Run();

    private:
        ApplicationStatus* mOwner;
    };

    class InnerApplicationStateListener : public ApplicationStateListener
    {
    public:
        InnerApplicationStateListener(
            /* [in] */ ApplicationStatus* owner);

        virtual CARAPI_(void) OnApplicationStateChange(
            /* [in] */ Int32 newState);

    private:
        ApplicationStatus* mOwner;
    };

public:
    /**
     * Initializes the activity status for a specified application.
     *
     * @param application The application whose status you wish to monitor.
     */
    static CARAPI_(void) Initialize(
        /* [in] */ BaseChromiumApplication* application);

    /**
     * Testing method to update the state of the specified activity.
     */
    static CARAPI_(void) OnStateChangeForTesting(
        /* [in] */ IActivity* activity,
        /* [in] */ Int32 newState);

    /**
     * @return The most recent focused {@link Activity} tracked by this class.  Being focused means
     *         out of all the activities tracked here, it has most recently gained window focus.
     */
    static CARAPI_(AutoPtr<IActivity>) GetLastTrackedFocusedActivity();

    /**
     * @return A {@link List} of all non-destroyed {@link Activity}s.
     */
    static CARAPI_(List<WeakReference<Activity>>) GetRunningActivities();

    /**
     * @return The {@link Context} for the {@link Application}.
     */
    static CARAPI_(AutoPtr<IContext>) GetApplicationContext();

    /**
     * Query the state for a given activity.  If the activity is not being tracked, this will
     * return {@link ActivityState#DESTROYED}.
     *
     * <p>
     * Please note that Chrome can have multiple activities running simultaneously.  Please also
     * look at {@link #getStateForApplication()} for more details.
     *
     * <p>
     * When relying on this method, be familiar with the expected life cycle state
     * transitions:
     * <a href="http://developer.android.com/guide/components/activities.html#Lifecycle">
     *   Activity Lifecycle
     * </a>
     *
     * <p>
     * During activity transitions (activity B launching in front of activity A), A will completely
     * paused before the creation of activity B begins.
     *
     * <p>
     * A basic flow for activity A starting, followed by activity B being opened and then closed:
     * <ul>
     *   <li> -- Starting Activity A --
     *   <li> Activity A - ActivityState.CREATED
     *   <li> Activity A - ActivityState.STARTED
     *   <li> Activity A - ActivityState.RESUMED
     *   <li> -- Starting Activity B --
     *   <li> Activity A - ActivityState.PAUSED
     *   <li> Activity B - ActivityState.CREATED
     *   <li> Activity B - ActivityState.STARTED
     *   <li> Activity B - ActivityState.RESUMED
     *   <li> Activity A - ActivityState.STOPPED
     *   <li> -- Closing Activity B, Activity A regaining focus --
     *   <li> Activity B - ActivityState.PAUSED
     *   <li> Activity A - ActivityState.STARTED
     *   <li> Activity A - ActivityState.RESUMED
     *   <li> Activity B - ActivityState.STOPPED
     *   <li> Activity B - ActivityState.DESTROYED
     * </ul>
     *
     * @param activity The activity whose state is to be returned.
     * @return The state of the specified activity (see {@link ActivityState}).
     */
    static CARAPI_(Int32) GetStateForActivity(
        /* [in] */ IActivity* activity);

    /**
     * @return The state of the application (see {@link ApplicationState}).
     */
    static CARAPI_(Int32) GetStateForApplication();

    /**
     * Checks whether or not any Activity in this Application is visible to the user.  Note that
     * this includes the PAUSED state, which can happen when the Activity is temporarily covered
     * by another Activity's Fragment (e.g.).
     * @return Whether any Activity under this Application is visible.
     */
    static CARAPI_(Boolean) HasVisibleActivities();

    /**
     * Checks to see if there are any active Activity instances being watched by ApplicationStatus.
     * @return True if all Activities have been destroyed.
     */
    static CARAPI_(Boolean) IsEveryActivityDestroyed();

    /**
     * Registers the given listener to receive state changes for all activities.
     * @param listener Listener to receive state changes.
     */
    static CARAPI_(void) RegisterStateListenerForAllActivities(
        /* [in] */ ActivityStateListener* listener);

    /**
     * Registers the given listener to receive state changes for {@code activity}.  After a call to
     * {@link ActivityStateListener#onActivityStateChange(Activity, int)} with
     * {@link ActivityState#DESTROYED} all listeners associated with that particular
     * {@link Activity} are removed.
     * @param listener Listener to receive state changes.
     * @param activity Activity to track or {@code null} to track all activities.
     */
    static CARAPI_(void) RegisterStateListenerForActivity(
        /* [in] */ ActivityStateListener* listener,
        /* [in] */ IActivity* activity);

    /**
     * Unregisters the given listener from receiving activity state changes.
     * @param listener Listener that doesn't want to receive state changes.
     */
    static CARAPI_(void) UnregisterActivityStateListener(
        /* [in] */ ActivityStateListener* listener);

    /**
     * Registers the given listener to receive state changes for the application.
     * @param listener Listener to receive state state changes.
     */
    static CARAPI_(void) RegisterApplicationStateListener(
        /* [in] */ ApplicationStateListener* listener);

    /**
     * Unregisters the given listener from receiving state changes.
     * @param listener Listener that doesn't want to receive state changes.
     */
    static CARAPI_(void) UnregisterApplicationStateListener(
        /* [in] */ ApplicationStateListener* listener);

private:
    ApplicationStatus();

    /**
     * Registers the single thread-safe native activity status listener.
     * This handles the case where the caller is not on the main thread.
     * Note that this is used by a leaky singleton object from the native
     * side, hence lifecycle management is greatly simplified.
     */
    //@CalledByNative
    static CARAPI_(void) RegisterThreadSafeNativeApplicationStateListener();

    /**
     * Determines the current application state as defined by {@link ApplicationState}.  This will
     * loop over all the activities and check their state to determine what the general application
     * state should be.
     * @return HAS_RUNNING_ACTIVITIES if any activity is not paused, stopped, or destroyed.
     *         HAS_PAUSED_ACTIVITIES if none are running and one is paused.
     *         HAS_STOPPED_ACTIVITIES if none are running/paused and one is stopped.
     *         HAS_DESTROYED_ACTIVITIES if none are running/paused/stopped.
     */
    static CARAPI_(Int32) DetermineApplicationState();

    // Called to notify the native side of state changes.
    // IMPORTANT: This is always called on the main thread!
    static CARAPI_(void) NativeOnApplicationStateChange(
        /* [in] */ Int32 newState);

    /**
     * Must be called by the main activity when it changes state.
     *
     * @param activity Current activity.
     * @param newState New state value.
     */
    static CARAPI_(void) OnStateChange(
        /* [in] */ IActivity* activity,
        /* [in] */ Int32 newState);

private:
    static AutoPtr<IApplication> sApplication;

    static Object sCachedApplicationStateLock;
    static AutoPtr<IInteger32> sCachedApplicationState;

    /** Last activity that was shown (or null if none or it was destroyed). */
    static AutoPtr<IActivity> sActivity;

    /** A lazily initialized listener that forwards application state changes to native. */
    static AutoPtr<ApplicationStateListener> sNativeApplicationStateListener;

    /**
     * A map of which observers listen to state changes from which {@link Activity}.
     */
    static final Map<Activity, ActivityInfo> sActivityInfo =
            new ConcurrentHashMap<Activity, ActivityInfo>();

    /**
     * A list of observers to be notified when any {@link Activity} has a state change.
     */
    static final ObserverList<ActivityStateListener> sGeneralActivityStateListeners =
            new ObserverList<ActivityStateListener>();

    /**
     * A list of observers to be notified when the visibility state of this {@link Application}
     * changes.  See {@link #getStateForApplication()}.
     */
    static final ObserverList<ApplicationStateListener> sApplicationStateListeners =
            new ObserverList<ApplicationStateListener>();
};

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_BASE_APPLICATIONSTATUS_H__
