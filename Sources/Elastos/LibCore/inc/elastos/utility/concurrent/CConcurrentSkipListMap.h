
#ifndef __ELASTOS_UTILITY_CCONCURRENTSKIPLISTMAP_H__
#define __ELASTOS_UTILITY_CCONCURRENTSKIPLISTMAP_H__

#include "_Elastos_Utility_Concurrent_CConcurrentSkipListMap.h"
#include "AbstractMap.h"
#include "Object.h"

using Elastos::Core::Object;
using Elastos::Core::IComparable;
using Elastos::Core::IComparator;
using Elastos::Core::IRandom;
using Elastos::IO::IObjectOutputStream;
using Elastos::IO::IObjectInputStream;

namespace Elastos {
namespace Utility {
namespace Concurrent {

CarClass(CConcurrentSkipListMap)
    , public AbstractMap
    , public IConcurrentNavigableMap
    , public INavigableMap
    , public ISortedMap
    , public IConcurrentMap
{
public:
    class Node
        : public Object
    {
    public:
        Node(
            /* [in] */ IInterface* key,
            /* [in] */ IInterface* value,
            /* [in] */ Node* next);

        Node(
            /* [in] */ Node* next);

        Boolean CasValue(
            /* [in] */ IInterface* cmp,
            /* [in] */ IInterface* val);

        Boolean CasNext(
            /* [in] */ Node* cmp,
            /* [in] */ Node* val);

        Boolean IsMarker();

        Boolean IsBaseHeader();

        Boolean AppendMarker(
            /* [in] */ Node* f);

        void HelpDelete(
            /* [in] */ Node* b,
            /* [in] */ Node* f);

        AutoPtr<IInterface> GetValidValue();

        AutoPtr<SimpleImmutableEntry> CreateSnapshot();

    public:
        AutoPtr<IInterface> mKey;
        /*volatile*/ AutoPtr<IInterface> mValue;
        /*volatile*/ AutoPtr<Node> mNext;

        // UNSAFE mechanics

    //     private static final sun.misc.Unsafe UNSAFE;
    //     private static final long valueOffset;
    //     private static final long nextOffset;

    //     static {
    //         try {
    //             UNSAFE = sun.misc.Unsafe.getUnsafe();
    //             Class<?> k = Node.class;
    //             valueOffset = UNSAFE.objectFieldOffset
    //                 (k.getDeclaredField("value"));
    //             nextOffset = UNSAFE.objectFieldOffset
    //                 (k.getDeclaredField("next"));
    //         } catch (Exception e) {
    //             throw new Error(e);
    //         }
    //     }
    };

    class Index
        : public Object
    {
    public:
        Index(
            /* [in] */ Node* node,
            /* [in] */ Index* down,
            /* [in] */ Index* right);

        Boolean CasRight(
            /* [in] */ Index* cmp,
            /* [in] */ Index* val);

        Boolean IndexesDeletedNode();

        Boolean Link(
            /* [in] */ Index* succ,
            /* [in] */ Index* newSucc);

        Boolean Unlink(
            /* [in] */ Index* succ);

        // Unsafe mechanics
        // private static final sun.misc.Unsafe UNSAFE;
        // private static final long rightOffset;
        // static {
        //     try {
        //         UNSAFE = sun.misc.Unsafe.getUnsafe();
        //         Class<?> k = Index.class;
        //         rightOffset = UNSAFE.objectFieldOffset
        //             (k.getDeclaredField("right"));
        //     } catch (Exception e) {
        //         throw new Error(e);
        //     }
        // }
    public:
        AutoPtr<Node> mNode;
        AutoPtr<Index> mDown;
        /*volatile*/ AutoPtr<Index> mRight;
    };

    /* ---------------- Head nodes -------------- */

    /**
     * Nodes heading each level keep track of their level.
     */
    class HeadIndex
        : public Index
    {
    public:
        HeadIndex(
            /* [in] */ Node* node,
            /* [in] */ Index* down,
            /* [in] */ Index* right,
            /* [in] */ Int32 level);

    public:
        Int32 mLevel;
    };

    /* ---------------- Comparison utilities -------------- */

    class ComparableUsingComparator
        : public IComparable
        , public Object
    {
    public:
        CAR_INTERFACE_DECL()

        ComparableUsingComparator(
            /* [in] */ IInterface* key,
            /* [in] */ IComparator* cmp);

        CARAPI CompareTo(
            /* [in] */ IInterface* another,
            /* [out] */ Int32* result);

    public:
        AutoPtr<IInterface> mActualKey;
        AutoPtr<IComparator> mCmp;
    };

        /* ---------------- Iterators -------------- */

    class Iter
        : public IIterator
        , public Object
    {
    public:
        CAR_INTERFACE_DECL();

        Iter(
            /* [in] */ CConcurrentSkipListMap* owner);

        CARAPI HasNext(
            /* [out] */ Boolean* result);

        CARAPI Advance();

        CARAPI Remove();

    public:
        /** the last node returned by next() */
        AutoPtr<Node> mLastReturned;
        /** the next node to return from next(); */
        AutoPtr<Node> mNext;
        /** Cache of next value field to maintain weak consistency */
        AutoPtr<IInterface> mNextValue;

        CConcurrentSkipListMap* mOwner;
    };

    class _ValueIterator
        : public Iter
    {
    public:
        _ValueIterator(
            /* [in] */ CConcurrentSkipListMap* owner);

        CARAPI GetNext(
            /* [out] */ IInterface** object);
    };

    class _KeyIterator
        : public Iter
    {
    public:
        _KeyIterator(
            /* [in] */ CConcurrentSkipListMap* owner);

        CARAPI GetNext(
            /* [out] */ IInterface** object);
    };

    class _EntryIterator
        : public Iter
    {
    public:
        _EntryIterator(
            /* [in] */ CConcurrentSkipListMap* owner);

        CARAPI GetNext(
            /* [out] */ IInterface** object);
    };

    class _KeySet
        : public AbstractSet
        , public INavigableSet
        , public ISortedSet
    {
    public:
        CAR_INTERFACE_DECL();

        _KeySet(
            /* [in] */ INavigableMap* map);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI Clear();

        CARAPI Lower(
            /* [in] */ IInterface* e,
            /* [out] */ IInterface** outface);

        CARAPI Floor(
            /* [in] */ IInterface* e,
            /* [out] */ IInterface** outface);

        CARAPI Ceiling(
            /* [in] */ IInterface* e,
            /* [out] */ IInterface** outface);

        CARAPI Higher(
            /* [in] */ IInterface* e,
            /* [out] */ IInterface** outface);

        CARAPI Comparator(
            /* [out] */ IComparator** outcom);

        CARAPI First(
            /* [out] */ IInterface** outface);

        CARAPI Last(
            /* [out] */ IInterface** outface);

        CARAPI PollFirst(
            /* [out] */ IInterface** outface);

        CARAPI PollLast(
            /* [out] */ IInterface** outface);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArray(
            /* [in] */ ArrayOf<IInterface*>* inArray,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

        CARAPI DescendingIterator(
            /* [out] */ IIterator** outiter);

        CARAPI SubSet(
            /* [in] */ IInterface* fromElement,
            /* [in] */ Boolean fromInclusive,
            /* [in] */ IInterface* toElement,
            /* [in] */ Boolean toInclusive,
            /* [out] */ INavigableSet** outnav);

        CARAPI HeadSet(
            /* [in] */ IInterface* toElement,
            /* [in] */ Boolean inclusive,
            /* [out] */ INavigableSet** outnav);

        CARAPI TailSet(
            /* [in] */ IInterface* fromElement,
            /* [in] */ Boolean inclusive,
            /* [out] */ INavigableSet** outnav);

        CARAPI SubSet(
            /* [in] */ IInterface* start,
            /* [in] */ IInterface* end,
            /* [out] */ ISortedSet** outsort);

        CARAPI HeadSet(
            /* [in] */ IInterface* end,
            /* [out] */ ISortedSet** outsort);

        CARAPI TailSet(
            /* [in] */ IInterface* start,
            /* [out] */ ISortedSet** outsort);

        CARAPI DescendingSet(
            /* [out] */ INavigableSet** outnav);

    private:
        AutoPtr<INavigableMap> mM;
    };

    class _Values
        : public AbstractCollection
    {
    public:
        _Values(
            /* [in] */ INavigableMap* map);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI Clear();

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArray(
            /* [in] */ ArrayOf<IInterface*>* inArray,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

    private:
        AutoPtr<INavigableMap> mM;
    };

    class _EntrySet
        : public AbstractSet
    {
    public:
        _EntrySet(
            /* [in] */ INavigableMap* map);

        CARAPI Remove(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* modified);

        CARAPI Equals(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI GetIterator(
            /* [out] */ IIterator** it);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI Contains(
            /* [in] */ IInterface* object,
            /* [out] */ Boolean* result);

        CARAPI Clear();

        CARAPI ToArray(
            /* [out, callee] */ ArrayOf<IInterface*>** array);

        CARAPI ToArray(
            /* [in] */ ArrayOf<IInterface*>* inArray,
            /* [out, callee] */ ArrayOf<IInterface*>** outArray);

    private:
        AutoPtr<INavigableMap> mM;
    };

    class _SubMap
        : public AbstractMap
        , public INavigableMap
        , public IConcurrentMap
        , public ISortedMap
    {
    public:
        class SubMapIter
            : public IIterator
            , public Object
        {
        public:
            CAR_INTERFACE_DECL();

            SubMapIter(
                /* [in] */ _SubMap* owner);

            CARAPI HasNext(
                /* [out] */ Boolean* result);

            void Advance();

            CARAPI Remove();

        private:
            void Ascend();

            void Descend();

        public:
            AutoPtr<Node> mLastReturned;
            AutoPtr<Node> mNext;
            AutoPtr<IInterface> mNextValue;
            _SubMap* mOwner;
        };

        class SubMapValueIterator
            : public SubMapIter
        {
        public:
            SubMapValueIterator(
                /* [in] */ _SubMap* owner);

            CARAPI GetNext(
                /* [out] */ IInterface** object);
        };

        class SubMapKeyIterator
            : public SubMapIter
        {
        public:
            SubMapKeyIterator(
                /* [in] */ _SubMap* owner);

            CARAPI GetNext(
                /* [out] */ IInterface** object);
        };

        class SubMapEntryIterator
            : public SubMapIter
        {
        public:
            SubMapEntryIterator(
                /* [in] */ _SubMap* owner);

            CARAPI GetNext(
                /* [out] */ IInterface** object);
        };

    public:
        CAR_INTERFACE_DECL();

        _SubMap(
            /* [in] */ CConcurrentSkipListMap* map,
            /* [in] */ IInterface* fromKey,
            /* [in] */ Boolean fromInclusive,
            /* [in] */ IInterface* toKey,
            /* [in] */ Boolean toInclusive,
            /* [in] */ Boolean isDescending);

        /* ----------------  Map API methods -------------- */

        CARAPI ContainsKey(
            /* [in] */ IInterface* key,
            /* [out] */ Boolean* result);

        CARAPI Get(
            /* [in] */ IInterface* key,
            /* [out] */ IInterface** value);

        CARAPI Put(
            /* [in] */ IInterface* key,
            /* [in] */ IInterface* value,
            /* [out] */ IInterface** oldValue);

        CARAPI Remove(
            /* [in] */ IInterface* key,
            /* [out] */ IInterface** value);

        CARAPI GetSize(
            /* [out] */ Int32* size);

        CARAPI IsEmpty(
            /* [out] */ Boolean* result);

        CARAPI ContainsValue(
            /* [in] */ IInterface* value,
            /* [out] */ Boolean* result);

        CARAPI Clear();

        /* ----------------  ConcurrentMap API methods -------------- */

        CARAPI PutIfAbsent(
            /* [in] */ PInterface key,
            /* [in] */ PInterface value,
            /* [out] */ PInterface* out);

        CARAPI Remove(
            /* [in] */ PInterface key,
            /* [in] */ PInterface value,
            /* [out] */ Boolean* res);

        CARAPI Replace(
            /* [in] */ PInterface key,
            /* [in] */ PInterface oldValue,
            /* [in] */ PInterface newValue,
            /* [out] */ Boolean* res);

        CARAPI Replace(
            /* [in] */ PInterface key,
            /* [in] */ PInterface value,
            /* [out] */ PInterface* res);

        /* ----------------  SortedMap API methods -------------- */

        CARAPI Comparator(
            /* [out] */ IComparator** comp);

        CARAPI SubMap(
            /* [in] */ PInterface fromKey,
            /* [in] */ Boolean fromInclusive,
            /* [in] */ PInterface toKey,
            /* [in] */ Boolean toInclusive,
            /* [out] */ INavigableMap** res);

        CARAPI HeadMap(
            /* [in] */ PInterface toKey,
            /* [in] */ Boolean inclusive,
            /* [out] */ INavigableMap** res);

        CARAPI TailMap(
            /* [in] */ PInterface fromKey,
            /* [in] */ Boolean inclusive,
            /* [out] */ INavigableMap** res);

        CARAPI SubMap(
            /* [in] */ PInterface fromKey,
            /* [in] */ PInterface toKey,
            /* [out] */ ISortedMap** res);

        CARAPI HeadMap(
            /* [in] */ PInterface toKey,
            /* [out] */ ISortedMap** res);

        CARAPI TailMap(
            /* [in] */ PInterface fromKey,
            /* [out] */ ISortedMap** res);

        CARAPI DescendingMap(
            /* [out] */ INavigableMap** res);

        /* ----------------  Relational methods -------------- */

        CARAPI CeilingEntry(
            /* [in] */ IInterface* key,
            /* [out] */ IMapEntry** outent);

        CARAPI CeilingKey(
            /* [in] */ IInterface* key,
            /* [out] */ IInterface** outface);

        CARAPI LowerEntry(
            /* [in] */ IInterface* key,
            /* [out] */ IMapEntry** outent);

        CARAPI LowerKey(
            /* [in] */ IInterface* key,
            /* [out] */ IInterface** outface);

        CARAPI FloorEntry(
            /* [in] */ IInterface* key,
            /* [out] */ IMapEntry** outent);

        CARAPI FloorKey(
            /* [in] */ IInterface* key,
            /* [out] */ IInterface** outface);

        CARAPI HigherEntry(
            /* [in] */ IInterface* key,
            /* [out] */ IMapEntry** outent);

        CARAPI HigherKey(
            /* [in] */ IInterface* key,
            /* [out] */ IInterface** outface);

        CARAPI FirstKey(
            /* [out] */ IInterface** outface);

        CARAPI LastKey(
            /* [out] */ IInterface** outface);

        CARAPI FirstEntry(
            /* [out] */ IMapEntry** outent);

        CARAPI LastEntry(
            /* [out] */ IMapEntry** outent);

        CARAPI PollFirstEntry(
            /* [out] */ IMapEntry** outent);

        CARAPI PollLastEntry(
            /* [out] */ IMapEntry** outent);

        /* ---------------- Submap Views -------------- */

        CARAPI GetKeySet(
            /* [out] */ ISet** res);

        CARAPI NavigableKeySet(
            /* [out] */ INavigableSet** res);

        CARAPI GetValues(
            /* [out] */ ICollection** value);

        CARAPI GetEntrySet(
            /* [out] */ ISet** entries);

        CARAPI DescendingKeySet(
            /* [out] */ INavigableSet** res);

        AutoPtr<IIterator> KeyIterator();

        AutoPtr<IIterator> ValueIterator();

        AutoPtr<IIterator> EntryIterator();

    private:
        /* ----------------  Utilities -------------- */

        Boolean TooLow(
            /* [in] */ IInterface* key);

        Boolean TooHigh(
            /* [in] */ PInterface key);

        Boolean InBounds(
            /* [in] */ PInterface key);

        CARAPI CheckKeyBounds(
            /* [in] */ PInterface key);

        Boolean IsBeforeEnd(
            /* [in] */ Node* n);

        AutoPtr<Node> LoNode();

        AutoPtr<Node> HiNode();

        AutoPtr<IInterface> LowestKey();

        AutoPtr<IInterface> HighestKey();

        AutoPtr<IMapEntry> LowestEntry();

        AutoPtr<IMapEntry> HighestEntry();

        AutoPtr<IMapEntry> RemoveLowest();

        AutoPtr<IMapEntry> RemoveHighest();

        AutoPtr<IMapEntry> GetNearEntry(
            /* [in] */ PInterface key,
            /* [in] */ Int32 rel);

        AutoPtr<IInterface> GetNearKey(
            /* [in] */ PInterface key,
            /* [in] */ Int32 rel);

        AutoPtr<_SubMap> NewSubMap(
            /* [in] */ PInterface fromKey,
            /* [in] */ Boolean fromInclusive,
            /* [in] */ PInterface toKey,
            /* [in] */ Boolean toInclusive);

    private:
//        static Int64 serialVersionUID = -7647078645895051609L;

        /** Underlying map */
        AutoPtr<CConcurrentSkipListMap> mM;
        /** lower bound key, or null if from start */
        AutoPtr<IInterface> mLo;
        /** upper bound key, or null if to end */
        AutoPtr<IInterface> mHi;
        /** inclusion flag for lo */
        Boolean mLoInclusive;
        /** inclusion flag for hi */
        Boolean mHiInclusive;
        /** direction */
        Boolean mIsDescending;

        // Lazily initialized view holders
        AutoPtr<_KeySet> mKeySetView;
        AutoPtr<ISet> mEntrySetView;
        AutoPtr<ICollection> mValuesView;
    };

public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CARAPI Initialize();

    Int32 Compare(
        /* [in] */ PInterface k1,
        /* [in] */ PInterface k2);

    Boolean InHalfOpenRange(
        /* [in] */ PInterface key,
        /* [in] */ PInterface least,
        /* [in] */ PInterface fence);

    Boolean InOpenRange(
        /* [in] */ PInterface key,
        /* [in] */ PInterface least,
        /* [in] */ PInterface fence);

    /* ---------------- Deletion -------------- */

    AutoPtr<IInterface> DoRemove(
        /* [in] */ PInterface okey,
        /* [in] */ PInterface value);

    /* ---------------- Finding and removing first element -------------- */

    AutoPtr<Node> FindFirst();

    AutoPtr<IMapEntry> DoRemoveFirstEntry();

    /* ---------------- Finding and removing last element -------------- */

    AutoPtr<Node> FindLast();

    AutoPtr<IMapEntry> DoRemoveLastEntry();

    /* ---------------- Relational operations -------------- */

    // Control values OR'ed as arguments to findNear

    AutoPtr<Node> FindNear(
        /* [in] */ PInterface kkey,
        /* [in] */ Int32 rel);

    AutoPtr<AbstractMap::SimpleImmutableEntry> GetNear(
        /* [in] */ PInterface key,
        /* [in] */ Int32 rel);

    /* ---------------- Constructors -------------- */

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IComparator* comparator);

    CARAPI constructor(
        /* [in] */ IMap* m);

    CARAPI constructor(
        /* [in] */ ISortedMap* m);

    CARAPI Clone(
        /* [out] */ IInterface** res);

    /* ------ Map API methods ------ */

    CARAPI ContainsKey(
        /* [in] */ IInterface* key,
        /* [out] */ Boolean* result);

    CARAPI Get(
        /* [in] */ IInterface* key,
        /* [out] */ IInterface** value);

    CARAPI Put(
        /* [in] */ IInterface* key,
        /* [in] */ IInterface* value,
        /* [out] */ IInterface** oldValue);

    CARAPI Remove(
        /* [in] */ IInterface* key,
        /* [out] */ IInterface** value);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI IsEmpty(
        /* [out] */ Boolean* result);

    CARAPI ContainsValue(
        /* [in] */ IInterface* value,
        /* [out] */ Boolean* result);

    CARAPI Clear();

    /* ---------------- View methods -------------- */

    CARAPI GetKeySet(
        /* [out] */ ISet** res);

    CARAPI NavigableKeySet(
        /* [out] */ INavigableSet** res);

    CARAPI GetValues(
        /* [out] */ ICollection** value);

    CARAPI GetEntrySet(
        /* [out] */ ISet** entries);

    CARAPI DescendingMap(
        /* [out] */ INavigableMap** res);

    CARAPI DescendingKeySet(
        /* [out] */ INavigableSet** res);

    /* ---------------- AbstractMap Overrides -------------- */

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    /* ------ ConcurrentMap API methods ------ */

    CARAPI PutIfAbsent(
        /* [in] */ PInterface key,
        /* [in] */ PInterface value,
        /* [out] */ PInterface* out);

    CARAPI Remove(
        /* [in] */ PInterface key,
        /* [in] */ PInterface value,
        /* [out] */ Boolean* res);

    CARAPI Replace(
        /* [in] */ PInterface key,
        /* [in] */ PInterface oldValue,
        /* [in] */ PInterface newValue,
        /* [out] */ Boolean* res);

    CARAPI Replace(
        /* [in] */ PInterface key,
        /* [in] */ PInterface value,
        /* [out] */ PInterface* res);

    /* ------ SortedMap API methods ------ */

    CARAPI Comparator(
        /* [out] */ IComparator** comp);

    CARAPI FirstKey(
        /* [out] */ IInterface** outface);

    CARAPI LastKey(
        /* [out] */ IInterface** outface);

    CARAPI SubMap(
        /* [in] */ PInterface fromKey,
        /* [in] */ Boolean fromInclusive,
        /* [in] */ PInterface toKey,
        /* [in] */ Boolean toInclusive,
        /* [out] */ INavigableMap** res);

    CARAPI HeadMap(
        /* [in] */ PInterface toKey,
        /* [in] */ Boolean inclusive,
        /* [out] */ INavigableMap** res);

    CARAPI TailMap(
        /* [in] */ PInterface fromKey,
        /* [in] */ Boolean inclusive,
        /* [out] */ INavigableMap** res);

    CARAPI SubMap(
        /* [in] */ PInterface fromKey,
        /* [in] */ PInterface toKey,
        /* [out] */ ISortedMap** res);

    CARAPI HeadMap(
        /* [in] */ PInterface toKey,
        /* [out] */ ISortedMap** res);

    CARAPI TailMap(
        /* [in] */ PInterface fromKey,
        /* [out] */ ISortedMap** res);

    /* ---------------- Relational operations -------------- */

    CARAPI LowerEntry(
        /* [in] */ IInterface* key,
        /* [out] */ IMapEntry** outent);

    CARAPI LowerKey(
        /* [in] */ IInterface* key,
        /* [out] */ IInterface** outface);

    CARAPI FloorEntry(
        /* [in] */ IInterface* key,
        /* [out] */ IMapEntry** outent);

    CARAPI FloorKey(
        /* [in] */ IInterface* key,
        /* [out] */ IInterface** outface);

    CARAPI HigherEntry(
        /* [in] */ IInterface* key,
        /* [out] */ IMapEntry** outent);

    CARAPI HigherKey(
        /* [in] */ IInterface* key,
        /* [out] */ IInterface** outface);

    CARAPI CeilingEntry(
        /* [in] */ IInterface* key,
        /* [out] */ IMapEntry** outent);

    CARAPI CeilingKey(
        /* [in] */ IInterface* key,
        /* [out] */ IInterface** outface);

    CARAPI FirstEntry(
        /* [out] */ IMapEntry** outent);

    CARAPI LastEntry(
        /* [out] */ IMapEntry** outent);

    CARAPI PollFirstEntry(
        /* [out] */ IMapEntry** outent);

    CARAPI PollLastEntry(
        /* [out] */ IMapEntry** outent);

    // Factory methods for iterators needed by ConcurrentSkipListSet etc

    AutoPtr<IIterator> KeyIterator();

    AutoPtr<IIterator> ValueIterator();

    AutoPtr<IIterator> EntryIterator();

    /* ---------------- View Classes -------------- */

    static AutoPtr<IList> ToList(
        /* [in] */ ICollection* c);

    CARAPI PutAll(
        /* [in] */ IMap* map);

private:
    Boolean CasHead(
        /* [in] */ HeadIndex* cmp,
        /* [in] */ HeadIndex* val);

    AutoPtr<IComparable> Comparable(
        /* [in] */ PInterface key);

    /* ---------------- Traversal -------------- */

    AutoPtr<Node> FindPredecessor(
        /* [in] */ IComparable* key);

    AutoPtr<Node> FindNode(
        /* [in] */ IComparable* key);

    AutoPtr<IInterface> DoGet(
        /* [in] */ PInterface okey);

    /* ---------------- Insertion -------------- */

    AutoPtr<IInterface> DoPut(
        /* [in] */ PInterface kkey,
        /* [in] */ PInterface value,
        /* [in] */ Boolean onlyIfAbsent);

    Int32 RandomLevel();

    void InsertIndex(
        /* [in] */ Node* z,
        /* [in] */ Int32 level);

    void AddIndex(
        /* [in] */ Index* idx,
        /* [in] */ HeadIndex* h,
        /* [in] */ Int32 indexLevel);

    void TryReduceLevel();

    void ClearIndexToFirst();

    AutoPtr<Node> FindPredecessorOfLast();

    void BuildFromSorted(
        /* [in] */ ISortedMap* map);
    /* ---------------- Serialization -------------- */

    void WriteObject(
        /* [in] */ IObjectOutputStream* s);

    void ReadObject(
        /* [in] */ IObjectInputStream* s);

private:
//    static long serialVersionUID = -8627078645895051609L;

    static AutoPtr<IRandom> sSeedGenerator;

    static AutoPtr<IInterface> sBASE_HEADER;

    /*volatile*/ AutoPtr<HeadIndex> mHead;

    AutoPtr<IComparator> mComparator;

    Int32 mRandomSeed;

    AutoPtr<_KeySet> mKeySet;
    AutoPtr<_EntrySet> mEntrySet;
    AutoPtr<_Values> mValues;
    AutoPtr<INavigableMap> mDescendingMap;

    static Int32 sEQ;
    static Int32 sLT;
    static Int32 sGT; // Actually checked as !LT

    // Unsafe mechanics
    // private static final sun.misc.Unsafe UNSAFE;
    // private static final long headOffset;
    // static {
    //     try {
    //         UNSAFE = sun.misc.Unsafe.getUnsafe();
    //         Class<?> k = ConcurrentSkipListMap.class;
    //         headOffset = UNSAFE.objectFieldOffset
    //             (k.getDeclaredField("head"));
    //     } catch (Exception e) {
    //         throw new Error(e);
    //     }
    // }
};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CCONCURRENTSKIPLISTMAP_H__