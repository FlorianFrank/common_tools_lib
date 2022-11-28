#ifdef PIL_LOGGING

#include "ctlib/Logging.h"

#include <stdarg.h> // va_start, va_end
#include <stdlib.h>
#include <errno.h> // errno
#include <stdio.h>
#include <sys/time.h>

#include "ctlib/Logging.h"
#include "ctlib/ThreadingDefines.h"
#include <pthread.h> // mutex

#ifdef __WIN32__
#include <windows.h>
#endif // __WIN32__


/** Mutex to provide mutual exclution when log function is called by different threads. */
pthread_mutex_t logMutex;

#ifdef __linux
// Color codes for different log levels
#define COLOR_DEFAULT   39 // black
#define COLOR_INFO      32 // black
#define COLOR_ERROR     31 // red
#define COLOR_WARNING   33 // yellow
#define COLOR_DEBUG     34 // blue
#endif // __linux__


/** Initialization of variables. */
Level logLevel;
FILE *logFileStream = NULL;
char *loggingBuffer = NULL;

#define TIME_STRING_MAX_SIZE 64
#define MESSAGE_OFFSET       256

#ifdef __linux__
int GetColorCode(Level level);
#endif // __linux__


/**
 * @brief Set log level. Only messages, with that log level or higher are logged.
 * Additionally the log file is set. If the logfile is set to null, print to stdout.
 * @param level every message of that level or with a higher level is printed.
 * @param file path and filename of the logfile, if file is null, log to stdout.
 */
PIL_ERROR_CODE InitializeLogging(const Level level, const char *file)
{
    logLevel = level;

    if (file != NULL)
    {
        // Define logfile
        logFileStream = fopen(file, "ab+");
        if(logFileStream == NULL)
            printf("Error could not open logfile %s (Error %s)\n",
                    file, strerror(errno));
    } else
        // Log to std::out if file is null
        logFileStream = stdout;

    if(loggingBuffer)
        return PIL_DOUBLE_INITIALIZE;
    loggingBuffer = (char*)malloc(LOG_BUF_SIZE * sizeof(char));
    printf("Logging initialized\n");
#if __linux__
    if(pthread_mutex_init(&logMutex, NULL) != 0)
        printf("Error could not initialize logMutex\n");
#endif // __linux__
    return PIL_NO_ERROR;
}

void LogMessageVA(Level level, const char *fileName, unsigned int lineNumber, const char *message, va_list vaList)
{
    vsnprintf(&loggingBuffer[MESSAGE_OFFSET], LOG_BUF_SIZE - MESSAGE_OFFSET -1,  message, vaList);
    LogMessage(level, fileName, lineNumber, &loggingBuffer[MESSAGE_OFFSET]);
}

/**
 * @brief Log to output stream. (Either to file or to stdout, depending on the initialization)
 * LogFile has format HH:MM:SS:microseconds LogLevel FileName:LineNumber Message
 * @param level Level of the message.
 * @param fileName Name of the file, from which the logging function is called. Simply set #__FILENAME__
 * @param lineNumber Line number from which the function is called. Simply use #_LINE_
 * @param message Message to send.
 * @param ... Additional parameters, which are send with the message in a printf style.
 */
void LogMessage(const Level level, const char *fileName,
                         unsigned int lineNumber, const char *message, ...)
{
if(logFileStream == NULL)
{
    printf("Logging enabled but not initialized!\n");
    return;
}
 //   Threading::AutoMutex am(&mutex);
#if defined( __linux__) || defined(__WIN32__)
    if(pthread_mutex_lock(&logMutex) != 0)
        printf("Error while calling pthread_mutex_lock\n");
#endif // __linux__


    if (level >= logLevel)
    {

        if(!loggingBuffer)
        {
            printf("Logging not initialized!\n");
            return;
        }
        va_list vaList;
        va_start(vaList, message);
        int ret = vsprintf(&loggingBuffer[MESSAGE_OFFSET], message, vaList);
        va_end(vaList);
        if (ret > 0)
        {
#ifdef __linux__
            if(logFileStream == stdout || logFileStream == stderr)
            {
                fprintf(logFileStream, "%s\033[%dm %s\033[%dm %s:%u %s\n",
                        GetActualTime(), GetColorCode(level), GetLogLevelStr(level), COLOR_DEFAULT, fileName, lineNumber,
                        &loggingBuffer[MESSAGE_OFFSET]);
            }else
            {
                fprintf(logFileStream, "%s %s %s:%u %s\n",
                        GetActualTime(), GetLogLevelStr(level), fileName, lineNumber,
                        &loggingBuffer[MESSAGE_OFFSET]);
            }

#else // __WIN32__
            fprintf(logFileStream, "%s %s %s:%d %s\r\n",
                    GetActualTime(), GetLogLevelStr(level), fileName, lineNumber,
                    &loggingBuffer[160]);
#endif // __linux__
            fflush(stdout);

        }
    }
#if defined(__linux__) || defined(__WIN32__)
    if(pthread_mutex_unlock(&logMutex) != 0)
        printf("Error while calling pthread_mutex_unlock\n");
#endif // __linux__
}


/**
 * @brief This function closes the file stream if opened.
 */
PIL_ERROR_CODE CloseLogfile(void)
{
    if(logFileStream != stdout && logFileStream != NULL)
    {
        int errCode = fclose(logFileStream);
        if (errCode != 0)
            return PIL_INTERFACE_CLOSED; // TODO
    }
    free(loggingBuffer);
    loggingBuffer = NULL;
    return PIL_NO_ERROR;
}

/**
 * Return current time as string in format HH:MM:SS:microseconds.
 * @return string containing the time.
 */
const char *GetActualTime(void)
{
#ifdef __linux__
    struct timeval time = { 0 };
    gettimeofday(&time, NULL);

    size_t size = strftime(loggingBuffer, TIME_STRING_MAX_SIZE, "%H:%M:%S", localtime(&time.tv_sec));
    if (size <= 0)
        printf("ERROR");

        //sprintf(loggingBuffer, "%s:%li", loggingBuffer, time.tv_usec); TODO
#endif // WIN32
#ifdef __WIN32__
    SYSTEMTIME t;
    GetSystemTime(&t);
    sprintf(&loggingBuffer[80], "%d:%d:%d", t.wHour, t.wMinute, t.wSecond); // TODO error handling
#endif // __linux__

    return loggingBuffer;
}

/**
 * @brief Return loglevel to string.
 * @param level Loglevel to convert to a string.
 * @return loglevel as string.
 */
const char *GetLogLevelStr(Level level)
{
    switch (level)
    {
        case (INFO_LVL):
            return "INFO";
        case (DEBUG_LVL):
            return "DEBUG";
        case (ERROR_LVL):
            return "ERROR";
        case (WARNING_LVL):
            return "WARNING";
        case (NONE_LVL): default:
            return "NONE";
    }
}

#ifdef __linux__
/**
 * @brief Return Color code depending on the Log level (Debug: blue, Error: red, Warning: yellow)
 * @param level loglevel to which the color code corresponds.
 * @return color code.
 */
int GetColorCode(Level level)
{
    switch (level)
    {
        case (INFO_LVL):
            return COLOR_INFO;
        case (DEBUG_LVL):
            return COLOR_DEBUG;
        case (ERROR_LVL):
            return COLOR_ERROR;
        case (WARNING_LVL):
            return COLOR_WARNING;
        case (NONE_LVL): default:
            return COLOR_DEFAULT;
    }
}
#endif // __linux__
#endif // PIL_LOGGING
