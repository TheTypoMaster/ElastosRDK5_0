#include "ext/frameworkdef.h"
#include "CMediaBrowserServiceBrowserRoot.h"

namespace Elastos {
namespace Droid {
namespace Service {
namespace Media {

CAR_OBJECT_IMPL(CMediaBrowserServiceBrowserRoot)

CMediaBrowserServiceBrowserRoot::CMediaBrowserServiceBrowserRoot()
	: mRootId(String(NULL))
{
}

/**
 * Constructs a browser root.
 * @param rootId The root id for browsing.
 * @param extras Any extras about the browser service.
 */
ECode CMediaBrowserServiceBrowserRoot::constructor(
    /* [in] */ const String& rootId,
    /* [in] */ IBundle * extras)
{
    if (rootId == NULL) {
        // throw new IllegalArgumentException("The root id in BrowserRoot cannot be null. " +
        //         "Use null for BrowserRoot instead.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mRootId = rootId;
    mExtras = extras;
    return NOERROR;
}

/**
 * Gets the root id for browsing.
 */
ECode CMediaBrowserServiceBrowserRoot::GetRootId(
    /* [out] */ String * result)
{
    VALIDATE_NOT_NULL(result)
    *result = mRootId;
    return NOERROR;
}

/**
 * Gets any extras about the brwoser service.
 */
ECode CMediaBrowserServiceBrowserRoot::GetExtras(
    /* [out] */ IBundle ** result)
{
    VALIDATE_NOT_NULL(result)
    *result = mExtras;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

} // namespace Media
} // namespace Service
} // namepsace Droid
} // namespace Elastos
