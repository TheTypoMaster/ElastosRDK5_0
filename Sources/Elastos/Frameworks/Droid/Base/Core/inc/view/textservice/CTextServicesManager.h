
#ifndef __CTEXTSERVICESMANAGER_H__
#define __CTEXTSERVICESMANAGER_H__

namespace Elastos {
namespace Droid {
namespace View {
namespace Textservice {

/**
 * System API to the overall text services, which arbitrates interaction between applications
 * and text services. You can retrieve an instance of this interface with
 * {@link Context#getSystemService(String) Context.getSystemService()}.
 *
 * The user can change the current text services in Settings. And also applications can specify
 * the target text services.
 *
 * <h3>Architecture Overview</h3>
 *
 * <p>There are three primary parties involved in the text services
 * framework (TSF) architecture:</p>
 *
 * <ul>
 * <li> The <strong>text services manager</strong> as expressed by this class
 * is the central point of the system that manages interaction between all
 * other parts.  It is expressed as the client-side API here which exists
 * in each application context and communicates with a global system service
 * that manages the interaction across all processes.
 * <li> A <strong>text service</strong> implements a particular
 * interaction model allowing the client application to retrieve information of text.
 * The system binds to the current text service that is in use, causing it to be created and run.
 * <li> Multiple <strong>client applications</strong> arbitrate with the text service
 * manager for connections to text services.
 * </ul>
 *
 * <h3>Text services sessions</h3>
 * <ul>
 * <li>The <strong>spell checker session</strong> is one of the text services.
 * {@link android.view.textservice.SpellCheckerSession}</li>
 * </ul>
 *
 */
CarClass(CTextServicesManager)
{
public:
    /**
     * Retrieve the global TextServicesManager instance, creating it if it doesn't already exist.
     * @hide
     */
    static CARAPI_(AutoPtr<ITextServicesManager>) GetInstance();

    /**
     * Get a spell checker session for the specified spell checker
     * @param locale the locale for the spell checker. If {@code locale} is null and
     * referToSpellCheckerLanguageSettings is true, the locale specified in Settings will be
     * returned. If {@code locale} is not null and referToSpellCheckerLanguageSettings is true,
     * the locale specified in Settings will be returned only when it is same as {@code locale}.
     * Exceptionally, when referToSpellCheckerLanguageSettings is true and {@code locale} is
     * only language (e.g. "en"), the specified locale in Settings (e.g. "en_US") will be
     * selected.
     * @param listener a spell checker session lister for getting results from a spell checker.
     * @param referToSpellCheckerLanguageSettings if true, the session for one of enabled
     * languages in settings will be returned.
     * @return the spell checker session of the spell checker
     */
    CARAPI NewSpellCheckerSession(
        /* [in] */ IBundle* bundle,
        /* [in] */ ILocale* locale,
        /* [in] */ ISpellCheckerSessionListener* listener,
        /* [in] */ Boolean referToSpellCheckerLanguageSettings,
        /* [out] */ ISpellCheckerSession** session);

    /**
     * @hide
     */
    CARAPI GetEnabledSpellCheckers(
        /* [out] */ ArrayOf<ISpellCheckerInfo*>** infoArray);

    /**
     * @hide
     */
    CARAPI GetCurrentSpellChecker(
        /* [out] */ ISpellCheckerInfo** info);

    /**
     * @hide
     */
    CARAPI SetCurrentSpellChecker(
        /* [in] */ ISpellCheckerInfo* sci);

    /**
     * @hide
     */
    CARAPI GetCurrentSpellCheckerSubtype(
        /* [in] */ Boolean allowImplicitlySelectedSubtype,
        /* [out] */ ISpellCheckerSubtype** subtype);

    /**
     * @hide
     */
    CARAPI SetSpellCheckerSubtype(
        /* [in] */ ISpellCheckerSubtype* subtype);

    /**
     * @hide
     */
    CARAPI SetSpellCheckerEnabled(
        /* [in] */ Boolean enabled);

    /**
     * @hide
     */
    CARAPI IsSpellCheckerEnabled(
        /* [out] */ Boolean* enabled);

//private:
//    TextServicesManager();
    CARAPI constructor();

private:

    static const String TAG;// = TextServicesManager.class.getSimpleName();
    static const Boolean DBG = FALSE;

    static AutoPtr<ITextServicesManager> sInstance;
    static AutoPtr<IITextServicesManager> sService;

    static Mutex sLock;
};

}   //namespace Textservice
}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif //__CTEXTSERVICESMANAGER_H__