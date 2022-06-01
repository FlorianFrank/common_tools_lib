
 //
// Created by florianfrank on 29.12.20.
//

#include <ctlib/DataTypeDefines.h>
#include <memory.h>
#include <stdio.h>
#include "ctlib/ErrorHandler.h"
#include "errno.h"

#if __WIN32__
#include <sec_api/string_s.h>
#endif

char errMsgBuff[512];

 const char* PIL_ErrorCodeToString(PIL_ERROR_CODE errorCode)
 {
     switch (errorCode)
     {
         case PIL_NO_ERROR:
             return "Success";
         case PIL_INVALID_ARGUMENTS:
             return "Invalid arguments";
         case PIL_TIMEOUT:
            return "Socket timeout";
         case PIL_ERRNO:
         {
#ifdef __WIN32__
             char errnoBUF[50];
             strerror_s(errnoBUF, 101 /*TODO*/, errno);
             sprintf(errMsgBuff, "Errno %d (%s)", errno, errnoBUF);
#else
             sprintf(errMsgBuff, "Errno %d (%s)", errno, strerror(errno));
#endif
             return errMsgBuff;
         }
         case PIL_INTERFACE_CLOSED:
             return "Socket is closed";
         case PIL_INVALID_BAUDRATE:
             return "Baudrate not supported";
         case PIL_INSUFFICIENT_RESOURCES:
             return "Insufficient resources";
         case PIL_DEADLOCK_DETECTED:
             return "Deadlock detected";
         case PIL_THREAD_NOT_JOINABLE:
             return "Thread not joinable";
         case PIL_THREAD_NOT_FOUND:
             return "Thread not found";
         case PIL_ONLY_PARTIALLY_READ_WRITTEN:
             return "File only partially written";
         case PIL_NO_SUCH_FILE:
             return "No such file or directory";
         default:
             return "Unknown error";
     }
     return "Unknown error";
 }

 PIL_BOOL PIL_ReturnErrorMessage(char *errorStr, PIL_ErrorHandle *socketStruct)
 {
    if(!errorStr)
        return FALSE;

    if(!socketStruct)
    {
        strcpy(errorStr, "socketStruct == NULL");
        return FALSE;
    }

    const char *errCodeStr = PIL_ErrorCodeToString(socketStruct->m_ErrorCode);
    if(socketStruct->m_ErrorCode == PIL_ERRNO)
    {
        sprintf(errorStr, "%s: %s", errCodeStr, strerror(socketStruct->m_ErrnoCode));
        return TRUE;
    }
    if(strcmp(socketStruct->m_ErrorMessage, "") != 0)
    {
        sprintf(errorStr, "%s: %s", errCodeStr, socketStruct->m_ErrorMessage);
        return TRUE;
    }
    strcpy(errorStr, errCodeStr);
    return TRUE;
 }

 PIL_BOOL PIL_SetLastError(PIL_ErrorHandle *errStruct, PIL_ERROR_CODE errorCode)
 {
     errStruct->m_ErrorCode = errorCode;
    if(errorCode == PIL_ERRNO)
    {
        errStruct->m_ErrnoCode = errno;
    }
    strcpy(errStruct->m_ErrorMessage, "");
     return TRUE;
 }

 PIL_BOOL PIL_SetLastErrorMsg(PIL_ErrorHandle *errStruct, PIL_ERROR_CODE errorCode, const char* errorMessage)
 {
    if(!errStruct)
        return FALSE;

    if(strlen(errorMessage) > MAX_ERROR_MSG_LEN)
        return FALSE;

    strcpy(errStruct->m_ErrorMessage, errorMessage);
     errStruct->m_ErrorCode = errorCode;

    return TRUE;
 }

 const char* PIL_ReturnErrorMessageAsString(PIL_ErrorHandle *errorHandle)
 {
     PIL_ReturnErrorMessage(errMsgBuff, errorHandle);
     return errMsgBuff;
 }

