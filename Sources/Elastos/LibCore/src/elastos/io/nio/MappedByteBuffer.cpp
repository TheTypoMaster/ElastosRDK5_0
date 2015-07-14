
#include "MappedByteBuffer.h"
#include "CLibcore.h"
#include "OsConstants.h"

using Elastos::IO::Channels::FileChannelMapMode_READ_ONLY;
using Elastos::IO::Channels::FileChannelMapMode_READ_WRITE;
using Libcore::IO::CLibcore;
using Libcore::IO::ILibcore;
using Libcore::IO::IOs;
using Elastos::Droid::System::OsConstants;

namespace Elastos {
namespace IO {

MappedByteBuffer::MappedByteBuffer()
{}

MappedByteBuffer::MappedByteBuffer(
    /* [in] */ MemoryBlock* block,
    /* [in] */ Int32 capacity,
    /* [in] */ FileChannelMapMode mapMode,
    /* [in] */ Int64 effectiveDirectAddress)
    : ByteBuffer(capacity, effectiveDirectAddress)
    , mBlock(block)
    , mMapMode(mapMode)
{
}

ECode MappedByteBuffer::IsLoaded(
    /* [out] */ Boolean* isLoaded)
{
    VALIDATE_NOT_NULL(isLoaded)
    Int64 address = mBlock->ToInt64();
    Int64 size = mBlock->GetSize();
    if (size == 0) {
        *isLoaded = TRUE;
        return NOERROR;
    }

    // try {
    *isLoaded = FALSE;
    Int32 pageSize = 0;
    Int64 tmpSize = 0;
    FAIL_RETURN(CLibcore::sOs->Sysconf(OsConstants::__SC_PAGE_SIZE, &tmpSize))
    pageSize = (Int32) tmpSize;
    Int32 pageOffset = (Int32) (address % pageSize);
    address -= pageOffset;
    size += pageOffset;
    Int32 pageCount = (Int32) ((size + pageSize - 1) / pageSize);
    AutoPtr< ArrayOf<Byte> > vector = ArrayOf<Byte>::Alloc(pageCount);
    FAIL_RETURN(CLibcore::sOs->Mincore(address, size, vector))
    for (Int32 i = 0; i < vector->GetLength(); ++i) {
        if (((*vector)[i] & 1) != 1) {
            *isLoaded = FALSE;
            return NOERROR;
        }
    }
    *isLoaded = TRUE;
    return NOERROR;
    // } catch (ErrnoException errnoException) {
    //     return false;
    // }
}

ECode MappedByteBuffer::Load()
{
    // try {
    ASSERT_SUCCEEDED(CLibcore::sOs->Mlock(mBlock->ToInt64(), mBlock->GetSize()))
    ASSERT_SUCCEEDED(CLibcore::sOs->Munlock(mBlock->ToInt64(), mBlock->GetSize()))
    // } catch (ErrnoException ignored) {
    // }
    return NOERROR;
}

ECode MappedByteBuffer::Force()
{
    if (mMapMode == FileChannelMapMode_READ_WRITE) {
    //     try {
        Int32 MS_SYNC = OsConstants::_MS_SYNC;
        FAIL_RETURN(CLibcore::sOs->Msync(mBlock->ToInt64(), mBlock->GetSize(), MS_SYNC))
    //     } catch (ErrnoException errnoException) {
    //         // The RI doesn't throw, presumably on the assumption that you can't get into
    //         // a state where msync(2) could return an error.
    //         throw new AssertionError(errnoException);
    //     }
    }
    return NOERROR;
}

ECode MappedByteBuffer::CheckIsMapped()
{
    if (mMapMode == NULL) {
        // throw new UnsupportedOperationException();
        return E_UNSUPPORTED_OPERATION_EXCEPTION;
    }
    return NOERROR;
}

} // namespace IO
} // namespace Elastos
