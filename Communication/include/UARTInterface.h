//
// Created by frank55 on 10.11.2020.
//

#ifndef AUTOMATIC_TESTER_UARTINTERFACE_H
#define AUTOMATIC_TESTER_UARTINTERFACE_H


#include <stdint.h>
#include <DataTypeDefines.h>
#include "ErrorCodeDefines.h"

#ifdef __WIN32__
#include <ntdef.h>
typedef HANDLE FileHandle;
#else // Linux
typedef int FileHandle;
#endif // WIN32


struct
{
    int m_Baudrate;
    char m_Interface[30];
    FileHandle m_FileHandle;
    PIL_BOOL m_Open;
    PIL_ErrorHandle errorHandle;
} typedef PIL_UART_Config;

    PIL_ERROR_CODE PIL_UART_CreateUartInterface(PIL_UART_Config *config, const char *interface, uint32_t baudrate);
    PIL_ERROR_CODE PIL_UART_Open(PIL_UART_Config *config, PIL_BOOL nonBlocking);
    PIL_ERROR_CODE PIL_UART_Close(PIL_UART_Config *config);
    PIL_ERROR_CODE PIL_UART_ReadData(PIL_UART_Config *config, char* buffer, int *bufferLen);
    PIL_ERROR_CODE PIL_UART_WriteData(PIL_UART_Config *config, const char *buffer, const int *write);
    PIL_ERROR_CODE PIL_UART_SetComParameters(PIL_UART_Config *config);

#endif //AUTOMATIC_TESTER_UARTINTERFACE_H
