
#ifndef __ELASTOS_DROID_APP_CCONTEXTIMPL_H__
#define __ELASTOS_DROID_APP_CCONTEXTIMPL_H__

#include "_Elastos_Droid_App_CContextImpl.h"
#include "app/LoadedPkg.h"
#include "app/CActivityThread.h"
#include "app/CDownloadManager.h"
#include "app/SharedPreferencesImpl.h"
#include "content/ContentResolver.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Core::IClassLoader;
using Elastos::Core::ICharSequence;
using Elastos::IO::IFile;
using Elastos::IO::IFileInputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::IFileDescriptor;
using Elastos::IO::IInputStream;
using Elastos::Droid::Os::ILooper;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Content::ContentResolver;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::IIntentSender;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::IComponentName;
using Elastos::Droid::Content::IComponentCallbacks;
using Elastos::Droid::Content::ISharedPreferences;
using Elastos::Droid::Content::IBroadcastReceiver;
using Elastos::Droid::Content::IServiceConnection;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::IResourcesTheme;
using Elastos::Droid::Content::Res::IAssetManager;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Pm::IActivityInfo;
using Elastos::Droid::View::IDisplay;
using Elastos::Droid::View::ICompatibilityInfoHolder;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Database::ICursor;
using Elastos::Droid::Database::IDatabaseErrorHandler;
using Elastos::Droid::Database::Sqlite::ISQLiteDatabase;
using Elastos::Droid::Database::Sqlite::ISQLiteDatabaseCursorFactory;


namespace Elastos {
namespace Droid {
namespace App {

CarClass(CContextImpl)
{
private:
    class ApplicationContentResolver : public ContentResolver
    {
    public:
        ApplicationContentResolver(
            /* [in] */ IContext* context,
            /* [in] */ CActivityThread* mainThread,
            /* [in] */ IUserHandle* user);

        CARAPI AcquireProvider(
            /* [in] */ IContext* context,
            /* [in] */ const String& auth,
            /* [out] */ IIContentProvider** provider);

        CARAPI AcquireExistingProvider(
            /* [in] */ IContext* context,
            /* [in] */ const String& auth,
            /* [out] */ IIContentProvider** provider);

        CARAPI ReleaseProvider(
            /* [in] */ IIContentProvider* provider,
            /* [out] */ Boolean* result);

        CARAPI AcquireUnstableProvider(
            /* [in] */ IContext* c,
            /* [in] */ const String& auth,
            /* [out] */ IIContentProvider** provider);

        CARAPI ReleaseUnstableProvider(
            /* [in] */ IIContentProvider* icp,
            /* [out] */ Boolean* result);

        CARAPI UnstableProviderDied(
            /* [in] */ IIContentProvider* icp);

    private:
        AutoPtr<CActivityThread> mMainThread;
        AutoPtr<IUserHandle> mUser;
    };

public:
    CContextImpl();

    ~CContextImpl();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContextImpl* context);

    static CARAPI_(AutoPtr<CContextImpl>) GetImpl(
        /* [in] */ IContext* context);

    CARAPI GetAssets(
        /* [out] */ IAssetManager** assetManager);

    CARAPI GetResources(
        /* [out] */ IResources** resources);

    CARAPI GetPackageManager(
        /* [out] */ IPackageManager** packageManager);

    CARAPI GetContentResolver(
        /* [out] */ IContentResolver** resolver);

    CARAPI GetMainLooper(
        /* [out] */ ILooper** looper);

    CARAPI GetApplicationContext(
        /* [out] */ IContext** ctx);

    CARAPI RegisterComponentCallbacks(
        /* [in] */ IComponentCallbacks* componentCallback);

    CARAPI UnregisterComponentCallbacks(
        /* [in] */ IComponentCallbacks* componentCallback);

    CARAPI GetText(
        /* [in] */ Int32 resId,
        /* [out] */ ICharSequence** text);

    CARAPI GetString(
        /* [in] */ Int32 resId,
        /* [out] */ String* str);

    CARAPI GetString(
        /* [in] */ Int32 resId,
        /* [in] */ ArrayOf<IInterface*>* formatArgs,
        /* [out] */ String* str);

    CARAPI SetTheme(
        /* [in] */ Int32 resid);

    CARAPI GetThemeResId(
        /* [out] */ Int32* resId);

    CARAPI GetTheme(
        /* [out] */ IResourcesTheme** theme);

