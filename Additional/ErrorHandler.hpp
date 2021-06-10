//
// Created by florianfrank on 30.05.21.
//

#ifndef PLATFORMINDEPENDENTLIB_WRAPPERCLASS_H
#define PLATFORMINDEPENDENTLIB_WRAPPERCLASS_H

#include "ErrorHandler.h"

class ErrorHandler
{
public:
    ErrorHandler(){}

protected:
    PIL_ERROR_CODE m_LastError;
};

#endif //PLATFORMINDEPENDENTLIB_WRAPPERCLASS_H
