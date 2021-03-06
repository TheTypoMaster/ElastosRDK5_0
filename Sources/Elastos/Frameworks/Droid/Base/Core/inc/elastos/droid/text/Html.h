#ifndef __ELASTOS_DROID_TEXT_Html_H__
#define __ELASTOS_DROID_TEXT_Html_H__

#include "ext/frameworkext.h"
#include <elastos/utility/etl/HashMap.h>
#include <elastos/core/StringBuilder.h>

//using Org::Ccil::Cowan::TagSoup::IParser;
using Org::Xml::Sax::IContentHandler;
using Org::Xml::Sax::IXMLReader;
using Org::Xml::Sax::IAttributes;
using Org::Xml::Sax::ILocator;
using Elastos::Utility::Etl::HashMap;
using Elastos::Core::StringBuilder;
using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Text {

/**
 * This class processes HTML strings into displayable styled text.
 * Not all HTML tags are supported.
 */
class Html
{
private:
    /**
     * Lazy initialization holder for HTML parser. This class will
     * a) be preloaded by the zygote, or b) not loaded until absolutely
     * necessary.
     */
    //static
    class HtmlParser
    {
    private:
//        static AutoPtr<IHTMLSchema> schema;// = new HTMLSchema();
    };

public:
    /**
     * Returns displayable styled text from the provided HTML string.
     * Any &lt;img&gt; tags in the HTML will display as a generic
     * replacement image which your program can then go through and
     * replace with real images.
     *
     * <p>This uses TagSoup to handle real HTML, including all of the brokenness found in the wild.
     */
    static CARAPI_(AutoPtr<ISpanned>) FromHtml(
        /* [in] */ const String& source);

    /**
     * Returns displayable styled text from the provided HTML string.
     * Any &lt;img&gt; tags in the HTML will use the specified ImageGetter
     * to request a representation of the image (use null if you don't
     * want this) and the specified TagHandler to handle unknown tags
     * (specify null if you don't want this).
     *
     * <p>This uses TagSoup to handle real HTML, including all of the brokenness found in the wild.
     */
    static CARAPI_(AutoPtr<ISpanned>) FromHtml(
        /* [in] */ const String& source,
        /* [in] */ IHtmlImageGetter* imageGetter,
        /* [in] */ IHtmlTagHandler* tagHandler);

    /**
     * Returns an HTML representation of the provided Spanned text.
     */
    static CARAPI_(String) ToHtml(
        /* [in] */ ISpanned* text);

    /**
     * Returns an HTML escaped representation of the given plain text.
     */
    static CARAPI_(String) EscapeHtml(
        /* [in] */ ICharSequence* text);

private:
    Html();

    static CARAPI_(void) WithinHtml(
        /* [in, out] */ StringBuilder* out,
        /* [in] */ ISpanned* text);

    static CARAPI_(void) WithinDiv(
        /* [in, out] */ StringBuilder* out,
        /* [in] */ ISpanned* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI_(String) GetOpenParaTagWithDirection(
        /* [in] */ ISpanned* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI_(void) WithinBlockquote(
        /* [in] */ StringBuilder* out,
        /* [in] */ ISpanned* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    static CARAPI_(void) WithinParagraph(
        /* [in] */ StringBuilder* out,
        /* [in] */ ISpanned* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ Int32 nl,
        /* [in] */ Boolean last);

    static CARAPI_(void) WithinStyle(
        /* [in] */ StringBuilder* out,
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);
};

class HtmlToSpannedConverter
    : public ElRefBase
    , public IContentHandler
{
private:
    class Bold : public ElRefBase
    { };
    class Italic : public ElRefBase
    { };
    class Underline : public ElRefBase
    { };
    class Big : public ElRefBase
    { };
    class Small : public ElRefBase
    { };
    class Monospace : public ElRefBase
    { };
    class Blockquote : public ElRefBase
    { };
    class Super : public ElRefBase
    { };
    class Sub : public ElRefBase
    { };

    class Font : public ElRefBase
    {
    public:
        Font(
            /* [in] */ const String& color,
            /* [in] */ const String& face);

    public:
        String mColor;
        String mFace;
    };

    class Href : public ElRefBase
    {
    public:
        Href(
            /* [in] */ const String& href);

    public:
        String mHref;
    };

    class Header : public ElRefBase
    {
    public:
        Header(
            /* [in] */ Int32 level);

    public://private:
        Int32 mLevel;
    };

public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface* Object,
        /* [out] */ InterfaceID* iID);

    HtmlToSpannedConverter(
        /* [in] */ const String& source,
        /* [in] */ IHtmlImageGetter* imageGetter,
        /* [in] */ IHtmlTagHandler* tagHandler,
        /* [in] */ /*IParser*/IInterface* parser);

    CARAPI_(AutoPtr<ISpanned>) Convert();

    CARAPI SetDocumentLocator(
        /* [in] */ ILocator* locator);

    CARAPI StartDocument(); // throws SAXException

    CARAPI EndDocument(); // throws SAXException

    CARAPI StartPrefixMapping(
        /* [in] */ const String& prefix,
        /* [in] */ const String& uri); // throws SAXException

    CARAPI EndPrefixMapping(
        /* [in] */ const String& prefix); // throws SAXException

    CARAPI StartElement(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [in] */ const String& qName,
        /* [in] */ IAttributes* attributes); // throws SAXException

    CARAPI EndElement(
        /* [in] */ const String& uri,
        /* [in] */ const String& localName,
        /* [in] */ const String& qName); // throws SAXException

    CARAPI Characters(
        /* [in] */ ArrayOf<Char32>* ch,
        /* [in] */ Int32 start,
        /* [in] */ Int32 length); // throws SAXException

    CARAPI IgnorableWhitespace(
        /* [in] */ ArrayOf<Char32>* ch,
        /* [in] */ Int32 start,
        /* [in] */ Int32 length); // throws SAXException

    CARAPI ProcessingInstruction(
        /* [in] */ const String& target,
        /* [in] */ const String& data); // throws SAXException

    CARAPI SkippedEntity(
        /* [in] */ const String& name); // throws SAXException

private:
    CARAPI_(void) HandleStartTag(
        /* [in] */ const String& tag,
        /* [in] */ IAttributes* attributes);

    CARAPI_(void) HandleEndTag(
        /* [in] */ const String& tag);

    static CARAPI_(void) HandleP(
        /* [in] */ ISpannableStringBuilder* text);

    static CARAPI_(void) HandleBr(
        /* [in] */ ISpannableStringBuilder* text);

    static CARAPI_(AutoPtr<IInterface>) GetLast(
        /* [in] */ ISpanned* text,
        /* [in] */ /*Class*/InterfaceID kind);

    static CARAPI_(void) Start(
        /* [in] */ ISpannableStringBuilder* text,
        /* [in] */ IInterface* mark);

    static CARAPI_(void) End(
        /* [in] */ ISpannableStringBuilder* text,
        /* [in] */ /*Class*/InterfaceID kind,
        /* [in] */ IInterface* repl);

    static CARAPI_(void) StartImg(
        /* [in] */ ISpannableStringBuilder* text,
        /* [in] */ IAttributes* attributes,
        /* [in] */ IHtmlImageGetter* img);

    static CARAPI_(void) StartFont(
        /* [in] */ ISpannableStringBuilder* text,
        /* [in] */ IAttributes* attributes);

    static CARAPI_(void) EndFont(
        /* [in] */ ISpannableStringBuilder* text);

    static CARAPI_(void) StartA(
        /* [in] */ ISpannableStringBuilder* text,
        /* [in] */ IAttributes* attributes);

    static CARAPI_(void) EndA(
        /* [in] */ ISpannableStringBuilder* text);

    static CARAPI_(void) EndHeader(
        /* [in] */ ISpannableStringBuilder* text);

    static AutoPtr< HashMap<String, Int32> > BuildColorMap();

    static CARAPI_(AutoPtr< ArrayOf<Float> >) InitStaticHeaderSizes();

    /**
     * Converts an HTML color (named or numeric) to an integer RGB value.
     *
     * @param color Non-null color string.
     * @return A color value, or {@code -1} if the color string could not be interpreted.
     */
    static CARAPI_(Int32) GetHtmlColor(
        /* [in] */ const String& color);

private:
    static AutoPtr< ArrayOf<Float> > HEADER_SIZES; // = { 1.5f, 1.4f, 1.3f, 1.2f, 1.1f, 1f, };

    String mSource;
    AutoPtr<IXMLReader> mReader;
    AutoPtr<ISpannableStringBuilder> mSpannableStringBuilder;
    AutoPtr<IHtmlImageGetter> mImageGetter;
    AutoPtr<IHtmlTagHandler> mTagHandler;

    static AutoPtr< HashMap<String, Int32> > COLORS;// = BuildColorMap();
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_TEXT_Html_H__
