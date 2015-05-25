
#include "cmdef.h"
#include "BlockGuard.h"
#include <elastos/Thread.h>
#include <elastos/StringUtils.h>
#include <cutils/log.h>

using Elastos::Core::StringUtils;
using Elastos::Core::Thread;

namespace Elastos {
namespace Core {

AutoPtr<IBlockGuardPolicy> BlockGuard::sPolicy;
Object BlockGuard::sLock;
pthread_key_t BlockGuard::sTlsKey;
pthread_once_t BlockGuard::sKeyOnce = PTHREAD_ONCE_INIT;

static void ThreadDestructor(void* st)
{
    IBlockGuardPolicy* policy = static_cast<IBlockGuardPolicy*>(st);
    if (policy) {
        policy->Release();
    }
}

static void MakeKey()
{
    ASSERT_TRUE(pthread_key_create(&BlockGuard::sTlsKey, ThreadDestructor) == 0);
}

CAR_INTERFACE_IMPL(BlockGuard::LAXPOLICY, IBlockGuardPolicy);

BlockGuard::LAXPOLICY::~LAXPOLICY()
{
}

ECode BlockGuard::LAXPOLICY::OnWriteToDisk()
{
    return NOERROR;
}

ECode BlockGuard::LAXPOLICY::OnReadFromDisk()
{
    return NOERROR;
}

ECode BlockGuard::LAXPOLICY::OnNetwork()
{
    return NOERROR;
}

ECode BlockGuard::LAXPOLICY::GetPolicyMask(
    /* [out] */ Int32* mask)
{
    VALIDATE_NOT_NULL(mask);

    *mask = 0;
    return NOERROR;
}

BlockGuard::~BlockGuard()
{
}

AutoPtr<IBlockGuardPolicy> BlockGuard::GetThreadPolicy()
{
    pthread_once(&sKeyOnce, MakeKey);

    AutoPtr<IBlockGuardPolicy> policy = (IBlockGuardPolicy*)pthread_getspecific(sTlsKey);
    if (policy == NULL) {
        if (sPolicy == NULL) {
            sPolicy = new BlockGuard::LAXPOLICY();
        }

        ASSERT_TRUE(pthread_setspecific(sTlsKey, sPolicy.Get()) == 0);
        sPolicy->AddRef();
        policy = (IBlockGuardPolicy*)pthread_getspecific(sTlsKey);
    }
    assert(policy.Get() != NULL && "check BlockGuardPolicy failed!");
    return policy;
}

ECode BlockGuard::GetThreadPolicy(
    /* [out] */ IBlockGuardPolicy** policy)
{
    VALIDATE_NOT_NULL(policy);

    AutoPtr<IBlockGuardPolicy> _policy = GetThreadPolicy();
    *policy = _policy;
    INTERFACE_ADDREF(*policy);
    return NOERROR;
}

ECode BlockGuard::SetThreadPolicy(
    /* [in] */ IBlockGuardPolicy* policy)
{
    if (policy == NULL) {
        //throw new NullPointerException("policy == null");
        return E_NULL_POINTER_EXCEPTION;
    }

    AutoPtr<IBlockGuardPolicy> _policy = GetThreadPolicy();
    assert(_policy.Get() != NULL);
    if (_policy.Get() != policy) {
        _policy->Release();

        Object::Autolock locK(sLock);
        ASSERT_TRUE(pthread_setspecific(BlockGuard::sTlsKey, policy) == 0);
        INTERFACE_ADDREF(policy);
    }

    return NOERROR;
}

CARAPI BlockGuard::GetLaxPolicy(
    /* [out] */ IBlockGuardPolicy** policy)
{
    VALIDATE_NOT_NULL(policy);

    GetThreadPolicy();
    assert(sPolicy.Get() != NULL);

    *policy = sPolicy;
    INTERFACE_ADDREF(*policy);
    return NOERROR;
}

} // namespace Core
} // namespace Elastos
