#ifndef __ELASTOS_DROID_GESTURE_CGESTUREPOINT_H__
#define __ELASTOS_DROID_GESTURE_CGESTUREPOINT_H__

#include "_Elastos_Droid_Gesture_CGesturePoint.h"

using Elastos::IO::IDataInputStream;

namespace Elastos {
namespace Droid {
namespace Gesture {

CarClass(CGesturePoint)
{
public:
    CGesturePoint()
        : mX(0)
        , mY(0)
        , mTimestamp(0)
    {}

    CARAPI constructor(
        /* [in] */ Float x,
        /* [in] */ Float y,
        /* [in] */ Int64 t);

    CARAPI GetX(
        /* [out] */ Float * x);

    CARAPI GetY(
        /* [out] */ Float * y);

    CARAPI GetTimestamp(
        /* [out] */ Int64 * timestamp);

    static CARAPI Deserialize(
        /* [in] */ IDataInputStream *in,
        /* [out] */ IGesturePoint **instance);

public:
    Float mX;
    Float mY;

    Int64 mTimestamp;
};

} // namespace Gesture
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_GESTURE_CGESTUREPOINT_H__
