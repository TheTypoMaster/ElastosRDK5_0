
#ifndef __ELASTOS_DROID_WEBKIT_MEDIA_MEDIADRMBRIDGE_H__
#define __ELASTOS_DROID_WEBKIT_MEDIA_MEDIADRMBRIDGE_H__

// import android.media.MediaCrypto;
// import android.media.MediaDrm;
// import android.os.AsyncTask;
// import android.os.Build;
// import android.os.Handler;
// import android.util.Log;

// import org.apache.http.HttpResponse;
// import org.apache.http.client.ClientProtocolException;
// import org.apache.http.client.HttpClient;
// import org.apache.http.client.methods.HttpPost;
// import org.apache.http.impl.client.DefaultHttpClient;
// import org.apache.http.util.EntityUtils;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

// import java.io.IOException;
// import java.nio.ByteBuffer;
// import java.nio.ByteOrder;
// import java.util.ArrayDeque;
// import java.util.HashMap;
// import java.util.UUID;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Media {

/**
 * A wrapper of the android MediaDrm class. Each MediaDrmBridge manages multiple
 * sessions for a single MediaSourcePlayer.
 */
//@JNINamespace("media")
class MediaDrmBridge
{
private:
    /**
     *  This class contains data needed to call createSession().
     */
    class PendingCreateSessionData
    {
    private:
        PendingCreateSessionData(
            /* [in] */ Int32 sessionId,
            /* [in] */ ArrayOf<Byte>* initData,
            /* [in] */ String mimeType);

        CARAPI_(Int32) SessionId();

        CARAPI_(AutoPtr< ArrayOf<Byte> >) InitData();

        CARAPI_(String) MimeType();

    private:
        const Int32 mSessionId;
        const AutoPtr< ArrayOf<Byte> > mInitData;
        const String mMimeType;
    };

    class MediaDrmListener
        : public Object
        , public IMediaDrmOnEventListener
    {
    public:
        MediaDrmListener(
            /* [in] */ MediaDrmBridge* owner);

        //@Override
        CARAPI OnEvent(
            /* [in] */ IMediaDrm* mediaDrm,
            /* [in] */ ArrayOf<Byte>* session_array,
            /* [in] */ Int32 event,
            /* [in] */ Int32 extra,
            /* [in] */ ArrayOf<Byte>* data);

    private:
        MediaDrmBridge* mOwner;
    };

    class PostRequestTask : public AsyncTask<String, Void, Void>
    {
    public:
        PostRequestTask(
            /* [in] */ MediaDrmBridge* owner,
            /* [in] */ ArrayOf<Byte>* drmRequest);

    protected:
        //@Override
        CARAPI_(Void) DoInBackground(String... urls);

        //@Override
        CARAPI_(void) OnPostExecute(Void v);

    private:
        CARAPI_(AutoPtr< ArrayOf<Byte> >) PostRequest(
            /* [in] */ String url,
            /* [in] */ ArrayOf<Byte>* drmRequest);

    private:
        static final String TAG;

        MediaDrmBridge* mOwner;

        AutoPtr< ArrayOf<Byte> > mDrmRequest;
        AutoPtr< ArrayOf<Byte> > mResponseBody;
    };

    class ResumePendingOperationsRunnable
        : public Object
        , public IRunnable
    {
    public:
        ResumePendingOperationsRunnable(
            /* [in] */ MediaDrmBridge* owner);

        CARAPI Run();

    private:
        MediaDrmBridge* mOwner;
    };

    class OnSessionCreatedRunnable
        : public Object
        , public IRunnable
    {
    public:
        OnSessionCreatedRunnable(
            /* [in] */ MediaDrmBridge* owner,
            /* [in] */ Int32 sessionId,
            /* [in] */ const String& webSessionId);

        CARAPI Run();

    private:
        MediaDrmBridge* mOwner;
        Int32 mSessionId,
        const String& mWebSessionId
    };

    class OnSessionMessageRunnable
        : public Object
        , public IRunnable
    {
    public:
        OnSessionMessageRunnable(
            /* [in] */ MediaDrmBridge* owner,
            /* [in] */ Int32 sessionId,
            /* [in] */ IMediaDrmKeyRequest* request);

        CARAPI Run();

    private:
        MediaDrmBridge* mOwner;
        Int32 mSessionId,
        AutoPtr<IMediaDrmKeyRequest> mRequest
    };

