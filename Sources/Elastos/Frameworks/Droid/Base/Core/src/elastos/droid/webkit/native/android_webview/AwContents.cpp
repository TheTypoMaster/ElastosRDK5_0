
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//===============================================================
//               AwContents::DependencyFactory
//===============================================================

AutoPtr<AwLayoutSizer> AwContents::DependencyFactory::CreateLayoutSizer()
{
    AutoPtr<AwLayoutSizer> als = new AwLayoutSizer();
    return als;
}

AutoPtr<AwScrollOffsetManager> AwContents::DependencyFactory::CreateScrollOffsetManager(
    /* [in] */ AwScrollOffsetManager::Delegate* delegate,
    /* [in] */ IOverScroller* overScroller)
{
    AutoPtr<AwScrollOffsetManager> asom = new AwScrollOffsetManager(delegate, overScroller);
    return asom;
}

//===============================================================
//                AwContents::DestroyRunnable
//===============================================================

AwContents::DestroyRunnable::DestroyRunnable(
    /* [in] */ Int64 nativeAwContents)
    : mNativeAwContents(nativeAwContents)
{
}

//@Override
ECode AwContents::DestroyRunnable::Run()
{
    NativeDestroy(mNativeAwContents);
    return NOERROR;
}

//===============================================================
//           AwContents::FullScreenTransitionsState
//===============================================================

AwContents::FullScreenTransitionsState::FullScreenTransitionsState(
    /* [in] */ IViewGroup* initialContainerView,
    /* [in] */ InternalAccessDelegate* initialInternalAccessAdapter,
    /* [in] */ AwViewMethods* initialAwViewMethods)
    : mInitialContainerView(initialContainerView)
    , mInitialInternalAccessAdapter(initialInternalAccessAdapter)
    , mInitialAwViewMethods(initialAwViewMethods)
{
}

void AwContents::FullScreenTransitionsState::EnterFullScreen(
    /* [in] */ FullScreenView fullScreenView)
{
    mFullScreenView = fullScreenView;
}

void AwContents::FullScreenTransitionsState::ExitFullScreen()
{
    mFullScreenView = NULL;
}

Boolean AwContents::FullScreenTransitionsState::IsFullScreen()
{
    return mFullScreenView != NULL;
}

AutoPtr<IViewGroup> AwContents::FullScreenTransitionsState::GetInitialContainerView()
{
    return mInitialContainerView;
}

AutoPtr<InternalAccessDelegate> AwContents::FullScreenTransitionsState::GetInitialInternalAccessDelegate()
{
    return mInitialInternalAccessAdapter;
}

AutoPtr<AwViewMethods> AwContents::FullScreenTransitionsState::GetInitialAwViewMethods()
{
    return mInitialAwViewMethods;
}

AutoPtr<FullScreenView> AwContents::FullScreenTransitionsState::GetFullScreenView()
{
    return mFullScreenView;
}

//===============================================================
//             AwContents::IoThreadClientImpl
//===============================================================

AwContents::IoThreadClientImpl::IoThreadClientImpl(
    /* [in] */ AwContents* owner)
    : mOwner(owner)
{
}

//@Override
Int32 AwContents::IoThreadClientImpl::GetCacheMode()
{
    return mSettings->GetCacheMode();
}

//@Override
AutoPtr<AwWebResourceResponse> AwContents::IoThreadClientImpl::ShouldInterceptRequest(
    /* [in] */ AwContentsClient::ShouldInterceptRequestParams* params)
{
    String url = params->url;
    AutoPtr<AwWebResourceResponse> awWebResourceResponse;
    // Return the response directly if the url is default video poster url.
    awWebResourceResponse = mDefaultVideoPosterRequestHandler->ShouldInterceptRequest(url);
    if (awWebResourceResponse != NULL) return awWebResourceResponse;

    awWebResourceResponse = mContentsClient->ShouldInterceptRequest(params);

    if (awWebResourceResponse == NULL) {
        mContentsClient->GetCallbackHelper()->PostOnLoadResource(url);
    }

    if (params.isMainFrame && awWebResourceResponse != NULL &&
            awWebResourceResponse->GetData() == NULL) {
        // In this case the intercepted URLRequest job will simulate an empty response
        // which doesn't trigger the onReceivedError callback. For WebViewClassic
        // compatibility we synthesize that callback. http://crbug.com/180950
        mContentsClient->GetCallbackHelper()->PostOnReceivedError(
                ErrorCodeConversionHelper::ERROR_UNKNOWN,
                NULL /* filled in by the glue layer */, url);
    }
    return awWebResourceResponse;
}

//@Override
Boolean AwContents::IoThreadClientImpl::ShouldBlockContentUrls()
{
    return (!mSettings->GetAllowContentAccess());
}

//@Override
Boolean AwContents::IoThreadClientImpl::ShouldBlockFileUrls()
{
    return (!mSettings->GetAllowFileAccess());
}

//@Override
Boolean AwContents::IoThreadClientImpl::ShouldBlockNetworkLoads()
{
    return mSettings->GetBlockNetworkLoads();
}

//@Override
Boolean AwContents::IoThreadClientImpl::ShouldAcceptThirdPartyCookies()
{
    return mSettings->GetAcceptThirdPartyCookies();
}

//@Override
void AwContents::IoThreadClientImpl::OnDownloadStart(
    /* [in] */ String url,
    /* [in] */ String userAgent,
    /* [in] */ String contentDisposition,
    /* [in] */ String mimeType,
    /* [in] */ Int64 contentLength)
{
    mContentsClient->GetCallbackHelper()->PostOnDownloadStart(url, userAgent,
            contentDisposition, mimeType, contentLength);
}

//@Override
void AwContents::IoThreadClientImpl::NewLoginRequest(
    /* [in] */ String realm,
    /* [in] */ String account,
    /* [in] */ String args)
{
    mContentsClient->GetCallbackHelper()->PostOnReceivedLoginRequest(realm, account, args);
}

//===============================================================
//       AwContents::InterceptNavigationDelegateImpl
//===============================================================

AwContents::InterceptNavigationDelegateImpl::InterceptNavigationDelegateImpl(
    /* [in] */ AwContents* owner)
    : mOwner(owner)
{
}

//@Override
Boolean AwContents::InterceptNavigationDelegateImpl::ShouldIgnoreNavigation(
    /* [in] */ NavigationParams* navigationParams)
{
    const String url = navigationParams->url;
    Boolean ignoreNavigation = FALSE;
    if (mDeferredShouldOverrideUrlLoadingIsPendingForPopup) {
        mDeferredShouldOverrideUrlLoadingIsPendingForPopup = FALSE;
        // If this is used for all navigations in future, cases for application initiated
        // load, redirect and backforward should also be filtered out.
        if (!navigationParams->isPost) {
            ignoreNavigation = mContentsClient->ShouldOverrideUrlLoading(url);
        }
    }
    // The shouldOverrideUrlLoading call might have resulted in posting messages to the
    // UI thread. Using sendMessage here (instead of calling onPageStarted directly)
    // will allow those to run in order.
    if (!ignoreNavigation) {
        mContentsClient->GetCallbackHelper()->PostOnPageStarted(url);
    }
    return ignoreNavigation;
}

//===============================================================
//             AwContents::AwLayoutSizerDelegate
//===============================================================

AwContents::AwLayoutSizerDelegate::AwLayoutSizerDelegate(
    /* [in] */ AwContents* owner)
    : mOwner(owner)
{
}

//@Override
void AwContents::AwLayoutSizerDelegate::RequestLayout()
{
    mContainerView->RequestLayout();
}

//@Override
void AwContents::AwLayoutSizerDelegate::SetMeasuredDimension(
    /* [in] */ Int32 measuredWidth,
    /* [in] */ Int32 measuredHeight)
{
    mInternalAccessAdapter->SetMeasuredDimension(measuredWidth, measuredHeight);
}

//@Override
Boolean AwContents::AwLayoutSizerDelegate::IsLayoutParamsHeightWrapContent()
{
    return mContainerView->GetLayoutParams() != NULL &&
            mContainerView->GetLayoutParams()->height == IViewGroupLayoutParams::WRAP_CONTENT;
}

//@Override
void AwContents::AwLayoutSizerDelegate::SetForceZeroLayoutHeight(
    /* [in] */ Boolean forceZeroHeight)
{
    GetSettings()->SetForceZeroLayoutHeight(forceZeroHeight);
}

//===============================================================
//          AwContents::AwScrollOffsetManagerDelegate
//===============================================================

AwContents::AwScrollOffsetManagerDelegate::AwScrollOffsetManagerDelegate(
    /* [in] */ AwContents* owner)
    : mOwner(owner)
{
}

//@Override
void AwContents::AwScrollOffsetManagerDelegate::OverScrollContainerViewBy(
    /* [in] */ Int32 deltaX,
    /* [in] */ Int32 deltaY,
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY,
    /* [in] */ Int32 scrollRangeX,
    /* [in] */ Int32 scrollRangeY,
    /* [in] */ Boolean isTouchEvent)
{
    mInternalAccessAdapter->OverScrollBy(deltaX, deltaY, scrollX, scrollY,
            scrollRangeX, scrollRangeY, 0, 0, isTouchEvent);
}

//@Override
void AwContents::AwScrollOffsetManagerDelegate::ScrollContainerViewTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    mInternalAccessAdapter->Super_scrollTo(x, y);
}

//@Override
void AwContents::AwScrollOffsetManagerDelegate::ScrollNativeTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    if (mNativeAwContents == 0) return;
    NativeScrollTo(mNativeAwContents, x, y);
}

//@Override
Int32 AwContents::AwScrollOffsetManagerDelegate::GetContainerViewScrollX()
{
    return mContainerView->GetScrollX();
}

//@Override
Int32 AwContents::AwScrollOffsetManagerDelegate::GetContainerViewScrollY()
{
    return mContainerView->GetScrollY();
}

//@Override
void AwContents::AwScrollOffsetManagerDelegate::Invalidate()
{
    mContainerView->Invalidate();
}

//===============================================================
//             AwContents::AwGestureStateListener
//===============================================================

AwContents::AwGestureStateListener::AwGestureStateListener(
    /* [in] */ AwContents* owner)
    : mOwner(owner)
{
}

//@Override
void AwContents::AwGestureStateListener::OnPinchStarted()
{
    // While it's possible to re-layout the view during a pinch gesture, the effect is very
    // janky (especially that the page scale update notification comes from the renderer
    // main thread, not from the impl thread, so it's usually out of sync with what's on
    // screen). It's also quite expensive to do a re-layout, so we simply postpone
    // re-layout for the duration of the gesture. This is compatible with what
    // WebViewClassic does.
    mLayoutSizer->FreezeLayoutRequests();
}

//@Override
void AwContents::AwGestureStateListener::OnPinchEnded()
{
    mLayoutSizer->UnfreezeLayoutRequests();
}

//@Override
void AwContents::AwGestureStateListener::OnFlingCancelGesture()
{
    mScrollOffsetManager->OnFlingCancelGesture();
}

//@Override
void AwContents::AwGestureStateListener::OnUnhandledFlingStartEvent(
    /* [in] */ Int32 velocityX,
    /* [in] */ Int32 velocityY)
{
    mScrollOffsetManager->OnUnhandledFlingStartEvent(velocityX, velocityY);
}

//@Override
void AwContents::AwGestureStateListener::OnScrollUpdateGestureConsumed()
{
    mScrollAccessibilityHelper->PostViewScrolledAccessibilityEventCallback();
}

//===============================================================
//              AwContents::AwComponentCallbacks
//===============================================================

AwContents::AwComponentCallbacks::AwComponentCallbacks(
    /* [in] */ AwContents* owner)
    : mOwner(owner)
{
}

//@Override
void AwContents::AwComponentCallbacks::OnTrimMemory(
    /* [in] */ Int32 level)
{
    if (mNativeAwContents == 0) return;
    Boolean visibleRectEmpty = GetGlobalVisibleRect()->IsEmpty();
    const Boolean visible = mIsViewVisible && mIsWindowVisible && !visibleRectEmpty;
    NativeTrimMemory(mNativeAwContents, level, visible);
}

//@Override
void AwContents::AwComponentCallbacks::OnLowMemory()
{
}

//@Override
void AwContents::AwComponentCallbacks::OnConfigurationChanged(
    /* [in] */ IConfiguration* configuration)
{
}

//===============================================================
//             AwContents::AwLayoutChangeListener
//===============================================================

AwContents::AwLayoutChangeListener::AwLayoutChangeListener(
    /* [in] */ AwContents* owner)
    : mOwner(owner)
{
}

