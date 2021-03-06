
#include "ext/frameworkext.h"
#include "CAutomaticGainControl.h"
#include "CAudioEffectHelper.h"
#include "AudioEffect.h"

using Elastos::Utility::IUUIDHelper;
using Elastos::Utility::CUUIDHelper;

namespace Elastos {
namespace Droid {
namespace Media {
namespace Audiofx {

const String CAutomaticGainControl::TAG("AutomaticGainControl");

IAUDIOEFFECT_METHODS_IMPL(CAutomaticGainControl, AudioEffect)

/**
 * Class constructor.
 * <p> The constructor is not guarantied to succeed and throws the following exceptions:
 * <ul>
 *  <li>IllegalArgumentException is thrown if the device does not implement an AEC</li>
 *  <li>UnsupportedOperationException is thrown is the resources allocated to audio
 *  pre-procesing are currently exceeded.</li>
 *  <li>RuntimeException is thrown if a memory allocation error occurs.</li>
 * </ul>
 *
 * @param audioSession system wide unique audio session identifier. The AcousticEchoCanceler
 * will be applied to the AudioRecord with the same audio session.
 *
 * @throws java.lang.IllegalArgumentException
 * @throws java.lang.UnsupportedOperationException
 * @throws java.lang.RuntimeException
 */
ECode CAutomaticGainControl::constructor(
    /* [in] */ Int32 audioSession)
{
    AutoPtr<IUUIDHelper> helper;
    CUUIDHelper::AcquireSingleton((IUUIDHelper**)&helper);
    AutoPtr<IUUID> typeAGC;
    AutoPtr<IUUID> typeNULL;
    helper->FromString(IAudioEffect::EFFECT_TYPE_AGC, (IUUID**)&typeAGC);
    helper->FromString(IAudioEffect::EFFECT_TYPE_NULL, (IUUID**)&typeNULL);
    Init(typeAGC, typeNULL, 0, audioSession);
}

/**
 * Checks if the device implements acoustic echo cancellation.
 * @return true if the device implements acoustic echo cancellation, false otherwise.
 */
ECode CAutomaticGainControl::IsAvailable(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IUUIDHelper> helper;
    CUUIDHelper::AcquireSingleton((IUUIDHelper**)&helper);
    AutoPtr<IUUID> typeAGC;
    helper->FromString(IAudioEffect::EFFECT_TYPE_AGC, (IUUID**)&typeAGC);
    AutoPtr<IAudioEffectHelper> aeHelper;
    CAudioEffectHelper::AcquireSingleton((IAudioEffectHelper**)&aeHelper);
    return aeHelper->IsEffectTypeAvailable(typeAGC, result);
}

PInterface CAutomaticGainControl::Probe(
    /* [in] */ REIID riid)
{
    return _CAutomaticGainControl::Probe(riid);
}

} // namespace Audiofx
} // namespace Media
} // namepsace Droid
} // namespace Elastos
