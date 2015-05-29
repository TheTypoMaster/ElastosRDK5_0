
#ifndef __CSTRUCTUTSNAME_H__
#define __CSTRUCTUTSNAME_H__

#include "coredef.h"
#include "core/Object.h"
#include "_Elastos_Droid_System_CStructUtsname.h"

using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace System {

/**
 * Information returned by uname(2). Corresponds to C's
 * {@code struct utsname} from
 * <a href="http://pubs.opengroup.org/onlinepubs/9699919799/basedefs/sys_utsname.h.html">&lt;sys/utsname.h&gt;</a>
 */
CarClass(CStructUtsname) , public Object
{
public:
    CAR_OBJECT_DECL()

    CAR_INTERFACE_DECL()

    CARAPI GetSysname(
        /* [out] */ String* sysname);

    CARAPI GetNodename(
        /* [out] */ String* nodename);

    CARAPI GetRelease(
        /* [out] */ String* release);

    CARAPI GetVersion(
        /* [out] */ String* version);

    CARAPI GetMachine(
        /* [out] */ String* machine);

    CARAPI constructor(
        /* [in] */ const String& sysname,
        /* [in] */ const String& nodename,
        /* [in] */ const String& release,
        /* [in] */ const String& version,
        /* [in] */ const String& machine);

private:
    /** The OS name, such as "Linux". */
    String mSysname;

    /** The machine's unqualified name on some implementation-defined network. */
    String mNodename;

    /** The OS release, such as "2.6.35-27-generic". */
    String mRelease;

    /** The OS version, such as "#48-Ubuntu SMP Tue Feb 22 20:25:29 UTC 2011". */
    String mVersion;

    /** The machine architecture, such as "armv7l" or "x86_64". */
    String mMachine;
};

} // namespace System
} // namespace Droid
} // namespace Elastos

#endif // __CSTRUCTUTSNAME_H__
