
#include "CGLES11.h"
#include "CGLES10.h"
#include <GLES/gl.h>
#include <GLES/glext.h>
#include <cmdef.h>
#include <elastos/utility/logging/Slogger.h>

#define LOGD(msg) SLOGGERD("CGLES11", msg)

using Elastos::IO::CNIOAccessHelper;
using Elastos::IO::INIOAccessHelper;

extern "C" {
GL_API void GL_APIENTRY glPointSizePointerOESBounds(GLenum type, GLsizei stride,
        const GLvoid *ptr, GLsizei count);
}

namespace Elastos {
namespace Droid {
namespace Opengl {

static AutoPtr<IGLES10> InitGl10()
{
    AutoPtr<IGLES10> gl10;
    CGLES10::AcquireSingleton((IGLES10**)&gl10);
    return gl10;
}

AutoPtr<IGLES10> CGLES11::sGl10 = InitGl10();
AutoPtr<IBuffer> CGLES11::sPointSizePointerOES = NULL;

ECode CGLES11::GlActiveTexture(
    /* [in] */ Int32 texture)
{
    return sGl10->GlActiveTexture(texture);
}

ECode CGLES11::GlAlphaFunc(
    /* [in] */ Int32 func,
    /* [in] */ Float ref)
{
    return sGl10->GlAlphaFunc(func, ref);
}

ECode CGLES11::GlAlphaFuncx(
    /* [in] */ Int32 func,
    /* [in] */ Int32 ref)
{
    return sGl10->GlAlphaFuncx(func, ref);
}

ECode CGLES11::GlBindTexture(
    /* [in] */ Int32 target,
    /* [in] */ Int32 texture)
{
    return sGl10->GlBindTexture(target, texture);
}

ECode CGLES11::GlBlendFunc(
    /* [in] */ Int32 sfactor,
    /* [in] */ Int32 dfactor)
{
    return sGl10->GlBlendFunc(sfactor, dfactor);
}

ECode CGLES11::GlClear(
    /* [in] */ Int32 mask)
{
    return sGl10->GlClear(mask);
}

ECode CGLES11::GlClearColor(
    /* [in] */ Float red,
    /* [in] */ Float green,
    /* [in] */ Float blue,
    /* [in] */ Float alpha)
{
    return sGl10->GlClearColor(red, green, blue, alpha);
}

ECode CGLES11::GlClearColorx(
    /* [in] */ Int32 red,
    /* [in] */ Int32 green,
    /* [in] */ Int32 blue,
    /* [in] */ Int32 alpha)
{
    return sGl10->GlClearColorx(red, green, blue, alpha);
}

ECode CGLES11::GlClearDepthf(
    /* [in] */ Float depth)
{
    return sGl10->GlClearDepthf(depth);
}

ECode CGLES11::GlClearDepthx(
    /* [in] */ Int32 depth)
{
    return sGl10->GlClearDepthx(depth);
}

ECode CGLES11::GlClearStencil(
    /* [in] */ Int32 s)
{
    return sGl10->GlClearStencil(s);
}

ECode CGLES11::GlClientActiveTexture(
    /* [in] */ Int32 texture)
{
    return sGl10->GlClientActiveTexture(texture);
}

ECode CGLES11::GlColor4f(
    /* [in] */ Float red,
    /* [in] */ Float green,
    /* [in] */ Float blue,
    /* [in] */ Float alpha)
{
    return sGl10->GlColor4f(red, green, blue, alpha);
}

ECode CGLES11::GlColor4x(
    /* [in] */ Int32 red,
    /* [in] */ Int32 green,
    /* [in] */ Int32 blue,
    /* [in] */ Int32 alpha)
{
    return sGl10->GlColor4x(red, green, blue, alpha);
}

ECode CGLES11::GlColorMask(
    /* [in] */ Boolean red,
    /* [in] */ Boolean green,
    /* [in] */ Boolean blue,
    /* [in] */ Boolean alpha)
{
    return sGl10->GlColorMask(red, green, blue, alpha);
}

ECode CGLES11::GlColorPointer(
    /* [in] */ Int32 size,
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ Elastos::IO::IBuffer* pointer)
{
    return sGl10->GlColorPointer(size, type, stride, pointer);
}

ECode CGLES11::GlCompressedTexImage2D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 border,
    /* [in] */ Int32 imageSize,
    /* [in] */ Elastos::IO::IBuffer* data_buf)
{
    return sGl10->GlCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, data_buf);
}

ECode CGLES11::GlCompressedTexSubImage2D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 xoffset,
    /* [in] */ Int32 yoffset,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 format,
    /* [in] */ Int32 imageSize,
    /* [in] */ Elastos::IO::IBuffer* data_buf)
{
    return sGl10->GlCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, data_buf);
}

ECode CGLES11::GlCopyTexImage2D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 border)
{
    return sGl10->GlCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
}

ECode CGLES11::GlCopyTexSubImage2D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 xoffset,
    /* [in] */ Int32 yoffset,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return sGl10->GlCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

ECode CGLES11::GlCullFace(
    /* [in] */ Int32 mode)
{
    return sGl10->GlCullFace(mode);
}

ECode CGLES11::GlDeleteTextures(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* textures_buf,
    /* [in] */ Int32 offset)
{
    return sGl10->GlDeleteTextures(n,  textures_buf, offset);
}

ECode CGLES11::GlDeleteTextures(
    /* [in] */ Int32 n,
    /* [in] */ Elastos::IO::IInt32Buffer* textures_buf)
{
    return sGl10->GlDeleteTextures(n, textures_buf);
}

ECode CGLES11::GlDepthFunc(
    /* [in] */ Int32 func)
{
    return sGl10->GlDepthFunc(func);
}

ECode CGLES11::GlDepthMask(
    /* [in] */ Boolean flag)
{
    return sGl10->GlDepthMask(flag);
}

ECode CGLES11::GlDepthRangef(
    /* [in] */ Float zNear,
    /* [in] */ Float zFar)
{
    return sGl10->GlDepthRangef(zNear, zFar);
}

ECode CGLES11::GlDepthRangex(
    /* [in] */ Int32 zNear,
    /* [in] */ Int32 zFar)
{
    return sGl10->GlDepthRangex(zNear, zFar);
}

ECode CGLES11::GlDisable(
    /* [in] */ Int32 cap)
{
    return sGl10->GlDisable(cap);
}

ECode CGLES11::GlDisableClientState(
    /* [in] */ Int32 array)
{
    return sGl10->GlDisableClientState(array);
}

ECode CGLES11::GlDrawArrays(
    /* [in] */ Int32 mode,
    /* [in] */ Int32 first,
    /* [in] */ Int32 count)
{
    return sGl10->GlDrawArrays(mode, first, count);
}

ECode CGLES11::GlDrawElements(
    /* [in] */ Int32 mode,
    /* [in] */ Int32 count,
    /* [in] */ Int32 type,
    /* [in] */ Elastos::IO::IBuffer* indices_buf)
{
    return sGl10->GlDrawElements(mode, count, type, indices_buf);
}

ECode CGLES11::GlEnable(
    /* [in] */ Int32 cap)
{
    return sGl10->GlEnable(cap);
}

ECode CGLES11::GlEnableClientState(
    /* [in] */ Int32 array)
{
    return sGl10->GlEnableClientState(array);
}

ECode CGLES11::GlFinish()
{
    return sGl10->GlFinish();
}

ECode CGLES11::GlFlush()
{
    return sGl10->GlFlush();
}

ECode CGLES11::GlFogf(
    /* [in] */ Int32 pname,
    /* [in] */ Float param)
{
    return sGl10->GlFogf(pname, param);
}

ECode CGLES11::GlFogfv(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    return sGl10->GlFogfv(pname, params_ref, offset);
}

ECode CGLES11::GlFogfv(
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IFloatBuffer* params_buf)
{
    return sGl10->GlFogfv(pname, params_buf);
}

ECode CGLES11::GlFogx(
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    return sGl10->GlFogx(pname, param);
}

ECode CGLES11::GlFogxv(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    return sGl10->GlFogxv(pname, params_ref, offset);
}

ECode CGLES11::GlFogxv(
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    return sGl10->GlFogxv(pname, params_buf);
}

ECode CGLES11::GlFrontFace(
    /* [in] */ Int32 mode)
{
    return sGl10->GlFrontFace(mode);
}

ECode CGLES11::GlFrustumf(
    /* [in] */ Float left,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Float top,
    /* [in] */ Float zNear,
    /* [in] */ Float zFar)
{
    return sGl10->GlFrustumf(left, right, bottom, top, zNear, zFar);
}

