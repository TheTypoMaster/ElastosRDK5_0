
#ifndef __ELASTOS_DROID_NET_CNETWORKPOLICYMANAGERHELPER_H__
#define __ELASTOS_DROID_NET_CNETWORKPOLICYMANAGERHELPER_H__

#include "_Elastos_Droid_Net_CNetworkPolicyManagerHelper.h"

using Elastos::Droid::Text::Format::ITime;
using Elastos::Droid::Content::IContext;
using Elastos::IO::IPrintWriter;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CNetworkPolicyManagerHelper)
{
public:
    CARAPI From(
        /* [in] */ IContext* context,
        /* [out] */ INetworkPolicyManager** result);

    CARAPI ComputeLastCycleBoundary(
        /* [in] */ Int64 currentTime,
        /* [in] */ INetworkPolicy* policy,
        /* [out] */ Int64* result);

    CARAPI ComputeNextCycleBoundary(
        /* [in] */ Int64 currentTime,
        /* [in] */ INetworkPolicy* policy,
        /* [out] */ Int64* result);

    CARAPI SnapToCycleDay(
        /* [in] */ ITime* time,
        /* [in] */ Int32 cycleDay);

    CARAPI IsUidValidForPolicy(
        /* [in] */ IContext* context,
        /* [in] */ Int32 uid,
        /* [out] */ Boolean* result);

    CARAPI DumpPolicy(
        /* [in] */ IPrintWriter* fout,
        /* [in] */ Int32 policy);

    CARAPI DumpRules(
        /* [in] */ IPrintWriter* fout,
        /* [in] */ Int32 rules);

private:
};

} // namespace Net
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_NET_CNETWORKPOLICYMANAGERHELPER_H__
