
#include "accounts/CAccount.h"
#include "app/ActivityManagerNative.h"
#include "app/AppGlobals.h"
#include "content/ContentResolver.h"
#include "content/CContentResolverOpenResourceIdResult.h"
#include "content/CContentProviderClient.h"
#include "content/res/CAssetFileDescriptor.h"
#include "os/UserHandle.h"
#include "os/ServiceManager.h"
#include "os/SystemClock.h"
#include "privacy/surrogate/PrivacyContentResolver.h"
#include "text/TextUtils.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/StringUtils.h>

// BEGIN privacy-added
#include "privacy/surrogate/PrivacyContentResolver.h"
// END privacy-added

using Elastos::Core::StringUtils;
using Elastos::Core::IInteger32;
using Elastos::Core::IInteger64;
using Elastos::Core::IBoolean;
using Elastos::Core::IFloat;
using Elastos::Core::IDouble;
using Elastos::Core::StringBuilder;
using Elastos::Core::CString;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::Core::CRandom;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Accounts::CAccount;
using Elastos::Droid::App::ActivityManagerNative;
using Elastos::Droid::App::AppGlobals;
using Elastos::Droid::App::IIActivityManager;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::CAssetFileDescriptor;
using Elastos::Droid::Database::IIContentObserver;
using Elastos::Droid::Database::EIID_ICrossProcessCursorWrapper;
using Elastos::Droid::Database::EIID_ICursorWrapper;
using Elastos::Droid::Os::IICancellationSignal;
using Elastos::Droid::Os::EIID_IParcelFileDescriptor;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::ServiceManager;
using Elastos::Droid::Os::IBinderHelper;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Privacy::Surrogate::PrivacyContentResolver;
using Elastos::Droid::Text::TextUtils;

namespace Elastos {
namespace Droid {
namespace Content {

const String ContentResolver::TAG("ContentResolver");
const Int32 ContentResolver::SLOW_THRESHOLD_MILLIS = 500;
const String ContentResolver::CursorWrapperInner::TAG("CursorWrapperInner");
AutoPtr<IContentService> ContentResolver::sContentService;

ContentResolver::CursorWrapperInner::CursorWrapperInner(
    /* [in] */ ICursor* cursor,
    /* [in] */ IIContentProvider* icp,
    /* [in] */ ContentResolver* contentResolver)
    : mContentProvider(icp)
    , mProviderReleased(FALSE)
    , mContentResolver(contentResolver)
{
    Init(cursor);
//    mCloseGuard.open("close");
}

ContentResolver::CursorWrapperInner::~CursorWrapperInner()
{
// try {
//        if (mCloseGuard != null) {
//            mCloseGuard.warnIfOpen();
//        }

    if (!mProviderReleased && NULL != mContentProvider) {
        // Even though we are using CloseGuard, log this anyway so that
        // application developers always see the message in the log.
        Logger::W(TAG, "Cursor finalized without prior close()");
        Boolean result = FALSE;
        mContentResolver->ReleaseProvider(mContentProvider, &result);
    }
    // } finally {
//        super.finalize();
    // }
}

CAR_INTERFACE_IMPL_2(ContentResolver::CursorWrapperInner, ICrossProcessCursorWrapper, ICursorWrapper)

ECode ContentResolver::CursorWrapperInner::Close()
{
    CrossProcessCursorWrapper::Close();
    Boolean result = FALSE;
    mContentResolver->ReleaseProvider(mContentProvider, &result);
    mProviderReleased = TRUE;

//    if (mCloseGuard != null) {
//        mCloseGuard.close();
//    }
    return NOERROR;
}

ECode ContentResolver::CursorWrapperInner::GetCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return CrossProcessCursorWrapper::GetCount(count);
}

ECode ContentResolver::CursorWrapperInner::GetPosition(
    /*[out]*/ Int32* position)
{
    VALIDATE_NOT_NULL(position);
    return CrossProcessCursorWrapper::GetPosition(position);
}

ECode ContentResolver::CursorWrapperInner::Move(
    /* [in] */ Int32 offset,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    return CrossProcessCursorWrapper::Move(offset, succeeded);
}

ECode ContentResolver::CursorWrapperInner::MoveToPosition(
    /* [in] */ Int32 position,
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    return CrossProcessCursorWrapper::MoveToPosition(position, succeeded);
}

ECode ContentResolver::CursorWrapperInner::MoveToFirst(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    return CrossProcessCursorWrapper::MoveToFirst(succeeded);
}

ECode ContentResolver::CursorWrapperInner::MoveToLast(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    return CrossProcessCursorWrapper::MoveToLast(succeeded);
}

ECode ContentResolver::CursorWrapperInner::MoveToNext(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    return CrossProcessCursorWrapper::MoveToNext(succeeded);
}

ECode ContentResolver::CursorWrapperInner::MoveToPrevious(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    return CrossProcessCursorWrapper::MoveToPrevious(succeeded);
}

ECode ContentResolver::CursorWrapperInner::IsFirst(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return CrossProcessCursorWrapper::IsFirst(result);
}

ECode ContentResolver::CursorWrapperInner::IsLast(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return CrossProcessCursorWrapper::IsLast(result);
}

ECode ContentResolver::CursorWrapperInner::IsBeforeFirst(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return CrossProcessCursorWrapper::IsBeforeFirst(result);
}

ECode ContentResolver::CursorWrapperInner::IsAfterLast(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return CrossProcessCursorWrapper::IsAfterLast(result);
}

ECode ContentResolver::CursorWrapperInner::GetColumnIndex(
    /* [in] */ const String& columnName,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    return CrossProcessCursorWrapper::GetColumnIndex(columnName, index);
}

ECode ContentResolver::CursorWrapperInner::GetColumnIndexOrThrow(
    /* [in] */  const String& columnName,
    /* [out] */ Int32* columnIndex)
{
    VALIDATE_NOT_NULL(columnIndex);
    return CrossProcessCursorWrapper::GetColumnIndexOrThrow(columnName, columnIndex);
}

ECode ContentResolver::CursorWrapperInner::GetColumnName(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return CrossProcessCursorWrapper::GetColumnName(columnIndex, name);
}

ECode ContentResolver::CursorWrapperInner::GetColumnNames(
    /* [out, callee] */ ArrayOf<String>** columnNames)
{
    VALIDATE_NOT_NULL(columnNames);
    return CrossProcessCursorWrapper::GetColumnNames(columnNames);
}

ECode ContentResolver::CursorWrapperInner::GetColumnCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    return CrossProcessCursorWrapper::GetColumnCount(count);
}

ECode ContentResolver::CursorWrapperInner::GetBlob(
    /* [in] */  Int32 columnIndex,
    /* [out,callee] */ ArrayOf<Byte>** blob)
{
    VALIDATE_NOT_NULL(blob);
    return CrossProcessCursorWrapper::GetBlob(columnIndex, blob);
}

ECode ContentResolver::CursorWrapperInner::GetString(
    /* [in] */ Int32 columnIndex,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    return CrossProcessCursorWrapper::GetString(columnIndex, value);
}

ECode ContentResolver::CursorWrapperInner::CopyStringToBuffer(
    /* [in] */ Int32 columnIndex,
    /* [in] */ ICharArrayBuffer* buffer)
{
    return CrossProcessCursorWrapper::CopyStringToBuffer(columnIndex, buffer);
}

ECode ContentResolver::CursorWrapperInner::GetInt32(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value);
    return CrossProcessCursorWrapper::GetInt32(columnIndex, value);
}

ECode ContentResolver::CursorWrapperInner::GetInt64(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value);
    return CrossProcessCursorWrapper::GetInt64(columnIndex, value);
}

ECode ContentResolver::CursorWrapperInner::GetInt16(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int16* value)
{
    VALIDATE_NOT_NULL(value);
    return CrossProcessCursorWrapper::GetInt16(columnIndex, value);
}

ECode ContentResolver::CursorWrapperInner::GetDouble(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Double* value)
{
    VALIDATE_NOT_NULL(value);
    return CrossProcessCursorWrapper::GetDouble(columnIndex, value);
}

ECode ContentResolver::CursorWrapperInner::GetFloat(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value);
    return CrossProcessCursorWrapper::GetFloat(columnIndex, value);
}

ECode ContentResolver::CursorWrapperInner::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras);
    return CrossProcessCursorWrapper::GetExtras(extras);
}

ECode ContentResolver::CursorWrapperInner::GetType(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    return CrossProcessCursorWrapper::GetType(columnIndex, type);
}

ECode ContentResolver::CursorWrapperInner::IsNull(
    /* [in] */ Int32 columnIndex,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    return CrossProcessCursorWrapper::IsNull(columnIndex, result);
}

ECode ContentResolver::CursorWrapperInner::RegisterContentObserver(
    /* [in] */ IContentObserver* observer)
{
    return CrossProcessCursorWrapper::RegisterContentObserver(observer);
}

ECode ContentResolver::CursorWrapperInner::RegisterDataSetObserver(
    /* [in] */IDataSetObserver* observer)
{
    return CrossProcessCursorWrapper::RegisterDataSetObserver(observer);
}

ECode ContentResolver::CursorWrapperInner::Requery(
    /* [out] */ Boolean* succeeded)
{
    VALIDATE_NOT_NULL(succeeded);
    return CrossProcessCursorWrapper::Requery(succeeded);
}

