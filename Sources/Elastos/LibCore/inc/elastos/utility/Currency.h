
#ifndef __ELASTOS_UTILITY_CURRENCY_H__
#define __ELASTOS_UTILITY_CURRENCY_H__

#include <elastos/utility/etl/HashMap.h>
#include <elastos/core/Object.h>

using Elastos::Utility::ILocale;
using Elastos::Utility::HashMap;

_ETL_NAMESPACE_BEGIN
#ifndef DEFINE_HASH_AUTOPTR_ILOCALE_USING_HASH_CODE
#define DEFINE_HASH_AUTOPTR_ILOCALE_USING_HASH_CODE(ILocale)
template<> struct Hash<AutoPtr<ILocale> >
{
    size_t operator()(const AutoPtr<ILocale> s) const
    {
        assert(s != NULL);
        Int32 hashCode;
        s->GetHashCode(&hashCode);
        return (size_t)hashCode;
    }
};

template<> struct EqualTo<AutoPtr<ILocale> >
{
    size_t operator()(const AutoPtr<ILocale> x, const AutoPtr<ILocale> y) const
    {
        assert(x != NULL);
        Boolean isEqual;
        x->Equals(y, &isEqual);
        return isEqual;
    }
};
#endif
_ETL_NAMESPACE_END

namespace Elastos{
namespace Utility{

class Currency
    : public Object
    , public ICurrency
{
public:
    CAR_INTERFACE_DECL()

    CARAPI GetCurrencyCode(
        /* [out] */ String* str);

    CARAPI GetDisplayName(
        /* [out] */ String* str);

    CARAPI GetDisplayName(
        /* [in] */ ILocale* locale,
        /* [out] */ String* str);

    CARAPI GetSymbol(
        /* [out] */ String* str);

    CARAPI GetSymbol(
        /* [in] */ ILocale* locale,
        /* [out] */ String* str);

    CARAPI GetDefaultFractionDigits(
        /* [out] */ Int32* value);

    CARAPI ToString(
        /* [out] */ String* str);

    static CARAPI_(AutoPtr<ICurrency>) GetInstance(
        /* [in] */ const String& currencyCode);

    static CARAPI GetInstance(
        /* [in] */ ILocale* locale,
        /* [out] */ ICurrency** instance);

    static CARAPI GetAvailableCurrencies(
        /* [out] */ ISet** currencies);

private:
    Currency(
        /* [in] */ const String& currencyCode);

    CARAPI_(AutoPtr<IInterface>) ReadResolve();

private:
    static HashMap<String, AutoPtr<ICurrency> > sCodesToCurrencies; // = new HashMap<String, Currency>();
    static HashMap<AutoPtr<ILocale>, AutoPtr<ICurrency> > sLocalesToCurrencies; // = new HashMap<Locale, Currency>();

    String mCurrencyCode;
};

} // namespace Utility
} // namespace Elastos

#endif //__ELASTOS_UTILITY_CURRENCY_H__
