
#ifndef __ELASTOS_DROID_GRAPHICS_CBITMAP_H__
#define __ELASTOS_DROID_GRAPHICS_CBITMAP_H__

#include "_Elastos_Droid_Graphics_CBitmap.h"

using Elastos::IO::IBuffer;
using Elastos::IO::IOutputStream;
using Elastos::Droid::Utility::IDisplayMetrics;

#ifdef __cplusplus
extern "C"
{
#endif
    extern void Elastos_Bitmap_InitCallback(Int32 cb);
#ifdef __cplusplus
}
#endif

namespace Elastos {
namespace Droid {
namespace Graphics {

struct ElaBitmapCallback
{
    AutoPtr<IInterface> (*createBitmap)(Int32 nativeBitmap, ArrayOf<Byte>* buffer, Boolean isMutable,
                                                        ArrayOf<Byte>* ninePatchChunk, Int32 density);
    AutoPtr<IInterface> (*createBitmap2)(Int32 nativeBitmap, ArrayOf<Byte>* buffer, Boolean isMutable,
                                    ArrayOf<Byte>* ninePatchChunk, ArrayOf<Int32>* layoutbounds, Int32 density);
};

CarClass(CBitmap)
    , public Object
    , public IBitmap
    , public IParcelable
{
public:
    CAR_INTERFACE_DECL();

    CAR_OBJECT_DECL();

    CBitmap();

    ~CBitmap();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Boolean isMutable,
        /* [in] */ ArrayOf<Byte>* ninePatchChunk,
        /* [in] */ Int32 density);

    CARAPI constructor(
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Boolean isMutable,
        /* [in] */ ArrayOf<Byte>* ninePatchChunk,
        /* [in] */ ArrayOf<Int32>* layoutBounds,
        /* [in] */ Int32 density);

    static CARAPI_(void) SetDefaultDensity(
        /* [in] */ Int32 density);

    static CARAPI_(Int32) GetDefaultDensity();

    /**
     * <p>Returns the density for this bitmap.</p>
     *
     * <p>The default density is the same density as the current display,
     * unless the current application does not support different screen
     * densities in which case it is
     * {@link android.util.DisplayMetrics#DENSITY_DEFAULT}.  Note that
     * compatibility mode is determined by the application that was initially
     * loaded into a process -- applications that share the same process should
     * all have the same compatibility, or ensure they explicitly set the
     * density of their bitmaps appropriately.</p>
     *
     * @return A scaling factor of the default density or {@link #DENSITY_NONE}
     *         if the scaling factor is unknown.
     *
     * @see #setDensity(int)
     * @see android.util.DisplayMetrics#DENSITY_DEFAULT
     * @see android.util.DisplayMetrics#densityDpi
     * @see #DENSITY_NONE
     */
    CARAPI GetDensity(
        /* [out] */ Int32* density);

    /**
     * <p>Specifies the density for this bitmap.  When the bitmap is
     * drawn to a Canvas that also has a density, it will be scaled
     * appropriately.</p>
     *
     * @param density The density scaling factor to use with this bitmap or
     *        {@link #DENSITY_NONE} if the density is unknown.
     *
     * @see #getDensity()
     * @see android.util.DisplayMetrics#DENSITY_DEFAULT
     * @see android.util.DisplayMetrics#densityDpi
     * @see #DENSITY_NONE
     */
    CARAPI SetDensity(
        /* [in] */ Int32 density);

    CARAPI SetNinePatchChunk(
        /* [in] */ ArrayOf<Byte>* chunk);

    /**
     * Sets the layout bounds as an array of left, top, right, bottom integers
     * @param bounds the array containing the padding values
     *
     * @hide
     */
    CARAPI SetLayoutBounds(
        /* [in] */ ArrayOf<Int32>* bounds);

    /**
     * Free the native object associated with this bitmap, and clear the
     * reference to the pixel data. This will not free the pixel data synchronously;
     * it simply allows it to be garbage collected if there are no other references.
     * The bitmap is marked as "dead", meaning it will throw an exception if
     * getPixels() or setPixels() is called, and will draw nothing. This operation
     * cannot be reversed, so it should only be called if you are sure there are no
     * further uses for the bitmap. This is an advanced call, and normally need
     * not be called, since the normal GC process will free up this memory when
     * there are no more references to this bitmap.
     */
    CARAPI Recycle();

