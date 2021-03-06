
#ifndef __ELASTOS_DROID_GRAPHICS_CPICTURE_H__
#define __ELASTOS_DROID_GRAPHICS_CPICTURE_H__

#include "_Elastos_Droid_Graphics_CPicture.h"
#include "graphics/Canvas.h"



using Elastos::IO::IInputStream;

namespace Elastos {
namespace Droid {
namespace Graphics {

/**
 * A picture records drawing calls (via the canvas returned by beginRecording)
 * and can then play them back (via picture.draw(canvas) or canvas.drawPicture).
 * The picture's contents can also be written to a stream, and then later
 * restored to a new picture (via writeToStream / createFromStream). For most
 * content (esp. text, lines, rectangles), drawing a sequence from a picture can
 * be faster than the equivalent API calls, since the picture performs its
 * playback without incurring any java-call overhead.
 */
CarClass(CPicture)
{
private:
    class _RecordingCanvas : public Canvas
    {
    public:
        _RecordingCanvas(
            /* [in] */ CPicture* pic,
            /* [in] */ Int32 nativeCanvas);

        CARAPI SetBitmap(
            /* [in] */ IBitmap* bitmap);

        CARAPI DrawPicture(
            /* [in] */ IPicture* picture);

    private:
        //Don't use AutoPtr here to avoid cycle reference with CPicture;
        CPicture* mPicture;
    };

    class RecordingCanvas
        : public ElRefBase
        , public _RecordingCanvas
        , public ICanvas
    {
    public:
        RecordingCanvas(
            /* [in] */ CPicture* pic,
            /* [in] */ Int32 nativeCanvas);

        ICANVAS_METHODS_DECL();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);
    };

public:
    CPicture();

    ~CPicture();

    CARAPI constructor();

    /**
     * Create a picture by making a copy of what has already been recorded in
     * src. The contents of src are unchanged, and if src changes later, those
     * changes will not be reflected in this picture.
     */
    CARAPI constructor(
        /* [in] */ IPicture* src);

    CARAPI constructor(
        /* [in] */ Int32 nativePicture,
        /* [in] */ Boolean fromStream);

    /**
     * To record a picture, call beginRecording() and then draw into the Canvas
     * that is returned. Nothing we appear on screen, but all of the draw
     * commands (e.g. drawRect(...)) will be recorded. To stop recording, call
     * endRecording(). At this point the Canvas that was returned must no longer
     * be referenced, and nothing should be drawn into it.
     */
    CARAPI BeginRecording(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [out] */ ICanvas** canvas);

    /**
     * Call endRecording when the picture is built. After this call, the picture
     * may be drawn, but the canvas that was returned by beginRecording must not
     * be referenced anymore. This is automatically called if Picture.draw() or
     * Canvas.drawPicture() is called.
     */
    CARAPI EndRecording();

    /**
     * Get the width of the picture as passed to beginRecording. This
     * does not reflect (per se) the content of the picture.
     */
    CARAPI GetWidth(
        /* [out] */ Int32* width);

    /**
     * Get the height of the picture as passed to beginRecording. This
     * does not reflect (per se) the content of the picture.
     */
    CARAPI GetHeight(
        /* [out] */ Int32* height);

    /**
     * Draw this picture on the canvas. The picture may have the side effect
     * of changing the matrix and clip of the canvas.
     *
     * @param canvas  The picture is drawn to this canvas
     */
    CARAPI Draw(
        /* [in] */ ICanvas* canvas);

    /**
     * Create a new picture (already recorded) from the data in the stream. This
     * data was generated by a previous call to writeToStream().
     *
     * <strong>Note:</strong> a picture created from an input stream cannot be
     * replayed on a hardware accelerated canvas.
     *
     * @see #writeToStream(java.io.OutputStream)
     */
    static CARAPI_(AutoPtr<IPicture>) CreateFromStream(
        /* [in] */ IInputStream* stream);

    /**
     * Write the picture contents to a stream. The data can be used to recreate
     * the picture in this or another process by calling createFromStream.
     *
     * <strong>Note:</strong> a picture created from an input stream cannot be
     * replayed on a hardware accelerated canvas.
     *
     * @see #createFromStream(java.io.InputStream)
     */
    CARAPI WriteToStream(
        /* [in] */ IOutputStream* stream);

    CARAPI GetNativePicture(
        /* [out] */ Handle32* natviePicture);

    /*package*/ CARAPI_(Int32) Ni();

private:
    // return empty picture if src is 0, or a copy of the native src
    static CARAPI_(Int32) NativeConstructor(
        /* [in] */ Int32 nativeSrcOr0);

    static CARAPI_(Int32) NativeCreateFromStream(
        /* [in] */ IInputStream* stream,
        /* [in] */ ArrayOf<Byte>* storage);

    static CARAPI_(Int32) NativeBeginRecording(
        /* [in] */ Int32 nativePicture,
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    static CARAPI_(void) NativeEndRecording(
        /* [in] */ Int32 nativePicture);

    static CARAPI_(void) NativeDraw(
        /* [in] */ Int32 nativeCanvas,
        /* [in] */ Int32 nativePicture);

    static CARAPI_(Boolean) NativeWriteToStream(
        /* [in] */ Int32 nativePicture,
        /* [in] */ IOutputStream* stream,
        /* [in] */ ArrayOf<Byte>* storage);

    static CARAPI_(void) NativeDestructor(
        /* [in] */ Int32 nativePicture);

public:
    Boolean mCreatedFromStream;

private:
    AutoPtr<ICanvas> mRecordingCanvas;
    Int32 mNativePicture;

    static const Int32 WORKING_STREAM_STORAGE = 16 * 1024;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_CPICTURE_H__
