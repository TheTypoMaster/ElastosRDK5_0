
#include "wm/ViewServer.h"
#include <elastos/core/Thread.h>
#include <elastos/core/StringUtils.h>

using Elastos::IO::ICloseable;
using Elastos::IO::IFlushable;
using Elastos::IO::IBufferedReader;
using Elastos::IO::IBufferedWriter;
using Elastos::IO::CBufferedReader;
using Elastos::IO::CBufferedWriter;
using Elastos::IO::IInputStream;
using Elastos::IO::IInputStreamReader;
using Elastos::IO::CInputStreamReader;
using Elastos::IO::IOutputStream;
using Elastos::IO::IOutputStreamWriter;
using Elastos::IO::COutputStreamWriter;
using Elastos::Net::IInetAddress;
using Elastos::Net::CServerSocket;
using Elastos::Net::IInetAddressHelper;
using Elastos::Net::CInetAddressHelper;
using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Wm {

ViewServer::ViewServerWorker::ViewServerWorker(
    /* [in] */ ISocket* client,
    /* [in] */ ViewServer* host)
    : mClient(client)
    , mNeedWindowListUpdate(FALSE)
    , mNeedFocusedWindowUpdate(FALSE)
    , mHost(host)
{ }

PInterface ViewServer::ViewServerWorker::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IRunnable*)this;
    }
    else if (riid == Elastos::Core::EIID_IRunnable) {
        return (IRunnable*)this;
    }
    return NULL;
}

UInt32 ViewServer::ViewServerWorker::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ViewServer::ViewServerWorker::Release()
{
    return ElRefBase::Release();
}

ECode ViewServer::ViewServerWorker::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode ViewServer::ViewServerWorker::Run()
{
    AutoPtr<IBufferedReader> in;
    // try {
    AutoPtr<IInputStream> inputStream;
    ECode ec = mClient->GetInputStream((IInputStream**)&inputStream);
    if (FAILED(ec)) {
        mClient->Close();
        return ec;
    }
    AutoPtr<IInputStreamReader> reader;
    CInputStreamReader::New(inputStream, (IInputStreamReader**)&reader);
    ec = CBufferedReader::New(reader, 1024, (IBufferedReader**)&in);
    if (FAILED(ec)) {
        mClient->Close();
        return ec;
    }

    String request;
    ec = in->ReadLine(&request);
    if (FAILED(ec)) {
        ICloseable::Probe(in)->Close();
        mClient->Close();
        return ec;
    }

    String command;
    String parameters;

    Int32 index = request.IndexOf(' ');
    if (index == -1) {
        command = request;
        parameters = String("");
    }
    else {
        command = request.Substring(0, index);
        parameters = request.Substring(index + 1);
    }

    Boolean result;
    if (COMMAND_PROTOCOL_VERSION.EqualsIgnoreCase(command)) {
        result = mHost->WriteValue(mClient, VALUE_PROTOCOL_VERSION);
    }
    else if (COMMAND_SERVER_VERSION.EqualsIgnoreCase(command)) {
        result = mHost->WriteValue(mClient, VALUE_SERVER_VERSION);
    }
    else if (COMMAND_WINDOW_MANAGER_LIST.EqualsIgnoreCase(command)) {
        result = mHost->mWindowManager->ViewServerListWindows(mClient);
    }
    else if (COMMAND_WINDOW_MANAGER_GET_FOCUS.EqualsIgnoreCase(command)) {
        result = mHost->mWindowManager->ViewServerGetFocusedWindow(mClient);
    }
    else if (COMMAND_WINDOW_MANAGER_AUTOLIST.EqualsIgnoreCase(command)) {
        result = WindowManagerAutolistLoop();
    }
    else {
        result = mHost->mWindowManager->ViewServerWindowCommand(mClient,
                command, parameters);
    }

    // if (!result) {
    //     Slog.w(LOG_TAG, "An error occurred with the command: " + command);
    // }
    // } catch(IOException e) {
    //     Slog.w(LOG_TAG, "Connection error: ", e);
    // } finally {
    if (in != NULL) {
        // try {
        ICloseable::Probe(in)->Close();

        // } catch (IOException e) {
        //     e.printStackTrace();
        // }
    }
    if (mClient != NULL) {
        // try {
        mClient->Close();
        // } catch (IOException e) {
        //     e.printStackTrace();
        // }
    }
    // }

    return NOERROR;
}