    CARAPI ObtainStyledAttributes(
        /* [in] */ ArrayOf<Int32>* attrs,
        /* [out] */ ITypedArray** styles);

    CARAPI ObtainStyledAttributes(
        /* [in] */ Int32 resid,
        /* [in] */ ArrayOf<Int32>* attrs,
        /* [out] */ ITypedArray** styles);

    CARAPI ObtainStyledAttributes(
        /* [in] */ IAttributeSet* set,
        /* [in] */ ArrayOf<Int32>* attrs,
        /* [out] */ ITypedArray** styles);

    CARAPI ObtainStyledAttributes(
        /* [in] */ IAttributeSet* set,
        /* [in] */ ArrayOf<Int32>* attrs,
        /* [in] */ Int32 defStyleAttr,
        /* [in] */ Int32 defStyleRes,
        /* [out] */ ITypedArray** styles);

    CARAPI GetClassLoader(
        /* [out] */ IClassLoader** loader);

    CARAPI GetPackageName(
        /* [out] */ String* packageName);

    CARAPI GetApplicationInfo(
        /* [out] */ IApplicationInfo** info);

    CARAPI GetPackageResourcePath(
        /* [out] */ String* path);

    CARAPI GetPackageCodePath(
        /* [out] */ String* codePath);

    CARAPI GetSharedPrefsFile(
        /* [in] */ const String& name,
        /* [out] */ IFile** file);

    CARAPI GetSharedPreferences(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [out] */ ISharedPreferences** prefs);

    CARAPI OpenFileInput(
        /* [in] */ const String& name,
        /* [out] */ IFileInputStream** fileInputStream);

    CARAPI OpenFileOutput(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [out] */IFileOutputStream** fileOutputStream);

    CARAPI DeleteFile(
        /* [in] */ const String& name,
        /* [out] */ Boolean* succeeded);

    CARAPI GetFileStreamPath(
        /* [in] */ const String& name,
        /* [out] */ IFile** file);

    CARAPI GetFilesDir(
        /* [out] */ IFile** filesDir);

    CARAPI GetExternalFilesDir(
        /* [in] */ const String& type,
        /* [out] */ IFile** filesDir);

    CARAPI GetObbDir(
        /* [out] */ IFile** obbDir);

    CARAPI GetCacheDir(
        /* [out] */ IFile** cacheDir);

    CARAPI GetExternalCacheDir(
        /* [out] */ IFile** externalDir);

    CARAPI GetFileList(
        /* [out, callee] */ ArrayOf<String>** fileList);

    CARAPI GetDir(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [out] */ IFile** dir);

    /** {@hide} */
    CARAPI GetUserId(
        /* [out] */ Int32* id);

    CARAPI OpenOrCreateDatabase(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [in] */ ISQLiteDatabaseCursorFactory* factory,
        /* [out] */ ISQLiteDatabase** sqliteDB);

    CARAPI OpenOrCreateDatabase(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [in] */ ISQLiteDatabaseCursorFactory* factory,
        /* [in] */ IDatabaseErrorHandler* errorHandler,
        /* [out] */ ISQLiteDatabase** sqliteDB);

    CARAPI DeleteDatabase(
        /* [in] */ const String& name,
        /* [out] */ Boolean* succeeded);

    CARAPI GetDatabasePath(
        /* [in] */ const String& name,
        /* [out] */ IFile** path);

    CARAPI GetDatabaseList(
        /* [out, callee] */ ArrayOf<String>** databaseList);

    CARAPI GetWallpaper(
        /* [out] */ IDrawable** drawable);

    CARAPI PeekWallpaper(
        /* [out] */ IDrawable** drawable);

    CARAPI GetWallpaperDesiredMinimumWidth(
        /* [out] */ Int32* minWidth);

    CARAPI GetWallpaperDesiredMinimumHeight(
        /* [out] */ Int32* minHeight);

    CARAPI SetWallpaper(
        /* [in] */ IBitmap* bitmap);

    CARAPI SetWallpaper(
        /* [in] */ IInputStream* data);

    CARAPI ClearWallpaper();

    CARAPI StartActivity(
        /* [in] */ IIntent* intent);

    CARAPI StartActivity(
        /* [in] */ IIntent* intent,
        /* [in] */ IBundle* options);

    CARAPI StartActivityAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user);

    CARAPI StartActivityAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IBundle* options,
        /* [in] */ IUserHandle* user);

    CARAPI StartActivities(
        /* [in] */ ArrayOf<IIntent*>* intents);

