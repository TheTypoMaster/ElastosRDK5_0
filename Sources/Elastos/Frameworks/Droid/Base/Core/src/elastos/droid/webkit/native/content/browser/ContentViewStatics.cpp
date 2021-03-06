
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

/**
 * Return the first substring consisting of the address of a physical location.
 * @see {@link android.webkit.WebView#findAddress(String)}
 *
 * @param addr The string to search for addresses.
 * @return the address, or if no address is found, return null.
 */
String ContentViewStatics::FindAddress(
    /* [in] */ String addr)
{
    if (addr == NULL) {
        //throw new NullPointerException("addr is null");
        assert(0);
    }

    String result = NativeFindAddress(addr);

    return result == NULL || result.IsEmpty() ? NULL : result;
}

/**
 * Suspends Webkit timers in all renderers.
 * New renderers created after this call will be created with the
 * default options.
 *
 * @param suspend true if timers should be suspended.
 */
void ContentViewStatics::SetWebKitSharedTimersSuspended(
    /* [in] */ Boolean suspend)
{
    NativeSetWebKitSharedTimersSuspended(suspend);
}

/**
 * Enables platform notifications of data state and proxy changes.
 * Notifications are enabled by default.
 */
void ContentViewStatics::EnablePlatformNotifications()
{
    ProxyChangeListener::SetEnabled(TRUE);
}

/**
 * Disables platform notifications of data state and proxy changes.
 * Notifications are enabled by default.
 */
void ContentViewStatics::DisablePlatformNotifications ()
{
    ProxyChangeListener::SetEnabled(FALSE);
}

// Native functions

String ContentViewStatics::NativeFindAddress(
    /* [in] */ String addr)
{
}

void ContentViewStatics::NativeSetWebKitSharedTimersSuspended(
    /* [in] */ Boolean suspend)
{
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
