//
// Created by florianfrank on 01.07.22.
//

#ifndef INSTRUMENT_CONTROL_LIB_EXCEPTIONHANDLER_H
#define INSTRUMENT_CONTROL_LIB_EXCEPTIONHANDLER_H

extern "C" {
#include "ErrorHandler.h"
};

#include <exception>


class ExceptionHandler: public std::exception
{
public:
    ExceptionHandler(PIL_ErrorHandle errHandle);
    ExceptionHandler(PIL_ERROR_CODE errorCode);

    const char * what () const throw ();

private:
    PIL_ErrorHandle m_ErrorHandle;
};


#endif //INSTRUMENT_CONTROL_LIB_EXCEPTIONHANDLER_H
