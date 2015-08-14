
#ifndef __ELASTOS_DROID_OS_CDEBUGMEMORYINFOHELPER_H__
#define __ELASTOS_DROID_OS_CDEBUGMEMORYINFOHELPER_H__

#include "_CDebugMemoryInfoHelper.h"

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CDebugMemoryInfoHelper)
{
public:
    CARAPI constructor();

    CARAPI GetOtherLabel(
        /* [in] */ Int32 which,
        /* [out] */ String* label);
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_OS_CDEBUGMEMORYINFOHELPER_H__
