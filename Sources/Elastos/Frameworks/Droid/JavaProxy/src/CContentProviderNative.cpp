
#include "CContentProviderNative.h"
#include "CCursorNative.h"
#include "CICancellationSignalNative.h"
#include "ext/frameworkext.h"
#include "Util.h"
#include <elastos/utility/logging/Logger.h>
#include <unistd.h>

using Elastos::Utility::Logging::Logger;

using Elastos::Droid::JavaProxy::CCursorNative;
using Elastos::Droid::JavaProxy::CICancellationSignalNative;

namespace Elastos {
namespace Droid {
namespace JavaProxy {

const String CContentProviderNative::TAG("CContentProviderNative");

CContentProviderNative::~CContentProviderNative()
{
    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);
    env->DeleteGlobalRef(mJInstance);
}

ECode CContentProviderNative::constructor(
    /* [in] */ Handle32 jVM,
    /* [in] */ Handle32 jInstance)
{
    mJVM = (JavaVM*)jVM;
    mJInstance = (jobject)jInstance;
    return NOERROR;
}

ECode CContentProviderNative::Call(
    /* [in] */ const String& method,
    /* [in] */ const String& arg,
    /* [in] */ IBundle* extras,
    /* [out] */ IBundle** bundle)
{
    // LOGGERD(TAG, "+ CContentProviderNative::Call() method:%s, arg:%s", method.string(), arg.string());

    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);

    jobject jmethod = Util::ToJavaString(env, method);
    jobject jarg = Util::ToJavaString(env, arg);
    jobject jextras = NULL;
    if (extras != NULL) {
        jextras = Util::ToJavaBundle(env, extras);
    }

    jclass c = env->FindClass("android/content/IContentProvider");
    Util::CheckErrorAndLog(env, "Call", "Fail FindClass: IContentProvider", __LINE__);

    jmethodID m = env->GetMethodID(c, "call",
        "(Ljava/lang/String;"\
         "Ljava/lang/String;"\
         "Landroid/os/Bundle;)Landroid/os/Bundle;");
    Util::CheckErrorAndLog(env, TAG, "GetMethodID: call Line: %d", __LINE__);

    jobject jbundle = env->CallObjectMethod(mJInstance, m, jmethod, jarg, jextras);
    Util::CheckErrorAndLog(env, TAG, "CallObjectMethod: call Line: %d", __LINE__);

    if (jbundle != NULL) {
        if(!Util::GetElBundle(env, jbundle, bundle)) {
            LOGGERD(TAG, "Call() GetElBundle fail!");
        }

        env->DeleteLocalRef(jbundle);
    }

    env->DeleteLocalRef(c);
    env->DeleteLocalRef(jmethod);
    env->DeleteLocalRef(jarg);

    if(jextras){
        env->DeleteLocalRef(jextras);
    }

    // LOGGERD(TAG, "- CContentProviderNative::Call()");
    return NOERROR;
}

ECode CContentProviderNative::Query(
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& sortOrder,
    /* [in] */ IICancellationSignal* cancellationSignal,
    /* [out] */ ICursor** cursor)
{
    // LOGGERD(TAG, "+ CContentProviderNative::Query() selection %s", selection.string());
    *cursor = NULL;

    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);

    jobject juri = NULL;
    if (uri != NULL) {
        juri = Util::ToJavaUri(env, uri);
    } else {
        LOGGERE(TAG, "CContentProviderNative::Query() uri is NULL!");
    }

    jobjectArray jprojection = NULL;
    if (projection != NULL) {
        jprojection = Util::ToJavaStringArray(env, projection);
    }

    jobject jselection = Util::ToJavaString(env, selection);

    jobjectArray jselectionArgs = NULL;
    if (selectionArgs != NULL) {
        jselectionArgs = Util::ToJavaStringArray(env, selectionArgs);
    }

    jobject jsortOrder = Util::ToJavaString(env, sortOrder);

    jobject jcancellationSignal = NULL;
    if (cancellationSignal != NULL) {
        LOGGERE(TAG, "Query() cancellationSignal not NULL!");
    }

    jclass c = env->FindClass("android/content/IContentProvider");
    Util::CheckErrorAndLog(env, "Query", "Fail FindClass: IContentProvider", __LINE__);

