#ifndef __ELASTOS_DROID_OS_CPARCELFILEDESCRIPTOR_H__
#define __ELASTOS_DROID_OS_CPARCELFILEDESCRIPTOR_H__

#include "_Elastos_Droid_Os_CParcelFileDescriptor.h"
#include "os/ParcelFileDescriptor.h"

using Elastos::IO::IFile;
using Elastos::IO::IFileDescriptor;

namespace Elastos {
namespace Droid {
namespace Os {

CarClass(CParcelFileDescriptor), public ParcelFileDescriptor
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcelFileDescriptor* descriptor);

    CARAPI constructor(
        /* [in] */ IFileDescriptor* descriptor);

    CARAPI GetFileDescriptor(
        /* [out] */ IFileDescriptor** des);

    CARAPI GetStatSize(
        /* [out] */ Int64* len);

    CARAPI SeekTo(
        /* [in] */ Int64 startOffset,
        /* [out] */ Int64* toOffset);

    CARAPI GetFd(
        /* [out] */ Int32* fd);

    CARAPI Close();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_OS_CPARCELFILEDESCRIPTOR_H__
