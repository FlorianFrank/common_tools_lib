//
// Created by florianfrank on 31.05.22.
//

#include "ctlib/Logging.hpp"
#include "ctlib/ErrorCodeDefines.h"
#include <stdarg.h>

PIL::Logging::Logging(Level logLevel, std::string &fileName)
{
    InitializeLogging(logLevel, fileName.c_str());
}

PIL::Logging::~Logging()
{
    CloseLogfile();
}

void PIL::Logging::CloseLogFile()
{
    ::CloseLogfile();
}

void PIL::Logging::LogMessage(Level level, const char* fileName, unsigned int lineNumber, const char* message, ...)
{
    va_list vaList;
    va_start(vaList, message);
    ::LogMessage(level, fileName, lineNumber, message, vaList);
}
