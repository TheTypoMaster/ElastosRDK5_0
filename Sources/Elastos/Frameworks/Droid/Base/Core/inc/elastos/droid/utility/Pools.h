
#ifndef __ELASTOS_DROID_UTILITY_POOLS_H__
#define __ELASTOS_DROID_UTILITY_POOLS_H__

#include "ext/frameworkext.h"
#include <elastos/core/Object.h>
#include <elastos/core/AutoLock.h>

namespace Elastos {
namespace Droid {
namespace Utility {

class Pools
{
private:
    Pools() {}

public:
    template<typename T>
    class Pool
        : public Object
    {
    public:
        /**
         * @return An instance from the pool if such, null otherwise.
         */
        virtual AutoPtr<T> AcquireItem() = 0;

        /**
         * Release an instance to the pool.
         *
         * @param instance The instance to release.
         * @return Whether the instance was put in the pool.
         *
         * @throws IllegalStateException If the instance is already in the pool.
         */
        virtual Boolean ReleaseItem(
            /* [in] */ T* instance) = 0;
    };

    /**
     * Simple (non-synchronized) pool of objects.
     *
     * @param <T> The pooled type.
     */
    template<typename T>
    class SimplePool
        : public Pool<T>
    {
    public:
        /**
         * Creates a new instance.
         *
         * @param maxPoolSize The max pool size.
         *
         * @throws IllegalArgumentException If the max pool size is less than zero.
         */
        SimplePool(
            /* [in] */ Int32 maxPoolSize)
        {
            assert(maxPoolSize > 0);
            mPool = ArrayOf<T*>::Alloc(maxPoolSize);
        }

        AutoPtr<T> AcquireItem()
        {
            if (mPoolSize > 0) {
                Int32 lastPooledIndex = mPoolSize - 1;
                AutoPtr<T> instance = (*mPool)[lastPooledIndex];
                mPool->Set(lastPooledIndex, NULL);
                mPoolSize--;
                return instance;
            }
            return NULL;
        }

        Boolean ReleaseItem(
            /* [in] */ T* instance)
        {
            if (IsInPool(instance)) {
                //throw new IllegalStateException("Already in the pool!");
                return FALSE;
            }

            if (mPoolSize < mPool->GetLength()) {
                mPool->Set(mPoolSize, instance);
                mPoolSize++;
                return TRUE;
            }
            return FALSE;
        }

    private:
        Boolean IsInPool(
            /* [in] */ T* instance) {
            for (Int32 i = 0; i < mPoolSize; i++) {
                if ((*mPool)[i] == instance) {
                    return TRUE;
                }
            }
            return FALSE;
        }

    private:
        AutoPtr<ArrayOf<T*> > mPool;
        Int32 mPoolSize;
    };

    /**
     * Synchronized) pool of objects.
     *
     * @param <T> The pooled type.
     */
    template<typename T>
    class SynchronizedPool
        : public SimplePool<T>
    {
    public:
        /**
         * Creates a new instance.
         *
         * @param maxPoolSize The max pool size.
         *
         * @throws IllegalArgumentException If the max pool size is less than zero.
         */
        SynchronizedPool(
            /* [in] */ Int32 maxPoolSize)
            : SimplePool<T>(maxPoolSize)
        {}

        AutoPtr<T> AcquireItem()
        {
            AutoLock lock(mLock);
            return SimplePool<T>::AcquireItem();
        }

        Boolean ReleaseItem(
            /* [in] */ T* instance)
        {
            AutoLock lock(mLock);
            return SimplePool<T>::ReleaseItem(instance);
        }

    private:
        Object mLock;
    };
};

} // namespace Utility
} // namepsace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_UTILITY_POOLS_H__
