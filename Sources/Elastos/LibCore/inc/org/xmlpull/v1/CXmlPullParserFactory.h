
#ifndef __CXMLPULLPARSERFACTORY_H__
#define __CXMLPULLPARSERFACTORY_H__

#include "_Org_Xmlpull_V1_CXmlPullParserFactory.h"
#include "XmlPullParserFactory.h"

namespace Org {
namespace Xmlpull {
namespace V1 {

CarClass(CXmlPullParserFactory), public XmlPullParserFactory
{
public:
    CAR_OBJECT_DECL()
};

} // namespace V1
} // namespace Xmlpull
} // namespace Org

#endif // __CXMLPULLPARSERFACTORY_H__