ECode ContentResolver::CursorWrapperInner::Respond(
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** bundle)
{
    VALIDATE_NOT_NULL(bundle);
    return CrossProcessCursorWrapper::Respond(extras, bundle);
}

ECode ContentResolver::CursorWrapperInner::SetNotificationUri(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* uri)
{
    return CrossProcessCursorWrapper::SetNotificationUri(cr, uri);
}

ECode ContentResolver::CursorWrapperInner::UnregisterContentObserver(
    /* [in] */ IContentObserver* observer)
{
    return CrossProcessCursorWrapper::UnregisterContentObserver(observer);
}

ECode ContentResolver::CursorWrapperInner::UnregisterDataSetObserver(
    /* [in] */ IDataSetObserver* observer)
{
    return CrossProcessCursorWrapper::UnregisterDataSetObserver(observer);
}

ECode ContentResolver::CursorWrapperInner::IsClosed(
    /* [out] */ Boolean* isClosed)
{
    VALIDATE_NOT_NULL(isClosed);
    return CrossProcessCursorWrapper::IsClosed(isClosed);
}

ECode ContentResolver::CursorWrapperInner::Deactivate()
{
    return CrossProcessCursorWrapper::Deactivate();
}

ECode ContentResolver::CursorWrapperInner::GetWantsAllOnMoveCalls(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value);
    return CrossProcessCursorWrapper::GetWantsAllOnMoveCalls(value);
}

ECode ContentResolver::CursorWrapperInner::GetWrappedCursor(
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor);
    return CrossProcessCursorWrapper::GetWrappedCursor(cursor);
}


ContentResolver::ParcelFileDescriptorInner::ParcelFileDescriptorInner(
    /* [in] */ IParcelFileDescriptor* pfd,
    /* [in] */ IIContentProvider* icp,
    /* [in] */ ContentResolver* contentResolver)
    : ParcelFileDescriptor(pfd)
    , mContentProvider(icp)
    , mReleaseProviderFlag(FALSE)
    , mContentResolver(contentResolver)
{
}

ContentResolver::ParcelFileDescriptorInner::~ParcelFileDescriptorInner()
{
    if (!mReleaseProviderFlag) {
        Close();
    }
}

CAR_INTERFACE_IMPL_2(ContentResolver::ParcelFileDescriptorInner, IParcelFileDescriptor, IParcelable)

ECode ContentResolver::ParcelFileDescriptorInner::Close()
{
    if (!mReleaseProviderFlag) {
        ParcelFileDescriptor::Close();
        Boolean result = FALSE;
        mContentResolver->ReleaseProvider(mContentProvider, &result);
        mReleaseProviderFlag = TRUE;
    }
    return NOERROR;
}

ECode ContentResolver::ParcelFileDescriptorInner::GetFileDescriptor(
    /* [out] */ IFileDescriptor** des)
{
    VALIDATE_NOT_NULL(des);
    return ParcelFileDescriptor::GetFileDescriptor(des);
}

ECode ContentResolver::ParcelFileDescriptorInner::GetStatSize(
    /* [out] */ Int64* len)
{
    VALIDATE_NOT_NULL(len);
    return ParcelFileDescriptor::GetStatSize(len);
}

ECode ContentResolver::ParcelFileDescriptorInner::SeekTo(
    /* [in] */ Int64 startOffset,
    /* [out] */ Int64* toOffset)
{
    VALIDATE_NOT_NULL(toOffset);
    return ParcelFileDescriptor::SeekTo(startOffset, toOffset);
}

ECode ContentResolver::ParcelFileDescriptorInner::GetFd(
    /* [out] */ Int32* fd)
{
    VALIDATE_NOT_NULL(fd);
    return ParcelFileDescriptor::GetFd(fd);
}

ECode ContentResolver::ParcelFileDescriptorInner::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return ParcelFileDescriptor::ReadFromParcel(source);
}

ECode ContentResolver::ParcelFileDescriptorInner::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return ParcelFileDescriptor::WriteToParcel(dest);
}

//========================================================================
// ContentResolver
//========================================================================
static AutoPtr<ArrayOf<String> > InitSYNC_ERROR_NAMES()
{
    AutoPtr<ArrayOf<String> > array = ArrayOf<String>::Alloc(8);

    array->Set(0, "already-in-progress");
    array->Set(1, "authentication-error");
    array->Set(2, "io-error");
    array->Set(3, "parse-error");
    array->Set(4, "conflict");
    array->Set(5, "too-many-deletions");
    array->Set(6, "too-many-retries");
    array->Set(7, "internal-error");
    return array;
}

const AutoPtr<ArrayOf<String> > ContentResolver::SYNC_ERROR_NAMES = InitSYNC_ERROR_NAMES();

const String ContentResolver::TAG("ContentResolver");

// Always log queries which take 500ms+; shorter queries are
// sampled accordingly.
const Boolean ContentResolver::ENABLE_CONTENT_SAMPLE = FALSE;
const Int32 ContentResolver::SLOW_THRESHOLD_MILLIS = 500;


AutoPtr<IContentService> ContentResolver::sContentService;

CAR_INTERFACE_IMPL(ContentResolver, Object, IContentResolver)

ContentResolver::ContentResolver()
{
}

ContentResolver::~ContentResolver()
{
    mContext = NULL;
}

ECode ContentResolver::constructor(
    /* [in] */ IContext* context)
{
    CRandom::New((IRandom**)&mRandom);

    if (context != NULL) {
        mContext = context;
    }
    else {
        mContext = ActivityThread::CurrentApplication();
    }

    mContext->GetOpPackageName(&mPackageName);
    return NOERROR;
}

ECode ContentResolver::AcquireExistingProvider(
    /* [in] */ IContext* c,
    /* [in] */ const String& name,
    /* [out] */ IIContentProvider** contentProvider)
{
    return AcquireProvider(c, name, contentProvider);
}

ECode ContentResolver::AppNotRespondingViaProvider(
    /* [in] */ IIContentProvider* icp)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode ContentResolver::GetType(
    /* [in] */ IUri* uri,
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type)
    *type = String(NULL);

    // XXX would like to have an acquireExistingUnstableProvider for this.
    AutoPtr<IIContentProvider> provider;
    FAIL_RETURN(AcquireExistingProvider(uri, (IIContentProvider**)&provider))
    if (NULL != provider) {
//        try {
        ECode ec = provider->GetType(uri, type);
//        } catch (RemoteException e) {
//            return null;
//        } catch (java.lang.Exception e) {
//            return null;
//        } finally {
        Boolean result;
        ReleaseProvider(provider, &result);
//        }
        FAIL_RETURN(ec)
    }

    String scheme;
    FAIL_RETURN(uri->GetScheme(&scheme))
    if (IContentResolver::SCHEME_CONTENT.Equals(scheme)) {
        return NOERROR;
    }

//    try {
    AutoPtr<IUri> newUri;
    ContentProvider::GetUriWithoutUserId(uri, (IUri**)&newUri);
    Int32 userId = ResolveUserId(uri);
    return ActivityManagerNative::GetDefault()->GetProviderMimeType(newUri, userId, type);
//    } catch (RemoteException e) {
//        // Arbitrary and not worth documenting, as Activity
//        // Manager will kill this process shortly anyway.
//        return null;
//    } catch (java.lang.Exception e) {
//        Log.w(TAG, "Failed to get type for: " + url + " (" + e.getMessage() + ")");
//        return null;
//    }
}

ECode ContentResolver::GetStreamTypes(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mimeTypeFilter,
    /* [out, callee] */ ArrayOf<String>** streamTypes)
{
    VALIDATE_NOT_NULL(streamTypes);
    *streamTypes = NULL;

    AutoPtr<IIContentProvider> provider;
    FAIL_RETURN(AcquireProvider(uri, (IIContentProvider**)&provider));
    if (NULL == provider) {
        return NOERROR;
    }

//    try {
    ECode ec = provider->GetStreamTypes(uri, mimeTypeFilter, streamTypes);
//    } catch (RemoteException e) {
//        // Arbitrary and not worth documenting, as Activity
//        // Manager will kill this process shortly anyway.
//        return null;
//    } finally {
    Boolean isRelease = FALSE;
    ReleaseProvider(provider, &isRelease);
//    }
    return ec;
}

ECode ContentResolver::Query(
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& sortOrder,
    /* [out] */ ICursor** cursor)
{
    return Query(uri, projection, selection, selectionArgs, sortOrder, NULL, cursor);
}

