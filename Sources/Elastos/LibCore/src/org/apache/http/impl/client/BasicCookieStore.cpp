
#include "BasicCookieStore.h"
#include "CCookieIdentityComparator.h"
#include <elastos/Logger.h>
#include <elastos/Utility/Collections.h>

using Elastos::Utility::CArrayList;
using Elastos::Utility::CDate;
using Elastos::Utility::Collections;
using Elastos::Utility::IIterable;
using Elastos::Utility::IIterator;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::Cookie::CCookieIdentityComparator;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Client {

BasicCookieStore::BasicCookieStore()
{
    CArrayList::New((IArrayList**)&mCookies);
    CCookieIdentityComparator::New((IComparator**)&mCookieComparator);
}

CAR_INTERFACE_DECL(BasicCookieStore, Object, IAuthenticationHandler)

ECode BasicCookieStore::AddCookie(
    /* [in] */ ICookie* cookie)
{
    synchronized(this) {
        if (cookie != NULL) {
            // first remove any old cookie that is equivalent
            AutoPtr<IIterable> able = IIterable::Probe(mCookies);
            AutoPtr<IIterator> it;
            able->GetIterator((IIterator**)&it);
            Boolean hasNext;
            while(it->HasNext(&hasNext), hasNext) {
                AutoPtr<IInterface> value;
                it->GetNext((IInterface**)&value);
                Int32 result;
                if (mCookieComparator->Compare(cookie, value, &result), result == 0) {
                    it->Remove();
                    break;
                }
            }
            AutoPtr<IDate> date;
            CDate::New((IDate**)&date);
            Boolean isExpired;
            if (cookie->IsExpired(date, &isExpired), !isExpired) {
                cookies->Add(cookie);
            }
        }
    }
    return NOERROR;
}

void BasicCookieStore::AddCookies(
    /* [in] */ ArrayOf<ICookie>* cookies)
{
    synchronized(this) {
        if (cookies != NULL) {
            for (Int32 i = 0; i < cookies->GetLength(); ++i) {
                AddCookie((*cookies)[i]);
            }
        }
    }
}

ECode BasicCookieStore::GetCookies(
    /* [out] */ IList** cookies)
{
    VALIDATE_NOT_NULL(cookies)
    synchronized(this) {
        Collections::UnmodifiableList(IList::Probe(mCookies), cookies);
    }
    return NOERROR;
}

ECode BasicCookieStore::ClearExpired(
    /* [in] */ IDate* date,
    /* [out] */ Boolean* purged)
{
    VALIDATE_NOT_NULL(purged)
    synchronized(this) {
        if (date == NULL) {
            *purged = FALSE;
            return NOERROR;
        }
        Boolean removed = FALSE;
        AutoPtr<IIterable> able = IIterable::Probe(mCookies);
        AutoPtr<IIterator> it;
        able->GetIterator((IIterator**)&it);
        Boolean hasNext;
        while(it->HasNext(&hasNext), hasNext) {
            AutoPtr<IInterface> value;
            it->GetNext((IInterface**)&value);
            AutoPtr<ICookie> cookie = ICookie::Probe(value);
            Boolean isExpired;
            if (cookie->IsExpired(date, &isExpired), isExpired) {
                it->Remove();
                removed = TRUE;
            }
        }
        *purged = removed;
    }
    return NOERROR;
}

ECode BasicCookieStore::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)
    return IObject::Probe(mCookies)->ToString(str);
}

ECode BasicCookieStore::Clear()
{
    synchronized(this) {
        mCookie->Clear();
    }
    return NOERROR;
}

} // namespace Client
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org