
#include "WaitingThreadAborter.h"

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Conn {
namespace Tsccm {

void WaitingThreadAborter::Abort()
{
    mAborted = TRUE;

    if (mWaitingThread != NULL)
        mWaitingThrea->Iinterrupt();
}

void WaitingThreadAborter::SetWaitingThread(
    /* [in] */ WaitingThread* waitingThread)
{
    mWaitingThread = waitingThread;
    if (mAborted)
        mWaitingThread->Interrupt();
}


} // namespace Tsccm
} // namespace Conn
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org