    class OnSessionReadyRunnable
        : public Object
        , public IRunnable
    {
    public:
        OnSessionReadyRunnable(
            /* [in] */ MediaDrmBridge* owner,
            /* [in] */ Int32 sessionId);

        CARAPI Run();

    private:
        MediaDrmBridge* mOwner;
        Int32 mSessionId
    };

    class OnSessionClosedRunnable
        : public Object
        , public IRunnable
    {
    public:
        OnSessionClosedRunnable(
            /* [in] */ MediaDrmBridge* owner,
            /* [in] */ Int32 sessionId);

        CARAPI Run();

    private:
        MediaDrmBridge* mOwner;
        Int32 mSessionId
    };

    class OnSessionErrorRunnable
        : public Object
        , public IRunnable
    {
    public:
        OnSessionErrorRunnable(
            /* [in] */ MediaDrmBridge* owner,
            /* [in] */ Int32 sessionId);

        CARAPI Run();

    private:
        MediaDrmBridge* mOwner;
        Int32 mSessionId
    };

public:
    /**
     * Provide the provisioning response to MediaDrm.
     * @returns false if the response is invalid or on error, true otherwise.
     */
    CARAPI_(Boolean) ProvideProvisionResponse(
        /* [in] */ ArrayOf<Byte>* response);

    static CARAPI_(void) AddKeySystemUuidMapping(
        /* [in] */ String keySystem,
        /* [in] */ IUUID* uuid);

private:
    MediaDrmBridge(
        /* [in] */ IUUID* schemeUUID,
        /* [in] */ Int64 nativeMediaDrmBridge,
        /* [in] */ Boolean singleSessionMode);

    static CARAPI_(AutoPtr<IUUID>) GetUUIDFromBytes(
        /* [in] */ ArrayOf<Byte>* data);

    /**
     *  Gets session associated with the sessionId.
     *
     *  @return session if sessionId maps a valid opened session. Returns null
     *  otherwise.
     */
    CARAPI_(AutoPtr<IByteBuffer>) GetSession(
        /* [in] */ Int32 sessionId);

    /**
     * Create a MediaCrypto object.
     *
     * @return whether a MediaCrypto object is successfully created.
     */
    CARAPI_(Boolean) CreateMediaCrypto();

    /**
     * Open a new session..
     *
     * @return the session opened. Returns null if unexpected error happened.
     */
    CARAPI_(AutoPtr<IByteBuffer>) OpenSession();

    /**
     * Close a session.
     *
     * @param session to be closed.
     */
    CARAPI_(void) CloseSession(
        /* [in] */ IByteBuffer* session);

    /**
     * Check whether the crypto scheme is supported for the given container.
     * If |containerMimeType| is an empty string, we just return whether
     * the crypto scheme is supported.
     *
     * @return true if the container and the crypto scheme is supported, or
     * false otherwise.
     */
    //@CalledByNative
    static CARAPI_(Boolean) IsCryptoSchemeSupported(
        /* [in] */ ArrayOf<Byte>* schemeUUID,
        /* [in] */ String containerMimeType);

    /**
     * Create a new MediaDrmBridge from the crypto scheme UUID.
     *
     * @param schemeUUID Crypto scheme UUID.
     * @param securityLevel Security level to be used.
     * @param nativeMediaDrmBridge Native object of this class.
     */
    //@CalledByNative
    static CARAPI_(AutoPtr<MediaDrmBridge>) Create(
        /* [in] */ ArrayOf<Byte>* schemeUUID,
        /* [in] */ Int64 nativeMediaDrmBridge);

    /**
     * Set the security level that the MediaDrm object uses.
     * This function should be called right after we construct MediaDrmBridge
     * and before we make any other calls.
     */
    //@CalledByNative
    CARAPI_(Boolean) SetSecurityLevel(
        /* [in] */ String securityLevel);

    /**
     * Return the MediaCrypto object if available.
     */
    //@CalledByNative
    CARAPI_(AutoPtr<IMediaCrypto>) GetMediaCrypto();

