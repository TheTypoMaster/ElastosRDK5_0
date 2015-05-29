#ifndef __OS_H__
#define __OS_H__

#include "core/Object.h"

namespace Elastos {
namespace Droid {
namespace System {

class Os
{
public:
    CARAPI Accept(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IInetSocketAddress* peerAddress,
        /* [out] */ Int32* retFd);

    CARAPI Access(
        /* [in] */ const String& path,
        /* [in] */ Int32 mode,
        /* [out] */ Boolean* succeed);

    CARAPI Elastos_getaddrinfo(
        /* [in] */ const String& node,
        /* [in] */ IStructAddrinfo* hints,
        /* [in] */ Int32 netId,
        /* [out, callee] */ ArrayOf<IInetAddress*>** info);

    CARAPI Bind(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 port);

    CARAPI Chmod(
        /* [in] */ const String& path,
        /* [in] */ Int32 mode);

    CARAPI Chown(
        /* [in] */ const String& path,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid);

    CARAPI Close(
        /* [in] */ IFileDescriptor* fd);

    CARAPI Connect(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 port);

    CARAPI Dup(
        /* [in] */ Int32 oldFd,
        /* [out] */ Int32* retFd);

    CARAPI Dup2(
        /* [in] */ Int32 oldFd,
        /* [in] */ Int32 newFd,
        /* [out] */ Int32* retFd);

    CARAPI Environ(
        /* [out, callee] */ ArrayOf<String>** env);

    CARAPI Execv(
        /* [in] */ const String& filename,
        /* [in] */ ArrayOf<String>* argv);

    CARAPI Execve(
        /* [in] */ const String& filename,
        /* [in] */ ArrayOf<String>* argv,
        /* [in] */ ArrayOf<String>* envp);

    CARAPI Fchmod(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 mode);

    CARAPI Fchown(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid);

    CARAPI FcntlVoid(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 cmd,
        /* [out] */ Int32* result);

    CARAPI FcntlInt64(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 cmd,
        /* [in] */ Int64 arg,
        /* [out] */ Int32* result);

    CARAPI FcntlFlock(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 cmd,
        /* [in] */ IStructFlock* arg,
        /* [out] */ Int32* result);

    CARAPI Fdatasync(
        /* [in] */ IFileDescriptor* fd);

    CARAPI Fstat(
        /* [in] */ IFileDescriptor* fd,
        /* [out] */ IStructStat** stat);

    CARAPI Fstatvfs(
        /* [in] */ IFileDescriptor* fd,
        /* [out] */ IStructStatVfs** statFs);

    CARAPI Fsync(
        /* [in] */ IFileDescriptor* fd);

    CARAPI Ftruncate(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int64 length);

    CARAPI Gai_strerror(
        /* [in] */ Int32 error,
        /* [out] */ String* strerror);

    CARAPI Getegid(
        /* [out] */ Int32* egid);

    CARAPI Geteuid(
        /* [out] */ Int32* euid);

    CARAPI Getgid(
        /* [out] */ Int32* gid);

    CARAPI Getenv(
        /* [in] */ const String& name,
        /* [out] */ String* env);

    /* TODO: break into getnameinfoHost and getnameinfoService? */
    CARAPI Getnameinfo(
        /* [in] */ IInetAddress* address,
        /* [in] */ Int32 flags,
        /* [out] */ String* nameinfo);

    CARAPI Getpeername(
        /* [in] */ IFileDescriptor* fd,
        /* [out] */ ISocketAddress** peername);

    CARAPI Getpid(
        /* [out] */ Int32* pid);

    CARAPI Getppid(
        /* [out] */ Int32* ppid);

    CARAPI Getpwnam(
        /* [in] */ const String& name,
        /* [out] */ IStructPasswd** pwnam);

    CARAPI Getpwuid(
        /* [in] */ Int32 uid,
        /* [out] */ IStructPasswd** pwuid);

    CARAPI Getsockname(
        /* [in] */ IFileDescriptor* fd,
        /* [out] */ ISocketAddress** sockname);

    CARAPI GetsockoptByte(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [out] */ Int32* sockopt);

    CARAPI GetsockoptInAddr(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [out] */ IInetAddress** addr);

    CARAPI GetsockoptInt(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [out] */ Int32* sockopt);

    CARAPI GetsockoptLinger(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [out] */ IStructLinger** linger);

    CARAPI GetsockoptTimeval(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [out] */ IStructTimeval** timeval);

    CARAPI GetsockoptUcred(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [out] */ IStructUcred** ucred);

    CARAPI Gettid(
        /* [out] */ Int32* tid);

