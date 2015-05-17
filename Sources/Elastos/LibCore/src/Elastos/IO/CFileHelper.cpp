
#include "cmdef.h"
#include "CFileHelper.h"
#include "CFile.h"

namespace Elastos {
namespace IO {

ECode CFileHelper::CreateTempFile(
    /* [in] */ const String& prefix,
    /* [in] */ const String& suffix,
    /* [out] */ IFile** file)
{
    VALIDATE_NOT_NULL(file);

    return CFile::CreateTempFile(prefix, suffix, file);
}

ECode CFileHelper::CreateTempFileEx(
    /* [in] */ const String& prefix,
    /* [in] */ const String& suffix,
    /* [in] */ IFile* directory,
    /* [out] */ IFile** file)
{
    VALIDATE_NOT_NULL(file);

    return CFile::CreateTempFile(prefix, suffix, directory, file);
}

ECode CFileHelper::GetSeparatorChar(
    /* [out] */ Char32* separatorChar)
{
    VALIDATE_NOT_NULL(separatorChar);

    *separatorChar = CFile::sSeparatorChar;
    return NOERROR;
}

ECode CFileHelper::GetSeparator(
    /* [out] */ String* separator)
{
    VALIDATE_NOT_NULL(separator);

    *separator = CFile::sSeparator;
    return NOERROR;
}

ECode CFileHelper::GetPathSeparatorChar(
    /* [out] */ Char32* pathSeparatorChar)
{
    VALIDATE_NOT_NULL(pathSeparatorChar);

    *pathSeparatorChar = CFile::sPathSeparatorChar;
    return NOERROR;
}

ECode CFileHelper::GetPathSeparator(
    /* [out] */ String* pathSeparator)
{
    VALIDATE_NOT_NULL(pathSeparator);

    *pathSeparator = CFile::sPathSeparator;
    return NOERROR;
}

ECode CFileHelper::ListRoots(
    /* [out, callee] */ ArrayOf<IFile*>** files)
{
    VALIDATE_NOT_NULL(files);

    return CFile::ListRoots(files);
}

} // namespace IO
} // namespace Elastos
