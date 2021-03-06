
#include "ext/frameworkext.h"
#include "gesture/CGestureStore.h"
#include "gesture/GestureUtils.h"
#include "gesture/GestureConstants.h"
#include "gesture/InstanceLearner.h"
#include "gesture/Instance.h"
#include "os/SystemClock.h"
#include <elastos/utility/etl/List.h>
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::ICharSequence;
using Elastos::Core::CObjectContainer;
using Elastos::Core::CString;
using Elastos::Core::StringUtils;
using Elastos::IO::ICloseable;
using Elastos::IO::IDataInput;
using Elastos::IO::IDataOutput;
using Elastos::IO::CBufferedOutputStream;
using Elastos::IO::IBufferedOutputStream;
using Elastos::IO::IBufferedInputStream;
using Elastos::IO::CBufferedInputStream;
using Elastos::IO::EIID_IBufferedInputStream;
using Elastos::IO::CDataInputStream;
using Elastos::IO::CDataOutputStream;
using Elastos::IO::IDataOutputStream;
using Elastos::IO::IFlushable;
using Elastos::Utility::Logging::Logger;
// using Elastos::Droid::Gesture::IGestureStore;
using Elastos::Droid::Os::SystemClock;

namespace Elastos {
namespace Droid {
namespace Gesture {

const Int16 CGestureStore::FILE_FORMAT_VERSION = 1;

const Boolean CGestureStore::PROFILE_LOADING_SAVING = FALSE;

CGestureStore::CGestureStore()
{
    mSequenceType =  IGestureStore::SEQUENCE_SENSITIVE;
    mOrientationStyle = IGestureStore::ORIENTATION_SENSITIVE;
    mChanged = FALSE;
}

ECode CGestureStore::constructor()
{
    mClassifier = new InstanceLearner();
    return NOERROR;
}

ECode CGestureStore::SetOrientationStyle(
    /* [in] */ Int32 style)
{
    mOrientationStyle = style;
    return NOERROR;
}

ECode CGestureStore::GetOrientationStyle(
    /* [out] */ Int32 *style)
{
    VALIDATE_NOT_NULL(style);
    *style = mOrientationStyle;
    return NOERROR;
}

ECode CGestureStore::SetSequenceType(
    /* [in] */ Int32 type)
{
    mSequenceType = type;
    return NOERROR;
}

ECode CGestureStore::GetSequenceType(
    /* [out] */ Int32 *type)
{
    VALIDATE_NOT_NULL(type);
    *type = mSequenceType;
    return NOERROR;
}

ECode CGestureStore::GetGestureEntries(
    /* [out] */ IObjectContainer **entries)
{
    VALIDATE_NOT_NULL(entries);

    AutoPtr<IObjectContainer> container;
    HashMap<String, AutoPtr<GestureList> >::Iterator iter = mNamedGestures.Begin();
    for (; iter != mNamedGestures.End(); ++iter) {
        AutoPtr<ICharSequence> keystr = NULL;
        CString::New(iter->mFirst, (ICharSequence**)&keystr);
        container->Add(keystr);

    }
    *entries = container;
    REFCOUNT_ADD(*entries);
    return NOERROR;
}

ECode CGestureStore::Recognize(
    /* [in] */ IGesture *gesture,
    /* [out] */ IArrayList** list)
{
    VALIDATE_NOT_NULL(list);

    AutoPtr<Instance> instance = Instance::CreateInstance(mSequenceType, mOrientationStyle,
        gesture, String(NULL));
    *list = mClassifier->Classify(mSequenceType, mOrientationStyle, instance->mVector);
    REFCOUNT_ADD(*list);
    return NOERROR;
}

ECode CGestureStore::AddGesture(
    /* [in] */ const String& entryName,
    /* [in] */ IGesture *gesture)
{
    if (entryName.IsNull() || entryName.GetLength() == 0) {
        return NOERROR;
    }

    AutoPtr<GestureList> gestures;
    HashMap<String, AutoPtr<GestureList> >::Iterator iter;
    for (iter = mNamedGestures.Begin(); iter != mNamedGestures.End(); ++iter) {
        if ((iter->mFirst).Equals(entryName) ) {
            gestures = iter->mSecond;
        }
    }

    if (gestures == NULL) {
        gestures = new List<AutoPtr<IGesture> >();
        mNamedGestures[entryName] = gestures;
    }
    gestures->PushBack(gesture);
    AutoPtr<Instance> instance = Instance::CreateInstance(mSequenceType, mOrientationStyle,
        gesture, entryName);
    mClassifier->AddInstance(instance);
    mChanged = TRUE;

    return NOERROR;
}

ECode CGestureStore::RemoveGesture(
    /* [in] */  const String& entryName,
    /* [in] */  IGesture *gesture)
{
    AutoPtr<GestureList> gestures;
    HashMap<String, AutoPtr<GestureList> >::Iterator iter;
    for (iter = mNamedGestures.Begin(); iter != mNamedGestures.End(); ++iter) {
        if ((iter->mFirst).Equals(entryName) ) {
            gestures = iter->mSecond;
        }
    }
    if (gestures == NULL) {
        return NOERROR;
    }

    gestures->Remove(gesture);

    // if there are no more samples, remove the entry automatically
    if (gestures->IsEmpty()) {
        mNamedGestures.Erase(entryName);
    }
    Int64 id;
    gesture->GetID(&id);
    mClassifier->RemoveInstance(id);

    mChanged = TRUE;
    return NOERROR;
}

ECode CGestureStore::RemoveEntry(
    /* [in] */ const String& entryName)
{
    mNamedGestures.Erase(entryName);
    mClassifier->RemoveInstances(entryName);
    mChanged = TRUE;
    return NOERROR;
}

ECode CGestureStore::GetGestures(
    /* [in] */ const String& entryName,
    /* [out] */ IObjectContainer** value)
{
    VALIDATE_NOT_NULL(value);
    *value = NULL;

    AutoPtr<GestureList> gestures;
    HashMap<String, AutoPtr<GestureList> >::Iterator iter = mNamedGestures.Find(entryName);
    if (iter != mNamedGestures.End()) gestures = iter->mSecond;

    GestureList::Iterator it;
    if (gestures != NULL) {
        CObjectContainer::New(value);
        for (it = gestures->Begin(); it != gestures->End(); ++it) {
            (*value)->Add(*it);
        }
    }
    return NOERROR;
}

ECode CGestureStore::HasChanged(
    /* [out] */  Boolean *hasChanged)
{
    VALIDATE_NOT_NULL(hasChanged);
    *hasChanged = mChanged;
    return NOERROR;
}

ECode CGestureStore::Save(
    /* [in] */ IOutputStream *stream)
{
    return Save(stream, FALSE);
}

ECode CGestureStore::Save(
    /* [in] */ IOutputStream *stream,
    /* [in] */ Boolean closeStream)
{
    AutoPtr<IDataOutputStream> out;

    //try {
    Int64 start;
    if (PROFILE_LOADING_SAVING) {
        start = SystemClock::GetElapsedRealtime();
    }

    HashMap<String, AutoPtr<GestureList> >& maps = mNamedGestures;

    if (IBufferedOutputStream::Probe(stream) != NULL) {
        CDataOutputStream::New(stream, (IDataOutputStream**)&out);
    }
    else {
        AutoPtr<IBufferedOutputStream> bos;
        CBufferedOutputStream::New(stream,
            GestureConstants::IO_BUFFER_SIZE, (IBufferedOutputStream**)&bos);
        CDataOutputStream::New(bos, (IDataOutputStream**)&out);
    }
    // Write version number
    IDataOutput::Probe(out)->WriteInt16(FILE_FORMAT_VERSION);
    // Write number of entries
    IDataOutput::Probe(out)->WriteInt32(maps.GetSize());

    HashMap<String, AutoPtr<GestureList> >::Iterator iter = maps.Begin();
    for (; iter != maps.End(); ++iter) {
        const String key = iter->mFirst;
        AutoPtr<List<AutoPtr<IGesture> > > examples = iter->mSecond;
        const Int32 count = examples->GetSize();

        // Write entry name
        IDataOutput::Probe(out)->WriteUTF(key);
        // Write number of examples for this entry
        IDataOutput::Probe(out)->WriteInt32(count);

        List<AutoPtr<IGesture> >::Iterator itList = examples->Begin();
        for (; itList != examples->End(); ++itList) {
            (*itList)->Serialize(out);
        }
    }

    IFlushable::Probe(out)->Flush();

    if (PROFILE_LOADING_SAVING) {
        Int64 end = SystemClock::GetElapsedRealtime();
        Int64 diff= end - start;
        String tmp = StringUtils::ToString(diff);
        String log = String("Saving gestures library = ") + tmp + String(" ms");
        Logger::D(GestureConstants::LOG_TAG, log.string());
    }

    mChanged = FALSE;
    //} finally {
    if (closeStream) GestureUtils::CloseStream(ICloseable::Probe(out));
    //}

    return NOERROR;
}

ECode CGestureStore::Load(
    /* [in] */ IInputStream *stream)
{
    return Load(stream, FALSE);
}

ECode CGestureStore::Load(
    /* [in] */ IInputStream *stream,
    /* [in] */ Boolean closeStream)
{
    AutoPtr<IDataInputStream> in = NULL;
    //try {

    if (stream->Probe(EIID_IBufferedInputStream) != NULL) {
        CDataInputStream::New(stream, (IDataInputStream**)&in);
    } else {
        AutoPtr<IBufferedInputStream> bis;
        CBufferedInputStream::New(stream,
            GestureConstants::IO_BUFFER_SIZE, (IBufferedInputStream**)&bis);
        CDataInputStream::New(bis, (IDataInputStream**)&in);
    }

    Int64 start;
    if (PROFILE_LOADING_SAVING) {
        start = SystemClock::GetElapsedRealtime();
    }

    // Read file format version number
    Int16 versionNumber;
    IDataInput::Probe(in)->ReadInt16(&versionNumber);
    switch (versionNumber) {
        case 1:
            ReadFormatV1(in);
            break;
    }

    if (PROFILE_LOADING_SAVING) {
        Int64 end = SystemClock::GetElapsedRealtime();
        Int64 diff= end - start;
        String tmp = StringUtils::ToString(diff);
        String log = String("Loading gestures library = ") + tmp + String(" ms");
        Logger::D(GestureConstants::LOG_TAG, log.string());
    }
    //} finally {
    if (closeStream) GestureUtils::CloseStream((ICloseable *)in);
    //}

    return NOERROR;
}

void CGestureStore::ReadFormatV1(
    /* [in] */ IDataInputStream *in)
{
    AutoPtr<Learner> classifier = mClassifier;
    mNamedGestures.Clear();

    // Number of entries in the library
    Int32 entriesCount;
    IDataInput::Probe(in)->ReadInt32(&entriesCount);

    for (Int32 i = 0; i < entriesCount; i++) {
        // Entry name
        String name;
        IDataInput::Probe(in)->ReadUTF(&name);
        // Number of gestures
        Int32 gestureCount;
        IDataInput::Probe(in)->ReadInt32(&gestureCount);

        AutoPtr<GestureList> gestures = new GestureList();
        for (Int32 j = 0; j < gestureCount; j++) {
            AutoPtr<IGesture> gesture;
            CGesture::Deserialize(in, (IGesture**)&gesture);
            gestures->PushBack(gesture);
            AutoPtr<Instance> instance = Instance::CreateInstance(mSequenceType, mOrientationStyle,
                gesture, name);
            classifier->AddInstance(instance);
        }

        mNamedGestures[name] = gestures;
    }
}

AutoPtr<Learner> CGestureStore::GetLearner()
{
    return mClassifier;
}

}//namespace Gesture
}//namespace Droid
}//namespace Elastos
