//
// Created by florianfrank on 29.12.20.
//

#ifndef PLATFORMINDEPENDENTLIB_ERRORHANDLER_H
#define PLATFORMINDEPENDENTLIB_ERRORHANDLER_H

#include <ctlib/ErrorCodeDefines.h>
#include <ctlib/DataTypeDefines.h>
#include <stdint.h>

const char* PIL_ErrorCodeToString(PIL_ERROR_CODE errorCode);

PIL_BOOL PIL_SetLastError(PIL_ErrorHandle *socketStruct, PIL_ERROR_CODE errorCode);

PIL_BOOL PIL_SetLastErrorMsg(PIL_ErrorHandle *errStruct, PIL_ERROR_CODE errorCode, const char* errorMessage);

const char* PIL_ReturnErrorMessageAsString(PIL_ErrorHandle *errorHandle);


#endif //PLATFORMINDEPENDENTLIB_ERRORHANDLER_H