//@Override
ECode AwContents::AwLayoutChangeListener::OnLayoutChange(
    /* [in] */ IView* v,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [in] */ Int32 oldLeft,
    /* [in] */ Int32 oldTop,
    /* [in] */ Int32 oldRight,
    /* [in] */ Int32 oldBottom)
{
    assert(v == mContainerView);
    mOwner->mLayoutSizer->OnLayoutChange();
}

//===============================================================
//         AwContents::InnerZoomSupportChangeListener
//===============================================================

AwContents::InnerZoomSupportChangeListener::InnerZoomSupportChangeListener(
    /* [in] */ AwContents* owner);

//@Override
void AwContents::InnerZoomSupportChangeListener::OnGestureZoomSupportChanged(
    /* [in] */ Boolean supportsDoubleTapZoom,
    /* [in] */ Boolean supportsMultiTouchZoom)
{
    mOwner->mContentViewCore->UpdateDoubleTapSupport(supportsDoubleTapZoom);
    mOwner->mContentViewCore->UpdateMultiTouchZoomSupport(supportsMultiTouchZoom);
}

//===============================================================
//                   AwContents::InnerCallable
//===============================================================

AwContents::InnerCallable::InnerCallable(
    /* [in] */ AwContents* owner)
    : mOwner(owner)
{
}

ECode AwContents::InnerCallable::Call(
    /* [out] */ IPicture** pic)
{
    VALIDATE_NOT_NULL(pic);
    *pic = mOwner->CapturePicture();
    REFCOUNT_ADD(*pic);
    return NOERROR;
}


//===============================================================
//                AwContents::InnerValueCallback
//===============================================================

AwContents::InnerValueCallback::InnerValueCallback(
    /* [in] */ AwContents* owner)
    : mOwner(owner)
{
}

ECode AwContents::InnerValueCallback::OnReceiveValue(
    /* [in] */ ArrayOf<String>* value)
{
    AutoPtr<IRunnable> runnable = new InnerRunnable(this);
    ThreadUtils::RunOnUiThread(runnable);
    return NOERROR;
}

//===============================================================
//        AwContents::InnerValueCallback::InnerRunnable
//===============================================================

AwContents::InnerValueCallback::InnerRunnable::InnerRunnable(
    /* [in] */ InnerValueCallback* owner,
    /* [in] */ ArrayOf<String>* value)
    : mOwner(owner)
    , mValue(value)
{
}

ECode AwContents::InnerValueCallback::InnerRunnable::Run()
{
    if (mOwner->mOwner->mNativeAwContents == 0) return;
    mOwner->mOwner->NativeAddVisitedLinks(mOwner->mOwner->mNativeAwContents, mValue);
    return NOERROR;
}

//===============================================================
//            AwContents::InnerJavaScriptCallback
//===============================================================

AwContents::InnerJavaScriptCallback::InnerJavaScriptCallback(
    /* [in] */ AwContents* owner,
    /* [in] */ IValueCallback* callback)
    : mOwner(owner)
    , mCallback(callback)
{
}

void AwContents::InnerJavaScriptCallback::HandleJavaScriptResult(
    /* [in] */ String jsonResult)
{
    mCallback->OnReceiveValue(jsonResult);
}

//===============================================================
//             AwContents::AwGeolocationCallback
//===============================================================

//@Override
void AwContents::AwGeolocationCallback::Invoke(
    /* [in] */ const String& origin,
    /* [in] */ Boolean allow,
    /* [in] */ Boolean retain)
{
    AutoPtr<IRunnable> runnable = new InnerRunnable(this, origin, allow, retain);
    ThreadUtils::RunOnUiThread(runnable);
}

//===============================================================
//       AwContents::AwGeolocationCallback::InnerRunnable
//===============================================================

AwContents::AwGeolocationCallback::InnerRunnable::InnerRunnable(
    /* [in] */ AwGeolocationCallback* owner,
    /* [in] */ const String& origin,
    /* [in] */ Boolean allow,
    /* [in] */ Boolean retain)
    : mOwner(owner)
    , mOrigin(origin)
    , mAllow(allow)
    , mRetain(retain)
{
}

ECode AwContents::AwGeolocationCallback::InnerRunnable::Run()
{
    if (mRetain) {
        if (mAllow) {
            mBrowserContext->GetGeolocationPermissions()->Allow(mOrigin);
        }
        else {
            mBrowserContext->GetGeolocationPermissions()->Deny(mOrigin);
        }
    }
    if (mNativeAwContents == 0) return;
    mOwner->NativeInvokeGeolocationCallback(mOwner->mNativeAwContents, mAllow, mOrigin);

    return NOERROR;
}

//===============================================================
//          AwContents::SaveWebArchiveInternalRunnable
//===============================================================

AwContents::SaveWebArchiveInternalRunnable::SaveWebArchiveInternalRunnable(
    /* [in] */ AwContents* owner,
    /* [in] */ const IValueCallback* callback)
    : mOwner(owner)
    , mCallback(callback)
{
}

ECode AwContents::SaveWebArchiveInternalRunnable::Run()
{
    return mCallback->OnReceiveValue(NULL);
}

//===============================================================
//            AwContents::InnerSmartClipDataListener
//===============================================================

AwContents::InnerSmartClipDataListener::InnerSmartClipDataListener(
    /* [in] */ AwContents* owner,
    /* [in] */ const IHandler* resultHandler)
    : mOwner(owner)
    , mResultHandler(resultHandler)
{
}

void AwContents::InnerSmartClipDataListener::OnSmartClipDataExtracted(
    /* [in] */ String text,
    /* [in] */ String html,
    /* [in] */ IRect* clipRect)
{
    AutoPtr<IBundle> bundle;
    CBundle::New((IBundle**)&bundle);
    bundle->PutString(String("url"), mContentViewCore->GetWebContents()->GetVisibleUrl());
    bundle->PutString(String("title"), mContentViewCore->GetWebContents()->GetTitle());
    bundle->PutParcelable(String("rect"), clipRect);
    bundle->PutString(String("text"), text);
    bundle->PutString(String("html"), html);
    //try {
        AutoPtr<IMessageHelper> msgHelper;
        CMessageHelper::AcquireSingleton((IMessageHelper**)&msgHelper);
        AutoPtr<IMessage> msg;
        msgHelper->Obtain(resultHandler, 0, (IMessageHelper**)&msgHelper);
        msg->SetData(bundle);
        msg->SendToTarget();
    //} catch (Exception e) {
    //    Log.e(TAG, "Error calling handler for smart clip data: ", e);
    //}
}

//===============================================================
//               AwContents::AwViewMethodsImpl
//===============================================================

static AutoPtr<IRect> ClipBoundsTemporaryCreate()
{
    AutoPtr<IRect> rect;
    CRect::New((IRect**)&rect);
    return rect;
}

AwContents::AwViewMethodsImpl::AwViewMethodsImpl(
    /* [in] */ AwContents* owner)
    : mOwner(owner)
    , mLayerType(IView::LAYER_TYPE_NONE)
    , mClipBoundsTemporary(ClipBoundsTemporaryCreate())
{
}

//@Override
void AwContents::AwViewMethodsImpl::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    if (mOwner->mNativeAwContents == 0) {
        canvas->DrawColor(mOwner->GetEffectiveBackgroundColor());
        return;
    }

    // For hardware draws, the clip at onDraw time could be different
    // from the clip during DrawGL.
    Boolean bIsHardwareAccelerated = FALSE;
    Boolean bIsNonEmpty = FALSE;
    canvas->IsHardwareAccelerated(&bIsHardwareAccelerated);
    canvas->GetClipBounds(mOwner->mClipBoundsTemporary, &bIsNonEmpty);
    if (!bIsHardwareAccelerated && !bIsNonEmpty) {
        return;
    }

    mOwner->mScrollOffsetManager->SyncScrollOffsetFromOnDraw();
    AutoPtr<IRect> globalVisibleRect = mOwner->GetGlobalVisibleRect();
    Int32 left, right, top, bottom;
    globalVisibleRect->GetLeft(&left);
    globalVisibleRect->GetRight(&right);
    globalVisibleRect->GetTop(&top);
    globalVisibleRect->GetBottom(&bottom);
    Int32 scrollX, scrollY;
    mOwner->mContainerView->GetScrollX(&scrollX);
    mOwner->mContainerView->GetScrollY(&scrollY);
    if (!mOwner->NativeOnDraw(mOwner->mNativeAwContents, canvas, bIsHardwareAccelerated,
            scrollX, scrollY, left, top, right, bottom)) {
        // Can happen during initialization when compositor is not set
        // up. Or when clearView
        // is in effect. Just draw background color instead.
        canvas->DrawColor(mOwner->GetEffectiveBackgroundColor());
    }

    if (mOwner->mOverScrollGlow != NULL && mOwner->mOverScrollGlow->DrawEdgeGlows(canvas,
            mOwner->mScrollOffsetManager->ComputeMaximumHorizontalScrollOffset(),
            mOwner->mScrollOffsetManager->ComputeMaximumVerticalScrollOffset())) {
        mOwner->mContainerView->Invalidate();
    }
}

//@Override
void AwContents::AwViewMethodsImpl::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    mLayoutSizer->OnMeasure(widthMeasureSpec, heightMeasureSpec);
}

//@Override
void AwContents::AwViewMethodsImpl::RequestFocus()
{
    if (mOwner->mNativeAwContents == 0) return;
    if (!mOwner->mContainerView->isInTouchMode() && mOwner->mSettings->ShouldFocusFirstNode()) {
        mOwner->NativeFocusFirstNode(mOwner->mNativeAwContents);
    }
}

//@Override
void AwContents::AwViewMethodsImpl::SetLayerType(
    /* [in] */ Int32 layerType,
    /* [in] */ IPaint* paint)
{
    mOwner->mLayerType = layerType;
    mOwner->UpdateHardwareAcceleratedFeaturesToggle();
}

void AwContents::AwViewMethodsImpl::UpdateHardwareAcceleratedFeaturesToggle()
{
    mOwner->mSettings.setEnableSupportedHardwareAcceleratedFeatures(
            mOwner->mIsAttachedToWindow && mOwner->mContainerView->IsHardwareAccelerated() &&
                    (mOwner->mLayerType == IView::LAYER_TYPE_NONE
                    || mOwner->mLayerType == IView::LAYER_TYPE_HARDWARE));
}

//@Override
AutoPtr<IInputConnection> AwContents::AwViewMethodsImpl::OnCreateInputConnection(
    /* [in] */ IEditorInfo* outAttrs)
{
    return mContentViewCore->OnCreateInputConnection(outAttrs);
}

//@Override
Boolean AwContents::AwViewMethodsImpl::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return mOwner->mContentViewCore->OnKeyUp(keyCode, event);
}

//@Override
Boolean AwContents::AwViewMethodsImpl::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    if (mOwner->IsDpadEvent(event)) {
        mOwner->mSettings->SetSpatialNavigationEnabled(TRUE);
    }
    return mOwner->mContentViewCore->DispatchKeyEvent(event);
}

Boolean AwContents::AwViewMethodsImpl::IsDpadEvent(
    /* [in] */ IKeyEvent* event)
{
    Int32 action;
    event->GetAction(&action);
    if (action == IKeyEvent::ACTION_DOWN) {
        Int32 code;
        event->GetKeyCode(&code);
        switch (code) {
            case IKeyEvent::KEYCODE_DPAD_CENTER:
            case IKeyEvent::KEYCODE_DPAD_DOWN:
            case IKeyEvent::KEYCODE_DPAD_UP:
            case IKeyEvent::KEYCODE_DPAD_LEFT:
            case IKeyEvent::KEYCODE_DPAD_RIGHT:
                return TRUE;
        }
    }
    return FALSE;
}

//@Override
Boolean AwContents::AwViewMethodsImpl::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    if (mOwner->mNativeAwContents == 0) return FALSE;

    Int32 mask;
    event->GetActionMasked(&mask);
    if (mask == IMotionEvent::ACTION_DOWN) {
        mOwner->mSettings->SetSpatialNavigationEnabled(FALSE);
    }

    mOwner->mScrollOffsetManager->SetProcessingTouchEvent(TRUE);
    Boolean rv = mOwner->mContentViewCore->OnTouchEvent(event);
    mOwner->mScrollOffsetManager->SetProcessingTouchEvent(FALSE);

    if (mask == IMotionEvent::ACTION_DOWN) {
        Int32 actionIndex;
        event->GetActionIndex(&actionIndex);

        // Note this will trigger IPC back to browser even if nothing is
        // hit.
        Int32 x, y;
        event->GetX(actionIndex, &x);
        event->GetY(actionIndex, &y);
        NativeRequestNewHitTestDataAt(mOwner->mNativeAwContents,
                (Int32) Math::Round(x / mOwner->mDIPScale),
                (Int32) Math::Round(y / mOwner->mDIPScale));
    }

    if (mOwner->mOverScrollGlow != NULL) {
        if (mask == IMotionEvent::ACTION_DOWN) {
            mOwner->mOverScrollGlow->SetShouldPull(TRUE);
        }
        else if (mask == IMotionEvent::ACTION_UP ||
                mask == IMotionEvent::ACTION_CANCEL) {
            mOwner->mOverScrollGlow->SetShouldPull(FALSE);
            mOwner->mOverScrollGlow->ReleaseAll();
        }
    }

    return rv;
}

