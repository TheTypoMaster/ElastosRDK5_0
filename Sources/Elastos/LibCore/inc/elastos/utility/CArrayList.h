#ifndef __ELASTOS_UTILITY_CARRAYLIST_H__
#define __ELASTOS_UTILITY_CARRAYLIST_H__

#include "_Elastos_Utility_CArrayList.h"
#include "AbstractList.h"

using Elastos::Core::ICloneable;
using Elastos::IO::IObjectOutputStream;
using Elastos::IO::IObjectInputStream;
using Elastos::IO::ISerializable;
using Elastos::Utility::AbstractList;

namespace Elastos {
namespace Utility {

CarClass(CArrayList)
    , public AbstractList
    , public IArrayList
    , public ICloneable
    , public ISerializable
    , public IRandomAccess
{
private:
    class ArrayListIterator
        : public Object
        , public IIterator
    {
    public:
        ArrayListIterator(
            /* [in] */ CArrayList* owner)
            : mRemaining(owner->mSize)
            , mRemovalIndex(-1)
            , mExpectedModCount(owner->mModCount)
            , mOwner(owner)
        {}

        CAR_INTERFACE_DECL()

        CARAPI HasNext(
            /* [out] */ Boolean* result);

        CARAPI GetNext(
            /* [out] */ IInterface** object);;

        CARAPI Remove();

    private:
        /** Number of elements remaining in this iteration */
        Int32 mRemaining;

        /** Index of element that remove() would remove, or -1 if no such elt */
        Int32 mRemovalIndex;

        /** The expected modCount value */
        Int32 mExpectedModCount;

        AutoPtr<CArrayList> mOwner;
    };

public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CArrayList();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ Int32 capacity);

    CARAPI constructor(
        /* [in] */ ICollection* collection);

    CARAPI Add(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* modified);

    CARAPI Add(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* object);

    CARAPI AddAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI AddAll(
        /* [in] */ Int32 location,
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI Clear();

    CARAPI Clone(
        /* [out] */ IInterface** object);

    CARAPI EnsureCapacity(
        /* [in] */ Int32 minimumCapacity);

    CARAPI Get(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** object);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI IsEmpty(
        /* [out] */ Boolean* result);

    CARAPI Contains(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI IndexOf(
        /* [in] */ IInterface* object,
        /* [out] */ Int32* index);

    CARAPI LastIndexOf(
        /* [in] */ IInterface* object,
        /* [out] */ Int32* index);

    CARAPI Remove(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** object);

    CARAPI Remove(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* modified);

    CARAPI Set(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* object,
        /* [out] */ IInterface** prevObject);

    CARAPI ToArray(
        /* [out, callee] */ ArrayOf<IInterface*>** array);

    CARAPI ToArray(
        /* [in] */ ArrayOf<IInterface*>* inArray,
        /* [out, callee] */ ArrayOf<IInterface*>** outArray);

    CARAPI TrimToSize();

    CARAPI GetIterator(
        /* [out] */ IIterator** it);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI ContainsAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* result);

    CARAPI RemoveAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI RetainAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI GetListIterator(
        /* [out] */ IListIterator** it);

    CARAPI GetListIterator(
        /* [in] */ Int32 location,
        /* [out] */ IListIterator** it);

    CARAPI GetSubList(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ IList** subList);

protected:
    CARAPI RemoveRange(
        /* [in] */ Int32 fromIndex,
        /* [in] */ Int32 toIndex);

private:
    static CARAPI_(Int32) NewCapacity(
        /* [in] */ Int32 currentCapacity);

    CARAPI WriteObject(
        /* [in] */ IObjectOutputStream* stream);

    CARAPI ReadObject(
        /* [in] */ IObjectInputStream* stream);

private:
    /**
     * The minimum amount by which the capacity of an ArrayList will increase.
     * This tuning parameter controls a time-space tradeoff. This value (12)
     * gives empirically good results and is arguably consistent with the。
     * RI's specified default initial capacity of 10: instead of 10, we start
     * with 0 (sans allocation) and jump to 12.
     */
    static const Int32 MIN_CAPACITY_INCREMENT;

    // static const Int64 mSerialVersionUID; // = 8683452581122892189L;

    /**
     * The number of elements in this list.
     */
    Int32 mSize;

    /**
     * The elements in this list, followed by nulls.
     */
    /* transient */ AutoPtr< ArrayOf<IInterface*> > mArray;
};

} // namespace Utility
} // namespace Elastos

#endif // __ELASTOS_UTILITY__CARRAYLIST_H__