    jmethodID m = env->GetMethodID(c, "query",
        "(Landroid/net/Uri;"\
         "[Ljava/lang/String;Ljava/lang/String;"\
         "[Ljava/lang/String;Ljava/lang/String;"\
         "Landroid/os/ICancellationSignal;)Landroid/database/Cursor;");
    Util::CheckErrorAndLog(env, TAG, "GetMethodID: query Line: %d", __LINE__);

    ECode ec = NOERROR;
    jobject jcursor = env->CallObjectMethod(mJInstance, m, juri, jprojection, jselection,
        jselectionArgs, jsortOrder, jcancellationSignal);

    // is it better to check in CheckErrorAndLog with JavaException2ECode?
    // throw android/database/sqlite/SQLiteException
    if (env->ExceptionCheck() != 0) {
        LOGGERD(TAG, "CallObjectMethod: Query Line: %d", __LINE__);
        env->ExceptionDescribe();
        env->ExceptionClear();

        ec = E_SQLITE_EXCEPTION;
    }
    else {
        if (jcursor != NULL) {
            jobject jInstance = env->NewGlobalRef(jcursor);
            env->DeleteLocalRef(jcursor);
            CCursorNative::New((Handle32)mJVM, (Handle32)jInstance, cursor);
        }
    }

    env->DeleteLocalRef(c);
    if(juri){
        env->DeleteLocalRef(juri);
    }

    if(jprojection){
        env->DeleteLocalRef(jprojection);
    }

    if(jselection){
        env->DeleteLocalRef(jselection);
    }

    if(jsortOrder){
        env->DeleteLocalRef(jsortOrder);
    }

    if(jcancellationSignal){
        env->DeleteLocalRef(jcancellationSignal);
    }

    // LOGGERD(TAG, "- CContentProviderNative::Query()");
    return ec;
}

ECode CContentProviderNative::OpenTypedAssetFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mimeType,
    /* [in] */ IBundle* opts,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    // LOGGERD(TAG, "+ CContentProviderNative::OpenTypedAssetFile()");
    *fileDescriptor = NULL;

    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);

    jobject juri = NULL;
    if (uri != NULL) {
        juri = Util::ToJavaUri(env, uri);
    }

    jobject jmimeType = Util::ToJavaString(env, mimeType);
    jobject jopts = NULL;
    if (opts != NULL) {
        jopts = Util::ToJavaBundle(env, opts);
    }

    jclass c = env->FindClass("android/content/IContentProvider");
    Util::CheckErrorAndLog(env, "OpenTypedAssetFile", "Fail FindClass: IContentProvider", __LINE__);

    jmethodID m = env->GetMethodID(c, "openTypedAssetFile",
        "(Landroid/net/Uri;"\
         "Ljava/lang/String;"\
         "Landroid/os/Bundle;)Landroid/content/res/AssetFileDescriptor;");
    Util::CheckErrorAndLog(env, TAG, "GetMethodID: openTypedAssetFile Line: %d", __LINE__);

    ECode ec = NOERROR;
    jobject jfileDescriptor = env->CallObjectMethod(mJInstance, m, juri, jmimeType, jopts);

    // is it better to check in CheckErrorAndLog with JavaException2ECode?
    // throw java/io/FileNotFoundException
    if (env->ExceptionCheck() != 0) {
        LOGGERD(TAG, "CallObjectMethod: OpenTypedAssetFile Line: %d", __LINE__);
        env->ExceptionDescribe();
        env->ExceptionClear();

        ec = E_FILE_NOT_FOUND_EXCEPTION;
    }
    else {
        if (jfileDescriptor != NULL) {
            if (!Util::GetElAssetFileDescriptor(env, jfileDescriptor, fileDescriptor)) {
                LOGGERE(TAG, "OpenTypedAssetFile() GetElAssetFileDescriptor fail!")
            }
            env->DeleteLocalRef(jfileDescriptor);
        }
    }

    env->DeleteLocalRef(c);

    if(juri){
        env->DeleteLocalRef(juri);
    }

    if(jmimeType){
        env->DeleteLocalRef(jmimeType);
    }

    if(jopts){
        env->DeleteLocalRef(jopts);
    }

    // LOGGERD(TAG, "- CContentProviderNative::OpenTypedAssetFile()");
    return ec;
}

