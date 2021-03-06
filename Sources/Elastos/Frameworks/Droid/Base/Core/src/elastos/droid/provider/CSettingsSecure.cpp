
#include "ext/frameworkdef.h"
#include "provider/CSettingsSecure.h"
#include "provider/Settings.h"

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CSettingsSecure::GetUriFor(
    /* [in] */ IUri* u,
    /* [in] */ const String& name,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri)
    return Settings::Secure::GetUriFor(u, name, uri);
}

ECode CSettingsSecure::GetContentUri(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri)
    *uri = Settings::Secure::CONTENT_URI;
    REFCOUNT_ADD(*uri)
    return NOERROR;
}

ECode CSettingsSecure::GetSettingsToBackup(
    /* [out] */ ArrayOf<String>** array)
{
    VALIDATE_NOT_NULL(array)
    *array = Settings::Secure::SETTINGS_TO_BACKUP;
    REFCOUNT_ADD(*array)
    return NOERROR;
}

ECode CSettingsSecure::GetMovedKeys(
    /* [in] */ IObjectContainer* outKeySet)
{
    Settings::Secure::GetMovedKeys(outKeySet);
    return NOERROR;
}

ECode CSettingsSecure::GetString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Secure::GetString(resolver, name, value);
}

ECode CSettingsSecure::GetStringForUser(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [in] */ Int32 userHandle,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Secure::GetStringForUser(resolver, name, userHandle, value);
}

ECode CSettingsSecure::PutString(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [in] */ const String& value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return Settings::Secure::PutString(resolver, name, value, result);
}

ECode CSettingsSecure::PutStringForUser(
    /* [in] */ IContentResolver* resolver,
    /* [in] */ const String& name,
    /* [in] */ const String& value,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return Settings::Secure::PutStringForUser(resolver, name, value, userHandle, result);
}

ECode CSettingsSecure::GetUriFor(
    /* [in] */ const String& name,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri)
    return Settings::Secure::GetUriFor(name, uri);
}

ECode CSettingsSecure::GetInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 def,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Secure::GetInt32(cr, name, def, value);
}

ECode CSettingsSecure::GetInt32ForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 def,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Secure::GetInt32ForUser(cr, name, def, userHandle, value);
}

ECode CSettingsSecure::GetInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Secure::GetInt32(cr, name, value);
}

ECode CSettingsSecure::GetInt32ForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int32* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Secure::GetInt32ForUser(cr, name, userHandle, value);
}

ECode CSettingsSecure::PutInt32(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return Settings::Secure::PutInt32(cr, name, value, result);
}

ECode CSettingsSecure::PutInt32ForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 value,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return Settings::Secure::PutInt32ForUser(cr, name, value, userHandle, result);
}

ECode CSettingsSecure::GetInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 def,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Secure::GetInt64(cr, name, def, value);
}

ECode CSettingsSecure::GetInt64ForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 def,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Secure::GetInt64ForUser(cr, name, def, userHandle, value);
}

ECode CSettingsSecure::GetInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Secure::GetInt64(cr, name, value);
}

ECode CSettingsSecure::GetInt64ForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 userHandle,
    /* [out] */ Int64* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Secure::GetInt64ForUser(cr, name, userHandle, value);
}

ECode CSettingsSecure::PutInt64(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return Settings::Secure::PutInt64(cr, name, value, result);
}

ECode CSettingsSecure::PutInt64ForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int64 value,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return Settings::Secure::PutInt64ForUser(cr, name, value, userHandle, result);
}

ECode CSettingsSecure::GetFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float def,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Secure::GetFloat(cr, name, def, value);
}

ECode CSettingsSecure::GetFloatForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float def,
    /* [in] */ Int32 userHandle,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Secure::GetFloatForUser(cr, name, def, userHandle, value);
}

ECode CSettingsSecure::GetFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Secure::GetFloat(cr, name, value);
}

ECode CSettingsSecure::GetFloatForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Int32 userHandle,
    /* [out] */ Float* value)
{
    VALIDATE_NOT_NULL(value)
    return Settings::Secure::GetFloatForUser(cr, name, userHandle, value);
}

ECode CSettingsSecure::PutFloat(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float value,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return Settings::Secure::PutFloat(cr, name, value, result);
}

ECode CSettingsSecure::PutFloatForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& name,
    /* [in] */ Float value,
    /* [in] */ Int32 userHandle,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return Settings::Secure::PutFloatForUser(cr, name, value, userHandle, result);
}

ECode CSettingsSecure::IsLocationProviderEnabled(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& provider,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return Settings::Secure::IsLocationProviderEnabled(cr, provider, result);
}

ECode CSettingsSecure::IsLocationProviderEnabledForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& provider,
    /* [in] */ Int32 userId,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    return Settings::Secure::IsLocationProviderEnabledForUser(cr, provider, userId, result);
}

ECode CSettingsSecure::SetLocationProviderEnabled(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& provider,
    /* [in] */ Boolean enabled)
{
    return Settings::Secure::SetLocationProviderEnabled(cr, provider, enabled);
}

ECode CSettingsSecure::SetLocationProviderEnabledForUser(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& provider,
    /* [in] */ Boolean enabled,
    /* [in] */ Int32 userId)
{
    return Settings::Secure::SetLocationProviderEnabledForUser(cr, provider, enabled, userId);
}

} //namespace Provider
} //namespace Droid
} //namespace Elastos
