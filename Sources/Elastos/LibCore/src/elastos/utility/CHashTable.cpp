
#include "CHashTable.h"

namespace Elastos {
namespace Utility {

ECode CHashTable::constructor()
{
    return HashTable::Init();
}

ECode CHashTable::constructor(
    /* [in] */ Int32 capacity)
{
    return HashTable::Init(capacity);
}

ECode CHashTable::constructor(
    /* [in] */ Int32 capacity,
    /* [in] */ Float loadFactor)
{
    return HashTable::Init(capacity, loadFactor);
}

ECode CHashTable::constructor(
    /* [in] */ IMap* map)
{
    return HashTable::Init(map);
}

ECode CHashTable::Clear()
{
    return HashTable::Clear();
}

ECode CHashTable::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    return HashTable::ContainsKey(key, result);
}

ECode CHashTable::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    return HashTable::ContainsValue(value, result);
}

ECode CHashTable::GetEntrySet(
    /* [out] */ ISet** entries)
{
    return HashTable::GetEntrySet(entries);
}

ECode CHashTable::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return HashTable::Equals(object, result);
}

ECode CHashTable::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return HashTable::Get(key, value);
}

ECode CHashTable::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return HashTable::GetHashCode(hashCode);
}

ECode CHashTable::IsEmpty(
    /* [out] */ Boolean* result)
{
    return HashTable::IsEmpty(result);
}

ECode CHashTable::GetKeySet(
    /* [out] */ ISet** keySet)
{
    return HashTable::GetKeySet(keySet);
}

ECode CHashTable::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return HashTable::Put(key, value, oldValue);
}

ECode CHashTable::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode CHashTable::PutAll(
    /* [in] */ IMap* map)
{
    return HashTable::PutAll(map);
}

ECode CHashTable::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return HashTable::Remove(key, value);
}

ECode CHashTable::Remove(
    /* [in] */ PInterface key)
{
    assert(0 && "TODO");
    return NOERROR;
}

ECode CHashTable::GetSize(
    /* [out] */ Int32* size)
{
    return HashTable::GetSize(size);
}

ECode CHashTable::GetValues(
    /* [out] */ ICollection** value)
{
    return HashTable::GetValues(value);
}

ECode CHashTable::Clone(
    /* [out] */ IInterface** object)
{
    return HashTable::Clone(object);
}

ECode CHashTable::GetElements(
    /* [out] */ IEnumeration** enm)
{
    return HashTable::GetElements(enm);
}

ECode CHashTable::GetKeys(
    /* [out] */ IEnumeration** enm)
{
    return HashTable::GetKeys(enm);
}

} // namespace Utility
} // namespace Elastos
