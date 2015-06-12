
#ifndef __ICU_CCOLLATIONKEYICU_H__
#define __ICU_CCOLLATIONKEYICU_H__

#include "_Libcore_ICU_CCollationKeyICU.h"
#include "CollationKeyICU.h"

namespace Libcore {
namespace ICU {

CarClass(CCollationKeyICU)
    , public CollationKeyICU
{
public:
    CAR_OBJECT_DECL()
};

} // namespace ICU
} // namespace Libcore

#endif // __ICU_CCOLLATIONKEYICU_H__
