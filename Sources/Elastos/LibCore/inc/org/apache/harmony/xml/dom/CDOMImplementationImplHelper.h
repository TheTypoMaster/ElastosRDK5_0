
#ifndef __ORG_APACHE_HARMONY_XML_DOM_CDOMIMPLEMENTATIONIMPLHELPER_H__
#define __ORG_APACHE_HARMONY_XML_DOM_CDOMIMPLEMENTATIONIMPLHELPER_H__

#include "_Org_Apache_Harmony_Xml_Dom_CDOMImplementationImplHelper.h"

namespace Org {
namespace Apache {
namespace Harmony {
namespace Xml {
namespace Dom {

CarClass(CDOMImplementationImplHelper)
{
public:
    CARAPI GetInstance(
        /* [out] */ Org::W3c::Dom::IDOMImplementation ** ppInstance);

private:
    // TODO: Add your private member variables here.
};

}
}
}
}
}

#endif // __ORG_APACHE_HARMONY_XML_DOM_CDOMIMPLEMENTATIONIMPLHELPER_H__
