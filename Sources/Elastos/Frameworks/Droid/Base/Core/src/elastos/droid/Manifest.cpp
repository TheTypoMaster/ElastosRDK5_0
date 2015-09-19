#include "Manifest.h"

namespace Elastos {
namespace Droid {

    // static constant belong to Permission class
    const String Manifest::Permission::ACCESS_ALL_EXTERNAL_STORAGE("android.permission.ACCESS_ALL_EXTERNAL_STORAGE");
    const String Manifest::Permission::ACCESS_CACHE_FILESYSTEM("android.permission.ACCESS_CACHE_FILESYSTEM");
    const String Manifest::Permission::ACCESS_CHECKIN_PROPERTIES("android.permission.ACCESS_CHECKIN_PROPERTIES");
    const String Manifest::Permission::ACCESS_COARSE_LOCATION("android.permission.ACCESS_COARSE_LOCATION");
    const String Manifest::Permission::ACCESS_CONTENT_PROVIDERS_EXTERNALLY("android.permission.ACCESS_CONTENT_PROVIDERS_EXTERNALLY");
    const String Manifest::Permission::ACCESS_FINE_LOCATION("android.permission.ACCESS_FINE_LOCATION");
    const String Manifest::Permission::ACCESS_LOCATION_EXTRA_COMMANDS("android.permission.ACCESS_LOCATION_EXTRA_COMMANDS");
    const String Manifest::Permission::ACCESS_MOCK_LOCATION("android.permission.ACCESS_MOCK_LOCATION");
    const String Manifest::Permission::ACCESS_MTP("android.permission.ACCESS_MTP");
    const String Manifest::Permission::ACCESS_NETWORK_STATE("android.permission.ACCESS_NETWORK_STATE");
    const String Manifest::Permission::ACCESS_SURFACE_FLINGER("android.permission.ACCESS_SURFACE_FLINGER");
    const String Manifest::Permission::ACCESS_WIFI_STATE("android.permission.ACCESS_WIFI_STATE");
    const String Manifest::Permission::ACCESS_WIMAX_STATE("android.permission.ACCESS_WIMAX_STATE");
    const String Manifest::Permission::ACCOUNT_MANAGER("android.permission.ACCOUNT_MANAGER");
    const String Manifest::Permission::ADD_VOICEMAIL("com.android.voicemail.permission.ADD_VOICEMAIL");
    const String Manifest::Permission::ALLOW_ANY_CODEC_FOR_PLAYBACK("android.permission.ALLOW_ANY_CODEC_FOR_PLAYBACK");
    const String Manifest::Permission::ASEC_ACCESS("android.permission.ASEC_ACCESS");
    const String Manifest::Permission::ASEC_CREATE("android.permission.ASEC_CREATE");
    const String Manifest::Permission::ASEC_DESTROY("android.permission.ASEC_DESTROY");
    const String Manifest::Permission::ASEC_MOUNT_UNMOUNT("android.permission.ASEC_MOUNT_UNMOUNT");
    const String Manifest::Permission::ASEC_RENAME("android.permission.ASEC_RENAME");
    const String Manifest::Permission::AUTHENTICATE_ACCOUNTS("android.permission.AUTHENTICATE_ACCOUNTS");
    const String Manifest::Permission::BACKUP("android.permission.BACKUP");
    const String Manifest::Permission::BATTERY_STATS("android.permission.BATTERY_STATS");
    const String Manifest::Permission::BIND_ACCESSIBILITY_SERVICE("android.permission.BIND_ACCESSIBILITY_SERVICE");
    const String Manifest::Permission::BIND_APPWIDGET("android.permission.BIND_APPWIDGET");
    const String Manifest::Permission::BIND_DEVICE_ADMIN("android.permission.BIND_DEVICE_ADMIN");
    const String Manifest::Permission::BIND_DIRECTORY_SEARCH("android.permission.BIND_DIRECTORY_SEARCH");
    const String Manifest::Permission::BIND_INPUT_METHOD("android.permission.BIND_INPUT_METHOD");
    const String Manifest::Permission::BIND_KEYGUARD_APPWIDGET("android.permission.BIND_KEYGUARD_APPWIDGET");
    const String Manifest::Permission::BIND_PACKAGE_VERIFIER("android.permission.BIND_PACKAGE_VERIFIER");
    const String Manifest::Permission::BIND_REMOTEVIEWS("android.permission.BIND_REMOTEVIEWS");
    const String Manifest::Permission::BIND_TEXT_SERVICE("android.permission.BIND_TEXT_SERVICE");
    const String Manifest::Permission::BIND_VPN_SERVICE("android.permission.BIND_VPN_SERVICE");
    const String Manifest::Permission::BIND_WALLPAPER("android.permission.BIND_WALLPAPER");
    const String Manifest::Permission::BLUETOOTH("android.permission.BLUETOOTH");
    const String Manifest::Permission::BLUETOOTH_ADMIN("android.permission.BLUETOOTH_ADMIN");
    const String Manifest::Permission::BLUETOOTH_STACK("android.permission.BLUETOOTH_STACK");
    const String Manifest::Permission::BRICK("android.permission.BRICK");
    const String Manifest::Permission::BROADCAST_PACKAGE_REMOVED("android.permission.BROADCAST_PACKAGE_REMOVED");
    const String Manifest::Permission::BROADCAST_SMS("android.permission.BROADCAST_SMS");
    const String Manifest::Permission::BROADCAST_STICKY("android.permission.BROADCAST_STICKY");
    const String Manifest::Permission::BROADCAST_WAP_PUSH("android.permission.BROADCAST_WAP_PUSH");
    const String Manifest::Permission::C2D_MESSAGE("android.intent.category.MASTER_CLEAR.permission.C2D_MESSAGE");
    const String Manifest::Permission::CALL_PHONE("android.permission.CALL_PHONE");
    const String Manifest::Permission::CALL_PRIVILEGED("android.permission.CALL_PRIVILEGED");
    const String Manifest::Permission::CAMERA("android.permission.CAMERA");
    const String Manifest::Permission::CHANGE_BACKGROUND_DATA_SETTING("android.permission.CHANGE_BACKGROUND_DATA_SETTING");
    const String Manifest::Permission::CHANGE_COMPONENT_ENABLED_STATE("android.permission.CHANGE_COMPONENT_ENABLED_STATE");
    const String Manifest::Permission::CHANGE_CONFIGURATION("android.permission.CHANGE_CONFIGURATION");
    const String Manifest::Permission::CHANGE_NETWORK_STATE("android.permission.CHANGE_NETWORK_STATE");
    const String Manifest::Permission::CHANGE_WIFI_MULTICAST_STATE("android.permission.CHANGE_WIFI_MULTICAST_STATE");
    const String Manifest::Permission::CHANGE_WIFI_STATE("android.permission.CHANGE_WIFI_STATE");
    const String Manifest::Permission::CHANGE_WIMAX_STATE("android.permission.CHANGE_WIMAX_STATE");
    const String Manifest::Permission::CLEAR_APP_CACHE("android.permission.CLEAR_APP_CACHE");
    const String Manifest::Permission::CLEAR_APP_USER_DATA("android.permission.CLEAR_APP_USER_DATA");
    const String Manifest::Permission::CONFIGURE_WIFI_DISPLAY("android.permission.CONFIGURE_WIFI_DISPLAY");
    const String Manifest::Permission::CONFIRM_FULL_BACKUP("android.permission.CONFIRM_FULL_BACKUP");
    const String Manifest::Permission::CONNECTIVITY_INTERNAL("android.permission.CONNECTIVITY_INTERNAL");
    const String Manifest::Permission::CONTROL_LOCATION_UPDATES("android.permission.CONTROL_LOCATION_UPDATES");
    const String Manifest::Permission::CONTROL_WIFI_DISPLAY("android.permission.CONTROL_WIFI_DISPLAY");
    const String Manifest::Permission::COPY_PROTECTED_DATA("android.permission.COPY_PROTECTED_DATA");
    const String Manifest::Permission::CRYPT_KEEPER("android.permission.CRYPT_KEEPER");
    const String Manifest::Permission::DELETE_CACHE_FILES("android.permission.DELETE_CACHE_FILES");
    const String Manifest::Permission::DELETE_PACKAGES("android.permission.DELETE_PACKAGES");
    const String Manifest::Permission::DEVICE_POWER("android.permission.DEVICE_POWER");
    const String Manifest::Permission::DIAGNOSTIC("android.permission.DIAGNOSTIC");
    const String Manifest::Permission::DISABLE_KEYGUARD("android.permission.DISABLE_KEYGUARD");
    const String Manifest::Permission::DUMP("android.permission.DUMP");
    const String Manifest::Permission::EXPAND_STATUS_BAR("android.permission.EXPAND_STATUS_BAR");
    const String Manifest::Permission::FACTORY_TEST("android.permission.FACTORY_TEST");
    const String Manifest::Permission::FILTER_EVENTS("android.permission.FILTER_EVENTS");
    const String Manifest::Permission::FLASHLIGHT("android.permission.FLASHLIGHT");
    const String Manifest::Permission::FORCE_BACK("android.permission.FORCE_BACK");
    const String Manifest::Permission::FORCE_STOP_PACKAGES("android.permission.FORCE_STOP_PACKAGES");
    const String Manifest::Permission::FREEZE_SCREEN("android.permission.FREEZE_SCREEN");
    const String Manifest::Permission::GET_ACCOUNTS("android.permission.GET_ACCOUNTS");
    const String Manifest::Permission::GET_DETAILED_TASKS("android.permission.GET_DETAILED_TASKS");
    const String Manifest::Permission::GET_PACKAGE_SIZE("android.permission.GET_PACKAGE_SIZE");
    const String Manifest::Permission::GET_TASKS("android.permission.GET_TASKS");
    const String Manifest::Permission::GLOBAL_SEARCH("android.permission.GLOBAL_SEARCH");
    const String Manifest::Permission::GLOBAL_SEARCH_CONTROL("android.permission.GLOBAL_SEARCH_CONTROL");
    const String Manifest::Permission::GRANT_REVOKE_PERMISSIONS("android.permission.GRANT_REVOKE_PERMISSIONS");
    const String Manifest::Permission::HARDWARE_TEST("android.permission.HARDWARE_TEST");
    const String Manifest::Permission::INJECT_EVENTS("android.permission.INJECT_EVENTS");
    const String Manifest::Permission::INSTALL_LOCATION_PROVIDER("android.permission.INSTALL_LOCATION_PROVIDER");
    const String Manifest::Permission::INSTALL_PACKAGES("android.permission.INSTALL_PACKAGES");
    const String Manifest::Permission::INTERACT_ACROSS_USERS("android.permission.INTERACT_ACROSS_USERS");
    const String Manifest::Permission::INTERACT_ACROSS_USERS_FULL("android.permission.INTERACT_ACROSS_USERS_FULL");
    const String Manifest::Permission::INTERNAL_SYSTEM_WINDOW("android.permission.INTERNAL_SYSTEM_WINDOW");
    const String Manifest::Permission::INTERNET("android.permission.INTERNET");
    const String Manifest::Permission::KILL_BACKGROUND_PROCESSES("android.permission.KILL_BACKGROUND_PROCESSES");
    const String Manifest::Permission::MAGNIFY_DISPLAY("android.permission.MAGNIFY_DISPLAY");
    const String Manifest::Permission::MANAGE_ACCOUNTS("android.permission.MANAGE_ACCOUNTS");
    const String Manifest::Permission::MANAGE_APP_TOKENS("android.permission.MANAGE_APP_TOKENS");
    const String Manifest::Permission::MANAGE_NETWORK_POLICY("android.permission.MANAGE_NETWORK_POLICY");
    const String Manifest::Permission::MANAGE_USB("android.permission.MANAGE_USB");
    const String Manifest::Permission::MANAGE_USERS("android.permission.MANAGE_USERS");
    const String Manifest::Permission::MASTER_CLEAR("android.permission.MASTER_CLEAR");
    const String Manifest::Permission::MODIFY_APPWIDGET_BIND_PERMISSIONS("android.permission.MODIFY_APPWIDGET_BIND_PERMISSIONS");
    const String Manifest::Permission::MODIFY_AUDIO_SETTINGS("android.permission.MODIFY_AUDIO_SETTINGS");
    const String Manifest::Permission::MODIFY_NETWORK_ACCOUNTING("android.permission.MODIFY_NETWORK_ACCOUNTING");
    const String Manifest::Permission::MODIFY_PHONE_STATE("android.permission.MODIFY_PHONE_STATE");
    const String Manifest::Permission::MOUNT_FORMAT_FILESYSTEMS("android.permission.MOUNT_FORMAT_FILESYSTEMS");
    const String Manifest::Permission::MOUNT_UNMOUNT_FILESYSTEMS("android.permission.MOUNT_UNMOUNT_FILESYSTEMS");
    const String Manifest::Permission::MOVE_PACKAGE("android.permission.MOVE_PACKAGE");
    const String Manifest::Permission::NET_ADMIN("android.permission.NET_ADMIN");
    const String Manifest::Permission::NET_TUNNELING("android.permission.NET_TUNNELING");
    const String Manifest::Permission::NFC("android.permission.NFC");
    const String Manifest::Permission::PACKAGE_USAGE_STATS("android.permission.PACKAGE_USAGE_STATS");
    const String Manifest::Permission::PACKAGE_VERIFICATION_AGENT("android.permission.PACKAGE_VERIFICATION_AGENT");
    const String Manifest::Permission::PERFORM_CDMA_PROVISIONING("android.permission.PERFORM_CDMA_PROVISIONING");
    const String Manifest::Permission::PERSISTENT_ACTIVITY("android.permission.PERSISTENT_ACTIVITY");
    const String Manifest::Permission::PROCESS_OUTGOING_CALLS("android.permission.PROCESS_OUTGOING_CALLS");
    const String Manifest::Permission::READ_CALENDAR("android.permission.READ_CALENDAR");
    const String Manifest::Permission::READ_CALL_LOG("android.permission.READ_CALL_LOG");
    const String Manifest::Permission::READ_CELL_BROADCASTS("android.permission.READ_CELL_BROADCASTS");
    const String Manifest::Permission::READ_CONTACTS("android.permission.READ_CONTACTS");
    const String Manifest::Permission::READ_DREAM_STATE("android.permission.READ_DREAM_STATE");
    const String Manifest::Permission::READ_EXTERNAL_STORAGE("android.permission.READ_EXTERNAL_STORAGE");
    const String Manifest::Permission::READ_FRAME_BUFFER("android.permission.READ_FRAME_BUFFER");
    const String Manifest::Permission::READ_HISTORY_BOOKMARKS("com.android.browser.permission.READ_HISTORY_BOOKMARKS");
    const String Manifest::Permission::READ_INPUT_STATE("android.permission.READ_INPUT_STATE");
    const String Manifest::Permission::READ_LOGS("android.permission.READ_LOGS");
    const String Manifest::Permission::READ_NETWORK_USAGE_HISTORY("android.permission.READ_NETWORK_USAGE_HISTORY");
    const String Manifest::Permission::READ_PHONE_STATE("android.permission.READ_PHONE_STATE");
    const String Manifest::Permission::READ_PRIVILEGED_PHONE_STATE("android.permission.READ_PRIVILEGED_PHONE_STATE");
    const String Manifest::Permission::READ_PROFILE("android.permission.READ_PROFILE");
    const String Manifest::Permission::READ_SMS("android.permission.READ_SMS");
    const String Manifest::Permission::READ_SOCIAL_STREAM("android.permission.READ_SOCIAL_STREAM");
    const String Manifest::Permission::READ_SYNC_SETTINGS("android.permission.READ_SYNC_SETTINGS");
    const String Manifest::Permission::READ_SYNC_STATS("android.permission.READ_SYNC_STATS");
    const String Manifest::Permission::READ_USER_DICTIONARY("android.permission.READ_USER_DICTIONARY");
    const String Manifest::Permission::REBOOT("android.permission.REBOOT");
    const String Manifest::Permission::RECEIVE_BOOT_COMPLETED("android.permission.RECEIVE_BOOT_COMPLETED");
    const String Manifest::Permission::RECEIVE_DATA_ACTIVITY_CHANGE("android.permission.RECEIVE_DATA_ACTIVITY_CHANGE");
    const String Manifest::Permission::RECEIVE_EMERGENCY_BROADCAST("android.permission.RECEIVE_EMERGENCY_BROADCAST");
    const String Manifest::Permission::RECEIVE_MMS("android.permission.RECEIVE_MMS");
    const String Manifest::Permission::RECEIVE_SMS("android.permission.RECEIVE_SMS");
    const String Manifest::Permission::RECEIVE_WAP_PUSH("android.permission.RECEIVE_WAP_PUSH");
    const String Manifest::Permission::RECORD_AUDIO("android.permission.RECORD_AUDIO");
    const String Manifest::Permission::REMOTE_AUDIO_PLAYBACK("android.permission.REMOTE_AUDIO_PLAYBACK");
    const String Manifest::Permission::REMOVE_TASKS("android.permission.REMOVE_TASKS");
    const String Manifest::Permission::REORDER_TASKS("android.permission.REORDER_TASKS");
    const String Manifest::Permission::RESTART_PACKAGES("android.permission.RESTART_PACKAGES");
    const String Manifest::Permission::RETRIEVE_WINDOW_CONTENT("android.permission.RETRIEVE_WINDOW_CONTENT");
    const String Manifest::Permission::RETRIEVE_WINDOW_INFO("android.permission.RETRIEVE_WINDOW_INFO");
    const String Manifest::Permission::SEND_SMS("android.permission.SEND_SMS");
    const String Manifest::Permission::SEND_SMS_NO_CONFIRMATION("android.permission.SEND_SMS_NO_CONFIRMATION");
    const String Manifest::Permission::SERIAL_PORT("android.permission.SERIAL_PORT");
    const String Manifest::Permission::SET_ACTIVITY_WATCHER("android.permission.SET_ACTIVITY_WATCHER");
    const String Manifest::Permission::SET_ALARM("com.android.alarm.permission.SET_ALARM");
    const String Manifest::Permission::SET_ALWAYS_FINISH("android.permission.SET_ALWAYS_FINISH");
    const String Manifest::Permission::SET_ANIMATION_SCALE("android.permission.SET_ANIMATION_SCALE");
    const String Manifest::Permission::SET_DEBUG_APP("android.permission.SET_DEBUG_APP");
    const String Manifest::Permission::SET_KEYBOARD_LAYOUT("android.permission.SET_KEYBOARD_LAYOUT");
    const String Manifest::Permission::SET_ORIENTATION("android.permission.SET_ORIENTATION");
    const String Manifest::Permission::SET_POINTER_SPEED("android.permission.SET_POINTER_SPEED");
    const String Manifest::Permission::SET_PREFERRED_APPLICATIONS("android.permission.SET_PREFERRED_APPLICATIONS");
    const String Manifest::Permission::SET_PROCESS_LIMIT("android.permission.SET_PROCESS_LIMIT");
    const String Manifest::Permission::SET_SCREEN_COMPATIBILITY("android.permission.SET_SCREEN_COMPATIBILITY");
    const String Manifest::Permission::SET_TIME("android.permission.SET_TIME");
    const String Manifest::Permission::SET_TIME_ZONE("android.permission.SET_TIME_ZONE");
    const String Manifest::Permission::SET_WALLPAPER("android.permission.SET_WALLPAPER");
    const String Manifest::Permission::SET_WALLPAPER_COMPONENT("android.permission.SET_WALLPAPER_COMPONENT");
    const String Manifest::Permission::SET_WALLPAPER_HINTS("android.permission.SET_WALLPAPER_HINTS");
    const String Manifest::Permission::SHUTDOWN("android.permission.SHUTDOWN");
    const String Manifest::Permission::SIGNAL_PERSISTENT_PROCESSES("android.permission.SIGNAL_PERSISTENT_PROCESSES");
    const String Manifest::Permission::START_ANY_ACTIVITY("android.permission.START_ANY_ACTIVITY");
    const String Manifest::Permission::STATUS_BAR("android.permission.STATUS_BAR");
    const String Manifest::Permission::STATUS_BAR_SERVICE("android.permission.STATUS_BAR_SERVICE");
    const String Manifest::Permission::STOP_APP_SWITCHES("android.permission.STOP_APP_SWITCHES");
    const String Manifest::Permission::SUBSCRIBED_FEEDS_READ("android.permission.SUBSCRIBED_FEEDS_READ");
    const String Manifest::Permission::SUBSCRIBED_FEEDS_WRITE("android.permission.SUBSCRIBED_FEEDS_WRITE");
    const String Manifest::Permission::SYSTEM_ALERT_WINDOW("android.permission.SYSTEM_ALERT_WINDOW");
    const String Manifest::Permission::TEMPORARY_ENABLE_ACCESSIBILITY("android.permission.TEMPORARY_ENABLE_ACCESSIBILITY");
    const String Manifest::Permission::UPDATE_DEVICE_STATS("android.permission.UPDATE_DEVICE_STATS");
    const String Manifest::Permission::UPDATE_LOCK("android.permission.UPDATE_LOCK");
    const String Manifest::Permission::USE_CREDENTIALS("android.permission.USE_CREDENTIALS");
    const String Manifest::Permission::USE_SIP("android.permission.USE_SIP");
    const String Manifest::Permission::VIBRATE("android.permission.VIBRATE");
    const String Manifest::Permission::WAKE_LOCK("android.permission.WAKE_LOCK");
    const String Manifest::Permission::WRITE_APN_SETTINGS("android.permission.WRITE_APN_SETTINGS");
    const String Manifest::Permission::WRITE_CALENDAR("android.permission.WRITE_CALENDAR");
    const String Manifest::Permission::WRITE_CALL_LOG("android.permission.WRITE_CALL_LOG");
    const String Manifest::Permission::WRITE_CONTACTS("android.permission.WRITE_CONTACTS");
    const String Manifest::Permission::WRITE_DREAM_STATE("android.permission.WRITE_DREAM_STATE");
    const String Manifest::Permission::WRITE_EXTERNAL_STORAGE("android.permission.WRITE_EXTERNAL_STORAGE");
    const String Manifest::Permission::WRITE_GSERVICES("android.permission.WRITE_GSERVICES");
    const String Manifest::Permission::WRITE_HISTORY_BOOKMARKS("com.android.browser.permission.WRITE_HISTORY_BOOKMARKS");
    const String Manifest::Permission::WRITE_MEDIA_STORAGE("android.permission.WRITE_MEDIA_STORAGE");
    const String Manifest::Permission::WRITE_PROFILE("android.permission.WRITE_PROFILE");
    const String Manifest::Permission::WRITE_SECURE_SETTINGS("android.permission.WRITE_SECURE_SETTINGS");
    const String Manifest::Permission::WRITE_SETTINGS("android.permission.WRITE_SETTINGS");
    const String Manifest::Permission::WRITE_SMS("android.permission.WRITE_SMS");
    const String Manifest::Permission::WRITE_SOCIAL_STREAM("android.permission.WRITE_SOCIAL_STREAM");
    const String Manifest::Permission::WRITE_SYNC_SETTINGS("android.permission.WRITE_SYNC_SETTINGS");
    const String Manifest::Permission::WRITE_USER_DICTIONARY("android.permission.WRITE_USER_DICTIONARY");