    CARAPI Getuid(
        /* [out] */ Int32* uid);

    CARAPI If_indextoname(
        /* [in] */ Int32 index,
        /* [out] */ String* name);

    CARAPI Inet_pton(
        /* [in] */ Int32 family,
        /* [in] */ const String& address,
        /* [out] */ IInetAddress** addr);

    CARAPI IoctlInetAddress(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 cmd,
        /* [in] */ const String& interfaceName,
        /* [out] */ IInetAddress** addr);

    CARAPI IoctlInt(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 cmd,
        /* [in] */ Int32* arg,
        /* [out] */ Int32* result);

    CARAPI Isatty(
        /* [in] */ IFileDescriptor* fd,
        /* [out] */ Boolean* isatty);

    CARAPI Kill(
        /* [in] */ Int32 pid,
        /* [in] */ Int32 signal);

    CARAPI Lchown(
        /* [in] */ const String& path,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid);

    CARAPI Link(
        /* [in] */ const String& oldPath,
        /* [in] */ const String& newPath);

    CARAPI Listen(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 backlog);

    CARAPI Lseek(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int64 offset,
        /* [in] */ Int32 whence,
        /* [out] */ Int64* result);

    CARAPI Lstat(
        /* [in] */ const String& path,
        /* [out] */ IStructStat** stat);

    CARAPI Mincore(
        /* [in] */ Int64 address,
        /* [in] */ Int64 byteCount,
        /* [in] */ ArrayOf<Byte>* vector);

    CARAPI Mkdir(
        /* [in] */ const String& path,
        /* [in] */ Int32 mode);

    CARAPI Mlock(
        /* [in] */ Int64 address,
        /* [in] */ Int64 byteCount);

    CARAPI Mkfifo(
        /* [in] */ const String& path,
        /* [in] */ Int32 mode);

    CARAPI Mmap(
        /* [in] */ Int64 address,
        /* [in] */ Int64 byteCount,
        /* [in] */ Int32 prot,
        /* [in] */ Int32 flags,
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int64 offset,
        /* [out] */ Int64* result);

    CARAPI Msync(
        /* [in] */ Int64 address,
        /* [in] */ Int64 byteCount,
        /* [in] */ Int32 flags);

    CARAPI Munlock(
        /* [in] */ Int64 address,
        /* [in] */ Int64 byteCount);

    CARAPI Munmap(
        /* [in] */ Int64 address,
        /* [in] */ Int64 byteCount);

    CARAPI Open(
        /* [in] */ const String& path,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 mode,
        /* [out] */ IFileDescriptor** fd);

    CARAPI Pipe(
        /* [out, callee] */ ArrayOf<Int32>** fds);

    /* TODO: if we used the non-standard ppoll(2) behind the scenes, we could take a long timeout. */
    CARAPI Poll(
        /* [in] */ ArrayOf<IStructPollfd*>* fds,
        /* [in] */ Int32 timeoutMs,
        /* [out] */ Int32* result);

    CARAPI Posix_fallocate(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int64 offset,
        /* [in] */ Int64 length);

    CARAPI Prctl(
        /* [in] */ Int32 option,
        /* [in] */ Int64 arg2,
        /* [in] */ Int64 arg3,
        /* [in] */ Int64 arg4,
        /* [in] */ Int64 arg5,
        /* [out] */ Int32* prctl);

    CARAPI Pread(
        /* [in] */ IFileDescriptor* fd,
        /* [out] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int64 offset,
        /* [out] */ Int32* num);

    CARAPI Pread(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IByteBuffer* buffer,
        /* [in] */ Int64 offset,
        /* [out] */ Int32* num);

    CARAPI Pwrite(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int64 offset,
        /* [out] */ Int32* num);

    CARAPI Pwrite(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IByteBuffer* buffer,
        /* [in] */ Int64 offset,
        /* [out] */ Int32* num);

    CARAPI Read(
        /* [in] */ IFileDescriptor* fd,
        /* [out] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [out] */ Int32* num);

    CARAPI Read(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IByteBuffer* buffer,
        /* [out] */ Int32* num);

    CARAPI Readlink(
        /* [in] */ const String& path,
        /* [out] */ String* link);

    CARAPI Readv(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ ArrayOf<IInterface*>* buffers,
        /* [in] */ ArrayOf<Int32>* offsets,
        /* [in] */ ArrayOf<Int32>* byteCounts,
        /* [out] */ Int32* num);

    CARAPI Recvfrom(
        /* [in] */ IFileDescriptor* fd,
        /* [out] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int32 flags,
        /* [in] */ IInetSocketAddress* srcAddress,
        /* [out] */ Int32* num);

