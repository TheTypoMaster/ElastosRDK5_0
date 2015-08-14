
#ifndef __ELASTOS_DROID_PROVIDER_CBROWSERCONTRACT_H__
#define __ELASTOS_DROID_PROVIDER_CBROWSERCONTRACT_H__

#include "_CBrowserContract.h"

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CBrowserContract)
{
public:
    CARAPI GetAUTHORITY_URI(
        /* [out] */ IUri** uri);
};

}
}
}