ECode CGLES11::GlFrustumx(
    /* [in] */ Int32 left,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [in] */ Int32 top,
    /* [in] */ Int32 zNear,
    /* [in] */ Int32 zFar)
{
    return sGl10->GlFrustumx(left, right, bottom, top, zNear, zFar);
}

ECode CGLES11::GlGenTextures(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* textures_ref,
    /* [in] */ Int32 offset)
{
    return sGl10->GlGenTextures(n, textures_ref, offset);
}

ECode CGLES11::GlGenTextures(
    /* [in] */ Int32 n,
    /* [in] */ Elastos::IO::IInt32Buffer* textures_buf)
{
    return sGl10->GlGenTextures(n, textures_buf);
}

ECode CGLES11::GlGetError(
    /* [out] */ Int32* error)
{
    return sGl10->GlGetError(error);
}

ECode CGLES11::GlGetIntegerv(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    return sGl10->GlGetIntegerv(pname, params_ref, offset);
}

ECode CGLES11::GlGetIntegerv(
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    return sGl10->GlGetIntegerv(pname, params_buf);
}

ECode CGLES11::GlGetString(
    /* [in] */ Int32 name,
    /* [out] */ String* str)
{
    return sGl10->GlGetString(name, str);
}

ECode CGLES11::GlHint(
    /* [in] */ Int32 target,
    /* [in] */ Int32 mode)
{
    return sGl10->GlHint(target, mode);
}

ECode CGLES11::GlLightModelf(
    /* [in] */ Int32 pname,
    /* [in] */ Float param)
{
    return sGl10->GlLightModelf(pname, param);
}

ECode CGLES11::GlLightModelfv(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    return sGl10->GlLightModelfv(pname, params_ref, offset);
}

ECode CGLES11::GlLightModelfv(
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IFloatBuffer* params_buf)
{
    return sGl10->GlLightModelfv(pname, params_buf);
}

ECode CGLES11::GlLightModelx(
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    return sGl10->GlLightModelx(pname, param);
}

ECode CGLES11::GlLightModelxv(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    return sGl10->GlLightModelxv(pname, params_ref, offset);
}

ECode CGLES11::GlLightModelxv(
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    return sGl10->GlLightModelxv(pname, params_buf);
}

ECode CGLES11::GlLightf(
    /* [in] */ Int32 light,
    /* [in] */ Int32 pname,
    /* [in] */ Float param)
{
    return sGl10->GlLightf(light, pname, param);
}

ECode CGLES11::GlLightfv(
    /* [in] */ Int32 light,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    return sGl10->GlLightfv(light, pname, params_ref, offset);
}

ECode CGLES11::GlLightfv(
    /* [in] */ Int32 light,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IFloatBuffer* params_buf)
{
    return sGl10->GlLightfv(light, pname, params_buf);
}

ECode CGLES11::GlLightx(
    /* [in] */ Int32 light,
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    return sGl10->GlLightx(light, pname, param);
}

ECode CGLES11::GlLightxv(
    /* [in] */ Int32 light,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    return sGl10->GlLightxv(light, pname, params_ref, offset);
}

ECode CGLES11::GlLightxv(
    /* [in] */ Int32 light,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    return sGl10->GlLightxv(light, pname, params_buf);
}

ECode CGLES11::GlLineWidth(
    /* [in] */ Float width)
{
    return sGl10->GlLineWidth(width);
}

ECode CGLES11::GlLineWidthx(
    /* [in] */ Int32 width)
{
    return sGl10->GlLineWidthx(width);
}

ECode CGLES11::GlLoadIdentity()
{
    return sGl10->GlLoadIdentity();
}

ECode CGLES11::GlLoadMatrixf(
    /* [in] */ ArrayOf<Float>* m,
    /* [in] */ Int32 offset)
{
    return sGl10->GlLoadMatrixf(m, offset);
}

ECode CGLES11::GlLoadMatrixf(
    /* [in] */ Elastos::IO::IFloatBuffer* m)
{
    return sGl10->GlLoadMatrixf(m);
}

ECode CGLES11::GlLoadMatrixx(
    /* [in] */ ArrayOf<Int32>* m,
    /* [in] */ Int32 offset)
{
    return sGl10->GlLoadMatrixx(m, offset);
}

ECode CGLES11::GlLoadMatrixx(
    /* [in] */ Elastos::IO::IInt32Buffer* m)
{
    return sGl10->GlLoadMatrixx(m);
}

ECode CGLES11::GlLogicOp(
    /* [in] */ Int32 opcode)
{
    return sGl10->GlLogicOp(opcode);
}

ECode CGLES11::GlMaterialf(
    /* [in] */ Int32 face,
    /* [in] */ Int32 pname,
    /* [in] */ Float param)
{
    return sGl10->GlMaterialf(face, pname, param);
}

ECode CGLES11::GlMaterialfv(
    /* [in] */ Int32 face,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    return sGl10->GlMaterialfv(face, pname, params_ref, offset);
}

ECode CGLES11::GlMaterialfv(
    /* [in] */ Int32 face,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IFloatBuffer* params_buf)
{
    return sGl10->GlMaterialfv(face, pname, params_buf);
}

ECode CGLES11::GlMaterialx(
    /* [in] */ Int32 face,
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    return sGl10->GlMaterialx(face, pname, param);
}

ECode CGLES11::GlMaterialxv(
    /* [in] */ Int32 face,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    return sGl10->GlMaterialxv(face, pname, params_ref, offset);
}

ECode CGLES11::GlMaterialxv(
    /* [in] */ Int32 face,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    return sGl10->GlMaterialxv(face, pname, params_buf);
}

ECode CGLES11::GlMatrixMode(
    /* [in] */ Int32 mode)
{
    return sGl10->GlMatrixMode(mode);
}

ECode CGLES11::GlMultMatrixf(
    /* [in] */ ArrayOf<Float>* m,
    /* [in] */ Int32 offset)
{
    return sGl10->GlMultMatrixf(m, offset);
}

ECode CGLES11::GlMultMatrixf(
    /* [in] */ Elastos::IO::IFloatBuffer* m)
{
    return sGl10->GlMultMatrixf(m);
}

ECode CGLES11::GlMultMatrixx(
    /* [in] */ ArrayOf<Int32>* m,
    /* [in] */ Int32 offset)
{
    return sGl10->GlMultMatrixx(m, offset);
}

ECode CGLES11::GlMultMatrixx(
    /* [in] */ Elastos::IO::IInt32Buffer* m)
{
    return sGl10->GlMultMatrixx(m);
}

ECode CGLES11::GlMultiTexCoord4f(
    /* [in] */ Int32 target,
    /* [in] */ Float s,
    /* [in] */ Float t,
    /* [in] */ Float r,
    /* [in] */ Float q)
{
    return sGl10->GlMultiTexCoord4f(target, s, t, r, q);
}

ECode CGLES11::GlMultiTexCoord4x(
    /* [in] */ Int32 target,
    /* [in] */ Int32 s,
    /* [in] */ Int32 t,
    /* [in] */ Int32 r,
    /* [in] */ Int32 q)
{
    return sGl10->GlMultiTexCoord4x(target, s, t, r, q);
}

ECode CGLES11::GlNormal3f(
    /* [in] */ Float nx,
    /* [in] */ Float ny,
    /* [in] */ Float nz)
{
    return sGl10->GlNormal3f(nx, ny, nz);
}

ECode CGLES11::GlNormal3x(
    /* [in] */ Int32 nx,
    /* [in] */ Int32 ny,
    /* [in] */ Int32 nz)
{
    return sGl10->GlNormal3x(nx, ny, nz);
}

ECode CGLES11::GlNormalPointer(
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ Elastos::IO::IBuffer* pointer)
{
    return sGl10->GlNormalPointer(type, stride, pointer);
}

ECode CGLES11::GlOrthof(
    /* [in] */ Float left,
    /* [in] */ Float right,
    /* [in] */ Float bottom,
    /* [in] */ Float top,
    /* [in] */ Float zNear,
    /* [in] */ Float zFar)
{
    return sGl10->GlOrthof(left, right, bottom, top, zNear, zFar);
}

ECode CGLES11::GlOrthox(
    /* [in] */ Int32 left,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [in] */ Int32 top,
    /* [in] */ Int32 zNear,
    /* [in] */ Int32 zFar)
{
    return sGl10->GlOrthox(left, right, bottom, top, zNear, zFar);
}

ECode CGLES11::GlPixelStorei(
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    return sGl10->GlPixelStorei(pname, param);
}

