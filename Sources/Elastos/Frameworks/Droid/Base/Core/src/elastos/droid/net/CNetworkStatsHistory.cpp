
#include "net/CNetworkStatsHistory.h"
#include "net/CNetworkStatsHistoryEntry.h"
#include "net/CNetworkStatsEntry.h"
#include "util/ArrayUtils.h"
#include <elastos/core/Math.h>

using Elastos::IO::IDataInput;
using Elastos::IO::IDataOutput;
using Elastos::IO::CDataInputStream;
using Elastos::Droid::Net::CNetworkStatsEntry;
using Elastos::Droid::Net::CNetworkStatsHistoryEntry;
using Elastos::Droid::Internal::Utility::ArrayUtils;

namespace Elastos {
namespace Droid {
namespace Net {

 const Int32 CNetworkStatsHistory::VERSION_INIT = 1;
 const Int32 CNetworkStatsHistory::VERSION_ADD_PACKETS = 2;
 const Int32 CNetworkStatsHistory::VERSION_ADD_ACTIVE = 3;

ECode CNetworkStatsHistory::DataStreamUtils::ReadFullLongArray(
    /* [in] */ IDataInputStream* in,
    /* [out] */ ArrayOf<Int64>** result)
{
    Int32 size;
    IDataInput::Probe(in)->ReadInt32(&size);
    AutoPtr< ArrayOf<Int64> > values = ArrayOf<Int64>::Alloc(size);

    for (int i = 0; i < values->GetLength(); i++) {
        IDataInput::Probe(in)->ReadInt64(&(*values)[i]);
    }
    *result =values;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CNetworkStatsHistory::DataStreamUtils::ReadVarLong(
    /* [in] */ IDataInputStream* in,
    /* [out] */ Int64* values)
{
    Int32 shift = 0;
    Int64 result = 0;
    while (shift < 64) {
        Byte b;
        IDataInput::Probe(in)->ReadByte(&b);
        result |= (Int64) (b & 0x7F) << shift;
        if ((b & 0x80) == 0) {
            *values = result;
            return NOERROR;
        }
        shift += 7;
    }
    //throw new ProtocolException("malformed long");
    return E_PROTOCOL_EXCEPTION;
}

ECode CNetworkStatsHistory::DataStreamUtils::WriteVarLong(
    /* [in] */ IDataOutputStream* out,
    /* [in] */ Int64 value)
{
    while (TRUE) {
        if ((value & ~0x7FL) == 0) {
            IDataOutput::Probe(out)->WriteByte((Int32) value);
            return NOERROR;
        }
        else {
            IDataOutput::Probe(out)->WriteByte(((int) value & 0x7F) | 0x80);
            value >>= 7; //value >>>= 7;
        }
    }
    return NOERROR;
}

ECode CNetworkStatsHistory::DataStreamUtils::ReadVarLongArray(
    /* [in] */ IDataInputStream* in,
    /* [out] */ ArrayOf<Int64>** result)
{
    Int32 size;
    IDataInput::Probe(in)->ReadInt32(&size);
    if (size == -1)
    {
        *result = NULL;
        return NOERROR;
    }
    AutoPtr< ArrayOf<Int64> > values = ArrayOf<Int64>::Alloc(size);
    for (int i = 0; i < values->GetLength(); i++) {
        IDataInput::Probe(in)->ReadInt64(&(*values)[i]);
    }
    *result =values;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CNetworkStatsHistory::DataStreamUtils::WriteVarLongArray(
    /* [in] */ IDataOutputStream* out,
    /* [in] */ ArrayOf<Int64>* values,
    /* [in] */ Int32 size)
{
    if (values == NULL) {
        IDataOutput::Probe(out)->WriteInt32(-1);
        return NOERROR;
    }
    if (size > values->GetLength()) {
        //throw new IllegalArgumentException("size larger than length");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    IDataOutput::Probe(out)->WriteInt32(size);
    for (int i = 0; i < size; i++) {
        DataStreamUtils::WriteVarLong(out, (*values)[i]);
    }
    return NOERROR;
}

ECode CNetworkStatsHistory::ParcelUtils::ReadLongArray(
    /* [in] */ IParcel* in,
    /* [out] */ArrayOf<Int64>** result)
{
    Int32 size;
    in->ReadInt32(&size);
    if (size == -1)
    {
        *result = NULL;
        return NOERROR;
    }
    AutoPtr< ArrayOf<Int64> > values = ArrayOf<Int64>::Alloc(size);
    for (int i = 0; i < values->GetLength(); i++) {
        in->ReadInt64(&(*values)[i]);
    }
    *result =values;
    REFCOUNT_ADD(*result);
    return NOERROR;
}
ECode CNetworkStatsHistory::ParcelUtils::WriteLongArray(
    /* [in] */ IParcel* out,
    /* [in] */ ArrayOf<Int64>* values,
    /* [in] */ Int32 size)
{
    if (values == NULL) {
        out->WriteInt32(-1);
        return NOERROR;
    }
    if (size > values->GetLength()) {
        // throw new IllegalArgumentException("size larger than length");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    out->WriteInt32(size);
    for (int i = 0; i < size; i++) {
        out->WriteInt64((*values)[i]);
    }
    return NOERROR;
}

ECode CNetworkStatsHistory::constructor()
{
    return NOERROR;
}

ECode CNetworkStatsHistory::constructor(
    /* [in] */ Int64 bucketDuration)
{
    constructor(bucketDuration, 10, FIELD_ALL);
    return NOERROR;
}

ECode CNetworkStatsHistory::constructor(
    /* [in] */ Int64 bucketDuration,
    /* [in] */ Int32 initialSize)
{
    constructor(bucketDuration, initialSize, FIELD_ALL);
    return NOERROR;
}

ECode CNetworkStatsHistory::constructor(
    /* [in] */ Int64 bucketDuration,
    /* [in] */ Int32 initialSize,
    /* [in] */ Int32 fields)
{
    mBucketDuration = bucketDuration;
    mBucketStart = ArrayOf<Int64>::Alloc(initialSize);

    if ((fields & FIELD_ACTIVE_TIME) != 0)
        mActiveTime = ArrayOf<Int64>::Alloc(initialSize);

    if ((fields & FIELD_RX_BYTES) != 0)
        mRxBytes = ArrayOf<Int64>::Alloc(initialSize);

    if ((fields & FIELD_RX_PACKETS) != 0)
        mRxPackets = ArrayOf<Int64>::Alloc(initialSize);

    if ((fields & FIELD_TX_BYTES) != 0)
        mTxBytes = ArrayOf<Int64>::Alloc(initialSize);

    if ((fields & FIELD_TX_PACKETS) != 0)
        mTxPackets = ArrayOf<Int64>::Alloc(initialSize);

    if ((fields & FIELD_OPERATIONS) != 0)
        mOperations = ArrayOf<Int64>::Alloc(initialSize);

    mBucketCount = 0;
    mTotalBytes = 0;
    return NOERROR;
}

ECode CNetworkStatsHistory::constructor(
    /* [in] */ INetworkStatsHistory* existing,
    /* [in] */ Int64 bucketDuration)
{
    Int32 resizeBuckets;
    existing->EstimateResizeBuckets(bucketDuration, &resizeBuckets);
    constructor(mBucketDuration, resizeBuckets);
    RecordEntireHistory(existing);
    return NOERROR;
}



ECode CNetworkStatsHistory::constructor(
    /* [in] */ IDataInputStream* in)
{
    Int32 version;
    IDataInput::Probe(in)->ReadInt32(&version);
    switch (version) {
        case VERSION_INIT: {
            IDataInput::Probe(in)->ReadInt64(&mBucketDuration);
            DataStreamUtils::ReadFullLongArray(in, (ArrayOf<Int64>**)&mBucketStart);
            DataStreamUtils::ReadFullLongArray(in, (ArrayOf<Int64>**)&mRxBytes);
            mRxPackets = ArrayOf<Int64>::Alloc(mBucketStart->GetLength());
            DataStreamUtils::ReadFullLongArray(in, (ArrayOf<Int64>**)&mTxBytes);
            mTxPackets = ArrayOf<Int64>::Alloc(mBucketStart->GetLength());
            mOperations = ArrayOf<Int64>::Alloc(mBucketStart->GetLength());
            mBucketCount = mBucketStart->GetLength();
            mTotalBytes = ArrayUtils::Total(mRxBytes) + ArrayUtils::Total(mTxBytes);
            break;
        }
        case VERSION_ADD_PACKETS:
        case VERSION_ADD_ACTIVE: {
            IDataInput::Probe(in)->ReadInt64(&mBucketDuration);
            DataStreamUtils::ReadVarLongArray(in, (ArrayOf<Int64>**)&mBucketStart );
            if (version >= VERSION_ADD_ACTIVE)
                DataStreamUtils::ReadVarLongArray(in, (ArrayOf<Int64>**)&mActiveTime);
            else
                mActiveTime = ArrayOf<Int64>::Alloc(mBucketStart->GetLength());
            DataStreamUtils::ReadVarLongArray(in, (ArrayOf<Int64>**)&mRxBytes);
            DataStreamUtils::ReadVarLongArray(in, (ArrayOf<Int64>**)&mRxPackets);
            DataStreamUtils::ReadVarLongArray(in, (ArrayOf<Int64>**)&mTxBytes);
            DataStreamUtils::ReadVarLongArray(in, (ArrayOf<Int64>**)&mTxPackets);
            DataStreamUtils::ReadVarLongArray(in, (ArrayOf<Int64>**)&mOperations);
            mBucketCount = mBucketStart->GetLength();
            mTotalBytes = ArrayUtils::Total(mRxBytes) + ArrayUtils::Total(mTxBytes);
            break;
        }
        default: {
            //throw new ProtocolException("unexpected version: " + version);
            return E_PROTOCOL_EXCEPTION;
        }
    }

    if (mBucketStart->GetLength() != mBucketCount || mRxBytes->GetLength() != mBucketCount
        || mRxPackets->GetLength() != mBucketCount || mTxBytes->GetLength() != mBucketCount
        || mTxPackets->GetLength() != mBucketCount || mOperations->GetLength() != mBucketCount) {
         //throw new ProtocolException("Mismatched history lengths");
        return E_PROTOCOL_EXCEPTION;
    }
    return NOERROR;
}

ECode CNetworkStatsHistory::WriteToStream(
    /* [in] */ IDataOutputStream* out)
{
    IDataOutput::Probe(out)->WriteInt32(VERSION_ADD_ACTIVE);
    IDataOutput::Probe(out)->WriteInt64(mBucketDuration);
    DataStreamUtils::WriteVarLongArray(out, mBucketStart, mBucketCount);
    DataStreamUtils::WriteVarLongArray(out, mActiveTime, mBucketCount);
    DataStreamUtils::WriteVarLongArray(out, mRxBytes, mBucketCount);
    DataStreamUtils::WriteVarLongArray(out, mRxPackets, mBucketCount);
    DataStreamUtils::WriteVarLongArray(out, mTxBytes, mBucketCount);
    DataStreamUtils::WriteVarLongArray(out, mTxPackets, mBucketCount);
    DataStreamUtils::WriteVarLongArray(out, mOperations, mBucketCount);
    return NOERROR;
}


ECode CNetworkStatsHistory::GetSize(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mBucketCount;
    return NOERROR;
}

ECode CNetworkStatsHistory::GetBucketDuration(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mBucketDuration;
    return NOERROR;
}

ECode CNetworkStatsHistory::GetStart(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    if (mBucketCount > 0) {
        *result = (*mBucketStart)[0];
    }
    else {
        *result = Elastos::Core::Math::INT64_MAX_VALUE;
    }
    return NOERROR;
}

ECode CNetworkStatsHistory::GetEnd(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    if (mBucketCount > 0) {
        *result = (*mBucketStart)[mBucketCount - 1] + mBucketDuration;
    }
    else {
        *result = Elastos::Core::Math::INT64_MIN_VALUE;
    }
    return NOERROR;
}

/**
 * Return total bytes represented by this history.
 */
ECode CNetworkStatsHistory::GetTotalBytes(
    /* [out] */ Int64* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mTotalBytes;
    return NOERROR;
}

/**
 * Return index of bucket that contains or is immediately before the
 * requested time.
 */
ECode CNetworkStatsHistory::GetIndexBefore(
    /* [in] */ Int64 time,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 index = mBucketStart->BinarySearch(time);

    if (index < 0) {
        index = (~index) - 1;
    }
    else {
        index -= 1;
    }
    *result = index < 0 ? 0 : (index > (mBucketCount - 1) ? (mBucketCount - 1) : index);
    return NOERROR;
}

/**
 * Return index of bucket that contains or is immediately after the
 * requested time.
 */
ECode CNetworkStatsHistory::GetIndexAfter(
    /* [in] */ Int64 time,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 index = mBucketStart->BinarySearch(time);
    if (index < 0) {
        index = ~index;
    }
    else {
        index += 1;
    }

    *result =index < 0 ? 0 : (index > (mBucketCount - 1) ? (mBucketCount - 1) : index);
    return NOERROR;
}

/**
 * Return specific stats entry.
 */
ECode CNetworkStatsHistory::GetValues(
    /* [in] */ Int32 i,
    /* [in] */ INetworkStatsHistoryEntry* recycle,
    /* [out] */ INetworkStatsHistoryEntry** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<INetworkStatsHistoryEntry> entry;
    if (recycle != NULL)
        entry = recycle;
    else
        CNetworkStatsHistoryEntry::New((INetworkStatsHistoryEntry**)&entry);

    entry->SetBucketStart((*mBucketStart)[i]);
    entry->SetBucketDuration(mBucketDuration);
    entry->SetActiveTime(GetLong(mActiveTime, i, INetworkStatsHistoryEntry::UNKNOWN));
    entry->SetRxBytes(GetLong(mRxBytes, i, INetworkStatsHistoryEntry::UNKNOWN));
    entry->SetRxPackets(GetLong(mRxPackets, i, INetworkStatsHistoryEntry::UNKNOWN));
    entry->SetTxBytes(GetLong(mTxBytes, i, INetworkStatsHistoryEntry::UNKNOWN));
    entry->SetTxPackets(GetLong(mTxPackets, i, INetworkStatsHistoryEntry::UNKNOWN));
    entry->SetOperations(GetLong(mOperations, i, INetworkStatsHistoryEntry::UNKNOWN));
    *result = entry;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * Record that data traffic occurred in the given time range. Will
 * distribute across internal buckets, creating new buckets as needed.
 */
//@Deprecated
ECode CNetworkStatsHistory::RecordData(
    /* [in] */ Int64 start,
    /* [in] */ Int64 end,
    /* [in] */ Int64 rxBytes,
    /* [in] */ Int64 txBytes)
{
    AutoPtr<INetworkStatsEntry> networkStatsEntry;
    CNetworkStatsEntry::New(
    INetworkStats::IFACE_ALL, INetworkStats::UID_ALL, INetworkStats::SET_DEFAULT,
    INetworkStats::TAG_NONE, rxBytes, 0L,  txBytes, 0L, 0L,
    (INetworkStatsEntry**)&networkStatsEntry);

    return RecordData(start, end, networkStatsEntry);
}

/**
 * Record that data traffic occurred in the given time range. Will
 * distribute across internal buckets, creating new buckets as needed.
 */
ECode CNetworkStatsHistory::RecordData(
    /* [in] */ Int64 start,
    /* [in] */ Int64 end,
    /* [in] */ INetworkStatsEntry* entry)
{
    Int64 rxBytes;
    entry->GetRxBytes(&rxBytes);
    Int64 rxPackets;
    entry->GetRxPackets(&rxPackets);
    Int64 txBytes;
    entry->GetTxBytes(&txBytes);
    Int64 txPackets;
    entry->GetTxPackets(&txPackets);
    Int64 operations;
    entry->GetOperations(&operations);

    Boolean bol;
    entry->IsNegative(&bol);
    if (bol) {
//        throw new IllegalArgumentException("tried recording negative data");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    entry->IsEmpty(&bol);
    if (bol) {
        return NOERROR;
    }
    // create any buckets needed by this range
    EnsureBuckets(start, end);

    // distribute data usage into buckets
    Int64 duration = end - start;
    Int32 startIndex;
    GetIndexAfter(end, &startIndex);
    for (Int32 i = startIndex; i >= 0; i--) {
        const Int64 curStart = (*mBucketStart)[i];
        const Int64 curEnd = curStart + mBucketDuration;

        // bucket is older than record; we're finished
        if (curEnd < start) break;
        // bucket is newer than record; keep looking
        if (curStart > end) continue;

        const Int64 overlap = Elastos::Core::Math::Min(curEnd, end) - Elastos::Core::Math::Max(curStart, start);
        if (overlap <= 0) continue;

        // integer math each time is faster than floating point
        const Int64 fracRxBytes = rxBytes * overlap / duration;
        const Int64 fracRxPackets = rxPackets * overlap / duration;
        const Int64 fracTxBytes = txBytes * overlap / duration;
        const Int64 fracTxPackets = txPackets * overlap / duration;
        const Int64 fracOperations = operations * overlap / duration;

        AddLong(mActiveTime, i, overlap);
        AddLong(mRxBytes, i, fracRxBytes);
        rxBytes -= fracRxBytes;
        AddLong(mRxPackets, i, fracRxPackets);
        rxPackets -= fracRxPackets;
        AddLong(mTxBytes, i, fracTxBytes);
        txBytes -= fracTxBytes;
        AddLong(mTxPackets, i, fracTxPackets);
        txPackets -= fracTxPackets;
        AddLong(mOperations, i, fracOperations);
        operations -= fracOperations;

        duration -= overlap;
    }

    mTotalBytes += rxBytes + txBytes;
    return NOERROR;
}

/**
 * Record an entire {@link NetworkStatsHistory} into this history. Usually
 * for combining together stats for external reporting.
 */
ECode CNetworkStatsHistory::RecordEntireHistory(
    /* [in] */ INetworkStatsHistory* input)
{
    RecordHistory(input, Elastos::Core::Math::INT64_MIN_VALUE, Elastos::Core::Math::INT64_MAX_VALUE);
    return NOERROR;
}

/**
 * Record given {@link NetworkStatsHistory} into this history, copying only
 * buckets that atomically occur in the inclusive time range. Doesn't
 * interpolate across partial buckets.
 */
ECode CNetworkStatsHistory::RecordHistory(
    /* [in] */ INetworkStatsHistory* input,
    /* [in] */ Int64 start,
    /* [in] */ Int64 end)
{
    AutoPtr<INetworkStatsEntry> entry;
    CNetworkStatsEntry::New(INetworkStats::IFACE_ALL, INetworkStats::UID_ALL, INetworkStats::SET_DEFAULT, INetworkStats::TAG_NONE, 0L, 0L, 0L, 0L, 0L, (INetworkStatsEntry**)&entry);
    Int32 bucketCount;
    input->GetBucketCount(&bucketCount);

    for (Int32 i = 0; i < bucketCount; i++) {
        AutoPtr< ArrayOf<Int64> > bucketStartArray;
        input->GetBucketStart((ArrayOf<Int64>**)&bucketStartArray);
        const Int64 bucketStart = (*bucketStartArray)[i];
        Int64 bucketDuration;
        input->GetBucketDuration(&bucketDuration);
        const Int64 bucketEnd = bucketStart + bucketDuration;

        //skip when bucket is outside requested range
        if (bucketStart < start || bucketEnd > end) continue;
        AutoPtr< ArrayOf<Int64> > rxBytes;
        input->GetRxBytes((ArrayOf<Int64>**)&rxBytes);
        AutoPtr< ArrayOf<Int64> > rxPackets;
        input->GetRxPackets((ArrayOf<Int64>**)&rxPackets);
        AutoPtr< ArrayOf<Int64> > txBytes;
        input->GetTxBytes((ArrayOf<Int64>**)&txBytes);
        AutoPtr< ArrayOf<Int64> > txPackets;
        input->GetTxPackets((ArrayOf<Int64>**)&txPackets);
        AutoPtr< ArrayOf<Int64> > operations;
        input->GetOperations((ArrayOf<Int64>**)&operations);

        entry->SetRxBytes(GetLong(rxBytes, i, 0L));
        entry->SetRxPackets(GetLong(rxPackets, i, 0L));
        entry->SetTxBytes(GetLong(txBytes, i, 0L));
        entry->SetTxPackets(GetLong(txPackets, i, 0L));
        entry->SetOperations(GetLong(operations, i, 0L));

        RecordData(bucketStart, bucketEnd, entry);
    }
    return NOERROR;
}

/**
 * Ensure that buckets exist for given time range, creating as needed.
 */
void CNetworkStatsHistory::EnsureBuckets(
    /* [in] */ Int64 start,
    /* [in] */ Int64 end)
{
    //normalize incoming range to bucket boundaries
    start -= start % mBucketDuration;
    end += (mBucketDuration - (end % mBucketDuration)) % mBucketDuration;

    for (Int64 now = start; now < end; now += mBucketDuration) {
        //try finding existing bucket
        Int32 index = mBucketStart->BinarySearch(now);
        if (index < 0) {
            // bucket missing, create and insert
            InsertBucket(~index, now);
        }
    }
}

/**
 * Insert new bucket at requested index and starting time.
 */
void CNetworkStatsHistory::InsertBucket(
    /* [in] */ Int32 index,
    /* [in] */ Int64 start)
{
    // create more buckets when needed
    if (mBucketCount >= mBucketStart->GetLength()) {
        const Int32 newLength = Elastos::Core::Math::Max(mBucketStart->GetLength(), 10) * 3 / 2;
        //not sure for copy method
        mBucketStart->Copy(0, mBucketStart, 0, newLength);
        if (mActiveTime != NULL) mActiveTime->Copy(0, mActiveTime, 0, newLength);
        if (mRxBytes != NULL) mRxBytes->Copy(0, mRxBytes, 0, newLength);
        if (mRxPackets != NULL) mRxPackets->Copy(0, mRxPackets, 0, newLength);
        if (mTxBytes != NULL) mTxBytes->Copy(0, mTxBytes, 0, newLength);
        if (mTxPackets != NULL) mTxPackets->Copy(0, mTxPackets, 0, newLength);
        if (mOperations != NULL) mOperations->Copy(0, mOperations, 0, newLength);
    }

    // create gap when inserting bucket in middle
    if (index < mBucketCount) {
        const Int32 dstPos = index + 1;
        const Int32 length = mBucketCount - index;
        //not sure for copy method
        mBucketStart->Copy(index, mBucketStart, dstPos, length);
        if (mActiveTime != NULL) mActiveTime->Copy(index, mActiveTime, dstPos, length);
        if (mRxBytes != NULL) mRxBytes->Copy(index, mRxBytes, dstPos, length);
        if (mRxPackets != NULL) mRxPackets->Copy(index, mRxPackets, dstPos, length);
        if (mTxBytes != NULL) mTxBytes->Copy(index, mTxBytes, dstPos, length);
        if (mTxPackets != NULL) mTxPackets->Copy(index, mTxPackets, dstPos, length);
        if (mOperations != NULL) mOperations->Copy(index, mOperations, dstPos, length);
    }

    (*mBucketStart)[index] = start;
    SetLong(mActiveTime, index, 0L);
    SetLong(mRxBytes, index, 0L);
    SetLong(mRxPackets, index, 0L);
    SetLong(mTxBytes, index, 0L);
    SetLong(mTxPackets, index, 0L);
    SetLong(mOperations, index, 0L);
    mBucketCount++;
}

/**
 * Remove buckets older than requested cutoff.
 */
//@Deprecated
ECode CNetworkStatsHistory::RemoveBucketsBefore(
    /* [in] */ Int64 cutoff)
{
    Int32 i;
    for (i = 0; i < mBucketCount; i++) {
        const Int64 curStart = (*mBucketStart)[i];
        const Int64 curEnd = curStart + mBucketDuration;

        // cutoff happens before or during this bucket; everything before
        // this bucket should be removed.
        if (curEnd > cutoff) break;
    }

    if (i > 0) {
        const Int32 length = mBucketStart->GetLength();
        //not sure for copy method
        mBucketStart->Copy(i, mBucketStart, 0, length-i);
        if (mActiveTime != NULL) mActiveTime->Copy(i, mActiveTime, 0, length-i);
        if (mRxBytes != NULL) mRxBytes->Copy(i, mRxBytes, 0, length-i);
        if (mRxPackets != NULL) mRxPackets->Copy(i, mRxPackets, 0, length-i);
        if (mTxBytes != NULL) mTxBytes->Copy(i, mTxBytes, 0, length-i);
        if (mTxPackets != NULL) mTxPackets->Copy(i, mTxPackets, 0, length-i);
        if (mOperations != NULL) mOperations->Copy(i, mOperations, 0, length-i);
        mBucketCount++;

        // TODO: subtract removed values from totalBytes
    }
    return NOERROR;

}

/**
 * Return interpolated data usage across the requested range. Interpolates
 * across buckets, so values may be rounded slightly.
 */
ECode CNetworkStatsHistory::GetValues(
    /* [in] */ Int64 start,
    /* [in] */ Int64 end,
    /* [in] */ INetworkStatsHistoryEntry* recycle,
    /* [out] */ INetworkStatsHistoryEntry** result)
{
    VALIDATE_NOT_NULL(result);
    return GetValues(start, end, Elastos::Core::Math::INT64_MAX_VALUE, recycle, result);
}

/**
 * Return interpolated data usage across the requested range. Interpolates
 * across buckets, so values may be rounded slightly.
 */
ECode CNetworkStatsHistory::GetValues(
    /* [in] */ Int64 start,
    /* [in] */ Int64 end,
    /* [in] */ Int64 now,
    /* [in] */ INetworkStatsHistoryEntry* recycle,
    /* [out] */ INetworkStatsHistoryEntry** result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<INetworkStatsHistoryEntry> entry;
    if (recycle != NULL)
        entry = recycle;
    else
        CNetworkStatsHistoryEntry::New((INetworkStatsHistoryEntry**)&entry);

    entry->SetBucketDuration(end - start);
    entry->SetBucketStart(start);
    entry->SetActiveTime(mActiveTime != NULL ? 0 : INetworkStatsHistoryEntry::UNKNOWN);
    entry->SetRxBytes(mRxBytes != NULL ? 0 : INetworkStatsHistoryEntry::UNKNOWN);
    entry->SetRxPackets(mRxPackets != NULL ? 0 : INetworkStatsHistoryEntry::UNKNOWN);
    entry->SetTxBytes(mTxBytes != NULL ? 0 : INetworkStatsHistoryEntry::UNKNOWN);
    entry->SetTxPackets(mTxPackets != NULL ? 0 : INetworkStatsHistoryEntry::UNKNOWN);
    entry->SetOperations(mOperations != NULL ? 0 : INetworkStatsHistoryEntry::UNKNOWN);

    Int32 startIndex;
    GetIndexAfter(end, &startIndex);

    for (Int32 i = startIndex; i >= 0; i--) {
        const Int64 curStart = (*mBucketStart)[i];
        const Int64 curEnd = curStart + mBucketDuration;

        // bucket is older than request; we're finished
        if (curEnd <= start) break;
        // bucket is newer than request; keep looking
        if (curStart >= end) continue;

        // include full value for active buckets, otherwise only fractional
        const Boolean activeBucket = (curStart < now) && (curEnd > now);
        Int64 overlap;
        if (activeBucket) {
            overlap = mBucketDuration;
        } else {
            const Int64 overlapEnd = curEnd < end ? curEnd : end;
            const Int64 overlapStart = curStart > start ? curStart : start;
            overlap = overlapEnd - overlapStart;
        }
        if (overlap <= 0) continue;

        // integer math each time is faster than floating point
        Int64 ori_activeTime;
        entry->GetActiveTime(&ori_activeTime);
        Int64 ori_rxBytes;
        entry->GetRxBytes(&ori_rxBytes);
        Int64 ori_rxPackets;
        entry->GetRxPackets(&ori_rxPackets);
        Int64 ori_txBytes;
        entry->GetTxBytes(&ori_txBytes);
        Int64 ori_txPackets;
        entry->GetTxPackets(&ori_txPackets);
        Int64 ori_operations;
        entry->GetOperations(&ori_operations);
        if (mActiveTime != NULL) entry->SetActiveTime(ori_activeTime + ((*mActiveTime)[i] *  overlap / mBucketDuration));
        if (mRxBytes != NULL) entry->SetRxBytes(ori_rxBytes + ((*mRxBytes)[i] *  overlap / mBucketDuration));
        if (mRxPackets != NULL) entry->SetRxPackets(ori_rxPackets + ((*mRxPackets)[i] *  overlap / mBucketDuration));
        if (mTxBytes != NULL) entry->SetTxBytes(ori_txBytes + ((*mTxBytes)[i] *  overlap / mBucketDuration));
        if (mTxPackets != NULL) entry->SetTxPackets(ori_txPackets + ((*mTxPackets)[i] *  overlap / mBucketDuration));
        if (mOperations != NULL) entry->SetOperations(ori_operations + ((*mOperations)[i] *  overlap / mBucketDuration));
    }
    *result = entry;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * @deprecated only for temporary testing
 */
//@Deprecated
ECode CNetworkStatsHistory::GenerateRandom(
    /* [in] */ Int64 start,
    /* [in] */ Int64 end,
    /* [in] */ Int64 bytes)
{
    AutoPtr<IRandom> random;
    CRandom:: New((IRandom**)&random);
    Float fractionRx;
    random->NextFloat(&fractionRx);
    Int64 rxBytes = (long) (bytes * fractionRx);
    Int64 txBytes = (long) (bytes * (1 - fractionRx));

    Int64 rxPackets = rxBytes / 1024;
    Int64 txPackets = txBytes / 1024;
    Int64 operations = rxBytes / 2048;

    return GenerateRandom(start, end, rxBytes, rxPackets, txBytes, txPackets, operations, random);
}

/**
 * @deprecated only for temporary testing
 */
//@Deprecated
ECode CNetworkStatsHistory::GenerateRandom(
    /* [in] */ Int64 start,
    /* [in] */ Int64 end,
    /* [in] */ Int64 rxBytes,
    /* [in] */ Int64 rxPackets,
    /* [in] */ Int64 txBytes,
    /* [in] */ Int64 txPackets,
    /* [in] */ Int64 operations,
    /* [in] */ IRandom* r)
{
    EnsureBuckets(start, end);

    AutoPtr<INetworkStatsEntry> entry;
    CNetworkStatsEntry::New(INetworkStats::IFACE_ALL, INetworkStats::UID_ALL, INetworkStats::SET_DEFAULT, INetworkStats::TAG_NONE, 0L, 0L, 0L, 0L, 0L, (INetworkStatsEntry**)&entry);

    while (rxBytes > 1024 || rxPackets > 128 || txBytes > 1024 || txPackets > 128
            || operations > 32) {
        /*const*/ Int64 curStart;
        RandomLong(r, start, end, &curStart);
        /*const*/ Int64 curEnd;
        Int64 endRandom;
        RandomLong(r, 0, (end - curStart) / 2, &endRandom);
        curEnd = curStart + endRandom;

        Int64 RxBytesRandom;
        RandomLong(r, 0, rxBytes, &RxBytesRandom);
        Int64 RxPacketsRandom;
        RandomLong(r, 0, rxPackets, &RxPacketsRandom);
        Int64 TxBytesRandom;
        RandomLong(r, 0, txBytes, &TxBytesRandom);
        Int64 TxPacketsRandom;
        RandomLong(r, 0, txPackets, &TxPacketsRandom);
        Int64 OperationsRandom;
        RandomLong(r, 0, operations, &OperationsRandom);

        entry->SetRxBytes(RxBytesRandom);
        entry->SetRxPackets(RxPacketsRandom);
        entry->SetTxBytes(TxBytesRandom);
        entry->SetTxPackets(TxPacketsRandom);
        entry->SetOperations(OperationsRandom);

        Int64 ori_rxBytes;
        entry->GetRxBytes(&ori_rxBytes);
        Int64 ori_rxPackets;
        entry->GetRxPackets(&ori_rxPackets);
        Int64 ori_txBytes;
        entry->GetTxBytes(&ori_txBytes);
        Int64 ori_txPackets;
        entry->GetTxPackets(&ori_txPackets);
        Int64 ori_operations;
        entry->GetOperations(&ori_operations);

        rxBytes = rxBytes - ori_rxBytes;
        rxPackets = rxPackets - ori_rxPackets;
        txBytes = txBytes - ori_txBytes;
        txPackets = txPackets - ori_txPackets;
        operations = operations - ori_operations;

        RecordData(curStart, curEnd, entry);
    }
    return NOERROR;
}

ECode CNetworkStatsHistory::RandomLong(
    /* [in] */ IRandom* r,
    /* [in] */ Int64 start,
    /* [in] */ Int64 end,
    /* [out] */ Int64* result)
{
    Float nextFloat;
    r->NextFloat(&nextFloat);
    *result = (Int64) (start + (nextFloat * (end - start)));
    return NOERROR;
}

ECode CNetworkStatsHistory::Dump(
     /* [in] */ IIndentingPrintWriter* pw,
     /* [in] */ Boolean fullHistory)
{
    // pw->Print("NetworkStatsHistory: bucketDuration="); pw->Println(bucketDuration);
    // pw->IncreaseIndent();

    // const Int32 start = fullHistory ? 0 : Elastos::Core::Math::Max(0, mBucketCount - 32);
    // if (start > 0) {
    //     pw->Print("(omitting "); pw->Print(start); pw->Println(" buckets)");
    // }

    // for (Int32 i = start; i < mBucketCount; i++) {
    //     pw->Print("bucketStart="); pw->Print((*mBucketStart)[i]);
    //     if (mActiveTime != NULL) { pw->Print(" activeTime="); pw->Print((*mActiveTime)[i]); }
    //     if (mRxBytes != NULL) { pw->Print(" rxBytes="); pw->Print((*mRxBytes)[i]); }
    //     if (mRxPackets != NULL) { pw->Print(" rxPackets="); pw->Print((*mRxPackets)[i]); }
    //     if (mTxBytes != NULL) { pw->Print(" txBytes="); pw->Print((*mTxBytes)[i]); }
    //     if (mTxPackets != NULL) { pw->Print(" txPackets="); pw->Print((*mTxPackets)[i]); }
    //     if (mOperations != NULL) { pw->Print(" operations="); pw->Print((*mOperations)[i]); }
    //     pw->Println();
    // }

    // pw->DecreaseIndent();
    //return NOERROR;
    assert(0);
    return E_NOT_IMPLEMENTED;
}

//@Override
ECode CNetworkStatsHistory::ToString(
    /* [out] */ String* result)
{
    //AutoPtr<Elastos::IO::ICharArrayWriter> writer;
    //Elastos::IO::CCharArrayWriter::New((Elastos::IO::ICharArrayWriter**)&writer);
    //Dump(new IndentingPrintWriter(writer, "  "), FALSE);
    //*result = writer.toString();
    //return NOERROR;
    assert(0);
    return E_NOT_IMPLEMENTED;
}

//@Override
ECode CNetworkStatsHistory::WriteToParcel(
    /* [in] */ IParcel* deset)
{
    VALIDATE_NOT_NULL(deset);
    deset->WriteInt64(mBucketDuration);
    ParcelUtils::WriteLongArray(deset, mBucketStart, mBucketCount);
    ParcelUtils::WriteLongArray(deset, mActiveTime, mBucketCount);
    ParcelUtils::WriteLongArray(deset, mRxBytes, mBucketCount);
    ParcelUtils::WriteLongArray(deset, mRxPackets, mBucketCount);
    ParcelUtils::WriteLongArray(deset, mTxBytes, mBucketCount);
    ParcelUtils::WriteLongArray(deset, mTxPackets, mBucketCount);
    ParcelUtils::WriteLongArray(deset, mOperations, mBucketCount);
    deset->WriteInt64(mTotalBytes);
    return NOERROR;
}

ECode CNetworkStatsHistory::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    VALIDATE_NOT_NULL(source);

    source->ReadInt64(&mBucketDuration);
    ParcelUtils::ReadLongArray(source, (ArrayOf<Int64>**)&mBucketStart);
    ParcelUtils::ReadLongArray(source, (ArrayOf<Int64>**)&mActiveTime);
    ParcelUtils::ReadLongArray(source, (ArrayOf<Int64>**)&mRxBytes);
    ParcelUtils::ReadLongArray(source, (ArrayOf<Int64>**)&mRxPackets);
    ParcelUtils::ReadLongArray(source, (ArrayOf<Int64>**)&mTxBytes);
    ParcelUtils::ReadLongArray(source, (ArrayOf<Int64>**)&mTxPackets);
    ParcelUtils::ReadLongArray(source, (ArrayOf<Int64>**)&mOperations);
    mBucketCount = mBucketStart->GetLength();
    source->ReadInt64(&mTotalBytes);
    return NOERROR;
}

ECode CNetworkStatsHistory::GetBucketCount(
    /* [out] */ Int32* bucketCount)
{
    VALIDATE_NOT_NULL(bucketCount);
    *bucketCount = mBucketCount;
    return NOERROR;
}

ECode CNetworkStatsHistory::GetBucketStart(
    /* [out, callee] */ ArrayOf<Int64>** bucketStart)
{
    VALIDATE_NOT_NULL(bucketStart);
    *bucketStart = mBucketStart;
    REFCOUNT_ADD(*bucketStart);
    return NOERROR;
}

ECode CNetworkStatsHistory::GetRxBytes(
    /* [out, callee] */ ArrayOf<Int64>** rxBytes)
{
    VALIDATE_NOT_NULL(rxBytes);
    *rxBytes = mRxBytes;
    REFCOUNT_ADD(*rxBytes);
    return NOERROR;
}

ECode CNetworkStatsHistory::GetRxPackets(
    /* [out, callee] */ ArrayOf<Int64>** rxPackets)
{
    VALIDATE_NOT_NULL(rxPackets);
    *rxPackets = mRxPackets;
    REFCOUNT_ADD(*rxPackets);
    return NOERROR;
}

ECode CNetworkStatsHistory::GetTxBytes(
    /* [out, callee] */ ArrayOf<Int64>** txBytes)
{
    VALIDATE_NOT_NULL(txBytes);
    *txBytes = mTxBytes;
    REFCOUNT_ADD(*txBytes);
    return NOERROR;
}

ECode CNetworkStatsHistory::GetTxPackets(
    /* [out, callee] */ ArrayOf<Int64>** txPackets)
{
    VALIDATE_NOT_NULL(txPackets);
    *txPackets = mTxPackets;
    REFCOUNT_ADD(*txPackets);
    return NOERROR;
}

ECode CNetworkStatsHistory::GetOperations(
    /* [out, callee] */ ArrayOf<Int64>** operations)
{
    VALIDATE_NOT_NULL(operations);
    *operations = mOperations;
    REFCOUNT_ADD(*operations);
    return NOERROR;
}

Int64 CNetworkStatsHistory::GetLong(
    /* [in] */ ArrayOf<Int64>* array,
    /* [in] */ Int32 i,
    /* [in] */ Int64 value)
{
    return array != NULL ? (*array)[i] : value;
}

void CNetworkStatsHistory::SetLong(
    /* [in] */ ArrayOf<Int64>* array,
    /* [in] */ Int32 i,
    /* [in] */ Int64 value)
{
    if (array != NULL) array->Set(i, value);
}

void CNetworkStatsHistory::AddLong(
    /* [in] */ ArrayOf<Int64>* array,
    /* [in] */ Int32 i,
    /* [in] */ Int64 value)
{
    if (array != NULL) array->Set(i, (*array)[i] + value);
}

ECode CNetworkStatsHistory::EstimateResizeBuckets(
    /* [in] */ Int64 newBucketDuration,
    /* [out] */ Int32* result)
{
    Int32 size;
    GetSize(&size);
    Int64 bucketDuration;
    GetBucketDuration(&bucketDuration);
    *result = (Int32) (size * bucketDuration / newBucketDuration);
    return NOERROR;
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
