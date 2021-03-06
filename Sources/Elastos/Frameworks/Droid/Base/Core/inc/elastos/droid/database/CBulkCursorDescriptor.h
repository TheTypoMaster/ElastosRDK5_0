#ifndef __ELASTOS_DROID_DATABASE_CSQLITEBULKCURSORDESCRIPTOR_H__
#define __ELASTOS_DROID_DATABASE_CSQLITEBULKCURSORDESCRIPTOR_H__

#include "ext/frameworkext.h"
#include "_Elastos_Droid_Database_CBulkCursorDescriptor.h"
#include <elastos/core/Object.h>

using Elastos::Core::Object;

namespace Elastos {
namespace Droid {
namespace Database {

CarClass(CBulkCursorDescriptor)
    , public Object
    , public IParcelable
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CBulkCursorDescriptor();

    //@Override
    CARAPI DescribeContents(
        /* [out] */ Int32* contents);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* writeout);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* readin);

    CARAPI constructor();

public:
    AutoPtr<IBulkCursor> mCursor;
    AutoPtr< ArrayOf<String> > mColumnNames;
    Boolean mWantsAllOnMoveCalls;
    Int32 mCount;
    AutoPtr<ICursorWindow> mWindow;
};

} //Database
} //Droid
} //Elastos
#endif //__ELASTOS_DROID_DATABASE_CSQLITEBULKCURSORDESCRIPTOR_H__
