
#include "provider/CContactsSettings.h"
#include "net/Uri.h"
#include "content/CContentValues.h"
#include <elastos/coredef.h>

using Elastos::Core::CString;
using Elastos::Droid::Net::Uri;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Content::IContentValues;
using Elastos::Droid::Content::CContentValues;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CContactsSettings::constructor()
{
    return NOERROR;
}

ECode CContactsSettings::GetCONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    return Uri::Parse(String("content://contacts/settings"), uri);
}

ECode CContactsSettings::GetSetting(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& account,
    /* [in] */ const String& key,
    /* [out] */ String* setting)
{
    VALIDATE_NOT_NULL(setting);

    // For now we only support a single account and the UI doesn't know what
    // the account name is, so we're using a global setting for SYNC_EVERYTHING.
    // Some day when we add multiple accounts to the UI this should honor the account
    // that was asked for.
    String selectString;
    AutoPtr<ArrayOf<String> > selectArgs;
    if (FALSE) {
        selectString = (account.IsNull())
                ? String("_sync_account is null AND key=?")
                : String("_sync_account=? AND key=?");
//                : "_sync_account=? AND _sync_account_type=? AND key=?";
        if (account.IsNull()) {
            selectArgs = ArrayOf<String>::Alloc(1);
            (*selectArgs)[0] = key;
        }
        else {
            selectArgs = ArrayOf<String>::Alloc(2);
            (*selectArgs)[0] = account;
            (*selectArgs)[1] = key;
        }
    }
    else {
        selectString = String("key=?");
        selectArgs = ArrayOf<String>::Alloc(1);
        (*selectArgs)[0] = key;
    }
    AutoPtr<ICursor> cursor;
    AutoPtr<IUri> uri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&uri))
    AutoPtr<ArrayOf<String> > args;
    args = ArrayOf<String>::Alloc(1);
    (*args)[0] = IContactsSettingsColumns::VALUE;
    FAIL_RETURN(cr->Query(uri, args, selectString, selectArgs, String(NULL), (ICursor**)&cursor))

    //try {
    Boolean isNext;
    FAIL_GOTO(cursor->MoveToNext(&isNext), EXIT)
    if (!isNext) {
        *setting = String(NULL);
    }
    else {
        FAIL_GOTO(cursor->GetString(0, setting), EXIT)
    }
    //} finally {
EXIT:
    return cursor->Close();
    //}
}

ECode CContactsSettings::SetSetting(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& account,
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    AutoPtr<IContentValues> values;
    FAIL_RETURN(CContentValues::New((IContentValues**)&values))
    // For now we only support a single account and the UI doesn't know what
    // the account name is, so we're using a global setting for SYNC_EVERYTHING.
    // Some day when we add multiple accounts to the UI this should honor the account
    // that was asked for.
    //values.put(_SYNC_ACCOUNT, account.mName);
    //values.put(_SYNC_ACCOUNT_TYPE, account.mType);
    AutoPtr<ICharSequence> keyStr;
    FAIL_RETURN(CString::New(key, (ICharSequence**)&keyStr))
    FAIL_RETURN(values->PutString(IContactsSettingsColumns::KEY, keyStr))
    AutoPtr<ICharSequence> valueStr;
    FAIL_RETURN(CString::New(value, (ICharSequence**)&valueStr))
    FAIL_RETURN(values->PutString(IContactsSettingsColumns::VALUE, valueStr))
    AutoPtr<IUri> uri;
    FAIL_RETURN(GetCONTENT_URI((IUri**)&uri))
    Int32 result;
    return cr->Update(uri, values, String(NULL), NULL, &result);
}

} // Provider
} // Droid
} // Elastos