ECode ContentResolver::Query(
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& sortOrder,
    /* [in] */ ICancellationSignal* cancellationSignal,
    /* [out] */ ICursor** cursor)
{
    VALIDATE_NOT_NULL(cursor)
    *cursor = NULL;

    AutoPtr<IIContentProvider> unstableProvider;
    FAIL_RETURN(AcquireUnstableProvider(uri, (IIContentProvider**)&unstableProvider))
    if (NULL == unstableProvider) {
        return NOERROR;
    }

    AutoPtr<IIContentProvider> stableProvider;
    AutoPtr<CursorWrapperInner> wrapper;
    Int32 count = 0;
    Boolean isRelease = FALSE;

//    try {
    Int64 startTime = SystemClock::GetUptimeMillis();
    Int64 durationMillis;

    AutoPtr<IICancellationSignal> remoteCancellationSignal;
    if (NULL != cancellationSignal) {
        cancellationSignal->ThrowIfCanceled();
        unstableProvider->CreateCancellationSignal((IICancellationSignal**)&remoteCancellationSignal);
        cancellationSignal->SetRemote(remoteCancellationSignal);
    }

    AutoPtr<ICursor> qCursor, temp;
    // try {
    ECode ec = unstableProvider->Query(uri, projection,
            selection, selectionArgs, sortOrder, remoteCancellationSignal, (ICursor**)&temp);

    if (ec == (ECode)E_DEAD_OBJECT_EXCEPTION) {
        // The remote process has died...  but we only hold an unstable
        // reference though, so we might recover!!!  Let's try!!!!
        // This is exciting!!1!!1!!!!1
        UnstableProviderDied(unstableProvider);
        stableProvider = NULL;
        AcquireProvider(uri, (IIContentProvider**)&stableProvider);
        if (stableProvider == NULL) {
            goto __EXIT__;
        }

        temp = NULL;
        ec = stableProvider->Query(uri, projection, selection, selectionArgs,
            sortOrder, remoteCancellationSignal, (ICursor**)&temp);
        FAIL_GOTO(ec, __EXIT__)
    }
    else if (FAILED(ec)) {
        goto __EXIT__;
    }

    if (qCursor == NULL) {
        goto __EXIT__;
    }

    // Force query execution.  Might fail and throw a runtime exception here.
    ec = qCursor->GetCount(&count);
    FAIL_GOTO(ec, __EXIT__)

    durationMillis = SystemClock::GetUptimeMillis() - startTime;
    MaybeLogQueryToEventLog(durationMillis, uri, projection, selection, sortOrder);

    // Wrap the cursor object into CursorWrapperInner object
    if (NULL == stableProvider) {
        ec = AcquireProvider(uri, (IIContentProvider**)&stableProvider);
        FAIL_GOTO(ec, __EXIT__)
    }
    wrapper = new CursorWrapperInner(qCursor, stableProvider, this);
    stableProvider = NULL;
    qCursor = NULL;
    goto __EXIT__;

__EXIT__:
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
       // Arbitrary and not worth documenting, as Activity
       // Manager will kill this process shortly anyway.
        return NOERROR;
    }

    if (qCursor != NULL) {
        ICloseable::Probe(qCursor)->Close();
    }

    if (cancellationSignal != NULL) {
        cancellationSignal->SetRemote(NULL);
    }

    if (NULL != unstableProvider) {
        ReleaseUnstableProvider(unstableProvider, &isRelease);
    }
    if (NULL != stableProvider) {
        ReleaseProvider(stableProvider, &isRelease);
    }
//    }
    *cursor = wrapper.Get();
    REFCOUNT_ADD(*cursor);
    return ec;
}

ECode ContentResolver::Canonicalize(
    /* [in] */ IUri* uri,
    /* [out] */ IUri** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    AutoPtr<IIContentProvider> provider;
    AcquireProvider(url, (IIContentProvider**)&provider);
    if (provider == NULL) {
        return NOERROR;
    }

    // try {
    ECode ec = provider->Canonicalize(mPackageName, url, result);
    // } catch (RemoteException e) {
    //     // Arbitrary and not worth documenting, as Activity
    //     // Manager will kill this process shortly anyway.
    //     return null;
    // } finally {
    ReleaseProvider(provider);
    // }

    return ec;
}

ECode ContentResolver::Uncanonicalize(
    /* [in] */ IUri* uri,
    /* [out] */ IUri** result)
{
    VALIDATE_NOT_NULL(result)
    *result = NULL;

    AutoPtr<IIContentProvider> provider;
    AcquireProvider(url, (IIContentProvider**)&provider);
    if (provider == NULL) {
        return NOERROR;
    }

    // try {
    ECode ec = provider->Uncanonicalize(mPackageName, url, result);
    // } catch (RemoteException e) {
    //     // Arbitrary and not worth documenting, as Activity
    //     // Manager will kill this process shortly anyway.
    //     return null;
    // } finally {
    ReleaseProvider(provider);
    // }

    return ec;
}

ECode ContentResolver::OpenInputStream(
    /* [in] */ IUri* uri,
    /* [out] */ IInputStream** inStream)
{
    VALIDATE_NOT_NULL(inStream);
    *inStream = NULL;

    String scheme;
    FAIL_RETURN(uri->GetScheme(&scheme));
    if (IContentResolver::SCHEME_ANDROID_RESOURCE.Equals(scheme)) {
        // Note: left here to avoid breaking compatibility.  May be removed
        // with sufficient testing.
        AutoPtr<IContentResolverOpenResourceIdResult> r;
        FAIL_RETURN(GetResourceId(uri, (IContentResolverOpenResourceIdResult**)&r));
//        try {
        AutoPtr<IResources> resources;
        FAIL_RETURN(r->GetResources((IResources**)&resources));
        Int32 id = 0;
        FAIL_RETURN(r->GetResourceId(&id));
        return resources->OpenRawResource(id, inStream);
//        } catch (Resources.NotFoundException ex) {
//            throw new FileNotFoundException("Resource does not exist: " + uri);
//        }
    }
    else if (SCHEME_FILE.Equals(scheme)) {
        // Note: left here to avoid breaking compatibility.  May be removed
        // with sufficient testing.
        String path;
        FAIL_RETURN(uri->GetPath(&path));
        AutoPtr<IFileInputStream> fi;
        CFileInputStream::New(path, (IFileInputStream**)&fi);
        *inStream = IInputStream::Probe(fi);
        REFCOUNT_ADD(*inStream);
        return NOERROR;
    }
    else {
        AutoPtr<IAssetFileDescriptor> fd;
        FAIL_RETURN(OpenAssetFileDescriptor(uri, String("r"), (IAssetFileDescriptor**)&fd));
//        try {
        if (fd == NULL) {
            return NOERROR;
        }
        else {
            AutoPtr<IFileInputStream> fi;
            fd->CreateInputStream((IFileInputStream**)&fi);
            *inStream = IInputStream::Probe(fi);
            REFCOUNT_ADD(*inStream);
            return NOERROR;
        }
//        } catch (IOException e) {
//            throw new FileNotFoundException("Unable to create stream");
//        }
    }
}

ECode ContentResolver::OpenOutputStream(
    /* [in] */ IUri* uri,
    /* [out] */ IOutputStream** outStream)
{
    return OpenOutputStream(uri, String("w"), outStream);
}

ECode ContentResolver::OpenOutputStream(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IOutputStream** outStream)
{
    VALIDATE_NOT_NULL(outStream)
    *outStream = NULL;

    AutoPtr<IAssetFileDescriptor> fd;
    FAIL_RETURN(OpenAssetFileDescriptor(uri, mode, (IAssetFileDescriptor**)&fd))
//    try {
    if (NULL != fd) {
        AutoPtr<IFileOutputStream> fo;
        FAIL_RETURN(fd->CreateOutputStream((IFileOutputStream**)&fo))
        *outStream = IOutputStream::Probe(fo);
        REFCOUNT_ADD(*outStream)
    }

    return NOERROR;
//    } catch (IOException e) {
//        throw new FileNotFoundException("Unable to create stream");
//    }
}

ECode ContentResolver::OpenFileDescriptor(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IParcelFileDescriptor** fileDescriptor)
{
    return OpenFileDescriptor(uri, mode, fileDescriptor);
}

ECode ContentResolver::OpenFileDescriptor(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [in] */ ICancellationSignal* signal,
    /* [out] */ IParcelFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor)
    *fileDescriptor = NULL;

    AutoPtr<IAssetFileDescriptor> afd;
    FAIL_RETURN(OpenAssetFileDescriptor(uri, mode, signal, (IAssetFileDescriptor**)&afd))
    if (NULL == afd) {
        return NOERROR;
    }

    Int64 length = 0;
    FAIL_RETURN(afd->GetDeclaredLength(&length))
    if (length < 0) {
        // This is a full file!
        return afd->GetParcelFileDescriptor(fileDescriptor);
    }

    // Client can't handle a sub-section of a file, so close what
    // we got and bail with an exception.
    ICloseable::Probe(afd)->Close();

    // throw new FileNotFoundException("Not a whole file");
    return E_FILE_NOT_FOUND_EXCEPTION;
}

ECode ContentResolver::OpenAssetFileDescriptor(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    return OpenAssetFileDescriptor(uri, mode, NULL, fileDescriptor);
}