ECode CGLES11::GlPointSize(
    /* [in] */ Float size)
{
    return sGl10->GlPointSize(size);
}

ECode CGLES11::GlPointSizex(
    /* [in] */ Int32 size)
{
    return sGl10->GlPointSizex(size);
}

ECode CGLES11::GlPolygonOffset(
    /* [in] */ Float factor,
    /* [in] */ Float units)
{
    return sGl10->GlPolygonOffset(factor, units);
}

ECode CGLES11::GlPolygonOffsetx(
    /* [in] */ Int32 factor,
    /* [in] */ Int32 units)
{
    return sGl10->GlPolygonOffsetx(factor, units);
}

ECode CGLES11::GlPopMatrix()
{
    return sGl10->GlPopMatrix();
}

ECode CGLES11::GlPushMatrix()
{
    return sGl10->GlPushMatrix();
}

ECode CGLES11::GlReadPixels(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 format,
    /* [in] */ Int32 type,
    /* [in] */ Elastos::IO::IBuffer* pixels)
{
    return sGl10->GlReadPixels(x, y, width, height, format, type, pixels);
}

ECode CGLES11::GlRotatef(
    /* [in] */ Float angle,
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float z)
{
    return sGl10->GlRotatef(angle, x, y, z);
}

ECode CGLES11::GlRotatex(
    /* [in] */ Int32 angle,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z)
{
    return sGl10->GlRotatex(angle, x, y, z);
}

ECode CGLES11::GlSampleCoverage(
    /* [in] */ Float value,
    /* [in] */ Boolean invert)
{
    return sGl10->GlSampleCoverage(value, invert);
}

ECode CGLES11::GlSampleCoveragex(
    /* [in] */ Int32 value,
    /* [in] */ Boolean invert)
{
    return sGl10->GlSampleCoveragex(value, invert);
}

ECode CGLES11::GlScalef(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float z)
{
    return sGl10->GlScalef(x, y, z);
}

ECode CGLES11::GlScalex(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z)
{
    return sGl10->GlScalex(x, y, z);
}

ECode CGLES11::GlScissor(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return sGl10->GlScissor(x, y, width, height);
}

ECode CGLES11::GlShadeModel(
    /* [in] */ Int32 mode)
{
    return sGl10->GlShadeModel(mode);
}

ECode CGLES11::GlStencilFunc(
    /* [in] */ Int32 func,
    /* [in] */ Int32 ref,
    /* [in] */ Int32 mask)
{
    return sGl10->GlStencilFunc(func, ref, mask);
}

ECode CGLES11::GlStencilMask(
    /* [in] */ Int32 mask)
{
    return sGl10->GlStencilMask(mask);
}

ECode CGLES11::GlStencilOp(
    /* [in] */ Int32 fail,
    /* [in] */ Int32 zfail,
    /* [in] */ Int32 zpass)
{
    return sGl10->GlStencilOp(fail, zfail, zpass);
}

ECode CGLES11::GlTexCoordPointer(
    /* [in] */ Int32 size,
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ Elastos::IO::IBuffer* pointer)
{
    return sGl10->GlTexCoordPointer(size, type, stride, pointer);
}

ECode CGLES11::GlTexEnvf(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Float param)
{
    return sGl10->GlTexEnvf(target, pname, param);
}

ECode CGLES11::GlTexEnvfv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    return sGl10->GlTexEnvfv(target, pname, params_ref, offset);
}

ECode CGLES11::GlTexEnvfv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IFloatBuffer* params_buf)
{
    return sGl10->GlTexEnvfv(target, pname, params_buf);
}

ECode CGLES11::GlTexEnvx(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    return sGl10->GlTexEnvx(target, pname, param);
}

ECode CGLES11::GlTexEnvxv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    return sGl10->GlTexEnvxv(target, pname, params_ref, offset);
}

ECode CGLES11::GlTexEnvxv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    return sGl10->GlTexEnvxv(target, pname, params_buf);
}

ECode CGLES11::GlTexImage2D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 internalformat,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 border,
    /* [in] */ Int32 format,
    /* [in] */ Int32 type,
    /* [in] */ Elastos::IO::IBuffer* pixels)
{
    return sGl10->GlTexImage2D(target, level, internalformat, width, height, border, format, type, pixels);
}

ECode CGLES11::GlTexParameterf(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Float param)
{
    return sGl10->GlTexParameterf(target, pname, param);
}

ECode CGLES11::GlTexParameterx(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    return sGl10->GlTexParameterx(target, pname, param);
}

ECode CGLES11::GlTexSubImage2D(
    /* [in] */ Int32 target,
    /* [in] */ Int32 level,
    /* [in] */ Int32 xoffset,
    /* [in] */ Int32 yoffset,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [in] */ Int32 format,
    /* [in] */ Int32 type,
    /* [in] */ Elastos::IO::IBuffer* pixels)
{
    return sGl10->GlTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, pixels);
}

ECode CGLES11::GlTranslatef(
    /* [in] */ Float x,
    /* [in] */ Float y,
    /* [in] */ Float z)
{
    return sGl10->GlTranslatef(x, y, z);
}

ECode CGLES11::GlTranslatex(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 z)
{
    return sGl10->GlTranslatex(x, y, z);
}

ECode CGLES11::GlVertexPointer(
    /* [in] */ Int32 size,
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ Elastos::IO::IBuffer* pointer)
{
    return sGl10->GlVertexPointer(size, type, stride, pointer);
}

ECode CGLES11::GlViewport(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height)
{
    return sGl10->GlViewport(x, y, width, height);
}

ECode CGLES11::GlBindBuffer(
    /* [in] */ Int32 target,
    /* [in] */ Int32 buffer)
{
    glBindBuffer(
        (GLenum)target,
        (GLuint)buffer
    );
    return NOERROR;
}

ECode CGLES11::GlBufferData(
    /* [in] */ Int32 target,
    /* [in] */ Int32 size,
    /* [in] */ Elastos::IO::IBuffer* data_buf,
    /* [in] */ Int32 usage)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLvoid *data = (GLvoid *) 0;
    Handle32 dataTmp;

    if (data_buf) {
        FAIL_RETURN(GetPointer(data_buf, &_array, &_remaining, &_bufferOffset, &dataTmp));
        data = (GLvoid *) dataTmp;
        if (_remaining < size) {
            LOGD("remaining() < size < needed")
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }
    if (data_buf && data == NULL) {
        char * _dataBase = reinterpret_cast<char *>(_array);
        data = (GLvoid *) (_dataBase + _bufferOffset);
    }
    glBufferData(
        (GLenum)target,
        (GLsizeiptr)size,
        (GLvoid *)data,
        (GLenum)usage
    );
    return NOERROR;
}

