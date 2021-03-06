
#ifndef __ELASTOS_DROID_TEXT_METHOD_CQWERTYKEYLISTENERHELPER_H__
#define __ELASTOS_DROID_TEXT_METHOD_CQWERTYKEYLISTENERHELPER_H__

#include "_Elastos_Droid_Text_Method_CQwertyKeyListenerHelper.h"

#include "Elastos.Droid.Core_server.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::View::IView;
using Elastos::Droid::View::IKeyEvent;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CQwertyKeyListenerHelper)
{
public:
    IBaseKeyListenerHelper_METHODS_DECL()

    CARAPI GetInstance(
        /* [in] */ Boolean autoText,
        /* [in] */ Capitalize cap,
        /* [out] */ IQwertyKeyListener** ret);

    CARAPI GetInstanceForFullKeyboard(
        /* [out] */ IQwertyKeyListener** ret);

    CARAPI MarkAsReplaced(
        /* [in] */ ISpannable* content,
        /* [in] */ Int32 start,
        /* [in] */ Int32 end,
        /* [in] */ const String& original);

};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_CQWERTYKEYLISTENERHELPER_H__
