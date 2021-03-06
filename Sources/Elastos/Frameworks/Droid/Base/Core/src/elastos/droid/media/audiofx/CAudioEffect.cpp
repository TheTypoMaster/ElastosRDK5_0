#include "media/media/audiofx/CAudioEffect.h"

namespace Elastos {
namespace Droid {
namespace Media {
namespace Audiofx {

ECode CAudioEffect::constructor(
    /* [in] */ IUUID* type,
    /* [in] */ IUUID* uuid,
    /* [in] */ Int32 priority,
    /* [in] */ Int32 audioSession)
{
    return AudioEffect::Init(type, uuid, priority, audioSession);
}

ECode CAudioEffect::ReleaseResources()
{
    return AudioEffect::ReleaseResources();
}

ECode CAudioEffect::GetDescriptor(
    /* [out] */ IAudioEffectDescriptor** descriptor)
{
    return AudioEffect::GetDescriptor(descriptor);
}

ECode CAudioEffect::SetEnabled(
    /* [in]  */ Boolean enabled,
    /* [out] */ Int32* result)
{
    return AudioEffect::SetEnabled(enabled, result);
}

ECode CAudioEffect::SetParameter(
    /* [in] */ ArrayOf<Byte>* param,
    /* [in] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* result)
{
    return AudioEffect::SetParameter(param, value, result);
}

ECode CAudioEffect::SetParameter(
    /* [in] */ Int32 param,
    /* [in] */ Int32 value,
    /* [out] */ Int32* result)
{
    return AudioEffect::SetParameter(param, value, result);
}

ECode CAudioEffect::SetParameter(
    /* [in] */ Int32 param,
    /* [in] */ Int16 value,
    /* [out] */ Int32* result)
{
    return AudioEffect::SetParameter(param, value, result);
}

ECode CAudioEffect::SetParameter(
    /* [in] */ Int32 param,
    /* [in] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* result)
{
    return AudioEffect::SetParameter(param, value, result);
}

ECode CAudioEffect::SetParameter(
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* result)
{
    return AudioEffect::SetParameter(param, value, result);
}

ECode CAudioEffect::SetParameter(
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* result)
{
    return AudioEffect::SetParameter(param, value, result);
}

ECode CAudioEffect::SetParameter(
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* result)
{
    return AudioEffect::SetParameter(param, value, result);
}

ECode CAudioEffect::GetParameter(
    /* [in] */ ArrayOf<Byte>* param,
    /* [out] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* status)
{
    return AudioEffect::GetParameter(param, value, status);
}

ECode CAudioEffect::GetParameter(
    /* [in] */ const Int32 param,
    /* [out] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* status)
{
    return AudioEffect::GetParameter(param, value, status);
}

ECode CAudioEffect::GetParameter(
    /* [in] */ Int32 param,
    /* [out] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* status)
{
    return AudioEffect::GetParameter(param, value, status);
}

ECode CAudioEffect::GetParameter(
    /* [in] */ Int32 param,
    /* [out] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* status)
{
    return AudioEffect::GetParameter(param, value, status);
}

ECode CAudioEffect::GetParameter(
    /* [in] */ ArrayOf<Int32>* param,
    /* [out] */ ArrayOf<Int32>* value,
    /* [out] */ Int32* status)
{
    return AudioEffect::GetParameter(param, value, status);
}

ECode CAudioEffect::GetParameter(
    /* [in] */ ArrayOf<Int32>* param,
    /* [out] */ ArrayOf<Int16>* value,
    /* [out] */ Int32* status)
{
    return AudioEffect::GetParameter(param, value, status);
}

ECode CAudioEffect::GetParameter(
    /* [in] */ ArrayOf<Int32>* param,
    /* [in] */ ArrayOf<Byte>* value,
    /* [out] */ Int32* status)
{
    return AudioEffect::GetParameter(param, value, status);
}

ECode CAudioEffect::Command(
    /* [in] */ Int32 cmdCode,
    /* [in] */ ArrayOf<Byte>* command,
    /* [in] */ ArrayOf<Byte>* reply,
    /* [out] */ Int32* result)
{
    return AudioEffect::Command(cmdCode, command, reply, result);
}

ECode CAudioEffect::GetId(
    /* [out] */ Int32* Id)
{
    return AudioEffect::GetId(Id);
}

ECode CAudioEffect::GetEnabled(
    /* [out] */ Boolean* enabled)
{
    return AudioEffect::GetEnabled(enabled);
}

ECode CAudioEffect::HasControl(
    /* [out] */ Boolean* control)
{
    return AudioEffect::HasControl(control);
}

ECode CAudioEffect::SetEnableStatusListener(
    /* [in] */ IAudioEffectOnEnableStatusChangeListener* listener)
{
    return AudioEffect::SetEnableStatusListener(listener);
}

ECode CAudioEffect::SetControlStatusListener(
    /* [in] */ IAudioEffectOnControlStatusChangeListener* listener)
{
    return AudioEffect::SetControlStatusListener(listener);
}

ECode CAudioEffect::SetParameterListener(
    /* [in] */ IAudioEffectOnParameterChangeListener* listener)
{
    return AudioEffect::SetParameterListener(listener);
}

ECode CAudioEffect::CheckState(
    /* [in] */ const String& methodName)
{
    return AudioEffect::CheckState(methodName);
}

ECode CAudioEffect::CheckStatus(
    /* [in] */ Int32 status)
{
    return AudioEffect::CheckStatus(status);
}

ECode CAudioEffect::ByteArrayToInt32(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [out] */ Int32* result)
{
    return AudioEffect::ByteArrayToInt32(valueBuf, result);
}

ECode CAudioEffect::ByteArrayToInt32(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [in] */ Int32 offset,
    /* [out] */ Int32* result)
{
    return AudioEffect::ByteArrayToInt32(valueBuf, offset, result);
}

ECode CAudioEffect::Int32ToByteArray(
    /* [in] */ Int32 value,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return AudioEffect::Int32ToByteArray(value, result);
}

ECode CAudioEffect::ByteArrayToInt16(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [out] */ Int16* result)
{
    return AudioEffect::ByteArrayToInt16(valueBuf, result);
}

ECode CAudioEffect::ByteArrayToInt16(
    /* [in] */ ArrayOf<Byte>* valueBuf,
    /* [in] */ Int32 offset,
    /* [out] */ Int16* result)
{
    return AudioEffect::ByteArrayToInt16(valueBuf, offset, result);
}

ECode CAudioEffect::Int16ToByteArray(
    /* [in] */ Int16 value,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return AudioEffect::Int16ToByteArray(value, result);
}

ECode CAudioEffect::ConcatArrays(
    /* [in] */ ArrayOf<Byte>* array1,
    /* [in] */ ArrayOf<Byte>* array2,
    /* [out, callee] */ ArrayOf<Byte>** result)
{
    return AudioEffect::ConcatArrays(array1, array2, result);
}

PInterface CAudioEffect::Probe(
    /* [in] */ REIID riid)
{
    return _CAudioEffect::Probe(riid);
}

} // namespace Audiofx
} // namespace Media
} // namepsace Droid
} // namespace Elastos