ECode CGLES11::GlBufferSubData(
    /* [in] */ Int32 target,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 size,
    /* [in] */ Elastos::IO::IBuffer* data_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLvoid *data = (GLvoid *) 0;
    Handle32 dataTmp;

    FAIL_RETURN(GetPointer(data_buf, &_array, &_remaining, &_bufferOffset, &dataTmp));
    data = (GLvoid *) dataTmp;
    if (_remaining < size) {
        LOGD("GlBufferSubData: remaining() < size < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (data == NULL) {
        char * _dataBase = reinterpret_cast<char *>(_array);
        data = (GLvoid *) (_dataBase + _bufferOffset);
    }
    glBufferSubData(
        (GLenum)target,
        (GLintptr)offset,
        (GLsizeiptr)size,
        (GLvoid *)data
    );

    return NOERROR;
}

ECode CGLES11::GlClipPlanef(
    /* [in] */ Int32 plane,
    /* [in] */ ArrayOf<Float>* equation_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *equation_base = (GLfloat *) 0;
    GLfloat *equation = (GLfloat *) 0;

    if (!equation_ref) {
        LOGD("GlClipPlanef: equation == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlClipPlanef: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    equation_base = (GLfloat *) equation_ref->GetPayload();
    equation = equation_base + offset;

    glClipPlanef(
        (GLenum)plane,
        (GLfloat *)equation
    );
    return NOERROR;
}

ECode CGLES11::GlClipPlanef(
    /* [in] */ Int32 plane,
    /* [in] */ Elastos::IO::IFloatBuffer* equation_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *equation = (GLfloat *) 0;
    Handle32 equationTmp;

    FAIL_RETURN(GetPointer(equation_buf, &_array, &_remaining, &_bufferOffset, &equationTmp));
    equation = (GLfloat *) equationTmp;
    if (equation == NULL) {
        char * _equationBase = reinterpret_cast<char *>(_array);
        equation = (GLfloat *) (_equationBase + _bufferOffset);
    }
    glClipPlanef(
        (GLenum)plane,
        (GLfloat *)equation
    );
    return NOERROR;
}

ECode CGLES11::GlClipPlanex(
    /* [in] */ Int32 plane,
    /* [in] */ ArrayOf<Int32>* equation_ref,
    /* [in] */ Int32 offset)
{
    GLfixed *equation_base = (GLfixed *) 0;
    GLfixed *equation = (GLfixed *) 0;

    if (!equation_ref) {
        LOGD("GlClipPlanex: equation == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlClipPlanex: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    equation_base = (GLfixed *) equation_ref->GetPayload();
    equation = equation_base + offset;

    glClipPlanex(
        (GLenum)plane,
        (GLfixed *) equation);

    return NOERROR;
}


ECode CGLES11::GlClipPlanex(
    /* [in] */ Int32 plane,
    /* [in] */ Elastos::IO::IInt32Buffer* equation_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfixed *equation = (GLfixed *) 0;
    Handle32 equationTmp;

    FAIL_RETURN(GetPointer(equation_buf, &_array, &_remaining, &_bufferOffset, &equationTmp));
    equation = (GLfixed *) equationTmp;
    if (equation == NULL) {
        char * _equationBase = reinterpret_cast<char *>(_array);
        equation = (GLfixed *) (_equationBase + _bufferOffset);
    }
    glClipPlanex(
        (GLenum)plane,
        (GLfixed *)equation
    );
    return NOERROR;
}

ECode CGLES11::GlColor4ub(
    /* [in] */ Byte red,
    /* [in] */ Byte green,
    /* [in] */ Byte blue,
    /* [in] */ Byte alpha)
{
    glColor4ub(
        (GLubyte)red,
        (GLubyte)green,
        (GLubyte)blue,
        (GLubyte)alpha
    );
    return NOERROR;
}

ECode CGLES11::GlColorPointer(
    /* [in] */ Int32 size,
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 offset)
{
    glColorPointer(
        (GLint)size,
        (GLenum)type,
        (GLsizei)stride,
        (const GLvoid *)offset
    );
    return NOERROR;
}

ECode CGLES11::GlDeleteBuffers(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* buffers_ref,
    /* [in] */ Int32 offset)
{
    GLuint *buffers_base = (GLuint *) 0;
    Int32 _remaining;
    GLuint *buffers = (GLuint *) 0;

    if (!buffers_ref) {
        LOGD("buffers == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = buffers_ref->GetLength() - offset;
    if (_remaining < n) {
        LOGD("length - offset < n < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    buffers_base = (GLuint *) buffers_ref->GetPayload();
    buffers = buffers_base + offset;

    glDeleteBuffers(
        (GLsizei)n,
        (GLuint *)buffers
    );
    return NOERROR;
}

ECode CGLES11::GlDeleteBuffers(
    /* [in] */ Int32 n,
    /* [in] */ Elastos::IO::IInt32Buffer* buffers_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLuint *buffers = (GLuint *) 0;
    Handle32 buffersTmp;

    FAIL_RETURN(GetPointer(buffers_buf, &_array, &_remaining, &_bufferOffset, &buffersTmp));
    buffers = (GLuint *) buffersTmp;
    if (_remaining < n) {
        LOGD("remaining() < n < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (buffers == NULL) {
        char * _buffersBase = reinterpret_cast<char *>(_array);
        buffers = (GLuint *) (_buffersBase + _bufferOffset);
    }
    glDeleteBuffers(
        (GLsizei)n,
        (GLuint *)buffers
    );
    return NOERROR;
}

ECode CGLES11::GlDrawElements(
    /* [in] */ Int32 mode,
    /* [in] */ Int32 count,
    /* [in] */ Int32 type,
    /* [in] */ Int32 offset)
{
    glDrawElements(
        (GLenum)mode,
        (GLsizei)count,
        (GLenum)type,
        (const GLvoid *)offset
    );
    return NOERROR;
}

ECode CGLES11::GlGenBuffers(
    /* [in] */ Int32 n,
    /* [in] */ ArrayOf<Int32>* buffers_ref,
    /* [in] */ Int32 offset)
{
    GLuint *buffers_base = (GLuint *) 0;
    Int32 _remaining;
    GLuint *buffers = (GLuint *) 0;

    if (!buffers_ref) {
        LOGD("buffers == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = buffers_ref->GetLength() - offset;
    if (_remaining < n) {
        LOGD("length - offset < n < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    buffers_base = (GLuint *) buffers_ref->GetPayload();
    buffers = buffers_base + offset;

    glGenBuffers(
        (GLsizei)n,
        (GLuint *)buffers
    );
    return NOERROR;
}

ECode CGLES11::GlGenBuffers(
    /* [in] */ Int32 n,
    /* [in] */ Elastos::IO::IInt32Buffer* buffers_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLuint *buffers = (GLuint *) 0;
    Handle32 buffersTmp;

    FAIL_RETURN(GetPointer(buffers_buf, &_array, &_remaining, &_bufferOffset, &buffersTmp));
    buffers = (GLuint *) buffersTmp;
    if (_remaining < n) {
        LOGD("remaining() < n < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (buffers == NULL) {
        char * _buffersBase = reinterpret_cast<char *>(_array);
        buffers = (GLuint *) (_buffersBase + _bufferOffset);
    }
    glGenBuffers(
        (GLsizei)n,
        (GLuint *)buffers
    );
    return NOERROR;
}

ECode CGLES11::GlGetBooleanv(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Boolean>* params_ref,
    /* [in] */ Int32 offset)
{
    GLboolean *params_base = (GLboolean *) 0;
    GLboolean *params = (GLboolean *) 0;

    if (!params_ref) {
        LOGD("params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLboolean *) params_ref->GetPayload();
    params = params_base + offset;

    glGetBooleanv(
        (GLenum)pname,
        (GLboolean *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetBooleanv(
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLboolean *params = (GLboolean *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLboolean *) paramsTmp;
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLboolean *) (_paramsBase + _bufferOffset);
    }
    glGetBooleanv(
        (GLenum)pname,
        (GLboolean *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetBufferParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLint *params_base = (GLint *) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    if (!params_ref) {
        LOGD("params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    if (_remaining < 1) {
        LOGD("length - offset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLint *) params_ref->GetPayload();
    params = params_base + offset;

    glGetBufferParameteriv(
        (GLenum)target,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetBufferParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLint *) paramsTmp;
    if (_remaining < 1) {
        LOGD("remaining() < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLint *) (_paramsBase + _bufferOffset);
    }
    glGetBufferParameteriv(
        (GLenum)target,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetClipPlanef(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* eqn_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *eqn_base = (GLfloat *) 0;
    GLfloat *eqn = (GLfloat *) 0;

    if (!eqn_ref) {
        LOGD("GlGetClipPlanef: eqn == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlGetClipPlanef: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    eqn_base = (GLfloat *) eqn_ref->GetPayload();
    eqn = eqn_base + offset;

    glGetClipPlanef(
        (GLenum)pname,
        (GLfloat *)eqn
    );
    return NOERROR;
}

ECode CGLES11::GlGetClipPlanef(
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IFloatBuffer* eqn_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *eqn = (GLfloat *) 0;
    Handle32 eqnTmp;

    FAIL_RETURN(GetPointer(eqn_buf, &_array, &_remaining, &_bufferOffset, &eqnTmp));
    eqn = (GLfloat *) eqnTmp;
    if (eqn == NULL) {
        char * _eqnBase = reinterpret_cast<char *>(_array);
        eqn = (GLfloat *) (_eqnBase + _bufferOffset);
    }
    glGetClipPlanef(
        (GLenum)pname,
        (GLfloat *)eqn
    );
    return NOERROR;
}

ECode CGLES11::GlGetClipPlanex(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* eqn_ref,
    /* [in] */ Int32 offset)
{
    GLfixed *eqn_base = (GLfixed *) 0;
    GLfixed *eqn = (GLfixed *) 0;

    if (!eqn_ref) {
        LOGD("GlGetClipPlanex: eqn == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlGetClipPlanex: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    eqn_base = (GLfixed *) eqn_ref->GetPayload();
    eqn = eqn_base + offset;

    glGetClipPlanex(
        (GLenum)pname,
        (GLfixed *)eqn
    );
    return NOERROR;
}

ECode CGLES11::GlGetClipPlanex(
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* eqn_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfixed *eqn = (GLfixed *) 0;
    Handle32 eqnTmp;

    FAIL_RETURN(GetPointer(eqn_buf, &_array, &_remaining, &_bufferOffset, &eqnTmp));
    eqn = (GLfixed *) eqnTmp;
    if (eqn == NULL) {
        char * _eqnBase = reinterpret_cast<char *>(_array);
        eqn = (GLfixed *) (_eqnBase + _bufferOffset);
    }
    glGetClipPlanex(
        (GLenum)pname,
        (GLfixed *)eqn
    );
    return NOERROR;
}

ECode CGLES11::GlGetFixedv(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfixed *params_base = (GLfixed *) 0;
    GLfixed *params = (GLfixed *) 0;

    if (!params_ref) {
        LOGD("GlGetFixedv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlGetFixedv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfixed *) params_ref->GetPayload();
    params = params_base + offset;

    glGetFixedv(
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetFixedv(
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfixed *) paramsTmp;
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfixed *) (_paramsBase + _bufferOffset);
    }
    glGetFixedv(
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetFloatv(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *params_base = (GLfloat *) 0;
    GLfloat *params = (GLfloat *) 0;

    if (!params_ref) {
        LOGD("GlGetFloatv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlGetFloatv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfloat *) params_ref->GetPayload();
    params = params_base + offset;

    glGetFloatv(
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetFloatv(
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IFloatBuffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfloat *) paramsTmp;
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfloat *) (_paramsBase + _bufferOffset);
    }
    glGetFloatv(
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetLightfv(
    /* [in] */ Int32 light,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *params_base = (GLfloat *) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;

    if (!params_ref) {
        LOGD("GlGetLightfv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlGetLightfv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    Int32 _needed;
    switch (pname) {
#if defined(GL_SPOT_EXPONENT)
        case GL_SPOT_EXPONENT:
#endif // defined(GL_SPOT_EXPONENT)
#if defined(GL_SPOT_CUTOFF)
        case GL_SPOT_CUTOFF:
#endif // defined(GL_SPOT_CUTOFF)
#if defined(GL_CONSTANT_ATTENUATION)
        case GL_CONSTANT_ATTENUATION:
#endif // defined(GL_CONSTANT_ATTENUATION)
#if defined(GL_LINEAR_ATTENUATION)
        case GL_LINEAR_ATTENUATION:
#endif // defined(GL_LINEAR_ATTENUATION)
#if defined(GL_QUADRATIC_ATTENUATION)
        case GL_QUADRATIC_ATTENUATION:
#endif // defined(GL_QUADRATIC_ATTENUATION)
            _needed = 1;
            break;
#if defined(GL_SPOT_DIRECTION)
        case GL_SPOT_DIRECTION:
#endif // defined(GL_SPOT_DIRECTION)
            _needed = 3;
            break;
#if defined(GL_AMBIENT)
        case GL_AMBIENT:
#endif // defined(GL_AMBIENT)
#if defined(GL_DIFFUSE)
        case GL_DIFFUSE:
#endif // defined(GL_DIFFUSE)
#if defined(GL_SPECULAR)
        case GL_SPECULAR:
#endif // defined(GL_SPECULAR)
#if defined(GL_EMISSION)
        case GL_EMISSION:
#endif // defined(GL_EMISSION)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlGetLightfv: length - offset < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfloat *) params_ref->GetPayload();
    params = params_base + offset;

    glGetLightfv(
        (GLenum)light,
        (GLenum)pname,
        (GLfloat *)params
    );

    return NOERROR;
}

ECode CGLES11::GlGetLightfv(
    /* [in] */ Int32 light,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IFloatBuffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfloat *) paramsTmp;
    Int32 _needed;
    switch (pname) {
#if defined(GL_SPOT_EXPONENT)
        case GL_SPOT_EXPONENT:
#endif // defined(GL_SPOT_EXPONENT)
#if defined(GL_SPOT_CUTOFF)
        case GL_SPOT_CUTOFF:
#endif // defined(GL_SPOT_CUTOFF)
#if defined(GL_CONSTANT_ATTENUATION)
        case GL_CONSTANT_ATTENUATION:
#endif // defined(GL_CONSTANT_ATTENUATION)
#if defined(GL_LINEAR_ATTENUATION)
        case GL_LINEAR_ATTENUATION:
#endif // defined(GL_LINEAR_ATTENUATION)
#if defined(GL_QUADRATIC_ATTENUATION)
        case GL_QUADRATIC_ATTENUATION:
#endif // defined(GL_QUADRATIC_ATTENUATION)
            _needed = 1;
            break;
#if defined(GL_SPOT_DIRECTION)
        case GL_SPOT_DIRECTION:
#endif // defined(GL_SPOT_DIRECTION)
            _needed = 3;
            break;
#if defined(GL_AMBIENT)
        case GL_AMBIENT:
#endif // defined(GL_AMBIENT)
#if defined(GL_DIFFUSE)
        case GL_DIFFUSE:
#endif // defined(GL_DIFFUSE)
#if defined(GL_SPECULAR)
        case GL_SPECULAR:
#endif // defined(GL_SPECULAR)
#if defined(GL_EMISSION)
        case GL_EMISSION:
#endif // defined(GL_EMISSION)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlGetLightfvEx: remaining() < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfloat *) (_paramsBase + _bufferOffset);
    }
    glGetLightfv(
        (GLenum)light,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetLightxv(
    /* [in] */ Int32 light,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfixed *params_base = (GLfixed *) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;

    if (!params_ref) {
        LOGD("GlGetLightxv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlGetLightxv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    Int32 _needed;
    switch (pname) {
#if defined(GL_SPOT_EXPONENT)
        case GL_SPOT_EXPONENT:
#endif // defined(GL_SPOT_EXPONENT)
#if defined(GL_SPOT_CUTOFF)
        case GL_SPOT_CUTOFF:
#endif // defined(GL_SPOT_CUTOFF)
#if defined(GL_CONSTANT_ATTENUATION)
        case GL_CONSTANT_ATTENUATION:
#endif // defined(GL_CONSTANT_ATTENUATION)
#if defined(GL_LINEAR_ATTENUATION)
        case GL_LINEAR_ATTENUATION:
#endif // defined(GL_LINEAR_ATTENUATION)
#if defined(GL_QUADRATIC_ATTENUATION)
        case GL_QUADRATIC_ATTENUATION:
#endif // defined(GL_QUADRATIC_ATTENUATION)
            _needed = 1;
            break;
#if defined(GL_SPOT_DIRECTION)
        case GL_SPOT_DIRECTION:
#endif // defined(GL_SPOT_DIRECTION)
            _needed = 3;
            break;
#if defined(GL_AMBIENT)
        case GL_AMBIENT:
#endif // defined(GL_AMBIENT)
#if defined(GL_DIFFUSE)
        case GL_DIFFUSE:
#endif // defined(GL_DIFFUSE)
#if defined(GL_SPECULAR)
        case GL_SPECULAR:
#endif // defined(GL_SPECULAR)
#if defined(GL_EMISSION)
        case GL_EMISSION:
#endif // defined(GL_EMISSION)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlGetLightxv: length - offset < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfixed *) params_ref->GetPayload();
    params = params_base + offset;

    glGetLightxv(
        (GLenum)light,
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetLightxv(
    /* [in] */ Int32 light,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfixed *) paramsTmp;
    Int32 _needed;
    switch (pname) {
#if defined(GL_SPOT_EXPONENT)
        case GL_SPOT_EXPONENT:
#endif // defined(GL_SPOT_EXPONENT)
#if defined(GL_SPOT_CUTOFF)
        case GL_SPOT_CUTOFF:
#endif // defined(GL_SPOT_CUTOFF)
#if defined(GL_CONSTANT_ATTENUATION)
        case GL_CONSTANT_ATTENUATION:
#endif // defined(GL_CONSTANT_ATTENUATION)
#if defined(GL_LINEAR_ATTENUATION)
        case GL_LINEAR_ATTENUATION:
#endif // defined(GL_LINEAR_ATTENUATION)
#if defined(GL_QUADRATIC_ATTENUATION)
        case GL_QUADRATIC_ATTENUATION:
#endif // defined(GL_QUADRATIC_ATTENUATION)
            _needed = 1;
            break;
#if defined(GL_SPOT_DIRECTION)
        case GL_SPOT_DIRECTION:
#endif // defined(GL_SPOT_DIRECTION)
            _needed = 3;
            break;
#if defined(GL_AMBIENT)
        case GL_AMBIENT:
#endif // defined(GL_AMBIENT)
#if defined(GL_DIFFUSE)
        case GL_DIFFUSE:
#endif // defined(GL_DIFFUSE)
#if defined(GL_SPECULAR)
        case GL_SPECULAR:
#endif // defined(GL_SPECULAR)
#if defined(GL_EMISSION)
        case GL_EMISSION:
#endif // defined(GL_EMISSION)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlGetLightxvEx: remaining() < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfixed *) (_paramsBase + _bufferOffset);
    }
    glGetLightxv(
        (GLenum)light,
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetMaterialfv(
    /* [in] */ Int32 face,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *params_base = (GLfloat *) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;

    if (!params_ref) {
        LOGD("GlGetMaterialfv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlGetMaterialfv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    Int32 _needed;
    switch (pname) {
#if defined(GL_SHININESS)
        case GL_SHININESS:
#endif // defined(GL_SHININESS)
            _needed = 1;
            break;
#if defined(GL_AMBIENT)
        case GL_AMBIENT:
#endif // defined(GL_AMBIENT)
#if defined(GL_DIFFUSE)
        case GL_DIFFUSE:
#endif // defined(GL_DIFFUSE)
#if defined(GL_SPECULAR)
        case GL_SPECULAR:
#endif // defined(GL_SPECULAR)
#if defined(GL_EMISSION)
        case GL_EMISSION:
#endif // defined(GL_EMISSION)
#if defined(GL_AMBIENT_AND_DIFFUSE)
        case GL_AMBIENT_AND_DIFFUSE:
#endif // defined(GL_AMBIENT_AND_DIFFUSE)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlGetMaterialfv: length - offset < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfloat *) params_ref->GetPayload();
    params = params_base + offset;

    glGetMaterialfv(
        (GLenum)face,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetMaterialfv(
    /* [in] */ Int32 face,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IFloatBuffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfloat *) paramsTmp;
    Int32 _needed;
    switch (pname) {
#if defined(GL_SHININESS)
        case GL_SHININESS:
#endif // defined(GL_SHININESS)
            _needed = 1;
            break;
#if defined(GL_AMBIENT)
        case GL_AMBIENT:
#endif // defined(GL_AMBIENT)
#if defined(GL_DIFFUSE)
        case GL_DIFFUSE:
#endif // defined(GL_DIFFUSE)
#if defined(GL_SPECULAR)
        case GL_SPECULAR:
#endif // defined(GL_SPECULAR)
#if defined(GL_EMISSION)
        case GL_EMISSION:
#endif // defined(GL_EMISSION)
#if defined(GL_AMBIENT_AND_DIFFUSE)
        case GL_AMBIENT_AND_DIFFUSE:
#endif // defined(GL_AMBIENT_AND_DIFFUSE)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlGetMaterialfvEx: remaining() < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfloat *) (_paramsBase + _bufferOffset);
    }
    glGetMaterialfv(
        (GLenum)face,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetMaterialxv(
    /* [in] */ Int32 face,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfixed *params_base = (GLfixed *) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;

    if (!params_ref) {
        LOGD("GlGetMaterialxv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlGetMaterialxv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    Int32 _needed;
    switch (pname) {
#if defined(GL_SHININESS)
        case GL_SHININESS:
#endif // defined(GL_SHININESS)
            _needed = 1;
            break;
#if defined(GL_AMBIENT)
        case GL_AMBIENT:
#endif // defined(GL_AMBIENT)
#if defined(GL_DIFFUSE)
        case GL_DIFFUSE:
#endif // defined(GL_DIFFUSE)
#if defined(GL_SPECULAR)
        case GL_SPECULAR:
#endif // defined(GL_SPECULAR)
#if defined(GL_EMISSION)
        case GL_EMISSION:
#endif // defined(GL_EMISSION)
#if defined(GL_AMBIENT_AND_DIFFUSE)
        case GL_AMBIENT_AND_DIFFUSE:
#endif // defined(GL_AMBIENT_AND_DIFFUSE)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlGetMaterialxv: length - offset < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfixed *) params_ref->GetPayload();
    params = params_base + offset;

    glGetMaterialxv(
        (GLenum)face,
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetMaterialxv(
    /* [in] */ Int32 face,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfixed *) paramsTmp;
    Int32 _needed;
    switch (pname) {
#if defined(GL_SHININESS)
        case GL_SHININESS:
#endif // defined(GL_SHININESS)
            _needed = 1;
            break;
#if defined(GL_AMBIENT)
        case GL_AMBIENT:
#endif // defined(GL_AMBIENT)
#if defined(GL_DIFFUSE)
        case GL_DIFFUSE:
#endif // defined(GL_DIFFUSE)
#if defined(GL_SPECULAR)
        case GL_SPECULAR:
#endif // defined(GL_SPECULAR)
#if defined(GL_EMISSION)
        case GL_EMISSION:
#endif // defined(GL_EMISSION)
#if defined(GL_AMBIENT_AND_DIFFUSE)
        case GL_AMBIENT_AND_DIFFUSE:
#endif // defined(GL_AMBIENT_AND_DIFFUSE)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlGetMaterialxvEx: remaining() < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfixed *) (_paramsBase + _bufferOffset);
    }
    glGetMaterialxv(
        (GLenum)face,
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetTexEnvfv(
    /* [in] */ Int32 env,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *params_base = (GLfloat *) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;

    if (!params_ref) {
        LOGD("GlGetTexEnvfv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlGetTexEnvfv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    Int32 _needed;
    switch (pname) {
#if defined(GL_TEXTURE_ENV_MODE)
        case GL_TEXTURE_ENV_MODE:
#endif // defined(GL_TEXTURE_ENV_MODE)
#if defined(GL_COMBINE_RGB)
        case GL_COMBINE_RGB:
#endif // defined(GL_COMBINE_RGB)
#if defined(GL_COMBINE_ALPHA)
        case GL_COMBINE_ALPHA:
#endif // defined(GL_COMBINE_ALPHA)
            _needed = 1;
            break;
#if defined(GL_TEXTURE_ENV_COLOR)
        case GL_TEXTURE_ENV_COLOR:
#endif // defined(GL_TEXTURE_ENV_COLOR)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlGetTexEnvfv: length - offset < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfloat *) params_ref->GetPayload();
    params = params_base + offset;

    glGetTexEnvfv(
        (GLenum)env,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetTexEnvfv(
    /* [in] */ Int32 env,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IFloatBuffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfloat *) paramsTmp;
    Int32 _needed;
    switch (pname) {
#if defined(GL_TEXTURE_ENV_MODE)
        case GL_TEXTURE_ENV_MODE:
#endif // defined(GL_TEXTURE_ENV_MODE)
#if defined(GL_COMBINE_RGB)
        case GL_COMBINE_RGB:
#endif // defined(GL_COMBINE_RGB)
#if defined(GL_COMBINE_ALPHA)
        case GL_COMBINE_ALPHA:
#endif // defined(GL_COMBINE_ALPHA)
            _needed = 1;
            break;
#if defined(GL_TEXTURE_ENV_COLOR)
        case GL_TEXTURE_ENV_COLOR:
#endif // defined(GL_TEXTURE_ENV_COLOR)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlGetTexEnvfvEx: remaining() < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfloat *) (_paramsBase + _bufferOffset);
    }
    glGetTexEnvfv(
        (GLenum)env,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetTexEnviv(
    /* [in] */ Int32 env,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLint *params_base = (GLint *) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    if (!params_ref) {
        LOGD("GlGetTexEnviv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlGetTexEnviv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    int _needed;
    switch (pname) {
#if defined(GL_TEXTURE_ENV_MODE)
        case GL_TEXTURE_ENV_MODE:
#endif // defined(GL_TEXTURE_ENV_MODE)
#if defined(GL_COMBINE_RGB)
        case GL_COMBINE_RGB:
#endif // defined(GL_COMBINE_RGB)
#if defined(GL_COMBINE_ALPHA)
        case GL_COMBINE_ALPHA:
#endif // defined(GL_COMBINE_ALPHA)
            _needed = 1;
            break;
#if defined(GL_TEXTURE_ENV_COLOR)
        case GL_TEXTURE_ENV_COLOR:
#endif // defined(GL_TEXTURE_ENV_COLOR)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlGetTexEnviv: length - offset < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLint *) params_ref->GetPayload();
    params = params_base + offset;

    glGetTexEnviv(
        (GLenum)env,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetTexEnviv(
    /* [in] */ Int32 env,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLint *) paramsTmp;
    Int32 _needed;
    switch (pname) {
#if defined(GL_TEXTURE_ENV_MODE)
        case GL_TEXTURE_ENV_MODE:
#endif // defined(GL_TEXTURE_ENV_MODE)
#if defined(GL_COMBINE_RGB)
        case GL_COMBINE_RGB:
#endif // defined(GL_COMBINE_RGB)
#if defined(GL_COMBINE_ALPHA)
        case GL_COMBINE_ALPHA:
#endif // defined(GL_COMBINE_ALPHA)
            _needed = 1;
            break;
#if defined(GL_TEXTURE_ENV_COLOR)
        case GL_TEXTURE_ENV_COLOR:
#endif // defined(GL_TEXTURE_ENV_COLOR)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlGetTexEnvivEx: remaining() < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLint *) (_paramsBase + _bufferOffset);
    }
    glGetTexEnviv(
        (GLenum)env,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetTexEnvxv(
    /* [in] */ Int32 env,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfixed *params_base = (GLfixed *) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;

    if (!params_ref) {
        LOGD("GlGetTexEnvxv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlGetTexEnvxv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    Int32 _needed;
    switch (pname) {
#if defined(GL_TEXTURE_ENV_MODE)
        case GL_TEXTURE_ENV_MODE:
#endif // defined(GL_TEXTURE_ENV_MODE)
#if defined(GL_COMBINE_RGB)
        case GL_COMBINE_RGB:
#endif // defined(GL_COMBINE_RGB)
#if defined(GL_COMBINE_ALPHA)
        case GL_COMBINE_ALPHA:
#endif // defined(GL_COMBINE_ALPHA)
            _needed = 1;
            break;
#if defined(GL_TEXTURE_ENV_COLOR)
        case GL_TEXTURE_ENV_COLOR:
#endif // defined(GL_TEXTURE_ENV_COLOR)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlGetTexEnvxv: length - offset < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfixed *) params_ref->GetPayload();
    params = params_base + offset;

    glGetTexEnvxv(
        (GLenum)env,
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetTexEnvxv(
    /* [in] */ Int32 env,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfixed *) paramsTmp;
    Int32 _needed;
    switch (pname) {
#if defined(GL_TEXTURE_ENV_MODE)
        case GL_TEXTURE_ENV_MODE:
#endif // defined(GL_TEXTURE_ENV_MODE)
#if defined(GL_COMBINE_RGB)
        case GL_COMBINE_RGB:
#endif // defined(GL_COMBINE_RGB)
#if defined(GL_COMBINE_ALPHA)
        case GL_COMBINE_ALPHA:
#endif // defined(GL_COMBINE_ALPHA)
            _needed = 1;
            break;
#if defined(GL_TEXTURE_ENV_COLOR)
        case GL_TEXTURE_ENV_COLOR:
#endif // defined(GL_TEXTURE_ENV_COLOR)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlGetTexEnvxvEx: remaining() < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfixed *) (_paramsBase + _bufferOffset);
    }
    glGetTexEnvxv(
        (GLenum)env,
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetTexParameterfv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *params_base = (GLfloat *) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;

    if (!params_ref) {
        LOGD("GlGetTexParameterfv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlGetTexParameterfv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    if (_remaining < 1) {
        LOGD("GlGetTexParameterfv: length - offset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfloat *) params_ref->GetPayload();
    params = params_base + offset;

    glGetTexParameterfv(
        (GLenum)target,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetTexParameterfvEx (
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IFloatBuffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfloat *) paramsTmp;
    if (_remaining < 1) {
        LOGD("GlGetTexParameterfvEx: remaining() < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfloat *) (_paramsBase + _bufferOffset);
    }
    glGetTexParameterfv(
        (GLenum)target,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetTexParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLint *params_base = (GLint *) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    if (!params_ref) {
        LOGD("GlGetTexParameteriv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlGetTexParameteriv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    if (_remaining < 1) {
        LOGD("GlGetTexParameteriv: length - offset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLint *) params_ref->GetPayload();
    params = params_base + offset;

    glGetTexParameteriv(
        (GLenum)target,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetTexParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLint *) paramsTmp;
    if (_remaining < 1) {
        LOGD("GlGetTexParameterivEx: remaining() < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLint *) (_paramsBase + _bufferOffset);
    }
    glGetTexParameteriv(
        (GLenum)target,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetTexParameterxv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfixed *params_base = (GLfixed *) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;

    if (!params_ref) {
        LOGD("GlGetTexParameterxv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlGetTexParameterxv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    if (_remaining < 1) {
        LOGD("GlGetTexParameterxv: length - offset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfixed *) params_ref->GetPayload();
    params = params_base + offset;

    glGetTexParameterxv(
        (GLenum)target,
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES11::GlGetTexParameterxv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfixed *) paramsTmp;
    if (_remaining < 1) {
        LOGD("GlGetTexParameterxvEx: remaining() < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfixed *) (_paramsBase + _bufferOffset);
    }
    glGetTexParameterxv(
        (GLenum)target,
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES11::GlIsBuffer(
    /* [in] */ Int32 buffer,
    /* [out] */ Boolean* isBuffer)
{
    GLboolean _returnValue;
    _returnValue = glIsBuffer(
        (GLuint)buffer
    );
    *isBuffer = _returnValue;
    return NOERROR;
}

ECode CGLES11::GlIsEnabled(
    /* [in] */ Int32 cap,
    /* [out] */ Boolean* isEnabled)
{
    GLboolean _returnValue;
    _returnValue = glIsEnabled(
        (GLenum)cap
    );
    *isEnabled = _returnValue;
    return NOERROR;
}

ECode CGLES11::GlIsTexture(
    /* [in] */ Int32 texture,
    /* [out] */ Boolean* isTexture)
{
    GLboolean _returnValue;
    _returnValue = glIsTexture(
        (GLuint)texture
    );
    *isTexture = _returnValue;
    return NOERROR;
}

ECode CGLES11::GlNormalPointer(
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 offset)
{
    glNormalPointer(
        (GLenum)type,
        (GLsizei)stride,
        (const GLvoid *)offset
    );
    return NOERROR;
}

ECode CGLES11::GlPointParameterf(
    /* [in] */ Int32 pname,
    /* [in] */ Float param)
{
    glPointParameterf(
        (GLenum)pname,
        (GLfloat)param
    );
    return NOERROR;
}

ECode CGLES11::GlPointParameterfv(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *params_base = (GLfloat *) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;

    if (!params_ref) {
        LOGD("GlPointParameterfv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlPointParameterfv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    if (_remaining < 1) {
        LOGD("GlPointParameterfv: length - offset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfloat *) params_ref->GetPayload();
    params = params_base + offset;

    glPointParameterfv(
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES11::GlPointParameterfv(
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IFloatBuffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfloat *) paramsTmp;
    if (_remaining < 1) {
        LOGD("GlPointParameterfvEx: remaining() < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfloat *) (_paramsBase + _bufferOffset);
    }
    glPointParameterfv(
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES11::GlPointParameterx(
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    glPointParameterx(
        (GLenum)pname,
        (GLfixed)param
    );
    return NOERROR;
}

ECode CGLES11::GlPointParameterxv(
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfixed *params_base = (GLfixed *) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;

    if (!params_ref) {
        LOGD("GlPointParameterxv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlPointParameterxv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    if (_remaining < 1) {
        LOGD("GlPointParameterxv: length - offset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfixed *) params_ref->GetPayload();
    params = params_base + offset;

    glPointParameterxv(
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES11::GlPointParameterxv(
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfixed *) paramsTmp;
    if (_remaining < 1) {
        LOGD("GlPointParameterxvEx: remaining() < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfixed *) (_paramsBase + _bufferOffset);
    }
    glPointParameterxv(
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES11::GlPointSizePointerOES(
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ Elastos::IO::IBuffer* pointer)
{
    Int32 remaining;
    pointer->GetRemaining(&remaining);
    ECode ec = GlPointSizePointerOESBounds(
        type,
        stride,
        pointer,
        remaining
    );
    if (((type == GL_FLOAT) ||
         (type == GL_FIXED)) &&
        (stride >= 0)) {
        sPointSizePointerOES = pointer;
    }
    return ec;
}

ECode CGLES11::GlPointSizePointerOESBounds(
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ Elastos::IO::IBuffer* pointer_buf,
    /* [in] */ Int32 remaining)
{
    GLvoid *pointer = (GLvoid *) 0;
    Handle32 pointerTmp;

    if (pointer_buf) {
        FAIL_RETURN(GetDirectBufferPointer(pointer_buf, &pointerTmp));
        pointer = (GLvoid *) pointerTmp;
        if ( ! pointer ) {
            return NOERROR;
        }
    }
    glPointSizePointerOESBounds(
        (GLenum)type,
        (GLsizei)stride,
        (GLvoid *)pointer,
        (GLsizei)remaining);

    return NOERROR;
}

ECode CGLES11::GlTexCoordPointer(
    /* [in] */ Int32 size,
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 offset)
{
    glTexCoordPointer(
        (GLint)size,
        (GLenum)type,
        (GLsizei)stride,
        (const GLvoid *)offset
    );
    return NOERROR;
}

ECode CGLES11::GlTexEnvi(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    glTexEnvi(
        (GLenum)target,
        (GLenum)pname,
        (GLint)param
    );
    return NOERROR;
}

ECode CGLES11::GlTexEnviv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLint *params_base = (GLint *) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    if (!params_ref) {
        LOGD("GlTexEnviv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlTexEnviv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    Int32 _needed;
    switch (pname) {
#if defined(GL_TEXTURE_ENV_MODE)
        case GL_TEXTURE_ENV_MODE:
#endif // defined(GL_TEXTURE_ENV_MODE)
#if defined(GL_COMBINE_RGB)
        case GL_COMBINE_RGB:
#endif // defined(GL_COMBINE_RGB)
#if defined(GL_COMBINE_ALPHA)
        case GL_COMBINE_ALPHA:
#endif // defined(GL_COMBINE_ALPHA)
            _needed = 1;
            break;
#if defined(GL_TEXTURE_ENV_COLOR)
        case GL_TEXTURE_ENV_COLOR:
#endif // defined(GL_TEXTURE_ENV_COLOR)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlTexEnviv: length - offset < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLint *) params_ref->GetPayload();
    params = params_base + offset;

    glTexEnviv(
        (GLenum)target,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

ECode CGLES11::GlTexEnviv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLint *) paramsTmp;
    Int32 _needed;
    switch (pname) {
#if defined(GL_TEXTURE_ENV_MODE)
        case GL_TEXTURE_ENV_MODE:
#endif // defined(GL_TEXTURE_ENV_MODE)
#if defined(GL_COMBINE_RGB)
        case GL_COMBINE_RGB:
#endif // defined(GL_COMBINE_RGB)
#if defined(GL_COMBINE_ALPHA)
        case GL_COMBINE_ALPHA:
#endif // defined(GL_COMBINE_ALPHA)
            _needed = 1;
            break;
#if defined(GL_TEXTURE_ENV_COLOR)
        case GL_TEXTURE_ENV_COLOR:
#endif // defined(GL_TEXTURE_ENV_COLOR)
            _needed = 4;
            break;
        default:
            _needed = 0;
            break;
    }
    if (_remaining < _needed) {
        LOGD("GlTexEnvivEx: remaining() < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLint *) (_paramsBase + _bufferOffset);
    }
    glTexEnviv(
        (GLenum)target,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

ECode CGLES11::GlTexParameterfv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Float>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfloat *params_base = (GLfloat *) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;

    if (!params_ref) {
        LOGD("GlTexParameterfv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlTexParameterfv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    if (_remaining < 1) {
        LOGD("GlTexParameterfv: length - offset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfloat *) params_ref->GetPayload();
    params = params_base + offset;

    glTexParameterfv(
        (GLenum)target,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES11::GlTexParameterfv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IFloatBuffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfloat *params = (GLfloat *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfloat *) paramsTmp;
    if (_remaining < 1) {
        LOGD("GlTexParameterfvEx: remaining() < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfloat *) (_paramsBase + _bufferOffset);
    }
    glTexParameterfv(
        (GLenum)target,
        (GLenum)pname,
        (GLfloat *)params
    );
    return NOERROR;
}

ECode CGLES11::GlTexParameteri(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Int32 param)
{
    glTexParameteri(
        (GLenum)target,
        (GLenum)pname,
        (GLint)param
    );
    return NOERROR;
}

ECode CGLES11::GlTexParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLint *params_base = (GLint *) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;

    if (!params_ref) {
        LOGD("GlTexParameteriv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlTexParameteriv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    if (_remaining < 1) {
        LOGD("GlTexParameteriv: length - offset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLint *) params_ref->GetPayload();
    params = params_base + offset;

    glTexParameteriv(
        (GLenum)target,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

ECode CGLES11::GlTexParameteriv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLint *params = (GLint *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLint *) paramsTmp;
    if (_remaining < 1) {
        LOGD("GlTexParameterivEx: remaining() < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLint *) (_paramsBase + _bufferOffset);
    }
    glTexParameteriv(
        (GLenum)target,
        (GLenum)pname,
        (GLint *)params
    );
    return NOERROR;
}

ECode CGLES11::GlTexParameterxv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ ArrayOf<Int32>* params_ref,
    /* [in] */ Int32 offset)
{
    GLfixed *params_base = (GLfixed *) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;

    if (!params_ref) {
        LOGD("GlTexParameterxv: params == null")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (offset < 0) {
        LOGD("GlTexParameterxv: offset < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    _remaining = params_ref->GetLength() - offset;
    if (_remaining < 1) {
        LOGD("GlTexParameterxv: length - offset < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    params_base = (GLfixed *) params_ref->GetPayload();
    params = params_base + offset;

    glTexParameterxv(
        (GLenum)target,
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES11::GlTexParameterxv(
    /* [in] */ Int32 target,
    /* [in] */ Int32 pname,
    /* [in] */ Elastos::IO::IInt32Buffer* params_buf)
{
    Handle32 _array = (Handle32) 0;
    Int32 _bufferOffset = (Int32) 0;
    Int32 _remaining;
    GLfixed *params = (GLfixed *) 0;
    Handle32 paramsTmp;

    FAIL_RETURN(GetPointer(params_buf, &_array, &_remaining, &_bufferOffset, &paramsTmp));
    params = (GLfixed *) paramsTmp;
    if (_remaining < 1) {
        LOGD("GlTexParameterxvEx: remaining() < 1 < needed")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
        char * _paramsBase = reinterpret_cast<char *>(_array);
        params = (GLfixed *) (_paramsBase + _bufferOffset);
    }
    glTexParameterxv(
        (GLenum)target,
        (GLenum)pname,
        (GLfixed *)params
    );
    return NOERROR;
}

ECode CGLES11::GlVertexPointer(
    /* [in] */ Int32 size,
    /* [in] */ Int32 type,
    /* [in] */ Int32 stride,
    /* [in] */ Int32 offset)
{
    glVertexPointer(
        (GLint)size,
        (GLenum)type,
        (GLsizei)stride,
        (const GLvoid *)offset
    );
    return NOERROR;
}

ECode CGLES11::GetDirectBufferPointer(
    /* [in] */ IBuffer* buffer,
    /* [out] */ Handle32* result)
{
    Handle32 primitiveArray;
    buffer->GetPrimitiveArray(&primitiveArray);
    if (primitiveArray != 0) {
        Int32 position, elementSizeShift;
        buffer->GetPosition(&position);
        buffer->GetElementSizeShift(&elementSizeShift);
        primitiveArray += position << elementSizeShift;
    } else {
        LOGD("GetDirectBufferPointer: Must use a native order direct Buffer")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *result = primitiveArray;
    return NOERROR;
}

ECode CGLES11::GetPointer(
    /* [in] */ IBuffer* buffer,
    /* [in, out] */ Handle32* array,
    /* [in, out] */ Int32* remaining,
    /* [in, out] */ Int32* offset,
    /* [out] */ Handle32* rst)
{
    Int32 position;
    Int32 limit;
    Int32 elementSizeShift;
    Int64 pointer;
    buffer->GetPosition(&position);
    buffer->GetLimit(&limit);
    buffer->GetElementSizeShift(&elementSizeShift);
    *remaining = (limit - position) << elementSizeShift;

    AutoPtr<INIOAccessHelper> helper;
    CNIOAccessHelper::AcquireSingleton((INIOAccessHelper**)&helper);

    helper->GetBasePointer(buffer, &pointer);
    if (pointer != 0L) {
        *array = 0;
        *rst = (Handle32)pointer;
        return NOERROR;
    }

    Boolean hasArray;
    if (buffer->HasArray(&hasArray), hasArray) {
        buffer->GetPrimitiveArray(array);
    } else {
        *array = 0;
    }
    helper->GetBaseArrayOffset(buffer, offset);

    *rst = 0;
    return NOERROR;
}

} // namespace Opengl
} // namespace Droid
} // namespace Elastos