ECode CContentProviderNative::GetType(
    /* [in] */ IUri* uri,
    /* [out] */ String* type)
{
    // LOGGERD(TAG, "+ CContentProviderNative::GetType()");

    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);

    jobject juri = NULL;
    if (uri != NULL) {
        juri = Util::ToJavaUri(env, uri);
    } else {
        LOGGERE(TAG, "CContentProviderNative::GetType() uri is NULL!");
    }

    jclass c = env->FindClass("android/content/IContentProvider");
    Util::CheckErrorAndLog(env, TAG, "FindClass: IContentProvider %d", __LINE__);

    jmethodID m = env->GetMethodID(c, "getType",
        "(Landroid/net/Uri;)Ljava/lang/String;");
    Util::CheckErrorAndLog(env, TAG, "GetMethodID: getType Line: %d", __LINE__);

    jstring jtype = (jstring)env->CallObjectMethod(mJInstance, m, juri);
    Util::CheckErrorAndLog(env, TAG, "CallObjectMethod: getType Line: %d", __LINE__);

    *type = Util::GetElString(env, jtype);

    env->DeleteLocalRef(c);
    env->DeleteLocalRef(juri);
    env->DeleteLocalRef(jtype);
    // LOGGERD(TAG, String("- CContentProviderNative::GetType()"));
    return NOERROR;
}

ECode CContentProviderNative::Insert(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* initialValues,
    /* [out] */ IUri** insertedUri)
{
    // LOGGERD(TAG, String("+ CContentProviderNative::Insert()"));

    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);

    jobject juri = NULL;
    if (uri != NULL) {
        juri = Util::ToJavaUri(env, uri);
    } else {
        LOGGERE(TAG, "CContentProviderNative::Insert() uri is NULL!");
    }

    jobject jInitialValues = NULL;
    if (initialValues != NULL) {
        jInitialValues = Util::ToJavaContentValues(env, initialValues);
    }
    else {
        LOGGERE(TAG, String("Insert() initialValues is NULL!"));
    }

    jclass c = env->FindClass("android/content/IContentProvider");
    Util::CheckErrorAndLog(env, "Insert", "Fail FindClass: IContentProvider", __LINE__);

    jmethodID m = env->GetMethodID(c, "insert",
        "(Landroid/net/Uri;Landroid/content/ContentValues;)Landroid/net/Uri;");
    Util::CheckErrorAndLog(env, TAG, "GetMethodID: insert Line: %d", __LINE__);

    jobject jrUri = env->CallObjectMethod(mJInstance, m, juri, jInitialValues);
    ECode ec = Util::CheckErrorAndLog(env, TAG, "CallObjectMethod: Insert Line: %d", __LINE__);

    if (SUCCEEDED(ec) && jrUri != NULL) {
        if (!Util::GetElUri(env, jrUri,insertedUri)) {
            LOGGERE(TAG, "Insert GetElUri fail");
        }
    } else {
        *insertedUri = NULL;
    }

    if (NULL != c) {
        env->DeleteLocalRef(c);
    }

    if (NULL != jInitialValues) {
        env->DeleteLocalRef(jInitialValues);
    }

    if (NULL != juri) {
        env->DeleteLocalRef(juri);
    }

    // LOGGERD(TAG, String("- CContentProviderNative::Insert()"));
    return ec;
}

ECode CContentProviderNative::BulkInsert(
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<IContentValues *>* initialValues,
    /* [out] */ Int32* number)
{
    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);

    jobject juri = NULL;
    if (uri != NULL) {
        juri = Util::ToJavaUri(env, uri);
    } else {
        LOGGERE(TAG, "CContentProviderNative::BulkInsert() uri is NULL!");
    }

    jobjectArray jinitialValues = NULL;
    if(initialValues != NULL) {
        jclass cvKlass = env->FindClass("android/content/ContentValues");
        Util::CheckErrorAndLog(env, TAG, "FindClass: ContentValues %d", __LINE__);

        Int32 count = initialValues->GetLength();
        jinitialValues = env->NewObjectArray((jsize)count, cvKlass, NULL);
        Util::CheckErrorAndLog(env, TAG, "NewObjectArray: ContentValues %d", __LINE__);

        for (Int32 i = 0; i < count; i++) {
            AutoPtr<IContentValues> cv = (*initialValues)[i];
            if (cv != NULL) {
                jobject jcv = Util::ToJavaContentValues(env, cv);

                env->SetObjectArrayElement(jinitialValues, i, jcv);
                Util::CheckErrorAndLog(env, TAG, "SetObjectArrayElement: ContentValues %d", __LINE__);
                env->DeleteLocalRef(jcv);
            }
        }

        env->DeleteLocalRef(cvKlass);
    }

    jclass c = env->FindClass("android/content/IContentProvider");
    Util::CheckErrorAndLog(env, "Insert", "Fail FindClass: IContentProvider", __LINE__);

    jmethodID m = env->GetMethodID(c, "bulkInsert", "(Landroid/net/Uri;[Landroid/content/ContentValues;)I");
    Util::CheckErrorAndLog(env, TAG, "GetMethodID: bulkInsert Line: %d", __LINE__);

    *number = env->CallIntMethod(mJInstance, m, juri, jinitialValues);
    Util::CheckErrorAndLog(env, TAG, "CallIntMethod: call bulkInsert: %d", __LINE__);

    env->DeleteLocalRef(juri);
    env->DeleteLocalRef(jinitialValues);
    env->DeleteLocalRef(c);
    return NOERROR;
}

