
#ifndef __CTEXT_H__
#define __CTEXT_H__

#include "_Org_W3c_Dom_CText.h"
#include "Elastos.CoreLibrary.h"
#include <elastos/core/Object.h>
#include <elastos/coredef.h>

using Elastos::Core::Object;

namespace Org {
namespace W3c {
namespace Dom {

CarClass(CText)
    , public Object
    , public IText
{
public:
    CAR_INTERFACE_DECL();

    CARAPI GetNodeName(
        /* [out] */ String * pStr);

    CARAPI GetNodeValue(
        /* [out] */ String * pStr);

    CARAPI SetNodeValue(
        /* [in] */ const String& nodeValue);

    CARAPI GetNodeType(
        /* [out] */ Int16 * pValue);

    CARAPI GetParentNode(
        /* [out] */ Org::W3c::Dom::INode ** ppNode);

    CARAPI GetChildNodes(
        /* [out] */ Org::W3c::Dom::INodeList ** ppNodelist);

    CARAPI GetFirstChild(
        /* [out] */ Org::W3c::Dom::INode ** ppNode);

    CARAPI GetLastChild(
        /* [out] */ Org::W3c::Dom::INode ** ppNode);

    CARAPI GetPreviousSibling(
        /* [out] */ Org::W3c::Dom::INode ** ppNode);

    CARAPI GetNextSibling(
        /* [out] */ Org::W3c::Dom::INode ** ppNode);

    CARAPI GetAttributes(
        /* [out] */ Org::W3c::Dom::INamedNodeMap ** ppNodemap);

    CARAPI GetOwnerDocument(
        /* [out] */ Org::W3c::Dom::IDocument ** ppDoc);

    CARAPI InsertBefore(
        /* [in] */ Org::W3c::Dom::INode * pNewChild,
        /* [in] */ Org::W3c::Dom::INode * pRefChild,
        /* [out] */ Org::W3c::Dom::INode ** ppNode);

    CARAPI ReplaceChild(
        /* [in] */ Org::W3c::Dom::INode * pNewChild,
        /* [in] */ Org::W3c::Dom::INode * pOldChild,
        /* [out] */ Org::W3c::Dom::INode ** ppNode);

    CARAPI RemoveChild(
        /* [in] */ Org::W3c::Dom::INode * pOldChild,
        /* [out] */ Org::W3c::Dom::INode ** ppNode);

    CARAPI AppendChild(
        /* [in] */ Org::W3c::Dom::INode * pNewChild,
        /* [out] */ Org::W3c::Dom::INode ** ppNode);

    CARAPI HasChildNodes(
        /* [out] */ Boolean * pValue);

    CARAPI CloneNode(
        /* [in] */ Boolean deep,
        /* [out] */ Org::W3c::Dom::INode ** ppNode);

    CARAPI Normalize();

    CARAPI IsSupported(
        /* [in] */ const String& feature,
        /* [in] */ const String& ver,
        /* [out] */ Boolean * pValue);

    CARAPI GetNamespaceURI(
        /* [out] */ String * pStr);

    CARAPI GetPrefix(
        /* [out] */ String * pStr);

    CARAPI SetPrefix(
        /* [in] */ const String& prefix);

    CARAPI GetLocalName(
        /* [out] */ String * pStr);

    CARAPI HasAttributes(
        /* [out] */ Boolean * pValue);

    CARAPI GetBaseURI(
        /* [out] */ String * pStr);

    CARAPI CompareDocumentPosition(
        /* [in] */ Org::W3c::Dom::INode * pOther,
        /* [out] */ Int16 * pValue);

    CARAPI GetTextContent(
        /* [out] */ String * pStr);

    CARAPI SetTextContent(
        /* [in] */ const String& textContent);

    CARAPI IsSameNode(
        /* [in] */ Org::W3c::Dom::INode * pOther,
        /* [out] */ Boolean * pValue);

    CARAPI LookupPrefix(
        /* [in] */ const String& namespaceURI,
        /* [out] */ String * pStr);

    CARAPI IsDefaultNamespace(
        /* [in] */ const String& namespaceURI,
        /* [out] */ Boolean * pValue);

    CARAPI LookupNamespaceURI(
        /* [in] */ const String& prefix,
        /* [out] */ String * pStr);

    CARAPI IsEqualNode(
        /* [in] */ Org::W3c::Dom::INode * pArg,
        /* [out] */ Boolean * pValue);

    CARAPI GetFeature(
        /* [in] */ const String& feature,
        /* [in] */ const String& ver,
        /* [out] */ IObject ** ppObj);

    CARAPI SetUserData(
        /* [in] */ const String& key,
        /* [in] */ IObject * pData,
        /* [in] */ Org::W3c::Dom::IUserDataHandler * pHandler,
        /* [out] */ IObject ** ppObj);

    CARAPI GetUserData(
        /* [in] */ const String& key,
        /* [out] */ IObject ** ppObj);

    CARAPI GetData(
        /* [out] */ String * pStr);

    CARAPI SetData(
        /* [in] */ const String& data);

    CARAPI GetLength(
        /* [out] */ Int32 * pValue);

    CARAPI SubstringData(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [out] */ String * pStr);

    CARAPI AppendData(
        /* [in] */ const String& arg);

    CARAPI InsertData(
        /* [in] */ Int32 offset,
        /* [in] */ const String& arg);

    CARAPI DeleteData(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count);

    CARAPI ReplaceData(
        /* [in] */ Int32 offset,
        /* [in] */ Int32 count,
        /* [in] */ const String& arg);

    CARAPI SplitText(
        /* [in] */ Int32 offset,
        /* [out] */ Org::W3c::Dom::IText ** ppText);

    CARAPI IsElementContentWhitespace(
        /* [out] */ Boolean * pValue);

    CARAPI GetWholeText(
        /* [out] */ String * pStr);

    CARAPI ReplaceWholeText(
        /* [in] */ const String& content,
        /* [out] */ Org::W3c::Dom::IText ** ppText);

private:
    // TODO: Add your private member variables here.
};

}
}
}

#endif // __CTEXT_H__
