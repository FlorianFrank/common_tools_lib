//
// Created by frank55 on 10.11.2020.
//

#ifndef AUTOMATIC_TESTER_UARTINTERFACE_H
#define AUTOMATIC_TESTER_UARTINTERFACE_H


#include <stdint.h>

#ifdef __WIN32__
#include <ntdef.h>
typedef HANDLE FileHandle;
#else // Linux
typedef int FileHandle;
#endif // WIN32

typedef uint8_t BOOL;
#define FALSE 0
#define TRUE 1

struct PIL_UART_Config
{
    int m_Baudrate;
    char m_Interface[30];
    FileHandle m_FileHandle;
    BOOL m_Open;
} typedef PIL_UART_Config;

    int PIL_UART_CreateUartInterface(PIL_UART_Config *config, const char *interface, int baudrate);
    BOOL PIL_UART_Open(PIL_UART_Config *config);
    BOOL PIL_UART_Close(PIL_UART_Config *config);
    BOOL PIL_UART_ReadData(PIL_UART_Config *config, char* buffer, int *bufferLen);
    BOOL PIL_UART_WriteData(PIL_UART_Config *config, const char *buffer, const int *write);
    BOOL PIL_UART_SetComParameters(PIL_UART_Config *config);

    char* GetErrorMessage();



#endif //AUTOMATIC_TESTER_UARTINTERFACE_H