    CARAPI IsRecycled(
        /* [out] */ Boolean* isRecycled);

    /**
     * Returns the generation ID of this bitmap. The generation ID changes
     * whenever the bitmap is modified. This can be used as an efficient way to
     * check if a bitmap has changed.
     *
     * @return The current generation ID for this bitmap.
     */
    CARAPI GetGenerationId(
        /* [out] */ Int32* id);

    /**
     * <p>Copy the bitmap's pixels into the specified buffer (allocated by the
     * caller). An exception is thrown if the buffer is not large enough to
     * hold all of the pixels (taking into account the number of bytes per
     * pixel) or if the Buffer subclass is not one of the support types
     * (ByteBuffer, ShortBuffer, IntBuffer).</p>
     * <p>The content of the bitmap is copied into the buffer as-is. This means
     * that if this bitmap stores its pixels pre-multiplied
     * (see {@link #isPremultiplied()}, the values in the buffer will also be
     * pre-multiplied.</p>
     */
    CARAPI CopyPixelsToBuffer(
        /* [in] */ IBuffer* dst);

    CARAPI CopyPixelsFromBuffer(
        /* [in] */ IBuffer* src);

    CARAPI Copy(
        /* [in] */ BitmapConfig config,
        /* [in] */ Boolean isMutable,
        /* [out] */ IBitmap** bitmap);

    /**
     * Creates a new bitmap, scaled from an existing bitmap, when possible. If the
     * specified width and height are the same as the current width and height of
     * the source btimap, the source bitmap is returned and now new bitmap is
     * created.
     *
     * @param src       The source bitmap.
     * @param dstWidth  The new bitmap's desired width.
     * @param dstHeight The new bitmap's desired height.
     * @param filter    true if the source should be filtered.
     * @return The new scaled bitmap or the source bitmap if no scaling is required.
     */
    static CARAPI CreateScaledBitmap(
        /* [in] */ IBitmap* src,
        /* [in] */ Int32 dstWidth,
        /* [in] */ Int32 dstHeight,
        /* [in] */ Boolean filter,
        /* [out] */ IBitmap** bitmap);

    /**
     * Returns an immutable bitmap from the source bitmap. The new bitmap may
     * be the same object as source, or a copy may have been made.  It is
     * initialized with the same density as the original bitmap.
     */
    static CARAPI CreateBitmap(
        /* [in] */ IBitmap* src,
        /* [out] */ IBitmap** bitmap);

    /**
     * Returns an immutable bitmap from the specified subset of the source
     * bitmap. The new bitmap may be the same object as source, or a copy may
     * have been made. It is initialized with the same density as the original
     * bitmap.
     *
     * @param source   The bitmap we are subsetting
     * @param x        The x coordinate of the first pixel in source
     * @param y        The y coordinate of the first pixel in source
     * @param width    The number of pixels in each row
     * @param height   The number of rows
     * @return A copy of a subset of the source bitmap or the source bitmap itself.
     */
    static CARAPI CreateBitmap(
        /* [in] */ IBitmap* source,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [out] */ IBitmap** bitmap);

    /**
     * Returns an immutable bitmap from subset of the source bitmap,
     * transformed by the optional matrix. The new bitmap may be the
     * same object as source, or a copy may have been made. It is
     * initialized with the same density as the original bitmap.
     *
     * If the source bitmap is immutable and the requested subset is the
     * same as the source bitmap itself, then the source bitmap is
     * returned and no new bitmap is created.
     *
     * @param source   The bitmap we are subsetting
     * @param x        The x coordinate of the first pixel in source
     * @param y        The y coordinate of the first pixel in source
     * @param width    The number of pixels in each row
     * @param height   The number of rows
     * @param m        Optional matrix to be applied to the pixels
     * @param filter   true if the source should be filtered.
     *                   Only applies if the matrix contains more than just
     *                   translation.
     * @return A bitmap that represents the specified subset of source
     * @throws IllegalArgumentException if the x, y, width, height values are
     *         outside of the dimensions of the source bitmap.
     */
    static CARAPI CreateBitmap(
        /* [in] */ IBitmap* source,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ IMatrix* m,
        /* [in] */ Boolean filter,
        /* [out] */ IBitmap** bitmap);