ECode ContentResolver::OpenAssetFileDescriptor(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [in] */ ICancellationSignal* cancellationSignal,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor)
    String scheme;
    FAIL_RETURN(uri->GetScheme(&scheme))

    if (IContentResolver::SCHEME_ANDROID_RESOURCE.Equals(scheme)) {
        if (!mode.Equals("r")) {
            // throw new FileNotFoundException("Can't write resources: " + uri);
            return E_FILE_NOT_FOUND_EXCEPTION;
        }
        AutoPtr<IContentResolverOpenResourceIdResult> r;
        FAIL_RETURN(GetResourceId(uri, (IContentResolverOpenResourceIdResult**)&r));
//        try {
        AutoPtr<IResources> resources;
        FAIL_RETURN(r->GetResources((IResources**)&resources));
        Int32 id = 0;
        FAIL_RETURN(r->GetResourceId(&id));
        return resources->OpenRawResourceFd(id, fileDescriptor);
//        } catch (Resources.NotFoundException ex) {
//            throw new FileNotFoundException("Resource does not exist: " + uri);
//        }
    }
    else if (IContentResolver::SCHEME_FILE.Equals(scheme)) {
        AutoPtr<IFile> file;
        AutoPtr<IParcelFileDescriptor> pfd;
        String path;
        FAIL_RETURN(uri->GetPath(&path));
        FAIL_RETURN(CFile::New(path, (IFile**)&file));
        Int32 nMode;
        FAIL_RETURN(ParcelFileDescriptor::ParseMode(mode, &nMode));
        FAIL_RETURN(ParcelFileDescriptor::Open(file, nMode, (IParcelFileDescriptor**)&pfd));
        return CAssetFileDescriptor::New(pfd, 0, -1, fileDescriptor);
    }
    else {
        if (mode.Equals("r")) {
            return OpenTypedAssetFileDescriptor(uri, String("*/*"), NULL, cancellationSignal, fileDescriptor);
        }
        else {
            AutoPtr<IIContentProvider> unstableProvider;
            FAIL_RETURN(AcquireUnstableProvider(uri, (IIContentProvider**)&unstableProvider));
            if (NULL == unstableProvider) {
                //throw new FileNotFoundException("No content provider: " + uri);
                return E_FILE_NOT_FOUND_EXCEPTION;
            }
            AutoPtr<IIContentProvider> stableProvider;
            AutoPtr<IAssetFileDescriptor> fd;
            Boolean isRelease = FALSE;

//            try {
            AutoPtr<IICancellationSignal> remoteCancellationSignal;
            if (cancellationSignal != NULL) {
                FAIL_RETURN(cancellationSignal->ThrowIfCanceled())
                FAIL_RETURN(unstableProvider->CreateCancellationSignal((IICancellationSignal**)&remoteCancellationSignal))
                FAIL_RETURN(cancellationSignal->SetRemote(remoteCancellationSignal))
            }
//                try {
            ECode ec = unstableProvider->OpenAssetFile(uri, mode, remoteCancellationSignal, (IAssetFileDescriptor**)&fd);
            if (ec == (ECode)E_DEAD_OBJECT_EXCEPTION) {
                // The remote process has died...  but we only hold an unstable
                // reference though, so we might recover!!!  Let's try!!!!
                // This is exciting!!1!!1!!!!1
                UnstableProviderDied(unstableProvider);
                stableProvider = NULL;
                AcquireProvider(uri, (IIContentProvider**)&stableProvider);
                if (stableProvider == NULL) {
                    // throw new FileNotFoundException("No content provider: " + uri);
                    return E_FILE_NOT_FOUND_EXCEPTION;
                }
                fd = NULL;
                stableProvider->OpenAssetFile(mPackageName, uri, mode, remoteCancellationSignal, (IAssetFileDescriptor**)&fd);
                if (fd == NULL) {
                    // The provider will be released by the finally{} clause
                    *fileDescriptor = NULL;
                    ec = NOERROR;
                    goto __RETURN__;
                }
            }
            else if (FAILED(ec)) {
                // The provider will be released by the finally{} clause
                *fileDescriptor = NULL;
                goto __EXIT__;
            }

            if (NULL == fd) {
                // The provider will be released by the finally{} clause
                *fileDescriptor = NULL;
                ec = NOERROR;
                goto __RETURN__;
            }

            if (NULL == stableProvider) {
                AcquireProvider(uri, (IIContentProvider**)&stableProvider);
            }

            ReleaseUnstableProvider(unstableProvider, &isRelease);
            AutoPtr<IParcelFileDescriptor> pfd2;
            fd->GetParcelFileDescriptor((IParcelFileDescriptor**)&pfd2);
            AutoPtr<IParcelFileDescriptor> pfd;
            pfd = new ParcelFileDescriptorInner(pfd2, stableProvider, this);

            // Success!  Don't release the provider when exiting, let
            // ParcelFileDescriptorInner do that when it is closed.
            stableProvider = NULL;

            Int64 startOffset = 0;
            Int64 length = 0;
            fd->GetStartOffset(&startOffset);
            fd->GetDeclaredLength(&length);
//            } catch (RemoteException e) {
//                // Whatever, whatever, we'll go away.
//                throw new FileNotFoundException(
//                      array->Set(0, "Failed opening content provider: " + uri);
//            } catch (FileNotFoundException e) {
//                throw e;
//            } finally {

__EXIT__:
            if (NULL != stableProvider) {
                ReleaseProvider(stableProvider, &isRelease);
            }
            if (NULL != unstableProvider) {
                ReleaseUnstableProvider(unstableProvider, &isRelease);
            }

            FAIL_RETURN(ec)
//            }
            return CAssetFileDescriptor::New(pfd, startOffset, length, fileDescriptor);

__RETURN__:
            if (NULL != stableProvider) {
                ReleaseProvider(stableProvider, &isRelease);
            }
            if (NULL != unstableProvider) {
                ReleaseUnstableProvider(unstableProvider, &isRelease);
            }
            return ec;
        }
    }
}


ECode ContentResolver::OpenTypedAssetFileDescriptor(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mimeType,
    /* [in] */ IBundle* opts,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    return OpenTypedAssetFileDescriptor(uri, mimeType, opts, NULL, fileDescriptor);
}

ECode ContentResolver::OpenTypedAssetFileDescriptor(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mimeType,
    /* [in] */ IBundle* opts,
    /* [in] */ ICancellationSignal* cancellationSignal,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    VALIDATE_NOT_NULL(fileDescriptor)
    *fileDescriptor = NULL;

    AutoPtr<IIContentProvider> unstableProvider;
    FAIL_RETURN(AcquireUnstableProvider(uri, (IIContentProvider**)&unstableProvider))
    if (NULL == unstableProvider) {
        //throw new FileNotFoundException("No content provider: " + uri);
        return E_FILE_NOT_FOUND_EXCEPTION;
    }
    AutoPtr<IIContentProvider> stableProvider;
    AutoPtr<IAssetFileDescriptor> fd;
    Boolean isRelease = FALSE;
    AutoPtr<IParcelFileDescriptor> pfd;
    AutoPtr<IParcelFileDescriptor> pfd2;
    Int64 startOffset = 0;
    Int64 length = 0;

//    try {
    AutoPtr<IICancellationSignal> remoteCancellationSignal;
    if (cancellationSignal != NULL) {
        FAIL_RETURN(cancellationSignal->ThrowIfCanceled())
        FAIL_RETURN(unstableProvider->CreateCancellationSignal((IICancellationSignal**)&remoteCancellationSignal))
        FAIL_RETURN(cancellationSignal->SetRemote(remoteCancellationSignal))
    }
 //       try {
    ECode ec = unstableProvider->OpenTypedAssetFile(uri, mimeType, opts, (IAssetFileDescriptor**)&fd);
    if (ec == (ECode)E_DEAD_OBJECT_EXCEPTION) {
       // The remote process has died...  but we only hold an unstable
       // reference though, so we might recover!!!  Let's try!!!!
       // This is exciting!!1!!1!!!!1
       UnstableProviderDied(unstableProvider);
       stableProvider = NULL;
       AcquireProvider(uri, (IIContentProvider**)&stableProvider);
       if (stableProvider == NULL) {
           // throw new FileNotFoundException("No content provider: " + uri);
            ec = E_FILE_NOT_FOUND_EXCEPTION;
            goto __EXIT__;
       }
       stableProvider->OpenTypedAssetFile(uri, mimeType, opts, (IAssetFileDescriptor**)&fd);
       if (fd == NULL) {
           // The provider will be released by the finally{} clause
           goto __RETURN__;
       }
    }
    else if (FAILED(ec)) {
        goto __EXIT__;
    }

    if (fd == NULL) {
        // The provider will be released by the finally{} clause
        goto __RETURN__;
    }

    if (NULL == stableProvider) {
        AcquireProvider(uri, (IIContentProvider**)&stableProvider);
    }
    ReleaseUnstableProvider(unstableProvider, &isRelease);
    fd->GetParcelFileDescriptor((IParcelFileDescriptor**)&pfd2);

    pfd = new ParcelFileDescriptorInner(pfd2, stableProvider, this);

    // Success!  Don't release the provider when exiting, let
    // ParcelFileDescriptorInner do that when it is closed.
    stableProvider = NULL;

    fd->GetStartOffset(&startOffset);
    fd->GetDeclaredLength(&length);
//    } catch (RemoteException e) {
//        // Whatever, whatever, we'll go away.
//        throw new FileNotFoundException(
//                "Failed opening content provider: " + uri);
//    } catch (FileNotFoundException e) {
//        throw e;
//    } finally {

__EXIT__:
    if (FAILED(ec)) {
        ec = E_FILE_NOT_FOUND_EXCEPTION;
    }
    else {
        ec = CAssetFileDescriptor::New(pfd, startOffset, length, fileDescriptor);
    }

    if (NULL != stableProvider) {
        ReleaseProvider(stableProvider, &isRelease);
    }
    if (NULL != unstableProvider) {
        ReleaseUnstableProvider(unstableProvider, &isRelease);
    }
//    }
    return ec;

__RETURN__:
    if (NULL != stableProvider) {
        ReleaseProvider(stableProvider, &isRelease);
    }
    if (NULL != unstableProvider) {
        ReleaseUnstableProvider(unstableProvider, &isRelease);
    }
    return NOERROR;
}

