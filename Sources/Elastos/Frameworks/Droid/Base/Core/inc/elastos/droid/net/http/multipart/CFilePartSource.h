
#ifndef __ELASTOS_DROID_NET_INTERNAL_HTTP_MULTIPART_CFILEPARTSOURCE_H__
#define __ELASTOS_DROID_NET_INTERNAL_HTTP_MULTIPART_CFILEPARTSOURCE_H__

#include "_Elastos_Droid_Net_Internal_Http_Multipart_CFilePartSource.h"

using Elastos::IO::IFile;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Internal {
namespace Http {
namespace Multipart {

/**
 * A PartSource that reads from a File.
 * 
 * @author <a href="mailto:becke@u.washington.edu">Michael Becke</a>
 * @author <a href="mailto:mdiggory@latte.harvard.edu">Mark Diggory</a>
 * @author <a href="mailto:mbowler@GargoyleSoftware.com">Mike Bowler</a>
 *   
 * @since 2.0 
 */
CarClass(CFilePartSource)
{
public:
    /**
     * Constructor for FilePartSource.
     * 
     * @param file the FilePart source File. 
     *
     * @throws FileNotFoundException if the file does not exist or 
     * cannot be read
     */
    CARAPI constructor(
        /* [in] */ Elastos::IO::IFile* file);

    /**
     * Constructor for FilePartSource.
     * 
     * @param fileName the file name of the FilePart
     * @param file the source File for the FilePart
     *
     * @throws FileNotFoundException if the file does not exist or 
     * cannot be read
     */
    CARAPI constructor(
        /* [in] */ const String& fileName,
        /* [in] */ Elastos::IO::IFile* file);

    /**
     * Return the length of the file
     * @return the length of the file.
     * @see PartSource#getLength()
     */
    CARAPI GetLength(
        /* [out] */ Int64* length);

    /**
     * Return the current filename
     * @return the filename.
     * @see PartSource#getFileName()
     */
    CARAPI GetFileName(
        /* [out] */ String* fileName);

    /**
     * Return a new {@link FileInputStream} for the current filename.
     * @return the new input stream.
     * @throws IOException If an IO problem occurs.
     * @see PartSource#createInputStream()
     */
    CARAPI CreateInputStream(
        /* [out] */ Elastos::IO::IInputStream** stream);

private:
    /** File part file. */
    AutoPtr<IFile> mFile;

    /** File part file name. */
    String mFileName;
};

}
}
}
}
}
}

#endif // __ELASTOS_DROID_NET_INTERNAL_HTTP_MULTIPART_CFILEPARTSOURCE_H__
