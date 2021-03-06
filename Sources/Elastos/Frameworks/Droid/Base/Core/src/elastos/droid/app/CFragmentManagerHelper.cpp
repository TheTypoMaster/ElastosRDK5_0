
#include "app/CFragmentManagerHelper.h"
#include "app/CFragmentManagerImpl.h"

namespace Elastos {
namespace Droid {
namespace App {

ECode CFragmentManagerHelper::EnableDebugLogging(
    /* [in] */ Boolean enabled)
{
    CFragmentManagerImpl::DEBUG = enabled;
    return NOERROR;
}

} // namespace App
} // namespace Droid
} // namespace Elastos
