
#ifdef __ELASTOS_DROID_CHROMIUM_CHROMIUM_WEBICONDATABASEADAPTER_H__
#define __ELASTOS_DROID_CHROMIUM_CHROMIUM_WEBICONDATABASEADAPTER_H__

// import android.content.ContentResolver;
// import android.webkit.WebIconDatabase;
// import android.webkit.WebIconDatabase.IconListener;

// import org.chromium.android_webview.AwContents;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Chromium {
namespace Chromium {

/**
 * Chromium implementation of WebIconDatabase -- big old no-op (base class is deprecated).
 */
class WebIconDatabaseAdapter
    : public WebIconDatabase
{
public:
    //@Override
    CARAPI Open(
        /* [in] */ String path);

    //@Override
    CARAPI Close();

    //@Override
    CARAPI RemoveAllIcons();

    //@Override
    CARAPI RequestIconForPageUrl(
        /* [in] */ String url,
        /* [in] */ IIconListener* listener);

    //@Override
    CARAPI BulkRequestIconForPageUrl(
        /* [in] */ IContentResolver* cr,
        /* [in] */ String where,
        /* [in] */ IIconListener* listener);

    //@Override
    CARAPI RetainIconForPageUrl(
        /* [in] */ String url);

    //@Override
    CARAPI ReleaseIconForPageUrl(
        /* [in] */ String url);
};

} // namespace Chromium
} // namespace Chromium
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_CHROMIUM_CHROMIUM_WEBICONDATABASEADAPTER_H__