//@Override
Boolean AwContents::AwViewMethodsImpl::OnHoverEvent(
    /* [in] */ IMotionEvent* event)
{
    return mOwner->mContentViewCore->OnHoverEvent(event);
}

//@Override
Boolean AwContents::AwViewMethodsImpl::OnGenericMotionEvent(
    /* [in] */ IMotionEvent* event)
{
    return mOwner->mContentViewCore->OnGenericMotionEvent(event);
}

//@Override
void AwContents::AwViewMethodsImpl::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    mOwner->mContentViewCore->OnConfigurationChanged(newConfig);
}

//@Override
void AwContents::AwViewMethodsImpl::OnAttachedToWindow()
{
    if (mOwner->mNativeAwContents == 0) return;
    if (mOwner->mIsAttachedToWindow) {
//        Log.w(TAG, "onAttachedToWindow called when already attached. Ignoring");
        return;
    }
    mOwner->mIsAttachedToWindow = TRUE;

    mOwner->mContentViewCore->OnAttachedToWindow();
    Int32 width, height;
    mOwner->mContainerView->GetWidth(&width);
    mOwner->mContainerView->GetHeight(&height);
    AwContents::AwViewMethodsImpl::NativeOnAttachedToWindow(mOwner->mNativeAwContents, height, height);
    AwContents::AwViewMethodsImpl::UpdateHardwareAcceleratedFeaturesToggle();

    if (mOwner->mComponentCallbacks != NULL) return;
    mOwner->mComponentCallbacks = new AwComponentCallbacks();
    mOwner->mContext->RegisterComponentCallbacks(mOwner->mComponentCallbacks);
}

//@Override
void AwContents::AwViewMethodsImpl::OnDetachedFromWindow()
{
    if (!mOwner->mIsAttachedToWindow) {
//        Log.w(TAG, "onDetachedFromWindow called when already detached. Ignoring");
        return;
    }
    mOwner->mIsAttachedToWindow = FALSE;
    HideAutofillPopup();
    if (mOwner->mNativeAwContents != 0) {
        NativeOnDetachedFromWindow(mOwner->mNativeAwContents);
    }

    mOwner->mContentViewCore->OnDetachedFromWindow();
    UpdateHardwareAcceleratedFeaturesToggle();

    if (mOwner->mComponentCallbacks != NULL) {
        mOwner->mContext->UnregisterComponentCallbacks(mOwner->mComponentCallbacks);
        mOwner->mComponentCallbacks = NULL;
    }

    mOwner->mScrollAccessibilityHelper->RemovePostedCallbacks();
    mOwner->mNativeGLDelegate->DetachGLFunctor();
}

//@Override
void AwContents::AwViewMethodsImpl::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    mOwner->mWindowFocused = hasWindowFocus;
    mOwner->mContentViewCore->OnWindowFocusChanged(hasWindowFocus);
}

//@Override
void AwContents::AwViewMethodsImpl::OnFocusChanged(
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    mOwner->mContainerViewFocused = focused;
    mOwner->mContentViewCore->OnFocusChanged(focused);
}

//@Override
void AwContents::AwViewMethodsImpl::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 ow,
    /* [in] */ Int32 oh)
{
    if (mOwner->mNativeAwContents == 0) return;
    mOwner->mScrollOffsetManager->SetContainerViewSize(w, h);
    // The AwLayoutSizer needs to go first so that if we're in
    // fixedLayoutSize mode the update
    // to enter fixedLayoutSize mode is sent before the first resize
    // update.
    mOwner->mLayoutSizer->OnSizeChanged(w, h, ow, oh);
    mOwner->mContentViewCore->OnPhysicalBackingSizeChanged(w, h);
    mOwner->mContentViewCore->OnSizeChanged(w, h, ow, oh);
    mOwner->NativeOnSizeChanged(mOwner->mNativeAwContents, w, h, ow, oh);
}

//@Override
void AwContents::AwViewMethodsImpl::OnVisibilityChanged(
    /* [in] */ IView* changedView,
    /* [in] */ Int32 _visibility)
{
    Int32 visibility;
    mOwner->mContainerView->GetVisibility(&visibility);
    Boolean viewVisible = visibility == IView::VISIBLE;
    if (mOwner->mIsViewVisible == viewVisible) return;
    mOwner->SetViewVisibilityInternal(viewVisible);
}

//@Override
void AwContents::AwViewMethodsImpl::OnWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    Boolean windowVisible = visibility == IView::VISIBLE;
    if (mOwner->mIsWindowVisible == windowVisible) return;
    mOwner->setWindowVisibilityInternal(windowVisible);
}

//===============================================================
//                         AwContents
//===============================================================

AutoPtr<IRect> sLocalGlobalVisibleRectCreate()
{
    AutoPtr<IRect> rect;
    CRect::New((IRect**)&rect);
    return rect;
}

const String AwContents::TAG("AwContents");
const String AwContents::WEB_ARCHIVE_EXTENSION(".mht");
const Float AwContents::ZOOM_CONTROLS_EPSILON;
const AutoPtr<IRect> AwContents::sLocalGlobalVisibleRect = sLocalGlobalVisibleRectCreate();
const String AwContents::SAVE_RESTORE_STATE_KEY("WEBVIEW_CHROMIUM_STATE");
const Boolean AwContents::SUPPORTS_ON_ANIMATION = Build.VERSION.SDK_INT >= Build.VERSION_CODES.JELLY_BEAN;

void AwContents::Init(
    /* [in] */ AwBrowserContext* browserContext,
    /* [in] */ IViewGroup* containerView,
    /* [in] */ IContext* context,
    /* [in] */ InternalAccessDelegate* internalAccessAdapter,
    /* [in] */ NativeGLDelegate* nativeGLDelegate,
    /* [in] */ AwContentsClient* contentsClient,
    /* [in] */ AwSettings* settings,
    /* [in] */ DependencyFactory* dependencyFactory)
{
    mNativeAwContents = 0;
    mIsPaused = FALSE;
    mIsViewVisible = FALSE;
    mIsWindowVisible = FALSE;
    mIsAttachedToWindow = FALSE;
    mHasRequestedVisitedHistoryFromClient = FALSE;
    mBaseBackgroundColor = IColor::WHITE;
    mPossiblyStaleHitTestData = new HitTestData();
    mContainerViewFocused = FALSE;
    mWindowFocused = FALSE;
    mPageScaleFactor = 1.0f;
    mMinPageScaleFactor = 1.0f;
    mMaxPageScaleFactor = 1.0f;
    mContentWidthDip = 0.0f;
    mContentHeightDip = 0.0f;
    mDeferredShouldOverrideUrlLoadingIsPendingForPopup = FALSE;
    mTemporarilyDetached = FALSE;
    mOverlayHorizontalScrollbar = TRUE;
    mOverlayVerticalScrollbar = FALSE;


    mBrowserContext = browserContext;
    mContainerView = containerView;
    mContext = context;
    mInternalAccessAdapter = internalAccessAdapter;
    mNativeGLDelegate = nativeGLDelegate;
    mContentsClient = contentsClient;
    mAwViewMethods = new AwViewMethodsImpl();
    mFullScreenTransitionsState = new FullScreenTransitionsState(
            mContainerView, mInternalAccessAdapter, mAwViewMethods);
    mContentViewClient = new AwContentViewClient(contentsClient, settings, this, mContext);
    mLayoutSizer = dependencyFactory->CreateLayoutSizer();
    mSettings = settings;
    mDIPScale = DeviceDisplayInfo->Create(mContext)->GetDIPScale();
    AutoPtr<AwLayoutSizerDelegate> alsd = new AwLayoutSizerDelegate();
    mLayoutSizer->SetDelegate(alsd);
    mLayoutSizer->SetDIPScale(mDIPScale);
    mWebContentsDelegate = new AwWebContentsDelegateAdapter(
            contentsClient, mContainerView, mContext);
    mContentsClientBridge = new AwContentsClientBridge(contentsClient,
            mBrowserContext->GetKeyStore(), AwContentsStatics->GetClientCertLookupTable());
    mZoomControls = new AwZoomControls(this);
    mIoThreadClient = new IoThreadClientImpl();
    mInterceptNavigationDelegate = new InterceptNavigationDelegateImpl();

    AutoPtr<AwSettings::ZoomSupportChangeListener> zoomListener = new InnerZoomSupportChangeListener(this);
    mSettings->SetZoomListener(zoomListener);
    mDefaultVideoPosterRequestHandler = new DefaultVideoPosterRequestHandler(mContentsClient);
    mSettings->SetDefaultVideoPosterURL(
            mDefaultVideoPosterRequestHandler.getDefaultVideoPosterURL());
    mSettings->SetDIPScale(mDIPScale);

    AutoPtr<AwScrollOffsetManagerDelegate> asomd = new AwScrollOffsetManagerDelegate();
    AutoPtr<IOverScroller> overScroller;
    COverScroller::New(mContext, (IOverScroller**)&overScroller);
    mScrollOffsetManager = dependencyFactory->CreateScrollOffsetManager(asomd, overScroller);
    mScrollAccessibilityHelper = new ScrollAccessibilityHelper(mContainerView);

    SetOverScrollMode(mContainerView->GetOverScrollMode());
    SetScrollBarStyle(mInternalAccessAdapter->Super_getScrollBarStyle());
    mLayoutChangeListener = new AwLayoutChangeListener();
    mContainerView->AddOnLayoutChangeListener(mLayoutChangeListener);

    SetNewAwContents(NativeInit(mBrowserContext));

    OnContainerViewChanged();
}

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
AwContents::AwContents(
    /* [in] */ AwBrowserContext* browserContext,
    /* [in] */ IViewGroup* containerView,
    /* [in] */ IContext* context,
    /* [in] */ InternalAccessDelegate* internalAccessAdapter,
    /* [in] */ NativeGLDelegate* nativeGLDelegate,
    /* [in] */ AwContentsClient* contentsClient,
    /* [in] */ AwSettings* awSettings)
{
    AutoPtr<DependencyFactory> factory = new DependencyFactory();
    Init(browserContext, containerView, context, internalAccessAdapter, nativeGLDelegate,
            contentsClient, awSettings, factory);
}

/**
 * @param dependencyFactory an instance of the DependencyFactory used to provide instances of
 *                          classes that this class depends on.
 *
 * This version of the constructor is used in test code to inject test versions of the above
 * documented classes.
 */
AwContents::AwContents(
    /* [in] */ AwBrowserContext* browserContext,
    /* [in] */ IViewGroup* containerView,
    /* [in] */ IContext* context,
    /* [in] */ InternalAccessDelegate* internalAccessAdapter,
    /* [in] */ NativeGLDelegate* nativeGLDelegate,
    /* [in] */ AwContentsClient* contentsClient,
    /* [in] */ AwSettings* settings,
    /* [in] */ DependencyFactory* dependencyFactory)
{
    Init(browserContext, containerView, context,
          internalAccessAdapter, nativeGLDelegate, contentsClient, settings, dependencyFactory)
}

AutoPtr<ContentViewCore> AwContents::CreateAndInitializeContentViewCore(
    /* [in] */ IViewGroup* containerView,
    /* [in] */ IContext* context,
    /* [in] */ InternalAccessDelegate* internalDispatcher,
    /* [in] */ Int64 nativeWebContents,
    /* [in] */ GestureStateListener* gestureStateListener,
    /* [in] */ ContentViewClient* contentViewClient,
    /* [in] */ ContentViewCore::ZoomControlsDelegate* zoomControlsDelegate)
{
    AutoPtr<ContentViewCore> contentViewCore = new ContentViewCore(context);
    AutoPtr<WindowAndroid> window;
    AutoPtr<IContext> ctx = (IContext*)context->Probe(EIID_IActivity);
    if (ctx != NULL) {
        window = new ActivityWindowAndroid(ctx);
    }
    else {
        context->GetApplicationContext((IContext**)&ctx);
        window = WindowAndroid(ctx);
    }
    contentViewCore->Initialize(containerView, internalDispatcher, nativeWebContents, window);
    contentViewCore->AddGestureStateListener(gestureStateListener);
    contentViewCore->SetContentViewClient(contentViewClient);
    contentViewCore->SetZoomControlsDelegate(zoomControlsDelegate);
    return contentViewCore;
}