ECode CContentProviderNative::Delete(
    /* [in] */ IUri* uri,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);

    jobject juri = NULL;
    if (uri != NULL) {
        juri = Util::ToJavaUri(env, uri);
    } else {
        LOGGERE(TAG, "CContentProviderNative::Delete() uri is NULL!");
    }

    jstring jselection = Util::ToJavaString(env, selection);

    jobjectArray jselectionArgs = NULL;
    if (selectionArgs != NULL) {
        jselectionArgs = Util::ToJavaStringArray(env, selectionArgs);
    }

    jclass c = env->FindClass("android/content/IContentProvider");
    Util::CheckErrorAndLog(env, "Delete()", "Fail FindClass: IContentProvider", __LINE__);

    jmethodID m = env->GetMethodID(c, "delete",
        "(Landroid/net/Uri;Ljava/lang/String;[Ljava/lang/String;)I");
    Util::CheckErrorAndLog(env, TAG, "GetMethodID: Delete Line: %d", __LINE__);

    *rowsAffected = env->CallIntMethod(mJInstance, m, juri, jselection, jselectionArgs);
    ECode ec = Util::CheckErrorAndLog(env, TAG, "CallIntMethod: Delete Line: %d", __LINE__);

    if (NULL != c) {
        env->DeleteLocalRef(c);
    }

    if (NULL != juri) {
        env->DeleteLocalRef(juri);
    }

    if (NULL != jselection) {
        env->DeleteLocalRef(jselection);
    }

    if (NULL != jselectionArgs) {
        env->DeleteLocalRef(jselectionArgs);
    }

    return ec;
}

ECode CContentProviderNative::Update(
    /* [in] */ IUri* uri,
    /* [in] */ IContentValues* values,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [out] */ Int32* rowsAffected)
{
    // LOGGERD(TAG, "+ CContentProviderNative::Update() ");
    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);

    jobject juri = NULL;
    if (uri != NULL) {
        juri = Util::ToJavaUri(env, uri);
    } else {
        LOGGERE(TAG, "CContentProviderNative::Update() uri is NULL!");
    }

    jobject jInitialValues = NULL;
    if (values != NULL) {
        jInitialValues = Util::ToJavaContentValues(env, values);
    }
    else {
        LOGGERE(TAG, "CContentProviderNative::Update() initialValues is NULL!");
    }

    jstring jselection = Util::ToJavaString(env, selection);

    jobjectArray jselectionArgs = NULL;
    if (selectionArgs != NULL) {
        jselectionArgs = Util::ToJavaStringArray(env, selectionArgs);
    }

    jclass c = env->FindClass("android/content/IContentProvider");
    Util::CheckErrorAndLog(env, "Update()", "Fail FindClass: IContentProvider", __LINE__);

    jmethodID m = env->GetMethodID(c, "update",
        "(Landroid/net/Uri;Landroid/content/ContentValues;Ljava/lang/String;[Ljava/lang/String;)I");
    Util::CheckErrorAndLog(env, TAG, "GetMethodID: Update Line: %d", __LINE__);

    *rowsAffected = env->CallIntMethod(mJInstance, m, juri, jInitialValues, jselection, jselectionArgs);
    ECode ec = Util::CheckErrorAndLog(env, TAG, "CallIntMethod: Update Line: %d", __LINE__);

    if (NULL != c) {
        env->DeleteLocalRef(c);
    }

    if (NULL != juri) {
        env->DeleteLocalRef(juri);
    }

    if (NULL != jselection) {
        env->DeleteLocalRef(jselection);
    }

    if (NULL != jselectionArgs) {
        env->DeleteLocalRef(jselectionArgs);
    }

    if (NULL != jInitialValues) {
        env->DeleteLocalRef(jInitialValues);
    }

    // LOGGERD(TAG, "- CContentProviderNative::Update() ");

    return ec;
}

