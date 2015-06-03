
#include "URLConnection.h"
#include "DefaultFileNameMap.h"
#include "CDateHelper.h"
#include "CSystem.h"
#include <elastos/core/Character.h>
#include <elastos/core/StringBuffer.h>
#include <elastos/core/StringUtils.h>

namespace Elastos {
namespace Net {

using Elastos::Core::StringBuffer;
using Elastos::Core::StringUtils;
using Elastos::Core::Character;
using Elastos::Core::ISystem;
using Elastos::Utility::IDateHelper;
using Elastos::Utility::CDateHelper;

PInterface URLConnection::DefaultContentHandler::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IContentHandler) {
        return (IContentHandler*)this;
    }

    return NULL;
}

UInt32 URLConnection::DefaultContentHandler::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 URLConnection::DefaultContentHandler::Release()
{
    return ElRefBase::Release();
}

ECode URLConnection::DefaultContentHandler::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode URLConnection::DefaultContentHandler::GetContent(
    /* [in] */ IURLConnection* uConn,
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj);
    return uConn->GetInputStream((IInputStream**)obj);
}

ECode URLConnection::DefaultContentHandler::GetContent(
    /* [in] */ IURLConnection* uConn,
    /* [in] */ const ArrayOf<InterfaceID>& types,
    /* [out] */ IInterface** obj)
{
    GetContent(uConn, obj);
    for(Int32 i =0; i < types.GetLength(); ++i)
    {
        if(((IInterface*)*obj)->Probe(types[i]) != NULL)
        {
            return NOERROR;
        }
    }
    *obj = NULL;
    return NOERROR;
}

// extern "C" const InterfaceID EIID_URLConnection =
//     { 0x6ef700a4, 0x2101, 0x4d37, { 0xbe, 0x26, 0x1f, 0xf9, 0xf6, 0x46, 0x35, 0x4f } };

Boolean URLConnection::sDefaultAllowUserInteraction = FALSE;
Boolean URLConnection::sDefaultUseCaches = TRUE;
HashMap<String, AutoPtr<IContentHandler> > URLConnection::sContentHandlers;
AutoPtr<IContentHandlerFactory> URLConnection::sContentHandlerFactory;
AutoPtr<IFileNameMap> URLConnection::sFileNameMap;
Mutex URLConnection::sLock;

URLConnection::URLConnection()
    : mIfModifiedSince(0)
    , mUseCaches(sDefaultUseCaches)
    , mConnected(FALSE)
    , mDoOutput(FALSE)
    , mDoInput(TRUE)
    , mAllowUserInteraction(sDefaultAllowUserInteraction)
    , mLastModified(-1)
    , mReadTimeout(0)
    , mConnectTimeout(0)
{
    mDefaultHandler = new DefaultContentHandler();
}

URLConnection::~URLConnection()
{}

ECode URLConnection::Init(
    /* [in] */ IURL* url)
{
    mUrl = url;
    return NOERROR;
}

ECode URLConnection::GetAllowUserInteraction(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mAllowUserInteraction;
    return NOERROR;
}

ECode URLConnection::GetContent(
    /* [out] */ IInterface** content)
{
    VALIDATE_NOT_NULL(content)

    if (!mConnected) {
        Connect();
    }

    GetContentType(&mContentType);
    if (mContentType.IsNull()) {
        String fileName;
        mUrl->GetFile(&fileName);
        mContentType = GuessContentTypeFromName(fileName);
        if (mContentType.IsNull()) {
            AutoPtr<IInputStream> inputStream;
            GetInputStream((IInputStream**)&inputStream);
            mContentType = GuessContentTypeFromStream(inputStream);
        }
    }
    if (!mContentType.IsNull()) {
        AutoPtr<IContentHandler> handler;
        GetContentHandler(mContentType, (IContentHandler**)&handler);
        return handler->GetContent((IURLConnection*)this->Probe(EIID_IURLConnection), content);
    }

    *content = NULL;
    return NOERROR;
}

//        public Object getContent(Class[] types);

