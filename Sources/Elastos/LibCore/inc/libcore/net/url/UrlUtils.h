#ifndef __LIBCORE_NET_URI_URLUTILS_H__
#define __LIBCORE_NET_URI_URLUTILS_H__

#include <eltypes.h>

namespace Libcore {
namespace Net {

class UrlUtils
{
public:
    static String CanonicalizePath(
        /* [in] */ const String& path,
        /* [in] */ Boolean discardRelativePrefix);

    static String AuthoritySafePath(
        /* [in] */ const String& authority,
        /* [in] */ const String& path);

    static String GetSchemePrefix(
        /* [in] */ const String& spec);

    static Boolean IsValidSchemeChar(
        /* [in] */ Int32 index,
        /* [in] */ Byte c);

    static Int32 FindFirstOf(
        /* [in] */ const String& string,
        /* [in] */ const String& chars,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

private:
    UrlUtils() {}
};

} // namespace Net
} // namespace Libcore

#endif // __LIBCORE_NET_URI_URLUTILS_H__
