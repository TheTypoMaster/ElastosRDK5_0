
#ifndef __ELASTOS_DROID_VIEW_ACTIONPROVIDER_H__
#define  __ELASTOS_DROID_VIEW_ACTIONPROVIDER_H__

#include "ext/frameworkext.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::View::IActionProvider;

namespace Elastos {
namespace Droid {
namespace View {

/**
 * An ActionProvider defines rich menu interaction in a single component.
 * ActionProvider can generate action views for use in the action bar,
 * dynamically populate submenus of a MenuItem, and handle default menu
 * item invocations.
 *
 * <p>An ActionProvider can be optionally specified for a {@link MenuItem} and will be
 * responsible for creating the action view that appears in the {@link android.app.ActionBar}
 * in place of a simple button in the bar. When the menu item is presented in a way that
 * does not allow custom action views, (e.g. in an overflow menu,) the ActionProvider
 * can perform a default action.</p>
 *
 * <p>There are two ways to use an action provider:
 * <ul>
 * <li>
 * Set the action provider on a {@link MenuItem} directly by calling
 * {@link MenuItem#setActionProvider(ActionProvider)}.
 * </li>
 * <li>
 * Declare the action provider in an XML menu resource. For example:
 * <pre>
 * <code>
 *   &lt;item android:id="@+id/my_menu_item"
 *     android:title="Title"
 *     android:icon="@drawable/my_menu_item_icon"
 *     android:showAsAction="ifRoom"
 *     android:actionProviderClass="foo.bar.SomeActionProvider" /&gt;
 * </code>
 * </pre>
 * </li>
 * </ul>
 * </p>
 *
 * @see MenuItem#setActionProvider(ActionProvider)
 * @see MenuItem#getActionProvider()
 */
class ActionProvider
{
public:
    virtual CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid) = 0;
    /**
     * Factory method called by the Android framework to create new action views.
     *
     * <p>This method has been deprecated in favor of {@link #onCreateActionView(MenuItem)}.
     * Newer apps that wish to support platform versions prior to API 16 should also
     * implement this method to return a valid action view.</p>
     *
     * @return A new action view.
     *
     * @deprecated use {@link #onCreateActionView(MenuItem)}
     */
    virtual CARAPI_(AutoPtr<IView>) OnCreateActionView() = 0;

    /**
     * Factory method called by the Android framework to create new action views.
     * This method returns a new action view for the given MenuItem.
     *
     * <p>If your ActionProvider implementation overrides the deprecated no-argument overload
     * {@link #onCreateActionView()}, overriding this method for devices running API 16 or later
     * is recommended but optional. The default implementation calls {@link #onCreateActionView()}
     * for compatibility with applications written for older platform versions.</p>
     *
     * @param forItem MenuItem to create the action view for
     * @return the new action view
     */
    virtual CARAPI_(AutoPtr<IView>) OnCreateActionView(
        /* [in] */ IMenuItem* forItem);

    /**
     * The result of this method determines whether or not {@link #isVisible()} will be used
     * by the {@link MenuItem} this ActionProvider is bound to help determine its visibility.
     *
     * @return true if this ActionProvider overrides the visibility of the MenuItem
     *         it is bound to, false otherwise. The default implementation returns false.
     * @see #isVisible()
     */
    virtual CARAPI_(Boolean) OverridesItemVisibility();

    /**
     * If {@link #overridesItemVisibility()} returns true, the return value of this method
     * will help determine the visibility of the {@link MenuItem} this ActionProvider is bound to.
     *
     * <p>If the MenuItem's visibility is explicitly set to false by the application,
     * the MenuItem will not be shown, even if this method returns true.</p>
     *
     * @return true if the MenuItem this ActionProvider is bound to is visible, false if
     *         it is invisible. The default implementation returns true.
     */
    virtual CARAPI_(Boolean) IsVisible();

    /**
     * If this ActionProvider is associated with an item in a menu,
     * refresh the visibility of the item based on {@link #overridesItemVisibility()} and
     * {@link #isVisible()}. If {@link #overridesItemVisibility()} returns false, this call
     * will have no effect.
     */
    virtual CARAPI RefreshVisibility();

    /**
     * Performs an optional default action.
     * <p>
     * For the case of an action provider placed in a menu item not shown as an action this
     * method is invoked if previous callbacks for processing menu selection has handled
     * the event.
     * </p>
     * <p>
     * A menu item selection is processed in the following order:
     * <ul>
     * <li>
     * Receiving a call to {@link MenuItem.OnMenuItemClickListener#onMenuItemClick
     *  MenuItem.OnMenuItemClickListener.onMenuItemClick}.
     * </li>
     * <li>
     * Receiving a call to {@link android.app.Activity#onOptionsItemSelected(MenuItem)
     *  Activity.onOptionsItemSelected(MenuItem)}
     * </li>
     * <li>
     * Receiving a call to {@link android.app.Fragment#onOptionsItemSelected(MenuItem)
     *  Fragment.onOptionsItemSelected(MenuItem)}
     * </li>
     * <li>
     * Launching the {@link android.content.Intent} set via
     * {@link MenuItem#setIntent(android.content.Intent) MenuItem.setIntent(android.content.Intent)}
     * </li>
     * <li>
     * Invoking this method.
     * </li>
     * </ul>
     * </p>
     * <p>
     * The default implementation does not perform any action and returns false.
     * </p>
     */
    virtual CARAPI_(Boolean) OnPerformDefaultAction();

    /**
     * Determines if this ActionProvider has a submenu associated with it.
     *
     * <p>Associated submenus will be shown when an action view is not. This
     * provider instance will receive a call to {@link #onPrepareSubMenu(SubMenu)}
     * after the call to {@link #onPerformDefaultAction()} and before a submenu is
     * displayed to the user.
     *
     * @return true if the item backed by this provider should have an associated submenu
     */
    virtual CARAPI_(Boolean) HasSubMenu();

    /**
     * Called to prepare an associated submenu for the menu item backed by this ActionProvider.
     *
     * <p>if {@link #hasSubMenu()} returns true, this method will be called when the
     * menu item is selected to prepare the submenu for presentation to the user. Apps
     * may use this to create or alter submenu content right before display.
     *
     * @param subMenu Submenu that will be displayed
     */
    virtual CARAPI OnPrepareSubMenu(
        /* [in] */ ISubMenu* subMenu);

    /**
     * Notify the system that the visibility of an action view's sub-UI such as
     * an anchored popup has changed. This will affect how other system
     * visibility notifications occur.
     *
     * @hide Pending future API approval
     */
    virtual CARAPI SubUiVisibilityChanged(
        /* [in] */ Boolean isVisible);

    /**
     * @hide Internal use only
     */
    virtual CARAPI SetSubUiVisibilityListener(
        /* [in] */ ISubUiVisibilityListener* listener);

    /**
     * Set a listener to be notified when this ActionProvider's overridden visibility changes.
     * This should only be used by MenuItem implementations.
     *
     * @param listener listener to set
     */
    virtual CARAPI SetVisibilityListener(
        /* [in] */ IVisibilityListener* listener);

private:
    static String TAG;
    AutoPtr<ISubUiVisibilityListener> mSubUiVisibilityListener;
    AutoPtr<IVisibilityListener> mVisibilityListener;
};

} // namespace View
} // namespace Droid
} // namespace Elastos

#endif  // __ELASTOS_DROID_VIEW_ACTIONPROVIDER_H__
