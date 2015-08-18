
#ifndef __ELASTOS_DROID_GRAPHICS_DRAWABLE_CNINEPATCHDRAWABLE_H__
#define __ELASTOS_DROID_GRAPHICS_DRAWABLE_CNINEPATCHDRAWABLE_H__

#include "_Elastos_Droid_Graphics_Drawable_CNinePatchDrawable.h"
#include "graphics/drawable/NinePatchDrawable.h"


namespace Elastos {
namespace Droid {
namespace Graphics {
namespace Drawable {

CarClass(CNinePatchDrawable), NinePatchDrawable
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IBitmap* bitmap,
        /* [in] */ ArrayOf<Byte>* chunk,
        /* [in] */ IRect* padding,
        /* [in] */ const String& srcName);

    CARAPI constructor(
        /* [in] */ IResources* res,
        /* [in] */ IBitmap* bitmap,
        /* [in] */ ArrayOf<Byte>* chunk,
        /* [in] */ IRect* padding,
        /* [in] */ const String& srcName);

    CARAPI constructor(
        /* [in] */ IResources* res,
        /* [in] */ IBitmap* bitmap,
        /* [in] */ ArrayOf<Byte>* chunk,
        /* [in] */ IRect* padding,
        /* [in] */ IRect* layoutInsets,
        /* [in] */ const String& srcName);

    CARAPI constructor(
        /* [in] */ INinePatch* patch);

    CARAPI constructor(
        /* [in] */ IResources* res,
        /* [in] */ INinePatch* patch);

    CARAPI constructor(
        /* [in] */ Handle32 state,
        /* [in] */ IResources* res);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    IDRAWABLE_METHODS_DECL()

    CARAPI SetTargetDensity(
        /* [in] */ ICanvas* canvas);

    CARAPI SetTargetDensity(
        /* [in] */ IDisplayMetrics* metrics);

    CARAPI SetTargetDensity(
        /* [in] */ Int32 density);

    CARAPI GetPaint(
        /* [out] */ IPaint** paint);
};

} // namespace Drawable
} // namespace Graphics
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_GRAPHICS_DRAWABLE_CNINEPATCHDRAWABLE_H__
