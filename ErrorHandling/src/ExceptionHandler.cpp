//
// Created by florianfrank on 01.07.22.
//


#include "ctlib/ExceptionHandler.h"


ExceptionHandler::ExceptionHandler(PIL_ErrorHandle errHandle): m_ErrorHandle(errHandle)
{
}

ExceptionHandler::ExceptionHandler(PIL_ERROR_CODE errorCode)
{
    m_ErrorHandle.m_ErrorCode = errorCode;
}

const char *ExceptionHandler::what() const throw()
{
        return PIL_ErrorCodeToString(m_ErrorHandle.m_ErrorCode);
}
