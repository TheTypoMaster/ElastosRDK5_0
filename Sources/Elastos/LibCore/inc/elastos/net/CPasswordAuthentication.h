
#ifndef __ELASTOS_NET_CPASSWORDAUTHENTICATION_H__
#define __ELASTOS_NET_CPASSWORDAUTHENTICATION_H__

#include "_ELASTOS_NET_CPasswordAuthentication.h"
#include <elastos/core/Object.h>


namespace Elastos {
namespace Net {

CarClass(CPasswordAuthentication)
    , public Object
    , public IPasswordAuthentication
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    ~CPasswordAuthentication();

    CARAPI constructor(
        /* [in] */ const String& userName,
        /* [in] */ const ArrayOf<Char32>& password);

    CARAPI GetPassword(
        /* [out, callee] */ ArrayOf<Char32>** password);

    CARAPI GetUserName(
        /* [out] */ String* userName);

private:
    String mUserName;

    AutoPtr<ArrayOf<Char32> > mPassword;
};

} // namespace Net
} // namespace Elastos

#endif //__ELASTOS_NET_CPASSWORDAUTHENTICATION_H__
