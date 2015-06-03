#include "CStructUcred.h"

namespace Elastos {
namespace Droid {
namespace System {
CAR_OBJECT_IMPL(CStructUcred)

CAR_INTERFACE_IMPL(CStructUcred, Object, IStructUcred)

ECode CStructUcred::constructor(
    /* [in] */ Int32 pid,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
    mPid = pid;
    mUid = uid;
    mGid = gid;
    return NOERROR;
}

ECode CStructUcred::GetPid(
    /* [out] */ Int32* pid)
{
    *pid = mPid;
    return NOERROR;
}

ECode CStructUcred::GetUid(
    /* [out] */ Int32* uid)
{
    *uid = mUid;
    return NOERROR;
}

ECode CStructUcred::GetGid(
    /* [out] */ Int32* gid)
{
    *gid = mGid;
    return NOERROR;
}

}// namespace System
}// namespace Droid
}// namespace Elastos