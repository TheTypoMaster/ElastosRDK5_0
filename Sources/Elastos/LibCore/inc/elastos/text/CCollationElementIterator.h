
#ifndef __ELASTOS_TEXT_CCOLLATIONELEMENTITERATOR_H__
#define __ELASTOS_TEXT_CCOLLATIONELEMENTITERATOR_H__

#include "_Elastos_Text_CCollationElementIterator.h"
#include <elastos/core/Object.h>

using Libcore::ICU::ICollationElementIteratorICU;
using Elastos::Core::Object;
using Elastos::Text::ICharacterIterator;

namespace Elastos {
namespace Text {

CarClass(CCollationElementIterator) , public Object
{
public:
    CARAPI constructor(
        /* [in] */ ICollationElementIteratorICU* iterator);

    CARAPI GetMaxExpansion(
        /* [in] */ Int32 order,
        /* [out] */ Int32* maxExpansion);

    CARAPI GetOffset(
        /* [out] */ Int32* offset);

    CARAPI GetNext(
        /* [out] */ Int32* nextValue);

    CARAPI Previous(
        /* [out] */ Int32* previousValue);

    CARAPI Reset();

    CARAPI SetOffset(
        /* [in] */ Int32 newOffset);

    CARAPI SetText(
        /* [in] */ ICharacterIterator* source);

    CARAPI SetText(
        /* [in] */ const String& source);

private:
    AutoPtr<ICollationElementIteratorICU> mIcuIterator;
};

} // namespace Text
} // namespace Elastos

#endif // __ELASTOS_TEXT_CCOLLATIONELEMENTITERATOR_H__
