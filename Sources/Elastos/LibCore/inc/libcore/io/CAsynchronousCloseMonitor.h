#ifndef __CASYNCHRONOUSCLOSEMONITOR_H__
#define __CASYNCHRONOUSCLOSEMONITOR_H__

#include "Singleton.h"
#include "_Libcore_IO_CAsynchronousCloseMonitor.h"

using Elastos::IO::IFileDescriptor;

namespace Libcore {
namespace IO {

CarClass(CAsynchronousCloseMonitor)
    , public Singleton
    , public IAsynchronousCloseMonitor
{
public:
    CAR_SINGLETON_DECL()

    CAR_INTERFACE_DECL()

    CARAPI SignalBlockedThreads(
        /* [in] */ IFileDescriptor* fd);
};

} // namespace IO
} // namespace Libcore

#endif