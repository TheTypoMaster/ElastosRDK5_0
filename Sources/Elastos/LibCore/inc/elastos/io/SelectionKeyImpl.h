#ifndef __ELASTOS_IO_CHANNELS_SELECTIONKEYIMPL_H__
#define __ELASTOS_IO_CHANNELS_SELECTIONKEYIMPL_H__

#include "AbstractSelectionKey.h"
#include "AbstractSelectableChannel.h"

using Elastos::Core::IRunnable;
using Elastos::IO::Channels::ISelector;
using Elastos::IO::Channels::ISocketChannel;
using Elastos::IO::Channels::ISelectableChannel;
using Elastos::IO::Channels::Spi::AbstractSelectionKey;
using Elastos::IO::Channels::Spi::IAbstractSelectableChannel;
using Elastos::IO::Channels::Spi::IAbstractSelector;

namespace Elastos {
namespace IO {

/**
 * Default implementation of SelectionKey
 */
class SelectionKeyImpl
    : public AbstractSelectionKey
{
public:
    SelectionKeyImpl(
        /* [in] */ IAbstractSelectableChannel* channel,
        /* [in] */ Int32 opts,
        /* [in] */ IObject* attach,
        /* [in] */ IAbstractSelector* selector);

    CARAPI GetChannel(
        /* [out] */ ISelectableChannel** channel);

    CARAPI GetInterestOps(
        /* [out] */ Int32* opts);

    Int32 InterestOpsNoCheck();

    CARAPI GetInterestOps(
        /* [in] */ Int32 operations,
        /* [out] */ ISelectionKey** key);

    CARAPI GetReadyOps(
        /* [out] */ Int32* ret);

    CARAPI GetSelector(
        /* [in] */ ISelector** selector);

    CARAPI SetReadyOps(
        /* [in] */ Int32 readyOps);

    CARAPI CheckValid();

    CARAPI IsConnected(
        /* [out] */ Boolean* isConnected);

private:
    AutoPtr<IAbstractSelectableChannel> mChannel;
    Int32 mInterestOps;
    Int32 mReadyOps;
    AutoPtr<IAbstractSelector> mSelector;
};


} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_CHANNELS_SELECTIONKEYIMPL_H__
