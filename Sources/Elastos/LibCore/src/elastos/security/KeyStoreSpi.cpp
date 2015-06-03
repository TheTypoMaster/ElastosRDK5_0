
#include "KeyStoreSpi.h"
#include "CSecurity.h"
#include "CKeyStoreSecretKeyEntry.h"
#include "CKeyStorePrivateKeyEntry.h"
#include "CKeyStoreTrustedCertificateEntry.h"

namespace Elastos {
namespace Security {

ECode KeyStoreSpi::EngineStoreEx(
    /* [in] */ IKeyStoreLoadStoreParameter *param)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode KeyStoreSpi::EngineLoadEx(
    /* [in] */ IKeyStoreLoadStoreParameter *param)
{
    if (param == NULL) {
        return EngineLoad(NULL, NULL);
    }
    AutoPtr<ArrayOf<Char32> > pwd;
    AutoPtr<IKeyStoreProtectionParameter> pp;
    param->GetProtectionParameter((IKeyStoreProtectionParameter**)&pp);
    if (IKeyStorePasswordProtection::Probe(pp.Get())) {
        IKeyStorePasswordProtection::Probe(pp.Get())->GetPassword((ArrayOf<Char32>**)&pwd);
        ECode ec;
        FAIL_GOTO(ec = EngineLoad(NULL, pwd), ERROR_PROCESS1)
ERROR_PROCESS1:
        if (ec == E_ILLEGAL_STATE_EXCEPTION) {
            ec = E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        return ec;
    }
    if (IKeyStoreCallbackHandlerProtection::Probe(pp.Get())) {
        ECode ec;
        FAIL_GOTO(ec = GetPasswordFromCallBack(pp, (ArrayOf<Char32>**)&pwd), ERROR_PROCESS2)
        return EngineLoad(NULL, pwd);
ERROR_PROCESS2:
        if (ec == E_UNRECOVERABLE_ENTRY_EXCEPTION) {
            ec = E_ILLEGAL_ARGUMENT_EXCEPTION;
        }
        return ec;
    }
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode KeyStoreSpi::EngineGetEntry(
    /* [in] */ const String& alias,
    /* [in] */ IKeyStoreProtectionParameter *protParam,
    /* [out] */ IKeyStoreEntry **entry)
{
    VALIDATE_NOT_NULL(entry)
    Boolean tag;
    if ((EngineContainsAlias(alias, &tag), !tag)) {
        return NOERROR;
    }
    if ((EngineIsCertificateEntry(alias, &tag), tag)) {
        AutoPtr<Elastos::Security::Cert::ICertificate> cert;
        EngineGetCertificate(alias, (Elastos::Security::Cert::ICertificate**)&cert);
        AutoPtr<IKeyStoreTrustedCertificateEntry> tce;
        CKeyStoreTrustedCertificateEntry::New(cert, (IKeyStoreTrustedCertificateEntry**)&tce);
        *entry = tce.Get();
        REFCOUNT_ADD(*entry)
        return NOERROR;
    }
    AutoPtr<ArrayOf<Char32> > passW;
    if (protParam) {
        if (IKeyStorePasswordProtection::Probe(protParam)) {
            ECode ec;
            FAIL_GOTO(ec = IKeyStorePasswordProtection::Probe(protParam)->GetPassword((ArrayOf<Char32>**)&passW), ERROR_PROCESS)
ERROR_PROCESS:
            if (ec == E_ILLEGAL_STATE_EXCEPTION) {
                ec = E_KEY_STORE_EXCEPTION;
            }
            FAIL_RETURN(ec)
        }
        else if (IKeyStoreCallbackHandlerProtection::Probe(protParam)) {
            GetPasswordFromCallBack(protParam, ((ArrayOf<Char32>**)&passW));
        }
        else {
            return E_UNRECOVERABLE_ENTRY_EXCEPTION;
        }
    }
    EngineIsKeyEntry(alias, &tag);
    if (tag) {
        AutoPtr<IKey> key;
        EngineGetKey(alias, passW, (IKey**)&key);
        if (IPrivateKey::Probe(key.Get())) {
            AutoPtr<ArrayOf<Elastos::Security::Cert::ICertificate*> > cert;
            EngineGetCertificateChain(alias, (ArrayOf<Elastos::Security::Cert::ICertificate*>**)&cert);
            AutoPtr<IKeyStorePrivateKeyEntry> pke;
            CKeyStorePrivateKeyEntry::New(IPrivateKey::Probe(key.Get()), cert, (IKeyStorePrivateKeyEntry**)&pke);
            *entry = pke.Get();
            REFCOUNT_ADD(*entry)
            return NOERROR;
        }
        if (ISecretKey::Probe(key.Get())) {
            AutoPtr<IKeyStoreSecretKeyEntry> ske;
            CKeyStoreSecretKeyEntry::New(ISecretKey::Probe(key.Get()), (IKeyStoreSecretKeyEntry**)&ske);
            *entry = ske.Get();
            REFCOUNT_ADD(*entry)
            return NOERROR;
        }
    }
    return E_NO_SUCH_ALGORITHM_EXCEPTION;
}

ECode KeyStoreSpi::EngineSetEntry(
    /* [in] */ const String& alias,
    /* [in] */ IKeyStoreEntry *entry,
    /* [in] */ IKeyStoreProtectionParameter *protParam)
{
    if (entry == NULL) {
        return E_KEY_STORE_EXCEPTION;
    }

    Boolean tag;
    EngineContainsAlias(alias, &tag);
    if (tag) {
        EngineDeleteEntry(alias);
    }

    if (IKeyStoreTrustedCertificateEntry::Probe(entry)) {
        AutoPtr<IKeyStoreTrustedCertificateEntry> trE = IKeyStoreTrustedCertificateEntry::Probe(entry);
        AutoPtr<Elastos::Security::Cert::ICertificate> tc;
        trE->GetTrustedCertificate((Elastos::Security::Cert::ICertificate**)&tc);
        return EngineSetCertificateEntry(alias, tc);
    }

    AutoPtr<ArrayOf<Char32> > passW;
    if (protParam) {
        if (IKeyStorePasswordProtection::Probe(protParam)) {
            ECode ec;
            FAIL_GOTO(ec = IKeyStorePasswordProtection::Probe(protParam)->GetPassword((ArrayOf<Char32>**)&passW), ERROR_PROCESS)
ERROR_PROCESS:
            if (ec == E_ILLEGAL_STATE_EXCEPTION) {
                ec = E_KEY_STORE_EXCEPTION;
            }
            FAIL_RETURN(ec)
        }
        else if (IKeyStoreCallbackHandlerProtection::Probe(protParam)) {
            if (FAILED(GetPasswordFromCallBack(protParam, ((ArrayOf<Char32>**)&passW)))) {
                return E_KEY_STORE_EXCEPTION;
            }
        }
        else {
            return E_KEY_STORE_EXCEPTION;
        }
    }

    if (IKeyStorePrivateKeyEntry::Probe(entry)) {
        AutoPtr<IKeyStorePrivateKeyEntry> prE = entry;
        AutoPtr<ArrayOf<Elastos::Security::Cert::ICertificate*> > cert;
        prE->GetCertificateChain((ArrayOf<Elastos::Security::Cert::ICertificate*>**)&cert);
        AutoPtr<IPrivateKey> pk;
        prE->GetPrivateKey((IPrivateKey**)&pk);
        return EngineSetKeyEntry(alias, pk, passW, cert);
    }

    if (IKeyStoreSecretKeyEntry::Probe(entry)) {
        AutoPtr<IKeyStoreSecretKeyEntry> sKE = entry;
        AutoPtr<ISecretKey> sk;
        sKE->GetSecretKey((ISecretKey**)&sk);
        return EngineSetKeyEntry(alias, sk, passW, NULL);
    }

    return E_KEY_STORE_EXCEPTION;
}

ECode KeyStoreSpi::EngineEntryInstanceOf(
    /* [in] */ const String& alias,
    /* [in] */ const ClassID& entryClass,
    /* [out] */ Boolean *result)
{
    /*
    if (!engineContainsAlias(alias)) {
        return false;
    }

    try {
        if (engineIsCertificateEntry(alias)) {
            return entryClass
                    .isAssignableFrom(Class
                            .forName("java.security.KeyStore$TrustedCertificateEntry"));
        }

        if (engineIsKeyEntry(alias)) {
            if (entryClass.isAssignableFrom(Class
                    .forName("java.security.KeyStore$PrivateKeyEntry"))) {
                return engineGetCertificate(alias) != null;
            }

            if (entryClass.isAssignableFrom(Class
                    .forName("java.security.KeyStore$SecretKeyEntry"))) {
                return engineGetCertificate(alias) == null;
            }
        }
    } catch (ClassNotFoundException ignore) {}

    return false;
    */
    return E_NOT_IMPLEMENTED;
}

ECode KeyStoreSpi::GetPasswordFromCallBack(
    /* [in] */ IKeyStoreProtectionParameter *protParam,
    /* [out, callee] */ ArrayOf<Char32> **pwd)
{
    VALIDATE_NOT_NULL(pwd)
    if (protParam == NULL) {
        return NOERROR;
    }

    if (!IKeyStoreCallbackHandlerProtection::Probe(protParam)) {
        return E_UNRECOVERABLE_ENTRY_EXCEPTION;
    }

    String clName;
    AutoPtr<ISecurity> security;
    CSecurity::AcquireSingleton((ISecurity**)&security);
    security->GetProperty(String("auth.login.defaultCallbackHandler"), &clName);
    if (clName.IsNull()) {
        return E_UNRECOVERABLE_ENTRY_EXCEPTION;
    }

/*
    try {
        Class<?> cl = Class.forName(clName);
        CallbackHandler cbHand = (CallbackHandler) cl.newInstance();
        PasswordCallback[] pwCb = { new PasswordCallback("password: ", true) };
        cbHand.handle(pwCb);
        return pwCb[0].getPassword();
    } catch (Exception e) {
        throw new UnrecoverableEntryException(e.toString());
    }
*/
    return E_NOT_IMPLEMENTED;

}

} // namespace Security
} // namespace Elastos
