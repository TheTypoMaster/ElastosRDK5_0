#ifndef __LIBCORE_IO_CIOUTILS_H__
#define __LIBCORE_IO_CIOUTILS_H__

#include "_Libcore_IO_CIoUtils.h"
#include "Singleton.h"

using Elastos::IO::ICloseable;
using Elastos::IO::IFileDescriptor;
using Elastos::IO::IFile;
using Elastos::Net::ISocket;

namespace Libcore {
namespace IO {

CarClass(CIoUtils)
    , public Singleton
    , public IIoUtils
{
public:
    CAR_INTERFACE_DECL()

    CAR_SINGLETON_DECL()

    CARAPI Close(
        /* [in] */ IFileDescriptor* fd);

    CARAPI CloseQuietly(
        /* [in] */ ICloseable* closeable);

    CARAPI CloseQuietly(
        /* [in] */ ISocket* socket);

    CARAPI CloseQuietly(
        /* [in] */ IFileDescriptor* fd);

    CARAPI SetBlocking(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Boolean blocking);

    CARAPI ReadFileAsByteArray(
        /* [in] */ const String& path,
        /* [out, callee] */ ArrayOf<Byte>** bytes);

    CARAPI ReadFileAsString(
        /* [in] */ const String& path,
        /* [out] */ String* result);

    // TODO: this should specify paths as Strings rather than as Files
    CARAPI DeleteContents(
        /* [in] */ IFile* dir);

    CARAPI CreateTemporaryDirectory(
        /* [in] */ const String& prefix,
        /* [out] */ IFile** file);

    CARAPI CanOpenReadOnly(
        /* [in] */ const String& path,
        /* [out] */ Boolean* result);
};


} // namespace IO
} // namespace Libcore

#endif //__LIBCORE_IO_CIOUTILS_H__
