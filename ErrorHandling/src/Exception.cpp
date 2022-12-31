
#include "ctlib/Exception.h"
#include <sstream>
#include <iostream>
#include <stdarg.h>

PIL::Exception::Exception(PIL_ErrorHandle errHandle)
{
    std::stringstream stringStream;
    stringStream << "Exception: " << PIL_ReturnErrorMessageAsString(&errHandle);
    m_ErrorMessage = stringStream.str();
}

PIL::Exception::Exception(PIL_ERROR_CODE errorCode)
{
    std::stringstream stringStream;
    stringStream << "Exception: " << PIL_ErrorCodeToString(errorCode);
    m_ErrorMessage = stringStream.str();
}

PIL::Exception::Exception(PIL_ERROR_CODE errorCode, std::string errorMessage)
{
    m_ErrorMessage = "Exception: " + std::string(PIL_ErrorCodeToString(errorCode)) + " (" + std::move(errorMessage) + ")";
}

PIL::Exception::Exception(PIL_ERROR_CODE errorCode, const char *fileName, unsigned int lineNumber,
                          const std::string message, ...)
{
    va_list args;
    va_start(args, message);
    setErrorMessageVarArgs(errorCode, fileName, lineNumber, message, args);
    va_end(args);
}

PIL::Exception::Exception(PIL_ERROR_CODE errorCode, const char *fileName, unsigned int lineNumber,
                          const std::string &message, va_list vaList)
{
    setErrorMessageVarArgs(errorCode, fileName, lineNumber, message, vaList);
}

void PIL::Exception::setErrorMessageVarArgs(PIL_ERROR_CODE errorCode, const char *fileName, unsigned int lineNumber,
                                            const std::string &message, va_list vaList)
{
    std::stringstream stringStream;
    stringStream << "Exception (" << fileName << ":" << lineNumber << "): ";
    stringStream << PIL_ErrorCodeToString(errorCode);

    if(!message.empty()) {
    char formatStrBuffer[MAX_ERROR_MESSAGE_LENGTH];
        vsprintf(formatStrBuffer, message.c_str(), vaList);
        stringStream << " (" << formatStrBuffer << ")";
    }

    m_ErrorMessage = stringStream.str();
}

const char *PIL::Exception::what() const noexcept
{
    return m_ErrorMessage.c_str();
}