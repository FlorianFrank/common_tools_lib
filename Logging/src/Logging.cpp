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

void PIL::Logging::LogMessage(Level level, std::string &fileName, unsigned int lineNumber, std::string &message, ...)
{
    va_list vaList;
    va_start(vaList, message);
    ::LogMessage(level, fileName.c_str(), lineNumber, message.c_str(), vaList);
}