    CARAPI StartActivities(
        /* [in] */ ArrayOf<IIntent*>* intents,
        /* [in] */ IBundle* options);

    CARAPI StartActivitiesAsUser(
        /* [in] */ ArrayOf<IIntent*>* intents,
        /* [in] */ IBundle* options,
        /* [in] */ IUserHandle* userHandle);

    CARAPI StartIntentSender(
        /* [in] */ IIntentSender* intent,
        /* [in] */ IIntent* fillInIntent,
        /* [in] */ Int32 flagsMask,
        /* [in] */ Int32 flagsValues,
        /* [in] */ Int32 extraFlags);

    CARAPI StartIntentSender(
        /* [in] */ IIntentSender* intent,
        /* [in] */ IIntent* fillInIntent,
        /* [in] */ Int32 flagsMask,
        /* [in] */ Int32 flagsValues,
        /* [in] */ Int32 extraFlags,
        /* [in] */ IBundle* options);

    CARAPI SendBroadcast(
        /* [in] */ IIntent* intent);

    CARAPI SendBroadcast(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& receiverPermission);

    CARAPI SendOrderedBroadcast(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& receiverPermission);

    CARAPI SendOrderedBroadcast(
        /* [in] */ IIntent* intent,
        /* [in] */ const String& receiverPermission,
        /* [in] */ IBroadcastReceiver* resultReceiver,
        /* [in] */ IHandler* scheduler,
        /* [in] */ Int32 initialCode,
        /* [in] */ const String& initialData,
        /* [in] */ IBundle* initialExtras);

    CARAPI SendBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user);

    CARAPI SendBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user,
        /* [in] */ const String& receiverPermission);

    CARAPI SendOrderedBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user,
        /* [in] */ const String& receiverPermission,
        /* [in] */ IBroadcastReceiver* resultReceiver,
        /* [in] */ IHandler* scheduler,
        /* [in] */ Int32 initialCode,
        /* [in] */ const String& initialData,
        /* [in] */ IBundle* initialExtras);

    CARAPI SendStickyBroadcast(
        /* [in] */ IIntent* intent);

    CARAPI SendStickyOrderedBroadcast(
        /* [in] */ IIntent* intent,
        /* [in] */ IBroadcastReceiver* resultReceiver,
        /* [in] */ IHandler* scheduler,
        /* [in] */ Int32 initialCode,
        /* [in] */ const String& initialData,
        /* [in] */ IBundle* initialExtras);

    CARAPI RemoveStickyBroadcast(
        /* [in] */ IIntent* intent);

    CARAPI SendStickyBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user);

    CARAPI SendStickyOrderedBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user,
        /* [in] */ IBroadcastReceiver* resultReceiver,
        /* [in] */ IHandler* scheduler,
        /* [in] */ Int32 initialCode,
        /* [in] */ const String& initialData,
        /* [in] */ IBundle* initialExtras);

    CARAPI RemoveStickyBroadcastAsUser(
        /* [in] */ IIntent* intent,
        /* [in] */ IUserHandle* user);

    CARAPI RegisterReceiver(
        /* [in] */ IBroadcastReceiver* receiver,
        /* [in] */ IIntentFilter* filter,
        /* [out] */ IIntent** intent);

    CARAPI RegisterReceiver(
        /* [in] */ IBroadcastReceiver* receiver,
        /* [in] */ IIntentFilter* filter,
        /* [in] */ const String& broadcastPermission,
        /* [in] */ IHandler* scheduler,
        /* [out] */ IIntent** intent);

    CARAPI RegisterReceiverAsUser(
        /* [in] */ IBroadcastReceiver* receiver,
        /* [in] */ IUserHandle* user,
        /* [in] */ IIntentFilter* filter,
        /* [in] */ const String& broadcastPermission,
        /* [in] */ IHandler* scheduler,
        /* [out] */ IIntent** stickyIntent);

    CARAPI UnregisterReceiver(
        /* [in] */ IBroadcastReceiver* receiver);

    CARAPI StartService(
        /* [in] */ IIntent* service,
        /* [out] */ IComponentName** name);

    CARAPI StopService(
        /* [in] */ IIntent* service,
        /* [out] */ Boolean* succeeded);

    CARAPI StartServiceAsUser(
        /* [in] */ IIntent* service,
        /* [in] */ IUserHandle* user,
        /* [out] */ IComponentName** name);

    CARAPI StopServiceAsUser(
        /* [in] */ IIntent* service,
        /* [in] */ IUserHandle* user,
        /* [out] */ Boolean* succeeded);

    CARAPI BindService(
        /* [in] */ IIntent* service,
        /* [in] */ IServiceConnection* conn,
        /* [in] */ Int32 flags,
        /* [out] */ Boolean* succeeded);

    CARAPI BindService(
        /* [in] */ IIntent* service,
        /* [in] */ IServiceConnection* conn,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 userHandle,
        /* [out] */ Boolean* succeeded);

    CARAPI UnbindService(
        /* [in] */ IServiceConnection* conn);

    CARAPI StartInstrumentation(
        /* [in] */ IComponentName* className,
        /* [in] */ const String& profileFile,
        /* [in] */ IBundle* arguments,
        /* [out] */ Boolean* succeeded);

    CARAPI GetSystemService(
        /* [in] */ const String& name,
        /* [out] */ IInterface** object);

