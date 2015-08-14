
#ifndef __ELASTOS_DROID_WEBKIT_BASE_CPUFEATURES_H__
#define __ELASTOS_DROID_WEBKIT_BASE_CPUFEATURES_H__

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

// The only purpose of this class is to allow sending CPU properties
// from the browser process to sandboxed renderer processes. This is
// needed because sandboxed processes cannot, on ARM, query the kernel
// about the CPU's properties by parsing /proc, so this operation must
// be performed in the browser process, and the result passed to
// renderer ones.
//
// For more context, see http://crbug.com/164154
//
// Technically, this is a wrapper around the native NDK cpufeatures
// library. The exact CPU features bits are never used in Java so
// there is no point in duplicating their definitions here.
//
//@JNINamespace("base::android")
class CpuFeatures
{
public:
    /**
     * Return the number of CPU Cores on the device.
     */
    static CARAPI_(Int32) GetCount();

    /**
     * Return the CPU feature mask.
     * This is a 64-bit integer that corresponds to the CPU's features.
     * The value comes directly from android_getCpuFeatures().
     */
    static CARAPI_(Int64) GetMask();

private:
    static CARAPI_(Int32) NativeGetCoreCount();
    static CARAPI_(Int64) NativeGetCpuFeatures();
};

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_BASE_CPUFEATURES_H__
