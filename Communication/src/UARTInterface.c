//
// Created by frank55 on 10.11.2020.
//

#include "ctlib/UARTInterface.h"
#include "ctlib/ErrorHandler.h"
#include "ctlib/Logging.h"
#include <string.h>
#include <stdio.h> // sprintf
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/select.h>

#ifdef __linux__
#include <linux/serial_core.h>
#endif // __linux__

#ifdef __WIN32__
#include <fileapi.h>
#include "windows.h"
#include <winbase.h>
#endif // WIN32

/**
 * @brief Creates a UART handle, sets the default parameters as well as the interface.
 * @param config UART handle which is returned.
 * @param interface Interface to open, for example, /dev/ttyUSB0 or COM3.
 * @param baudrate Baudrate to set, e.g. 9600.
 * @return PIL_NO_ERROR if function was successful.
 */
PIL_ERROR_CODE PIL_UART_CreateUartInterface(PIL_UART_Config *config, const char *interface, uint32_t baudrate)
{
    if(!config)
        return PIL_INVALID_ARGUMENTS;

    strcpy(config->m_Interface, interface);
    config->m_Baudrate = baudrate;

    // Set default parameters
    config->m_StopBits = StopBits1;
    config->m_Parity = NoParity;
    config->m_ByteSize = ByteSize8;

    config->m_Open = 0;

    return PIL_NO_ERROR;
}


/**
 * @brief Opens the UART interface which must be specified by the PIL_UART_CreateUartInterface function.
 * @param config configuration file previously returned by calling PIL_UART_CreateUartInterface.
 * @param nonBlocking if true, interface is opened in none blocking mode, else in blocking mode.
 * @return PIL_NO_ERROR if function was successful.
 */
PIL_ERROR_CODE PIL_UART_Open(PIL_UART_Config *config, PIL_BOOL nonBlocking)
{
    if(!config)
        return PIL_INVALID_ARGUMENTS;

#ifdef __WIN32__
    char comBuff[20];
    if (sprintf(comBuff, R"(\\.\%s)", config->m_Interface) == -1)
    {
        LogMessage(ERROR_LVL, __FILENAME__, __LINE__, "sprintf returned with error %s", strerror(errno));
        return false;
    }
    config->m_FileHandle = ::CreateFile(comBuff, (unsigned long)GENERIC_READ | (unsigned long)GENERIC_WRITE, 0,
                                nullptr, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, nullptr);
    if (config->m_FileHandle == INVALID_HANDLE_VALUE)
    {
        LogMessage(ERROR_LVL, __FILENAME__, __LINE__, "Create File (%s) returned with error %s", comBuff, GetErrorMessage());
        return false;
    }
    LogMessage(DEBUG_LVL, __FILENAME__, __LINE__, "Open file %s successfully", comBuff);
    return true;
#else // Linux
    uint32_t flags = (uint32_t)O_RDWR | ((nonBlocking) ? (uint32_t)O_NONBLOCK : (uint16_t)0);
    config->m_FileHandle = open(config->m_Interface, flags);
    if(config->m_FileHandle < 0)
    {
        PIL_SetLastError(&config->errorHandle, PIL_ERRNO);
        return PIL_ERRNO;
    }
    config->m_Open = TRUE;
    return PIL_UART_SetComParameters(config);
#endif // WIN32
}

/**
 * @brief Closes the UART interface.
 * @param config handle corresponding to the interface to close.
 * @return PIL_NO_ERROR if function was successful.
 */
PIL_ERROR_CODE PIL_UART_Close(PIL_UART_Config *config)
{
    if(!config)
        return PIL_INVALID_ARGUMENTS;
#ifdef __WIN32__
    ::CloseHandle(m_FileHandle);
#else
    if(close(config->m_FileHandle) == -1)
    {
        PIL_SetLastError(&config->errorHandle, PIL_ERRNO);
        return PIL_ERRNO;
    }
#endif // __WIN32__
return PIL_NO_ERROR;
}

/**
 * @brief Read data from an opened UART interface.
 * @param config handle corresponding to the opened UART interface.
 * @param buffer buffer to store the received data.
 * @param bufferLen input: pointer to the max buffer length. Output: actually read data.
 * @return PIL_NO_ERROR if function was successful.
 */
PIL_ERROR_CODE PIL_UART_ReadData(PIL_UART_Config *config, char *buffer, int *bufferLen)
{
    if(!config || !bufferLen || *bufferLen <= 0)
        return PIL_INVALID_ARGUMENTS;

    if(!config->m_Open)
    {
        PIL_SetLastError(&config->errorHandle, PIL_INTERFACE_CLOSED);
        return PIL_INTERFACE_CLOSED;
    }
#ifdef __WIN32__
    ReadFile(m_FileHandle, buffer, maxBufferLen, (LPDWORD)bufferLen, nullptr);
#else // Linux

    int readRet = read(config->m_FileHandle, buffer, *bufferLen);
    if(readRet < 0)
    {
        PIL_SetLastError(&config->errorHandle, PIL_ERRNO);
        return PIL_ERRNO;
    }
    *bufferLen = readRet;
#endif // WIN32
    return PIL_NO_ERROR;
}



#if defined(__linux__)
/**
 * @brief Function to approximate a baudrate not predefined. Baudrates up to a deviation of 2% are accepted.
 * @param config handle corresponding to the opened UART interface and containing the baudrate to set.
 * @return PIL_NO_ERROR if function was successful.
 */
int SetCustomBaudrate(PIL_UART_Config *config)
{
    struct serial_struct serialInfo;
    /* Custom divisor */
    serialInfo.reserved_char[0] = 0;
    if (ioctl(config->m_FileHandle, TIOCGSERIAL, &serialInfo) < 0)
        return -1;
    serialInfo.flags = ASYNC_SPD_CUST | ASYNC_LOW_LATENCY; // NOLINT(hicpp-signed-bitwise)
    serialInfo.custom_divisor = (serialInfo.baud_base + (config->m_Baudrate / 2)) / config->m_Baudrate;
    if (serialInfo.custom_divisor < 1)
        serialInfo.custom_divisor = 1;
    if (ioctl(config->m_FileHandle, TIOCSSERIAL, &serialInfo) < 0)
        return -1;
    if (serialInfo.custom_divisor * config->m_Baudrate != serialInfo.baud_base) {
        //printf("actual baudrate is %d / %d = %f", serialInfo.baud_base, serialInfo.custom_divisor,
        //	(float)serialInfo.baud_base / serialInfo.custom_divisor);
        return 0;

    }
    return -1;
}
#endif

/**
 * @brief Function to write data via an opened UART interface.
 * @param config handle corresponding to the opened UART interface and containing the baudrate to set.
 * @param buffer Buffer to send.
 * @param buffSize input: pointer to buffer size to send. Output:  size actually written.
 * @return PIL_NO_ERROR if function was successful.
 */
PIL_ERROR_CODE PIL_UART_WriteData(PIL_UART_Config *config, const char *buffer, const int *buffSize)
{
    if(!config)
        return PIL_INVALID_ARGUMENTS;
#ifdef __WIN32__
    DWORD written;
    ::WriteFile(m_FileHandle, buffer, *write, &written, nullptr);
    *write = (int)written;
#else // Linux
    int writeRet = write(config->m_FileHandle, buffer, *buffSize);
    if (writeRet < 0)
    {
        PIL_SetLastError(&config->errorHandle, PIL_ERRNO);
        return PIL_ERRNO;
    }
#endif // WIN32
    return PIL_NO_ERROR;
}

/**
 * @brief Sets parameters like the baudrate, stopbits, bytesize and parity bit of the uart interface.
 * @param config config file identifying the interface and contains the parameters.
 * @return PIL_NO_ERROR if no error occures.
 */
PIL_ERROR_CODE PIL_UART_SetComParameters(PIL_UART_Config *config)
{
    if(!config)
        return PIL_INVALID_ARGUMENTS;
#ifdef __WIN32__
    DCB commDCB;
    commDCB.DCBlength = sizeof(DCB);
    commDCB.BaudRate = m_Baudrate;
    commDCB.ByteSize = 8;
    commDCB.Parity = 0; // None

    commDCB.fBinary = true;
    commDCB.fParity = true;
    commDCB.fOutxCtsFlow = false;
    commDCB.fOutxDsrFlow = false;
    commDCB.fDsrSensitivity = false;
    commDCB.fTXContinueOnXoff = false;
    commDCB.fOutX = false;
    commDCB.fInX = false;
    commDCB.fNull = false;
    commDCB.fRtsControl = false;
    commDCB.fAbortOnError = false;
    commDCB.fDtrControl = false;

    if (::SetCommState(m_FileHandle, &commDCB) == 0)
    {
        LogMessage(ERROR_LVL, __FILENAME__, __LINE__, "Error function SetCommState returns with error %s.",
                   GetErrorMessage());
        return false;
    }

    COMMTIMEOUTS timeOut;
    timeOut.ReadIntervalTimeout = MAXDWORD;
    timeOut.ReadTotalTimeoutMultiplier = 0;
    timeOut.ReadTotalTimeoutConstant = 1000; // TODO
    if(::SetCommTimeouts(m_FileHandle, &timeOut) == 0)
    {
        LogMessage(ERROR_LVL, __FILENAME__, __LINE__, "Error function SetCommTimeouts returns with error %s",
                   GetErrorMessage());
        return false;
    }
    return true;
#else // Linux
    uint32_t baudRate;
    switch(config->m_Baudrate)
    {
        case(110):
            baudRate = B110;
            break;
        case(300):
            baudRate = B300;
            break;
        case(600):
            baudRate = B600;
            break;
        case(1200):
            baudRate = B1200;
            break;
        case(2400):
            baudRate = B2400;
            break;
        case(4800):
            baudRate = B4800;
            break;
        case(9600):
            baudRate = B9600;
            break;
        case(19200):
            baudRate = B19200;
            break;
        case(38400):
            baudRate = B38400;
            break;
        case(57600):
            baudRate = B57600;
            break;
        case(115200):
            baudRate = B115200;
            break;
        case(921600) :
            baudRate = B921600;
            break;
        case(2000000):
            baudRate = B2000000;
            break;
        case(4000000):
            baudRate = B4000000;
            break;
        default:
            // Approximate baudRate by clock divider
            if (SetCustomBaudrate(config) != -1)
                baudRate = B38400;
            else
            {
                PIL_SetLastErrorMsg(&config->errorHandle, PIL_INVALID_BAUDRATE,
                                    "Custom baudrate cannot be approximated");
                return PIL_INVALID_BAUDRATE;
            }
            break;
    }

    // Retrieve last config
    struct termios tty;
    memset(&tty, 0, sizeof tty);
    if (tcgetattr (config->m_FileHandle, &tty) != 0)
    {
        PIL_SetLastError(&config->errorHandle, PIL_ERRNO);
        return PIL_ERRNO;
    }

    // Set output baudrate
    if(cfsetospeed (&tty, baudRate) == -1)
    {
        PIL_SetLastError(&config->errorHandle, PIL_ERRNO);
        return PIL_ERRNO;
    }

    // Set input baudrate
    if(cfsetispeed (&tty, baudRate) == -1)
    {
        PIL_SetLastError(&config->errorHandle, PIL_ERRNO);
        return PIL_ERRNO;
    }

    //Equal to cfmakeraw function
    tty.c_iflag &= (uint32_t) (~((uint32_t) IGNBRK | (uint32_t) BRKINT | (uint32_t) PARMRK | (uint32_t) ISTRIP |
                                 (uint32_t) INLCR | (uint32_t) IGNCR | (uint32_t) ICRNL | (uint32_t) IXON));

    tty.c_oflag &= (uint32_t) (~(uint32_t) OPOST);

    // echo off, echo newline off, canonical mode off,
    // extended input processing off, signal chars off
    tty.c_lflag &= (uint32_t) (~((uint32_t) ECHO | (uint32_t) ECHONL | (uint32_t) ICANON | (uint32_t) ISIG |
                                 (uint32_t) IEXTEN) | ((config->m_StopBits == StopBits2) ? CSTOPB : 0)
                                         );



    // Enable parity check and set parity even or odd
    tty.c_cflag &= (uint32_t) (~((uint32_t) CSIZE | ((config->m_Parity != NoParity) ? ((uint32_t) PARENB) : 0u) |
                                 ((config->m_Parity == Odd) ? (uint32_t) PARODD : (uint32_t) 0u)));
    speed_t byteSize = CS8;
    switch(config->m_ByteSize)
    {
        case ByteSize5:
            byteSize = CS5;
            break;
        case ByteSize6:
            byteSize = CS6;
            break;
        case ByteSize7:
            byteSize = CS7;
            break;
        case ByteSize8:
            byteSize = CS8;
            break;
        default:
            break;
    }

    tty.c_cflag |= byteSize;

    // TODO parameter
    tty.c_cc[VMIN] = 0;  // amount of byte to read until the read function returns
    tty.c_cc[VTIME] = 5; // Timeout in 10th of a second

    tcflush(config->m_FileHandle, TCIOFLUSH);
    if (tcsetattr(config->m_FileHandle, TCSANOW, &tty) != 0)
    {
        PIL_SetLastError(&config->errorHandle, PIL_ERRNO);
        return PIL_ERRNO;
    }
    return PIL_NO_ERROR;
#endif // WIN32
}

/**
 * @brief Function waits until data is available without using busy waiting.
 * @param config Config file to identify the UART interface.
 * @param timeoutMS timeout in milliseconds.
 * @return PIL_NO_ERROR if no error occures.
 */
PIL_ERROR_CODE PIL_UART_WaitForDataAvail(PIL_UART_Config *config, uint32_t timeoutMS)
{
#ifndef embedded
    if (!config)
        return PIL_INVALID_ARGUMENTS;

    fd_set readFD = {0};
    struct timeval timeout = {0};

    // set select write handle to zero
    FD_ZERO(&readFD);
    FD_SET(config->m_FileHandle, &readFD);

    timeout.tv_usec = (timeoutMS % 1000) * 1000;
    timeout.tv_sec = (timeoutMS - (timeoutMS % 1000)) / 1000;
    int ret = select(config->m_FileHandle + 1, &readFD, NULL, NULL, &timeout);
    if (ret == -1)
        return PIL_ERRNO;
    if(ret == 0)
        return PIL_TIMEOUT;
    return PIL_NO_ERROR;
#else
    return 0;
#endif // !embedded
}