    CARAPI Recvfrom(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IByteBuffer* buffer,
        /* [in] */ Int32 flags,
        /* [in] */ IInetSocketAddress* srcAddress,
        /* [out] */ Int32* num);

    CARAPI Remove(
        /* [in] */ const String& path);

    CARAPI Rename(
        /* [in] */ const String& oldPath,
        /* [in] */ const String& newPath);

    CARAPI Sendto(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [in] */ Int32 flags,
        /* [in] */ IInetAddress* inetAddress,
        /* [in] */ Int32 port,
        /* [out] */ Int32* result);

    CARAPI Sendto(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IByteBuffer* buffer,
        /* [in] */ Int32 flags,
        /* [in] */ IInetAddress* inetAddress,
        /* [in] */ Int32 port,
        /* [out] */ Int32* result);

    CARAPI Sendfile(
        /* [in] */ Int32 outFd,
        /* [in] */ Int32 inFd,
        [in/*, out */] Int64* inOffset,
        /* [in] */ Int64 byteCount,
        /* [out] */ Int64* result);

    CARAPI Setegid(
        /* [in] */ Int32 egid);

    CARAPI Setenv(
        /* [in] */ const String& name,
        /* [in] */ const String& value,
        /* [in] */ Boolean overwrite);

    CARAPI Seteuid(
        /* [in] */ Int32 euid);

    CARAPI Setgid(
        /* [in] */ Int32 gid);

    CARAPI Setsid(
        /* [out] */ Int32* sid);

    CARAPI SetsockoptByte(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [in] */ Int32 value);

    CARAPI SetsockoptIfreq(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [in] */ const String& interfaceName);

    CARAPI SetsockoptInt(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [in] */ Int32 value);

    CARAPI SetsockoptIpMreqn(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [in] */ Int32 value);

    CARAPI SetsockoptGroupReq(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [in] */ IStructGroupReq* value);

    CARAPI SetsockoptGroupSourceReq(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [in] */ IStructGroupSourceReq* value);

    CARAPI SetsockoptLinger(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [in] */ IStructLinger* value);

    CARAPI SetsockoptTimeval(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 level,
        /* [in] */ Int32 option,
        /* [in] */ IStructTimeval* value);

    CARAPI Setuid(
        /* [in] */ Int32 uid);

    CARAPI Shutdown(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ Int32 how);

    CARAPI Socket(
        /* [in] */ Int32 socketDomain,
        /* [in] */ Int32 type,
        /* [in] */ Int32 protocol,
        /* [out] */ IFileDescriptor** fd);

    CARAPI Socketpair(
        /* [in] */ Int32 socketDomain,
        /* [in] */ Int32 type,
        /* [in] */ Int32 protocol,
        /* [out] */ IFileDescriptor** fd1,
        /* [out] */ IFileDescriptor** fd2);

    CARAPI Stat(
        /* [in] */ const String& path,
        /* [out] */ IStructStat** stat);

    /* TODO: replace statfs with statvfs. */
    CARAPI StatVfs(
        /* [in] */ const String& path,
        /* [out] */ IStructStatVfs** statfs);

    CARAPI Strerror(
        /* [in] */ Int32 errno,
        /* [out] */ String* strerr);

    CARAPI Symlink(
        /* [in] */ const String& oldPath,
        /* [in] */ const String& newPath);

    CARAPI Sysconf(
        /* [in] */ Int32 name,
        /* [out] */ Int64* result);

    CARAPI Tcdrain(
        /* [in] */ IFileDescriptor* fd);

    CARAPI Umask(
        /* [in] */ Int32 mask,
        /* [out] */ Int32* result);

    CARAPI Uname(
        /* [out] */ IStructUtsname** uname);

    CARAPI Waitpid(
        /* [in] */ Int32 pid,
        [in/*, out */] Int32* status,
        /* [in] */ Int32 options,
        /* [out] */ Int32* result);

    CARAPI Write(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ ArrayOf<Byte>* bytes,
        /* [in] */ Int32 byteOffset,
        /* [in] */ Int32 byteCount,
        /* [out] */ Int32* num);

    CARAPI Write(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ IByteBuffer* buffer,
        /* [out] */ Int32* num);

    CARAPI Writev(
        /* [in] */ IFileDescriptor* fd,
        /* [in] */ ArrayOf<IInterface*>* buffers,
        /* [in] */ ArrayOf<Int32>* offsets,
        /* [in] */ ArrayOf<Int32>* byteCounts,
        /* [out] */ Int32* result);
private:
    CARAPI Os();
};

}// namespace System
}// namespace Droid
}// namespace Elastos

#endif