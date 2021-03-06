
#ifndef __ELASTOS_DROID_DROIDRUNTIME_H__
#define __ELASTOS_DROID_DROIDRUNTIME_H__

#include <elastos.h>

using namespace Elastos;

namespace Elastos {
namespace Droid {

class DroidRuntime : public ElLightRefBase
{
public:
    DroidRuntime(
        /* [in] */ char* argBlockStart,
        /* [in] */ size_t argBlockSize);

    virtual ~DroidRuntime();

    enum StartMode {
        Zygote,
        SystemServer,
        Application,
        Tool,
    };

    void SetArgv0(
        /* [in] */ const String& argv0);

    void AddOption(
        /* [in] */ const String& optionString,
        /* [in] */ void* extra_info = NULL);

    CARAPI CallMain(
        /* [in] */ const String& moduleName,
        /* [in] */ const String& className,
        /* [in] */ ArrayOf<String>* args);

    CARAPI_(void) Start(
        /* [in] */ const String& moduleName,
        /* [in] */ const String& className,
        /* [in] */ ArrayOf<String>* options);

    CARAPI_(void) Exit(
        /* [in] */ Int32 code);

    static CARAPI_(AutoPtr<DroidRuntime>) GetRuntime();

    /**
     * This gets called after the VM has been created, but before we
     * run any code. Override it to make any FindClass calls that need
     * to use CLASSPATH.
     */
    //virtual void OnVmCreated(JNIEnv* env);

    /**
     * This gets called after the JavaVM has initialized.  Override it
     * with the system's native entry point.
     */
    virtual void OnStarted() = 0;

    /**
     * This gets called after the JavaVM has initialized after a Zygote
     * fork. Override it to initialize threads, etc. Upon return, the
     * correct static main will be invoked.
     */
    virtual void OnZygoteInit() { }

    /**
     * Called when the Java application exits to perform additional cleanup actions
     * before the process is terminated.
     */
    virtual void OnExit(
        /* [in] */ Int32 code) { }

private:
    Boolean mExitWithoutCleanup;
    char* const mArgBlockStart;
    const size_t mArgBlockLength;
};

} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_DROIDRUNTIME_H__