ECode URLConnection::GetContentEncoding(
    /* [out] */ String* encoding)
{
    VALIDATE_NOT_NULL(encoding)

    return GetHeaderFieldByKey(String("Content-Encoding"), encoding);
}

ECode URLConnection::GetContentHandler(
    /* [in] */ const String& type,
    /* [out] */ IContentHandler** contentHandler)
{
    VALIDATE_NOT_NULL(contentHandler)

    // Replace all non-alphanumeric character by '_'
    String typeString = ParseTypeString(type.Replace('/', '.'));

    // if there's a cached content handler, use it
    HashMap<String, AutoPtr<IContentHandler> >::Iterator it = sContentHandlers.Find(type);
    if (it != sContentHandlers.End()) {
        *contentHandler = it->mSecond;
        REFCOUNT_ADD(*contentHandler);
        return NOERROR;
    }

    AutoPtr<IContentHandler> cHandler;
    if (sContentHandlerFactory != NULL) {
        sContentHandlerFactory->CreateContentHandler(type, (IContentHandler**)&cHandler);
        sContentHandlers[type] = cHandler;
        *contentHandler = cHandler;
        REFCOUNT_ADD(*contentHandler);
        return NOERROR;
    }

    // search through the package list for the right class for the Content
    // Type
//  zhangjingcheng not implement
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    String packageList;
    system->GetEnv(String("java.content.handler.pkgs"), &packageList);
    if (packageList.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

//    String packageList = AccessController
//            .doPrivileged(new PriviAction<String>(
//                    "java.content.handler.pkgs"));
//    if (!packageList.IsNull()) {
        // AutoPtr<ArrayOf<String> > values;
        // StringUtils::Split(packageList, String("|"), (ArrayOf<String>**)&values);
        // Int32 count = (values ? values->GetLength() : 0);
        // for (Int32 i = 0; i < count; ++i) {
//            try {
//                Class<?> cl = Class.forName((*values)[i] + "."
//                        + typeString, true, ClassLoader
//                        .getSystemClassLoader());
//                cHandler = cl.newInstance();
//            } catch (ClassNotFoundException e) {
//            } catch (IllegalAccessException e) {
//            } catch (InstantiationException e) {
//            }
//        }
//    }

    if (cHandler == NULL) {
//        cHandler = AccessController
//                .doPrivileged(new PrivilegedAction<Object>() {
//                    public Object run() {
//                        try {
//                            // Try looking up AWT image content handlers
//                            String className = "org.apache.harmony.awt.www.content."
//                                    + typeString;
//                            return Class.forName(className).newInstance();
//                        } catch (ClassNotFoundException e) {
//                        } catch (IllegalAccessException e) {
//                        } catch (InstantiationException e) {
//                        }
//                        return null;
//                   }
//                });
    }
    if (cHandler != NULL) {
//        if (!(cHandler instanceof ContentHandler)) {
//            throw new UnknownServiceException();
//        }
        sContentHandlers[type] = cHandler; // if we got the handler,
        // cache it for next time
        *contentHandler = cHandler;
        REFCOUNT_ADD(*contentHandler);
        return NOERROR;
    }

    *contentHandler = mDefaultHandler;
    REFCOUNT_ADD(*contentHandler);
    return NOERROR;
}

ECode URLConnection::GetContentLength(
    /* [out] */ Int32* length)
{
    return GetHeaderFieldInt32(String("Content-Length"), -1, length);
}

ECode URLConnection::GetContentType(
    /* [out] */ String* type)
{
    return GetHeaderFieldByKey(String("Content-Type"), type);
}

ECode URLConnection::GetDate(
    /* [out] */ Int64* date)
{
    return GetHeaderFieldDate(String("Date"), 0, date);
}

Boolean URLConnection::GetDefaultAllowUserInteraction()
{
    return sDefaultAllowUserInteraction;
}

String URLConnection::GetDefaultRequestProperty(
    /* [in] */ const String& field)
{
    return String(NULL);
}

ECode URLConnection::GetDefaultUseCaches(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    *value = sDefaultUseCaches;
    return NOERROR;
}

ECode URLConnection::GetDoInput(
    /* [out] */ Boolean* allowed)
{
    VALIDATE_NOT_NULL(allowed);
    *allowed = mDoInput;
    return NOERROR;
}

ECode URLConnection::GetDoOutput(
    /* [out] */ Boolean* allowed)
{
    VALIDATE_NOT_NULL(allowed);
    *allowed = mDoOutput;
    return NOERROR;
}

ECode URLConnection::GetExpiration(
    /* [out] */ Int64* expiration)
{
    return GetHeaderFieldDate(String("Expires"), 0, expiration);
}

AutoPtr<IFileNameMap> URLConnection::GetFileNameMap()
{
    Mutex::Autolock lock(&sLock);

    if (sFileNameMap == NULL) {
        sFileNameMap = new DefaultFileNameMap();
    }
    return sFileNameMap;
}

ECode URLConnection::GetHeaderFieldByPosition(
    /* [in] */ Int32 pos,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)

    *value = NULL;
    return NOERROR;
}