Boolean AwContents::IsFullScreen()
{
    return mFullScreenTransitionsState->IsFullScreen();
}

/**
 * Transitions this {@link AwContents} to fullscreen mode and returns the
 * {@link View} where the contents will be drawn while in fullscreen.
 */
AutoPtr<IView> AwContents::EnterFullScreen()
{
    assert(!IsFullScreen());

    // Detach to tear down the GL functor if this is still associated with the old
    // container view. It will be recreated during the next call to onDraw attached to
    // the new container view.
    OnDetachedFromWindow();

    // In fullscreen mode FullScreenView owns the AwViewMethodsImpl and AwContents
    // a NullAwViewMethods.
    AutoPtr<FullScreenView> fullScreenView = new FullScreenView(mContext, mAwViewMethods);
    mFullScreenTransitionsState->EnterFullScreen(fullScreenView);
    mAwViewMethods = new NullAwViewMethods(this, mInternalAccessAdapter, mContainerView);
    mContainerView->RemoveOnLayoutChangeListener(mLayoutChangeListener);
    fullScreenView->AddOnLayoutChangeListener(mLayoutChangeListener);

    // Associate this AwContents with the FullScreenView.
    SetInternalAccessAdapter(fullScreenView->GetInternalAccessAdapter());
    SetContainerView(fullScreenView);

    return fullScreenView;
}

/**
 * Returns this {@link AwContents} to embedded mode, where the {@link AwContents} are drawn
 * in the WebView.
 */
void AwContents::ExitFullScreen()
{
    if (!IsFullScreen())
        // exitFullScreen() can be called without a prior call to enterFullScreen() if a
        // "misbehave" app overrides onShowCustomView but does not add the custom view to
        // the window. Exiting avoids a crash.
        return;

    // Detach to tear down the GL functor if this is still associated with the old
    // container view. It will be recreated during the next call to onDraw attached to
    // the new container view.
    // NOTE: we cannot use mAwViewMethods here because its type is NullAwViewMethods.
    AutoPtr<AwViewMethods> awViewMethodsImpl = mFullScreenTransitionsState->GetInitialAwViewMethods();
    awViewMethodsImpl->OnDetachedFromWindow();

    // Swap the view delegates. In embedded mode the FullScreenView owns a
    // NullAwViewMethods and AwContents the AwViewMethodsImpl.
    AutoPtr<FullScreenView> fullscreenView = mFullScreenTransitionsState->GetFullScreenView();
    AutoPtr<NullAwViewMethods> viewMethods = new NullAwViewMethods(this, fullscreenView->GetInternalAccessAdapter(), fullscreenView);
    fullscreenView->SetAwViewMethods(viewMethods);
    mAwViewMethods = awViewMethodsImpl;
    AutoPtr<IViewGroup> initialContainerView = mFullScreenTransitionsState->GetInitialContainerView();
    initialContainerView->AddOnLayoutChangeListener(mLayoutChangeListener);
    fullscreenView->RemoveOnLayoutChangeListener(mLayoutChangeListener);

    // Re-associate this AwContents with the WebView.
    SetInternalAccessAdapter(mFullScreenTransitionsState->GetInitialInternalAccessDelegate());
    SetContainerView(initialContainerView);

    mFullScreenTransitionsState->ExitFullScreen();
}

void AwContents::SetInternalAccessAdapter(
    /* [in] */ InternalAccessDelegate* internalAccessAdapter)
{
    mInternalAccessAdapter = internalAccessAdapter;
    mContentViewCore->SetContainerViewInternals(mInternalAccessAdapter);
}

void AwContents::SetContainerView(
    /* [in] */ IViewGroup* newContainerView)
{
    mContainerView = newContainerView;
    mContentViewCore->SetContainerView(mContainerView);
    if (mAwPdfExporter != NULL) {
        mAwPdfExporter->SetContainerView(mContainerView);
    }
    mWebContentsDelegate->SetContainerView(mContainerView);

    OnContainerViewChanged();
}

/**
 * Reconciles the state of this AwContents object with the state of the new container view.
 */
void AwContents::OnContainerViewChanged()
{
    // NOTE: mAwViewMethods is used by the old container view, the WebView, so it might refer
    // to a NullAwViewMethods when in fullscreen. To ensure that the state is reconciled with
    // the new container view correctly, we bypass mAwViewMethods and use the real
    // implementation directly.
    AutoPtr<AwViewMethods> awViewMethodsImpl = mFullScreenTransitionsState->GetInitialAwViewMethods();
    awViewMethodsImpl->OnVisibilityChanged(mContainerView, mContainerView->GetVisibility());
    awViewMethodsImpl->OnWindowVisibilityChanged(mContainerView->GetWindowVisibility());
    if (mContainerView->IsAttachedToWindow()) {
        awViewMethodsImpl->OnAttachedToWindow();
    }
    else {
        awViewMethodsImpl->OnDetachedFromWindow();
    }
    awViewMethodsImpl->OnSizeChanged(
            mContainerView->GetWidth(), mContainerView->GetHeight(), 0, 0);
    awViewMethodsImpl->OnWindowFocusChanged(mContainerView->HasWindowFocus());
    awViewMethodsImpl->OnFocusChanged(mContainerView->HasFocus(), 0, NULL);
    mContainerView->RequestLayout();
}

/* Common initialization routine for adopting a native AwContents instance into this
 * java instance.
 *
 * TAKE CARE! This method can get called multiple times per java instance. Code accordingly.
 * ^^^^^^^^^  See the native class declaration for more details on relative object lifetimes.
 */
void AwContents::SetNewAwContents(
    /* [in] */ Int64 newAwContentsPtr)
{
    if (mNativeAwContents != 0) {
        Destroy();
        mContentViewCore = NULL;
    }

    assert(mNativeAwContents == 0 && mCleanupReference == NULL && mContentViewCore == NULL);

    mNativeAwContents = newAwContentsPtr;
    // TODO(joth): when the native and java counterparts of AwBrowserContext are hooked up to
    // each other, we should update |mBrowserContext| according to the newly received native
    // WebContent's browser context.

    // The native side object has been bound to this java instance, so now is the time to
    // bind all the native->java relationships.
    AutoPtr<IRunnable> runnable = new DestroyRunnable(mNativeAwContents);
    mCleanupReference = new CleanupReference(this, runnable);

    Int64 nativeWebContents = NativeGetWebContents(mNativeAwContents);
    AutoPtr<AwGestureStateListener> listener = new AwGestureStateListener();
    mContentViewCore = CreateAndInitializeContentViewCore(
            mContainerView, mContext, mInternalAccessAdapter, nativeWebContents,
            listener, mContentViewClient, mZoomControls);
    NativeSetJavaPeers(mNativeAwContents, this, mWebContentsDelegate, mContentsClientBridge,
            mIoThreadClient, mInterceptNavigationDelegate);
    mContentsClient->InstallWebContentsObserver(mContentViewCore);
    mSettings->SetWebContents(nativeWebContents);
    NativeSetDipScale(mNativeAwContents, (Float) mDIPScale);

    // The only call to onShow. onHide should never be called.
    mContentViewCore->OnShow();
}

/**
 * Called on the "source" AwContents that is opening the popup window to
 * provide the AwContents to host the pop up content.
 */
void AwContents::SupplyContentsForPopup(
    /* [in] */ AwContents newContents)
{
    Int64 popupNativeAwContents = NativeReleasePopupAwContents(mNativeAwContents);
    if (popupNativeAwContents == 0) {
//        Log.w(TAG, "Popup WebView bind failed: no pending content.");
        if (newContents != NULL) newContents->Destroy();
        return;
    }
    if (newContents == NULL) {
        NativeDestroy(popupNativeAwContents);
        return;
    }

    newContents->ReceivePopupContents(popupNativeAwContents);
}

// Recap: supplyContentsForPopup() is called on the parent window's content, this method is
// called on the popup window's content.
void AwContents::ReceivePopupContents(
    /* [in] */ Int64 popupNativeAwContents)
{
    mDeferredShouldOverrideUrlLoadingIsPendingForPopup = TRUE;
    // Save existing view state.
    const Boolean wasAttached = mIsAttachedToWindow;
    const Boolean wasViewVisible = mIsViewVisible;
    const Boolean wasWindowVisible = mIsWindowVisible;
    const Boolean wasPaused = mIsPaused;
    const Boolean wasFocused = mContainerViewFocused;
    const Boolean wasWindowFocused = mWindowFocused;

    // Properly clean up existing mContentViewCore and mNativeAwContents.
    if (wasFocused) OnFocusChanged(FALSE, 0, NULL);
    if (wasWindowFocused) OnWindowFocusChanged(FALSE);
    if (wasViewVisible) SetViewVisibilityInternal(FALSE);
    if (wasWindowVisible) SetWindowVisibilityInternal(FALSE);
    if (wasAttached) OnDetachedFromWindow();
    if (!wasPaused) OnPause();

    // Save injected JavaScript interfaces.
    Map<String, Pair<Object, Class>> javascriptInterfaces =
            new HashMap<String, Pair<Object, Class>>();
    if (mContentViewCore != null) {
        javascriptInterfaces.putAll(mContentViewCore.getJavascriptInterfaces());
    }

    SetNewAwContents(popupNativeAwContents);

    // Finally refresh all view state for mContentViewCore and mNativeAwContents.
    if (!wasPaused) OnResume();
    if (wasAttached) {
        OnAttachedToWindow();
        PostInvalidateOnAnimation();
    }
    OnSizeChanged(mContainerView->GetWidth(), mContainerView->GetHeight(), 0, 0);
    if (wasWindowVisible) SetWindowVisibilityInternal(TRUE);
    if (wasViewVisible) SetViewVisibilityInternal(TRUE);
    if (wasWindowFocused) OnWindowFocusChanged(wasWindowFocused);
    if (wasFocused) OnFocusChanged(TRUE, 0, NULL);

    // Restore injected JavaScript interfaces.
    for (Map.Entry<String, Pair<Object, Class>> entry : javascriptInterfaces.entrySet()) {
        mContentViewCore.addPossiblyUnsafeJavascriptInterface(
                entry.getValue().first,
                entry.getKey(),
                entry.getValue().second);
    }
}

/**
 * Deletes the native counterpart of this object.
 */
void AwContents::Destroy()
{
    if (mCleanupReference != NULL) {
        assert(mNativeAwContents != 0);
        // If we are attached, we have to call native detach to clean up
        // hardware resources.
        if (mIsAttachedToWindow) {
            NativeOnDetachedFromWindow(mNativeAwContents);
        }

        // We explicitly do not null out the mContentViewCore reference here
        // because ContentViewCore already has code to deal with the case
        // methods are called on it after it's been destroyed, and other
        // code relies on AwContents.mContentViewCore to be non-null.
        mContentViewCore->Destroy();
        mNativeAwContents = 0;

        mCleanupReference->CleanupNow();
        mCleanupReference = NULL;
    }

    assert(!mContentViewCore->IsAlive());
    assert(mNativeAwContents == 0);
}

//@VisibleForTesting
AutoPtr<ContentViewCore> AwContents::GetContentViewCore()
{
    return mContentViewCore;
}

// Can be called from any thread.
AutoPtr<AwSettings> AwContents::GetSettings()
{
    return mSettings;
}

AutoPtr<AwPdfExporter> AwContents::GetPdfExporter()
{
    // mNativeAwContents can be null, due to destroy().
    if (mNativeAwContents == 0) {
        return NULL;
    }
    if (mAwPdfExporter == NULL) {
        mAwPdfExporter = new AwPdfExporter(mContainerView);
        NativeCreatePdfExporter(mNativeAwContents, mAwPdfExporter);
    }
    return mAwPdfExporter;
}

void AwContents::SetAwDrawSWFunctionTable(
    /* [in] */ Int64 functionTablePointer)
{
    NativeSetAwDrawSWFunctionTable(functionTablePointer);
}

void AwContents::SetAwDrawGLFunctionTable(
    /* [in] */ Int64 functionTablePointer)
{
    NativeSetAwDrawGLFunctionTable(functionTablePointer);
}

Int64 AwContents::GetAwDrawGLFunction()
{
    return NativeGetAwDrawGLFunction();
}

void AwContents::SetShouldDownloadFavicons()
{
    NativeSetShouldDownloadFavicons();
}

/**
 * Disables contents of JS-to-Java bridge objects to be inspectable using
 * Object.keys() method and "for .. in" loops. This is intended for applications
 * targeting earlier Android releases where this was not possible, and we want
 * to ensure backwards compatible behavior.
 */
void AwContents::DisableJavascriptInterfacesInspection()
{
    mContentViewCore->SetAllowJavascriptInterfacesInspection(FALSE);
}

