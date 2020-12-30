//
// Created by frank55 on 10.11.2020.
//

#include "UARTInterface.h"
#include "ErrorHandler.h"
#include "Logging.h"
#include <string.h>
#include <stdio.h> // sprintf
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

#ifdef __WIN32__
#include <fileapi.h>
#include "windows.h"
#include <winbase.h>
#endif // WIN32

char err[256];

PIL_ERROR_CODE PIL_UART_CreateUartInterface(PIL_UART_Config *config, const char *interface, uint32_t baudrate)
{
    if(!config)
        return PIL_INVALID_ARGUMENTS;

    strcpy(config->m_Interface, interface);
    config->m_Baudrate = baudrate;
    config->m_Open = 0;

    return PIL_NO_ERROR;
}


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

    return PIL_NO_ERROR;
#endif // WIN32
}

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

PIL_ERROR_CODE PIL_UART_ReadData(PIL_UART_Config *config, char *buffer, int *bufferLen)
{
    if(!config)
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
    int baudrate = 0;
    switch(config->m_Baudrate)
    {
        case(110):
            baudrate = B110;
            break;
        case(300):
            baudrate = B300;
            break;
        case(600):
            baudrate = B600;
            break;
        case(1200):
            baudrate = B1200;
            break;
        case(2400):
            baudrate = B2400;
            break;
        case(4800):
            baudrate = B4800;
            break;
        case(9600):
            baudrate = B9600;
            break;
        case(19200):
            baudrate = B19200;
            break;
        case(38400):
            baudrate = B38400;
            break;
        case(57600):
            baudrate = B57600;
            break;
        case(115200):
            baudrate = B115200;
            break;
        case(921600) :
            baudrate = B921600;
            break;
        default:
            //     if (SetCustomBaudrate(m_Baudrate) != -1)
            //       baudrate = B38400;
            // else
               baudrate = 0;
            break;
    }

    struct termios tty;
    memset (&tty, 0, sizeof tty);
    if (tcgetattr (config->m_FileHandle, &tty) != 0)
    {
        return FALSE;
    }

    speed_t bytesize = CS8;

    /*switch(rs232Params.bytesize)
    {
        case 5:
            bytesize = CS5;
            break;
        case 6:
            bytesize = CS6;
            break;
        case 7:
            bytesize = CS7;
            break;
        default:
            break;
    }*/

    if(cfsetospeed (&tty, baudrate) == -1) printf("a"); // TODO

    if(cfsetispeed (&tty, baudrate) == -1) printf("a"); // TODO



    //Equal to cfmakeraw function
    tty.c_iflag &= ~(IGNBRK | BRKINT | PARMRK | ISTRIP
                     | INLCR | IGNCR | ICRNL | IXON);

    tty.c_oflag &= ~OPOST;

    // echo off, echo newline off, canonical mode off,
    // extended input processing off, signal chars off
    tty.c_lflag &= ~(ECHO | ECHONL | ICANON | ISIG | IEXTEN);


    //tty.c_cflag  |= CLOCAL;

    // clear current char size mask, no parity checking,
    // no output processing, force 8 bit input
    tty.c_cflag &= ~(CSIZE | PARENB);
    tty.c_cflag |= bytesize;

    tty.c_cc[VMIN]= 0;
    tty.c_cc[VTIME] = 5;

    tcflush(config->m_FileHandle, TCIOFLUSH);
    if (tcsetattr (config->m_FileHandle, TCSANOW, &tty) != 0)
    {
        PIL_SetLastError(&config->errorHandle, PIL_ERRNO);
        return PIL_ERRNO;
    }
    return PIL_NO_ERROR;
#endif // WIN32
}