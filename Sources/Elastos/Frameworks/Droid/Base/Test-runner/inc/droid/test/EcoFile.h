
#ifndef __ECOFILE__H__
#define __ECOFILE__H__

#include <ext/frameworkdef.h>
#include "Elastos.Droid.Test_server.h"

using Elastos::Core::IComparable;
using Elastos::Utility::IEnumeration;

namespace Elastos {
namespace Droid {
namespace Test {

class EcoFile : public ElRefBase
{
public:
    class ClassInfo
        : public ElLightRefBase
        , public IComparable
    {
    public:
        ClassInfo(
            /* [in] */ IClassInfo* classInfo)
            : mClassInfo(classInfo)
        {}

        CAR_INTERFACE_DECL()

        CARAPI CompareTo(
            /* [in] */ IInterface* another,
            /* [out] */ Int32* result);

    public:
        AutoPtr<IClassInfo> mClassInfo;
    };

private:
    class EFEnum
        : public ElLightRefBase
        , public IEnumeration
    {
    public:
        EFEnum(
            /* [in] */ EcoFile* ef);

        CAR_INTERFACE_DECL()

        CARAPI HasMoreElements(
            /* [out] */ Boolean* result);

        CARAPI NextElement(
            /* [out] */ IInterface** element);

    private:
        Int32 mIndex;
        AutoPtr< ArrayOf<ClassInfo*> > mClassInfoList;
    };

public:
    ~EcoFile();

    static CARAPI NewEcoFile(
        /* [in] */ const String& fileName,
        /* [out] */ EcoFile** ecoFile);

    CARAPI_(AutoPtr<IEnumeration>) Entries();

    /**
     * Closes the DEX file.
     * <p>
     * This may not be able to release any resources. If classes from this
     * DEX file are still resident, the DEX file can't be unmapped.
     *
     * @throws IOException
     *             if an I/O error occurs during closing the file, which
     *             normally should not happen
     */
    CARAPI Close();

private:
    EcoFile();

    CARAPI Init(
        /* [in] */ const String& fileName);

    /* return a String array with class names */
    static CARAPI_(AutoPtr< ArrayOf<ClassInfo*> >) GetClassInfoList(
        /* [in] */ IModuleInfo* ecoModule);

    static CARAPI OpenEcoFile(
        /* [in] */ const String& sourceName,
        /* [out] */ IModuleInfo** ecoModule);

private:
    const static String TAG;
    AutoPtr<IModuleInfo> mEcoModule;
    String mFileName;
};

} // namespace Test
} // namespace Droid
} // namespace Elastos

#endif //__ECOFILE__H__