/**
 * Intended for test code.
 * @return the number of native instances of this class.
 */
//@VisibleForTesting
Int32 AwContents::GetNativeInstanceCount()
{
    return NativeGetNativeInstanceCount();
}

Int64 AwContents::GetAwDrawGLViewContext()
{
    // Only called during early construction, so client should not have had a chance to
    // call destroy yet.
    assert(mNativeAwContents != 0);

    // Using the native pointer as the returned viewContext. This is matched by the
    // reinterpret_cast back to BrowserViewRenderer pointer in the native DrawGLFunction.
    return NativeGetAwDrawGLViewContext(mNativeAwContents);
}

AutoPtr<IRect> AwContents::GetGlobalVisibleRect()
{
    if (!mContainerView->GetGlobalVisibleRect(sLocalGlobalVisibleRect)) {
        sLocalGlobalVisibleRect->SetEmpty();
    }
    return sLocalGlobalVisibleRect;
}

//--------------------------------------------------------------------------------------------
//  WebView[Provider] method implementations (where not provided by ContentViewCore)
//--------------------------------------------------------------------------------------------

void AwContents::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    mAwViewMethods->OnDraw(canvas);
}

void AwContents::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    mAwViewMethods->OnMeasure(widthMeasureSpec, heightMeasureSpec);
}

Int32 AwContents::GetContentHeightCss()
{
    return (Int32) Math::Ceil(mContentHeightDip);
}

Int32 AwContents::GetContentWidthCss()
{
    return (Int32) Math::Ceil(mContentWidthDip);
}

AutoPtr<IPicture> AwContents::CapturePicture()
{
    if (mNativeAwContents == 0) return NULL;
    AutoPtr<IPicture> picture = new AwPicture(NativeCapturePicture(mNativeAwContents,
            mScrollOffsetManager->ComputeHorizontalScrollRange(),
            mScrollOffsetManager->ComputeVerticalScrollRange()));

    return picture;
}

void AwContents::ClearView()
{
    if (mNativeAwContents == 0) return;
    NativeClearView(mNativeAwContents);
}

/**
 * Enable the onNewPicture callback.
 * @param enabled Flag to enable the callback.
 * @param invalidationOnly Flag to call back only on invalidation without providing a picture.
 */
void AwContents::EnableOnNewPicture(
    /* [in] */ Boolean enabled,
    /* [in] */ Boolean invalidationOnly)
{
    if (mNativeAwContents == 0) return;
    if (invalidationOnly) {
        mPictureListenerContentProvider = null;
    }
    else if (enabled && mPictureListenerContentProvider == NULL) {
        mPictureListenerContentProvider = new InnerCallable(this);
    }
    NativeEnableOnNewPicture(mNativeAwContents, enabled);
}

void AwContents::FindAllAsync(
    /* [in] */ String searchString)
{
    if (mNativeAwContents == 0) return;
    NativeFindAllAsync(mNativeAwContents, searchString);
}

void AwContents::FindNext(
    /* [in] */ Boolean forward)
{
    if (mNativeAwContents == 0) return;
    NativeFindNext(mNativeAwContents, forward);
}

void AwContents::ClearMatches()
{
    if (mNativeAwContents == 0) return;
    NativeClearMatches(mNativeAwContents);
}

/**
 * @return load progress of the WebContents.
 */
Int32 AwContents::GetMostRecentProgress()
{
    // WebContentsDelegateAndroid conveniently caches the most recent notified value for us.
    return mWebContentsDelegate->GetMostRecentProgress();
}

AutoPtr<IBitmap> AwContents::GetFavicon()
{
    return mFavicon;
}

void AwContents::RequestVisitedHistoryFromClient()
{
    AutoPtr<IValueCallback> callback = new InnerValueCallback(this);
    mContentsClient->GetVisitedHistory(callback);
}

/**
 * Load url without fixing up the url string. Consumers of ContentView are responsible for
 * ensuring the URL passed in is properly formatted (i.e. the scheme has been added if left
 * off during user input).
 *
 * @param params Parameters for this load.
 */
void AwContents::LoadUrl(
    /* [in] */ LoadUrlParams* params) {
    if (params->GetLoadUrlType() == LoadUrlParams::LOAD_TYPE_DATA &&
            !params->IsBaseUrlDataScheme()) {
        // This allows data URLs with a non-data base URL access to file:///android_asset/ and
        // file:///android_res/ URLs. If AwSettings.getAllowFileAccess permits, it will also
        // allow access to file:// URLs (subject to OS level permission checks).
        params->SetCanLoadLocalResources(TRUE);
    }

    // If we are reloading the same url, then set transition type as reload.
    if (params->GetUrl() != NULL &&
            params->GetUrl()->Equals(mContentViewCore->GetUrl()) &&
            params->GetTransitionType() == PageTransitionTypes::PAGE_TRANSITION_LINK) {
        params->SetTransitionType(PageTransitionTypes::PAGE_TRANSITION_RELOAD);
    }
    params->SetTransitionType(
            params->GetTransitionType() | PageTransitionTypes::PAGE_TRANSITION_FROM_API);

    // For WebView, always use the user agent override, which is set
    // every time the user agent in AwSettings is modified.
    params->SetOverrideUserAgent(LoadUrlParams::UA_OVERRIDE_TRUE);


    // We don't pass extra headers to the content layer, as WebViewClassic
    // was adding them in a very narrow set of conditions. See http://crbug.com/306873
    // However, if the embedder is attempting to inject a Referer header for their
    // loadUrl call, then we set that separately and remove it from the extra headers map/
    const String REFERER = "referer";
    Map<String, String> extraHeaders = params.getExtraHeaders();
    if (extraHeaders != null) {
        for (String header : extraHeaders.keySet()) {
            if (REFERER.equals(header.toLowerCase(Locale.US))) {
                params.setReferrer(new Referrer(extraHeaders.remove(header), 1));
                params.setExtraHeaders(extraHeaders);
                break;
            }
        }
    }

    if (mNativeAwContents != 0) {
        NativeSetExtraHeadersForUrl(
                mNativeAwContents, params->GetUrl(), params->GetExtraHttpRequestHeadersString());
    }
    params->SetExtraHeaders(new HashMap<String, String>());

    mContentViewCore->LoadUrl(params);

    // The behavior of WebViewClassic uses the populateVisitedLinks callback in WebKit.
    // Chromium does not use this use code path and the best emulation of this behavior to call
    // request visited links once on the first URL load of the WebView.
    if (!mHasRequestedVisitedHistoryFromClient) {
        mHasRequestedVisitedHistoryFromClient = TRUE;
        RequestVisitedHistoryFromClient();
    }

    if (params->GetLoadUrlType() == LoadUrlParams::LOAD_TYPE_DATA &&
            params->GetBaseUrl() != NULL) {
        // Data loads with a base url will be resolved in Blink, and not cause an onPageStarted
        // event to be sent. Sending the callback directly from here.
        mContentsClient->GetCallbackHelper()->PostOnPageStarted(params->GetBaseUrl());
    }
}

/**
 * Get the URL of the current page.
 *
 * @return The URL of the current page or null if it's empty.
 */
String AwContents::GetUrl()
{
    String url =  mContentViewCore->GetUrl();
    if (url == NULL || url.Trim().IsEmpty()) return String(NULL);
    return url;
}

void AwContents::RequestFocus() {
    mAwViewMethods->RequestFocus();
}

void AwContents::SetBackgroundColor(
    /* [in] */ Int32 color)
{
    mBaseBackgroundColor = color;
    if (mNativeAwContents != 0) NativeSetBackgroundColor(mNativeAwContents, color);
}

/**
 * @see android.view.View#setLayerType()
 */
void AwContents::SetLayerType(
    /* [in] */ Int32 layerType,
    /* [in] */ IPaint paint)
{
    mAwViewMethods->SetLayerType(layerType, paint);
}

Int32 AwContents::GetEffectiveBackgroundColor()
{
    // Do not ask the ContentViewCore for the background color, as it will always
    // report white prior to initial navigation or post destruction,  whereas we want
    // to use the client supplied base value in those cases.
    if (mNativeAwContents == 0 || !mContentsClient->IsCachedRendererBackgroundColorValid()) {
        return mBaseBackgroundColor;
    }
    return mContentsClient->GetCachedRendererBackgroundColor();
}

Boolean AwContents::IsMultiTouchZoomSupported()
{
    return mSettings->SupportsMultiTouchZoom();
}

AutoPtr<IView> AwContents::GetZoomControlsForTest()
{
    return mZoomControls->GetZoomControlsViewForTest();
}

/**
 * @see ContentViewCore#getContentSettings()
 */
AutoPtr<ContentSettings> AwContents::GetContentSettings()
{
    return mContentViewCore->GetContentSettings();
}

/**
 * @see View#setOverScrollMode(int)
 */
void AwContents::SetOverScrollMode(int mode)
{
    if (mode != View.OVER_SCROLL_NEVER) {
        mOverScrollGlow = new OverScrollGlow(mContext, mContainerView);
    }
    else {
        mOverScrollGlow = NULL;
    }
}

/**
 * @see View#setScrollBarStyle(int)
 */
void AwContents::SetScrollBarStyle(
    /* [in] */ Int32 style)
{
    if (style == IView::SCROLLBARS_INSIDE_OVERLAY
            || style == IView::SCROLLBARS_OUTSIDE_OVERLAY) {
        mOverlayHorizontalScrollbar = mOverlayVerticalScrollbar = TRUE;
    }
    else {
        mOverlayHorizontalScrollbar = mOverlayVerticalScrollbar = FALSE;
    }
}

/**
 * @see View#setHorizontalScrollbarOverlay(boolean)
 */
void AwContents::SetHorizontalScrollbarOverlay(
    /* [in] */ Boolean overlay)
{
    mOverlayHorizontalScrollbar = overlay;
}

/**
 * @see View#setVerticalScrollbarOverlay(boolean)
 */
void AwContents::SetVerticalScrollbarOverlay(
    /* [in] */ Boolean overlay)
{
    mOverlayVerticalScrollbar = overlay;
}

/**
 * @see View#overlayHorizontalScrollbar()
 */
Boolean AwContents::OverlayHorizontalScrollbar()
{
    return mOverlayHorizontalScrollbar;
}

/**
 * @see View#overlayVerticalScrollbar()
 */
Boolean AwContents::OverlayVerticalScrollbar()
{
    return mOverlayVerticalScrollbar;
}

/**
 * Called by the embedder when the scroll offset of the containing view has changed.
 * @see View#onScrollChanged(int,int)
 */
void AwContents::OnContainerViewScrollChanged(
    /* [in] */ Int32 l,
    /* [in] */ Int32 t,
    /* [in] */ Int32 oldl,
    /* [in] */ Int32 oldt)
{
    // A side-effect of View.onScrollChanged is that the scroll accessibility event being sent
    // by the base class implementation. This is completely hidden from the base classes and
    // cannot be prevented, which is why we need the code below.
    mScrollAccessibilityHelper->RemovePostedViewScrolledAccessibilityEventCallback();
    mScrollOffsetManager->OnContainerViewScrollChanged(l, t);
}

/**
 * Called by the embedder when the containing view is to be scrolled or overscrolled.
 * @see View#onOverScrolled(int,int,int,int)
 */
void AwContents::OnContainerViewOverScrolled(
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY,
    /* [in] */ Boolean clampedX,
    /* [in] */ Boolean clampedY)
{
    Int32 oldX = mContainerView->GetScrollX();
    Int32 oldY = mContainerView->GetScrollY();

    mScrollOffsetManager->OnContainerViewOverScrolled(scrollX, scrollY, clampedX, clampedY);

    if (mOverScrollGlow != NULL) {
        mOverScrollGlow->PullGlow(mContainerView->GetScrollX(), mContainerView->GetScrollY(),
                oldX, oldY,
                mScrollOffsetManager->ComputeMaximumHorizontalScrollOffset(),
                mScrollOffsetManager->ComputeMaximumVerticalScrollOffset());
    }
}

/**
 * @see android.webkit.WebView#requestChildRectangleOnScreen(View, Rect, boolean)
 */
Boolean AwContents::RequestChildRectangleOnScreen(
    /* [in] */ IView* child,
    /* [in] */ IRect* rect,
    /* [in] */ Boolean immediate)
{
    Int32 x, y, left, top;
    child->GetLeft(&left);
    child->GetScrollX(&x);
    child->GetTop(&top);
    child->GetScrollY(&y);
    return mScrollOffsetManager->RequestChildRectangleOnScreen(
            left - x, top - y,
            rect, immediate);
}

/**
 * @see View.computeScroll()
 */
