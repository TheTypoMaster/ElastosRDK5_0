
#include "CAcousticEchoCancelerHelper.h"
#include "CAcousticEchoCanceler.h"

namespace Elastos {
namespace Droid {
namespace Media {
namespace Audiofx {

/**
 * Checks if the device implements acoustic echo cancellation.
 * @return true if the device implements acoustic echo cancellation, false otherwise.
 */
ECode CAcousticEchoCancelerHelper::IsAvailable(
    /* [out] */ Boolean* result)
{
    return CAcousticEchoCanceler::IsAvailable(result);
}

/**
 * Creates an AcousticEchoCanceler and attaches it to the AudioRecord on the audio
 * session specified.
 * @param audioSession system wide unique audio session identifier. The AcousticEchoCanceler
 * will be applied to the AudioRecord with the same audio session.
 * @return AcousticEchoCanceler created or null if the device does not implement AEC.
 */
ECode CAcousticEchoCancelerHelper::Create(
    /* [in] */ Int32 audioSession,
    /* [out] */ IAcousticEchoCanceler** result)
{
    return CAcousticEchoCanceler::New(audioSession, result);
}

} // namespace Audiofx
} // namespace Media
} // namepsace Droid
} // namespace Elastos
