
#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCONTENTS_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCONTENTS_H__

// import android.annotation.SuppressLint;
// import android.app.Activity;
// import android.content.ComponentCallbacks2;
// import android.content.Context;
// import android.content.res.Configuration;
// import android.graphics.Bitmap;
// import android.graphics.Canvas;
// import android.graphics.Color;
// import android.graphics.Paint;
// import android.graphics.Picture;
// import android.graphics.Rect;
// import android.net.Uri;
// import android.net.http.SslCertificate;
// import android.os.AsyncTask;
// import android.os.Build;
// import android.os.Bundle;
// import android.os.Handler;
// import android.os.Message;
// import android.text.TextUtils;
// import android.util.Log;
// import android.util.Pair;
// import android.view.KeyEvent;
// import android.view.MotionEvent;
// import android.view.View;
// import android.view.ViewGroup;
// import android.view.accessibility.AccessibilityEvent;
// import android.view.accessibility.AccessibilityNodeInfo;
// import android.view.accessibility.AccessibilityNodeProvider;
// import android.view.inputmethod.EditorInfo;
// import android.view.inputmethod.InputConnection;
// import android.webkit.GeolocationPermissions;
// import android.webkit.ValueCallback;
// import android.widget.OverScroller;

// import com.google.common.annotations.VisibleForTesting;

// import org.chromium.android_webview.permission.AwPermissionRequest;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import org.chromium.base.ThreadUtils;
// import org.chromium.components.navigation_interception.InterceptNavigationDelegate;
// import org.chromium.components.navigation_interception.NavigationParams;
// import org.chromium.content.browser.ContentSettings;
// import org.chromium.content.browser.ContentViewClient;
// import org.chromium.content.browser.ContentViewCore;
// import org.chromium.content.browser.ContentViewStatics;
// import org.chromium.content.browser.LoadUrlParams;
// import org.chromium.content.browser.NavigationHistory;
// import org.chromium.content.browser.PageTransitionTypes;
// import org.chromium.content.browser.SmartClipProvider;
// import org.chromium.content.common.CleanupReference;
// import org.chromium.content_public.Referrer;
// import org.chromium.content_public.browser.GestureStateListener;
// import org.chromium.ui.base.ActivityWindowAndroid;
// import org.chromium.ui.base.WindowAndroid;
// import org.chromium.ui.gfx.DeviceDisplayInfo;

// import java.io.File;
// import java.lang.annotation.Annotation;
// import java.net.MalformedURLException;
// import java.net.URL;
// import java.util.HashMap;
// import java.util.Locale;
// import java.util.Map;
// import java.util.concurrent.Callable;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * Exposes the native AwContents class, and together these classes wrap the ContentViewCore
 * and Browser components that are required to implement Android WebView API. This is the
 * primary entry point for the WebViewProvider implementation; it holds a 1:1 object
 * relationship with application WebView instances.
 * (We define this class independent of the hidden WebViewProvider interfaces, to allow
 * continuous build & test in the open source SDK-based tree).
 */
