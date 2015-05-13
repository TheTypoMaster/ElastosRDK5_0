
#include "CCopyOnWriteArrayListHelper.h"
#include "CopyOnWriteArrayList.h"

namespace Elastos {
namespace Utility {
namespace Concurrent {

ECode CCopyOnWriteArrayListHelper::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [in] */ const ArrayOf<IInterface*>& snapshot,
    /* [in] */ Int32 from,
    /* [in] */ Int32 to,
    /* [out] */ Boolean* value)
{
    return CopyOnWriteArrayList::ContainsAll(collection, snapshot, from, to, value);
}

ECode CCopyOnWriteArrayListHelper::LastIndexOf(
    /* [in] */ IInterface* o,
    /* [in] */ const ArrayOf<IInterface*>& data,
    /* [in] */ Int32 from,
    /* [in] */ Int32 to,
    /* [out] */ Int32* value)
{
    return CopyOnWriteArrayList::LastIndexOf(o, data, from, to, value);
}

ECode CCopyOnWriteArrayListHelper::IndexOf(
    /* [in] */ IInterface* o,
    /* [in] */ const ArrayOf<IInterface*>& data,
    /* [in] */ Int32 from,
    /* [in] */ Int32 to,
    /* [out] */ Int32* value)
{
    return CopyOnWriteArrayList::IndexOf(o, data, from, to, value);
}

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos
