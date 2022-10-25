/**
 * @brief This file contains enums and defines required for a platform independent UART implementation.
 * @author Florian Frank
 */
#ifndef PLATFORM_INDEPENDENT_LIB_UART_DEFINES_H
#define PLATFORM_INDEPENDENT_LIB_UART_DEFINES_H

#include <ctlib/DataTypeDefines.h>
#include "ctlib/ErrorCodeDefines.h"
#include <stdint.h>

#ifdef __WIN32__
#include <ntdef.h>
typedef HANDLE FileHandle;
#else // Linux
typedef int FileHandle;
#endif // __WIN32__

/** @brief Define the number of stop bits on a UART data frame.
 * One or two stop bits are possible.*/
enum {
    StopBits1,
    StopBits2
} typedef StopBits;

/** @brief Define the byte size used for a UART data frame. Byte sizes of 5, 6, 7 or 8 bit are possible. */
enum {
    ByteSize5,
    ByteSize6,
    ByteSize7,
    ByteSize8
} typedef ByteSize;

/** @brief Defines if a parity bit should be set and if defined if even or odd parity should be used. */
enum {
    NoParity,
    Even,
    Odd
} typedef Parity;

/**
 * @brief UART handle containing all important properties of an UART interface.
 * */
struct  {
    /** File handle, e.g. on Linux /dev/ttyUSBO on Windows, e.g. COM3 */
    FileHandle m_FileHandle;
    /** Name of the interface e.g. COM3. */
    char m_Interface[30];
    /** The Baudrate of the interface, e.g. 115200. */
    int m_Baudrate;
    /** The amount of Stop bits. */
    StopBits m_StopBits;
    /** The parity, none, even or odd. */
    Parity m_Parity;
    /** The byte size of UART data packages. */
    ByteSize m_ByteSize;
    /** True if the connection is open other the value contains False. */
    PIL_BOOL m_Open;
    /** An error handle which can be requested to get the last error */
    PIL_ErrorHandle errorHandle;
} typedef PIL_UART_Config;

#endif //PLATFORM_INDEPENDENT_LIB_UART_DEFINES_H