    /**
     * Returns a mutable bitmap with the specified width and height.  Its
     * initial density is as per {@link #getDensity}.
     *
     * @param width    The width of the bitmap
     * @param height   The height of the bitmap
     * @param config   The bitmap config to create.
     * @throws IllegalArgumentException if the width or height are <= 0
     */
    static CARAPI CreateBitmap(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ BitmapConfig config,
        /* [out] */ IBitmap** bitmap);

    /**
     * Returns a mutable bitmap with the specified width and height.  Its
     * initial density is determined from the given {@link DisplayMetrics}.
     *
     * @param display  Display metrics for the display this bitmap will be
     *                 drawn on.
     * @param width    The width of the bitmap
     * @param height   The height of the bitmap
     * @param config   The bitmap config to create.
     * @throws IllegalArgumentException if the width or height are <= 0
     */
    static CARAPI CreateBitmap(
        /* [in] */ IDisplayMetrics* display,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ BitmapConfig config,
        /* [out] */ IBitmap** bitmap);

    /**
     * Returns a immutable bitmap with the specified width and height, with each
     * pixel value set to the corresponding value in the colors array.  Its
     * initial density is as per {@link #getDensity}.
     *
     * @param colors   Array of {@link Color} used to initialize the pixels.
     * @param offset   Number of values to skip before the first color in the
     *                 array of colors.
     * @param stride   Number of colors in the array between rows (must be >=
     *                 width or <= -width).
     * @param width    The width of the bitmap
     * @param height   The height of the bitmap
     * @param config   The bitmap config to create. If the config does not
     *                 support per-pixel alpha (e.g. RGB_565), then the alpha
     *                 bytes in the colors[] will be ignored (assumed to be FF)
     * @throws IllegalArgumentException if the width or height are <= 0, or if
     *         the color array's length is less than the number of pixels.
     */
    static CARAPI CreateBitmap(
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ BitmapConfig config,
        /* [out] */ IBitmap** bitmap);

    /**
     * Returns a immutable bitmap with the specified width and height, with each
     * pixel value set to the corresponding value in the colors array.  Its
     * initial density is determined from the given {@link DisplayMetrics}.
     *
     * @param display  Display metrics for the display this bitmap will be
     *                 drawn on.
     * @param colors   Array of {@link Color} used to initialize the pixels.
     * @param offset   Number of values to skip before the first color in the
     *                 array of colors.
     * @param stride   Number of colors in the array between rows (must be >=
     *                 width or <= -width).
     * @param width    The width of the bitmap
     * @param height   The height of the bitmap
     * @param config   The bitmap config to create. If the config does not
     *                 support per-pixel alpha (e.g. RGB_565), then the alpha
     *                 bytes in the colors[] will be ignored (assumed to be FF)
     * @throws IllegalArgumentException if the width or height are <= 0, or if
     *         the color array's length is less than the number of pixels.
     */
    static CARAPI CreateBitmap(
        /* [in] */ IDisplayMetrics* display,
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ BitmapConfig config,
        /* [out] */ IBitmap** bitmap);

    /**
     * Returns a immutable bitmap with the specified width and height, with each
     * pixel value set to the corresponding value in the colors array.  Its
     * initial density is as per {@link #getDensity}.
     *
     * @param colors   Array of {@link Color} used to initialize the pixels.
     *                 This array must be at least as large as width * height.
     * @param width    The width of the bitmap
     * @param height   The height of the bitmap
     * @param config   The bitmap config to create. If the config does not
     *                 support per-pixel alpha (e.g. RGB_565), then the alpha
     *                 bytes in the colors[] will be ignored (assumed to be FF)
     * @throws IllegalArgumentException if the width or height are <= 0, or if
     *         the color array's length is less than the number of pixels.
     */
    static CARAPI CreateBitmap(
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ BitmapConfig config,
        /* [out] */ IBitmap** bitmap);