ECode ContentResolver::GetResourceId(
    /* [in] */ IUri* uri,
    /* [out] */ IContentResolverOpenResourceIdResult** resourceIdResult)
{
    VALIDATE_NOT_NULL(resourceIdResult)
    *resourceIdResult = NULL;

    String authority;
    FAIL_RETURN(uri->GetAuthority(&authority))
    AutoPtr<IResources> r;
    if (authority.IsNullOrEmpty()) {
        // throw new FileNotFoundException("No authority: " + uri);
        return E_FILE_NOT_FOUND_EXCEPTION;
    }
    else {
        AutoPtr<IPackageManager> packageManager;
        FAIL_RETURN(mContext->GetPackageManager((IPackageManager**)&packageManager))
        ECode ecode = packageManager->GetResourcesForApplication(authority, (IResources**)&r);
        if ((ECode)E_NAME_NOT_FOUND_EXCEPTION == ecode) {
            // throw new FileNotFoundException("No package found for authority: " + uri);
            return E_FILE_NOT_FOUND_EXCEPTION;
        }
    }
    AutoPtr<ArrayOf<String> > path;
    FAIL_RETURN(uri->GetPathSegments((ArrayOf<String>**)&path))
    if (NULL == path) {
        // throw new FileNotFoundException("No path: " + uri);
        return E_FILE_NOT_FOUND_EXCEPTION;
    }
    Int32 len = path->GetLength();
    Int32 id = 0;
    if (len == 1) {
        // try {
        id = StringUtils::ParseInt32((*path)[0]);
        // } catch (NumberFormatException e) {
        //     throw new FileNotFoundException("Single path segment is not a resource ID: " + uri);
        // }
    }
    else if (len == 2) {
        FAIL_RETURN(r->GetIdentifier((*path)[1], (*path)[0], authority, &id));
    }
    else {
        // throw new FileNotFoundException("More than two path segments: " + uri);
        return E_FILE_NOT_FOUND_EXCEPTION;
    }
    if (id == 0) {
        // throw new FileNotFoundException("No resource found for: " + uri);
        return E_FILE_NOT_FOUND_EXCEPTION;
    }

    AutoPtr<IContentResolverOpenResourceIdResult> res;
    FAIL_RETURN(CContentResolverOpenResourceIdResult::New((IContentResolverOpenResourceIdResult**)&res));
    FAIL_RETURN(res->SetResources(r))
    FAIL_RETURN(res->SetResourceId(id))
    *resourceIdResult = res;
    REFCOUNT_ADD(*resourceIdResult)
    return NOERROR;
}

