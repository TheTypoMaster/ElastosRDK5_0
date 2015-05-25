
#ifndef __COBJECTIDENTIFIERINUTILS_H__
#define __COBJECTIDENTIFIERINUTILS_H__

#include "_CObjectIdentifierInUtils.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Security {
namespace Utils {

CarClass(CObjectIdentifierInUtils)
{
public:
    CARAPI GetOid(
        /* [out, callee] */ ArrayOf<Int32> ** ppOid);

    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI GetGroup(
        /* [out] */ IObject ** ppGroup);

    CARAPI Equals(
        /* [in] */ IInterface * pO,
        /* [out] */ Boolean * pResult);

    CARAPI ToOIDString(
        /* [out] */ String * pStr);

    CARAPI ToString(
        /* [out] */ String * pStr);

    CARAPI GetHashCode(
        /* [out] */ Int32 * pHashCode);

    CARAPI constructor(
        /* [in] */ ArrayOf<Int32> * pOid);

    CARAPI constructor(
        /* [in] */ ArrayOf<Int32> * pOid,
        /* [in] */ const String& name,
        /* [in] */ IObject * pOidGroup);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __COBJECTIDENTIFIERINUTILS_H__