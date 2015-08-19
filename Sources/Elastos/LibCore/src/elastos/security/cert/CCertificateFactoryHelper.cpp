
#include "CCertificateFactoryHelper.h"
#include "CCertificateFactory.h"

namespace Elastos {
namespace Security {
namespace Cert {

CAR_INTERFACE_IMPL(CCertificateFactoryHelper, Singleton, ICertificateFactoryHelper)
CAR_SINGLETON_IMPL(CCertificateFactoryHelper)
ECode CCertificateFactoryHelper::GetInstance(
    /* [in] */ const String& type,
    /* [out] */ ICertificateFactory** factory)
{
    return CCertificateFactory::GetInstance(type, factory);
}

ECode CCertificateFactoryHelper::GetInstance(
    /* [in] */ const String& type,
    /* [in] */ const String& provider,
    /* [out] */ ICertificateFactory** factory)
{
    return CCertificateFactory::GetInstance(type, provider, factory);
}

ECode CCertificateFactoryHelper::GetInstance(
    /* [in] */ const String& type,
    /* [in] */ IProvider* provider,
    /* [out] */ ICertificateFactory** factory)
{
    return CCertificateFactory::GetInstance(type, provider, factory);
}

} // end Cert
} // end Security
} // end Elastos