ECode ContentResolver::Insert(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values,
    /* [out] */ IUri** insertedUri)
{
    VALIDATE_NOT_NULL(insertedUri)
    AutoPtr<IIContentProvider> provider;
    FAIL_RETURN(AcquireProvider(uri, (IIContentProvider**)&provider))
    if (NULL == provider) {
        // throw new IllegalArgumentException("Unknown URL " + url);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
//    try {
    Int64 startTime = SystemClock::GetUptimeMillis();
    AutoPtr<IUri> createdRow;
    ECode ec = provider->Insert(uri, values, (IUri**)&createdRow);
    if (SUCCEEDED(ec)) {
        Int64 durationMillis = SystemClock::GetUptimeMillis() - startTime;
        MaybeLogUpdateToEventLog(durationMillis, uri, String("insert"), String(NULL) /* where */);
        *insertedUri = createdRow;
        REFCOUNT_ADD(*insertedUri)
    }
    else {
        *insertedUri = NULL;
    }
//    } catch (RemoteException e) {
//        // Arbitrary and not worth documenting, as Activity
//        // Manager will kill this process shortly anyway.
//        return null;
//    } finally {
    Boolean isRelease = FALSE;
    ReleaseProvider(provider, &isRelease);
//    }
    return (ec == (ECode)E_REMOTE_EXCEPTION) ? NOERROR : ec;
}

ECode ContentResolver::ApplyBatch(
    /* [in] */ const String& authority,
    /* [in] */ IArrayList* operations,
    /* [out, callee] */ ArrayOf<IContentProviderResult*>** providerResults)
{
    AutoPtr<IContentProviderClient> providerClient;
    *providerResults = NULL;

    FAIL_RETURN(AcquireContentProviderClient(authority, (IContentProviderClient**)&providerClient))
    if (NULL == providerClient) {
        //throw new IllegalArgumentException("Unknown authority " + authority);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    ECode ec = providerClient->ApplyBatch(operations, providerResults);
    Boolean isRelease = FALSE;
    providerClient->ReleaseProvider(&isRelease);
    return ec;
}

ECode ContentResolver::BulkInsert(
    /* [in] */ IUri* url,
    /* [in] */ ArrayOf<IContentValues*>* values,
    /* [out] */ Int32* number)
{
    VALIDATE_NOT_NULL(number);
    *number = 0;

    AutoPtr<IIContentProvider> provider;
    FAIL_RETURN(AcquireProvider(url, (IIContentProvider**)&provider))
    if (NULL == provider) {
        // throw new IllegalArgumentException("Unknown URL " + url);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
//    try {
    Int64 startTime = SystemClock::GetUptimeMillis();
    Int32 rowsCreated = 0;
    provider->BulkInsert(url, values, &rowsCreated);
    Int64 durationMillis = SystemClock::GetUptimeMillis() - startTime;
    MaybeLogUpdateToEventLog(durationMillis, url, String("bulkinsert"), String(NULL) /* where */);
    *number = rowsCreated;
    Boolean isRelease = FALSE;
    ReleaseProvider(provider, &isRelease);
    return NOERROR;
//    } catch (RemoteException e) {
//        // Arbitrary and not worth documenting, as Activity
//        // Manager will kill this process shortly anyway.
//        return 0;
//    } finally {
//        releaseProvider(provider);
//    }
}

ECode ContentResolver::Delete(
    /* [in] */ IUri* uri,
    /* [in] */ const String& where,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    VALIDATE_NOT_NULL(rowsAffected);
    *rowsAffected = -1;

    AutoPtr<IIContentProvider> provider;
    FAIL_RETURN(AcquireProvider(uri, (IIContentProvider**)&provider))
    if (NULL == provider) {
        // throw new IllegalArgumentException("Unknown URL " + url);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
//    try {
    Int64 startTime = SystemClock::GetUptimeMillis();
    Int32 rowsDeleted = 0;
    ECode ec = provider->Delete(uri, where, selectionArgs, &rowsDeleted);
    if (SUCCEEDED(ec)) {
        Int64 durationMillis = SystemClock::GetUptimeMillis() - startTime;
        MaybeLogUpdateToEventLog(durationMillis, uri, String("delete"), where);
        *rowsAffected = rowsDeleted;
    }
    else {
        *rowsAffected = -1;
    }
    Boolean isRelease = FALSE;
    ReleaseProvider(provider, &isRelease);
    return (ec == (ECode)E_REMOTE_EXCEPTION) ? NOERROR : ec;
//    } catch (RemoteException e) {
//        // Arbitrary and not worth documenting, as Activity
//        // Manager will kill this process shortly anyway.
//        return -1;
//    } finally {
//        releaseProvider(provider);
//    }
}

ECode ContentResolver::Update(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& where,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    VALIDATE_NOT_NULL(rowsAffected);
    *rowsAffected = -1;

    AutoPtr<IIContentProvider> provider;
    FAIL_RETURN(AcquireProvider(uri, (IIContentProvider**)&provider))
    if (NULL == provider) {
        // throw new IllegalArgumentException("Unknown URL " + url);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
//    try {
    Int64 startTime = SystemClock::GetUptimeMillis();
    Int32 rowsUpdated = 0;
    ECode ec = provider->Update(uri, values, where, selectionArgs, &rowsUpdated);
    if (SUCCEEDED(ec)) {
        Int64 durationMillis = SystemClock::GetUptimeMillis() - startTime;
        MaybeLogUpdateToEventLog(durationMillis, uri, String("update"), where);
        *rowsAffected = rowsUpdated;
    }
    else {
        *rowsAffected = -1;
    }
    Boolean isRelease = FALSE;
    ReleaseProvider(provider, &isRelease);
    return (ec == (ECode)E_REMOTE_EXCEPTION) ? NOERROR : ec;
//    } catch (RemoteException e) {
//        // Arbitrary and not worth documenting, as Activity
//        // Manager will kill this process shortly anyway.
//        return -1;
//    } finally {
//        releaseProvider(provider);
//    }
}

ECode ContentResolver::Call(
    /* [in] */ IUri* uri,
    /* [in] */ const String& method,
    /* [in] */ const String& arg,
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** bundle)
{
    VALIDATE_NOT_NULL(bundle)
    *bundle = NULL;

    if (NULL == uri) {
        //throw new NullPointerException("uri == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (method.IsNull()) {
        //throw new NullPointerException("method == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<IIContentProvider> provider;
    FAIL_RETURN(AcquireProvider(uri, (IIContentProvider**)&provider))
    if (NULL == provider) {
        //throw new IllegalArgumentException("Unknown URI " + uri);
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
//    try {
    ECode ec = provider->Call(method, arg, extras, bundle);
    Boolean isRelease = FALSE;
    ReleaseProvider(provider, &isRelease);
//    } catch (RemoteException e) {
//        // Arbitrary and not worth documenting, as Activity
//        // Manager will kill this process shortly anyway.
//        return null;
//    } finally {
//        releaseProvider(provider);
//    }
    return ec;
}

ECode ContentResolver::AcquireProvider(
    /* [in] */ IUri* uri,
    /* [out] */ IIContentProvider** contentProvider)
{
    VALIDATE_NOT_NULL(contentProvider)
    *contentProvider = NULL;

    String scheme;
    uri->GetScheme(&scheme);
    if (!IContentResolver::SCHEME_CONTENT.Equals(scheme)) {
        return NOERROR;
    }

    String auth;
    uri->GetAuthority(&auth);
    if (!auth.IsNull()) {
        return AcquireProvider(mContext, auth, contentProvider);
    }

    return NOERROR;
}

ECode ContentResolver::AcquireExistingProvider(
    /* [in] */ IUri* uri,
    /* [out] */ IIContentProvider** contentProvider)
{
    VALIDATE_NOT_NULL(contentProvider);
    *contentProvider = NULL;

    String scheme;
    uri->GetScheme(&scheme);
    if (!IContentResolver::SCHEME_CONTENT.Equals(scheme)) {
        return NOERROR;
    }

    String auth;
    uri->GetAuthority(&auth);
    if (!auth.IsNull()) {
        return AcquireExistingProvider(mContext, auth, contentProvider);
    }

    return NOERROR;
}

ECode ContentResolver::AcquireProvider(
    /* [in] */ const String& name,
    /* [out] */ IIContentProvider** contentProvider)
{
    VALIDATE_NOT_NULL(contentProvider)

    if (name.IsNull()) {
        *contentProvider = NULL;
        return NOERROR;
    }
    return AcquireProvider(mContext, name, contentProvider);
}

ECode ContentResolver::AcquireUnstableProvider(
    /* [in] */ IUri* uri,
    /* [out] */ IIContentProvider** contentProvider)
{
    VALIDATE_NOT_NULL(contentProvider)
    *contentProvider = NULL;

    String scheme;
    uri->GetScheme(&scheme);
    if (!IContentResolver::SCHEME_CONTENT.Equals(scheme)) {
        return NOERROR;
    }

    String auth;
    uri->GetAuthority(&auth);
    if (!auth.IsNull()) {
        return AcquireUnstableProvider(mContext, auth, contentProvider);
    }

    return NOERROR;
}

ECode ContentResolver::AcquireUnstableProvider(
    /* [in] */ const String& name,
    /* [out] */ IIContentProvider** contentProvider)
{
    VALIDATE_NOT_NULL(contentProvider)
    if (name.IsNull()) {
        *contentProvider = NULL;
        return NOERROR;
    }
    return AcquireUnstableProvider(mContext, name, contentProvider);
}

ECode ContentResolver::AcquireContentProviderClient(
    /* [in] */ IUri* uri,
    /* [out] */ IContentProviderClient** client)
{
    VALIDATE_NOT_NULL(client)
    AutoPtr<IIContentProvider> provider;
    FAIL_RETURN(AcquireProvider(uri, (IIContentProvider**)&provider))
    if (NULL != provider) {
        return CContentProviderClient::New(THIS_PROBE(IContentResolver), provider, TRUE, client);
    }

    *client = NULL;
    return NOERROR;
}

ECode ContentResolver::AcquireContentProviderClient(
    /* [in] */ const String& name,
    /* [out] */ IContentProviderClient** client)
{
    VALIDATE_NOT_NULL(client)
    AutoPtr<IIContentProvider> provider;
    FAIL_RETURN(AcquireProvider(name, (IIContentProvider**)&provider))
    if (NULL != provider) {
        return CContentProviderClient::New(THIS_PROBE(IContentResolver), provider, TRUE, client);
    }

    *client = NULL;
    return NOERROR;
}

ECode ContentResolver::AcquireUnstableContentProviderClient(
    /* [in] */ IUri* uri,
    /* [out] */ IContentProviderClient** client)
{
    VALIDATE_NOT_NULL(client)
    AutoPtr<IIContentProvider> provider;
    FAIL_RETURN(AcquireUnstableProvider(uri, (IIContentProvider**)&provider))
    if (NULL != provider) {
        return CContentProviderClient::New(THIS_PROBE(IContentResolver), provider, FALSE, client);
    }

    *client = NULL;
    return NOERROR;
}

ECode ContentResolver::AcquireUnstableContentProviderClient(
    /* [in] */ const String& name,
    /* [out] */ IContentProviderClient** client)
{
    VALIDATE_NOT_NULL(client);
    AutoPtr<IIContentProvider> provider;
    FAIL_RETURN(AcquireUnstableProvider(name, (IIContentProvider**)&provider))
    if (NULL != provider) {
        return CContentProviderClient::New(THIS_PROBE(IContentResolver), provider, FALSE, client);
    }

    *client = NULL;
    return NOERROR;
}

ECode ContentResolver::RegisterContentObserver(
    /* [in] */ IUri* uri,
    /* [in] */ Boolean notifyForDescendents,
    /* [in] */ IContentObserver* observer)
{
    return RegisterContentObserver(uri, notifyForDescendents, observer, UserHandle::GetMyUserId());
}

ECode ContentResolver::RegisterContentObserver(
    /* [in] */ IUri* uri,
    /* [in] */ Boolean notifyForDescendents,
    /* [in] */ IContentObserver* observer,
    /* [in] */ Int32 userHandle)
{
    VALIDATE_NOT_NULL(observer)
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService));
    assert(contentService != NULL && "Error: Content Service not started.");
    AutoPtr<IIContentObserver> contentObserver;
    FAIL_RETURN(observer->GetContentObserver((IIContentObserver**)&contentObserver));

    return contentService->RegisterContentObserver(uri, notifyForDescendents, contentObserver, userHandle);
}

ECode ContentResolver::UnregisterContentObserver(
    /* [in] */ IContentObserver* observer)
{
    VALIDATE_NOT_NULL(observer);
    AutoPtr<IIContentObserver> contentObserver;
    FAIL_RETURN(observer->ReleaseContentObserver((IIContentObserver**)&contentObserver));
    if (NULL != contentObserver) {
        AutoPtr<IContentService> contentService;
        FAIL_RETURN(GetContentService((IContentService**)&contentService))
        return contentService->UnregisterContentObserver(contentObserver);
    }
    return NOERROR;
}

ECode ContentResolver::NotifyChange(
    /* [in] */ IUri* uri,
    /* [in] */ IContentObserver* observer)
{
    return NotifyChange(uri, observer, TRUE /* sync to network */);
}

ECode ContentResolver::NotifyChange(
    /* [in] */ IUri* uri,
    /* [in] */ IContentObserver* observer,
    /* [in] */ Boolean syncToNetwork)
{
    return NotifyChange(uri, observer, syncToNetwork, UserHandle::GetCallingUserId());
}

ECode ContentResolver::NotifyChange(
    /* [in] */ IUri* uri,
    /* [in] */ IContentObserver* observer,
    /* [in] */ Boolean syncToNetwork,
    /* [in] */ Int32 userHandle)
{
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService))
    AutoPtr<IIContentObserver> contentObserver;
    Boolean deliverSelfNotification = FALSE;
    if (NULL != observer) {
        FAIL_RETURN(observer->GetContentObserver((IIContentObserver**)&contentObserver));
        FAIL_RETURN(observer->DeliverSelfNotifications(&deliverSelfNotification));
    }
    return contentService->NotifyChange(uri, contentObserver,
        (observer != NULL && deliverSelfNotification), syncToNetwork, userHandle);
}

ECode ContentResolver::TakePersistableUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    try {
        ActivityManagerNative.getDefault().takePersistableUriPermission(
                ContentProvider.getUriWithoutUserId(uri), modeFlags, resolveUserId(uri));
    } catch (RemoteException e) {
    }
    return NOERROR;
}

ECode ContentResolver::ReleasePersistableUriPermission(
    /* [in] */ IUri* uri,
    /* [in] */ Int32 modeFlags)
{
    try {
        ActivityManagerNative.getDefault().releasePersistableUriPermission(
                ContentProvider.getUriWithoutUserId(uri), modeFlags, resolveUserId(uri));
    } catch (RemoteException e) {
    }
    return NOERROR;
}

ECode ContentResolver::GetPersistedUriPermissions(
    /* [out] */ IList** perms)
{
    try {
        return ActivityManagerNative.getDefault()
                .getPersistedUriPermissions(mPackageName, true).getList();
    } catch (RemoteException e) {
        throw new RuntimeException("Activity manager has died", e);
    }
    return NOERROR;
}

ECode ContentResolver::GetOutgoingPersistedUriPermissions(
    /* [out] */ IList** perms)
{
    try {
        return ActivityManagerNative.getDefault()
                .getPersistedUriPermissions(mPackageName, false).getList();
    } catch (RemoteException e) {
        throw new RuntimeException("Activity manager has died", e);
    }
    return NOERROR;
}

ECode ContentResolver::StartSync(
    /* [in] */ IUri* uri,
    /* [in] */ IBundle* extras)
{
    AutoPtr<IAccount> account;
    if (NULL != extras) {
        String accountName;
        FAIL_RETURN(extras->GetString(IContentResolver::SYNC_EXTRAS_ACCOUNT, &accountName))
        if (!accountName.IsNullOrEmpty()) {
            CAccount::New(accountName, String("com.google"), (IAccount**)&account);
        }
        FAIL_RETURN(extras->Remove(IContentResolver::SYNC_EXTRAS_ACCOUNT))
    }
    String authority;
    if (NULL != uri) {
        uri->GetAuthority(&authority);
    }
    return RequestSync(account, authority, extras);
}

ECode ContentResolver::RequestSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ IBundle* extras)
{
    FAIL_RETURN(ValidateSyncExtrasBundle(extras))
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService))
    return contentService->RequestSync(account, authority, extras);
}