    /**
     * Returns a immutable bitmap with the specified width and height, with each
     * pixel value set to the corresponding value in the colors array.  Its
     * initial density is determined from the given {@link DisplayMetrics}.
     *
     * @param display  Display metrics for the display this bitmap will be
     *                 drawn on.
     * @param colors   Array of {@link Color} used to initialize the pixels.
     *                 This array must be at least as large as width * height.
     * @param width    The width of the bitmap
     * @param height   The height of the bitmap
     * @param config   The bitmap config to create. If the config does not
     *                 support per-pixel alpha (e.g. RGB_565), then the alpha
     *                 bytes in the colors[] will be ignored (assumed to be FF)
     * @throws IllegalArgumentException if the width or height are <= 0, or if
     *         the color array's length is less than the number of pixels.
     */
    static CARAPI CreateBitmap(
        /* [in] */ IDisplayMetrics* display,
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ BitmapConfig config,
        /* [out] */ IBitmap** bitmap);

    CARAPI GetNinePatchChunk(
        /* [out, callee] */ ArrayOf<Byte>** data);

    /**
     * @hide
     * @return the layout padding [left, right, top, bottom]
     */
    CARAPI GetLayoutBounds(
        /* [out, callee] */ ArrayOf<Int32>** bounds);

    CARAPI Compress(
        /* [in] */ BitmapCompressFormat format,
        /* [in] */ Int32 quality,
        /* [in] */ IOutputStream* stream,
        /* [out] */ Boolean* isSucceeded);

    CARAPI IsMutable(
        /* [out] */ Boolean* isMutable);

    /**
     * <p>Indicates whether pixels stored in this bitmaps are stored pre-multiplied.
     * When a pixel is pre-multiplied, the RGB components have been multiplied by
     * the alpha component. For instance, if the original color is a 50%
     * translucent red <code>(128, 255, 0, 0)</code>, the pre-multiplied form is
     * <code>(128, 128, 0, 0)</code>.</p>
     *
     * <p>This method always returns false if {@link #getConfig()} is
     * {@link Bitmap.Config#RGB_565}.</p>
     *
     * <p>This method only returns true if {@link #hasAlpha()} returns true.
     * A bitmap with no alpha channel can be used both as a pre-multiplied and
     * as a non pre-multiplied bitmap.</p>
     *
     * @return true if the underlying pixels have been pre-multiplied, false
     *         otherwise
     */
     CARAPI IsPremultiplied(
         /* [out] */ Boolean* isPremultiplied);

    CARAPI GetWidth(
        /* [out] */ Int32* width);

    CARAPI GetHeight(
        /* [out] */ Int32* height);

    CARAPI GetScaledWidth(
        /* [in] */ ICanvas* canvas,
        /* [out] */ Int32* width);

    CARAPI GetScaledHeight(
        /* [in] */ ICanvas* canvas,
        /* [out] */ Int32* height);

    CARAPI GetScaledWidth(
        /* [in] */ IDisplayMetrics* metrics,
        /* [out] */ Int32* width);

    CARAPI GetScaledHeight(
        /* [in] */ IDisplayMetrics* metrics,
        /* [out] */ Int32* height);

    CARAPI GetScaledWidth(
        /* [in] */ Int32 targetDensity,
        /* [out] */ Int32* width);

    CARAPI GetScaledHeight(
        /* [in] */ Int32 targetDensity,
        /* [out] */ Int32* height);

    static CARAPI_(Int32) ScaleFromDensity(
        /* [in] */ Int32 size,
        /* [in] */ Int32 sdensity,
        /* [in] */ Int32 tdensity);

    CARAPI GetRowBytes(
        /* [out] */ Int32* bytes);

    /**
     * Returns the number of bytes used to store this bitmap's pixels.
     */
    CARAPI GetByteCount(
        /* [out] */ Int32* bytes);

    CARAPI GetConfig(
        /* [out] */ BitmapConfig* config);

    CARAPI HasAlpha(
        /* [out] */ Boolean* hasAlpha);

    CARAPI SetHasAlpha(
        /* [in] */ Boolean hasAlpha);