    // static constant belong to Permission_Group class
    const String Manifest::Permission_Group::ACCOUNTS("android.permission-group.ACCOUNTS");
    const String Manifest::Permission_Group::AFFECTS_BATTERY("android.permission-group.AFFECTS_BATTERY");
    const String Manifest::Permission_Group::APP_INFO("android.permission-group.APP_INFO");
    const String Manifest::Permission_Group::AUDIO_SETTINGS("android.permission-group.AUDIO_SETTINGS");
    const String Manifest::Permission_Group::BLUETOOTH_NETWORK("android.permission-group.BLUETOOTH_NETWORK");
    const String Manifest::Permission_Group::BOOKMARKS("android.permission-group.BOOKMARKS");
    const String Manifest::Permission_Group::CALENDAR("android.permission-group.CALENDAR");
    const String Manifest::Permission_Group::CAMERA("android.permission-group.CAMERA");
    const String Manifest::Permission_Group::COST_MONEY("android.permission-group.COST_MONEY");
    const String Manifest::Permission_Group::DEVELOPMENT_TOOLS("android.permission-group.DEVELOPMENT_TOOLS");
    const String Manifest::Permission_Group::DEVICE_ALARMS("android.permission-group.DEVICE_ALARMS");
    const String Manifest::Permission_Group::DISPLAY("android.permission-group.DISPLAY");
    const String Manifest::Permission_Group::HARDWARE_CONTROLS("android.permission-group.HARDWARE_CONTROLS");
    const String Manifest::Permission_Group::LOCATION("android.permission-group.LOCATION");
    const String Manifest::Permission_Group::MESSAGES("android.permission-group.MESSAGES");
    const String Manifest::Permission_Group::MICROPHONE("android.permission-group.MICROPHONE");
    const String Manifest::Permission_Group::NETWORK("android.permission-group.NETWORK");
    const String Manifest::Permission_Group::PERSONAL_INFO("android.permission-group.PERSONAL_INFO");
    const String Manifest::Permission_Group::PHONE_CALLS("android.permission-group.PHONE_CALLS");
    const String Manifest::Permission_Group::SCREENLOCK("android.permission-group.SCREENLOCK");
    const String Manifest::Permission_Group::SOCIAL_INFO("android.permission-group.SOCIAL_INFO");
    const String Manifest::Permission_Group::STATUS_BAR("android.permission-group.STATUS_BAR");
    const String Manifest::Permission_Group::STORAGE("android.permission-group.STORAGE");
    const String Manifest::Permission_Group::SYNC_SETTINGS("android.permission-group.SYNC_SETTINGS");
    const String Manifest::Permission_Group::SYSTEM_CLOCK("android.permission-group.SYSTEM_CLOCK");
    const String Manifest::Permission_Group::SYSTEM_TOOLS("android.permission-group.SYSTEM_TOOLS");
    const String Manifest::Permission_Group::USER_DICTIONARY("android.permission-group.USER_DICTIONARY");
    const String Manifest::Permission_Group::VOICEMAIL("android.permission-group.VOICEMAIL");
    const String Manifest::Permission_Group::WALLPAPER("android.permission-group.WALLPAPER");
    const String Manifest::Permission_Group::WRITE_USER_DICTIONARY("android.permission-group.WRITE_USER_DICTIONARY");
};
};