ECode ContentResolver::RequestSyncAsUser(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ Int32 userId,
    /* [in] */ IBundle* extras)
{
    if (extras == null) {
        throw new IllegalArgumentException("Must specify extras.");
    }
    SyncRequest request =
        new SyncRequest.Builder()
            .setSyncAdapter(account, authority)
            .setExtras(extras)
            .syncOnce()     // Immediate sync.
            .build();
    try {
        getContentService().syncAsUser(request, userId);
    } catch(RemoteException e) {
        // Shouldn't happen.
    }
    return NOERROR;
}

ECode ContentResolver::RequestSync(
    /* [in] */ ISyncRequest* request)
{
    try {
        getContentService().sync(request);
    } catch(RemoteException e) {
        // Shouldn't happen.
    }
}

ECode ContentResolver::ValidateSyncExtrasBundle(
    /* [in] */ IBundle* extras)
{
    VALIDATE_NOT_NULL(extras)
    // try {
    AutoPtr<IObjectContainer> ObjContainer;
    FAIL_RETURN(extras->KeySet((IObjectContainer**)&ObjContainer))
    if (NULL != ObjContainer) {
        AutoPtr<IObjectEnumerator> ObjEnumerator;
        ObjContainer->GetObjectEnumerator((IObjectEnumerator**)&ObjEnumerator);
        Boolean hasNext = FALSE;
        while ((ObjEnumerator->MoveNext(&hasNext), hasNext)) {
            AutoPtr<ICharSequence> keyObj;
            ObjEnumerator->Current((IInterface**)&keyObj);
            String key;
            keyObj->ToString(&key);
            AutoPtr<IInterface> valueObj;
            FAIL_RETURN(extras->Get(key, (IInterface**)&valueObj))
            if (NULL == valueObj) continue;
            if (IInteger64::Probe(valueObj) != NULL) continue;
            if (IInteger32::Probe(valueObj) != NULL) continue;
            if (IBoolean::Probe(valueObj) != NULL) continue;
            if (IFloat::Probe(valueObj) != NULL) continue;
            if (IDouble::Probe(valueObj) != NULL) continue;
            if (ICharSequence::Probe(valueObj) != NULL) continue;
            if (IAccount::Probe(valueObj) != NULL) continue;
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
    }
    return NOERROR;
    // } catch (IllegalArgumentException e) {
    //     throw e;
    // } catch (RuntimeException exc) {
    //     throw new IllegalArgumentException("error unparceling Bundle", exc);
    // }
}

ECode ContentResolver::CancelSync(
    /* [in] */ IUri* uri)
{
    String authority;
    if (NULL != uri) {
        uri->GetAuthority(&authority);
    }
    return CancelSync(NULL /* all accounts */, authority);
}

ECode ContentResolver::CancelSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority)
{
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService))
    return contentService->CancelSync(account, authority, NULL);
}

ECode ContentResolver::CancelSyncAsUser(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ Int32 userId)
{
    try {
        getContentService().cancelSyncAsUser(account, authority, null, userId);
    } catch (RemoteException e) {
    }
}

ECode ContentResolver::GetSyncAdapterTypes(
    /* [out, callee] */ ArrayOf<ISyncAdapterType*>** types)
{
    VALIDATE_NOT_NULL(types)
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService))
    return contentService->GetSyncAdapterTypes(types);
}

ECode ContentResolver::GetSyncAdapterTypesAsUser(
    /* [in] */ Int32 userId,
    /* [out, callee] */ ArrayOf<ISyncAdapterType*>** types)
{
    VALIDATE_NOT_NULL(types)
    try {
        return getContentService().getSyncAdapterTypesAsUser(userId);
    } catch (RemoteException e) {
        throw new RuntimeException("the ContentService should always be reachable", e);
    }
    return NOERROR;
}

ECode ContentResolver::GetSyncAutomatically(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ Boolean* isSynced)
{
    VALIDATE_NOT_NULL(isSynced)
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService))
    return contentService->GetSyncAutomatically(account, authority, isSynced);
}

ECode ContentResolver::GetSyncAutomaticallyAsUser(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ Int32 userId,
    /* [out] */ Boolean* isSynced)
{
    try {
        return getContentService().getSyncAutomaticallyAsUser(account, authority, userId);
    } catch (RemoteException e) {
        throw new RuntimeException("the ContentService should always be reachable", e);
    }
    return NOERROR;
}

ECode ContentResolver::SetSyncAutomatically(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ Boolean sync)
{
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService))
    return contentService->SetSyncAutomatically(account, authority, sync);
}

ECode ContentResolver::SetSyncAutomaticallyAsUser(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ Int32 userId,
    /* [in] */ Boolean sync)
{
    try {
        getContentService().setSyncAutomaticallyAsUser(account, authority, sync, userId);
    } catch (RemoteException e) {
        // exception ignored; if this is thrown then it means the runtime is in the midst of
        // being restarted
    }
    return NOERROR;
}

ECode ContentResolver::AddPeriodicSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ IBundle* extras,
    /* [in] */ Int64 pollFrequency)
{
    FAIL_RETURN(ValidateSyncExtrasBundle(extras))
    if (NULL == account) {
        ///throw new IllegalArgumentException("account must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (authority.IsNull()) {
        //throw new IllegalArgumentException("authority must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    Boolean value = FALSE;
    if ((extras->GetBoolean(IContentResolver::SYNC_EXTRAS_MANUAL, FALSE, &value), value)
            || (extras->GetBoolean(IContentResolver::SYNC_EXTRAS_DO_NOT_RETRY, FALSE, &value), value)
            || (extras->GetBoolean(IContentResolver::SYNC_EXTRAS_IGNORE_BACKOFF, FALSE, &value), value)
            || (extras->GetBoolean(IContentResolver::SYNC_EXTRAS_IGNORE_SETTINGS, FALSE, &value), value)
            || (extras->GetBoolean(IContentResolver::SYNC_EXTRAS_INITIALIZE, FALSE, &value), value)
            || (extras->GetBoolean(IContentResolver::SYNC_EXTRAS_FORCE, FALSE, &value), value)
            || (extras->GetBoolean(IContentResolver::SYNC_EXTRAS_EXPEDITED, FALSE, &value), value)) {
        //throw new IllegalArgumentException("illegal extras were set");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService))
    return contentService->AddPeriodicSync(account, authority, extras, pollFrequency);
}

ECode ContentResolver::InvalidPeriodicExtras(
    /* [in] */ IBundle* extras,
    /* [out] */ Boolean* result)
{
    if (extras.getBoolean(ContentResolver.SYNC_EXTRAS_MANUAL, false)
            || extras.getBoolean(ContentResolver.SYNC_EXTRAS_DO_NOT_RETRY, false)
            || extras.getBoolean(ContentResolver.SYNC_EXTRAS_IGNORE_BACKOFF, false)
            || extras.getBoolean(ContentResolver.SYNC_EXTRAS_IGNORE_SETTINGS, false)
            || extras.getBoolean(ContentResolver.SYNC_EXTRAS_INITIALIZE, false)
            || extras.getBoolean(ContentResolver.SYNC_EXTRAS_FORCE, false)
            || extras.getBoolean(ContentResolver.SYNC_EXTRAS_EXPEDITED, false)) {
        return true;
    }
    return false;
}

ECode ContentResolver::RemovePeriodicSync(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ IBundle* extras)
{
    FAIL_RETURN(ValidateSyncExtrasBundle(extras))
    if (NULL == account) {
        ///throw new IllegalArgumentException("account must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (authority.IsNull()) {
        //throw new IllegalArgumentException("authority must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService))
    return contentService->RemovePeriodicSync(account, authority, extras);
}

ECode ContentResolver::CancelSync(
    /* [in] */ ISyncRequest* request)
{
    if (request == null) {
        throw new IllegalArgumentException("request cannot be null");
    }
    try {
        getContentService().cancelRequest(request);
    } catch (RemoteException e) {
        // exception ignored; if this is thrown then it means the runtime is in the midst of
        // being restarted
    }
    return NOERROR;
}

ECode ContentResolver::GetPeriodicSyncs(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ IList** periodicSyncs)
{
    if (NULL == account) {
        // throw new IllegalArgumentException("account must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (authority.IsNull()) {
        // throw new IllegalArgumentException("authority must not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService))
    return contentService->GetPeriodicSyncs(account, authority, periodicSyncs);
}

ECode ContentResolver::GetIsSyncable(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ Int32* isSyncable)
{
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService))
    return contentService->GetIsSyncable(account, authority, isSyncable);
}

ECode ContentResolver::GetIsSyncableAsUser(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ Int32 userId，
    /* [out] */ Int32* isSyncable)
{
    try {
        return getContentService().getIsSyncableAsUser(account, authority, userId);
    } catch (RemoteException e) {
        throw new RuntimeException("the ContentService should always be reachable", e);
    }
}

ECode ContentResolver::SetIsSyncable(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ Int32 syncable)
{
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService))
    return contentService->SetIsSyncable(account, authority, syncable);
}