void AwContents::ComputeScroll()
{
    mScrollOffsetManager->ComputeScrollAndAbsorbGlow(mOverScrollGlow);
}

/**
 * @see View#computeHorizontalScrollRange()
 */
Int32 AwContents::ComputeHorizontalScrollRange()
{
    return mScrollOffsetManager->ComputeHorizontalScrollRange();
}

/**
 * @see View#computeHorizontalScrollOffset()
 */
Int32 AwContents::ComputeHorizontalScrollOffset()
{
    return mScrollOffsetManager->ComputeHorizontalScrollOffset();
}

/**
 * @see View#computeVerticalScrollRange()
 */
Int32 AwContents::ComputeVerticalScrollRange()
{
    return mScrollOffsetManager->ComputeVerticalScrollRange();
}

/**
 * @see View#computeVerticalScrollOffset()
 */
Int32 AwContents::ComputeVerticalScrollOffset()
{
    return mScrollOffsetManager->ComputeVerticalScrollOffset();
}

/**
 * @see View#computeVerticalScrollExtent()
 */
Int32 AwContents::ComputeVerticalScrollExtent()
{
    return mScrollOffsetManager->ComputeVerticalScrollExtent();
}

/**
 * @see android.webkit.WebView#stopLoading()
 */
void AwContents::StopLoading()
{
    mContentViewCore->StopLoading();
}

/**
 * @see android.webkit.WebView#reload()
 */
void AwContents::Reload()
{
    mContentViewCore->Reload(TRUE);
}

/**
 * @see android.webkit.WebView#canGoBack()
 */
Boolean AwContents::CanGoBack()
{
    return mContentViewCore->CanGoBack();
}

/**
 * @see android.webkit.WebView#goBack()
 */
void AwContents::GoBack()
{
    mContentViewCore->GoBack();
}

/**
 * @see android.webkit.WebView#canGoForward()
 */
Boolean AwContents::CanGoForward()
{
    return mContentViewCore->CanGoForward();
}

/**
 * @see android.webkit.WebView#goForward()
 */
void AwContents::GoForward()
{
    mContentViewCore->GoForward();
}

/**
 * @see android.webkit.WebView#canGoBackOrForward(int)
 */
Boolean AwContents::CanGoBackOrForward(
    /* [in] */ Int32 steps)
{
    return mContentViewCore->CanGoToOffset(steps);
}

/**
 * @see android.webkit.WebView#goBackOrForward(int)
 */
void AwContents::GoBackOrForward(
    /* [in] */ Int32 steps)
{
    mContentViewCore->GoToOffset(steps);
}

/**
 * @see android.webkit.WebView#pauseTimers()
 */
void AwContents::PauseTimers()
{
    ContentViewStatics::SetWebKitSharedTimersSuspended(TRUE);
}

/**
 * @see android.webkit.WebView#resumeTimers()
 */
void AwContents::ResumeTimers() {
    ContentViewStatics::SetWebKitSharedTimersSuspended(FALSE);
}

/**
 * @see android.webkit.WebView#onPause()
 */
void AwContents::OnPause() {
    if (mIsPaused || mNativeAwContents == 0) return;
    mIsPaused = TRUE;
    NativeSetIsPaused(mNativeAwContents, mIsPaused);
}

/**
 * @see android.webkit.WebView#onResume()
 */
void AwContents::OnResume() {
    if (!mIsPaused || mNativeAwContents == 0) return;
    mIsPaused = FALSE;
    NativeSetIsPaused(mNativeAwContents, mIsPaused);
}

/**
 * @see android.webkit.WebView#isPaused()
 */
Boolean AwContents::IsPaused()
{
    return mIsPaused;
}

/**
 * @see android.webkit.WebView#onCreateInputConnection(EditorInfo)
 */
AutoPtr<InputConnection> AwContents::OnCreateInputConnection(
    /* [in] */ IEditorInfo* outAttrs)
{
    return mAwViewMethods->OnCreateInputConnection(outAttrs);
}

/**
 * @see android.webkit.WebView#onKeyUp(int, KeyEvent)
 */
Boolean AwContents::OnKeyUp(
    /* [in] */ Int32 keyCode,
    /* [in] */ IKeyEvent* event)
{
    return mAwViewMethods->OnKeyUp(keyCode, event);
}

/**
 * @see android.webkit.WebView#dispatchKeyEvent(KeyEvent)
 */
Boolean AwContents::DispatchKeyEvent(
    /* [in] */ IKeyEvent* event)
{
    return mAwViewMethods->DispatchKeyEvent(event);
}

/**
 * Clears the resource cache. Note that the cache is per-application, so this will clear the
 * cache for all WebViews used.
 *
 * @param includeDiskFiles if false, only the RAM cache is cleared
 */
void AwContents::ClearCache(
    /* [in] */ Boolean includeDiskFiles)
{
    if (mNativeAwContents == 0) return;
    NativeClearCache(mNativeAwContents, includeDiskFiles);
}

void AwContents::DocumentHasImages(
    /* [in] */ IMessage* message)
{
    if (mNativeAwContents == 0) return;
    NativeDocumentHasImages(mNativeAwContents, message);
}

void AwContents::SaveWebArchive(
    /* [in] */ const String& basename,
    /* [in] */ Boolean autoname,
    /* [in] */ const IValueCallback* callback)
{
    if (!autoname) {
        SaveWebArchiveInternal(basename, callback);
        return;
    }
    // If auto-generating the file name, handle the name generation on a background thread
    // as it will require I/O access for checking whether previous files existed.
    new AsyncTask<Void, Void, String>() {
        @Override
        protected String doInBackground(Void... params) {
            return generateArchiveAutoNamePath(getOriginalUrl(), basename);
        }

        @Override
        protected void onPostExecute(String result) {
            saveWebArchiveInternal(result, callback);
        }
    }.execute();
}

String AwContents::GetOriginalUrl()
{
    AutoPtr<NavigationHistory> history = mContentViewCore->GetNavigationHistory();
    Int32 currentIndex = history->GetCurrentEntryIndex();
    if (currentIndex >= 0 && currentIndex < history->GetEntryCount()) {
        return history->GetEntryAtIndex(currentIndex)->GetOriginalUrl();
    }
    return NULL;
}

/**
 * @see ContentViewCore#getNavigationHistory()
 */
AutoPtr<NavigationHistory> AwContents::GetNavigationHistory()
{
    return mContentViewCore->GetNavigationHistory();
}

/**
 * @see android.webkit.WebView#getTitle()
 */
String AwContents::GetTitle()
{
    return mContentViewCore->GetTitle();
}

/**
 * @see android.webkit.WebView#clearHistory()
 */
void AwContents::ClearHistory()
{
    mContentViewCore->ClearHistory();
}

AutoPtr< ArrayOf<String> > AwContents::GetHttpAuthUsernamePassword(
    /* [in] */ String host,
    /* [in] */ String realm)
{
    return mBrowserContext->GetHttpAuthDatabase(mContext)->GetHttpAuthUsernamePassword(host, realm);
}

void AwContents::SetHttpAuthUsernamePassword(
    /* [in] */ String host,
    /* [in] */ String realm,
    /* [in] */ String username,
    /* [in] */ String password)
{
    mBrowserContext->GetHttpAuthDatabase(mContext)->SetHttpAuthUsernamePassword(host, realm, username, password);
}

/**
 * @see android.webkit.WebView#getCertificate()
 */
AutoPtr<ISslCertificate> AwContents::GetCertificate()
{
    if (mNativeAwContents == 0) return NULL;
    return SslUtil::GetCertificateFromDerBytes(NativeGetCertificate(mNativeAwContents));
}

/**
 * @see android.webkit.WebView#clearSslPreferences()
 */
void AwContents::ClearSslPreferences()
{
    mContentViewCore->ClearSslPreferences();
}

// TODO(sgurun) remove after this rolls in. To keep internal tree happy.
void AwContents::ClearClientCertPreferences()
{
}

/**
 * Method to return all hit test values relevant to public WebView API.
 * Note that this expose more data than needed for WebView.getHitTestResult.
 * Unsafely returning reference to mutable internal object to avoid excessive
 * garbage allocation on repeated calls.
 */
AutoPtr<HitTestData> AwContents::GetLastHitTestResult()
{
    if (mNativeAwContents == 0) return NULL;
    NativeUpdateLastHitTestData(mNativeAwContents);
    return mPossiblyStaleHitTestData;
}

/**
 * @see android.webkit.WebView#requestFocusNodeHref()
 */
void AwContents::RequestFocusNodeHref(
    /* [in] */ IMessage* msg)
{
    if (msg == NULL || mNativeAwContents == 0) return;

    NativeUpdateLastHitTestData(mNativeAwContents);
    AutoPtr<IBundle> data;
    msg->GetData((IBundle**)&data);

    // In order to maintain compatibility with the old WebView's implementation,
    // the absolute (full) url is passed in the |url| field, not only the href attribute.
    // Note: HitTestData could be cleaned up at this point. See http://crbug.com/290992.
    data->PutString(String("url"), mPossiblyStaleHitTestData.href);
    data->PutString(String("title"), mPossiblyStaleHitTestData.anchorText);
    data->PutString(String("src"), mPossiblyStaleHitTestData.imgSrc);
    msg->SetData(data);
    msg->SendToTarget();
}

/**
 * @see android.webkit.WebView#requestImageRef()
 */
void AwContents::RequestImageRef(
    /* [in] */ IMessage* msg)
{
    if (msg == NULL || mNativeAwContents == 0) return;

    NativeUpdateLastHitTestData(mNativeAwContents);
    AutoPtr<IBundle> data;
    msg->GetData((IBundle**)&data);
    data->PutString(String("url"), mPossiblyStaleHitTestData.imgSrc);
    msg->SetData(data);
    msg->SendToTarget();
}

//@VisibleForTesting
Float AwContents::GetPageScaleFactor()
{
    return mPageScaleFactor;
}

/**
 * @see android.webkit.WebView#getScale()
 *
 * Please note that the scale returned is the page scale multiplied by
 * the screen density factor. See CTS WebViewTest.testSetInitialScale.
 */
Float AwContents::GetScale()
{
    return (Float)(mPageScaleFactor * mDIPScale);
}

/**
 * @see android.webkit.WebView#flingScroll(int, int)
 */
void AwContents::FlingScroll(
    /* [in] */ Int32 velocityX,
    /* [in] */ Int32 velocityY)
{
    mScrollOffsetManager->FlingScroll(velocityX, velocityY);
}

/**
 * @see android.webkit.WebView#pageUp(boolean)
 */
Boolean AwContents::PageUp(
    /* [in] */ Boolean top)
{
    return mScrollOffsetManager->PageUp(top);
}

/**
 * @see android.webkit.WebView#pageDown(boolean)
 */
Boolean AwContents::PageDown(
    /* [in] */ Boolean bottom)
{
    return mScrollOffsetManager->PageDown(bottom);
}

/**
 * @see android.webkit.WebView#canZoomIn()
 */
// This method uses the term 'zoom' for legacy reasons, but relates
// to what chrome calls the 'page scale factor'.
Boolean AwContents::CanZoomIn()
{
    const Float zoomInExtent = mMaxPageScaleFactor - mPageScaleFactor;
    return zoomInExtent > ZOOM_CONTROLS_EPSILON;
}

/**
 * @see android.webkit.WebView#canZoomOut()
 */
// This method uses the term 'zoom' for legacy reasons, but relates
// to what chrome calls the 'page scale factor'.
Boolean AwContents::CanZoomOut()
{
    const Float zoomOutExtent = mPageScaleFactor - mMinPageScaleFactor;
    return zoomOutExtent > ZOOM_CONTROLS_EPSILON;
}

/**
 * @see android.webkit.WebView#zoomIn()
 */
// This method uses the term 'zoom' for legacy reasons, but relates
// to what chrome calls the 'page scale factor'.
Boolean AwContents::ZoomIn()
{
    if (!CanZoomIn()) {
        return FALSE;
    }
    return ZoomBy(1.25f);
}

/**
 * @see android.webkit.WebView#zoomOut()
 */
// This method uses the term 'zoom' for legacy reasons, but relates
// to what chrome calls the 'page scale factor'.
Boolean AwContents::ZoomOut()
{
    if (!CanZoomOut()) {
        return FALSE;
    }
    return ZoomBy(0.8f);
}

/**
 * @see android.webkit.WebView#zoomBy()
 */
// This method uses the term 'zoom' for legacy reasons, but relates
// to what chrome calls the 'page scale factor'.
Boolean AwContents::ZoomBy(
    /* [in] */ Float delta)
{
    if (delta < 0.01f || delta > 100.0f) {
        throw new IllegalStateException("zoom delta value outside [0.01, 100] range.");
    }
    return mContentViewCore->PinchByDelta(delta);
}

