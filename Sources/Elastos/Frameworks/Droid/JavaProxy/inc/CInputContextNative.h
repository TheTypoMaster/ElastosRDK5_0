
#ifndef __CINPUTCONTEXTNATIVE_H__
#define __CINPUTCONTEXTNATIVE_H__

#include "_CInputContextNative.h"
#include <jni.h>

using Elastos::Core::ICharSequence;

using Elastos::Droid::Os::IBundle;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::View::InputMethod::ICompletionInfo;
using Elastos::Droid::View::InputMethod::ICorrectionInfo;
using Elastos::Droid::View::InputMethod::IExtractedTextRequest;
using Elastos::Droid::Internal::View::IInputContextCallback;

namespace Elastos {
namespace Droid {
namespace JavaProxy {

CarClass(CInputContextNative)
{
public:
    ~CInputContextNative();

    CARAPI constructor(
        /* [in] */ Handle32 jVM,
        /* [in] */ Handle32 jInstance);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI GetTextBeforeCursor(
        /* [in] */ Int32 length,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 seq,
        /* [in] */ IInputContextCallback* icCallback);

    CARAPI GetTextAfterCursor(
        /* [in] */ Int32 length,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 seq,
        /* [in] */ IInputContextCallback* icCallback);

    CARAPI GetCursorCapsMode(
        /* [in] */ Int32 reqModes,
        /* [in] */ Int32 seq,
        /* [in] */ IInputContextCallback* icCallback);

    CARAPI GetExtractedText(
        /* [in] */ IExtractedTextRequest* request,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 seq,
        /* [in] */ IInputContextCallback* icCallback);

    CARAPI DeleteSurroundingText(
        /* [in] */ Int32 leftLength,
        /* [in] */ Int32 rightLength);

    CARAPI SetComposingText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 newCursorPosition);

    CARAPI FinishComposingText();

    CARAPI CommitText(
        /* [in] */ ICharSequence* text,
        /* [in] */ Int32 newCursorPosition);

    CARAPI CommitCompletion(
        /* [in] */ ICompletionInfo* completion);

    CARAPI CommitCorrection(
        /* [in] */ ICorrectionInfo* correction);

    CARAPI SetSelection(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI PerformEditorAction(
        /* [in] */ Int32 actionCode);

    CARAPI PerformContextMenuAction(
        /* [in] */ Int32 id);

    CARAPI BeginBatchEdit();

    CARAPI EndBatchEdit();

    CARAPI ReportFullscreenMode(
        /* [in] */ Boolean enabled);

    CARAPI SendKeyEvent(
        /* [in] */ IKeyEvent* event);

    CARAPI ClearMetaKeyStates(
        /* [in] */ Int32 states);

    CARAPI PerformPrivateCommand(
        /* [in] */ const String& action,
        /* [in] */ IBundle* data);

    CARAPI SetComposingRegion(
        /* [in] */ Int32 start,
        /* [in] */ Int32 end);

    CARAPI GetSelectedText(
        /* [in] */ Int32 flags,
        /* [in] */ Int32 seq,
        /* [in] */ IInputContextCallback* icCallback);

private:
    static const String TAG;

    JavaVM* mJVM;
    jobject mJInstance;
};

}
}
}

#endif // __CINPUTCONTEXTNATIVE_H__
