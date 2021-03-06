
#ifndef __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_CRYPTO_CIPHERFACTORY_H__
#define __ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_CRYPTO_CIPHERFACTORY_H__

// import android.os.AsyncTask;
// import android.os.Bundle;
// import android.util.Log;

// import java.security.GeneralSecurityException;
// import java.security.Key;
// import java.security.SecureRandom;
// import java.util.Arrays;
// import java.util.concurrent.Callable;
// import java.util.concurrent.ExecutionException;
// import java.util.concurrent.FutureTask;

// import javax.annotation.concurrent.ThreadSafe;
// import javax.crypto.Cipher;
// import javax.crypto.KeyGenerator;
// import javax.crypto.spec.IvParameterSpec;
// import javax.crypto.spec.SecretKeySpec;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {
namespace Crypto {

/**
 * Generates {@link Cipher} instances for encrypting session data that is temporarily stored.
 *
 * When an Activity is sent to the background, Android gives it the opportunity to save state to
 * restore a user's session when the Activity is restarted. In addition to saving state to disk,
 * Android has a mechanism for saving instance state through {@link Bundle}s, which help
 * differentiate between users pausing and ending a session:
 * - If the Activity is killed in the background (e.g. to free up resources for other Activities),
 *   Android gives a {@link Bundle} to the Activity when the user restarts the Activity. The
 *   {@link Bundle} is expected to be small and fast to generate, and is managed by Android.
 * - If the Activity was explicitly killed (e.g. the user swiped away the task from Recent Tasks),
 *   Android does not restore the {@link Bundle} when the user restarts the Activity.
 *
 * To securely save temporary session data to disk:
 * - Encrypt data with a {@link Cipher} from {@link CipherFactory#getCipher(int)} before storing it.
 * - Store {@link Cipher} parameters in the Bundle via {@link CipherFactory#saveToBundle(Bundle)}.
 *
 * Explicitly ending the session destroys the {@link Bundle}, making the previous session's data
 * unreadable.
 */
//@ThreadSafe
class CipherFactory
{
private:
    /** Holds intermediate data for the computation. */
    class CipherData
    {
    public:
        CipherData(
            /* [in] */ IKey* key,
            /* [in] */ ArrayOf<Byte>* iv);

    public:
        const AutoPtr<IKey> key;
        const AutoPtr< ArrayOf<Byte> > iv;
    };

    /** Singleton holder for the class. */
    class LazyHolder
    {
    private:
        static AutoPtr<CipherFactory> sInstance;
    };

public:
    static const Int32 NUM_BYTES = 16;

    static const String BUNDLE_IV;
    static const String BUNDLE_KEY;

public:
    /** @return The Singleton instance. Creates it if it doesn't exist. */
    static CARAPI_(AutoPtr<CipherFactory>) GetInstance();

    /**
     * Creates a secure Cipher for encrypting data.
     * This function blocks until data needed to generate a Cipher has been created by the
     * background thread.
     * @param opmode One of Cipher.{ENCRYPT,DECRYPT}_MODE.
     * @return A Cipher, or null if it is not possible to instantiate one.
     */
    CARAPI_(AutoPtr<ICipher>) GetCipher(
        /* [in] */ Int32 opmode);

    /**
     * Returns data required for generating the Cipher.
     * @param generateIfNeeded Generates data on the background thread, blocking until it is done.
     * @return Data to use for the Cipher, null if it couldn't be generated.
     */
    CARAPI_(AutoPtr<CipherData>) GetCipherData(
        /* [in] */ Boolean generateIfNeeded);

    /**
     * Generates the encryption key and IV on a background thread (if necessary).
     * Should be explicitly called when the Activity determines that it will need a Cipher rather
     * than immediately calling {@link CipherFactory#getCipher(int)}.
     */
    CARAPI_(void) TriggerKeyGeneration();

    /**
     * Saves the encryption data in a bundle. Expected to be called when an Activity saves its state
     * before being sent to the background.
     *
     * The IV *could* go into the first block of the payload. However, since the staleness of the
     * data is determined by whether or not it's able to be decrypted, the IV should not be read
     * from it.
     *
     * @param outState The data bundle to store data into.
     */
    CARAPI_(void) SaveToBundle(
        /* [in] */ IBundle* outState);

    /**
     * Restores the encryption key from the given Bundle. Expected to be called when an Activity is
     * being restored after being killed in the background. If the Activity was explicitly killed by
     * the user, Android gives no Bundle (and therefore no key).
     *
     * @param savedInstanceState Bundle containing the Activity's previous state. Null if the user
     *                           explicitly killed the Activity.
     * @return                   True if the data was restored successfully from the Bundle, or if
     *                           the CipherData in use matches the Bundle contents.
     *
     */
    CARAPI_(Boolean) RestoreFromBundle(
        /* [in] */ IBundle* savedInstanceState);

    /**
     * Overrides the random number generated that is normally used by the class.
     * @param mockProvider Should be used to provide non-random data.
     */
    CARAPI_(void) SetRandomNumberProviderForTests(
        /* [in] */ ByteArrayGenerator* mockProvider);

private:
    CipherFactory();

    /**
     * Creates a Callable that generates the data required to create a Cipher. This is done on a
     * background thread to prevent blocking on the I/O required for
     * {@link ByteArrayGenerator#getBytes(int)}.
     * @return Callable that generates the Cipher data.
     */
    CARAPI_(Callable<CipherData>) CreateGeneratorCallable();

private:
    static const String TAG;

    /**
     * Synchronization primitive to prevent thrashing the cipher parameters between threads
     * attempting to restore previous parameters and generate new ones.
     */
    const Object mDataLock;

    /** Used to generate data needed for the Cipher on a background thread. */
    FutureTask<CipherData> mDataGenerator;

    /** Holds data for cipher generation. */
    AutoPtr<CipherData> mData;

    /** Generates random data for the Ciphers. May be swapped out for tests. */
    AutoPtr<ByteArrayGenerator> mRandomNumberProvider;
};

} // namespace Crypto
} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_CONTENT_BROWSER_CRYPTO_CIPHERFACTORY_H__
