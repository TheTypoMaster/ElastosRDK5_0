#ifndef __ELASTOS_DROID_APP_CACITONKEYINFO_H__
#define __ELASTOS_DROID_APP_CACITONKEYINFO_H__

#include "_Elastos_Droid_App_CActionKeyInfo.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
namespace Elastos {
namespace Droid {
namespace App {

CarClass(CActionKeyInfo)
{
public:
    CActionKeyInfo();

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IContext* activityContext,
        /* [in] */ IAttributeSet* attr);

    CARAPI GetKeyCode(
        /* [out] */ Int32* keyCode);

    CARAPI GetQueryActionMsg(
        /* [out] */ String* actionMsg);

    CARAPI GetSuggestActionMsg(
        /* [out] */ String* actionMsg);

    CARAPI GetSuggestActionMsgColumn(
        /* [out] */ String* column);

    CARAPI DescribeContents(
        /* [out] */ Int32* rst);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel *source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel *dest);
private:
    Int32 mKeyCode;
    String mQueryActionMsg;
    String mSuggestActionMsg;
    String mSuggestActionMsgColumn;
};

}// namespace App
}// namespace Droid
}// namespace Elastos

#endif