ECode CContentProviderNative::OpenFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IParcelFileDescriptor** fileDescriptor)
{
    LOGGERD(TAG, String("+ CContentProviderNative::OpenFile()"));

    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);

    jobject juri = NULL;
    if (uri != NULL) {
        juri = Util::ToJavaUri(env, uri);
    } else {
        LOGGERE(TAG, "CContentProviderNative::OpenFile() uri is NULL!");
    }

    jstring jmode = Util::ToJavaString(env, mode);

    jclass c = env->FindClass("android/content/IContentProvider");
    Util::CheckErrorAndLog(env, TAG, "FindClass: IContentProvider %d", __LINE__);

    jmethodID m = env->GetMethodID(c, "openFile",
        "(Landroid/net/Uri;Ljava/lang/String;)Landroid/os/ParcelFileDescriptor;");
    Util::CheckErrorAndLog(env, TAG, "GetMethodID: openFile Line: %d", __LINE__);

    jobject jfileDescriptor = env->CallObjectMethod(mJInstance, m, juri, jmode);
    Util::CheckErrorAndLog(env, TAG, "CallObjectMethod: openFile Line: %d", __LINE__);

    if (jfileDescriptor != NULL) {
        if (!Util::GetElParcelFileDescriptor(env, jfileDescriptor, fileDescriptor)) {
            LOGGERE(TAG, "OpenFile() GetElParcelFileDescriptor fail!");
        }
    }

    env->DeleteLocalRef(c);
    env->DeleteLocalRef(juri);
    env->DeleteLocalRef(jmode);
    env->DeleteLocalRef(jfileDescriptor);
    LOGGERD(TAG, String("- CContentProviderNative::OpenFile()"));
    return NOERROR;
}

ECode CContentProviderNative::OpenAssetFile(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mode,
    /* [out] */ IAssetFileDescriptor** fileDescriptor)
{
    LOGGERD(TAG, String("+ CContentProviderNative::OpenAssetFile()"));

    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);

    jobject juri = NULL;
    if (uri != NULL) {
        juri = Util::ToJavaUri(env, uri);
    } else {
        LOGGERE(TAG, "CContentProviderNative::OpenAssetFile() uri is NULL!");
    }

    jstring jmode = Util::ToJavaString(env, mode);

    jclass c = env->FindClass("android/content/IContentProvider");
    Util::CheckErrorAndLog(env, TAG, "FindClass: IContentProvider %d", __LINE__);

    jmethodID m = env->GetMethodID(c, "openAssetFile",
        "(Landroid/net/Uri;Ljava/lang/String;)Landroid/os/ParcelFileDescriptor;");
    Util::CheckErrorAndLog(env, TAG, "GetMethodID: openAssetFile Line: %d", __LINE__);

    jobject jfileDescriptor = env->CallObjectMethod(mJInstance, m, juri, jmode);
    Util::CheckErrorAndLog(env, TAG, "CallObjectMethod: openAssetFile Line: %d", __LINE__);

    if (jfileDescriptor != NULL) {
        if (!Util::GetElAssetFileDescriptor(env, jfileDescriptor, fileDescriptor)) {
            LOGGERE(TAG, "openAssetFile() GetElAssetFileDescriptor fail!");
        }
    }

    env->DeleteLocalRef(c);
    env->DeleteLocalRef(juri);
    env->DeleteLocalRef(jmode);
    env->DeleteLocalRef(jfileDescriptor);
    LOGGERD(TAG, String("- CContentProviderNative::OpenAssetFile()"));
    return NOERROR;
}

