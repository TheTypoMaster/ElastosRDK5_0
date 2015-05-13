
#ifndef __COBJECTCONTAINER_H__
#define __COBJECTCONTAINER_H__

#include "_CObjectContainer.h"
#include <elastos.h>
#include "container.h"
#include "CObjectEnumerator.h"

namespace Elastos{
namespace Core{

CarClass(CObjectContainer), public SimpleContainer
{
public:
    CObjectContainer();

    virtual ~CObjectContainer();

    CARAPI Add(
        /* [in] */ IInterface* pObject);

    CARAPI Remove(
        /* [in] */ IInterface* pObject);

    CARAPI GetObjectEnumerator(
        /* [out] */ IObjectEnumerator** ppEnumerator);

    CARAPI GetObjectCount(
        /* [out] */ Int32* pCount);

    CARAPI Contains(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* contains);

    CARAPI ContainsAll(
        /* [in] */ IObjectContainer* objectContainer,
        /* [out] */ Boolean* contains);

    CARAPI Dispose();

private:
    Int32               mCount;
    Int32               mState;
};

} // namespace Core
} // namespace Elastos

#endif //__COBJECTCONTAINER_H__
