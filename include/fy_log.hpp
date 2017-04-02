#ifndef FY_GUIDANCE_LOG_H
#define FY_GUIDANCE_LOG_H

#include <stdlib.h>

namespace fy
{

struct SFoundationLogLevel
{
    enum Enum
    {
        LOG_DEBUG = 0x01,
        LOG_TRACE = 0X02,
        LOG_INFO = 0X04,
        LOG_WARN = 0X08,
        LOG_ERROR = 0X10,
        LOG_FATAL = 0X20,
        LOG_ALL = 0XFF,
    };
};

class Log
{
  public:
    static void EnableLog(SFoundationLogLevel::Enum, bool isEnable);
    static void LogMessage(SFoundationLogLevel::Enum, const char *inTag, const char *inMessage, const char *inDetails);

    //convenience callbacks
    static void EnableDebug(bool isEnable) { EnableLog(SFoundationLogLevel::LOG_DEBUG, isEnable); }
    static void EnableTrace(bool isEnable) { EnableLog(SFoundationLogLevel::LOG_TRACE, isEnable); }
    static void EnableInfo(bool isEnable) { EnableLog(SFoundationLogLevel::LOG_INFO, isEnable); }
    static void EnableWarn(bool isEnable) { EnableLog(SFoundationLogLevel::LOG_WARN, isEnable); }
    static void EnableError(bool isEnable) { EnableLog(SFoundationLogLevel::LOG_ERROR, isEnable); }
    static void EnableFatal(bool isEnable) { EnableLog(SFoundationLogLevel::LOG_FATAL, isEnable); }
    static void EnableAll(bool isEnable) { EnableLog(SFoundationLogLevel::LOG_ALL, isEnable); }

    static void LogDebug(const char *inTag, const char *inMessage, const char *inDetails, ...);
    static void LogTrace(const char *inTag, const char *inMessage, const char *inDetails, ...);
    static void LogInfo(const char *inTag, const char *inMessage, const char *inDetails, ...);
    static void LogWarn(const char *inTag, const char *inMessage, const char *inDetails, ...);
    static void LogError(const char *inTag, const char *inMessage, const char *inDetails, ...);
    static void LogFatal(const char *inTag, const char *inMessage, const char *inDetails, ...);
};
}

#endif