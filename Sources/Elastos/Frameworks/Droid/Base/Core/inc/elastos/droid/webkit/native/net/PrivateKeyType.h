// wuweizuo automatic build .h file from .java file.

#ifndef _ELASTOS_DROID_WEBKIT_NET_PRIVATEKEYTYPE_H_
#define _ELASTOS_DROID_WEBKIT_NET_PRIVATEKEYTYPE_H_

#include "elatypes.h"
#include "elautoptr.h"
#include "ext/frameworkext.h"

// package org.chromium.net;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

class PrivateKeyType
{
public:
    static const Int32 RSA = 0;
    static const Int32 DSA = 1;
    static const Int32 ECDSA = 2;
    static const Int32 INVALID = 255;
};

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_NET_PRIVATEKEYTYPE_H_