//    public Map<String, List<String>> getHeaderFields()

//    public Map<String, List<String>> getRequestProperties()

ECode URLConnection::CheckNotConnected()
{
    if (mConnected) {
//        throw new IllegalStateException("Already connected");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

    return NOERROR;
}

ECode URLConnection::AddRequestProperty(
    /* [in] */ const String& field,
    /* [in] */ const String& newValue)
{
    FAIL_RETURN(CheckNotConnected());
    if (field.IsNull()) {
//        throw new NullPointerException("field == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    return NOERROR;
}

ECode URLConnection::GetHeaderFieldByKey(
    /* [in] */ const String& key,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)

    *value = NULL;
    return NOERROR;
}

ECode URLConnection::GetHeaderFieldDate(
    /* [in] */ const String& field,
    /* [in] */ Int64 defaultValue,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)

    String date;
    FAIL_RETURN(GetHeaderFieldByKey(field, &date));
    if (date.IsNull()) {
        *value = defaultValue;
        return NOERROR;
    }

    AutoPtr<IDateHelper> dateHelper;
    CDateHelper::AcquireSingleton((IDateHelper**)&dateHelper);
    ECode ec = dateHelper->Parse(date, value);
    if(FAILED(ec))
    {
        *value = defaultValue;
    }
    return NOERROR;
}

ECode URLConnection::GetHeaderFieldInt32(
    /* [in] */ const String& field,
    /* [in] */ Int32 defaultValue,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
//    try {
//    return Integer.parseInt(getHeaderField(field));
    String s;
    GetHeaderFieldByKey(field, &s);
    *value = StringUtils::ParseInt32(s, 10, defaultValue);
    return NOERROR;
//    } catch (NumberFormatException e) {
//        return defaultValue;
//    }
}

ECode URLConnection::GetHeaderFieldKey(
    /* [in] */ Int32 posn,
    /* [out] */ String* key)
{
    VALIDATE_NOT_NULL(key)

    *key = NULL;
    return NOERROR;
}

ECode URLConnection::GetIfModifiedSince(
    /* [out] */ Int64* time)
{
    VALIDATE_NOT_NULL(time)

    *time = mIfModifiedSince;
    return NOERROR;
}

ECode URLConnection::GetInputStream(
    /* [out] */ IInputStream** is)
{
//    throw new UnknownServiceException("Does not support writing to the input stream");
    return E_UNKNOWN_SERVICE_EXCEPTION;
}

ECode URLConnection::GetLastModified(
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)

    if (mLastModified != -1) {
        *value = mLastModified;
        return NOERROR;
    }
    GetHeaderFieldDate(String("Last-Modified"), 0, &mLastModified);
    *value = mLastModified;
    return NOERROR;
}

ECode URLConnection::GetOutputStream(
    /* [out] */ IOutputStream** os)
{
//    throw new UnknownServiceException("Does not support writing to the output stream");
    return E_UNKNOWN_SERVICE_EXCEPTION;
}

