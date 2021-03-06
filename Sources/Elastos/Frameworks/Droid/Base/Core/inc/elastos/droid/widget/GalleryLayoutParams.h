
#ifndef __ELASTOS_DROID_WIDGET_GALLERYLAYOUTPARAMS_H__
#define __ELASTOS_DROID_WIDGET_GALLERYLAYOUTPARAMS_H__

#include "view/ViewGroupLayoutParams.h"

using Elastos::Droid::View::IViewGroupLayoutParams;

namespace Elastos{
namespace Droid{
namespace Widget{

/**
 * Gallery extends LayoutParams to provide a place to hold current
 * Transformation information along with previous position/transformation
 * info.
 *
 */

class GalleryLayoutParams : public Elastos::Droid::View::ViewGroupLayoutParams
{
public:
    GalleryLayoutParams(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    GalleryLayoutParams(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    GalleryLayoutParams(
        /* [in] */ ViewGroupLayoutParams* source);

    CARAPI Init(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h);

    CARAPI Init(
        /* [in] */ IViewGroupLayoutParams* source);

protected:
    GalleryLayoutParams();
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif

