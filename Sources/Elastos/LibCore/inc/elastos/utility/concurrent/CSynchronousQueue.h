
#ifndef __ELASTOS_UTILITY_CSYNCHRONOUSQUEUE_H__
#define __ELASTOS_UTILITY_CSYNCHRONOUSQUEUE_H__

#include "_Elastos_Utility_Concurrent_CSynchronousQueue.h"
#include "AbstractQueue.h"

using Elastos::IO::ISerializable;
using Elastos::IO::IObjectInputStream;
using Elastos::IO::IObjectOutputStream;
using Elastos::Core::IThread;
using Elastos::Utility::Concurrent::Locks::IReentrantLock;

namespace Elastos {
namespace Utility {
namespace Concurrent {

CarClass(CSynchronousQueue)
    , public AbstractQueue
    , public ISynchronousQueue
    , public IBlockingQueue
    , public ISerializable
{
public:
    /**
     * Shared internal API for dual stacks and queues.
     */
    class Transferer
        : public Object
    {
    public:
        /**
         * Performs a put or take.
         *
         * @param e if non-null, the item to be handed to a consumer;
         *          if null, requests that transfer return an item
         *          offered by producer.
         * @param timed if this operation should timeout
         * @param nanos the timeout, in nanoseconds
         * @return if non-null, the item provided or received; if null,
         *         the operation failed due to timeout or interrupt --
         *         the caller can distinguish which of these occurred
         *         by checking Thread.interrupted.
         */
        virtual AutoPtr<IInterface> Transfer(
            /* [in] */ IInterface* e,
            /* [in] */ Boolean timed,
            /* [in] */ Int64 nanos) = 0;
    };

    /** Dual stack */
    class TransferStack
        : public Transferer
        , public ISynchronousQueueTransferStack
    {
    public:
        /** Node class for TransferStacks. */
        class SNode
            : public Object
        {
        public:
            // Note: item and mode fields don't need to be volatile
            // since they are always written before, and read after,
            // other volatile/atomic operations.

            SNode(
                /* [in] */ IInterface* item);

            Boolean CasNext(
                /* [in] */ SNode* cmp,
                /* [in] */ SNode* val);

            /**
             * Tries to match node s to this node, if so, waking up thread.
             * Fulfillers call tryMatch to identify their waiters.
             * Waiters block until they have been matched.
             *
             * @param s the node to match
             * @return true if successfully matched to s
             */
            Boolean TryMatch(
                /* [in] */ SNode* s);

            /**
             * Tries to cancel a wait by matching node to itself.
             */
            void TryCancel();

            Boolean IsCancelled();

        public:
            volatile AutoPtr<SNode> mNext;        // next node in stack
            volatile AutoPtr<SNode> mMatch;       // the node matched to this
            volatile AutoPtr<IThread> mWaiter;     // to control park/unpark
            AutoPtr<IInterface> mItem;                // data; or null for REQUESTs
            Int32 mMode;

            // // Unsafe mechanics
            // private static final sun.misc.Unsafe UNSAFE;
            // private static final long matchOffset;
            // private static final long nextOffset;

            // static {
            //     try {
            //         UNSAFE = sun.misc.Unsafe.getUnsafe();
            //         Class<?> k = SNode.class;
            //         matchOffset = UNSAFE.objectFieldOffset
            //             (k.getDeclaredField("match"));
            //         nextOffset = UNSAFE.objectFieldOffset
            //             (k.getDeclaredField("next"));
            //     } catch (Exception e) {
            //         throw new Error(e);
            //     }
            // }
        };

    public:
        CAR_INTERFACE_DECL()

        Boolean CasHead(
            /* [in] */ SNode* h,
            /* [in] */ SNode* nh);

        /**
         * Creates or resets fields of a node. Called only from transfer
         * where the node to push on stack is lazily created and
         * reused when possible to help reduce intervals between reads
         * and CASes of head and to avoid surges of garbage when CASes
         * to push nodes fail due to contention.
         */
        static AutoPtr<SNode> Snode(
            /* [in] */ SNode* s,
            /* [in] */ IInterface* e,
            /* [in] */ SNode* next,
            /* [in] */ Int32 mode);

        /**
         * Puts or takes an item.
         */
        AutoPtr<IInterface> Transfer(
            /* [in] */ IInterface* e,
            /* [in] */ Boolean timed,
            /* [in] */ Int64 nanos);

        /**
         * Spins/blocks until node s is matched by a fulfill operation.
         *
         * @param s the waiting node
         * @param timed true if timed wait
         * @param nanos timeout value
         * @return matched node, or s if cancelled
         */
        AutoPtr<SNode> AwaitFulfill(
            /* [in] */ SNode* s,
            /* [in] */ Boolean timed,
            /* [in] */ Int64 nanos);

        /**
         * Returns true if node s is at head or there is an active
         * fulfiller.
         */
        Boolean ShouldSpin(
            /* [in] */ SNode* s);

        /**
         * Unlinks s from the stack.
         */
        void Clean(
            /* [in] */ SNode* s);

        /** Returns true if m has fulfilling bit set. */
        static Boolean IsFulfilling(
            /* [in] */ Int32 m);

    public:
        /* Modes for SNodes, ORed together in node fields */
        /** Node represents an unfulfilled consumer */
        static Int32 REQUEST;
        /** Node represents an unfulfilled producer */
        static Int32 DATA;
        /** Node is fulfilling another unfulfilled DATA or REQUEST */
        static Int32 FULFILLING;

        /** The head (top) of the stack */
        volatile AutoPtr<SNode> mHead;

        // // Unsafe mechanics
        // private static final sun.misc.Unsafe UNSAFE;
        // private static final long headOffset;
        // static {
        //     try {
        //         UNSAFE = sun.misc.Unsafe.getUnsafe();
        //         Class<?> k = TransferStack.class;
        //         headOffset = UNSAFE.objectFieldOffset
        //             (k.getDeclaredField("head"));
        //     } catch (Exception e) {
        //         throw new Error(e);
        //     }
        // }
    };

    /** Dual Queue */
    class TransferQueue
        : public Transferer
        , public ISynchronousQueueTransferQueue
    {
    public:
        /*
         * This extends Scherer-Scott dual queue algorithm, differing,
         * among other ways, by using modes within nodes rather than
         * marked pointers. The algorithm is a little simpler than
         * that for stacks because fulfillers do not need explicit
         * nodes, and matching is done by CAS'ing QNode.item field
         * from non-null to null (for put) or vice versa (for take).
         */

        /** Node class for TransferQueue. */
        class QNode
            : public Object
        {
        public:
            QNode(
                /* [in] */ IInterface* item,
                /* [in] */ Boolean isData);

            Boolean CasNext(
                /* [in] */ QNode* cmp,
                /* [in] */ QNode* val);

            Boolean CasItem(
                /* [in] */ IInterface* cmp,
                /* [in] */ IInterface* val);

            /**
             * Tries to cancel by CAS'ing ref to this as item.
             */
            void TryCancel(
                /* [in] */ IInterface* cmp);

            Boolean IsCancelled();

            /**
             * Returns true if this node is known to be off the queue
             * because its next pointer has been forgotten due to
             * an advanceHead operation.
             */
            Boolean IsOffList();

        public:
            volatile AutoPtr<QNode> mNext;          // next node in queue
            volatile AutoPtr<IInterface> mItem;         // CAS'ed to or from null
            volatile AutoPtr<IThread> mWaiter;       // to control park/unpark
            Boolean mIsData;

            // // Unsafe mechanics
            // private static final sun.misc.Unsafe UNSAFE;
            // private static final long itemOffset;
            // private static final long nextOffset;

            // static {
            //     try {
            //         UNSAFE = sun.misc.Unsafe.getUnsafe();
            //         Class<?> k = QNode.class;
            //         itemOffset = UNSAFE.objectFieldOffset
            //             (k.getDeclaredField("item"));
            //         nextOffset = UNSAFE.objectFieldOffset
            //             (k.getDeclaredField("next"));
            //     } catch (Exception e) {
            //         throw new Error(e);
            //     }
            // }
        };

    public:
        CAR_INTERFACE_DECL()

        TransferQueue();

        /**
         * Tries to cas nh as new head; if successful, unlink
         * old head's next node to avoid garbage retention.
         */
        void AdvanceHead(
            /* [in] */ QNode* h,
            /* [in] */ QNode* nh);

        /**
         * Tries to cas nt as new tail.
         */
        void AdvanceTail(
            /* [in] */ QNode* t,
            /* [in] */ QNode* nt);

        /**
         * Tries to CAS cleanMe slot.
         */
        Boolean CasCleanMe(
            /* [in] */ QNode* cmp,
            /* [in] */ QNode* val);

        /**
         * Puts or takes an item.
         */
        AutoPtr<IInterface> Transfer(
            /* [in] */ IInterface* e,
            /* [in] */ Boolean timed,
            /* [in] */ Int64 nanos);

        /**
         * Spins/blocks until node s is fulfilled.
         *
         * @param s the waiting node
         * @param e the comparison value for checking match
         * @param timed true if timed wait
         * @param nanos timeout value
         * @return matched item, or s if cancelled
         */
        AutoPtr<IInterface> AwaitFulfill(
            /* [in] */ QNode* s,
            /* [in] */ IInterface* e,
            /* [in] */ Boolean timed,
            /* [in] */ Int64 nanos);

        /**
         * Gets rid of cancelled node s with original predecessor pred.
         */
        void Clean(
            /* [in] */ QNode* pred,
            /* [in] */ QNode* s);

    public:
        /** Head of queue */
        volatile AutoPtr<QNode> mHead;
        /** Tail of queue */
        volatile AutoPtr<QNode> mTail;
        /**
         * Reference to a cancelled node that might not yet have been
         * unlinked from queue because it was the last inserted node
         * when it was cancelled.
         */
        volatile AutoPtr<QNode> mCleanMe;

        // private static final sun.misc.Unsafe UNSAFE;
        // private static final long headOffset;
        // private static final long tailOffset;
        // private static final long cleanMeOffset;
        // static {
        //     try {
        //         UNSAFE = sun.misc.Unsafe.getUnsafe();
        //         Class<?> k = TransferQueue.class;
        //         headOffset = UNSAFE.objectFieldOffset
        //             (k.getDeclaredField("head"));
        //         tailOffset = UNSAFE.objectFieldOffset
        //             (k.getDeclaredField("tail"));
        //         cleanMeOffset = UNSAFE.objectFieldOffset
        //             (k.getDeclaredField("cleanMe"));
        //     } catch (Exception e) {
        //         throw new Error(e);
        //     }
        // }
    };

    // Replicated from a previous version of Collections
    class EmptyIterator
        : public Object
        , public IIterator
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI HasNext(
            /* [out] */ Boolean* result);

        CARAPI GetNext(
            /* [out] */ IInterface** object);

        CARAPI Remove();

    public:
        static AutoPtr<EmptyIterator> EMPTY_ITERATOR;
    };

    /*
     * To cope with serialization strategy in the 1.5 version of
     * SynchronousQueue, we declare some unused classes and fields
     * that exist solely to enable serializability across versions.
     * These fields are never used, so are initialized only if this
     * object is ever serialized or deserialized.
     */
    class WaitQueue
        : public Object
        , public IWaitQueue
        , public ISerializable
    {
    public:
        CAR_INTERFACE_DECL()
    };

    class LifoWaitQueue
        : public WaitQueue
        , public ILifoWaitQueue
    {
    public:
        CAR_INTERFACE_DECL()

    private:
        static Int64 mSerialVersionUID;
    };

    class FifoWaitQueue
        : public WaitQueue
        , public IFifoWaitQueue
    {
    public:
        CAR_INTERFACE_DECL()

    private:
        static Int64 mSerialVersionUID;
    };

public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    /**
     * Creates a {@code SynchronousQueue} with nonfair access policy.
     */
    CARAPI constructor();

    /**
     * Creates a {@code SynchronousQueue} with the specified fairness policy.
     *
     * @param fair if true, waiting threads contend in FIFO order for
     *        access; otherwise the order is unspecified.
     */
    CARAPI constructor(
        /* [in] */ Boolean fair);

    CARAPI Put(
        /* [in] */ IInterface* e);

    CARAPI Offer(
        /* [in] */ IInterface* e,
        /* [in] */ Int64 timeout,
        /* [in] */ ITimeUnit* unit,
        /* [out] */ Boolean* result);

    CARAPI Offer(
        /* [in] */ IInterface* e,
        /* [out] */ Boolean* result);

    CARAPI Take(
        /* [out] */ IInterface** res);

    CARAPI Poll(
        /* [in] */ Int64 timeout,
        /* [in] */ ITimeUnit* unit,
        /* [out] */ IInterface** res);

    CARAPI Poll(
        /* [out] */ IInterface** res);

    CARAPI IsEmpty(
        /* [out] */ Boolean* result);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI RemainingCapacity(
        /* [out] */ Int32* capacity);

    CARAPI Clear();

    CARAPI Contains(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI Remove(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* modified);

    CARAPI ContainsAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* result);

    CARAPI RemoveAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI RetainAll(
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* modified);

    CARAPI Peek(
        /* [out] */ IInterface** e);

    CARAPI GetIterator(
        /* [out] */ IIterator** it);

    CARAPI ToArray(
        /* [out, callee] */ ArrayOf<IInterface*>** array);

    CARAPI ToArray(
        /* [in] */ ArrayOf<IInterface*>* inArray,
        /* [out, callee] */ ArrayOf<IInterface*>** outArray);