void ViewServer::ViewServerWorker::WindowsChanged()
{
    AutoLock lock(mLock);
    mNeedWindowListUpdate = TRUE;
    mLock.NotifyAll();
}

void ViewServer::ViewServerWorker::FocusChanged()
{
    AutoLock lock(mLock);
    mNeedFocusedWindowUpdate = TRUE;
    mLock.NotifyAll();
}

Boolean ViewServer::ViewServerWorker::WindowManagerAutolistLoop()
{
    mHost->mWindowManager->AddWindowChangeListener(
            (CWindowManagerService::IWindowChangeListener*)this);
    AutoPtr<IBufferedWriter> out;
    // try {
    AutoPtr<IOutputStream> outputStream;
    if (FAILED(mClient->GetOutputStream((IOutputStream**)&outputStream))) {
        mHost->mWindowManager->RemoveWindowChangeListener(
                (CWindowManagerService::IWindowChangeListener*)this);
        return TRUE;
    }
    AutoPtr<IOutputStreamWriter> writer;
    if (FAILED(COutputStreamWriter::New(outputStream, (IOutputStreamWriter**)&writer))) {
        mHost->mWindowManager->RemoveWindowChangeListener(
                (CWindowManagerService::IWindowChangeListener*)this);
        return TRUE;
    }
    if (FAILED(CBufferedWriter::New(writer, (IBufferedWriter**)&out))) {
        mHost->mWindowManager->RemoveWindowChangeListener(
                (CWindowManagerService::IWindowChangeListener*)this);
        return TRUE;
    }
    while (!Thread::Interrupted()) {
        Boolean needWindowListUpdate = FALSE;
        Boolean needFocusedWindowUpdate = FALSE;

        {
            AutoLock lock(mLock);
            while (!mNeedWindowListUpdate && !mNeedFocusedWindowUpdate) {
                mLock.Wait();
            }
            if (mNeedWindowListUpdate) {
                mNeedWindowListUpdate = FALSE;
                needWindowListUpdate = TRUE;
            }
            if (mNeedFocusedWindowUpdate) {
                mNeedFocusedWindowUpdate = FALSE;
                needFocusedWindowUpdate = TRUE;
            }
        }

        if (needWindowListUpdate) {
            out->WriteString(String("LIST UPDATE\n"));
            IFlushable::Probe(out)->Flush();
        }
        if (needFocusedWindowUpdate) {
            out->WriteString(String("FOCUS UPDATE\n"));
            IFlushable::Probe(out)->Flush();
        }
    }
    // } catch (Exception e) {
    //     // Ignore
    // } finally {
    if (out != NULL) {
        // try {
        ICloseable::Probe(out)->Close();
        // } catch (IOException e) {
        //     // Ignore
        // }
    }
    mHost->mWindowManager->RemoveWindowChangeListener(
            (CWindowManagerService::IWindowChangeListener*)this);
    // }
    return TRUE;
}

const Int32 ViewServer::VIEW_SERVER_DEFAULT_PORT = 4939;
const Int32 ViewServer::VIEW_SERVER_MAX_CONNECTIONS = 10;
const String ViewServer::LocalLOG_TAG("ViewServer");
const String ViewServer::VALUE_PROTOCOL_VERSION("4");
const String ViewServer::VALUE_SERVER_VERSION("4");
const String ViewServer::COMMAND_PROTOCOL_VERSION("PROTOCOL");
const String ViewServer::COMMAND_SERVER_VERSION("SERVER");
const String ViewServer::COMMAND_WINDOW_MANAGER_LIST("LIST");
const String ViewServer::COMMAND_WINDOW_MANAGER_AUTOLIST("AUTOLIST");
const String ViewServer::COMMAND_WINDOW_MANAGER_GET_FOCUS("GET_FOCUS");

ViewServer::ViewServer(
    /* [in] */ CWindowManagerService* windowManager,
    /* [in] */ Int32 port)
    : mWindowManager(windowManager)
    , mPort(port)
{ }

