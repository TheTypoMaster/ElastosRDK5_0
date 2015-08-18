
#ifndef __ORG_APACHE_HARMONY_XNET_PROVIDER_JSSE_CDELEGATEDTASK_H__
#define __ORG_APACHE_HARMONY_XNET_PROVIDER_JSSE_CDELEGATEDTASK_H__

#include "_Org_Apache_Harmony_Xnet_Provider_Jsse_CDelegatedTask.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CDelegatedTask)
{
public:
    CARAPI Run();

    CARAPI constructor(
        /* [in] */ Elastos::Core::IRunnable * pAction,
        /* [in] */ Org::Apache::Harmony::Xnet::Provider::Jsse::IHandshakeProtocol * pHandshaker);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_XNET_PROVIDER_JSSE_CDELEGATEDTASK_H__
