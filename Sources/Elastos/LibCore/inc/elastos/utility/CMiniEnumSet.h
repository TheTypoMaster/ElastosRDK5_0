#ifndef __UTILITY_CMINIENUMSET_H__
#define __UTILITY_CMINIENUMSET_H__

#include "_Elastos_Utility_CMiniEnumSet.h"
#include "MiniEnumSet.h"

namespace Elastos {
namespace Utility {

CarClass(CMiniEnumSet)
    , public MiniEnumSet
{
public:
    CAR_OBJECT_DECL()
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_CMINIENUMSET_H__
