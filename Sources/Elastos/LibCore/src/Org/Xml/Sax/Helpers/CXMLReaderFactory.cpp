
#include "cmdef.h"
#include "CXMLReaderFactory.h"
#include "NewInstanceHelper.h"
#include "CParserFactory.h"
#include "CParserAdapter.h"

using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::IO::IInputStream;
using Elastos::IO::IBufferedReader;
using Elastos::IO::CBufferedReader;
using Elastos::IO::IInputStreamReader;
using Elastos::IO::CInputStreamReader;
using Elastos::IO::Charset::ICharset;
using Elastos::IO::Charset::ICharsets;
using Elastos::IO::Charset::CCharsets;

namespace Org {
namespace Xml {
namespace Sax {
namespace Helpers {

const String XMLReaderFactory::property("org.xml.sax.driver");

ECode CXMLReaderFactory::CreateXMLReader(
    /* [out] */ IXMLReader** reader)
{
    return XMLReaderFactory::CreateXMLReader(reader);
}

ECode CXMLReaderFactory::CreateXMLReaderEx(
    /* [in] */ const String& className,
    /* [out] */ IXMLReader** reader)
{
    return XMLReaderFactory::CreateXMLReader(className,reader);
}

ECode XMLReaderFactory::CreateXMLReader(
    /* [out] */ IXMLReader** xmlreader)
{
    VALIDATE_NOT_NULL(xmlreader)

    AutoPtr<IClassLoader> loader = NewInstanceHelper::GetClassLoader();

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    // 1. try the JVM-instance-wide system property
    // try {
    String className;
    system->GetProperty(property, &className);
    // }
    // catch (RuntimeException e) { /* normally fails for applets */ }

    // 2. if that fails, try META-INF/services/
    if (className.IsNull()) {
        // try {
        String service = String("META-INF/services/") + property;
        AutoPtr<IInputStream> in;
        AutoPtr<IBufferedReader> reader;

        if (loader == NULL)
            assert(0 && "TODO");
            // in = ClassLoader.getSystemResourceAsStream (service);
        else
            assert(0 && "TODO");
            // in = loader.getResourceAsStream (service);

        if (in != NULL) {
            AutoPtr<ICharsets> csets;
            CCharsets::AcquireSingleton((ICharsets**)&csets);
            AutoPtr<ICharset> charset;
            csets->GetUTF_8((ICharset**)&charset);
            String utf8str;
            charset->GetName(&utf8str);
            AutoPtr<IInputStreamReader> isr;
            FAIL_RETURN(CInputStreamReader::New(in, utf8str, (IInputStreamReader**)&isr));
            FAIL_RETURN(CBufferedReader::New(isr, (IBufferedReader**)&reader));
            reader->ReadLine(&className);
            in->Close ();
        }
        // } catch (Exception e) {
        // }
    }

    // 3. Distro-specific fallback
    if (className.IsNull()) {
    // BEGIN DISTRIBUTION-SPECIFIC

            // EXAMPLE:
            // className = "com.example.sax.XmlReader";
            // or a $JAVA_HOME/jre/lib/*properties setting...

    // END DISTRIBUTION-SPECIFIC
    }

    // do we know the XMLReader implementation class yet?
    if (!className.IsNull())
        return LoadClass(loader, className, xmlreader);

    // 4. panic -- adapt any SAX1 parser
    // try {
    AutoPtr<IXMLReader> outreader;
    AutoPtr<IParser> outparser;
    ParserFactory::MakeParser((IParser**)&outparser);

    CParserAdapter::New(outparser, (IXMLReader**)&outreader);
    *xmlreader = outreader;
    INTERFACE_ADDREF(*xmlreader)
    // } catch (Exception e) {
    //     throw new SAXException ("Can't create default XMLReader; "
    //         + "is system property org.xml.sax.driver set?");
    // }
    return NOERROR;
}

ECode XMLReaderFactory::CreateXMLReader(
    /* [in] */ const String& className,
    /* [out] */ IXMLReader** reader)
{
    AutoPtr<IClassLoader> loader = NewInstanceHelper::GetClassLoader();
    return LoadClass(loader, className, reader);
}

ECode XMLReaderFactory::LoadClass(
    /* [in] */ IClassLoader* loader,
    /* [in] */ const String& className,
    /* [out] */ IXMLReader** xmlReader)
{
    VALIDATE_NOT_NULL(xmlReader)
    // try {
    AutoPtr<IXMLReader> outreader = (IXMLReader*) NewInstanceHelper::NewInstance(loader, className)->Probe(EIID_IXMLReader);
    if (outreader == NULL)
    {
        return E_XML_SAX_EXCEPTION;
    }
    // } catch (ClassNotFoundException e1) {
    //     throw new SAXException("SAX2 driver class " + className +
    //                " not found", e1);
    // } catch (IllegalAccessException e2) {
    //     throw new SAXException("SAX2 driver class " + className +
    //                " found but cannot be loaded", e2);
    // } catch (InstantiationException e3) {
    //     throw new SAXException("SAX2 driver class " + className +
    //    " loaded but cannot be instantiated (no empty public constructor?)",
    //                e3);
    // } catch (ClassCastException e4) {
    //     throw new SAXException("SAX2 driver class " + className +
    //                " does not implement XMLReader", e4);
    // }
    *xmlReader = outreader;
    INTERFACE_ADDREF(*xmlReader)
    return NOERROR;
}

} // namespace Helpers
} // namespace Sax
} // namespace Xml
} // namespace Org