//    /* package */
//    static CARAPI_(AutoPtr<IDropBoxManager>) CreateDropBoxManager();

    CARAPI CheckPermission(
        /* [in] */ const String& permission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [out] */ Int32 * result);

    CARAPI CheckCallingPermission(
        /* [in] */ const String& permission,
        /* [out] */ Int32* value);

    CARAPI CheckCallingOrSelfPermission(
        /* [in] */ const String& permission,
        /* [out] */ Int32* perm);

    CARAPI EnforcePermission(
        /* [in] */ const String& permission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ const String& message);

    CARAPI EnforceCallingPermission(
        /* [in] */ const String& permission,
        /* [in] */ const String& message);

    CARAPI EnforceCallingOrSelfPermission(
        /* [in] */ const String& permission,
        /* [in] */ const String& message);

    CARAPI GrantUriPermission(
        /* [in] */ const String& toPackage,
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags);

    CARAPI RevokeUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags);

    CARAPI CheckUriPermission(
        /* [in] */ IUri * uri,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32 * result);

    CARAPI CheckCallingUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32* result);

    CARAPI CheckCallingOrSelfUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32* result);

    CARAPI CheckUriPermission(
        /* [in] */ IUri * uri,
        /* [in] */ const String& readPermission,
        /* [in] */ const String& writePermission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [out] */ Int32 * result);

    CARAPI EnforceUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [in] */ const String& message);

    CARAPI EnforceCallingUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [in] */ const String& message);

    CARAPI EnforceCallingOrSelfUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ Int32 modeFlags,
        /* [in] */ const String& message);

    CARAPI EnforceUriPermission(
        /* [in] */ IUri* uri,
        /* [in] */ const String& readPermission,
        /* [in] */ const String& writePermission,
        /* [in] */ Int32 pid,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 modeFlags,
        /* [in] */ const String& message);

    CARAPI CreatePackageContext(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 flags,
        /* [out] */ IContext** ctx);

    CARAPI CreatePackageContextAsUser(
        /* [in] */ const String& packageName,
        /* [in] */ Int32 flags,
        /* [in] */ IUserHandle* user,
        /* [out] */ IContext** ctx);

    CARAPI CreateConfigurationContext(
        /* [in] */ IConfiguration* overrideConfiguration,
        /* [out] */ IContext** ctx);

    CARAPI CreateDisplayContext(
        /* [in] */ IDisplay* display,
        /* [out] */ IContext** ctx);

    CARAPI IsRestricted(
        /* [out] */ Boolean* isRestricted);

    CARAPI GetCompatibilityInfo(
        /* [in] */ Int32 displayId,
        /* [out] */ ICompatibilityInfoHolder** infoHolder);

    static CARAPI_(AutoPtr<CContextImpl>) CreateSystemContext(
        /* [in] */ IActivityThread* mainThread);

    CARAPI Init(
        /* [in] */ LoadedPkg* packageInfo,
        /* [in] */ IBinder* activityToken,
        /* [in] */ CActivityThread* mainThread);

    CARAPI Init(
        /* [in] */ LoadedPkg* packageInfo,
        /* [in] */ IBinder* activityToken,
        /* [in] */ CActivityThread* mainThread,
        /* [in] */ IResources* container,
        /* [in] */ const String& basePackageName,
        /* [in] */ IUserHandle* user);

    CARAPI Init(
        /* [in] */ IResources* resources,
        /* [in] */ CActivityThread* mainThread,
        /* [in] */ IUserHandle* user);

    CARAPI ScheduleFinalCleanup(
        /* [in] */ const String& who,
        /* [in] */ const String& what);

    CARAPI PerformFinalCleanup(
        /* [in] */ const String& who,
        /* [in] */ const String& what);

    CARAPI GetReceiverRestrictedContext(
        /* [out] */ IContext** ctx);

    CARAPI_(void) SetOuterContext(
        /* [in] */ IContext* context);

    CARAPI_(AutoPtr<IContext>) GetOuterContext();

    CARAPI_(AutoPtr<IBinder>) GetActivityToken();

    static CARAPI_(void) SetFilePermissionsFromMode(
        /* [in] */ const String& name,
        /* [in] */ Int32 mode,
        /* [in] */ Int32 extraPermissions);

