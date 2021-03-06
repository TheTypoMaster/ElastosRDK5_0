
#include "CIIntentSenderNative.h"
#include <elastos/utility/logging/Logger.h>
#include "Util.h"

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace JavaProxy {

const String CIIntentSenderNative::TAG("CIIntentSenderNative");

CIIntentSenderNative::~CIIntentSenderNative()
{
    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);
    env->DeleteGlobalRef(mJInstance);
}

ECode CIIntentSenderNative::constructor(
    /* [in] */ Handle32 jVM,
    /* [in] */ Handle32 jInstance)
{
    mJVM = (JavaVM*)jVM;
    mJInstance = (jobject)jInstance;
    return NOERROR;
}

ECode CIIntentSenderNative::Send(
    /* [in] */ Int32 code,
    /* [in] */ IIntent* intent,
    /* [in] */ const String& resolvedType,
    /* [in] */ IIntentReceiver* finishedReceiver,
    /* [in] */ const String& requiredPermission,
    /* [out] */ Int32* result)
{
    // LOGGERD(TAG, String("+ CIIntentSenderNative::Send()"));

    JNIEnv* env;
    mJVM->AttachCurrentThread(&env, NULL);

    jobject jintent = NULL;
    if (intent != NULL) {
        jintent = Util::ToJavaIntent(env, intent);
    }

    jstring jresolvedType = Util::ToJavaString(env, resolvedType);

    jobject jfinishedReceiver = NULL;
    if (finishedReceiver != NULL) {
        jfinishedReceiver = Util::ToJavaIntentReceiver(env, finishedReceiver);
    }

    jstring jrequiredPermission = Util::ToJavaString(env, requiredPermission);

    jclass c = env->FindClass("android/content/IIntentSender");
    Util::CheckErrorAndLog(env, TAG, "Fail FindClass: IIntentSender %d", __LINE__);

    jmethodID m = env->GetMethodID(c, "send", "(ILandroid/content/Intent;Ljava/lang/String;Landroid/content/IIntentReceiver;Ljava/lang/String;)I");
    Util::CheckErrorAndLog(env, TAG, "GetMethodID: send %d", __LINE__);

    *result = env->CallIntMethod(mJInstance, m, (jint)code, jintent, jresolvedType, jfinishedReceiver, jrequiredPermission);
    Util::CheckErrorAndLog(env, TAG, "CallVoidMethod: send %d", __LINE__);

    env->DeleteLocalRef(c);

    if(jintent){
        env->DeleteLocalRef(jintent);
    }

    env->DeleteLocalRef(jresolvedType);

    if(jfinishedReceiver){
        env->DeleteLocalRef(jfinishedReceiver);
    }

    env->DeleteLocalRef(jrequiredPermission);

    // LOGGERD(TAG, String("- CIIntentSenderNative::Send()"));
    return NOERROR;
}

ECode CIIntentSenderNative::ToString(
    /* [out] */ String* str)
{
    // LOGGERD(TAG, String("+ CIIntentSenderNative::ToString()"));

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

    // LOGGERD(TAG, String("- CIIntentSenderNative::ToString()"));
    return NOERROR;
}

}
}
}

