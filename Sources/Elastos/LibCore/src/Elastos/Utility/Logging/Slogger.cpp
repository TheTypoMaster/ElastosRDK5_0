
#include <elastos/Logger.h>
#include <elastos/Slogger.h>

namespace Elastos {
namespace Utility {
namespace Logging {

extern ECode __PrintlnImpl(Int32 bufID, Int32 priority, CString tag, const char *msgBuf);

#define MSG_BUF_SIZE    1024

ECode Slogger::D(CString tag, const char *fmt, ...)
{
    char msgBuf[MSG_BUF_SIZE];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msgBuf, MSG_BUF_SIZE, fmt, args);
    va_end(args);

    return __PrintlnImpl(Logger::LOG_ID_SYSTEM, Logger::___DEBUG, tag, msgBuf);
}

/**
 * Send an ERROR log message.
 */
ECode Slogger::E(CString tag, const char *fmt, ...)
{
    char msgBuf[MSG_BUF_SIZE];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msgBuf, MSG_BUF_SIZE, fmt, args);
    va_end(args);

    return __PrintlnImpl(Logger::LOG_ID_SYSTEM, Logger::ERROR, tag, msgBuf);
}

/**
 * Send an INFO log message.
 */
ECode Slogger::I(CString tag, const char *fmt, ...)
{
    char msgBuf[MSG_BUF_SIZE];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msgBuf, MSG_BUF_SIZE, fmt, args);
    va_end(args);

    return __PrintlnImpl(Logger::LOG_ID_SYSTEM, Logger::INFO, tag, msgBuf);
}

/**
 * Send an VERBOSE log message.
 */
ECode Slogger::V(CString tag, const char *fmt, ...)
{
    char msgBuf[MSG_BUF_SIZE];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msgBuf, MSG_BUF_SIZE, fmt, args);
    va_end(args);

    return __PrintlnImpl(Logger::LOG_ID_SYSTEM, Logger::VERBOSE, tag, msgBuf);
}

/**
 * Send a WARN log message.
 */
ECode Slogger::W(CString tag, const char *fmt, ...)
{
    char msgBuf[MSG_BUF_SIZE];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msgBuf, MSG_BUF_SIZE, fmt, args);
    va_end(args);

    return __PrintlnImpl(Logger::LOG_ID_SYSTEM, Logger::WARN, tag, msgBuf);
}

/**
 * Low-level logging call.
 */
ECode Slogger::Println(Int32 priority, CString tag, const char *fmt, ...)
{
    char msgBuf[MSG_BUF_SIZE];
    va_list args;
    va_start(args, fmt);
    vsnprintf(msgBuf, MSG_BUF_SIZE, fmt, args);
    va_end(args);

    return __PrintlnImpl(Logger::LOG_ID_SYSTEM, priority, tag, msgBuf);
}

} // Logging
} // Utility
} // Elastos
