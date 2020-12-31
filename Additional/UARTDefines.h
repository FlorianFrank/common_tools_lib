//
// Created by florianfrank on 31.12.20.
//

#ifndef PLATFORMINDEPENDENTLIB_UARTDEFINES_H
#define PLATFORMINDEPENDENTLIB_UARTDEFINES_H

#include <stdint.h>
#include <DataTypeDefines.h>
#include "ErrorCodeDefines.h"

#ifdef __WIN32__
#include <ntdef.h>
typedef HANDLE FileHandle;
#else // Linux
typedef int FileHandle;
#endif // WIN32



enum {
    StopBits5,
    StopBits8
} typedef StopBits;

enum {
    ByteSize5,
    ByteSize6,
    ByteSize7,
    ByteSize8
} typedef ByteSize;

enum{
    NoParity,
    Even,
    Odd
} typedef Parity;

struct
{
    FileHandle m_FileHandle;
    char m_Interface[30];
    int m_Baudrate;
    StopBits m_StopBits;
    Parity m_Parity;
    ByteSize m_ByteSize;
    PIL_BOOL m_Open;
    PIL_ErrorHandle errorHandle;

} typedef PIL_UART_Config;

#endif //PLATFORMINDEPENDENTLIB_UARTDEFINES_H
