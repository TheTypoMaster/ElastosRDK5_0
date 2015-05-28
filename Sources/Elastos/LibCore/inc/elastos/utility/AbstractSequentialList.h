#ifndef __UTILITY_ABSTRACTSEQUENTIALLIST_H__
#define __UTILITY_ABSTRACTSEQUENTIALLIST_H__

#include "AbstractList.h"

namespace Elastos {
namespace Utility {

class AbstractSequentialList : public AbstractList
{
public:
    CARAPI Add(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* object);

    CARAPI AddAll(
        /* [in] */ Int32 location,
        /* [in] */ ICollection* collection,
        /* [out] */ Boolean* result);

    CARAPI Get(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** object);

    CARAPI GetIterator(
        /* [out] */ IIterator** result);

    virtual CARAPI GetListIterator(
        /* [in] */ Int32 location,
        /* [out] */ IListIterator** it) = 0;

    CARAPI Remove(
        /* [in] */ Int32 location,
        /* [out] */ IInterface** object);

    CARAPI Set(
        /* [in] */ Int32 location,
        /* [in] */ IInterface* object,
        /* [out] */ IInterface** prevObject);

protected:
    /**
      * Constructs a new instance of this AbstractSequentialList.
    */
    AbstractSequentialList() {}
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_ABSTRACTSEQUENTIALLIST_H__
