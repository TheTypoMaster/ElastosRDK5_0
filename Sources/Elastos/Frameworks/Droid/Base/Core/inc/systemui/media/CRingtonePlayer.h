#ifndef __CRINGTONEPLAYER_H__
#define __CRINGTONEPLAYER_H__

#include "_CRingtonePlayer.h"
#include "SystemUI.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Core::Mutex;
using Elastos::Droid::Os::IBinder;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Media::IRingtone;
using Elastos::Droid::Media::IIRingtonePlayer;
using Elastos::Droid::Media::IIAudioService;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IConfiguration;

namespace Elastos{
namespace Droid{
namespace SystemUI{
namespace Media{

CarClass(CRingtonePlayer), public SystemUI
{
private:
    class Client
        : public IProxyDeathRecipient
        , public ElRefBase
    {
    public:
        Client(
            /* [in] */ IBinder* token,
            /* [in] */ IUri* uri,
            /* [in] */ IUserHandle* user,
            /* [in] */ Int32 streamType,
            /* [in] */ CRingtonePlayer* host);

        CAR_INTERFACE_DECL();

        ProxyDied();

    public:
        AutoPtr<IBinder> mToken;
        AutoPtr<IRingtone> mRingtone;
        Mutex mLock;
        CRingtonePlayer* mHost;
    };

    class MyRingtonePlayer
        : public IIRingtonePlayer
        , public ElRefBase
    {
    public:
        MyRingtonePlayer(
            /* [in] */ CRingtonePlayer* host)
            : mHost(host)
        {}

        CAR_INTERFACE_DECL();

        Play(
            /* [in] */ IBinder* token,
            /* [in] */ IUri* uri,
            /* [in] */ Int32 streamType);

        Stop(
            /* [in] */ IBinder* token);

        IsPlaying(
            /* [in] */ IBinder* token,
            /* [out] */ Boolean* result);

        /** Used for Notification sound playback. */
        PlayAsync(
            /* [in] */ IUri* uri,
            /* [in] */ IUserHandle* user,
            /* [in] */ Boolean looping,
            /* [in] */ Int32 streamType);

        StopAsync();

    private:
        CRingtonePlayer* mHost;
        Mutex mLock;
    };

public:
    CRingtonePlayer();

    CARAPI constructor();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI Start();

    CARAPI SetContext(
        /* [in] */ IContext* context);

    CARAPI GetContext(
        /* [out]*/ IContext** context);

    CARAPI OnConfigurationChanged(
        /* [in] */ IConfiguration* newConfig);

private:
    CARAPI_(void) GetContextForUser(
        /* [in] */ IUserHandle* user,
        /* [out]*/ IContext** context);

private:
    static const String TAG; // = "RingtonePlayer";
    static const Boolean LOGD; // = false;

    // TODO: support Uri switching under same IBinder

    AutoPtr<IIAudioService> mAudioService;

    // AutoPtr<INotificationPlayer> mAsyncPlayer; // = new NotificationPlayer(TAG);
    HashMap<AutoPtr<IBinder>, AutoPtr<Client> > mClients; // = Maps.newHashMap();

    AutoPtr<MyRingtonePlayer> mCallback;

};

} // namespace Media
} // namespace SystemUI
} // namespace Droid
} // namespace Elastos

#endif // __CRINGTONEPLAYER_H__
