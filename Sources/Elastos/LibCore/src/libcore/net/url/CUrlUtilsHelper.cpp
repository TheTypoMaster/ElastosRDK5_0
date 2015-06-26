
#include "CUrlUtilsHelper.h"
#include "UrlUtils.h"

namespace Libcore {
namespace Net {
namespace Url {

CAR_INTERFACE_IMPL(CUrlUtilsHelper, Singleton, IUrlUtilsHelper);

CAR_SINGLETON_IMPL(CUrlUtilsHelper);

ECode CUrlUtilsHelper::CanonicalizePath(
    /* [in] */ const String& path,
    /* [in] */ Boolean discardRelativePrefix,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = UrlUtils::CanonicalizePath(path, discardRelativePrefix);
    return NOERROR;
}

ECode CUrlUtilsHelper::AuthoritySafePath(
    /* [in] */ const String& authority,
    /* [in] */ const String& path,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = UrlUtils::AuthoritySafePath(authority, path);
    return NOERROR;
}

ECode CUrlUtilsHelper::GetSchemePrefix(
    /* [in] */ const String& spec,
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str)

    *str = UrlUtils::GetSchemePrefix(spec);
    return NOERROR;
}

ECode CUrlUtilsHelper::IsValidSchemeChar(
    /* [in] */ Int32 index,
    /* [in] */ Char32 c,
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = UrlUtils::IsValidSchemeChar(index, c);
    return NOERROR;
}

ECode CUrlUtilsHelper::FindFirstOf(
    /* [in] */ const String& string,
    /* [in] */ const String& chars,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)

    *value = UrlUtils::FindFirstOf(string, chars, start, end);
    return NOERROR;
}

} // namespace Url
} // namespace Net
} // namespace Libcore