PInterface ViewServer::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (IInterface*)(IRunnable*)this;
    }
    else if (riid == Elastos::Core::EIID_IRunnable) {
        return (IAnimation*)this;
    }
    return NULL;
}

UInt32 ViewServer::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ViewServer::Release()
{
    return ElRefBase::Release();
}

ECode ViewServer::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode ViewServer::Start(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (mThread != NULL) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<IInetAddress> addr;
    AutoPtr<IInetAddressHelper> addrHelper;
    CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&addrHelper);
    addrHelper->GetLocalHost((IInetAddress**)&addr);
    FAIL_RETURN(CServerSocket::New(mPort, VIEW_SERVER_MAX_CONNECTIONS, addr,
            (IServerSocket**)&mServer));
    FAIL_RETURN(CThread::New((IRunnable*)this,
            String("Remote View Server [port=") + StringUtils::Int32ToString(mPort) + String("]"),
            (IThread**)&mThread));
    // AutoPtr<IExecutors> executors;
    // CExecutors::AcquireSingleton((IExecutors**)&executors);
    // FAIL_RETURN(executors->NewFixedThreadPool(VIEW_SERVER_MAX_CONNECTIONS,
    //         (IExecutorService**)&mThreadPool));
    mThread->Start();

    *result = TRUE;
    return NOERROR;
}

Boolean ViewServer::Stop()
{
    if (mThread != NULL) {

        mThread->Interrupt();
        // if (mThreadPool != NULL) {
        //     // try {
        //     mThreadPool->ShutdownNow();
        //     // } catch (SecurityException e) {
        //     //     Slog.w(LOG_TAG, "Could not stop all view server threads");
        //     // }
        // }
        // mThreadPool = NULL;
        mThread = NULL;
        // try {
        mServer->Close();
        mServer = NULL;
        return TRUE;
        // } catch (IOException e) {
        //     Slog.w(LOG_TAG, "Could not close the view server");
        // }
    }
    return FALSE;
}

Boolean ViewServer::IsRunning()
{
    Boolean isAlive;
    mThread->IsAlive(&isAlive);
    return mThread != NULL && isAlive;
}

ECode ViewServer::Run()
{
    while (Thread::GetCurrentThread() == mThread) {
        // Any uncaught exception will crash the system process
        // try {
        AutoPtr<ISocket> client;
        mServer->Accept((ISocket**)&client);
        // if (mThreadPool != NULL) {
        //     mThreadPool->Submit((IRunnable*)new ViewServerWorker(client, this));
        // }
        // else {
            // try {
            client->Close();
            // } catch (IOException e) {
            //     e.printStackTrace();
            // }
        // }
        // } catch (Exception e) {
        //     Slog.w(LOG_TAG, "Connection error: ", e);
        // }
    }
    return NOERROR;
}

Boolean ViewServer::WriteValue(
    /* [in] */ ISocket* client,
    /* [in] */ const String& value)
{
    Boolean result;
    AutoPtr<IBufferedWriter> out;
    AutoPtr<IOutputStreamWriter> writer;
    // try {
    AutoPtr<IOutputStream> clientStream;
    if (FAILED(client->GetOutputStream((IOutputStream**)&clientStream))) {
        result = FALSE;
        goto fail;
    }
    COutputStreamWriter::New(clientStream, (IOutputStreamWriter**)&writer);
    if (FAILED(CBufferedWriter::New(writer , 8 * 1024, (IBufferedWriter**)&out))) {
        result = FALSE;
        goto fail;
    }
    if (FAILED(out->WriteString(String(value)))) {
        result = FALSE;
        goto fail;
    }
    if (FAILED(out->WriteString(String("\n")))) {
        result = FALSE;
        goto fail;
    }
    if (FAILED(IFlushable::Probe(out)->Flush())) {
        result = FALSE;
        goto fail;
    }
    result = TRUE;
    // } catch (Exception e) {
    //     result = false;
    // } finally {
fail:
    if (out != NULL) {
        // try {
        if (FAILED(ICloseable::Probe(out)->Close())) {
            result = FALSE;
        }
        // } catch (IOException e) {
        //     result = false;
        // }
    }
    // }
    return result;
}


} // Wm
} // Server
} // Droid
} // Elastos
