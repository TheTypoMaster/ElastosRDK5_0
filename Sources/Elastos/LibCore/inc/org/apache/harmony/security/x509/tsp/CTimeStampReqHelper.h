
#ifndef __ORG_APACHE_HARMONY_SECURITY_X509_TSP_CTIMESTAMPREQHELPER_H__
#define __ORG_APACHE_HARMONY_SECURITY_X509_TSP_CTIMESTAMPREQHELPER_H__

#include "_CTimeStampReqHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace X509 {
namespace Tsp {

CarClass(CTimeStampReqHelper)
{
public:
    CARAPI GetASN1(
        /* [out] */ Org::Apache::Harmony::Security::Asn1::IASN1Sequence ** ppAsn1);

    CARAPI SetASN1(
        /* [in] */ Org::Apache::Harmony::Security::Asn1::IASN1Sequence * pAsn1);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_SECURITY_X509_TSP_CTIMESTAMPREQHELPER_H__
