
#ifndef __ELASTOS_DROID_GRAPHICS_RASTERIZER_H__
#define __ELASTOS_DROID_GRAPHICS_RASTERIZER_H__

#include <elastos.h>

using namespace Elastos;

namespace Elastos {
namespace Droid {
namespace Graphics {

extern const InterfaceID EIID_Rasterizer;

class Rasterizer
    : public Object
    , public IRasterizer
{
    friend class Paint;

public:
    ~Rasterizer();

private:
    static CARAPI_(void) Finalizer(
        /* [in] */ Int32 nativeInstance);

protected:
    Int32 mNativeInstance;
};

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GRAPHICS_RASTERIZER_H__