//        public java.security.Permission getPermission();

ECode URLConnection::GetRequestProperty(
    /* [in] */ const String& field,
    /* [out] */ String* property)
{
    VALIDATE_NOT_NULL(property)

    FAIL_RETURN(CheckNotConnected());
    *property = NULL;
    return NOERROR;
}

ECode URLConnection::GetURL(
    /* [out] */ IURL** url)
{
    VALIDATE_NOT_NULL(url)

    *url = mUrl;
    REFCOUNT_ADD(*url);
    return NOERROR;
}

ECode URLConnection::GetUseCaches(
    /* [out] */ Boolean* allowed)
{
    VALIDATE_NOT_NULL(allowed)

    *allowed = mUseCaches;
    return NOERROR;
}

String URLConnection::GuessContentTypeFromName(
    /* [in] */ const String& url)
{
    String s;
    GetFileNameMap()->GetContentTypeFor(url, &s);
    return s;
}

String URLConnection::GuessContentTypeFromStream(
    /* [in] */ IInputStream* is)
{
    Boolean supported;
    if (is->IsMarkSupported(&supported), !supported) {
        return String(NULL);
    }
    // Look ahead up to 64 bytes for the longest encoded header
    is->Mark(64);
    ArrayOf_<Byte, 64> bytes;
    Int32 length;
    is->ReadBytes(&bytes, &length);
    is->Reset();

    // If there is no data from the input stream, we can't determine content type.
    if (length == -1) {
        return String(NULL);
    }

    // Check for Unicode BOM encoding indicators
    CString encoding = "US-ASCII";
    Int32 start = 0;
    if (length > 1) {
        if ((bytes[0] == (Byte)0xFF) && (bytes[1] == (Byte)0xFE)) {
            encoding = "UTF-16LE";
            start = 2;
            length -= length & 1;
        }
        if ((bytes[0] == (Byte)0xFE) && (bytes[1] == (Byte)0xFF)) {
            encoding = "UTF-16BE";
            start = 2;
            length -= length & 1;
        }
        if (length > 2) {
            if ((bytes[0] == (Byte)0xEF) && (bytes[1] == (Byte)0xBB)
                    && (bytes[2] == (Byte)0xBF)) {
                encoding = "UTF-8";
                start = 3;
            }
            if (length > 3) {
                if ((bytes[0] == (Byte)0x00) && (bytes[1] == (Byte)0x00)
                        && (bytes[2] == (Byte)0xFE)
                        && (bytes[3] == (Byte)0xFF)) {
                    encoding = "UTF-32BE";
                    start = 4;
                    length -= length & 3;
                }
                if ((bytes[0] == (Byte)0xFF) && (bytes[1] == (Byte)0xFE)
                        && (bytes[2] == (Byte)0x00)
                        && (bytes[3] == (Byte)0x00)) {
                    encoding = "UTF-32LE";
                    start = 4;
                    length -= length & 3;
                }
            }
        }
    }

    String header((const char*)bytes.GetPayload() + start, length - start/*, encoding*/);

    // Check binary types
    if (header.StartWith("PK")) {
        return String("application/zip");
    }
    if (header.StartWith("GI")) {
        return String("image/gif");
    }

    // Check text types
    String textHeader = header.Trim().ToUpperCase();
    if (textHeader.StartWith("<!DOCTYPE HTML") ||
            textHeader.StartWith("<HTML") ||
            textHeader.StartWith("<HEAD") ||
            textHeader.StartWith("<BODY") ||
            textHeader.StartWith("<HEAD")) {
        return String("text/html");
    }

    if (textHeader.StartWith("<?XML")) {
        return String("application/xml");
    }

    // Give up
    return String(NULL);
}

