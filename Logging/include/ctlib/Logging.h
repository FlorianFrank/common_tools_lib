/**
 * @file This file contains all the functionality used for logging.
 * @addtogroup ASOACryptMiddleware
 */
#pragma once
#define LOGGING_ENABLED 1

#include <string.h> // strrchr
#include <stdarg.h>
#include "ctlib/ErrorCodeDefines.h"

/** Max size of an message to log. **/
#define LOG_BUF_SIZE 10000

/** Remove path from file name. e.g. /home/user/SampleFile.c -> SampleFile.c. */
#ifdef __WIN32__
#define __FILENAME__ (strrchr(__FILE__, '\\') ? strrchr(__FILE__, '\\') + 1 : __FILE__)
#else // Linux
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#endif // __WIN32__

/** Enum storing the loglevel, to indicate the type of message. */
 enum
{
    INFO_LVL,
    DEBUG_LVL,
    ERROR_LVL,
    WARNING_LVL,
    NONE_LVL
} typedef Level;
#ifdef LOGGING_ENABLED

PIL_ERROR_CODE InitializeLogging(Level level, const char *file);

PIL_ERROR_CODE CloseLogfile();

void LogMessage(Level level, const char *fileName, unsigned int lineNumber, const char *message, ...);
void LogMessageVA(Level level, const char *fileName, unsigned int lineNumber, const char *message, va_list vaList);

// Helperfunctions
const char *GetLogLevelStr(Level level);
const char *GetActualTime();

#else // Logging Disabled
/** Dummy Defines when logging is disabled do nothing. */
#define InitializeLogging(level, file)
#define LogMessage(level, fileName, lineNumber, message, ...)
#define CloseLogfile()
#define convertHostID(x)
#endif // LOGGING_ENABLED
/*
 * };
 * */
