#ifndef __ELASTOS_IO_SOCKETCHANNELIMPL_H__
#define __ELASTOS_IO_SOCKETCHANNELIMPL_H__

#include "Elastos.CoreLibrary_server.h"
#include "SocketChannel.h"
#include "CFileDescriptor.h"
#include "FilterOutputStream.h"
#include "FilterInputStream.h"
// #include "CInetSocketAddress.h"
// #include "Socket.h"

using Elastos::IO::Channels::SocketChannel;
// using Elastos::Net::CInetSocketAddress;
using Elastos::Net::IInetAddress;
using Elastos::Net::IInetSocketAddress;
using Elastos::Net::IPlainSocketImpl;

namespace Elastos {
namespace IO {

class SocketChannelInputStream;
class SocketChannelOutputStream;
class SocketAdapter;

class SocketChannelImpl
    : public SocketChannel
    , public IFileDescriptorChannel
{
private:
    /*
     * Adapter classes for internal socket.
     */
    class SocketAdapter // : public Socket
    {
    public:
        SocketAdapter(
            /* [in] */ IPlainSocketImpl* socketImpl,
            /* [in] */ SocketChannelImpl* channel);

        CARAPI_(AutoPtr<SocketChannel>) GetChannel();

        CARAPI Connect(
            /* [in] */ ISocketAddress* remoteAddr,
            /* [in] */ Int32 timeout);

        CARAPI Bind(
            /* [in] */ ISocketAddress* localAddr);

        CARAPI Close();

        CARAPI GetInputStream(
            /* [out] */ IInputStream** inputStream);

        CARAPI GetOutputStream(
            /* [out] */ IOutputStream** outstream);

        CARAPI GetFileDescriptor(
            /* [out] */ IFileDescriptor** outfd);

    private:
        AutoPtr<SocketChannelImpl> mChannel;
        // PlainSocketImpl* mSocketImpl;
    };


    /*
     * This output stream delegates all operations to the associated channel.
     * Throws an IllegalBlockingModeException if the channel is in non-blocking
     * mode when performing write operations.
     */
    class BlockingCheckOutputStream : public FilterOutputStream
    {
    public:
        BlockingCheckOutputStream(
            /* [in] */ IOutputStream* out,
            /* [in] */ SocketChannel* channel);

        CARAPI Write(
            /* [in] */ ArrayOf<Byte>* buffer,
            /* [in] */ Int32 offset,
            /* [in] */ Int32 count);

        CARAPI Write(
            /* [in] */ Int32 oneByte);

        CARAPI Write(
            /* [in] */ ArrayOf<Byte>* buffer);

        CARAPI Close();

    private:
        CARAPI CheckBlocking();

    private:
            AutoPtr<SocketChannel> mChannel;
    };

    /*
     * This input stream delegates all operations to the associated channel.
     * Throws an IllegalBlockingModeException if the channel is in non-blocking
     * mode when performing read operations.
    */
    class BlockingCheckInputStream : public FilterInputStream
    {
    public:
        BlockingCheckInputStream(
            /* [in] */ IInputStream* in,
            /* [in] */ SocketChannel* channel);

        CARAPI Read(
            /* [out] */ Int32* value);

        CARAPI Read(
            /* [in] */ ArrayOf<Byte>* buffer,
            /* [in] */ Int32 byteOffset,
            /* [in] */ Int32 byteCount,
            /* [out] */ Int32* number);

        CARAPI Read(
            /* [in] */ ArrayOf<Byte>* buffer,
            /* [out] */ Int32* number);

        CARAPI Close();

    private:
        CARAPI CheckBlocking();

    private:
        AutoPtr<SocketChannel> mChannel;
    };

public:
    /*
     * Constructor for creating a connected socket channel.
     */
    SocketChannelImpl(
        /* [in] */ ISelectorProvider* provider);

    /*
     * Constructor for creating an optionally connected socket channel.
     */
    SocketChannelImpl(
        /* [in] */ ISelectorProvider* provider,
        /* [in] */ Boolean connect);

    /*
     * Constructor for use by Pipe.SinkChannel and Pipe.SourceChannel.
     */
    SocketChannelImpl(
        /* [in] */ ISelectorProvider* provider,
        /* [in] */ IFileDescriptor* existingFd);

    CAR_INTERFACE_DECL()

    /*
     * Getting the internal Socket If we have not the socket, we create a new
     * one.
     */
    CARAPI GetSocket(
        /* [out] */ ISocket** socket);

    CARAPI IsConnected(
        /* [out] */ Boolean* ret);

    CARAPI IsConnectionPending(
        /* [out] */ Boolean* ret);

    CARAPI Connect(
        /* [in] */ ISocketAddress* addr,
        /* [out] */ Boolean* ret);

    CARAPI FinishConnect(
        /* [out] */ Boolean* ret);

    CARAPI Read(
        /* [in] */ IByteBuffer* dst,
        /* [out] */ Int32* ret);

    CARAPI Read(
        /* [in] */ ArrayOf<IByteBuffer*>& target,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 len,
        /* [out] */ Int32* ret);

    CARAPI Write(
        /* [in] */ IByteBuffer* src);

    CARAPI Write(
        /* [in] */ ArrayOf<IByteBuffer*>& source,
        /* [in] */ Int32 offset,
        /* [in] */ Int32 len,
        /* [out] */ Int32* ret);

    /*
     * Get local address.
     */
    CARAPI GetLocalAddress(
        /* [out] */ IInetAddress** addr);

    /*
     * Get the fd.
     */
    CARAPI GetFD(
        /* [out] */ IFileDescriptor** dstcriptor);

    /* @hide used by ServerSocketChannelImpl to sync channel state during accept() */
    CARAPI OnAccept(
        /* [in] */ IInetSocketAddress* remoteAddress,
        /* [in] */ Boolean updateSocketState);

protected:
    /**
     * Initialise the isBound, localAddress and localPort state from the file descriptor. Used when
     * some or all of the bound state has been left to the OS to decide, or when the Socket handled
     * bind() or connect().
     *
     * @param updateSocketState
     *      if the associated socket (if present) needs to be updated
     * @hide package visible for other nio classes
     */
    CARAPI_(void) OnBind(
        /* [in] */ Boolean updateSocketState);

    /**
     * Initialise the connect() state with the supplied information.
     *
     * @param updateSocketState
     *     if the associated socket (if present) needs to be updated
     * @hide package visible for other nio classes
     */
    CARAPI_(void) OnConnectStatusChanged(
        /* [in] */ IInetSocketAddress* address,
        /* [in] */ Int32 status,
        /* [in] */ Boolean updateSocketState);

    CARAPI_(void) SetConnected();

    CARAPI_(void) SetBound(
        /* [in] */ Boolean flag);

    CARAPI_(void) FinishAccept();

    /*
     * Shared by this class and DatagramChannelImpl, to do the address transfer
     * and check.
     */
    static CARAPI ValidateAddress(
        /* [in] */ ISocketAddress* addr,
        /* [out] */ IInetSocketAddress** sockAddr);

    /*
     * Do really closing action here.
     */
    CARAPI ImplCloseSelectableChannel();

    CARAPI ImplConfigureBlocking(
        /* [in] */ Boolean block);

private:
    CARAPI_(Boolean) IsEINPROGRESS(
        /* [in] */ ECode e);

    CARAPI InitLocalAddressAndPort();

    CARAPI ReadImpl(
        /* [in] */ IByteBuffer* dst,
        /* [out] */ Int32* ret);

    CARAPI WriteImpl(
        /* [in] */ IByteBuffer* src,
        /* [out] */ Int32* ret);

    /*
     * Status check, open and "connected", when read and write.
     */
    CARAPI CheckOpenConnected();

    /*
     * Status check, open and "unconnected", before connection.
     */
    CARAPI CheckUnconnected();

private:
    const static Int32 SOCKET_STATUS_UNINITITLIZED;

    // Status before connect.
    const static Int32 SOCKET_STATUS_UNCONNECTED;

    // Status connection pending.
    const static Int32 SOCKET_STATUS_PENDING;

    // Status after connection success.
    const static Int32 SOCKET_STATUS_CONNECTED;

    // Status closed.
    const static Int32 SOCKET_STATUS_CLOSED;

    CFileDescriptor* mFileDescriptor;

    // Our internal Socket.
    SocketAdapter* mSocket;

    // The address to be connected.
    AutoPtr<IInetSocketAddress> mConnectAddress;

    IInetAddress* mLocalAddress;

    Int32 mLocalPort;

    volatile Boolean mIsBound;

    Int32 mStatus;

    Object mReadLock;

    Object mWriteLock;
};

} // namespace IO
} // namespace Elastos

#endif // __ELASTOS_IO_SOCKETCHANNELIMPL_H__
