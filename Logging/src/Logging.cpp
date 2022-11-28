//
// Created by florianfrank on 31.05.22.
//

#include "ctlib/Logging.hpp"
#include <stdarg.h>

/*static*/ ::Level PIL::Logging::PILLOGDefinesToCLogDefines(Level level){
    switch (level)
    {
        case INFO:
            return INFO_LVL;
        case DEBUG:
            return DEBUG_LVL;
        case WARNING:
            return WARNING_LVL;
        case ERROR:
            return ERROR_LVL;
    }
    return INFO_LVL;
}

PIL::Logging::Logging(Level logLevel, std::string *fileName)
{
    if(fileName)
        ::InitializeLogging(PILLOGDefinesToCLogDefines(logLevel), fileName->c_str());
    else
        ::InitializeLogging(PILLOGDefinesToCLogDefines(logLevel), nullptr);
}

PIL::Logging::~Logging()
{
    CloseLogfile();
}

void PIL::Logging::CloseLogFile()
{
    ::CloseLogfile();
}

void PIL::Logging::LogMessage(Level level, const char* fileName, unsigned int lineNumber, std::string message, ...)
{
    va_list vaList;
    va_start(vaList, message);
    ::LogMessageVA(PILLOGDefinesToCLogDefines(level), fileName, lineNumber, message.c_str(), vaList);
    va_end(vaList);
}

PIL::Logging::Logging()
{
    ::InitializeLogging(INFO_LVL, nullptr);
}

void PIL::Logging::Log(PIL::Level level, const char *fileName, unsigned int lineNumber, std::string message)
{
    switch (level)
    {
        case INFO:
            ::LogMessage(INFO_LVL, fileName, lineNumber, message.c_str());
            break;
        case DEBUG:
            ::LogMessage(DEBUG_LVL, fileName, lineNumber, message.c_str());
            break;
        case WARNING:
            ::LogMessage(WARNING_LVL, fileName, lineNumber, message.c_str());
            break;
        case ERROR:
            ::LogMessage(ERROR_LVL, fileName, lineNumber, message.c_str());
            break;
    }

}