    /**
     * Reset the device DRM credentials.
     */
    //@CalledByNative
    CARAPI_(void) ResetDeviceCredentials();

    /**
     * Release the MediaDrmBridge object.
     */
    //@CalledByNative
    CARAPI_(void) Release();

    /**
     * Get a key request.
     *
     * @param session Session on which we need to get the key request.
     AutoPtr<key request.
     * @param mime Mime type to get the key request.
     *
     * @return the key request.
     */
    CARAPI_(AutoPtr<IMediaDrmKeyRequest>) GetKeyRequest(
        /* [in] */ IByteBuffer* session,
        /* [in] */ ArrayOf<Byte>* data,
        /* [in] */ String mime);

    /**
     * Save data to |mPendingCreateSessionDataQueue| so that we can resume the
     * createSession() call later.
     */
    CARAPI_(void) SavePendingCreateSessionData(
        /* [in] */ Int32 sessionId,
        /* [in] */ ArrayOf<Byte>* initData,
        /* [in] */ String mime);

    /**
     * Process all pending createSession() calls synchronously.
     */
    CARAPI_(void) ProcessPendingCreateSessionData();

    /**
     * Process pending operations asynchrnously.
     */
    CARAPI_(void) ResumePendingOperations();

    /**
     * Create a session with |sessionId|, |initData| and |mime|.
     * In multiple session mode, a new session will be open. In single session
     * mode, the mMediaCryptoSession will be used.
     *
     * @param sessionId ID for the session to be created.
     * @param initData Data needed to generate the key request.
     * @param mime Mime type.
     */
    //@CalledByNative
    CARAPI_(void) CreateSession(
        /* [in] */ Int32 sessionId,
        /* [in] */ ArrayOf<Byte>* initData,
        /* [in] */ String mime);

    /**
     * Returns whether |sessionId| is a valid key session, excluding the media
     * crypto session in multi-session mode.
     *
     * @param sessionId Crypto session Id.
     */
    CARAPI_(Boolean) SessionExists(
        /* [in] */ IByteBuffer* session);

    /**
     * Cancel a key request for a session Id.
     *
     * @param sessionId Reference ID of session to be released.
     */
    //@CalledByNative
    CARAPI_(void) ReleaseSession(
        /* [in] */ Int32 sessionId);

    /**
     * Add a key for a session Id.
     *
     * @param sessionId Reference ID of session to be updated.
     * @param key Response data from the server.
     */
    //@CalledByNative
    CARAPI_(void) UpdateSession(
        /* [in] */ Int32 sessionId,
        /* [in] */ ArrayOf<Byte>* key);

    /**
     * Return the security level of this DRM object.
     */
    //@CalledByNative
    CARAPI_(String) GetSecurityLevel();

    CARAPI_(void) StartProvisioning();

    /**
     * Called when the provision response is received.
     *
     * @param response Response data from the provision server.
     */
    CARAPI_(void) OnProvisionResponse(
        /* [in] */ ArrayOf<Byte>* response);

    CARAPI_(void) OnSessionCreated(
        /* [in] */ Int32 sessionId,
        /* [in] */ const String& webSessionId);

    CARAPI_(void) OnSessionMessage(
        /* [in] */ Int32 sessionId,
        /* [in] */ IMediaDrmKeyRequest* request);

    CARAPI_(void) OnSessionReady(
        /* [in] */ Int32 sessionId);

    CARAPI_(void) OnSessionClosed(
        /* [in] */ Int32 sessionId);

    CARAPI_(void) OnSessionError(
        /* [in] */ Int32 sessionId);

    CARAPI_(String) GetWebSessionId(
        /* [in] */ IByteBuffer* session);

    CARAPI_(void) NativeOnMediaCryptoReady(
        /* [in] */ Int64 nativeMediaDrmBridge);

    CARAPI_(void) NativeOnSessionCreated(
        /* [in] */ Int64 nativeMediaDrmBridge,
        /* [in] */ Int32 sessionId,
        /* [in] */ String webSessionId);

    CARAPI_(void) NativeOnSessionMessage(
        /* [in] */ Int64 nativeMediaDrmBridge,
        /* [in] */ Int32 sessionId,
        /* [in] */ ArrayOf<Byte>* message,
        /* [in] */ String destinationUrl);

