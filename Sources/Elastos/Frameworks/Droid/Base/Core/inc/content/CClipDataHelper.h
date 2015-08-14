
#ifndef __ELASTOS_DROID_CONTENT_CCLIPDATAHELPER_H__
#define __ELASTOS_DROID_CONTENT_CCLIPDATAHELPER_H__

#include "_CClipDataHelper.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Net::IUri;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CClipDataHelper)
{
public:
    /**
     * Create a new ClipData holding data of the type
     * {@link ClipDescription#MIMETYPE_TEXT_PLAIN}.
     *
     * @param label User-visible label for the clip data.
     * @param text The actual text in the clip.
     * @return Returns a new ClipData containing the specified data.
     */
    CARAPI NewPlainText(
        /* [in] */ ICharSequence* label,
        /* [in] */ ICharSequence* text,
        /* [out] */ IClipData** clipData);

    /**
     * Create a new ClipData holding data of the type
     * {@link ClipDescription#MIMETYPE_TEXT_HTML}.
     *
     * @param label User-visible label for the clip data.
     * @param text The text of clip as plain text, for receivers that don't
     * handle HTML.  This is required.
     * @param htmlText The actual HTML text in the clip.
     * @return Returns a new ClipData containing the specified data.
     */
    CARAPI NewHtmlText(
        /* [in] */ ICharSequence* label,
        /* [in] */ ICharSequence* text,
        /* [in] */ const String& htmlText,
        /* [out] */ IClipData** clipData);

    /**
     * Create a new ClipData holding an Intent with MIME type
     * {@link ClipDescription#MIMETYPE_TEXT_INTENT}.
     *
     * @param label User-visible label for the clip data.
     * @param intent The actual Intent in the clip.
     * @return Returns a new ClipData containing the specified data.
     */
    CARAPI NewIntent(
        /* [in] */ ICharSequence* label,
        /* [in] */ IIntent* intent,
        /* [out] */ IClipData** clipData);

    /**
     * Create a new ClipData holding a URI.  If the URI is a content: URI,
     * this will query the content provider for the MIME type of its data and
     * use that as the MIME type.  Otherwise, it will use the MIME type
     * {@link ClipDescription#MIMETYPE_TEXT_URILIST}.
     *
     * @param resolver ContentResolver used to get information about the URI.
     * @param label User-visible label for the clip data.
     * @param uri The URI in the clip.
     * @return Returns a new ClipData containing the specified data.
     */
    CARAPI NewUri(
        /* [in] */ IContentResolver* resolver,
        /* [in] */ ICharSequence* label,
        /* [in] */ IUri* uri,
        /* [out] */ IClipData** clipData);

    /**
     * Create a new ClipData holding an URI with MIME type
     * {@link ClipDescription#MIMETYPE_TEXT_URILIST}.
     * Unlike {@link #newUri(ContentResolver, CharSequence, Uri)}, nothing
     * is inferred about the URI -- if it is a content: URI holding a bitmap,
     * the reported type will still be uri-list.  Use this with care!
     *
     * @param label User-visible label for the clip data.
     * @param uri The URI in the clip.
     * @return Returns a new ClipData containing the specified data.
     */
    CARAPI NewRawUri(
        /* [in] */ ICharSequence* label,
        /* [in] */ IUri* uri,
        /* [out] */ IClipData** clipData);

    CARAPI GetMimeTypes(
        /* [in] */ const String& name,
        /* [out, callee] */ ArrayOf<String>** types);
};

} // Content
} // Droid
} // Elastos

#endif // __ELASTOS_DROID_CONTENT_CCLIPDATAHELPER_H__
