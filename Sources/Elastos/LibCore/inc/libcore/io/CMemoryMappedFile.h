#ifndef __CMEMORYMAPPEDFILE_H__
#define __CMEMORYMAPPEDFILE_H__

#include "_Libcore_IO_CMemoryMappedFile.h"
#include "Object.h"

using Elastos::Core::Object;
using Elastos::Core::IAutoCloseable;

namespace Libcore {
namespace IO {

CarClass(CMemoryMappedFile)
    , public Object
    , public IMemoryMappedFile
    , public IAutoCloseable
{
public:
    CAR_OBJECT_DECL()

    CAR_INTERFACE_DECL()

    CARAPI constructor(
        /* [in] */ Int64 address,
        /* [in] */ Int64 size);

    /**
     * Returns a new iterator that treats the mapped data as big-endian.
     */
    CARAPI BigEndianIterator(
        /* [out] */ IBufferIterator** it);

    /**
     * Returns a new iterator that treats the mapped data as little-endian.
     */
    CARAPI LittleEndianIterator(
        /* [out] */ IBufferIterator** it);

    /**
     * Returns the size in bytes of the memory-mapped region.
     */
    CARAPI Size(
        /* [out] */ Int64* size);

    CARAPI Close();

    static AutoPtr<IMemoryMappedFile> MmapRO(
        /* [in] */ String path);

private:
    Int64 mAddress;
    Int64 mSize;
};

} // namespace IO
} // namespace Libcore

#endif