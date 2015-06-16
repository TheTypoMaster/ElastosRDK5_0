#ifndef __CBASICLRUCACHE_H__
#define __CBASICLRUCACHE_H__

#include "_Libcore_Utility_CBasicLruCache.h"
#include "BasicLruCache.h"

namespace Libcore {
namespace Utility {

CarClass(CBasicLruCache)
    , public BasicLruCache
{
public:
    CAR_OBJECT_DECL()

    // /**
    //  * Returns the value for {@code key} if it exists in the cache or can be
    //  * created by {@code #create}. If a value was returned, it is moved to the
    //  * head of the queue. This returns null if a value is not cached and cannot
    //  * be created.
    //  */
    // CARAPI Get(
    //     /* [in] */ IInterface* key,
    //     /* [out] */ IInterface** ouface);

    // *
    //  * Caches {@code value} for {@code key}. The value is moved to the head of
    //  * the queue.
    //  *
    //  * @return the previous value mapped by {@code key}. Although that entry is
    //  *     no longer cached, it has not been passed to {@link #entryEvicted}.

    // CARAPI Put(
    //     /* [in] */ IInterface* key,
    //     /* [in] */ IInterface* value,
    //     /* [out] */ IInterface** outface);

    // /**
    //  * Returns a copy of the current contents of the cache, ordered from least
    //  * recently accessed to most recently accessed.
    //  */
    // CARAPI Snapshot(
    //     /* [out] */ IMap** outmap);

    // /**
    //  * Clear the cache, calling {@link #entryEvicted} on each removed entry.
    //  */
    // CARAPI EvictAll();
};

}
}

#endif