private:
    CARAPI_(AutoPtr<IFile>) GetPreferencesDir();

    CARAPI_(AutoPtr<IFile>) GetDatabasesDir();

    CARAPI RegisterReceiverInternal(
        /* [in] */ IBroadcastReceiver* receiver,
        /* [in] */ Int32 userId,
        /* [in] */ IIntentFilter* filter,
        /* [in] */ const String& broadcastPermission,
        /* [in] */ IHandler* scheduler,
        /* [in] */ IContext* context,
        /* [out] */ IIntent** intent);

    CARAPI_(AutoPtr<IWallpaperManager>) GetWallpaperManager();

    CARAPI Enforce(
        /* [in] */ const String& permission,
        /* [in] */ Int32 resultOfCheck,
        /* [in] */ Boolean selfToo,
        /* [in] */ Int32 uid,
        /* [in] */ const String& message);

    CARAPI UriModeFlagToString(
        /* [in] */ Int32 uriModeFlags,
        /* [out] */ String* mode);

    CARAPI EnforceForUri(
        /* [in] */ Int32 modeFlags,
        /* [in] */ Int32 resultOfCheck,
        /* [in] */ Boolean selfToo,
        /* [in] */ Int32 uid,
        /* [in] */ IUri* uri,
        /* [in] */ const String& message);

    CARAPI_(void) WarnIfCallingFromSystemProcess();

    CARAPI_(Int32) GetDisplayId();

    CARAPI GetDataDirFile(
        /* [out] */ IFile** file);

    CARAPI_(AutoPtr<IFile>) ValidateFilePath(
        /* [in] */ const String& name,
        /* [in] */ Boolean createDirectory);

    CARAPI MakeFilename(
        /* [in] */ IFile* base,
        /* [in] */ const String& name,
        /* [out] */ IFile** file);

public:
    /*package*/ AutoPtr<LoadedPkg> mPackageInfo;
    /*package*/ AutoPtr<CActivityThread> mMainThread;

private:
    const static String TAG;
    const static Boolean DEBUG;

    static HashMap<String, AutoPtr<SharedPreferencesImpl> > sSharedPrefs;
    static Object sSharedPrefsLock;

    String mBasePackageName;
    AutoPtr<IResources> mResources;
    // mOuterContext maybe is this, IActivity, IService or IApplication
    // IActivity, IService, IApplication has this's reference
    //
    AutoPtr<IWeakReference> mOuterContext; // IContext*

    AutoPtr<IBinder> mActivityToken;
    AutoPtr<ApplicationContentResolver> mContentResolver;
    Int32 mThemeResource;
    AutoPtr<IResourcesTheme> mTheme;
    AutoPtr<IPackageManager> mPackageManager;
    AutoPtr<IDisplay> mDisplay; // may be null if default display
    AutoPtr<IContext> mReceiverRestrictedContext;
    Boolean mRestricted;
    AutoPtr<IUserHandle> mUser;

    AutoPtr<IInterface> mSync;

    AutoPtr<IFile> mDatabasesDir;
    AutoPtr<IFile> mPreferencesDir;
    AutoPtr<IFile> mFilesDir;
    AutoPtr<IFile> mCacheDir;
    AutoPtr<IFile> mObbDir;
    AutoPtr<IFile> mExternalFilesDir;
    AutoPtr<IFile> mExternalCacheDir;

    static AutoPtr< ArrayOf<String> > EMPTY_FILE_LIST;

    HashMap< String, AutoPtr<IInterface> > mServiceCache;
    Object mCacheLock;

    Object mSyncLock;

    static AutoPtr<IWallpaperManager> sWallpaperManager;
};


} // namespace App
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_APP_CCONTEXTIMPL_H__
