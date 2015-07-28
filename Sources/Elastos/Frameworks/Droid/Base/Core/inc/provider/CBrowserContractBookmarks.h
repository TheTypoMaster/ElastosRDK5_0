
#ifndef __CBROWSERCONTRACTBOOKMARKS_H__
#define __CBROWSERCONTRACTBOOKMARKS_H__

#include "_CBrowserContractBookmarks.h"

namespace Elastos {
namespace Droid {
namespace Provider {

CarClass(CBrowserContractBookmarks)
{
public:
    /**
     * The content:// style URI for this table
     */
    CARAPI GetCONTENT_URI(
        /* [out] */ IUri** uri);

    /**
     * The content:// style URI for the default folder
     * @hide
     */
    CARAPI GetCONTENT_URIDEFAULTFOLDER(
        /* [out] */ IUri** uri);

    /**
     * This utility class cannot be instantiated.
     */
    CARAPI constructor();

    /**
     * Builds a URI that points to a specific folder.
     * @param folderId the ID of the folder to point to
     * @hide
     */
    CARAPI BuildFolderUri(
        /* [in] */ Int64 folderId,
        /* [out] */ IUri** uri);
};

}
}
}

#endif //__CBROWSERCONTRACTBOOKMARKS_H__