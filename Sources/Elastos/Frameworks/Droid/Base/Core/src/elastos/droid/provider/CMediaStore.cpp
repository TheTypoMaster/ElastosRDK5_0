
#include "provider/CMediaStore.h"
#include "ext/frameworkext.h"
#include "net/CUriHelper.h"
#include "content/CContentUris.h"
#include "graphics/CBitmapFactory.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>
#include "provider/CMediaStoreVideoMedia.h"
#include "provider/CMediaStoreImagesMedia.h"
#include "media/media/CMiniThumbFile.h"

using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;
using Elastos::Droid::Content::IContentUris;
using Elastos::Droid::Content::CContentUris;
using Elastos::Droid::Os::IParcelFileDescriptor;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;
using Elastos::Droid::Media::IMiniThumbFile;
using Elastos::Droid::Media::CMiniThumbFile;

namespace Elastos {
namespace Droid {
namespace Provider {

const String CMediaStore::TAG("CMediaStore");
const String CMediaStore::CONTENT_AUTHORITY_SLASH = String("content://") + IMediaStore::AUTHORITY + "/";

ECode CMediaStore::GetMediaScannerUri(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    return helper->Parse(CONTENT_AUTHORITY_SLASH + "none/media_scanner", uri);
}

ECode CMediaStore::GetVersion(
    /* [in] */ IContext* context,
    /* [out] */ String* version)
{
    VALIDATE_NOT_NULL(version);

    AutoPtr<IContentResolver> cr;
    context->GetContentResolver((IContentResolver**)&cr);

    AutoPtr<IUriHelper> helper;
    AutoPtr<IUri> uri;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    helper->Parse(CONTENT_AUTHORITY_SLASH + "none/version", (IUri**)&uri);

    AutoPtr<ICursor> c;
    cr->Query(uri, NULL, String(NULL), NULL, String(NULL), (ICursor**)&c);
    if (c != NULL) {
        // try {
            Boolean b;
            if (c->MoveToFirst(&b),b) {
                c->GetString(0, version);
                return NOERROR;
            }
        // } finally {
            c->Close();
        // }
    }
    return NOERROR;
}

} //namespace Provider
} //namespace Droid
} //namespace Elastos