/**
 * @see android.webkit.WebView#invokeZoomPicker()
 */
void AwContents::InvokeZoomPicker()
{
    mContentViewCore->InvokeZoomPicker();
}

/**
 * @see android.webkit.WebView#preauthorizePermission(Uri, long)
 */
void AwContents::PreauthorizePermission(
    /* [in] */ IUri* origin,
    /* [in] */ Int64 resources)
{
    if (mNativeAwContents == 0) return;
    String originStr;
    origin->ToString(&originStr);
    NativePreauthorizePermission(mNativeAwContents, originStr, resources);
}

/**
 * @see ContentViewCore.evaluateJavaScript(String, ContentViewCore.JavaScriptCallback)
 */
void AwContents::EvaluateJavaScript(
    /* [in] */ String script,
    /* [in] */ const IValueCallback* callback)
{
    AutoPtr<ContentViewCore::JavaScriptCallback> jsCallback;
    if (callback != NULL) {
        jsCallback = new InnerJavaScriptCallback(this, callback);
    }

    mContentViewCore->EvaluateJavaScript(script, jsCallback);
}

/**
 * @see ContentViewCore.evaluateJavaScriptEvenIfNotYetNavigated(String)
 */
void AwContents::EvaluateJavaScriptEvenIfNotYetNavigated(
    /* [in] */ String script)
{
    mContentViewCore->EvaluateJavaScriptEvenIfNotYetNavigated(script);
}

//--------------------------------------------------------------------------------------------
//  View and ViewGroup method implementations
//--------------------------------------------------------------------------------------------

/**
 * @see android.webkit.View#onTouchEvent()
 */
Boolean AwContents::OnTouchEvent(
    /* [in] */ IMotionEvent* event)
{
    return mAwViewMethods->OnTouchEvent(event);
}

/**
 * @see android.view.View#onHoverEvent()
 */
Boolean AwContents::OnHoverEvent(
    /* [in] */ IMotionEvent* event)
{
    return mAwViewMethods->OnHoverEvent(event);
}

/**
 * @see android.view.View#onGenericMotionEvent()
 */
Boolean AwContents::OnGenericMotionEvent(
    /* [in] */ IMotionEvent* event)
{
    return mContentViewCore->OnGenericMotionEvent(event);
}

/**
 * @see android.view.View#onConfigurationChanged()
 */
void AwContents::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    mAwViewMethods->OnConfigurationChanged(newConfig);
}

/**
 * @see android.view.View#onAttachedToWindow()
 */
void AwContents::OnAttachedToWindow()
{
    mTemporarilyDetached = FALSE;
    mAwViewMethods->OnAttachedToWindow();
}

/**
 * @see android.view.View#onDetachedFromWindow()
 */
//@SuppressLint("MissingSuperCall")
void AwContents::OnDetachedFromWindow()
{
    mAwViewMethods->OnDetachedFromWindow();
}

/**
 * @see android.view.View#onWindowFocusChanged()
 */
void AwContents::OnWindowFocusChanged(
    /* [in] */ Boolean hasWindowFocus)
{
    mAwViewMethods->OnWindowFocusChanged(hasWindowFocus);
}

/**
 * @see android.view.View#onFocusChanged()
 */
void AwContents::OnFocusChanged(
    /* [in] */ Boolean focused,
    /* [in] */ Int32 direction,
    /* [in] */ IRect* previouslyFocusedRect)
{
    if (!mTemporarilyDetached) {
        mAwViewMethods->OnFocusChanged(focused, direction, previouslyFocusedRect);
    }
}

/**
 * @see android.view.View#onStartTemporaryDetach()
 */
void AwContents::OnStartTemporaryDetach()
{
    mTemporarilyDetached = TRUE;
}

/**
 * @see android.view.View#onFinishTemporaryDetach()
 */
void AwContents::OnFinishTemporaryDetach()
{
    mTemporarilyDetached = FALSE;
}

/**
 * @see android.view.View#onSizeChanged()
 */
void AwContents::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 ow,
    /* [in] */ Int32t oh)
{
    mAwViewMethods->OnSizeChanged(w, h, ow, oh);
}

/**
 * @see android.view.View#onVisibilityChanged()
 */
void AwContents::OnVisibilityChanged(
    /* [in] */ IView* changedView,
    /* [in] */ Int32 visibility)
{
    mAwViewMethods->OnVisibilityChanged(changedView, visibility);
}

/**
 * @see android.view.View#onWindowVisibilityChanged()
 */
void AwContents::OnWindowVisibilityChanged(
    /* [in] */ Int32 visibility)
{
    mAwViewMethods->OnWindowVisibilityChanged(visibility);
}

void AwContents::SetViewVisibilityInternal(
    /* [in] */ Boolean visible)
{
    mIsViewVisible = visible;
    if (mNativeAwContents == 0) return;
    NativeSetViewVisibility(mNativeAwContents, mIsViewVisible);
}

void AwContents::SetWindowVisibilityInternal(
    /* [in] */ Boolean visible)
{
    mIsWindowVisible = visible;
    if (mNativeAwContents == 0) return;
    NativeSetWindowVisibility(mNativeAwContents, mIsWindowVisible);
}

/**
 * Save the state of this AwContents into provided Bundle.
 * @return False if saving state failed.
 */
Boolean AwContents::SaveState(
    /* [in] */ IBundle* outState)
{
    if (mNativeAwContents == 0 || outState == NULL) return FALSE;

    AutoPtr< ArrayOf<Byte> > state = NativeGetOpaqueState(mNativeAwContents);
    if (state == NULL) return FALSE;

    outState->PutByteArray(SAVE_RESTORE_STATE_KEY, state);
    return TRUE;
}

/**
 * Restore the state of this AwContents into provided Bundle.
 * @param inState Must be a bundle returned by saveState.
 * @return False if restoring state failed.
 */
Boolean AwContents::RestoreState(
    /* [in] */ IBundle* inState)
{
    if (mNativeAwContents == 0 || inState == NULL) return FALSE;

    AutoPtr< ArrayOf<Byte> > state = inState->GetByteArray(SAVE_RESTORE_STATE_KEY);
    if (state == NULL) return FALSE;

    Boolean result = NativeRestoreFromOpaqueState(mNativeAwContents, state);

    // The onUpdateTitle callback normally happens when a page is loaded,
    // but is optimized out in the restoreState case because the title is
    // already restored. See WebContentsImpl::UpdateTitleForEntry. So we
    // call the callback explicitly here.
    if (result) mContentsClient->OnReceivedTitle(mContentViewCore->GetTitle());

    return result;
}

/**
 * @see ContentViewCore#addPossiblyUnsafeJavascriptInterface(Object, String, Class)
 */
void AwContents::AddPossiblyUnsafeJavascriptInterface(
    /* [in] */ Object* object,
    /* [in] */ String name,
    /* [in] */ IInterface* requiredAnnotation)
{
    mContentViewCore->AddPossiblyUnsafeJavascriptInterface(object, name, requiredAnnotation);
}

/**
 * @see android.webkit.WebView#removeJavascriptInterface(String)
 */
void AwContents::RemoveJavascriptInterface(
    /* [in] */ String interfaceName)
{
    mContentViewCore->RemoveJavascriptInterface(interfaceName);
}

/**
 * If native accessibility (not script injection) is enabled, and if this is
 * running on JellyBean or later, returns an AccessibilityNodeProvider that
 * implements native accessibility for this view. Returns null otherwise.
 * @return The AccessibilityNodeProvider, if available, or null otherwise.
 */
AutoPtr<AccessibilityNodeProvider> AwContents::GetAccessibilityNodeProvider()
{
    return mContentViewCore->GetAccessibilityNodeProvider();
}

/**
 * @see android.webkit.WebView#onInitializeAccessibilityNodeInfo(AccessibilityNodeInfo)
 */
void AwContents::OnInitializeAccessibilityNodeInfo(
    /* [in] */ AccessibilityNodeInfo* info)
{
    mContentViewCore->OnInitializeAccessibilityNodeInfo(info);
}

/**
 * @see android.webkit.WebView#onInitializeAccessibilityEvent(AccessibilityEvent)
 */
void AwContents::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    mContentViewCore->OnInitializeAccessibilityEvent(event);
}

Boolean AwContents::SupportsAccessibilityAction(
    /* [in] */ Int32 action)
{
    return mContentViewCore->SupportsAccessibilityAction(action);
}

/**
 * @see android.webkit.WebView#performAccessibilityAction(int, Bundle)
 */
Boolean AwContents::PerformAccessibilityAction(
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments)
{
    return mContentViewCore->PerformAccessibilityAction(action, arguments);
}

/**
 * @see android.webkit.WebView#clearFormData()
 */
void AwContents::HideAutofillPopup()
{
    if (mAwAutofillClient != NULL) {
        mAwAutofillClient->HideAutofillPopup();
    }
}

void AwContents::SetNetworkAvailable(
    /* [in] */ Boolean networkUp)
{
    if (mNativeAwContents == 0) return;
    NativeSetJsOnlineProperty(mNativeAwContents, networkUp);
}

//--------------------------------------------------------------------------------------------
//  Methods called from native via JNI
//--------------------------------------------------------------------------------------------

//@CalledByNative
void AwContents::OnDocumentHasImagesResponse(
    /* [in] */ Boolean result,
    /* [in] */ IMessage* message)
{
    message->SetArg1(result ? 1 : 0);
    message->SendToTarget();
}

//@CalledByNative
void AwContents::OnReceivedTouchIconUrl(
    /* [in] */ String url,
    /* [in] */ boolean precomposed)
{
    mContentsClient->OnReceivedTouchIconUrl(url, precomposed);
}

//@CalledByNative
void AwContents::OnReceivedIcon(
    /* [in] */ IBitmap* bitmap)
{
    mContentsClient->OnReceivedIcon(bitmap);
    mFavicon = bitmap;
}

/** Callback for generateMHTML. */
//@CalledByNative
void AwContents::GenerateMHTMLCallback(
    /* [in] */ String path,
    /* [in] */ Int64 size,
    /* [in] */ IValueCallback* callback)
{
    if (callback == NULL) return;
    callback->OnReceiveValue(size < 0 ? NULL : path);
}

//@CalledByNative
void AwContents::OnReceivedHttpAuthRequest(
    /* [in] */ AwHttpAuthHandler* handler,
    /* [in] */ String host,
    /* [in] */ String realm)
{
    mContentsClient->OnReceivedHttpAuthRequest(handler, host, realm);
}

//@CalledByNative
void AwContents::OnGeolocationPermissionsShowPrompt(
    /* [in] */ String origin)
{
    if (mNativeAwContents == 0) return;
    AutoPtr<AwGeolocationPermissions> permissions = mBrowserContext->GetGeolocationPermissions();
    // Reject if geoloaction is disabled, or the origin has a retained deny
    if (!mSettings->GetGeolocationEnabled()) {
        NativeInvokeGeolocationCallback(mNativeAwContents, FALSE, origin);
        return;
    }
    // Allow if the origin has a retained allow
    if (permissions->HasOrigin(origin)) {
        NativeInvokeGeolocationCallback(mNativeAwContents, permissions->IsOriginAllowed(origin),
                origin);
        return;
    }
    AutoPtr<AwGeolocationCallback> callbak =  new AwGeolocationCallback();
    mContentsClient->OnGeolocationPermissionsShowPrompt(origin, callbak);
}

//@CalledByNative
void AwContents::OnGeolocationPermissionsHidePrompt()
{
    mContentsClient->OnGeolocationPermissionsHidePrompt();
}

//@CalledByNative
void AwContents::OnPermissionRequest(
    /* [in] */ AwPermissionRequest awPermissionRequest)
{
    mContentsClient->OnPermissionRequest(awPermissionRequest);
}

//@CalledByNative
void AwContents::OnPermissionRequestCanceled(
    /* [in] */ AwPermissionRequest* awPermissionRequest)
{
    mContentsClient->OnPermissionRequestCanceled(awPermissionRequest);
}

//@CalledByNative
void AwContents::OnFindResultReceived(
    /* [in] */ Int32 activeMatchOrdinal,
    /* [in] */ Int32 numberOfMatches,
    /* [in] */ Boolean isDoneCounting)
{
    mContentsClient->OnFindResultReceived(activeMatchOrdinal, numberOfMatches, isDoneCounting);
}

//@CalledByNative
void AwContents::OnNewPicture()
{
    // Don't call capturePicture() here but instead defer it until the posted task runs within
    // the callback helper, to avoid doubling back into the renderer compositor in the middle
    // of the notification it is sending up to here.
    mContentsClient->GetCallbackHelper()->PostOnNewPicture(mPictureListenerContentProvider);
}