//@JNINamespace("android_webview")
class AwContents
    : public Object
    , public SmartClipProvider
{
public:
    /**
     * WebKit hit test related data strcutre. These are used to implement
     * getHitTestResult, requestFocusNodeHref, requestImageRef methods in WebView.
     * All values should be updated together. The native counterpart is
     * AwHitTestData.
     */
    struct HitTestData
    {
        HitTestData()
            : hitTestResultType(0)
        {
        }

        // Used in getHitTestResult.
        Int32 hitTestResultType;
        String hitTestResultExtraData;

        // Used in requestFocusNodeHref (all three) and requestImageRef (only imgSrc).
        String href;
        String anchorText;
        String imgSrc;
    };

    /**
     * Interface that consumers of {@link AwContents} must implement to allow the proper
     * dispatching of view methods through the containing view.
     */
    class InternalAccessDelegate : public ContentViewCore::InternalAccessDelegate
    {
    public:
        /**
         * @see View#overScrollBy(int, int, int, int, int, int, int, int, boolean);
         */
        virtual CARAPI_(void) OverScrollBy(
            /* [in] */ Int32 deltaX,
            /* [in] */ Int32 deltaY,
            /* [in] */ Int32 scrollX,
            /* [in] */ Int32 scrollY,
            /* [in] */ Int32 scrollRangeX,
            /* [in] */ Int32 scrollRangeY,
            /* [in] */ Int32 maxOverScrollX,
            /* [in] */ Int32 maxOverScrollY,
            /* [in] */ Boolean isTouchEvent) = 0;

        /**
         * @see View#scrollTo(int, int)
         */
        virtual CARAPI_(void) Super_scrollTo(
            /* [in] */ Int32 scrollX,
            /* [in] */ Int32 scrollY) = 0;

        /**
         * @see View#setMeasuredDimension(int, int)
         */
        virtual CARAPI_(void) SetMeasuredDimension(
            /* [in] */ Int32 measuredWidth,
            /* [in] */ Int32 measuredHeight) = 0;

        /**
         * @see View#getScrollBarStyle()
         */
        virtual CARAPI_(Int32) Super_getScrollBarStyle() = 0;
    };

    /**
     * Interface that consumers of {@link AwContents} must implement to support
     * native GL rendering.
     */
    class NativeGLDelegate
    {
    public:
        /**
         * Requests a callback on the native DrawGL method (see getAwDrawGLFunction)
         * if called from within onDraw, |canvas| will be non-null and hardware accelerated.
         * Otherwise, |canvas| will be null, and the container view itself will be hardware
         * accelerated. If |waitForCompletion| is true, this method will not return until
         * functor has returned.
         * Should avoid setting |waitForCompletion| when |canvas| is not null.
         * |containerView| is the view where the AwContents should be drawn.
         *
         * @return false indicates the GL draw request was not accepted, and the caller
         *         should fallback to the SW path.
         */
        virtual CARAPI_(Boolean) RequestDrawGL(
            /* [in] */ ICanvas* canvas,
            /* [in] */ Boolean waitForCompletion,
            /* [in] */ IView* containerView) = 0;

        /**
         * Detaches the GLFunctor from the view tree.
         */
        virtual CARAPI_(void) DetachGLFunctor() = 0;
    };

    /**
     * Class to facilitate dependency injection. Subclasses by test code to provide mock versions of
     * certain AwContents dependencies.
     */
    class DependencyFactory
    {
    public:
        virtual CARAPI_(AutoPtr<AwLayoutSizer>) CreateLayoutSizer();

        virtual CARAPI_(AutoPtr<AwScrollOffsetManager>) CreateScrollOffsetManager(
            /* [in] */ AwScrollOffsetManager::Delegate* delegate,
            /* [in] */ IOverScroller* overScroller);
    };

private:
    class DestroyRunnable
        : public Object
        , public IRunnable
    {
    public:
        //@Override
        CARAPI Run();

    private:
        DestroyRunnable(
            /* [in] */ Int64 nativeAwContents);

        const Int64 mNativeAwContents;
    };

    /**
     * A class that stores the state needed to enter and exit fullscreen.
     */
    class FullScreenTransitionsState
    {
    private:
        FullScreenTransitionsState(
            /* [in] */ IViewGroup* initialContainerView,
            /* [in] */ InternalAccessDelegate* initialInternalAccessAdapter,
            /* [in] */ AwViewMethods* initialAwViewMethods);

        CARAPI_(void) EnterFullScreen(
            /* [in] */ FullScreenView* fullScreenView);

        CARAPI_(void) ExitFullScreen();

        CARAPI_(Boolean) IsFullScreen();

        CARAPI_(AutoPtr<IViewGroup>) GetInitialContainerView();

        CARAPI_(AutoPtr<InternalAccessDelegate>) GetInitialInternalAccessDelegate();

        CARAPI_(AutoPtr<AwViewMethods>) GetInitialAwViewMethods();

        CARAPI_(AutoPtr<FullScreenView>) GetFullScreenView();

    private:
        const AutoPtr<IViewGroup> mInitialContainerView;
        const AutoPtr<InternalAccessDelegate> mInitialInternalAccessAdapter;
        const AutoPtr<AwViewMethods> mInitialAwViewMethods;
        AutoPtr<FullScreenView> mFullScreenView;
    };

    //--------------------------------------------------------------------------------------------
    class IoThreadClientImpl : public AwContentsIoThreadClient
    {
    public:
        IoThreadClientImpl(
            /* [in] */ AwContents* owner);

        // All methods are called on the IO thread.

        //@Override
        CARAPI_(Int32) GetCacheMode();

        //@Override
        CARAPI_(AutoPtr<AwWebResourceResponse>) ShouldInterceptRequest(
            /* [in] */ AwContentsClient::ShouldInterceptRequestParams* params);

        //@Override
        CARAPI_(Boolean) ShouldBlockContentUrls();

        //@Override
        CARAPI_(Boolean) ShouldBlockFileUrls();

        //@Override
        CARAPI_(Boolean) ShouldBlockNetworkLoads();

        //@Override
        CARAPI_(Boolean) ShouldAcceptThirdPartyCookies();

        //@Override
        CARAPI_(void) OnDownloadStart(
            /* [in] */ String url,
            /* [in] */ String userAgent,
            /* [in] */ String contentDisposition,
            /* [in] */ String mimeType,
            /* [in] */ Int64 contentLength);

        //@Override
        CARAPI_(void) NewLoginRequest(
            /* [in] */ String realm,
            /* [in] */ String account,
            /* [in] */ String args);

    private:
        AwContents* mOwner;
    };

    //--------------------------------------------------------------------------------------------
    // When the navigation is for a newly created WebView (i.e. a popup), intercept the navigation
    // here for implementing shouldOverrideUrlLoading. This is to send the shouldOverrideUrlLoading
    // callback to the correct WebViewClient that is associated with the WebView.
    // Otherwise, use this delegate only to post onPageStarted messages.
    //
    // We are not using WebContentsObserver.didStartLoading because of stale URLs, out of order
    // onPageStarted's and double onPageStarted's.
    //
    class InterceptNavigationDelegateImpl implements InterceptNavigationDelegate
    {
    public:
        InterceptNavigationDelegateImpl(
            /* [in] */ AwContents* owner);

        //@Override
        CARAPI_(Boolean) ShouldIgnoreNavigation(
            /* [in] */ NavigationParams* navigationParams);

    private:
        AwContents* mOwner;
    };

    //--------------------------------------------------------------------------------------------
    class AwLayoutSizerDelegate : public AwLayoutSizer::Delegate
    {
    public:
        AwLayoutSizerDelegate(
            /* [in] */ AwContents* owner);

        //@Override
        CARAPI_(void) RequestLayout();

        //@Override
        CARAPI_(void) SetMeasuredDimension(
            /* [in] */ Int32 measuredWidth,
            /* [in] */ Int32 measuredHeight);

        //@Override
        CARAPI_(Boolean) IsLayoutParamsHeightWrapContent();

        //@Override
        CARAPI_(void) SetForceZeroLayoutHeight(
            /* [in] */ Boolean forceZeroHeight);

    private:
        AwContents* mOwner;
    };

    //--------------------------------------------------------------------------------------------
    class AwScrollOffsetManagerDelegate : public AwScrollOffsetManager::Delegate
    {
    public:
        AwScrollOffsetManagerDelegate(
            /* [in] */ AwContents* owner);

        //@Override
        CARAPI_(void) OverScrollContainerViewBy(
            /* [in] */ Int32 deltaX,
            /* [in] */ Int32 deltaY,
            /* [in] */ Int32 scrollX,
            /* [in] */ Int32 scrollY,
            /* [in] */ Int32 scrollRangeX,
            /* [in] */ Int32 scrollRangeY,
            /* [in] */ Boolean isTouchEvent);

        //@Override
        CARAPI_(void) ScrollContainerViewTo(
            /* [in] */ Int32 x,
            /* [in] */ Int32 y);

        //@Override
        CARAPI_(void) ScrollNativeTo(
            /* [in] */ Int32 x,
            /* [in] */ Int32 y);

        //@Override
        CARAPI_(Int32) GetContainerViewScrollX();

        //@Override
        CARAPI_(Int32) GetContainerViewScrollY();

        //@Override
        CARAPI_(void) Invalidate();

    private:
        AwContents* mOwner;
    };

    //--------------------------------------------------------------------------------------------
    class AwGestureStateListener : public GestureStateListener
    {
    public:
        AwGestureStateListener(
            /* [in] */ AwContents* owner);

        //@Override
        CARAPI_(void) OnPinchStarted();

        //@Override
        CARAPI_(void) OnPinchEnded();

        //@Override
        CARAPI_(void) OnFlingCancelGesture();

        //@Override
        CARAPI_(void) OnUnhandledFlingStartEvent(
            /* [in] */ Int32 velocityX,
            /* [in] */ Int32 velocityY);

        //@Override
        CARAPI_(void) OnScrollUpdateGestureConsumed();

    private:
        AwContents* mOwner;
    };

    //--------------------------------------------------------------------------------------------
    class AwComponentCallbacks
        : public Object
        , public IComponentCallbacks2
    {
    public:
        AwComponentCallbacks(
            /* [in] */ AwContents* owner);

        //@Override
        CARAPI_(void) OnTrimMemory(
            /* [in] */ Int32 level);

        //@Override
        CARAPI_(void) OnLowMemory();

        //@Override
        CARAPI_(void) OnConfigurationChanged(
            /* [in] */ IConfiguration* configuration);

    private:
        AwContents* mOwner;
    };

    //--------------------------------------------------------------------------------------------
    class AwLayoutChangeListener
        : public Object
        , public IViewOnLayoutChangeListener
    {
    public:
        AwLayoutChangeListener(
            /* [in] */ AwContents* owner);

        //@Override
        CARAPI OnLayoutChange(
            /* [in] */ IView* v,
            /* [in] */ Int32 left,
            /* [in] */ Int32 top,
            /* [in] */ Int32 right,
            /* [in] */ Int32 bottom,
            /* [in] */ Int32 oldLeft,
            /* [in] */ Int32 oldTop,
            /* [in] */ Int32 oldRight,
            /* [in] */ Int32 oldBottom);

    private:
        AwContents* mOwner;
    };

    class AwGeolocationCallback
        : public Object
        , public IGeolocationPermissionsCallback
    {
    private:
        class InnerRunnable
            : public Object
            , public IRunnable
        {
        public:
            InnerRunnable(
                /* [in] */ AwGeolocationCallback* owner,
                /* [in] */ const String& origin,
                /* [in] */ Boolean allow,
                /* [in] */ Boolean retain);

            CARAPI Run();

        private:
            AwGeolocationCallback* mOwner;
            const String& mOrigin;
            Boolean mAllow,
            Boolean mRetain;
        };

    public:
        AwGeolocationCallback(
            /* [in] */ AwContents* owner);

        //@Override
        CARAPI Invoke(
            /* [in] */ const String& origin,
            /* [in] */ Boolean allow,
            /* [in] */ Boolean retain);

    private:
        AwContents* mOwner;
    };

    // --------------------------------------------------------------------------------------------
    // This is the AwViewMethods implementation that does real work. The AwViewMethodsImpl is
    // hooked up to the WebView in embedded mode and to the FullScreenView in fullscreen mode,
    // but not to both at the same time.
    class AwViewMethodsImpl : public AwViewMethods
    {
    public:
        AwViewMethodsImpl(
            /* [in] */ AwContents* owner);

        //@Override
        CARAPI_(void) OnDraw(
            /* [in] */ ICanvas* canvas);

        //@Override
        CARAPI_(void) OnMeasure(
            /* [in] */ Int32 widthMeasureSpec,
            /* [in] */ Int32 heightMeasureSpec);

        //@Override
        CARAPI_(void) RequestFocus();

        //@Override
        CARAPI_(void) SetLayerType(
            /* [in] */ Int32 layerType,
            /* [in] */ IPaint* paint);

        //@Override
        CARAPI_(AutoPtr<IInputConnection>) OnCreateInputConnection(
            /* [in] */ IEditorInfo* outAttrs);

        //@Override
        CARAPI_(Boolean) OnKeyUp(
            /* [in] */ Int32 keyCode,
            /* [in] */ IKeyEvent* event);

        //@Override
        CARAPI_(Boolean) DispatchKeyEvent(
            /* [in] */ IKeyEvent* event);

        //@Override
        CARAPI_(Boolean) OnTouchEvent(
            /* [in] */ IMotionEvent* event);

        //@Override
        CARAPI_(Boolean) OnHoverEvent(
            /* [in] */ IMotionEvent* event);

        //@Override
        CARAPI_(Boolean) OnGenericMotionEvent(
            /* [in] */ IMotionEvent* event);

        //@Override
        CARAPI_(void) OnConfigurationChanged(
            /* [in] */ IConfiguration* newConfig);

        //@Override
        CARAPI_(void) OnAttachedToWindow();

        //@Override
        CARAPI_(void) OnDetachedFromWindow();

        //@Override
        CARAPI_(void) OnWindowFocusChanged(
            /* [in] */ Boolean hasWindowFocus);

        //@Override
        CARAPI_(void) OnFocusChanged(
            /* [in] */ Boolean focused,
            /* [in] */ Int32 direction,
            /* [in] */ IRect* previouslyFocusedRect);

        //@Override
        CARAPI_(void) OnSizeChanged(
            /* [in] */ Int32 w,
            /* [in] */ Int32 h,
            /* [in] */ Int32 ow,
            /* [in] */ Int32 oh);

        //@Override
        CARAPI_(void) OnVisibilityChanged(
            /* [in] */ IView* changedView,
            /* [in] */ Int32 visibility);

        //@Override
        CARAPI_(void) OnWindowVisibilityChanged(
            /* [in] */ Int32 visibility);

    private:
        CARAPI_(void) UpdateHardwareAcceleratedFeaturesToggle();

        CARAPI_(Boolean) IsDpadEvent(
            /* [in] */ IKeyEvent* event);

    private:
        AwContents* mOwner;
        // Only valid within software onDraw().
        const Rect mClipBoundsTemporary = new Rect();
        Int32 mLayerType = View.LAYER_TYPE_NONE;
        AutoPtr<IComponentCallbacks2> mComponentCallbacks;
    };

    class InnerZoomSupportChangeListener
        : public Object
        , public ZoomSupportChangeListener
    {
    public:
        InnerZoomSupportChangeListener(
            /* [in] */ AwContents* owner);

        //@Override
        CARAPI_(void) OnGestureZoomSupportChanged(
            /* [in] */ Boolean supportsDoubleTapZoom,
            /* [in] */ Boolean supportsMultiTouchZoom);

    private:
        AwContents* mOwner;
    };

    class InnerCallable
        : public Object
        , public ICallable
    {
    public:
        InnerCallable(
            /* [in] */ AwContents* owner);

        CARAPI Call(
            /* [out] */ IPicture** pic);

    private:
        AwContents* mOwner;
    };

    class InnerValueCallback
        : public Object
        , public IValueCallback
    {
    private:
        class InnerRunnable
            : public Object
            , public IRunnable
        {
        public:
            InnerRunnable(
                /* [in] */ InnerValueCallback* owner,
                /* [in] */ ArrayOf<String>* value);

            CARAPI Run();

        private:
            InnerValueCallback* mOwner;
            ArrayOf<String>* mValue;
        };

    public:
        InnerValueCallback(
            /* [in] */ AwContents* owner);

        CARAPI OnReceiveValue(
            /* [in] */ ArrayOf<String>* value);

    private:
        AwContents* mOwner;
    };

    class InnerJavaScriptCallback : public ContentViewCore::JavaScriptCallback
    {
    public:
        InnerJavaScriptCallback(
            /* [in] */ AwContents* owner,
            /* [in] */ IValueCallback* callback);

        CARAPI_(void) HandleJavaScriptResult(
            /* [in] */ String jsonResult);

    private:
        AwContents* mOwner;
        IValueCallback* mCallback;
    };

    class SaveWebArchiveInternalRunnable
        : public Object
        , public IRunnable
    {
    public:
        SaveWebArchiveInternalRunnable(
            /* [in] */ AwContents* owner,
            /* [in] */ const IValueCallback* callback);

        CARAPI Run();

    private:
        AwContents* mOwner;
        const IValueCallback* mCallback;
    };

    class InnerSmartClipDataListener
        : public Object
        , public ContentViewCore::SmartClipDataListener
    {
    public:
        InnerSmartClipDataListener(
            /* [in] */ AwContents* owner,
            /* [in] */ const IHandler* resultHandler);

        CARAPI_(void) OnSmartClipDataExtracted(
            /* [in] */ String text,
            /* [in] */ String html,
            /* [in] */ IRect* clipRect);

    private:
        AwContents* mOwner;
        const IHandler* mResultHandler;
    };

public:
    /**
     * Key for opaque state in bundle. Note this is only public for tests.
     */
    static const String SAVE_RESTORE_STATE_KEY;

public:
    /**
     * @param browserContext the browsing context to associate this view contents with.
     * @param containerView the view-hierarchy item this object will be bound to.
     * @param context the context to use, usually containerView.getContext().
     * @param internalAccessAdapter to access private methods on containerView.
     * @param nativeGLDelegate to access the GL functor provided by the WebView.
     * @param contentsClient will receive API callbacks from this WebView Contents.
     * @param awSettings AwSettings instance used to configure the AwContents.
     *
     * This constructor uses the default view sizing policy.
     */
    AwContents(
        /* [in] */ AwBrowserContext* browserContext,
        /* [in] */ IViewGroup* containerView,
        /* [in] */ IContext* context,
        /* [in] */ InternalAccessDelegate* internalAccessAdapter,
        /* [in] */ NativeGLDelegate* nativeGLDelegate,
        /* [in] */ AwContentsClient* contentsClient,
        /* [in] */ AwSettings* awSettings);

    /**
     * @param dependencyFactory an instance of the DependencyFactory used to provide instances of
     *                          classes that this class depends on.
     *
     * This version of the constructor is used in test code to inject test versions of the above
     * documented classes.
     */
    AwContents(
        /* [in] */ AwBrowserContext* browserContext,
        /* [in] */ IViewGroup* containerView,
        /* [in] */ IContext* context,
        /* [in] */ InternalAccessDelegate* internalAccessAdapter,
        /* [in] */ NativeGLDelegate* nativeGLDelegate,
        /* [in] */ AwContentsClient* contentsClient,
        /* [in] */ AwSettings* settings,
        /* [in] */ DependencyFactory* dependencyFactory);

    virtual CARAPI_(Boolean) IsFullScreen();

    /**
     * Transitions this {@link AwContents} to fullscreen mode and returns the
     * {@link View} where the contents will be drawn while in fullscreen.
     */
    virtual CARAPI_(AutoPtr<IView>) EnterFullScreen();

    /**
     * Returns this {@link AwContents} to embedded mode, where the {@link AwContents} are drawn
     * in the WebView.
     */
    virtual CARAPI_(void) ExitFullScreen();

    /**
     * Called on the "source" AwContents that is opening the popup window to
     * provide the AwContents to host the pop up content.
     */
    virtual CARAPI_(void) SupplyContentsForPopup(
        /* [in] */ AwContents* newContents);

    /**
     * Deletes the native counterpart of this object.
     */
    virtual CARAPI_(void) Destroy();

    //@VisibleForTesting
    virtual CARAPI_(AutoPtr<ContentViewCore>) GetContentViewCore();

    // Can be called from any thread.
    virtual CARAPI_(AutoPtr<AwSettings>) GetSettings();

    virtual CARAPI_(AutoPtr<AwPdfExporter>) GetPdfExporter();

    static CARAPI_(void) SetAwDrawSWFunctionTable(
        /* [in] */ Int64 functionTablePointer);

    static CARAPI_(void) SetAwDrawGLFunctionTable(
        /* [in] */ Int64 functionTablePointer);

    static CARAPI_(Int64) GetAwDrawGLFunction();

    static CARAPI_(void) SetShouldDownloadFavicons();

    /**
     * Disables contents of JS-to-Java bridge objects to be inspectable using
     * Object.keys() method and "for .. in" loops. This is intended for applications
     * targeting earlier Android releases where this was not possible, and we want
     * to ensure backwards compatible behavior.
     */
    virtual CARAPI_(void) DisableJavascriptInterfacesInspection();

    /**
     * Intended for test code.
     * @return the number of native instances of this class.
     */
    //@VisibleForTesting
    static CARAPI_(Int32) GetNativeInstanceCount();

    virtual CARAPI_(Int64) GetAwDrawGLViewContext();

    //--------------------------------------------------------------------------------------------
    //  WebView[Provider] method implementations (where not provided by ContentViewCore)
    //--------------------------------------------------------------------------------------------

    virtual CARAPI_(void) OnDraw(
        /* [in] */ ICanvas* canvas);

    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

    virtual CARAPI_(Int32) GetContentHeightCss();

    virtual CARAPI_(Int32) GetContentWidthCss();

    virtual CARAPI_(AutoPtr<IPicture>) CapturePicture();

    virtual CARAPI_(void) ClearView();

    /**
     * Enable the onNewPicture callback.
     * @param enabled Flag to enable the callback.
     * @param invalidationOnly Flag to call back only on invalidation without providing a picture.
     */
    virtual CARAPI_(void) EnableOnNewPicture(
        /* [in] */ Boolean enabled,
        /* [in] */ Boolean invalidationOnly);

    virtual CARAPI_(void) FindAllAsync(
        /* [in] */ String searchString);

    virtual CARAPI_(void) FindNext(
        /* [in] */ Boolean forward);

    virtual CARAPI_(void) ClearMatches();

    /**
     * @return load progress of the WebContents.
     */
    virtual CARAPI_(Int32) GetMostRecentProgress();

    virtual CARAPI_(AutoPtr<IBitmap>) GetFavicon();

    /**
     * Load url without fixing up the url string. Consumers of ContentView are responsible for
     * ensuring the URL passed in is properly formatted (i.e. the scheme has been added if left
     * off during user input).
     *
     * @param params Parameters for this load.
     */
    virtual CARAPI_(void) LoadUrl(
        /* [in] */ LoadUrlParams* params);

    /**
     * Get the URL of the current page.
     *
     * @return The URL of the current page or null if it's empty.
     */
    virtual CARAPI_(String) GetUrl();

    virtual CARAPI_(void) RequestFocus();

    virtual CARAPI_(void) SetBackgroundColor(
        /* [in] */ Int32 color);

    /**
     * @see android.view.View#setLayerType()
     */
    virtual CARAPI_(void) SetLayerType(
        /* [in] */ Int32 layerType,
        /* [in] */ IPaint* paint);

    virtual CARAPI_(Int32) GetEffectiveBackgroundColor();

    virtual CARAPI_(Boolean) IsMultiTouchZoomSupported();

    virtual CARAPI_(AutoPtr<IView>) GetZoomControlsForTest();

    /**
     * @see ContentViewCore#getContentSettings()
     */
    virtual CARAPI_(AutoPtr<ContentSettings>) GetContentSettings();

    /**
     * @see View#setOverScrollMode(int)
     */
    virtual CARAPI_(void) SetOverScrollMode(
        /* [in] */ Int32 mode);

    /**
     * @see View#setScrollBarStyle(int)
     */
    virtual CARAPI_(void) SetScrollBarStyle(
        /* [in] */ Int32 style);

    /**
     * @see View#setHorizontalScrollbarOverlay(boolean)
     */
    virtual CARAPI_(void) SetHorizontalScrollbarOverlay(
        /* [in] */ Boolean overlay);

    /**
     * @see View#setVerticalScrollbarOverlay(boolean)
     */
    virtual CARAPI_(void) SetVerticalScrollbarOverlay(
        /* [in] */ Boolean overlay);

    /**
     * @see View#overlayHorizontalScrollbar()
     */
    virtual CARAPI_(Boolean) OverlayHorizontalScrollbar();

    /**
     * @see View#overlayVerticalScrollbar()
     */
    virtual CARAPI_(Boolean) OverlayVerticalScrollbar();

    /**
     * Called by the embedder when the scroll offset of the containing view has changed.
     * @see View#onScrollChanged(int,int)
     */
    virtual CARAPI_(void) OnContainerViewScrollChanged(
        /* [in] */ Int32 l,
        /* [in] */ Int32 t,
        /* [in] */ Int32 oldl,
        /* [in] */ Int32 oldt);

    /**
     * Called by the embedder when the containing view is to be scrolled or overscrolled.
     * @see View#onOverScrolled(int,int,int,int)
     */
    virtual CARAPI_(void) OnContainerViewOverScrolled(
        /* [in] */ Int32 scrollX,
        /* [in] */ Int32 scrollY,
        /* [in] */ Boolean clampedX,
        /* [in] */ Boolean clampedY);

    /**
     * @see android.webkit.WebView#requestChildRectangleOnScreen(View, Rect, boolean)
     */
    virtual CARAPI_(Boolean) RequestChildRectangleOnScreen(
        /* [in] */ IView* child,
        /* [in] */ IRect* rect,
        /* [in] */ Boolean immediate);

    /**
     * @see View.computeScroll()
     */
    virtual CARAPI_(void) ComputeScroll();

    /**
     * @see View#computeHorizontalScrollRange()
     */
    virtual CARAPI_(Int32) ComputeHorizontalScrollRange();

    /**
     * @see View#computeHorizontalScrollOffset()
     */
    virtual CARAPI_(Int32) ComputeHorizontalScrollOffset();

    /**
     * @see View#computeVerticalScrollRange()
     */
    virtual CARAPI_(Int32) ComputeVerticalScrollRange();

    /**
     * @see View#computeVerticalScrollOffset()
     */
    virtual CARAPI_(Int32) ComputeVerticalScrollOffset();

    /**
     * @see View#computeVerticalScrollExtent()
     */
    virtual CARAPI_(Int32) ComputeVerticalScrollExtent();

    /**
     * @see android.webkit.WebView#stopLoading()
     */
    virtual CARAPI_(void) StopLoading();

    /**
     * @see android.webkit.WebView#reload()
     */
    virtual CARAPI_(void) Reload();

    /**
     * @see android.webkit.WebView#canGoBack()
     */
    virtual CARAPI_(Boolean) CanGoBack();

    /**
     * @see android.webkit.WebView#goBack()
     */
    virtual CARAPI_(void) GoBack();

    /**
     * @see android.webkit.WebView#canGoForward()
     */
    virtual CARAPI_(Boolean) CanGoForward();

    /**
     * @see android.webkit.WebView#goForward()
     */
    virtual CARAPI_(void) GoForward();

    /**
     * @see android.webkit.WebView#canGoBackOrForward(int)
     */
    virtual CARAPI_(Boolean) CanGoBackOrForward(int steps);

    /**
     * @see android.webkit.WebView#goBackOrForward(int)
     */
    virtual CARAPI_(void) GoBackOrForward(int steps);

    /**
     * @see android.webkit.WebView#pauseTimers()
     */
    virtual CARAPI_(void) PauseTimers();

    /**
     * @see android.webkit.WebView#resumeTimers()
     */
    virtual CARAPI_(void) ResumeTimers();

    /**
     * @see android.webkit.WebView#onPause()
     */
    virtual CARAPI_(void) OnPause();

    /**
     * @see android.webkit.WebView#onResume()
     */
    virtual CARAPI_(void) OnResume();

    /**
     * @see android.webkit.WebView#isPaused()
     */
    virtual CARAPI_(Boolean) IsPaused();

    /**
     * @see android.webkit.WebView#onCreateInputConnection(EditorInfo)
     */
    virtual CARAPI_(AutoPtr<IInputConnection>) OnCreateInputConnection(
        /* [in] */ IEditorInfo* outAttrs);

    /**
     * @see android.webkit.WebView#onKeyUp(int, KeyEvent)
     */
    virtual CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event);

    /**
     * @see android.webkit.WebView#dispatchKeyEvent(KeyEvent)
     */
    virtual CARAPI_(Boolean) DispatchKeyEvent(
        /* [in] */ IKeyEvent* event);

    /**
     * Clears the resource cache. Note that the cache is per-application, so this will clear the
     * cache for all WebViews used.
     *
     * @param includeDiskFiles if false, only the RAM cache is cleared
     */
    virtual CARAPI_(void) ClearCache(
        /* [in] */ Boolean includeDiskFiles);

    virtual CARAPI_(void) DocumentHasImages(
        /* [in] */ IMessage* message);

    virtual CARAPI_(void) SaveWebArchive(
        /* [in] */ const String& basename,
        /* [in] */ Boolean autoname,
        /* [in] */ IValueCallback* callback);

    virtual CARAPI_(String) GetOriginalUrl();

    /**
     * @see ContentViewCore#getNavigationHistory()
     */
    virtual CARAPI_(AutoPtr<NavigationHistory>) GetNavigationHistory();

    /**
     * @see android.webkit.WebView#getTitle()
     */
    virtual CARAPI_(String) GetTitle();

    /**
     * @see android.webkit.WebView#clearHistory()
     */
    virtual CARAPI_(void) ClearHistory();

    virtual CARAPI_(AutoPtr< ArrayOf<String> >) GetHttpAuthUsernamePassword(
        /* [in] */ String host,
        /* [in] */ String realm);

    virtual CARAPI_(void) SetHttpAuthUsernamePassword(
        /* [in] */ String host,
        /* [in] */ String realm,
        /* [in] */ String username,
        /* [in] */ String password);

    /**
     * @see android.webkit.WebView#getCertificate()
     */
    virtual CARAPI_(AutoPtr<ISslCertificate>) GetCertificate();

    /**
     * @see android.webkit.WebView#clearSslPreferences()
     */
    virtual CARAPI_(void) ClearSslPreferences();

    // TODO(sgurun) remove after this rolls in. To keep internal tree happy.
    virtual CARAPI_(void) ClearClientCertPreferences();

    /**
     * Method to return all hit test values relevant to public WebView API.
     * Note that this expose more data than needed for WebView.getHitTestResult.
     * Unsafely returning reference to mutable internal object to avoid excessive
     * garbage allocation on repeated calls.
     */
    virtual CARAPI_(AutoPtr<HitTestData>) GetLastHitTestResult();

    /**
     * @see android.webkit.WebView#requestFocusNodeHref()
     */
    virtual CARAPI_(void) RequestFocusNodeHref(
        /* [in] */ IMessage* msg);

    /**
     * @see android.webkit.WebView#requestImageRef()
     */
    virtual CARAPI_(void) RequestImageRef(
        /* [in] */ IMessage* msg);

    //@VisibleForTesting
    virtual CARAPI_(Float) GetPageScaleFactor();

    /**
     * @see android.webkit.WebView#getScale()
     *
     * Please note that the scale returned is the page scale multiplied by
     * the screen density factor. See CTS WebViewTest.testSetInitialScale.
     */
    virtual CARAPI_(Float) GetScale();

    /**
     * @see android.webkit.WebView#flingScroll(int, int)
     */
    virtual CARAPI_(void) FlingScroll(
        /* [in] */ Int32 velocityX,
        /* [in] */ Int32 velocityY);

    /**
     * @see android.webkit.WebView#pageUp(boolean)
     */
    virtual CARAPI_(Boolean) PageUp(
        /* [in] */ Boolean top);

    /**
     * @see android.webkit.WebView#pageDown(boolean)
     */
    virtual CARAPI_(Boolean) PageDown(
        /* [in] */ Boolean bottom);

    /**
     * @see android.webkit.WebView#canZoomIn()
     */
    // This method uses the term 'zoom' for legacy reasons, but relates
    // to what chrome calls the 'page scale factor'.
    virtual CARAPI_(Boolean) CanZoomIn();

    /**
     * @see android.webkit.WebView#canZoomOut()
     */
    // This method uses the term 'zoom' for legacy reasons, but relates
    // to what chrome calls the 'page scale factor'.
    virtual CARAPI_(Boolean) CanZoomOut();

    /**
     * @see android.webkit.WebView#zoomIn()
     */
    // This method uses the term 'zoom' for legacy reasons, but relates
    // to what chrome calls the 'page scale factor'.
    virtual CARAPI_(Boolean) ZoomIn();

    /**
     * @see android.webkit.WebView#zoomOut()
     */
    // This method uses the term 'zoom' for legacy reasons, but relates
    // to what chrome calls the 'page scale factor'.
    virtual CARAPI_(Boolean) ZoomOut();

    /**
     * @see android.webkit.WebView#zoomBy()
     */
    // This method uses the term 'zoom' for legacy reasons, but relates
    // to what chrome calls the 'page scale factor'.
    virtual CARAPI_(Boolean) ZoomBy(
        /* [in] */ Float delta);

    /**
     * @see android.webkit.WebView#invokeZoomPicker()
     */
    virtual CARAPI_(void) InvokeZoomPicker();

    /**
     * @see android.webkit.WebView#preauthorizePermission(Uri, long)
     */
    virtual CARAPI_(void) PreauthorizePermission(
        /* [in] */ IUri* origin,
        /* [in] */ Int64 resources);

    /**
     * @see ContentViewCore.evaluateJavaScript(String, ContentViewCore.JavaScriptCallback)
     */
    virtual CARAPI_(void) EvaluateJavaScript(
        /* [in] */ String script,
        /* [in] */ const IValueCallback* callback);

    /**
     * @see ContentViewCore.evaluateJavaScriptEvenIfNotYetNavigated(String)
     */
    virtual CARAPI_(void) EvaluateJavaScriptEvenIfNotYetNavigated(
        /* [in] */ String script);

    //--------------------------------------------------------------------------------------------
    //  View and ViewGroup method implementations
    //--------------------------------------------------------------------------------------------

    /**
     * @see android.webkit.View#onTouchEvent()
     */
    virtual CARAPI_(Boolean) OnTouchEvent(
        /* [in] */ IMotionEvent* event);

    /**
     * @see android.view.View#onHoverEvent()
     */
    virtual CARAPI_(Boolean) OnHoverEvent(
        /* [in] */ IMotionEvent* event);

    /**
     * @see android.view.View#onGenericMotionEvent()
     */
    virtual CARAPI_(Boolean) OnGenericMotionEvent(
        /* [in] */ IMotionEvent* event);

    /**
     * @see android.view.View#onConfigurationChanged()
     */
    virtual CARAPI_(void) OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

    /**
     * @see android.view.View#onAttachedToWindow()
     */
    virtual CARAPI_(void) OnAttachedToWindow();

    /**
     * @see android.view.View#onDetachedFromWindow()
     */
    //@SuppressLint("MissingSuperCall")
    virtual CARAPI_(void) OnDetachedFromWindow();

    /**
     * @see android.view.View#onWindowFocusChanged()
     */
    virtual CARAPI_(void) OnWindowFocusChanged(
        /* [in] */ Boolean hasWindowFocus);

    /**
     * @see android.view.View#onFocusChanged()
     */
    virtual CARAPI_(void) OnFocusChanged(
        /* [in] */ Boolean focused,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    /**
     * @see android.view.View#onStartTemporaryDetach()
     */
    virtual CARAPI_(void) OnStartTemporaryDetach();

    /**
     * @see android.view.View#onFinishTemporaryDetach()
     */
    virtual CARAPI_(void) OnFinishTemporaryDetach();

    /**
     * @see android.view.View#onSizeChanged()
     */
    virtual CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 ow,
        /* [in] */ Int32 oh);

    /**
     * @see android.view.View#onVisibilityChanged()
     */
    virtual CARAPI_(void) OnVisibilityChanged(
        /* [in] */ IView* changedView,
        /* [in] */ Int32 visibility);

    /**
     * @see android.view.View#onWindowVisibilityChanged()
     */
    virtual CARAPI_(void) OnWindowVisibilityChanged(
        /* [in] */ Int32 visibility);

    /**
     * Save the state of this AwContents into provided Bundle.
     * @return False if saving state failed.
     */
    virtual CARAPI_(Boolean) SaveState(
        /* [in] */ IBundle* outState);

    /**
     * Restore the state of this AwContents into provided Bundle.
     * @param inState Must be a bundle returned by saveState.
     * @return False if restoring state failed.
     */
    virtual CARAPI_(Boolean) RestoreState(
        /* [in] */ IBundle* inState);

    /**
     * @see ContentViewCore#addPossiblyUnsafeJavascriptInterface(Object, String, Class)
     */
    virtual CARAPI_(void) AddPossiblyUnsafeJavascriptInterface(
        /* [in] */ Object* object,
        /* [in] */ String name,
        /* [in] */ IInterface* requiredAnnotation);

    /**
     * @see android.webkit.WebView#removeJavascriptInterface(String)
     */
    virtual CARAPI_(void) RemoveJavascriptInterface(
        /* [in] */ String interfaceName);

    /**
     * If native accessibility (not script injection) is enabled, and if this is
     * running on JellyBean or later, returns an AccessibilityNodeProvider that
     * implements native accessibility for this view. Returns null otherwise.
     * @return The AccessibilityNodeProvider, if available, or null otherwise.
     */
    virtual CARAPI_(AutoPtr<IAccessibilityNodeProvider>) GetAccessibilityNodeProvider();

    /**
     * @see android.webkit.WebView#onInitializeAccessibilityNodeInfo(AccessibilityNodeInfo)
     */
    virtual CARAPI_(void) OnInitializeAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info);

    /**
     * @see android.webkit.WebView#onInitializeAccessibilityEvent(AccessibilityEvent)
     */
    virtual CARAPI_(void) OnInitializeAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    virtual CARAPI_(Boolean) SupportsAccessibilityAction(
        /* [in] */ Int32 action);

    /**
     * @see android.webkit.WebView#performAccessibilityAction(int, Bundle)
     */
    virtual CARAPI_(Boolean) PerformAccessibilityAction(
        /* [in] */ Int32 action,
        /* [in] */ IBundle* arguments);

    /**
     * @see android.webkit.WebView#clearFormData()
     */
    virtual CARAPI_(void) HideAutofillPopup();

    virtual CARAPI_(void) SetNetworkAvailable(
        /* [in] */ Boolean networkUp);

    //@CalledByNative
    virtual CARAPI_(void) OnFindResultReceived(
        /* [in] */ Int32 activeMatchOrdinal,
        /* [in] */ Int32 numberOfMatches,
        /* [in] */ Boolean isDoneCounting);

    //@CalledByNative
    virtual CARAPI_(void) OnNewPicture();

    //@Override
    virtual CARAPI_(void) ExtractSmartClipData(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    //@Override
    virtual CARAPI_(void) SetSmartClipResultHandler(
        /* [in] */ const IHandler* resultHandler);

private:
    static CARAPI_(AutoPtr<ContentViewCore>) CreateAndInitializeContentViewCore(
        /* [in] */ IViewGroup* containerView,
        /* [in] */ IContext* context,
        /* [in] */ InternalAccessDelegate* internalDispatcher,
        /* [in] */ Int64 nativeWebContents,
        /* [in] */ GestureStateListener* gestureStateListener,
        /* [in] */ ContentViewClient* contentViewClient,
        /* [in] */ ContentViewCore::ZoomControlsDelegate* zoomControlsDelegate);

    CARAPI_(void) SetInternalAccessAdapter(
        /* [in] */ InternalAccessDelegate* internalAccessAdapter);

    CARAPI_(void) SetContainerView(
        /* [in] */ IViewGroup* newContainerView);

    /**
     * Reconciles the state of this AwContents object with the state of the new container view.
     */
    CARAPI_(void) OnContainerViewChanged();

    /* Common initialization routine for adopting a native AwContents instance into this
     * java instance.
     *
     * TAKE CARE! This method can get called multiple times per java instance. Code accordingly.
     * ^^^^^^^^^  See the native class declaration for more details on relative object lifetimes.
     */
    CARAPI_(void) SetNewAwContents(
        /* [in] */ Int64 newAwContentsPtr);


    // Recap: supplyContentsForPopup() is called on the parent window's content, this method is
    // called on the popup window's content.
    CARAPI_(void) ReceivePopupContents(
        /* [in] */ Int64 popupNativeAwContents);

    CARAPI_(AutoPtr<IRect>) GetGlobalVisibleRect();

    CARAPI_(void) RequestVisitedHistoryFromClient();

    CARAPI_(void) SetViewVisibilityInternal(
        /* [in] */ Boolean visible);

    CARAPI_(void) SetWindowVisibilityInternal(
        /* [in] */ Boolean visible);

    //--------------------------------------------------------------------------------------------
    //  Methods called from native via JNI
    //--------------------------------------------------------------------------------------------

    //@CalledByNative
    static CARAPI_(void) OnDocumentHasImagesResponse(
        /* [in] */ Boolean result,
        /* [in] */ IMessage* message);

    //@CalledByNative
    CARAPI_(void) OnReceivedTouchIconUrl(
        /* [in] */ String url,
        /* [in] */ Boolean precomposed);

    //@CalledByNative
    CARAPI_(void) OnReceivedIcon(
        /* [in] */ IBitmap* bitmap);

    /** Callback for generateMHTML. */
    //@CalledByNative
    static CARAPI_(void) GenerateMHTMLCallback(
        /* [in] */ String path,
        /* [in] */ Int64 size,
        /* [in] */ IValueCallback* callback);

    //@CalledByNative
    CARAPI_(void) OnReceivedHttpAuthRequest(
        /* [in] */ AwHttpAuthHandler* handler,
        /* [in] */ String host,
        /* [in] */ String realm);

    //@CalledByNative
    CARAPI_(void) OnGeolocationPermissionsShowPrompt(
        /* [in] */ String origin);

    //@CalledByNative
    CARAPI_(void) OnGeolocationPermissionsHidePrompt();

    //@CalledByNative
    CARAPI_(void) OnPermissionRequest(
        /* [in] */ AwPermissionRequest* awPermissionRequest);

    //@CalledByNative
    CARAPI_(void) OnPermissionRequestCanceled(
        /* [in] */ AwPermissionRequest* awPermissionRequest);

    // Called as a result of nativeUpdateLastHitTestData.
    //@CalledByNative
    CARAPI_(void) UpdateHitTestData(
        /* [in] */ Int32 type,
        /* [in] */ String extra,
        /* [in] */ String href,
        /* [in] */ String anchorText,
        /* [in] */ String imgSrc);

    //@CalledByNative
    CARAPI_(Boolean) RequestDrawGL(
        /* [in] */ ICanvas* canvas,
        /* [in] */ Boolean waitForCompletion);

    //@CalledByNative
    CARAPI_(void) PostInvalidateOnAnimation();

    // Call postInvalidateOnAnimation for invalidations. This is only used to synchronize
    // draw functor destruction.
    //@CalledByNative
    CARAPI_(void) InvalidateOnFunctorDestroy();

    //@CalledByNative
    CARAPI_(AutoPtr< ArrayOf<Int32> >) GetLocationOnScreen();

    //@CalledByNative
    CARAPI_(void) OnWebLayoutPageScaleFactorChanged(
        /* [in] */ Float webLayoutPageScaleFactor);

    //@CalledByNative
    CARAPI_(void) OnWebLayoutContentsSizeChanged(
        /* [in] */ Int32 widthCss,
        /* [in] */ Int32 heightCss);

    //@CalledByNative
    CARAPI_(void) ScrollContainerViewTo(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    //@CalledByNative
    CARAPI_(Boolean) IsFlingActive();

    //@CalledByNative
    CARAPI_(void) UpdateScrollState(
        /* [in] */ Int32 maxContainerViewScrollOffsetX,
        /* [in] */ Int32 maxContainerViewScrollOffsetY,
        /* [in] */ Int32 contentWidthDip,
        /* [in] */ Int32 contentHeightDip,
        /* [in] */ Float pageScaleFactor,
        /* [in] */ Float minPageScaleFactor,
        /* [in] */ Float maxPageScaleFactor);

    //@CalledByNative
    CARAPI_(void) SetAwAutofillClient(
        /* [in] */ AwAutofillClient* client);

    //@CalledByNative
    CARAPI_(void) DidOverscroll(
        /* [in] */ Int32 deltaX,
        /* [in] */ Int32 deltaY);

    // -------------------------------------------------------------------------------------------
    // Helper methods
    // -------------------------------------------------------------------------------------------

    CARAPI_(void) SetPageScaleFactorAndLimits(
        /* [in] */ Float pageScaleFactor,
        /* [in] */ Float minPageScaleFactor,
        /* [in] */ Float maxPageScaleFactor);

    CARAPI_(void) SaveWebArchiveInternal(
        /* [in] */ String path,
        /* [in] */ const IValueCallback* callback);

    /**
     * Try to generate a pathname for saving an MHTML archive. This roughly follows WebView's
     * autoname logic.
     */
    static CARAPI_(String) GenerateArchiveAutoNamePath(
        /* [in] */ String originalUrl,
        /* [in] */ String baseName);


    // Return true if the GeolocationPermissionAPI should be used.
    //@CalledByNative
    CARAPI_(Boolean) UseLegacyGeolocationPermissionAPI();

private:
    //--------------------------------------------------------------------------------------------
    //  Native methods
    //--------------------------------------------------------------------------------------------

    static CARAPI_(Int64) NativeInit(
        /* [in] */ AwBrowserContext* browserContext);

    static CARAPI_(void) NativeDestroy(
        /* [in] */ Int64 nativeAwContents);

    static CARAPI_(void) NativeSetAwDrawSWFunctionTable(
        /* [in] */ Int64 functionTablePointer);

    static CARAPI_(void) NativeSetAwDrawGLFunctionTable(
        /* [in] */ Int64 functionTablePointer);

    static CARAPI_(Int64) NativeGetAwDrawGLFunction();

    static CARAPI_(Int32) NativeGetNativeInstanceCount();

    static CARAPI_(void) NativeSetShouldDownloadFavicons();

    CARAPI_(void) NativeSetJavaPeers(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ AwContents* awContents,
        /* [in] */ AwWebContentsDelegate* webViewWebContentsDelegate,
        /* [in] */ AwContentsClientBridge* contentsClientBridge,
        /* [in] */ AwContentsIoThreadClient* ioThreadClient,
        /* [in] */ InterceptNavigationDelegate* navigationInterceptionDelegate);

    CARAPI_(Int64) NativeGetWebContents(
        /* [in] */ Int64 nativeAwContents);

    CARAPI_(void) NativeDocumentHasImages(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ IMessage* message);

    CARAPI_(void) NativeGenerateMHTML(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ String path,
        /* [in] */ IValueCallback* callback);

    CARAPI_(void) NativeAddVisitedLinks(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ ArrayOf<String>* visitedLinks);

    CARAPI_(Boolean) NativeOnDraw(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ ICanvas* canvas,
        /* [in] */ Boolean isHardwareAccelerated,
        /* [in] */ Int32 scrollX,
        /* [in] */ Int32 scrollY,
        /* [in] */ Int32 visibleLeft,
        /* [in] */ Int32 visibleTop,
        /* [in] */ Int32 visibleRight,
        /* [in] */ Int32 visibleBottom);

    CARAPI_(void) NativeFindAllAsync(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ String searchString);

    CARAPI_(void) NativeFindNext(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ Boolean forward);

    CARAPI_(void) NativeClearMatches(
        /* [in] */ Int64 nativeAwContents);

    CARAPI_(void) NativeClearCache(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ Boolean includeDiskFiles);

    CARAPI_(AutoPtr< ArrayOf<Byte> >) NativeGetCertificate(
        /* [in] */ Int64 nativeAwContents);

    // Coordinates in desity independent pixels.
    CARAPI_(void) NativeRequestNewHitTestDataAt(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI_(void) NativeUpdateLastHitTestData(
        /* [in] */ Int64 nativeAwContents);

    CARAPI_(void) NativeOnSizeChanged(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 ow,
        /* [in] */ Int32 oh);

    CARAPI_(void) NativeScrollTo(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI_(void) NativeSetViewVisibility(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ Boolean visible);

    CARAPI_(void) NativeSetWindowVisibility(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ Boolean visible);

    CARAPI_(void) NativeSetIsPaused(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ Boolean paused);

    CARAPI_(void) NativeOnAttachedToWindow(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    static CARAPI_(void) NativeOnDetachedFromWindow(
        /* [in] */ Int64 nativeAwContents);

    CARAPI_(void) NativeSetDipScale(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ Float dipScale);

    // Returns null if save state fails.
    CARAPI_(AutoPtr< ArrayOf<Byte> >) NativeGetOpaqueState(
        /* [in] */ Int64 nativeAwContents);

    // Returns false if restore state fails.
    CARAPI_(Boolean) NativeRestoreFromOpaqueState(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ ArrayOf<Byte>* state);

    CARAPI_(Int64) NativeReleasePopupAwContents(
        /* [in] */ Int64 nativeAwContents);

    CARAPI_(void) NativeFocusFirstNode(
        /* [in] */ Int64 nativeAwContents);

    CARAPI_(void) NativeSetBackgroundColor(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ Int32 color);

    CARAPI_(Int64) NativeGetAwDrawGLViewContext(
        /* [in] */ Int64 nativeAwContents);

    CARAPI_(Int64) NativeCapturePicture(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI_(void) NativeEnableOnNewPicture(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ Boolean enabled);

    CARAPI_(void) NativeClearView(
        /* [in] */ Int64 nativeAwContents);

    CARAPI_(void) NativeSetExtraHeadersForUrl(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ String url,
        /* [in] */ String extraHeaders);

    CARAPI_(void) NativeInvokeGeolocationCallback(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ Boolean value,
        /* [in] */ String requestingFrame);

    CARAPI_(void) NativeSetJsOnlineProperty(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ Boolean networkUp);

    CARAPI_(void) NativeTrimMemory(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ Int32 level,
        /* [in] */ Boolean visible);

    CARAPI_(void) NativeCreatePdfExporter(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ AwPdfExporter* awPdfExporter);

    CARAPI_(void) NativePreauthorizePermission(
        /* [in] */ Int64 nativeAwContents,
        /* [in] */ String origin,
        /* [in] */ Int64 resources);

    CARAPI_(void) Init(
        /* [in] */ AwBrowserContext* browserContext,
        /* [in] */ IViewGroup* containerView,
        /* [in] */ IContext* context,
        /* [in] */ InternalAccessDelegate* internalAccessAdapter,
        /* [in] */ NativeGLDelegate* nativeGLDelegate,
        /* [in] */ AwContentsClient* contentsClient,
        /* [in] */ AwSettings* settings,
        /* [in] */ DependencyFactory* dependencyFactory);

private:
    // This is only to avoid heap allocations inside getGlobalVisibleRect. It should treated
    // as a local variable in the function and not used anywhere else.
    static const AutoPtr<IRect> sLocalGlobalVisibleRect;

    // TODO(mkosiba): In WebViewClassic these appear in some of the scroll extent calculation
    // methods but toggling them has no visiual effect on the content (in other words the scrolling
    // code behaves as if the scrollbar-related padding is in place but the onDraw code doesn't
    // take that into consideration).
    // http://crbug.com/269032
    Boolean mOverlayHorizontalScrollbar;
    Boolean mOverlayVerticalScrollbar;

    static const Boolean SUPPORTS_ON_ANIMATION;

    static const String TAG;

    static const String WEB_ARCHIVE_EXTENSION;

    // Used to avoid enabling zooming in / out if resulting zooming will
    // produce little visible difference.
    static const Float ZOOM_CONTROLS_EPSILON;

    Int64 mNativeAwContents;
    const AutoPtr<AwBrowserContext> mBrowserContext;
    AutoPtr<IViewGroup> mContainerView;
    const AutoPtr<AwLayoutChangeListener> mLayoutChangeListener;
    const AutoPtr<IContext> mContext;
    AutoPtr<ContentViewCore> mContentViewCore;
    const AutoPtr<AwContentsClient> mContentsClient;
    const AutoPtr<AwContentViewClient> mContentViewClient;
    const AutoPtr<AwContentsClientBridge> mContentsClientBridge;
    const AutoPtr<AwWebContentsDelegateAdapter> mWebContentsDelegate;
    const AutoPtr<AwContentsIoThreadClient> mIoThreadClient;
    const InterceptNavigationDelegateImpl> mInterceptNavigationDelegate;
    AutoPtr<InternalAccessDelegate> mInternalAccessAdapter;
    const AutoPtr<NativeGLDelegate> mNativeGLDelegate;
    const AutoPtr<AwLayoutSizer> mLayoutSizer;
    const AutoPtr<AwZoomControls> mZoomControls;
    const AutoPtr<AwScrollOffsetManager> mScrollOffsetManager;
    AutoPtr<OverScrollGlow> mOverScrollGlow;
    // This can be accessed on any thread after construction. See AwContentsIoThreadClient.
    const AutoPtr<AwSettings> mSettings;
    const AutoPtr<ScrollAccessibilityHelper> mScrollAccessibilityHelper;

    Boolean mIsPaused;
    Boolean mIsViewVisible;
    Boolean mIsWindowVisible;
    Boolean mIsAttachedToWindow;
    AutoPtr<IBitmap> mFavicon;
    Boolean mHasRequestedVisitedHistoryFromClient;
    // TODO(boliu): This should be in a global context, not per webview.
    const Double mDIPScale;

    // The base background color, i.e. not accounting for any CSS body from the current page.
    Int32 mBaseBackgroundColor;

    // Must call nativeUpdateLastHitTestData first to update this before use.
    const AutoPtr<HitTestData> mPossiblyStaleHitTestData;

    const AutoPtr<DefaultVideoPosterRequestHandler> mDefaultVideoPosterRequestHandler;

    // Bound method for suppling Picture instances to the AwContentsClient. Will be null if the
    // picture listener API has not yet been enabled, or if it is using invalidation-only mode.
    AutoPtr<ICallable> mPictureListenerContentProvider;

    Boolean mContainerViewFocused;
    Boolean mWindowFocused;

    // These come from the compositor and are updated synchronously (in contrast to the values in
    // ContentViewCore, which are updated at end of every frame).
    Float mPageScaleFactor;
    Float mMinPageScaleFactor;
    Float mMaxPageScaleFactor;
    Float mContentWidthDip;
    Float mContentHeightDip;

    AutoPtr<AwAutofillClient> mAwAutofillClient;

    AutoPtr<AwPdfExporter> mAwPdfExporter;

    AutoPtr<AwViewMethods> mAwViewMethods;

    const FullScreenTransitionsState mFullScreenTransitionsState;

    // This flag indicates that ShouldOverrideUrlNavigation should be posted
    // through the resourcethrottle. This is only used for popup windows.
    Boolean mDeferredShouldOverrideUrlLoadingIsPendingForPopup;

    // The framework may temporarily detach our container view, for example during layout if
    // we are a child of a ListView. This may cause many toggles of View focus, which we suppress
    // when in this state.
    Boolean mTemporarilyDetached;

    // Reference to the active mNativeAwContents pointer while it is active use
    // (ie before it is destroyed).
    AutoPtr<CleanupReference> mCleanupReference;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWCONTENTS_H__
