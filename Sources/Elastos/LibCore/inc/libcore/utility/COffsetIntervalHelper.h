#ifndef __COFFSETINTERVALHELPER_H__
#define __COFFSETINTERVALHELPER_H__

#include "Singleton.h"
#include "_Libcore_Utility_COffsetIntervalHelper.h"

using Elastos::Core::Singleton;

namespace Libcore {
namespace Utility {

CarClass(COffsetIntervalHelper)
    , public Singleton
    , public IOffsetIntervalHelper
{
public:
    CAR_SINGLETON_DECL()

    CAR_INTERFACE_DECL()

    CARAPI Create(
        /* [in] */ IZoneInfo* timeZone,
        /* [in] */ Int32 transitionIndex,
        /* [out] */ IOffsetInterval** result);
};

} // namespace Utility
} // namespace Libcore

#endif