
#include "view/CViewRootImplW.h"
#include "view/ViewRootImpl.h"
#include "app/ActivityManagerNative.h"
#include "os/Binder.h"

using Elastos::Droid::App::IIActivityManager;
using Elastos::Droid::App::ActivityManagerNative;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Os::Binder;

namespace Elastos {
namespace Droid {
namespace View {

ECode CViewRootImplW::constructor(
    /* [in] */ Handle32 _viewRoot)
{
    ViewRootImpl* viewRoot = (ViewRootImpl*)_viewRoot;
    assert(viewRoot);
    ((IWeakReferenceSource*)viewRoot->Probe(EIID_IWeakReferenceSource))->GetWeakReference((IWeakReference**)&mViewAncestor);
    mWindowSession = viewRoot->mWindowSession;

    return NOERROR;
}

ECode CViewRootImplW::Resized(
    /* [in] */ IRect* frame,
    /* [in] */ IRect* coveredInsets,
    /* [in] */ IRect* visibleInsets,
    /* [in] */ Boolean reportDraw,
    /* [in] */ IConfiguration* newConfig)
{
    AutoPtr<ViewRootImpl> viewRoot = GetViewRootImpl();
    if (viewRoot != NULL) {
        viewRoot->DispatchResized(frame, coveredInsets,
            visibleInsets, reportDraw, newConfig);
    }

    return NOERROR;
}

ECode CViewRootImplW::Moved(
    /* [in] */ Int32 newX,
    /* [in] */ Int32 newY)
{
    AutoPtr<ViewRootImpl> viewRoot = GetViewRootImpl();
    if (viewRoot != NULL) {
        viewRoot->DispatchMoved(newX, newY);
    }

    return NOERROR;
}

ECode CViewRootImplW::DispatchAppVisibility(
    /* [in] */ Boolean visible)
{
    AutoPtr<ViewRootImpl> viewRoot = GetViewRootImpl();
    if (viewRoot != NULL) {
        viewRoot->DispatchAppVisibility(visible);
    }

    return NOERROR;
}

ECode CViewRootImplW::DispatchScreenState(
    /* [in] */ Boolean on)
{
    AutoPtr<ViewRootImpl> viewRoot = GetViewRootImpl();
    if (viewRoot != NULL) {
        viewRoot->DispatchScreenStateChange(on);
    }

    return NOERROR;
}

ECode CViewRootImplW::DispatchGetNewSurface()
{
    AutoPtr<ViewRootImpl> viewRoot = GetViewRootImpl();
    if (viewRoot != NULL) {
        viewRoot->DispatchGetNewSurface();
    }

    return NOERROR;
}

ECode CViewRootImplW::WindowFocusChanged(
    /* [in] */ Boolean hasFocus,
    /* [in] */ Boolean inTouchMode)
{
    AutoPtr<ViewRootImpl> viewRoot = GetViewRootImpl();
    if (viewRoot != NULL) {
        viewRoot->WindowFocusChanged(hasFocus, inTouchMode);
    }

    return NOERROR;
}

Int32 CViewRootImplW::CheckCallingPermission(
    /* [in] */ const String& permission)
{
    AutoPtr<IIActivityManager> am = ActivityManagerNative::GetDefault();

    Int32 res;
    if (FAILED(am->CheckPermission(
            permission, Binder::GetCallingPid(), Binder::GetCallingUid(), &res))) {
        return IPackageManager::PERMISSION_DENIED;
    }

    return res;
}

ECode CViewRootImplW::ExecuteCommand(
    /* [in] */ const String& command,
    /* [in] */ const String& parameters,
    /* [in] */ IParcelFileDescriptor* descriptor)
{
//    final ViewRootImpl viewRoot = mViewAncestor.get();
//    if (viewRoot != null) {
//        final View view = viewRoot.mView;
//        if (view != null) {
//            if (checkCallingPermission(Elastos::Droid::Manifest::Permission::DUMP) !=
//                    PackageManager.PERMISSION_GRANTED) {
//                throw new SecurityException("Insufficient permissions to invoke"
//                        + " executeCommand() from pid=" + Binder.getCallingPid()
//                        + ", uid=" + Binder.getCallingUid());
//            }
//
//            OutputStream clientStream = null;
//            try {
//                clientStream = new ParcelFileDescriptor.AutoCloseOutputStream(out);
//                ViewDebug.dispatchCommand(view, command, parameters, clientStream);
//            } catch (IOException e) {
//                e.printStackTrace();
//            } finally {
//                if (clientStream != null) {
//                    try {
//                        clientStream.close();
//                    } catch (IOException e) {
//                        e.printStackTrace();
//                    }
//                }
//            }
//        }
//    }
    return E_NOT_IMPLEMENTED;
}

ECode CViewRootImplW::CloseSystemDialogs(
    /* [in] */ const String& reason)
{
    AutoPtr<ViewRootImpl> viewRoot = GetViewRootImpl();
    if (viewRoot != NULL) {
        viewRoot->DispatchCloseSystemDialogs(reason);
    }

    return NOERROR;
}

ECode CViewRootImplW::DispatchWallpaperOffsets(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float xStep,
    /* [in] */ Float yStep,
    /* [in] */ Boolean sync)
{
    if (sync) {
        return mWindowSession->WallpaperOffsetsComplete(IBinder::Probe(this));
    }

    return NOERROR;
}

ECode CViewRootImplW::DispatchWallpaperCommand(
    /* [in] */ const String& action,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z,
    /* [in] */ IBundle* extras,
    /* [in] */ Boolean sync)
{
    if (sync) {
        return mWindowSession->WallpaperCommandComplete(IBinder::Probe(this), NULL);
    }

    return NOERROR;
}

/* Drag/drop */
ECode CViewRootImplW::DispatchDragEvent(
    /* [in] */ IDragEvent* event)
{
    AutoPtr<ViewRootImpl> viewRoot = GetViewRootImpl();
    if (viewRoot != NULL) {
        viewRoot->DispatchDragEvent(event);
    }

    return NOERROR;
}

ECode CViewRootImplW::DispatchSystemUiVisibilityChanged(
    /* [in] */ Int32 seq,
    /* [in] */ Int32 globalVisibility,
    /* [in] */ Int32 localValue,
    /* [in] */ Int32 localChanges)
{
    AutoPtr<ViewRootImpl> viewRoot = GetViewRootImpl();
    if (viewRoot != NULL) {
        viewRoot->DispatchSystemUiVisibilityChanged(
            seq, globalVisibility, localValue, localChanges);
    }

    return NOERROR;
}

ECode CViewRootImplW::DoneAnimating()
{
    AutoPtr<ViewRootImpl> viewRoot = GetViewRootImpl();
    if (viewRoot != NULL) {
        viewRoot->DispatchDoneAnimating();
    }

    return NOERROR;
}

ECode CViewRootImplW::ToString(
    /* [out] */ String* description)
{
    VALIDATE_NOT_NULL(description);
    *description = String("CViewRootImplW");
    return NOERROR;
}

AutoPtr<ViewRootImpl> CViewRootImplW::GetViewRootImpl()
{
    AutoPtr<IInterface> infc;
    mViewAncestor->Resolve(EIID_IInterface, (IInterface**)&infc);
    AutoPtr<ViewRootImpl> viewRoot;
    if (infc != NULL) {
        viewRoot = reinterpret_cast<ViewRootImpl*>(infc->Probe(EIID_ViewRootImpl));
    }
    return viewRoot;
}

} // namespace View
} // namespace Droid
} // namespace Elastos
