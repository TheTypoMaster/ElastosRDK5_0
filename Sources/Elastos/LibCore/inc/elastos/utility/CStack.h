#ifndef __UTILITY_CSTACK_H__
#define __UTILITY_CSTACK_H__

#include "_CStack.h"
#include "Vector.h"

namespace Elastos {
namespace Utility {

CarClass(CStack), public _Vector
{
public:
    CARAPI constructor();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI Add(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* object);

    CARAPI Add(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* modified);

    CARAPI AddAll(
        /* [in] */ Int32 location,
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI AddAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI AddElement(
        /* [in] */ IInterface* object);

    CARAPI Capacity(
        /* [out] */ Int32* value);

    CARAPI Clear();

    CARAPI Clone(
        /* [out] */ IInterface** object);

    CARAPI Contains(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI ContainsAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* result);

    CARAPI CopyInto(
        /* [in] */ ArrayOf<IInterface*>* elements);

    CARAPI ElementAt(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** outface);

    CARAPI GetElements(
        /* [out] */ IEnumeration** enu);

    CARAPI EnsureCapacity(
        /* [in] */ Int32 minimumCapacity);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI FirstElement(
        /* [out] */ IInterface** outface);

    CARAPI Get(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** object);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI IndexOf(
        /* [in] */ IInterface* object,
        /* [out] */ Int32* index);

    CARAPI IndexOf(
        /* [in] */ IInterface* object,
        /* [in] */ Int32 location,
        /* [out] */ Int32* value);

    CARAPI InsertElementAt(
        /* [in] */ IInterface* object,
        /* [in] */ Int32 location);

    CARAPI IsEmpty(
        /* [out] */ Boolean* result);

    CARAPI LastElement(
        /* [out] */ IInterface** outface);

    CARAPI LastIndexOf(
        /* [in] */ IInterface* object,
        /* [out] */ Int32* index);

    CARAPI LastIndexOf(
        /* [in] */ IInterface* object,
        /* [in] */ Int32 location,
        /* [out] */ Int32* value);

    CARAPI Remove(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** object);

    CARAPI Remove(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* modified);

    CARAPI RemoveAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI RemoveAllElements();

    CARAPI RemoveElement(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* value);

    CARAPI RemoveElementAt(
        /* [in] */ Int32 location);

    CARAPI RetainAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI Set(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* object,
        /* [out] */ IInterface** prevObject);

    CARAPI SetElementAt(
        /* [in] */ IInterface* object,
        /* [in] */ Int32 location);

    CARAPI SetSize(
        /* [in] */ Int32 length);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI GetSubList(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [out] */ IList** subList);

    CARAPI ToArray(
        /* [out, callee] */ ArrayOf<IInterface*>** array);

    CARAPI ToArray(
        /* [in] */ ArrayOf<IInterface*>* inArray,
        /* [out, callee] */ ArrayOf<IInterface*>** outArray);

    CARAPI ToString(
        /* [out] */ String* result);

    CARAPI TrimToSize();

    CARAPI GetIterator(
        /* [out] */ IIterator** it);

    CARAPI GetListIterator(
        /* [out] */ IListIterator** it);

    CARAPI GetListIterator(
        /* [in] */ Int32 location,
        /* [out] */ IListIterator** it);

    /**
     * Returns whether the stack is empty or not.
     *
     * @return {@code true} if the stack is empty, {@code false} otherwise.
     */
    CARAPI Empty(
        /* [out] */ Boolean* value);

    /**
     * Returns the element at the top of the stack without removing it.
     *
     * @return the element at the top of the stack.
     * @throws EmptyStackException
     *             if the stack is empty.
     * @see #pop
     */
    // @SuppressWarnings("unchecked")
    CARAPI Peek(
        /* [out] */ IInterface** outface);

    /**
     * Returns the element at the top of the stack and removes it.
     *
     * @return the element at the top of the stack.
     * @throws EmptyStackException
     *             if the stack is empty.
     * @see #peek
     * @see #push
     */
    // @SuppressWarnings("unchecked")
    CARAPI Pop(
        /* [out] */ IInterface** outface);

    /**
     * Pushes the specified object onto the top of the stack.
     *
     * @param object
     *            The object to be added on top of the stack.
     * @return the object argument.
     * @see #peek
     * @see #pop
     */
    CARAPI Push(
        /* [in] */ IInterface* object);

    /**
     * Returns the index of the first occurrence of the object, starting from
     * the top of the stack.
     *
     * @return the index of the first occurrence of the object, assuming that
     *         the topmost object on the stack has a distance of one.
     * @param o
     *            the object to be searched.
     */
    CARAPI Search(
        /* [in] */ IInterface* o,
        /* [out] */ Int32* value);

protected:
    CARAPI_(Mutex*) GetSelfLock();
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_CSTACK_H__