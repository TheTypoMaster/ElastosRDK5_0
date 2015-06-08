
#include "DefaultFileNameMap.h"
#include "CMimeUtils.h"

namespace Elastos {
namespace Net {

CAR_INTERFACE_IMPL(DefaultFileNameMap, Object, IFileNameMap)

PInterface DefaultFileNameMap::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IFileNameMap) {
        return (IFileNameMap*)this;
    }

    return NULL;
}

UInt32 DefaultFileNameMap::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 DefaultFileNameMap::Release()
{
    return ElRefBase::Release();
}

ECode DefaultFileNameMap::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode DefaultFileNameMap::GetContentTypeFor(
    /* [in] */ const String& filename,
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type);
    AutoPtr<IMimeUtils> utils;
    FAIL_RETURN(CMimeUtils::AcquireSingleton((IMimeUtils**)&utils));
    if (filename.EndWith("/")) {
         //a directory, return html
        return utils->GuessMimeTypeFromExtension(String("html"), type);
    }
    Int32 lastCharInExtension = filename.LastIndexOf('#');
    if (lastCharInExtension < 0) {
        lastCharInExtension = filename.GetLength();
    }
    Int32 firstCharInExtension = filename.LastIndexOf('.') + 1;
    String ext("");
    if (firstCharInExtension > filename.LastIndexOf('/')) {
        ext = filename.Substring(firstCharInExtension, lastCharInExtension).ToLowerCase();
    }
    return utils->GuessMimeTypeFromExtension(ext, type);
}

} // namespace Net
} // namespace Elastos
