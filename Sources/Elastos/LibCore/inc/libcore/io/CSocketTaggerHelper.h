#ifndef __CSOCKETTAGGERhELPER_H__
#define __CSOCKETTAGGERhELPER_H__

#include "core/Singleton.h"
#include "SocketTagger.h"
#include "_Libcore_IO_CSocketTaggerHelper.h"

using Elastos::Core::Singleton;

namespace Libcore {
namespace IO {

CarClass(CSocketTaggerHelper)
    , public Singleton
    , public ISocketTaggerHelper
{
public:
    CAR_SINGLETON_DECL()

    CAR_INTERFACE_DECL()

    /**
     * Sets this process' socket tagger to {@code tagger}.
     */
    CARAPI Set(
        /* [in] */ ISocketTagger* tagger);

    /**
     * Returns this process socket tagger.
     */
    CARAPI Get(
        /* [out] */ ISocketTagger** tagger);
};

} // namespace IO
} // namespace Libcore

#endif // __CSOCKETTAGGERhELPER_H__