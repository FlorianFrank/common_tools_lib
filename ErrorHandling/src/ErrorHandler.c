
 //
// Created by florianfrank on 29.12.20.
//

#include <DataTypeDefines.h>
#include <memory.h>
#include <stdio.h>
#include "../include/ErrorHandler.h"
#include "errno.h"

 const char *PIL_ErrorCodeToString(PIL_ERROR_CODE errorCode)
 {
/*     switch (errorCode)
     {

         case PIL_NO_ERROR:
             break;
         case PIL_INVALID_ARGUMENTS:
             break;
         case PIL_SOCKET_TIMEOUT:
             break;
         case PIL_ERRNO:
             break;
     }*/
     return "Unknown error";
 }

 PIL_BOOL PIL_SetLastError(PIL_ErrorHandle *socketStruct, PIL_ERROR_CODE errorCode)
 {
    socketStruct->errorCode = errorCode;
    if(errorCode == PIL_ERRNO)
    {
        socketStruct->errnoCode = errno;
    }
     return TRUE;
 }

 PIL_BOOL PIL_SetLastErrorMsg(PIL_ErrorHandle *socketStruct, PIL_ERROR_CODE errorCode, const char* errorMessage)
 {
    if(!socketStruct)
        return FALSE;

    if(strlen(errorMessage) > MAX_ERROR_MSG_LEN)
        return FALSE;

    strcpy(socketStruct->errorMessage, errorMessage);
    socketStruct->errorCode = errorCode;

    return TRUE;
 }