    /**
     * Indicates whether the renderer responsible for drawing this
     * bitmap should attempt to use mipmaps when this bitmap is drawn
     * scaled down.
     *
     * If you know that you are going to draw this bitmap at less than
     * 50% of its original size, you may be able to obtain a higher
     * quality
     *
     * This property is only a suggestion that can be ignored by the
     * renderer. It is not guaranteed to have any effect.
     *
     * @return true if the renderer should attempt to use mipmaps,
     *         false otherwise
     *
     * @see #setHasMipMap(boolean)
     */
    CARAPI HasMipMap(
        /* [out] */ Boolean* hasMipMap);

    /**
     * Set a hint for the renderer responsible for drawing this bitmap
     * indicating that it should attempt to use mipmaps when this bitmap
     * is drawn scaled down.
     *
     * If you know that you are going to draw this bitmap at less than
     * 50% of its original size, you may be able to obtain a higher
     * quality by turning this property on.
     *
     * Note that if the renderer respects this hint it might have to
     * allocate extra memory to hold the mipmap levels for this bitmap.
     *
     * This property is only a suggestion that can be ignored by the
     * renderer. It is not guaranteed to have any effect.
     *
     * @param hasMipMap indicates whether the renderer should attempt
     *                  to use mipmaps
     *
     * @see #hasMipMap()
     */
    CARAPI SetHasMipMap(
        /* [in] */ Boolean hasMipMap);


    CARAPI EraseColor(
        /* [in] */ Int32 c);

    /**
     * Returns the {@link Color} at the specified location. Throws an exception
     * if x or y are out of bounds (negative or >= to the width or height
     * respectively). The returned color is a non-premultiplied ARGB value.
     *
     * @param x    The x coordinate (0...width-1) of the pixel to return
     * @param y    The y coordinate (0...height-1) of the pixel to return
     * @return     The argb {@link Color} at the specified coordinate
     * @throws IllegalArgumentException if x, y exceed the bitmap's bounds
     */
    CARAPI GetPixel(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [out] */ Int32* pixel);

