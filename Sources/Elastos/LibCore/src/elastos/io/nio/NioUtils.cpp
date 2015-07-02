
#include "NioUtils.h"
#include "DirectByteBuffer.h"
#include "MappedByteBuffer.h"
#include "FileChannelImpl.h"
#include "CByteArrayBuffer.h"

namespace Elastos {
namespace IO {

ECode NioUtils::FreeDirectBuffer(
    /* [in] */ IByteBuffer* buffer)
{
    assert(0 && "TODO");
    // if (buffer == null) {
    //     return;
    // }
    // ((DirectByteBuffer) buffer).free();
}

AutoPtr<IFileDescriptor> NioUtils::GetFD(
    /* [in] */ IFileChannel* fc)
{
    AutoPtr<IFileDescriptor> outfd;
    IFileChannelImpl::Probe(fc)->GetFD((IFileDescriptor**)&outfd);
    return outfd;
}

AutoPtr<IFileChannel> NioUtils::NewFileChannel(
    /* [in] */ ICloseable *stream,
    /* [in] */ IFileDescriptor *fd,
    /* [in] */ Int32 mode)
{
    AutoPtr<IFileChannel> res = (IFileChannel *) new FileChannelImpl(stream, fd, mode);
    return res;
}

AutoPtr<ArrayOf<Byte> > NioUtils::GetUnsafeArray(
    /* [in] */ IByteBuffer* b)
{
    return ((CByteArrayBuffer*)b)->mBackingArray;
}

Int32 NioUtils::GetUnsafeArrayOffset(
    /* [in] */ IByteBuffer* b)
{
    return ((CByteArrayBuffer*)b)->mArrayOffset;
}

} // namespace IO
} // namespace Elastos
