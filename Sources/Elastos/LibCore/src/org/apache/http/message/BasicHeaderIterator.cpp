
#include "BasicHeaderIterator.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;

namespace Org {
namespace Apache {
namespace Http {
namespace Message {

BasicHeaderIterator::BasicHeaderIterator()
    : mCurrentIndex(-1)
{}

CAR_INTERFACE_IMPL_3(BasicHeaderIterator, Object, IBasicHeaderIterator, IHeaderIterator, IIterator)

ECode BasicHeaderIterator::Init(
    /* [in] */ ArrayOf<IHeader*>* headers,
    /* [in] */ const String& name)
{
    if (headers == NULL) {
        Logger::E("BasicHeaderIterator", "Header array must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mAllHeaders = headers;
    mHeaderName = name;
    FindNext(-1, &mCurrentIndex);
}

ECode BasicHeaderIterator::FindNext(
    /* [in] */ Int32 from,
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index)
    if (from < -1) {
        *index = -1;
        return NOERROR
    }

    Int32 to = mAllHeaders->GetLength() - 1;
    Boolean found = FALSE;
    while (!found && (from < to)) {
        from++;
        FilterHeader(from, &found);
    }
    *index = found ? from : -1;
    return NOERROR;
}

ECode BasicHeaderIterator::FilterHeader(
    /* [in] */ Int32 index,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    String name;
    *result = (mHeaderName.IsNull()) || ((*mAllHeaders)[index]->GetName(&name), mHeaderName.EqualsIgnoreCase(name));
    return NOERROR;
}

ECode BasicHeaderIterator::HasNext(
    /* [out] */ Boolean* hasNext)
{
    VALIDATE_NOT_NULL(hasNext)
    *hasNext = (mCurrentIndex >= 0);
    return NOERROR;
}

ECode BasicHeaderIterator::NextHeader(
    /* [out] */ IHeader** nextHeader)
{
    VALIDATE_NOT_NULL(nextHeader)
    *nextHeader = NULL;

    Int32 current = mCurrentIndex;
    if (current < 0) {
        Logger::E("BasicHeaderIterator", "Iteration already finished.");
        return E_NO_SUCH_ELEMENT_EXCEPTION;
    }

    FindNext(current, &mCurrentIndex);
    *nextHeader = (*mAllHeaders)[current];
    REFCOUNT_ADD(*nextHeader)
    return NOERROR;
}

ECode BasicHeaderIterator::GetNext(
    /* [out] */ IInterface** object)
{
    VALIDATE_NOT_NULL(object)
    AutoPtr<IHeader> h;
    NextHeader((IHeader**)&h);
    *object = h->Probe(EIID_IInterface);
    REFCOUNT_ADD(*object)
    return NOERROR;
}

ECode BasicHeaderIterator::Remove()
{
    Logger::E("BasicHeaderElementIterator", "Removing headers is not supported.");
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

} // namespace Message
} // namespace Http
} // namespace Apache
} // namespace Org