    /**
     * Returns in pixels[] a copy of the data in the bitmap. Each value is
     * a packed int representing a {@link Color}. The stride parameter allows
     * the caller to allow for gaps in the returned pixels array between
     * rows. For normal packed results, just pass width for the stride value.
     * The returned colors are non-premultiplied ARGB values.
     *
     * @param pixels   The array to receive the bitmap's colors
     * @param offset   The first index to write into pixels[]
     * @param stride   The number of entries in pixels[] to skip between
     *                 rows (must be >= bitmap's width). Can be negative.
     * @param x        The x coordinate of the first pixel to read from
     *                 the bitmap
     * @param y        The y coordinate of the first pixel to read from
     *                 the bitmap
     * @param width    The number of pixels to read from each row
     * @param height   The number of rows to read
     *
     * @throws IllegalArgumentException if x, y, width, height exceed the
     *         bounds of the bitmap, or if abs(stride) < width.
     * @throws ArrayIndexOutOfBoundsException if the pixels array is too small
     *         to receive the specified number of pixels.
     */
    CARAPI GetPixels(
        /* [in] */ const ArrayOf<Int32>& pixels,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI SetPixel(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 color);

    /**
     * <p>Replace pixels in the bitmap with the colors in the array. Each element
     * in the array is a packed int prepresenting a non-premultiplied ARGB
     * {@link Color}.</p>
     *
     * @param pixels   The colors to write to the bitmap
     * @param offset   The index of the first color to read from pixels[]
     * @param stride   The number of colors in pixels[] to skip between rows.
     *                 Normally this value will be the same as the width of
     *                 the bitmap, but it can be larger (or negative).
     * @param x        The x coordinate of the first pixel to write to in
     *                 the bitmap.
     * @param y        The y coordinate of the first pixel to write to in
     *                 the bitmap.
     * @param width    The number of colors to copy from pixels[] per row
     * @param height   The number of rows to write to the bitmap
     *
     * @throws IllegalStateException if the bitmap is not mutable
     * @throws IllegalArgumentException if x, y, width, height are outside of
     *         the bitmap's bounds.
     * @throws ArrayIndexOutOfBoundsException if the pixels array is too small
     *         to receive the specified number of pixels.
     */
    CARAPI SetPixels(
        /* [in] */ const ArrayOf<Int32>& pixels,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI ExtractAlpha(
        /* [out] */ IBitmap** bitmap);

    CARAPI ExtractAlpha(
        /* [in] */ IPaint* paint,
        /* [in] */ ArrayOf<Int32>* offsetXY,
        /* [out] */ IBitmap** bitmap);

    /**
     *  Given another bitmap, return true if it has the same dimensions, config,
     *  and pixel data as this bitmap. If any of those differ, return false.
     *  If other is null, return false.
     */
    CARAPI SameAs(
        /* [in] */ IBitmap* other,
        /* [out] */ Boolean* isSame);

    CARAPI PrepareToDraw();

    CARAPI GetNativeBitmap(
        /* [out] */ Handle32* bitmap);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI_(Int32) Ni();

    static AutoPtr<IInterface> CreateBitmap(
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Boolean isMutable,
        /* [in] */ ArrayOf<Byte>* ninePatchChunk,
        /* [in] */ Int32 density);

    static AutoPtr<IInterface> CreateBitmap(
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ ArrayOf<Byte>* buffer,
        /* [in] */ Boolean isMutable,
        /* [in] */ ArrayOf<Byte>* ninePatchChunk,
        /* [in] */ ArrayOf<Int32>* layoutbounds,
        /* [in] */ Int32 density);

    static CARAPI_(BitmapConfig) NativeToConfig(
        /* [in] */ Int32 ni);

private:
    /**
     * This is called by methods that want to throw an exception if the bitmap
     * has already been recycled.
     */
    CARAPI CheckRecycled(
        /* [in] */ CString errorMessage);

    static CARAPI CheckXYSign(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    static CARAPI CheckWidthHeight(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    /**
     * Returns a mutable bitmap with the specified width and height.  Its
     * initial density is as per {@link #getDensity}.
     *
     * @param width    The width of the bitmap
     * @param height   The height of the bitmap
     * @param config   The bitmap config to create.
     * @param hasAlpha If the bitmap is ARGB_8888 this flag can be used to mark the
     *                 bitmap as opaque. Doing so will clear the bitmap in black
     *                 instead of transparent.
     *
     * @throws IllegalArgumentException if the width or height are <= 0
     */
    static CARAPI CreateBitmap(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ BitmapConfig config,
        /* [in] */ Boolean hasAlpha,
        /* [out] */ IBitmap** bitmp);

    /**
     * Returns a mutable bitmap with the specified width and height.  Its
     * initial density is determined from the given {@link DisplayMetrics}.
     *
     * @param display  Display metrics for the display this bitmap will be
     *                 drawn on.
     * @param width    The width of the bitmap
     * @param height   The height of the bitmap
     * @param config   The bitmap config to create.
     * @param hasAlpha If the bitmap is ARGB_8888 this flag can be used to mark the
     *                 bitmap as opaque. Doing so will clear the bitmap in black
     *                 instead of transparent.
     *
     * @throws IllegalArgumentException if the width or height are <= 0
     */
    static CARAPI CreateBitmap(
        /* [in] */ IDisplayMetrics* display,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ BitmapConfig config,
        /* [in] */ Boolean hasAlpha,
        /* [out] */ IBitmap** bitmp);

    CARAPI CheckPixelAccess(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    CARAPI CheckPixelsAccess(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ const ArrayOf<Int32>& pixels);

    static CARAPI NativeCreate(
        /* [in] */ const ArrayOf<Int32>* colors,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height,
        /* [in] */ Int32 nativeConfig,
        /* [in] */ Boolean isMutable,
        /* [out] */ CBitmap** bitmap);

    static CARAPI NativeCopy(
        /* [in] */ Int32 srcBitmap,
        /* [in] */ Int32 nativeConfig,
        /* [in] */ Boolean isMutable,
        /* [out] */ CBitmap** bitmap);

    static CARAPI_(void) NativeDestructor(
        /* [in] */ Int32 nativeBitmap);

    static CARAPI_(Boolean) NativeRecycle(
        /* [in] */ Int32 nativeBitmap);

    static CARAPI_(Boolean) NativeCompress(
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ Int32 format,
        /* [in] */ Int32 quality,
        /* [in] */ IOutputStream* stream,
        /* [in] */ ArrayOf<Byte>* tempStorage);

    static CARAPI_(void) NativeErase(
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ Int32 color);

    static CARAPI_(Int32) NativeWidth(
        /* [in] */ Int32 nativeBitmap);

    static CARAPI_(Int32) NativeHeight(
        /* [in] */ Int32 nativeBitmap);

    static CARAPI_(Int32) NativeRowBytes(
        /* [in] */ Int32 nativeBitmap);

    static CARAPI_(Int32) NativeConfig(
        /* [in] */ Int32 nativeBitmap);

    static CARAPI_(Int32) NativeGetPixel(
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y);

    static CARAPI_(void) NativeGetPixels(
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ const ArrayOf<Int32>& pixels,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    static CARAPI_(void) NativeSetPixel(
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 color);

    static CARAPI_(void) NativeSetPixels(
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ const ArrayOf<Int32>& colors,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 stride,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    static CARAPI_(void) NativeCopyPixelsToBuffer(
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ IBuffer* dst);

    static CARAPI_(void) NativeCopyPixelsFromBuffer(
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ IBuffer* src);

    static CARAPI_(Int32) NativeGenerationId(
        /* [in] */ Int32 nativeBitmap);

    static CARAPI NativeCreateFromParcel(
        /* [in] */ IParcel* p,
        /* [in] */ CBitmap* bitmap);

    static CARAPI_(Boolean) NativeWriteToParcel(
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ Boolean isMutable,
        /* [in] */ Int32 density,
        /* [in] */ IParcel* p);

    static CARAPI NativeExtractAlpha(
        /* [in] */ Int32 nativeBitmap,
        /* [in] */ Int32 nativePaint,
        /* [in] */ ArrayOf<Int32>* offsetXY,
        /* [out] */ CBitmap** bitmap);

    static CARAPI_(void) NativePrepareToDraw(
        /* [in] */ Int32 nativeBitmap);

    static CARAPI_(Boolean) NativeHasAlpha(
        /* [in] */ Int32 nativeBitmap);

    static CARAPI_(void) NativeSetHasAlpha(
        /* [in] */ Int32 bitmap,
        /* [in] */ Boolean hasAlpha);

    static CARAPI_(Boolean) NativeHasMipMap(
        /* [in] */ Int32 nativeBitmap);

    static CARAPI_(void) NativeSetHasMipMap(
        /* [in] */ Int32 bitmap,
        /* [in] */ Boolean hasMipMap);

    static CARAPI_(Boolean) NativeSameAs(
        /* [in] */ Int32 nb0,
        /* [in] */ Int32 nb1);

public:
    /**
     * Indicates that the bitmap was created for an unknown pixel density.
     *
     * @see Bitmap#getDensity()
     * @see Bitmap#setDensity(int)
     */
    static const Int32 DENSITY_NONE = 0;

    /**
     * Note:  mNativeBitmap is used by FaceDetector_jni.cpp
     * Don't change/rename without updating FaceDetector_jni.cpp
     *
     * @hide
     */
    Int32 mNativeBitmap;

    /**
     * Backing buffer for the Bitmap.
     * Made public for quick access from drawing methods -- do NOT modify
     * from outside this class.
     *
     * @hide
     */
    AutoPtr< ArrayOf<Byte> > mBuffer;

    // Package-scoped for fast access.
    Int32           mDensity;

private:
    Boolean         mIsMutable;
    AutoPtr< ArrayOf<Byte> >  mNinePatchChunk;   // may be null
    AutoPtr< ArrayOf<Int32> >  mLayoutBounds;   // may be null
    Int32           mWidth;
    Int32           mHeight;
    Boolean         mRecycled;

    static AutoPtr<IMatrix> sScaleMatrix;

    static Int32 sDefaultDensity;

    /**
     * Number of bytes of temp storage we use for communicating between the
     * native compressor and the java OutputStream.
     */
    static Int32 WORKING_COMPRESS_STORAGE;

    static Object sClassLock;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_CBITMAP_H__

