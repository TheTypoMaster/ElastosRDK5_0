#include "UrlUtils.h"

namespace Libcore {
namespace Net {

String UrlUtils::CanonicalizePath(
    /* [in] */ const String& path,
    /* [in] */ Boolean discardRelativePrefix)
{
    // the first character of the current path segment
    Int32 segmentStart = 0;

    // the number of segments seen thus far that can be erased by sequences of '..'.
    Int32 deletableSegments = 0;

    for (Int32 i = 0; i <= path.GetLength(); ) {
        Int32 nextSegmentStart;
        if (i == path.GetLength()) {
            nextSegmentStart = i;
        } else if (path.GetChar(i) == '/') {
            nextSegmentStart = i + 1;
        } else {
            i++;
            continue;
        }

        /*
         * We've encountered either the end of a segment or the end of the
         * complete path. If the final segment was "." or "..", remove the
         * appropriate segments of the path.
         */
        if (i == segmentStart + 1 && path.RegionMatches(segmentStart, String("."), 0, 1)) {
            // Given "abc/def/./ghi", remove "./" to get "abc/def/ghi".
            path = path.Substring(0, segmentStart) + path.Substring(nextSegmentStart);
            i = segmentStart;
        } else if (i == segmentStart + 2 && path.RegionMatches(segmentStart, String(".."), 0, 2)) {
            if (deletableSegments > 0 || discardRelativePrefix) {
                // Given "abc/def/../ghi", remove "def/../" to get "abc/ghi".
                deletableSegments--;
                Int32 prevSegmentStart = path.LastIndexOf('/', segmentStart - 2) + 1;
                path = path.Substring(0, prevSegmentStart) + path.Substring(nextSegmentStart);
                i = segmentStart = prevSegmentStart;
            } else {
                // There's no segment to delete; this ".." segment must be retained.
                i++;
                segmentStart = i;
            }
        } else {
            if (i > 0) {
                deletableSegments++;
            }
            i++;
            segmentStart = i;
        }
    }
    return path;
}

String UrlUtils::AuthoritySafePath(
    /* [in] */ const String& authority,
    /* [in] */ const String& path)
{
    if (!authority.IsNull() &&
            !authority.IsEmpty() &&
            !path.IsEmpty() &&
            !path.StartWith(String("/")))
    {
        return String("/") + path;
    }
    return path;
}

String UrlUtils::GetSchemePrefix(
    /* [in] */const String& spec)
{
    Int32 colon = spec.IndexOf(':');

    if (colon < 1) {
        return String(NULL);
    }

    AutoPtr<ArrayOf<Char32> > char32Array = spec.GetChars();
    for (int i = 0; i < colon; i++) {
        if (!IsValidSchemeChar(i, (Byte)(*char32Array)[i])) {
            return String(NULL);
        }
    }

    String result = spec.Substring(0, colon).ToLowerCase();
    return result;
}

Boolean UrlUtils::IsValidSchemeChar(
    /* [in] */ Int32 index,
    /* [in] */ Byte c)
{
    if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
        return TRUE;
    }
    if (index > 0 && ((c >= '0' && c <= '9') || c == '+' || c == '-' || c == '.')) {
        return TRUE;
    }
    return FALSE;
}

Int32 UrlUtils::FindFirstOf(
    /* [in] */ const String& string,
    /* [in] */ const String& chars,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end)
{
    AutoPtr<ArrayOf<Char32> > char32Array = string.GetChars();
    for (Int32 i = start; i < end; i++) {
        if (chars.IndexOf((*char32Array)[i]) != -1) {
            return i;
        }
    }
    return end;
}

} // namespace Net
} // namespace Libcore
