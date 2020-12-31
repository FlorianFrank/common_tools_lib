//
// Created by florianfrank on 29.12.20.
//

#ifndef PIL_ERROR_CODE_DEFINES_H
#define PIL_ERROR_CODE_DEFINES_H

#include <stdint.h>

#define MAX_ERROR_MSG_LEN 128

enum
{
    PIL_NO_ERROR, PIL_INVALID_ARGUMENTS, PIL_ERRNO,

    /* Socket errors */
    PIL_SOCKET_TIMEOUT, PIL_INTERFACE_CLOSED,

    PIL_INVALID_BAUDRATE

} typedef PIL_ERROR_CODE;


struct
{
    PIL_ERROR_CODE errorCode;
    uint32_t errnoCode;
    char errorMessage[MAX_ERROR_MSG_LEN];
} typedef PIL_ErrorHandle;

#endif //PIL_ERROR_CODE_DEFINES_H