    CARAPI DrainTo(
        /* [in] */ ICollection* c,
        /* [out] */ Int32* number);

    CARAPI DrainTo(
        /* [in] */ ICollection* c,
        /* [in] */ Int32 maxElements,
        /* [out] */ Int32* number);

private:
    /**
     * Saves this queue to a stream (that is, serializes it).
     */
    void WriteObject(
        /* [in] */ IObjectOutputStream* s);

    /**
     * Reconstitutes this queue from a stream (that is, deserializes it).
     */
    void ReadObject(
        /* [in] */ IObjectInputStream* s);

public:
    /** The number of CPUs, for spin control */
    static Int32 NCPUS;

    /**
     * The number of times to spin before blocking in timed waits.
     * The value is empirically derived -- it works well across a
     * variety of processors and OSes. Empirically, the best value
     * seems not to vary with number of CPUs (beyond 2) so is just
     * a constant.
     */
    static Int32 mMaxTimedSpins;

    /**
     * The number of times to spin before blocking in untimed waits.
     * This is greater than timed value because untimed waits spin
     * faster since they don't need to check times on each spin.
     */
    static Int32 mMaxUntimedSpins;

    /**
     * The number of nanoseconds for which it is faster to spin
     * rather than to use timed park. A rough estimate suffices.
     */
    static Int64 mSpinForTimeoutThreshold;

private:
    static Int64 mSerialVersionUID;
    AutoPtr<IReentrantLock> mQlock;
    AutoPtr<WaitQueue> mWaitingProducers;
    AutoPtr<WaitQueue> mWaitingConsumers;
    /**
     * The transferer. Set only in constructor, but cannot be declared
     * as final without further complicating serialization.  Since
     * this is accessed only at most once per public method, there
     * isn't a noticeable performance penalty for using volatile
     * instead of final here.
     */
    volatile AutoPtr<Transferer> mTransferer;

    // // Unsafe mechanics
    // static long objectFieldOffset(sun.misc.Unsafe UNSAFE,
    //                               String field, Class<?> klazz) {
    //     try {
    //         return UNSAFE.objectFieldOffset(klazz.getDeclaredField(field));
    //     } catch (NoSuchFieldException e) {
    //         // Convert Exception to corresponding Error
    //         NoSuchFieldError error = new NoSuchFieldError(field);
    //         error.initCause(e);
    //         throw error;
    //     }
    // }

};

} // namespace Concurrent
} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CSYNCHRONOUSQUEUE_H__
