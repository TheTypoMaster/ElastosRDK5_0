
#include "coredef.h"
#include "CCodingErrorAction.h"

namespace Elastos {
namespace IO {
namespace Charset {

AutoPtr<ICodingErrorAction> CreateAction(
    /* [in] */ const String& action)
{
    AutoPtr<CCodingErrorAction> p;
    CCodingErrorAction::NewByFriend((CCodingErrorAction**)&p);
    p->mAction = action;
    return (ICodingErrorAction*)p.Get();
}

const AutoPtr<ICodingErrorAction> CCodingErrorAction::IGNORE = CreateAction(String("IGNORE"));
const AutoPtr<ICodingErrorAction> CCodingErrorAction::REPLACE = CreateAction(String("REPLACE"));
const AutoPtr<ICodingErrorAction> CCodingErrorAction::REPORT = CreateAction(String("REPORT"));

ECode CCodingErrorAction::constructor()
{
    return NOERROR;
}

ECode CCodingErrorAction::GetIGNORE(
    /* [out] */ ICodingErrorAction** ignore)
{
    VALIDATE_NOT_NULL(ignore);
    *ignore = IGNORE;
    INTERFACE_ADDREF(*ignore);
    return NOERROR;
}

ECode CCodingErrorAction::GetREPLACE(
    /* [out] */ ICodingErrorAction** replace)
{
    VALIDATE_NOT_NULL(replace);
    *replace = REPLACE;
    INTERFACE_ADDREF(*replace);
    return NOERROR;
}

ECode CCodingErrorAction::GetREPORT(
    /* [out] */ ICodingErrorAction** report)
{
    VALIDATE_NOT_NULL(report);
    *report = REPORT;
    INTERFACE_ADDREF(*report);
    return NOERROR;
}

ECode CCodingErrorAction::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    *str = String("Action: ");
    *str += (mAction);
    return NOERROR;
}

} // namespace Charset
} // namespace IO
} // namespace Elastos
