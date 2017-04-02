#include "fy_log.hpp"

#include <stdarg.h>
#include <cstring>
#include <sstream>
#include <memory>

#include "fy_compiler.hpp"

#if FY_PLATFORM_WINDOWS
#include <windows.h>
#endif

FY_DISABLE_VC_WARNING(4018)

namespace fy
{

#define LOG_MAX_LENGTH 16 * 1024

#ifndef NDEBUG
static int gLogMask = SFoundationLogLevel::LOG_ALL;
#else
static int gLogMask = SFoundationLogLevel::LOG_WARN | SFoundationLogLevel::LOG_ERROR | SFoundationLogLevel::LOG_FATAL;
#endif

const char *LogLevelToString(SFoundationLogLevel::Enum inLevel)
{
    static const char theLevelString[][10] = {"FY_DEBUG", "FY_TRACE", "FY_INFO", "FY_WARN", "FY_ERROR", "FY_FATAL", "FY_???"};
    switch (inLevel)
    {
    case SFoundationLogLevel::LOG_DEBUG:
        return theLevelString[0];
    case SFoundationLogLevel::LOG_TRACE:
        return theLevelString[1];
    case SFoundationLogLevel::LOG_INFO:
        return theLevelString[2];
    case SFoundationLogLevel::LOG_WARN:
        return theLevelString[3];
    case SFoundationLogLevel::LOG_ERROR:
        return theLevelString[4];
    case SFoundationLogLevel::LOG_FATAL:
        return theLevelString[5];
    default:
        return theLevelString[6];
    }
}

void Log::EnableLog(SFoundationLogLevel::Enum inLevel, bool isEnable)
{
    if (isEnable)
        gLogMask |= inLevel;
    else
        gLogMask &= ~inLevel;
}

void Log::LogMessage(SFoundationLogLevel::Enum inLevel, const char *inTag, const char *inMessage, const char *inDetails)
{
    if (strlen(inTag) > LOG_MAX_LENGTH)
        inTag = "(Tag too big)";
    if (strlen(inMessage) > LOG_MAX_LENGTH)
        inMessage = "(Message too long)";
    if (strlen(inDetails) > LOG_MAX_LENGTH)
        inDetails = "(Details too long)";
    printf("%s(%s): %s - %s\n", LogLevelToString(inLevel), inTag, inMessage, inDetails);
#if FY_PLATFORM_WINDOWS
    std::stringstream msg;
    msg << LogLevelToString(inLevel) << "(" << inTag << "): " << inMessage << " - " << inDetails << std::endl;
    OutputDebugStringA(msg.str().c_str());
#endif
}

#if FY_COMPILER_MSVC
#define FY_VSNPRINTF(buffer, size, format, args) vsnprintf_s(buffer, size, _TRUNCATE, format, args)
#else
#define FY_VSNPRINTF(buffer, size, format, args) vsnprintf(buffer, size, format, args)
#endif

#define LOG_FORMAT_DETAILS(level)                                    \
    std::unique_ptr<char[]> formatted;                               \
    std::size_t n = std::strlen(inDetails) * 2;                      \
    va_list theArgs;                                                 \
    for (;;)                                                         \
    {                                                                \
        formatted.reset(new char[n]);                                \
        va_start(theArgs, inDetails);                                \
        int fn = FY_VSNPRINTF(&formatted[0], n, inDetails, theArgs); \
        va_end(theArgs);                                             \
        if (fn < 0 || fn >= n)                                       \
            n += abs(static_cast<long>(fn - n + 1));                 \
        else                                                         \
            break;                                                   \
    }

void Log::LogDebug(const char *inTag, const char *inMessage, const char *inDetails, ...)
{
    if (((SFoundationLogLevel::LOG_DEBUG)&gLogMask) == 0)
        return;
    LOG_FORMAT_DETAILS(SFoundationLogLevel::LOG_DEBUG);
    LogMessage(SFoundationLogLevel::LOG_DEBUG, inTag, inMessage, &formatted[0]);
}

void Log::LogTrace(const char *inTag, const char *inMessage, const char *inDetails, ...)
{
    if (((SFoundationLogLevel::LOG_TRACE)&gLogMask) == 0)
        return;
    LOG_FORMAT_DETAILS(SFoundationLogLevel::LOG_TRACE);
    LogMessage(SFoundationLogLevel::LOG_TRACE, inTag, inMessage, &formatted[0]);
}

void Log::LogInfo(const char *inTag, const char *inMessage, const char *inDetails, ...)
{
    if (((SFoundationLogLevel::LOG_INFO)&gLogMask) == 0)
        return;
    LOG_FORMAT_DETAILS(SFoundationLogLevel::LOG_INFO);
    LogMessage(SFoundationLogLevel::LOG_INFO, inTag, inMessage, &formatted[0]);
}

void Log::LogWarn(const char *inTag, const char *inMessage, const char *inDetails, ...)
{
    if (((SFoundationLogLevel::LOG_WARN)&gLogMask) == 0)
        return;
    LOG_FORMAT_DETAILS(SFoundationLogLevel::LOG_WARN);
    LogMessage(SFoundationLogLevel::LOG_WARN, inTag, inMessage, &formatted[0]);
}

void Log::LogError(const char *inTag, const char *inMessage, const char *inDetails, ...)
{
    if (((SFoundationLogLevel::LOG_ERROR)&gLogMask) == 0)
        return;
    LOG_FORMAT_DETAILS(SFoundationLogLevel::LOG_ERROR);
    LogMessage(SFoundationLogLevel::LOG_ERROR, inTag, inMessage, &formatted[0]);
}

void Log::LogFatal(const char *inTag, const char *inMessage, const char *inDetails, ...)
{
    if (((SFoundationLogLevel::LOG_FATAL)&gLogMask) == 0)
        return;
    LOG_FORMAT_DETAILS(SFoundationLogLevel::LOG_FATAL);
    LogMessage(SFoundationLogLevel::LOG_FATAL, inTag, inMessage, &formatted[0]);
}

#undef LOG_MAX_LENGTH
}

FY_RESTORE_VC_WARNING()