
#include "CEGL14.h"
#include "EGLContextC.h"
#include "EGLDisplayC.h"
#include "EGLSurfaceC.h"
#include "EGLConfigC.h"
#include <EGL/egl.h>
#include <gui/SurfaceTexture.h>
#include <gui/SurfaceTextureClient.h>
#include <elastos/utility/logging/Slogger.h>

#include <ui/ANativeObjectBase.h>

#define LOGD(msg) SLOGGERD("CEGL14", msg)

using Elastos::Droid::View::ISurface;
using Elastos::Droid::View::ISurfaceView;
using Elastos::Droid::View::ISurfaceHolder;
using Elastos::Droid::Graphics::ISurfaceTexture;

namespace Elastos {
namespace Droid {
namespace Opengl {

AutoPtr<IEGLContext> CEGL14::InitStaticContext()
{
    AutoPtr<IEGLContext> tmp = EGLContextC::CreateInstance((Int32)EGL_NO_CONTEXT);
    return tmp;
}

AutoPtr<IEGLDisplay> CEGL14::InitStaticDisplay()
{
    AutoPtr<IEGLDisplay> tmp = EGLDisplayC::CreateInstance((Int32)EGL_NO_DISPLAY);
    return tmp;
}

AutoPtr<IEGLSurface> CEGL14::InitStaticSurface()
{
    AutoPtr<IEGLSurface> tmp = EGLSurfaceC::CreateInstance((Int32)EGL_NO_SURFACE);
    return tmp;
}

AutoPtr<IEGLContext> CEGL14::eglNoContextObject = InitStaticContext();
AutoPtr<IEGLDisplay> CEGL14::eglNoDisplayObject = InitStaticDisplay();
AutoPtr<IEGLSurface> CEGL14::eglNoSurfaceObject = InitStaticSurface();


ECode CEGL14::EglGetError(
    /* [out] */ Int32* error)
{
    EGLint _returnValue = (EGLint) 0;
    _returnValue = eglGetError();
    *error = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglGetDisplay(
    /* [in] */ Int32 display_id,
    /* [out] */ Elastos::Droid::Opengl::IEGLDisplay** display)
{
    EGLDisplay _returnValue = (EGLDisplay) 0;
    _returnValue = eglGetDisplay(
        (EGLNativeDisplayType)display_id
    );
    *display = IEGLDisplay::Probe(ToEGLHandle(EIID_IEGLDisplay, _returnValue));
    return NOERROR;
}

ECode CEGL14::EglInitialize(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ ArrayOf<Int32>* major_ref,
    /* [in] */ Int32 majorOffset,
    /* [in] */ ArrayOf<Int32>* minor_ref,
    /* [in] */ Int32 minorOffset,
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);
    EGLint *major_base = (EGLint *) 0;
    Int32 _majorRemaining;
    EGLint *major = (EGLint *) 0;
    EGLint *minor_base = (EGLint *) 0;
    Int32 _minorRemaining;
    EGLint *minor = (EGLint *) 0;

    if (!major_ref) {
        LOGD("EglInitialize: major == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (majorOffset < 0) {
        LOGD("EglInitialize: majorOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _majorRemaining = major_ref->GetLength() - majorOffset;
    if (_majorRemaining < 1) {
        LOGD("EglInitialize: length - majorOffset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    major_base = (EGLint *) major_ref->GetPayload();
    major = major_base + majorOffset;

    if (!minor_ref) {
        LOGD("EglInitialize: minor == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (minorOffset < 0) {
        LOGD("EglInitialize: minorOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _minorRemaining = minor_ref->GetLength() - minorOffset;
    if (_minorRemaining < 1) {
        LOGD("EglInitialize: length - minorOffset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    minor_base = (EGLint *) minor_ref->GetPayload();
    minor = minor_base + minorOffset;

    _returnValue = eglInitialize(
        (EGLDisplay)dpy_native,
        (EGLint *)major,
        (EGLint *)minor
    );
    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglTerminate(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);

    _returnValue = eglTerminate(
        (EGLDisplay)dpy_native
    );
    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglQueryString(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Int32 name,
    /* [out] */ String* str)
{
    const char* chars = (const char*) eglQueryString(
        (EGLDisplay)FromEGLHandle(dpy),
        (EGLint)name
    );
    *str = String(chars);
    return NOERROR;
}

ECode CEGL14::EglGetConfigs(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ ArrayOf<Elastos::Droid::Opengl::IEGLConfig *>* configs_ref,
    /* [in] */ Int32 configsOffset,
    /* [in] */ Int32 config_size,
    /* [in] */ ArrayOf<Int32>* num_config_ref,
    /* [in] */ Int32 num_configOffset,
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);
    Int32 _configsRemaining;
    EGLConfig *configs = (EGLConfig *) 0;
    EGLint *num_config_base = (EGLint *) 0;
    EGLint *num_config = (EGLint *) 0;

    if (!configs_ref) {
        LOGD("EglGetConfigs: configs == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (configsOffset < 0) {
        LOGD("EglGetConfigs: configsOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _configsRemaining = configs_ref->GetLength() - configsOffset;
    if (_configsRemaining < config_size) {
        LOGD("EglGetConfigs: length - configsOffset < config_size < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    configs = new EGLConfig[_configsRemaining];

    if (!num_config_ref) {
        LOGD("EglGetConfigs: num_config == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (num_configOffset < 0) {
        LOGD("EglGetConfigs: num_configOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    num_config_base = (EGLint *) num_config_ref->GetPayload();
    num_config = num_config_base + num_configOffset;

    _returnValue = eglGetConfigs(
        (EGLDisplay)dpy_native,
        (EGLConfig *)configs,
        (EGLint)config_size,
        (EGLint *)num_config
    );
    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglChooseConfig(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ ArrayOf<Int32>* attrib_list_ref,
    /* [in] */ Int32 attrib_listOffset,
    /* [in] */ ArrayOf<Elastos::Droid::Opengl::IEGLConfig *>* configs_ref,
    /* [in] */ Int32 configsOffset,
    /* [in] */ Int32 config_size,
    /* [in] */ ArrayOf<Int32>* num_config_ref,
    /* [in] */ Int32 num_configOffset,
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);
    Boolean attrib_list_sentinel = FALSE;
    EGLint *attrib_list_base = (EGLint *) 0;
    Int32 _attrib_listRemaining;
    EGLint *attrib_list = (EGLint *) 0;
    Int32 _configsRemaining;
    EGLConfig *configs = (EGLConfig *) 0;
    EGLint *num_config_base = (EGLint *) 0;
    Int32 _num_configRemaining;
    EGLint *num_config = (EGLint *) 0;

    if (!attrib_list_ref) {
        LOGD("EglChooseConfig: attrib_list == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (attrib_listOffset < 0) {
        LOGD("EglChooseConfig: attrib_listOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _attrib_listRemaining = attrib_list_ref->GetLength() - attrib_listOffset;
    attrib_list_base = (EGLint *) attrib_list_ref->GetPayload();
    attrib_list = attrib_list_base + attrib_listOffset;
    attrib_list_sentinel = FALSE;
    for (int i = _attrib_listRemaining - 1; i >= 0; i--)  {
        if (attrib_list[i] == EGL_NONE){
            attrib_list_sentinel = TRUE;
            break;
        }
    }
    if (attrib_list_sentinel == FALSE) {
        LOGD("EglChooseConfig: attrib_list must contain EGL_NONE!")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    if (!configs_ref) {
        LOGD("EglChooseConfig: configs == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (configsOffset < 0) {
        LOGD("EglChooseConfig: configsOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _configsRemaining = configs_ref->GetLength() - configsOffset;
    if (_configsRemaining < config_size) {
        LOGD("EglChooseConfig: length - configsOffset < config_size < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    configs = new EGLConfig[_configsRemaining];

    if (!num_config_ref) {
        LOGD("EglChooseConfig: num_config == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (num_configOffset < 0) {
        LOGD("EglChooseConfig: num_configOffset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _num_configRemaining = num_config_ref->GetLength() - num_configOffset;
    if (_num_configRemaining < 1) {
        LOGD("EglChooseConfig: length - num_configOffset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    num_config_base = (EGLint *) num_config_ref->GetPayload();
    num_config = num_config_base + num_configOffset;

    _returnValue = eglChooseConfig(
        (EGLDisplay)dpy_native,
        (EGLint *)attrib_list,
        (EGLConfig *)configs,
        (EGLint)config_size,
        (EGLint *)num_config
    );
    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglGetConfigAttrib(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Elastos::Droid::Opengl::IEGLConfig* config,
    /* [in] */ Int32 attribute,
    /* [in] */ ArrayOf<Int32>* value_ref,
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);
    EGLConfig config_native = (EGLConfig) FromEGLHandle(config);
    EGLint *value_base = (EGLint *) 0;
    Int32 _remaining;
    EGLint *value = (EGLint *) 0;

    if (!value_ref) {
        LOGD("EglGetConfigAttrib: value == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("EglGetConfigAttrib: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = value_ref->GetLength() - offset;
    if (_remaining < 1) {
        LOGD("EglGetConfigAttrib: length - offset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    value_base = (EGLint *) value_ref->GetPayload();
    value = value_base + offset;

    _returnValue = eglGetConfigAttrib(
        (EGLDisplay)dpy_native,
        (EGLConfig)config_native,
        (EGLint)attribute,
        (EGLint *)value
    );

    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglCreateWindowSurface(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Elastos::Droid::Opengl::IEGLConfig* config,
    /* [in] */ IInterface* win,
    /* [in] */ ArrayOf<Int32>* attrib_list_ref,
    /* [in] */ Int32 offset,
    /* [out] */ Elastos::Droid::Opengl::IEGLSurface** surface)
{
        AutoPtr<ISurface> sur;
        if (ISurfaceView::Probe(win) != NULL) {
            AutoPtr<ISurfaceView> surfaceView = ISurfaceView::Probe(win);
            AutoPtr<ISurfaceHolder> holder;
            surfaceView->GetHolder((ISurfaceHolder**)&holder);
            holder->GetSurface((ISurface**)&sur);
        } else if (ISurfaceHolder::Probe(win) != NULL) {
            AutoPtr<ISurfaceHolder> holder = ISurfaceHolder::Probe(win);
            holder->GetSurface((ISurface**)&sur);
        } else if (ISurface::Probe(win) != NULL) {
            sur = ISurface::Probe(win);
        }

        if (sur != NULL) {
             _EglCreateWindowSurface(dpy, config, sur, attrib_list_ref, offset, surface);
        } else if (ISurfaceTexture::Probe(win)) {
            _EglCreateWindowSurfaceTexture(dpy, config,
                    win, attrib_list_ref, offset, surface);
        } else {
            return E_UNSUPPORTED_OPERATION_EXCEPTION;
        }

    return NOERROR;
}

ECode CEGL14::_EglCreateWindowSurface(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Elastos::Droid::Opengl::IEGLConfig* config,
    /* [in] */ IInterface* win,
    /* [in] */ ArrayOf<Int32>* attrib_list_ref,
    /* [in] */ Int32 offset,
    /* [out] */ Elastos::Droid::Opengl::IEGLSurface** surface)
{
    EGLSurface _returnValue = (EGLSurface) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);
    EGLConfig config_native = (EGLConfig) FromEGLHandle(config);
    Int32 attrib_list_sentinel = 0;
    EGLint *attrib_list_base = (EGLint *) 0;
    Int32 _remaining;
    EGLint *attrib_list = (EGLint *) 0;
    android::sp<ANativeWindow> window;

    if (!attrib_list_ref) {
        LOGD("_EglCreateWindowSurface: attrib_list == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("_EglCreateWindowSurface: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (win == NULL) {
not_valid_surface:
        LOGD("_EglCreateWindowSurface: Make sure the SurfaceView or associated SurfaceHolder has a valid Surface")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    window = SurfaceGetNativeWindow(win);

    if (window == NULL)
        goto not_valid_surface;

    _remaining = attrib_list_ref->GetLength() - offset;
    attrib_list_base = (EGLint *) attrib_list_ref->GetPayload();
    attrib_list = attrib_list_base + offset;
    attrib_list_sentinel = 0;
    for (Int32 i = _remaining - 1; i >= 0; i--)  {
        if (*((EGLint*)(attrib_list + i)) == EGL_NONE){
            attrib_list_sentinel = 1;
            break;
        }
    }
    if (attrib_list_sentinel == 0) {
        LOGD("_EglCreateWindowSurface: attrib_list must contain EGL_NONE!")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    _returnValue = eglCreateWindowSurface(
        (EGLDisplay)dpy_native,
        (EGLConfig)config_native,
        (EGLNativeWindowType)window.get(),
        (EGLint *)attrib_list
    );

    *surface = IEGLSurface::Probe(ToEGLHandle(EIID_IEGLSurface, _returnValue));
    return NOERROR;
}

ECode CEGL14::EglCreatePbufferSurface(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Elastos::Droid::Opengl::IEGLConfig* config,
    /* [in] */ ArrayOf<Int32>* attrib_list_ref,
    /* [in] */ Int32 offset,
    /* [out] */ Elastos::Droid::Opengl::IEGLSurface** surface)
{
    EGLSurface _returnValue = (EGLSurface) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);
    EGLConfig config_native = (EGLConfig) FromEGLHandle(config);
    Boolean attrib_list_sentinel = FALSE;
    EGLint *attrib_list_base = (EGLint *) 0;
    Int32 _remaining;
    EGLint *attrib_list = (EGLint *) 0;

    if (!attrib_list_ref) {
        LOGD("EglCreatePbufferSurface: attrib_list == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("EglCreatePbufferSurface: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = attrib_list_ref->GetLength() - offset;
    attrib_list_base = (EGLint *) attrib_list_ref->GetPayload();
    attrib_list = attrib_list_base + offset;
    attrib_list_sentinel = FALSE;
    for (Int32 i = _remaining - 1; i >= 0; i--)  {
        if (attrib_list[i] == EGL_NONE){
            attrib_list_sentinel = TRUE;
            break;
        }
    }
    if (attrib_list_sentinel == FALSE) {
        LOGD("EglCreatePbufferSurface: attrib_list must contain EGL_NONE!")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    _returnValue = eglCreatePbufferSurface(
        (EGLDisplay)dpy_native,
        (EGLConfig)config_native,
        (EGLint *)attrib_list
    );

    *surface = IEGLSurface::Probe(ToEGLHandle(EIID_IEGLSurface, _returnValue));
    return NOERROR;
}

ECode CEGL14::EglCreatePixmapSurface(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Elastos::Droid::Opengl::IEGLConfig* config,
    /* [in] */ Int32 pixmap,
    /* [in] */ ArrayOf<Int32>* attrib_list,
    /* [in] */ Int32 offset,
    /* [out] */ Elastos::Droid::Opengl::IEGLSurface** surface)
{
    *surface = IEGLSurface::Probe(ToEGLHandle(EIID_IEGLSurface, (EGLSurface) 0));
    LOGD("EglCreatePixmapSurface: Unsupported Operation")
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode CEGL14::EglDestroySurface(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Elastos::Droid::Opengl::IEGLSurface* surface,
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);
    EGLSurface surface_native = (EGLSurface) FromEGLHandle(surface);

    _returnValue = eglDestroySurface(
        (EGLDisplay)dpy_native,
        (EGLSurface)surface_native
    );
    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglQuerySurface(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Elastos::Droid::Opengl::IEGLSurface* surface,
    /* [in] */ Int32 attribute,
    /* [in] */ ArrayOf<Int32>* value_ref,
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);
    EGLSurface surface_native = (EGLSurface) FromEGLHandle(surface);
    EGLint *value_base = (EGLint *) 0;
    Int32 _remaining;
    EGLint *value = (EGLint *) 0;

    if (!value_ref) {
        LOGD("EglQuerySurface: value == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("EglQuerySurface: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = value_ref->GetLength() - offset;
    if (_remaining < 1) {
        LOGD("EglQuerySurface: length - offset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    value_base = (EGLint *) value_ref->GetPayload();
    value = value_base + offset;

    _returnValue = eglQuerySurface(
        (EGLDisplay)dpy_native,
        (EGLSurface)surface_native,
        (EGLint)attribute,
        (EGLint *)value
    );
    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglBindAPI(
    /* [in] */ Int32 api,
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    _returnValue = eglBindAPI(
        (EGLenum)api
    );
    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglQueryAPI(
    /* [out] */ Int32* value)
{
    EGLenum _returnValue = (EGLenum) 0;
    _returnValue = eglQueryAPI();
    *value = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglWaitClient(
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    _returnValue = eglWaitClient();
    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglReleaseThread(
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    _returnValue = eglReleaseThread();
    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglCreatePbufferFromClientBuffer(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Int32 buftype,
    /* [in] */ Int32 buffer,
    /* [in] */ Elastos::Droid::Opengl::IEGLConfig* config,
    /* [in] */ ArrayOf<Int32>* attrib_list_ref,
    /* [in] */ Int32 offset,
    /* [out] */ Elastos::Droid::Opengl::IEGLSurface** surface)
{
    EGLSurface _returnValue = (EGLSurface) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);
    EGLConfig config_native = (EGLConfig) FromEGLHandle(config);
    Boolean attrib_list_sentinel = FALSE;
    EGLint *attrib_list_base = (EGLint *) 0;
    Int32 _remaining;
    EGLint *attrib_list = (EGLint *) 0;

    if (!attrib_list_ref) {
        LOGD("EglCreatePbufferFromClientBuffer: attrib_list == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("EglCreatePbufferFromClientBuffer: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = attrib_list_ref->GetLength() - offset;
    attrib_list_base = (EGLint *) attrib_list_ref->GetPayload();
    attrib_list = attrib_list_base + offset;
    attrib_list_sentinel = FALSE;
    for (Int32 i = _remaining - 1; i >= 0; i--)  {
        if (attrib_list[i] == EGL_NONE){
            attrib_list_sentinel = TRUE;
            break;
        }
    }
    if (attrib_list_sentinel == FALSE) {
        LOGD("EglCreatePbufferFromClientBuffer: attrib_list must contain EGL_NONE!")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    _returnValue = eglCreatePbufferFromClientBuffer(
        (EGLDisplay)dpy_native,
        (EGLenum)buftype,
        (EGLClientBuffer)buffer,
        (EGLConfig)config_native,
        (EGLint *)attrib_list
    );
    *surface = IEGLSurface::Probe(ToEGLHandle(EIID_IEGLSurface, _returnValue));
    return NOERROR;
}

ECode CEGL14::EglSurfaceAttrib(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Elastos::Droid::Opengl::IEGLSurface* surface,
    /* [in] */ Int32 attribute,
    /* [in] */ Int32 value,
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);
    EGLSurface surface_native = (EGLSurface) FromEGLHandle(surface);

    _returnValue = eglSurfaceAttrib(
        (EGLDisplay)dpy_native,
        (EGLSurface)surface_native,
        (EGLint)attribute,
        (EGLint)value
    );
    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglBindTexImage(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Elastos::Droid::Opengl::IEGLSurface* surface,
    /* [in] */ Int32 buffer,
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);
    EGLSurface surface_native = (EGLSurface) FromEGLHandle(surface);

    _returnValue = eglBindTexImage(
        (EGLDisplay)dpy_native,
        (EGLSurface)surface_native,
        (EGLint)buffer
    );
    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglReleaseTexImage(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Elastos::Droid::Opengl::IEGLSurface* surface,
    /* [in] */ Int32 buffer,
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);
    EGLSurface surface_native = (EGLSurface) FromEGLHandle(surface);

    _returnValue = eglReleaseTexImage(
        (EGLDisplay)dpy_native,
        (EGLSurface)surface_native,
        (EGLint)buffer
    );
    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglSwapInterval(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Int32 interval,
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);

    _returnValue = eglSwapInterval(
        (EGLDisplay)dpy_native,
        (EGLint)interval
    );
    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglCreateContext(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Elastos::Droid::Opengl::IEGLConfig* config,
    /* [in] */ Elastos::Droid::Opengl::IEGLContext* share_context,
    /* [in] */ ArrayOf<Int32>* attrib_list_ref,
    /* [in] */ Int32 offset,
    /* [out] */ Elastos::Droid::Opengl::IEGLContext** context)
{
    EGLContext _returnValue = (EGLContext) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);
    EGLConfig config_native = (EGLConfig) FromEGLHandle(config);
    EGLContext share_context_native = (EGLContext) FromEGLHandle(share_context);
    bool attrib_list_sentinel = FALSE;
    EGLint *attrib_list_base = (EGLint *) 0;
    Int32 _remaining;
    EGLint *attrib_list = (EGLint *) 0;

    if (!attrib_list_ref) {
        LOGD("EglCreateContext: attrib_list == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("EglCreateContext: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = attrib_list_ref->GetLength() - offset;
    attrib_list_base = (EGLint *) attrib_list_ref->GetPayload();
    attrib_list = attrib_list_base + offset;
    attrib_list_sentinel = FALSE;
    for (Int32 i = _remaining - 1; i >= 0; i--)  {
        if (attrib_list[i] == EGL_NONE){
            attrib_list_sentinel = TRUE;
            break;
        }
    }
    if (attrib_list_sentinel == FALSE) {
        LOGD("EglCreateContext: attrib_list must contain EGL_NONE!")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    _returnValue = eglCreateContext(
        (EGLDisplay)dpy_native,
        (EGLConfig)config_native,
        (EGLContext)share_context_native,
        (EGLint *)attrib_list
    );
    *context = IEGLContext::Probe(ToEGLHandle(EIID_IEGLContext, _returnValue));
    return NOERROR;
}

ECode CEGL14::EglDestroyContext(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Elastos::Droid::Opengl::IEGLContext* ctx,
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);
    EGLContext ctx_native = (EGLContext) FromEGLHandle(ctx);

    _returnValue = eglDestroyContext(
        (EGLDisplay)dpy_native,
        (EGLContext)ctx_native
    );
    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglMakeCurrent(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Elastos::Droid::Opengl::IEGLSurface* draw,
    /* [in] */ Elastos::Droid::Opengl::IEGLSurface* read,
    /* [in] */ Elastos::Droid::Opengl::IEGLContext* ctx,
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);
    EGLSurface draw_native = (EGLSurface) FromEGLHandle(draw);
    EGLSurface read_native = (EGLSurface) FromEGLHandle(read);
    EGLContext ctx_native = (EGLContext) FromEGLHandle(ctx);

    _returnValue = eglMakeCurrent(
        (EGLDisplay)dpy_native,
        (EGLSurface)draw_native,
        (EGLSurface)read_native,
        (EGLContext)ctx_native
    );
    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglGetCurrentContext(
    /* [out] */ Elastos::Droid::Opengl::IEGLContext** context)
{
    EGLContext _returnValue = (EGLContext) 0;
    _returnValue = eglGetCurrentContext();
    *context = IEGLContext::Probe(ToEGLHandle(EIID_IEGLContext, _returnValue));
    return NOERROR;
}

ECode CEGL14::EglGetCurrentSurface(
    /* [in] */ Int32 readdraw,
    /* [out] */ Elastos::Droid::Opengl::IEGLSurface** surface)
{
    EGLSurface _returnValue = (EGLSurface) 0;
    _returnValue = eglGetCurrentSurface(
        (EGLint)readdraw
    );
    *surface = IEGLSurface::Probe(ToEGLHandle(EIID_IEGLSurface, _returnValue));
    return NOERROR;
}

ECode CEGL14::EglGetCurrentDisplay(
    /* [out] */ Elastos::Droid::Opengl::IEGLDisplay** display)
{
    EGLDisplay _returnValue = (EGLDisplay) 0;
    _returnValue = eglGetCurrentDisplay();
    *display = IEGLDisplay::Probe(ToEGLHandle(EIID_IEGLDisplay, _returnValue));
    return NOERROR;
}

ECode CEGL14::EglQueryContext(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Elastos::Droid::Opengl::IEGLContext* ctx,
    /* [in] */ Int32 attribute,
    /* [in] */ ArrayOf<Int32>* value_ref,
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);
    EGLContext ctx_native = (EGLContext) FromEGLHandle(ctx);
    EGLint *value_base = (EGLint *) 0;
    Int32 _remaining;
    EGLint *value = (EGLint *) 0;

    if (!value_ref) {
        LOGD("EglQueryContext: value == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("EglQueryContext: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = value_ref->GetLength() - offset;
    if (_remaining < 1) {
        LOGD("EglQueryContext: length - offset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    value_base = (EGLint *) value_ref->GetPayload();
    value = value_base + offset;

    _returnValue = eglQueryContext(
        (EGLDisplay)dpy_native,
        (EGLContext)ctx_native,
        (EGLint)attribute,
        (EGLint *)value
    );
    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglWaitGL(
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    _returnValue = eglWaitGL();
    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglWaitNative(
    /* [in] */ Int32 engine,
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    _returnValue = eglWaitNative(
        (EGLint)engine
    );
    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglSwapBuffers(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Elastos::Droid::Opengl::IEGLSurface* surface,
    /* [out] */ Boolean* result)
{
    EGLBoolean _returnValue = (EGLBoolean) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);
    EGLSurface surface_native = (EGLSurface) FromEGLHandle(surface);

    _returnValue = eglSwapBuffers(
        (EGLDisplay)dpy_native,
        (EGLSurface)surface_native
    );
    *result = _returnValue;
    return NOERROR;
}

ECode CEGL14::EglCopyBuffers(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Elastos::Droid::Opengl::IEGLSurface* surface,
    /* [in] */ Int32 target,
    /* [out] */ Boolean* result)
{
    *result = (EGLBoolean)0;
    LOGD("EglCopyBuffers: Unsupported Operation")
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

AutoPtr<IInterface> CEGL14::ToEGLHandle(
    /* [in] */ REIID riid,
    /* [in] */ void* handle)
{
    if (riid == EIID_IEGLContext && (EGLContext)handle == EGL_NO_CONTEXT) {
        return eglNoContextObject;
    } else if (riid == EIID_IEGLContext) {
        return EGLContextC::CreateInstance((Int32)handle);
    }

    if (riid == EIID_IEGLDisplay && (EGLDisplay)handle == EGL_NO_DISPLAY) {
        return eglNoDisplayObject;
    } else if (riid == EIID_IEGLDisplay) {
        return EGLDisplayC::CreateInstance((Int32)handle);
    }

    if (riid == EIID_IEGLSurface && (EGLSurface)handle == EGL_NO_SURFACE) {
           return eglNoSurfaceObject;
    } else if (riid == EIID_IEGLSurface) {
        return EGLSurfaceC::CreateInstance((Int32)handle);
    }

    if (riid == EIID_IEGLConfig)
        return EGLConfigC::CreateInstance((Int32)handle);

    return NULL;
}

Int32 CEGL14::FromEGLHandle(
    /* [in] */ IEGLObjectHandle* obj)
{
    if (obj == NULL) {
        LOGD("FromEGLHandle: Object is set to null.")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 handle;
    obj->GetHandle(&handle);
    return handle;
}

android::sp<ANativeWindow> CEGL14::SurfaceGetNativeWindow(
    /* [in] */ IInterface* surfaceObj)
{
    AutoPtr<ISurface> surface = ISurface::Probe(surfaceObj);
    if (surface == NULL)
        return NULL;
    Handle32 tmp;
    surface->GetSurface(&tmp);
    android::sp<android::Surface> result =reinterpret_cast<android::Surface*>(tmp);
    if (result == NULL) {
        /*
         * if this method is called from the WindowManager's process, it means
         * the client is is not remote, and therefore is allowed to have
         * a Surface (data), so we create it here.
         * If we don't have a SurfaceControl, it means we're in a different
         * process.
         */
        surface->GetNativeSurfaceControl(&tmp);
        android::SurfaceControl* const control = reinterpret_cast<android::SurfaceControl*>(tmp);
        if (control) {
            result = control->getSurface();
            if (result != NULL) {
                result->incStrong(surfaceObj);
                surface->SetSurface(reinterpret_cast<Handle32>(result.get()));
            }
        }
    }

    return NULL;
}

ECode CEGL14::_EglCreateWindowSurfaceTexture(
    /* [in] */ Elastos::Droid::Opengl::IEGLDisplay* dpy,
    /* [in] */ Elastos::Droid::Opengl::IEGLConfig* config,
    /* [in] */ IInterface* win,
    /* [in] */ ArrayOf<Int32>* attrib_list_ref,
    /* [in] */ Int32 offset,
    /* [out] */ Elastos::Droid::Opengl::IEGLSurface** surface)
{
    EGLSurface _returnValue = (EGLSurface) 0;
    EGLDisplay dpy_native = (EGLDisplay) FromEGLHandle(dpy);
    EGLConfig config_native = (EGLConfig) FromEGLHandle(config);
    Int32 attrib_list_sentinel = 0;
    EGLint *attrib_list_base = (EGLint *) 0;
    Int32 _remaining;
    EGLint *attrib_list = (EGLint *) 0;
    android::sp<ANativeWindow> window;
    android::sp<android::SurfaceTexture> surfaceTexture;

    if (!attrib_list_ref) {
        LOGD("_EglCreateWindowSurfaceTexture: attrib_list == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("_EglCreateWindowSurfaceTexture: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (win == NULL) {
not_valid_surface:
        LOGD("_EglCreateWindowSurfaceTexture: Make sure the SurfaceView or associated SurfaceHolder has a valid Surface")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Int32 surTexture;
    AutoPtr<ISurfaceTexture> st = ISurfaceTexture::Probe(win);
    st->GetSurfaceTexture(&surTexture);
    surfaceTexture = reinterpret_cast<android::SurfaceTexture*>(surTexture);
    window = new android::SurfaceTextureClient(surfaceTexture);

    if (window == NULL)
        goto not_valid_surface;

    _remaining = attrib_list_ref->GetLength() - offset;
    attrib_list_base = (EGLint *) attrib_list_ref->GetPayload();
    attrib_list = attrib_list_base + offset;
    attrib_list_sentinel = 0;
    for (Int32 i = _remaining - 1; i >= 0; i--)  {
        if (*((EGLint*)(attrib_list + i)) == EGL_NONE){
            attrib_list_sentinel = 1;
            break;
        }
    }
    if (attrib_list_sentinel == 0) {
        LOGD("_EglCreateWindowSurfaceTexture: attrib_list must contain EGL_NONE!")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    _returnValue = eglCreateWindowSurface(
        (EGLDisplay)dpy_native,
        (EGLConfig)config_native,
        (EGLNativeWindowType)window.get(),
        (EGLint *)attrib_list
    );

    *surface = IEGLSurface::Probe(ToEGLHandle(EIID_IEGLSurface, _returnValue));
    return NOERROR;
}


} // namespace Opengl
} // namespace Droid
} // namespace Elastos

