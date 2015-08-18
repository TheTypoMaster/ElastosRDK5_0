
#ifndef __ELASTOS_DROID_NET_INTERNAL_HTTP_MULTIPART_CFILEPARTHELPER_H__
#define __ELASTOS_DROID_NET_INTERNAL_HTTP_MULTIPART_CFILEPARTHELPER_H__

#include "_Elastos_Droid_Net_Internal_Http_Multipart_CFilePartHelper.h"

namespace Elastos {
namespace Droid {
namespace Net {
namespace Internal {
namespace Http {
namespace Multipart {

CarClass(CFilePartHelper)
{
public:
    CARAPI GetBoundary(
        /* [out] */ String* boundary);

    CARAPI SendParts(
        /* [in] */ Elastos::IO::IOutputStream* outStream,
        /* [in] */ ArrayOf<IPart*>* iparts,
        /* [in] */ ArrayOf<Byte>* partBoundary);

    CARAPI GetLengthOfParts(
        /* [in] */ ArrayOf<IPart*>* iparts,
        /* [out] */ Int64* length);

    CARAPI GetLengthOfParts(
        /* [in] */ ArrayOf<IPart*>* iparts,
        /* [in] */ ArrayOf<Byte>* partBoundary,
        /* [out] */ Int64* length);
};

}
}
}
}
}
}

#endif // __ELASTOS_DROID_NET_INTERNAL_HTTP_MULTIPART_CFILEPARTHELPER_H__