ECode CContentProviderNative::ApplyBatch(
    /* [in] */ IObjectContainer* operations,
    /* [out, callee] */ ArrayOf<IContentProviderResult*>** providerResults)
{
    LOGGERD(TAG, String("+ CContentProviderNative::ApplyBatch()"));

    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);

     jobject joperations = NULL;
    if (operations != NULL) {
        Int32 count = 0;
        operations->GetObjectCount(&count);
        if (count > 0) {
            jclass listKlass = env->FindClass("java/util/ArrayList");
            Util::CheckErrorAndLog(env, TAG, "NewObject: ArrayList line: %d", __LINE__);

            jmethodID m = env->GetMethodID(listKlass, "<init>", "()V");
            Util::CheckErrorAndLog(env, TAG, "GetMethodID: ArrayList line: %d", __LINE__);

            joperations = env->NewObject(listKlass, m);
            Util::CheckErrorAndLog(env, TAG, "NewObject: ArrayList line: %d", __LINE__);

            jmethodID mAdd = env->GetMethodID(listKlass, "add", "(Ljava/lang/Object;)Z");
            Util::CheckErrorAndLog(env, TAG, "GetMethodID: add line: %d", __LINE__);

            AutoPtr<IObjectEnumerator> it;
            operations->GetObjectEnumerator((IObjectEnumerator**)&it);

            Boolean hasNext;
            while (it->MoveNext(&hasNext), hasNext) {
                AutoPtr<IInterface> obj;
                it->Current((IInterface**)&obj);
                AutoPtr<IContentProviderOperation> operation = IContentProviderOperation::Probe(obj);

                jobject joperation = Util::ToJavaContentProviderOperation(env, operation);

                env->CallBooleanMethod(joperations, mAdd, joperation);
                Util::CheckErrorAndLog(env, "ToJavaProviderRequest", "CallBooleanMethod: add : %d!\n", __LINE__);

                env->DeleteLocalRef(joperation);
            }
            env->DeleteLocalRef(listKlass);
        }
    }

    jclass c = env->FindClass("android/content/IContentProvider");
    Util::CheckErrorAndLog(env, TAG, "FindClass: IContentProvider %d", __LINE__);

    jmethodID m = env->GetMethodID(c, "applyBatch",  "(Ljava/util/ArrayList;)[Landroid/content/ContentProviderResult;");
    Util::CheckErrorAndLog(env, TAG, "GetMethodID: applyBatch Line: %d", __LINE__);

    jobjectArray jproviderResults = (jobjectArray)env->CallObjectMethod(mJInstance, m, joperations);
    ECode ec = Util::CheckErrorAndLog(env, TAG, "CallObjectMethod: applyBatch Line: %d", __LINE__);

    if(SUCCEEDED(ec) && jproviderResults != NULL) {
        jint jcount = env->GetArrayLength(jproviderResults);
        if (jcount > 0) {
            *providerResults = ArrayOf<IContentProviderResult*>::Alloc((Int32)jcount);
            if (*providerResults  != NULL) {
                for (Int32 i = 0; i < jcount; i++) {
                    jobject jproviderResult = env->GetObjectArrayElement(jproviderResults, i);
                    Util::CheckErrorAndLog(env, "GetElProviderInfo", "Fail GetObjectArrayElement: PatternMatcher %d", __LINE__);

                    AutoPtr<IContentProviderResult> providerResult;
                    if (!Util::GetElContentProviderResult(env, jproviderResult, (IContentProviderResult**)&providerResult)) {
                        (*providerResults)->Set(i, providerResult);
                    } else {
                        LOGGERE(TAG, "ApplyBatch() GetElContentProviderResult fail!");
                    }

                    env->DeleteLocalRef(jproviderResult);
                }
            } else {
                LOGGERE(TAG, "ApplyBatch() Alloc fail!");
            }
        } else {
            *providerResults = ArrayOf<IContentProviderResult*>::Alloc(0);
        }

        env->DeleteLocalRef(jproviderResults);
    } else {
        *providerResults = NULL;
    }

    env->DeleteLocalRef(c);
    env->DeleteLocalRef(joperations);
    LOGGERD(TAG, String("- CContentProviderNative::ApplyBatch()"));
    return NOERROR;
}