// Called as a result of nativeUpdateLastHitTestData.
//@CalledByNative
void AwContents::UpdateHitTestData(
    /* [in] */ Int32 type,
    /* [in] */ String extra,
    /* [in] */ String href,
    /* [in] */ String anchorText,
    /* [in] */ String imgSrc)
{
    mPossiblyStaleHitTestData->hitTestResultType = type;
    mPossiblyStaleHitTestData->hitTestResultExtraData = extra;
    mPossiblyStaleHitTestData->href = href;
    mPossiblyStaleHitTestData->anchorText = anchorText;
    mPossiblyStaleHitTestData->imgSrc = imgSrc;
}

//@CalledByNative
Boolean AwContents::RequestDrawGL(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Boolean waitForCompletion)
{
    return mNativeGLDelegate->RequestDrawGL(canvas, waitForCompletion, mContainerView);
}

//@CalledByNative
void AwContents::PostInvalidateOnAnimation()
{
    if (SUPPORTS_ON_ANIMATION) {
        mContainerView->PostInvalidateOnAnimation();
    }
    else {
        mContainerView->PostInvalidate();
    }
}

// Call postInvalidateOnAnimation for invalidations. This is only used to synchronize
// draw functor destruction.
//@CalledByNative
void AwContents::InvalidateOnFunctorDestroy()
{
    mContainerView->Invalidate();
}

//@CalledByNative
AutoPtr< ArrayOf<Int32> > AwContents::GetLocationOnScreen()
{
    AutoPtr< ArrayOf<Int32> > result = ArrayOf<Int32>::Alloc(2);
    mContainerView->GetLocationOnScreen((ArrayOf<Int32>**)&result);
    return result;
}

//@CalledByNative
void AwContents::OnWebLayoutPageScaleFactorChanged(
    /* [in] */ Float webLayoutPageScaleFactor)
{
    // This change notification comes from the renderer thread, not from the cc/ impl thread.
    mLayoutSizer->OnPageScaleChanged(webLayoutPageScaleFactor);
}

//@CalledByNative
void AwContents::OnWebLayoutContentsSizeChanged(
    /* [in] */ Int32 widthCss,
    /* [in] */ Int32 heightCss)
{
    // This change notification comes from the renderer thread, not from the cc/ impl thread.
    mLayoutSizer->OnContentSizeChanged(widthCss, heightCss);
}

//@CalledByNative
void AwContents::ScrollContainerViewTo(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y)
{
    mScrollOffsetManager->ScrollContainerViewTo(x, y);
}

//@CalledByNative
Boolean AwContents::IsFlingActive()
{
    return mScrollOffsetManager->IsFlingActive();
}

//@CalledByNative
void AwContents::UpdateScrollState(
    /* [in] */ Int32 maxContainerViewScrollOffsetX,
    /* [in] */ Int32 maxContainerViewScrollOffsetY,
    /* [in] */ Int32 contentWidthDip,
    /* [in] */ Int32 contentHeightDip,
    /* [in] */ Float pageScaleFactor,
    /* [in] */ Float minPageScaleFactor,
    /* [in] */ Float maxPageScaleFactor)
{
    mContentWidthDip = contentWidthDip;
    mContentHeightDip = contentHeightDip;
    mScrollOffsetManager->SetMaxScrollOffset(maxContainerViewScrollOffsetX,
        maxContainerViewScrollOffsetY);
    SetPageScaleFactorAndLimits(pageScaleFactor, minPageScaleFactor, maxPageScaleFactor);
}

//@CalledByNative
void AwContents::SetAwAutofillClient(
    /* [in] */ AwAutofillClient* client)
{
    mAwAutofillClient = client;
    client->Init(mContentViewCore);
}

//@CalledByNative
void AwContents::DidOverscroll(
    /* [in] */ Int32 deltaX,
    /* [in] */ Int32 deltaY)
{
    if (mOverScrollGlow != NULL) {
        mOverScrollGlow->SetOverScrollDeltas(deltaX, deltaY);
    }

    mScrollOffsetManager->OverScrollBy(deltaX, deltaY);

    if (mOverScrollGlow != NULL && mOverScrollGlow->IsAnimating()) {
        mContainerView->Invalidate();
    }
}

// -------------------------------------------------------------------------------------------
// Helper methods
// -------------------------------------------------------------------------------------------

void AwContents::SetPageScaleFactorAndLimits(
    /* [in] */ Float pageScaleFactor,
    /* [in] */ Float minPageScaleFactor,
    /* [in] */ Float maxPageScaleFactor)
{
    if (mPageScaleFactor == pageScaleFactor &&
            mMinPageScaleFactor == minPageScaleFactor &&
            mMaxPageScaleFactor == maxPageScaleFactor) {
        return;
    }
    mMinPageScaleFactor = minPageScaleFactor;
    mMaxPageScaleFactor = maxPageScaleFactor;
    if (mPageScaleFactor != pageScaleFactor) {
        Float oldPageScaleFactor = mPageScaleFactor;
        mPageScaleFactor = pageScaleFactor;
        mContentsClient->GetCallbackHelper()->PostOnScaleChangedScaled(
                (Float)(oldPageScaleFactor * mDIPScale),
                (Float)(mPageScaleFactor * mDIPScale));
    }
}

void AwContents::SaveWebArchiveInternal(
    /* [in] */ String path,
    /* [in] */ const IValueCallback* callback)
{
    if (path == null || mNativeAwContents == 0) {
        AutoPtr<IRunnable> runnable = new SaveWebArchiveInternalRunnable(this, callback);
        ThreadUtils::RunOnUiThread(runnable);
    }
    else {
        NativeGenerateMHTML(mNativeAwContents, path, callback);
    }
}

/**
 * Try to generate a pathname for saving an MHTML archive. This roughly follows WebView's
 * autoname logic.
 */
String AwContents::GenerateArchiveAutoNamePath(
    /* [in] */ String originalUrl,
    /* [in] */ String baseName)
{
    String name;
    if (originalUrl != NULL && !originalUrl.IsEmpty()) {
        //try {
            AutoPtr<IUrl> url;
            CUrl::New(originalUrl, (IUrl**)&url);
            String path;
            url->GetPath(&path);
            Int32 lastSlash = path.LastIndexOf('/');
            if (lastSlash > 0) {
                name = path.Substring(lastSlash + 1);
            }
            else {
                name = path;
            }
        //} catch (MalformedURLException e) {
            // If it fails parsing the URL, we'll just rely on the default name below.
        //}
    }

    if (TextUtils::IsEmpty(name)) name = "index";

    String testName = baseName + name + WEB_ARCHIVE_EXTENSION;
    AutoPtr<IFile> file;
    CFile::New(testName, (IFile**)&file);
    Boolean bExists = FALSE;
    file->Exists(&bExists);
    if (!bExists) return testName;

    for (Int32 i = 1; i < 100; i++) {
        testName = baseName + name + "-" + i + WEB_ARCHIVE_EXTENSION;
        if (!bExists) return testName;
    }

//    Log.e(TAG, "Unable to auto generate archive name for path: " + baseName);
    return String(NULL);
}

//@Override
void AwContents::ExtractSmartClipData(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    mContentViewCore->ExtractSmartClipData(x, y, width, height);
}

//@Override
void AwContents::SetSmartClipResultHandler(
    /* [in] */ const IHandler* resultHandler)
{
    if (resultHandler == NULL) {
        mContentViewCore->SetSmartClipDataListener(NULL);
        return;
    }

    AutoPtr<ContentViewCore::SmartClipDataListener> listener = new InnerSmartClipDataListener(this, resultHandler);
    mContentViewCore->SetSmartClipDataListener(listener);
}

// Return true if the GeolocationPermissionAPI should be used.
//@CalledByNative
Boolean AwContents::UseLegacyGeolocationPermissionAPI()
{
    // Always return true since we are not ready to swap the geolocation yet.
    // TODO: If we decide not to migrate the geolocation, there are some unreachable
    // code need to remove. http://crbug.com/396184.
    return TRUE;
}

//--------------------------------------------------------------------------------------------
//  Native methods
//--------------------------------------------------------------------------------------------

Int64 AwContents::NativeInit(
    /* [in] */ AwBrowserContext* browserContext);

void AwContents::NativeDestroy(
    /* [in] */ Int64 nativeAwContents);

void AwContents::NativeSetAwDrawSWFunctionTable(
    /* [in] */ Int64 functionTablePointer);

void AwContents::NativeSetAwDrawGLFunctionTable(
    /* [in] */ Int64 functionTablePointer);

Int64 AwContents::NativeGetAwDrawGLFunction();

Int32 AwContents::NativeGetNativeInstanceCount();

void AwContents::NativeSetShouldDownloadFavicons();

void AwContents::NativeSetJavaPeers(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ AwContents* awContents,
    /* [in] */ AwWebContentsDelegate* webViewWebContentsDelegate,
    /* [in] */ AwContentsClientBridge* contentsClientBridge,
    /* [in] */ AwContentsIoThreadClient* ioThreadClient,
    /* [in] */ InterceptNavigationDelegate* navigationInterceptionDelegate);

Int64 AwContents::NativeGetWebContents(
    /* [in] */ Int64 nativeAwContents);

void AwContents::NativeDocumentHasImages(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ IMessage* message);

void AwContents::NativeGenerateMHTML(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ String path,
    /* [in] */ IValueCallback* callback);

void AwContents::NativeAddVisitedLinks(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ ArrayOf<String>* visitedLinks);

Boolean AwContents::NativeOnDraw(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ ICanvas* canvas,
    /* [in] */ Boolean isHardwareAccelerated,
    /* [in] */ Int32 scrollX,
    /* [in] */ Int32 scrollY,
    /* [in] */ Int32 visibleLeft,
    /* [in] */ Int32 visibleTop,
    /* [in] */ Int32 visibleRight,
    /* [in] */ Int32 visibleBottom);

void AwContents::NativeFindAllAsync(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ String searchString);

void AwContents::NativeFindNext(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ Boolean forward);

void AwContents::NativeClearMatches(
    /* [in] */ Int64 nativeAwContents);

void AwContents::NativeClearCache(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ Boolean includeDiskFiles);

AutoPtr< ArrayOf<Byte> > AwContents::NativeGetCertificate(
    /* [in] */ Int64 nativeAwContents);

// Coordinates in desity independent pixels.
void AwContents::NativeRequestNewHitTestDataAt(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y);

void AwContents::NativeUpdateLastHitTestData(
    /* [in] */ Int64 nativeAwContents);

void AwContents::NativeOnSizeChanged(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */  Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 ow,
    /* [in] */ Int32 oh);

void AwContents::NativeScrollTo(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y);

void AwContents::NativeSetViewVisibility(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ boolean visible);

void AwContents::NativeSetWindowVisibility(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ Boolean visible);

void AwContents::NativeSetIsPaused(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ Boolean paused);

void AwContents::NativeOnAttachedToWindow(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ Int32 w,
    /* [in] */ Int32 h);

void AwContents::NativeOnDetachedFromWindow(
    /* [in] */ Int64 nativeAwContents);

void AwContents::NativeSetDipScale(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ Float dipScale);

// Returns null if save state fails.
AutoPtr< ArrayOf<Byte> > AwContents::NativeGetOpaqueState(
    /* [in] */ Int64 nativeAwContents);

// Returns false if restore state fails.
Boolean AwContents::NativeRestoreFromOpaqueState(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ ArrayOf<Byte>* state);

Int64 AwContents::NativeReleasePopupAwContents(
    /* [in] */ Int64 nativeAwContents);

void AwContents::NativeFocusFirstNode(
    /* [in] */ Int64 nativeAwContents);

void AwContents::NativeSetBackgroundColor(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ Int32 color);

Int64 AwContents::NativeGetAwDrawGLViewContext(
    /* [in] */ Int64 nativeAwContents);

Int64 AwContents::NativeCapturePicture(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height);

void AwContents::NativeEnableOnNewPicture(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ Boolean enabled);

void AwContents::NativeClearView(
    /* [in] */ Int64 nativeAwContents);

void AwContents::NativeSetExtraHeadersForUrl(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ String url,
    /* [in] */ String extraHeaders);

void AwContents::NativeInvokeGeolocationCallback(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ Boolean value,
    /* [in] */ String requestingFrame);

void AwContents::NativeSetJsOnlineProperty(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ Boolean networkUp);

void AwContents::NativeTrimMemory(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ Int32 level,
    /* [in] */ Boolean visible);

void AwContents::NativeCreatePdfExporter(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ AwPdfExporter* awPdfExporter);

void AwContents::NativePreauthorizePermission(
    /* [in] */ Int64 nativeAwContents,
    /* [in] */ String origin,
    /* [in] */ Int64 resources);

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