    CARAPI_(void) NativeOnSessionReady(
        /* [in] */ Int64 nativeMediaDrmBridge,
        /* [in] */ Int32 sessionId);

    CARAPI_(void) NativeOnSessionClosed(
        /* [in] */ Int64 nativeMediaDrmBridge,
        /* [in] */ Int32 sessionId);

    CARAPI_(void) NativeOnSessionError(
        /* [in] */ Int64 nativeMediaDrmBridge,
        /* [in] */ Int32 sessionId);

    CARAPI_(void) NativeOnResetDeviceCredentialsCompleted(
        /* [in] */ Int64 nativeMediaDrmBridge,
        /* [in] */ Boolean success);

    static CARAPI_(void) NativeAddKeySystemUuidMapping(
        /* [in] */ String keySystem,
        /* [in] */ IByteBuffer* uuid);

private:
    // Implementation Notes:
    // - A media crypto session (mMediaCryptoSession) is opened after MediaDrm
    //   is created. This session will be added to mSessionIds.
    //   a) In multiple session mode, this session will only be used to create
    //      the MediaCrypto object. It's associated mime type is always null and
    //      it's session ID is always INVALID_SESSION_ID.
    //   b) In single session mode, this session will be used to create the
    //      MediaCrypto object and will be used to call getKeyRequest() and
    //      manage all keys.  The session ID will always be the lastest session
    //      ID passed by the caller.
    // - Each createSession() call creates a new session. All sessions are
    //   managed in mSessionIds.
    // - Whenever NotProvisionedException is thrown, we will clean up the
    //   current state and start the provisioning process.
    // - When provisioning is finished, we will try to resume suspended
    //   operations:
    //   a) Create the media crypto session if it's not created.
    //   b) Finish createSession() if previous createSession() was interrupted
    //      by a NotProvisionedException.
    // - Whenever an unexpected error occurred, we'll call release() to release
    //   all resources and clear all states. In that case all calls to this
    //   object will be no-op. All public APIs and callbacks should check
    //   mMediaBridge to make sure release() hasn't been called. Also, we call
    //   release() immediately after the error happens (e.g. after mMediaDrm)
    //   calls. Indirect calls should not call release() again to avoid
    //   duplication (even though it doesn't hurt to call release() twice).

    static const String TAG;
    static const String SECURITY_LEVEL;
    static const String PRIVACY_MODE;
    static const String SESSION_SHARING;
    static const String ENABLE;
    static const Int32 INVALID_SESSION_ID = 0;

    AutoPtr<IMediaDrm> mMediaDrm;
    Int64 mNativeMediaDrmBridge;
    AutoPtr<IUUID> mSchemeUUID;
    AutoPtr<IHandler> mHandler;

    // In this mode, we only open one session, i.e. mMediaCryptoSession.
    Boolean mSingleSessionMode;

    // A session only for the purpose of creating a MediaCrypto object.
    // This session is opened when createSession() is called for the first
    // time.
    // - In multiple session mode, all following createSession() calls
    // should create a new session and use it to call getKeyRequest(). No
    // getKeyRequest() should ever be called on this media crypto session.
    // - In single session mode, all createSession() calls use the same
    // media crypto session. When createSession() is called with a new
    // initData, previously added keys may not be available anymore.
    AutoPtr<IByteBuffer> mMediaCryptoSession;
    AutoPtr<IMediaCrypto> mMediaCrypto;

    // The map of all opened sessions to their session reference IDs.
    HashMap<ByteBuffer, Integer> mSessionIds;
    // The map of all opened sessions to their mime types.
    HashMap<ByteBuffer, String> mSessionMimeTypes;

    // The queue of all pending createSession() data.
    ArrayDeque<PendingCreateSessionData> mPendingCreateSessionDataQueue;

    Boolean mResetDeviceCredentialsPending;

    // MediaDrmBridge is waiting for provisioning response from the server.
    //
    // Notes about NotProvisionedException: This exception can be thrown in a
    // lot of cases. To streamline implementation, we do not catch it in private
    // non-native methods and only catch it in public APIs.
    Boolean mProvisioningPending;
};

} // namespace Media
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_MEDIA_MEDIADRMBRIDGE_H__
