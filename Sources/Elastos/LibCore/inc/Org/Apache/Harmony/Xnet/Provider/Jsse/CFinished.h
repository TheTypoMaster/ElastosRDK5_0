
#ifndef __CFINISHED_H__
#define __CFINISHED_H__

#include "_CFinished.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xnet {
namespace Provider {
namespace Jsse {

CarClass(CFinished)
{
public:
    CARAPI GetLength(
        /* [out] */ Int32 * pLength);

    CARAPI GetData(
        /* [out, callee] */ ArrayOf<Byte> ** ppData);

    CARAPI constructor(
        /* [in] */ ArrayOf<Byte> * pBytes);

    CARAPI constructor(
        /* [in] */ Org::Apache::Harmony::Xnet::Provider::Jsse::IHandshakeIODataStream * pIds,
        /* [in] */ Int32 length);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}
}

#endif // __CFINISHED_H__