ECode ContentResolver::GetMasterSyncAutomatically(
    /* [out] */ Boolean* result)
{
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService))
    return contentService->GetMasterSyncAutomatically(result);
}

ECode ContentResolver::GetMasterSyncAutomaticallyAsUser(
    /* [in] */ Int32 userId，
    /* [out] */ Boolean* result)
{
    try {
        return getContentService().getMasterSyncAutomaticallyAsUser(userId);
    } catch (RemoteException e) {
        throw new RuntimeException("the ContentService should always be reachable", e);
    }
}

ECode ContentResolver::SetMasterSyncAutomatically(
    /* [in] */ Boolean sync)
{
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService))
    return contentService->SetMasterSyncAutomatically(sync);
}

ECode ContentResolver::SetMasterSyncAutomaticallyAsUser(
    /* [in] */ Int32 userId，
    /* [in] */ Boolean sync)
{
    try {
        getContentService().setMasterSyncAutomaticallyAsUser(sync, userId);
    } catch (RemoteException e) {
        // exception ignored; if this is thrown then it means the runtime is in the midst of
        // being restarted
    }
}

ECode ContentResolver::IsSyncActive(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ Boolean* isSyncActive)
{
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService))
    return contentService->IsSyncActive(account, authority, isSyncActive);
}

ECode ContentResolver::GetCurrentSync(
    /* [out] */ ISyncInfo** syncInfo)
{
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService))
    AutoPtr<IObjectContainer> syncInfoList;
    FAIL_RETURN(contentService->GetCurrentSyncs((IObjectContainer**)&syncInfoList))
    Int32 count = 0;
    syncInfoList->GetObjectCount(&count);
    if (0 == count) {
        *syncInfo = NULL;
        return NOERROR;
    }
    AutoPtr<IObjectEnumerator> ObjEnumerator;
    syncInfoList->GetObjectEnumerator((IObjectEnumerator**)&ObjEnumerator);
    Boolean hasNext = FALSE;
    while ((ObjEnumerator->MoveNext(&hasNext), hasNext)) {
        AutoPtr<ISyncInfo> valueObj;
        ObjEnumerator->Current((IInterface**)&valueObj);
        *syncInfo = valueObj;
        REFCOUNT_ADD(*syncInfo);
        break;
    }
    return NOERROR;
}

ECode ContentResolver::GetCurrentSyncs(
    /* [out] */ IList** syncInfoList)
{
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService))
    return contentService->GetCurrentSyncs(syncInfoList);
}

ECode ContentResolver::GetCurrentSyncsAsUser(
    /* [in] */ Int32 userId，
    /* [out] */ IList** syncInfoList)
{
    try {
        return getContentService().getCurrentSyncsAsUser(userId);
    } catch (RemoteException e) {
        throw new RuntimeException("the ContentService should always be reachable", e);
    }
}

ECode ContentResolver::GetSyncStatus(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ ISyncStatusInfo** syncStatusInfo)
{
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService))
    return contentService->GetSyncStatus(account, authority, syncStatusInfo);
}

ECode ContentResolver::GetSyncStatusAsUser(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ Int32 userId，
    /* [out] */ ISyncStatusInfo** syncStatusInfo)
{
    try {
        return getContentService().getSyncStatusAsUser(account, authority, null, userId);
    } catch (RemoteException e) {
        throw new RuntimeException("the ContentService should always be reachable", e);
    }
}

ECode ContentResolver::IsSyncPending(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [out] */ Boolean* isSyncPending)
{
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService))
    return contentService->IsSyncPending(account, authority, isSyncPending);
}

ECode ContentResolver::IsSyncPendingAsUser(
    /* [in] */ IAccount* account,
    /* [in] */ const String& authority,
    /* [in] */ Int32 userId,
    /* [out] */ Boolean* isSyncPending)
{
    try {
        return getContentService().isSyncPendingAsUser(account, authority, null, userId);
    } catch (RemoteException e) {
        throw new RuntimeException("the ContentService should always be reachable", e);
    }
}

ECode ContentResolver::AddStatusChangeListener(
    /* [in] */ Int32 mask,
    /* [in] */ ISyncStatusObserver* observerCallback,
    /* [out] */ IInterface** item)
{
    if (NULL == observerCallback) {
        //throw new IllegalArgumentException("you passed in a null callback");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

//    try {
//        ISyncStatusObserver.Stub observer = new ISyncStatusObserver.Stub() {
//            public void onStatusChanged(int which) throws RemoteException {
//                callback.onStatusChanged(which);
//            }
//        };
//        getContentService().addStatusChangeListener(mask, observer);
//        return observer;
//    } catch (RemoteException e) {
//        throw new RuntimeException("the ContentService should always be reachable", e);
//    }
    assert(0);
    return E_NOT_IMPLEMENTED;
}

ECode ContentResolver::RemoveStatusChangeListener(
    /* [in] */ IInterface* handle)
{
    if (NULL == handle) {
        //throw new IllegalArgumentException("you passed in a null handle");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    AutoPtr<IContentService> contentService;
    FAIL_RETURN(GetContentService((IContentService**)&contentService))
    return contentService->RemoveStatusChangeListener(IISyncStatusObserver::Probe(handle));
}

Int32 ContentResolver::SamplePercentForDuration(
    /* [in] */ Int64 durationMillis)
{
    if (durationMillis >= SLOW_THRESHOLD_MILLIS) {
        return 100;
    }
    return (Int32) (100 * durationMillis / SLOW_THRESHOLD_MILLIS) + 1;
}

ECode ContentResolver::MaybeLogQueryToEventLog(
    /* [in] */ Int64 durationMillis,
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ const String& selection,
    /* [in] */ const String& sortOrder)
{
    Int32 samplePercent = SamplePercentForDuration(durationMillis);
    if (samplePercent < 100) {
        {
            AutoLock lock(mRandomLock);

            Int32 rando;
            mRandom->NextInt32(100, &rando);
            if (rando >= samplePercent) {
                return NOERROR;
            }
        }
    }

    StringBuilder projectionBuffer(100);
    if (NULL != projection) {
        for (Int32 i = 0; i < projection->GetLength(); ++i) {
            // Note: not using a comma delimiter here, as the
            // multiple arguments to EventLog.writeEvent later
            // stringify with a comma delimiter, which would make
            // parsing uglier later.
            if (i != 0) projectionBuffer += '/';
            projectionBuffer += (*projection)[i];
        }
    }

    // ActivityThread.currentPackageName() only returns non-null if the
    // current thread is an application main thread.  This parameter tells
    // us whether an event loop is blocked, and if so, which app it is.
    String blockingPackage = AppGlobals::GetInitialPackage();

    // TODO:
//    EventLog.writeEvent(
//        EventLogTags.CONTENT_QUERY_SAMPLE,
//        uri.toString(),
//        projectionBuffer.toString(),
//        selection != null ? selection : "",
//        sortOrder != null ? sortOrder : "",
//        durationMillis,
//        blockingPackage != null ? blockingPackage : "",
//        samplePercent);
    return NOERROR;
}

ECode ContentResolver::MaybeLogUpdateToEventLog(
    /* [in] */ Int64 durationMillis,
    /* [in] */ IUri* uri,
    /* [in] */ const String& operation,
    /* [in] */ const String& selection)
{
    Int32 samplePercent = SamplePercentForDuration(durationMillis);
    if (samplePercent < 100) {
        {
            AutoLock lock(mRandomLock);

            Int32 rando;
            mRandom->NextInt32(100, &rando);
            if (rando >= samplePercent) {
                return NOERROR;
            }
        }
    }
    String blockingPackage = AppGlobals::GetInitialPackage();

    //TODO:
//    EventLog.writeEvent(
//        EventLogTags.CONTENT_UPDATE_SAMPLE,
//        uri.toString(),
//        operation,
//        selection != null ? selection : "",
//        durationMillis,
//        blockingPackage != null ? blockingPackage : "",
//        samplePercent);
    return NOERROR;
}

ECode ContentResolver::GetContentService(
    /* [out] */ IContentService** contentService)
{
    VALIDATE_NOT_NULL(contentService)

    if (NULL != sContentService) {
        *contentService = sContentService;
        REFCOUNT_ADD(*contentService);
        return NOERROR;
    }

    sContentService = (IContentService*)ServiceManager::GetService(IContentResolver::CONTENT_SERVICE_NAME).Get();
    // if (false) Log.v("ContentService", "default service binder = " + b);
    *contentService = sContentService;
    // if (false) Log.v("ContentService", "default service = " + sContentService);
    REFCOUNT_ADD(*contentService)
    return NOERROR;
}

String ContentResolver::GetPackageName()
{
    return mPackageName;
}

Int32 ContentResolver::ResolveUserId(
    /* [in] */ IUri* uri)
{
    return ContentProvider.getUserIdFromUri(uri, mContext.getUserId());
}

} // namespace Content
} // namespace Droid
} // namespace Elastos