String URLConnection::ParseTypeString(
    /* [in] */ const String& typeString)
{
    StringBuffer typeStringBuffer(typeString);
    for (Int32 i = 0; i < typeStringBuffer.GetByteCount(); i++) {
        // if non-alphanumeric, replace it with '_'
        Char32 c;
        typeStringBuffer.GetCharAt(i, &c);
        if (!(Character::IsLetter(c) || Character::IsDigit(c) || c == '.')) {
            typeStringBuffer.SetChar(i, '_');
        }
    }
    String result;
    typeStringBuffer.ToString(&result);
    return result;
}

ECode URLConnection::SetAllowUserInteraction(
    /* [in] */ Boolean newValue)
{
    FAIL_RETURN(CheckNotConnected());
    mAllowUserInteraction = newValue;

    return NOERROR;
}

ECode URLConnection::SetContentHandlerFactory(
    /* [in] */ IContentHandlerFactory* contentFactory)
{
    Mutex::Autolock lock(&sLock);

    if (sContentHandlerFactory != NULL) {
//        throw new Error("Factory already set");
        return NOERROR;
    }
//    SecurityManager sManager = System.getSecurityManager();
//    if (sManager != null) {
//        sManager.checkSetFactory();
//    }
    sContentHandlerFactory = contentFactory;
    return NOERROR;
}

void URLConnection::SetDefaultAllowUserInteraction(
    /* [in] */ Boolean allows)
{
    sDefaultAllowUserInteraction = allows;
}

void URLConnection::SetDefaultRequestProperty(
    /* [in] */ const String& field,
    /* [in] */ const String& value)
{
}

ECode URLConnection::SetDefaultUseCaches(
    /* [in] */ Boolean newValue)
{
    sDefaultUseCaches = newValue;
    return NOERROR;
}

ECode URLConnection::SetDoInput(
    /* [in] */ Boolean newValue)
{
    FAIL_RETURN(CheckNotConnected());
    mDoInput = newValue;

    return NOERROR;
}

ECode URLConnection::SetDoOutput(
    /* [in] */ Boolean newValue)
{
    FAIL_RETURN(CheckNotConnected());
    mDoOutput = newValue;

    return NOERROR;
}

void URLConnection::SetFileNameMap(
    /* [in] */ IFileNameMap* map)
{
//    SecurityManager manager = System.getSecurityManager();
//    if (manager != null) {
//        manager.checkSetFactory();
//    }
    Mutex::Autolock lock(&sLock);

    sFileNameMap = map;
}

ECode URLConnection::SetIfModifiedSince(
    /* [in] */ Int64 newValue)
{
    FAIL_RETURN(CheckNotConnected());
    mIfModifiedSince = newValue;

    return NOERROR;
}

ECode URLConnection::SetRequestProperty(
    /* [in] */ const String& field,
    /* [in] */ const String& newValue)
{
    FAIL_RETURN(CheckNotConnected());
    if (field.IsNull()) {
//        throw new NullPointerException("field == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    return NOERROR;
}

ECode URLConnection::SetUseCaches(
    /* [in] */ Boolean newValue)
{
    FAIL_RETURN(CheckNotConnected());
    mUseCaches = newValue;

    return NOERROR;
}

ECode URLConnection::SetConnectTimeout(
    /* [in] */ Int32 timeout)
{
    if (timeout < 0) {
//        throw new IllegalArgumentException("timeout < 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mConnectTimeout = timeout;
    return NOERROR;
}

ECode URLConnection::GetConnectTimeout(
    /* [out] */ Int32* timeout)
{
    VALIDATE_NOT_NULL(timeout)

    *timeout = mConnectTimeout;
    return NOERROR;
}

ECode URLConnection::SetReadTimeout(
    /* [in] */ Int32 timeout)
{
    if (timeout < 0) {
//        throw new IllegalArgumentException("timeout < 0")
        return E_ILLEGAL_ARGUMENT_EXCEPTION;;
    }
    mReadTimeout = timeout;
    return NOERROR;
}

ECode URLConnection::GetReadTimeout(
    /* [out] */ Int32* timeout)
{
    VALIDATE_NOT_NULL(timeout)

    *timeout = mReadTimeout;
    return NOERROR;
}

} // namespace Net
} // namespace Elastos
