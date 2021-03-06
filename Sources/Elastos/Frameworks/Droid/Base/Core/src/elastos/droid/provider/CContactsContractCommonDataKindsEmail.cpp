
#include "provider/CContactsContractCommonDataKindsEmail.h"
#include "provider/ContactsContractData.h"
#include "net/Uri.h"
#include "text/TextUtils.h"
#include "R.h"

using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsContractCommonDataKindsEmail::constructor()
{
    return NOERROR;
}

ECode CContactsContractCommonDataKindsEmail::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(ContactsContractData::GetCONTENT_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("emails"), uri);
}

ECode CContactsContractCommonDataKindsEmail::GetCONTENT_LOOKUP_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("lookup"), uri);
}

ECode CContactsContractCommonDataKindsEmail::GetCONTENT_FILTER_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUri> auUri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&auUri))
    return Uri::WithAppendedPath(auUri, String("filter"), uri);
}

ECode CContactsContractCommonDataKindsEmail::GetTypeLabelResource(
    /* [in] */ Int32 type,
    /* [out] */ Int32* resource)
{
    VALIDATE_NOT_NULL(resource);

    switch (type) {
        case TYPE_HOME: *resource = Elastos::Droid::R::string::emailTypeHome; return NOERROR;
        case TYPE_WORK: *resource = Elastos::Droid::R::string::emailTypeWork; return NOERROR;
        case TYPE_OTHER: *resource = Elastos::Droid::R::string::emailTypeOther; return NOERROR;
        case TYPE_MOBILE: *resource = Elastos::Droid::R::string::emailTypeMobile; return NOERROR;
        default: *resource = Elastos::Droid::R::string::emailTypeCustom; return NOERROR;
    }
    return NOERROR;
}

ECode CContactsContractCommonDataKindsEmail::GetTypeLabel(
    /* [in] */ IResources* res,
    /* [in] */ Int32 type,
    /* [in] */ ICharSequence* label,
    /* [out] */ ICharSequence** lb)
{
    if (type == TYPE_CUSTOM && !TextUtils::IsEmpty(label)) {
        *lb = label;
        REFCOUNT_ADD(*lb);
        return NOERROR;
    } else {
        Int32 labelRes;
        FAIL_RETURN(GetTypeLabelResource(type, &labelRes))
        return res->GetText(labelRes, lb);
    }
}

} //Provider
} //Droid
} //Elastos