ECode CContentProviderNative::CreateCancellationSignal(
    /* [out] */ IICancellationSignal** cancellationSignal)
{
    LOGGERD(TAG, String("+ CContentProviderNative::CreateCancellationSignal()"));

    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);

    jclass c = env->FindClass("android/content/IContentProvider");
    Util::CheckErrorAndLog(env, TAG, "FindClass: IContentProvider %d", __LINE__);

    jmethodID m = env->GetMethodID(c, "createCancellationSignal",
        "()Landroid/os/ParcelFileDescriptor;");
    Util::CheckErrorAndLog(env, TAG, "GetMethodID: createCancellationSignal Line: %d", __LINE__);

    jobject jcancellationSignal = env->CallObjectMethod(mJInstance, m);
    Util::CheckErrorAndLog(env, TAG, "CallObjectMethod: createCancellationSignal Line: %d", __LINE__);

    if (jcancellationSignal != NULL) {
        jobject jInstance = env->NewGlobalRef(jcancellationSignal);
        env->DeleteLocalRef(jcancellationSignal);

        if(NOERROR != CICancellationSignalNative::New((Handle32)mJVM, (Handle32)jInstance, cancellationSignal)) {
            LOGGERE(TAG, String("CreateCancellationSignal() new CICancellationSignalNative fail!"));
        }
    }

    env->DeleteLocalRef(c);
    LOGGERD(TAG, String("- CContentProviderNative::CreateCancellationSignal()"));
    return NOERROR;
}

ECode CContentProviderNative::GetStreamTypes(
    /* [in] */ IUri* uri,
    /* [in] */ const String& mimeTypeFilter,
    /* [out, callee] */ ArrayOf<String>** streamTypes)
{
    LOGGERD(TAG, String("+ CContentProviderNative::GetStreamTypes()"));

    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);

    jobject juri = NULL;
    if (uri != NULL) {
        juri = Util::ToJavaUri(env, uri);
    } else {
        LOGGERE(TAG, "CContentProviderNative::GetStreamTypes() uri is NULL!");
    }

    jstring jmimeTypeFilter = Util::ToJavaString(env, mimeTypeFilter);

    jclass c = env->FindClass("android/content/IContentProvider");
    Util::CheckErrorAndLog(env, TAG, "FindClass: IContentProvider %d", __LINE__);

    jmethodID m = env->GetMethodID(c, "getStreamTypes",
        "(Landroid/net/Uri;Ljava/lang/String;)[Ljava/lang/String;");
    Util::CheckErrorAndLog(env, TAG, "GetMethodID: getStreamTypes Line: %d", __LINE__);

    jobjectArray jstreamTypes = (jobjectArray)env->CallObjectMethod(mJInstance, m, juri, jmimeTypeFilter);
    Util::CheckErrorAndLog(env, TAG, "CallObjectMethod: getStreamTypes Line: %d", __LINE__);

    if (jstreamTypes != NULL) {
        int size = env->GetArrayLength(jstreamTypes);
        Util::CheckErrorAndLog(env, TAG, "GetArrayLength: Line: %d", __LINE__);

        *streamTypes = ArrayOf<String>::Alloc(size);

        for(Int32 i = 0; i < size; i++){
            jstring streamType = (jstring)env->GetObjectArrayElement(jstreamTypes, i);
            Util::CheckErrorAndLog(env, TAG, "GetObjectArrayElement: Line: %d", __LINE__);
            (*streamTypes)->Set(i, Util::GetElString(env, streamType));
            env->DeleteLocalRef(streamType);
        }
    }

    env->DeleteLocalRef(c);
    env->DeleteLocalRef(juri);
    env->DeleteLocalRef(jmimeTypeFilter);
    env->DeleteLocalRef(jstreamTypes);
    LOGGERD(TAG, String("- CContentProviderNative::GetStreamTypes()"));
    return NOERROR;
}

ECode CContentProviderNative::ToString(
    /* [out] */ String* str)
{
    // LOGGERD(TAG, String("+ CContentProviderNative::ToString()"));

    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);

    jclass c = env->FindClass("java/lang/Object");
    Util::CheckErrorAndLog(env, TAG, "FindClass: Object %d", __LINE__);

    jmethodID m = env->GetMethodID(c, "toString", "()Ljava/lang/String;");
    Util::CheckErrorAndLog(env, TAG, "GetMethodID: toString %d", __LINE__);

    jstring jstr = (jstring)env->CallObjectMethod(mJInstance, m);
    Util::CheckErrorAndLog(env, TAG, "CallVoidMethod: toString %d", __LINE__);

    *str = Util::GetElString(env, jstr);

    env->DeleteLocalRef(c);
    env->DeleteLocalRef(jstr);

    // LOGGERD(TAG, String("- CContentProviderNative::ToString()"));
    return NOERROR;
}

}
}
}

