//
// Created by frank55 on 10.11.2020.
//

#include "UARTInterface.h"
#include "Logging.h"
#include <cstring>
#include <cstdio> // sprintf
#include <string>

#ifdef __WIN32__
#include <fileapi.h>
#include "windows.h"
#include <winbase.h>
#endif // WIN32

char err[256];

UARTInterface::UARTInterface(const char *comInterf, int baudrate) : m_Baudrate(baudrate)
{
    strcpy(m_Interface, comInterf);
}


bool UARTInterface::Open()
{
#ifdef __WIN32__
    char comBuff[20];
    if (sprintf(comBuff, R"(\\.\%s)", m_Interface) == -1)
    {
        LogMessage(ERROR_LVL, __FILENAME__, __LINE__, "sprintf returned with error %s", strerror(errno));
        return false;
    }
    m_FileHandle = ::CreateFile(comBuff, (unsigned long)GENERIC_READ | (unsigned long)GENERIC_WRITE, 0,
                                nullptr, OPEN_EXISTING, FILE_FLAG_WRITE_THROUGH, nullptr);
    if (m_FileHandle == INVALID_HANDLE_VALUE)
    {
        LogMessage(ERROR_LVL, __FILENAME__, __LINE__, "Create File (%s) returned with error %s", comBuff, GetErrorMessage());
        return false;
    }
    LogMessage(DEBUG_LVL, __FILENAME__, __LINE__, "Open file %s successfully", comBuff);
    return true;
#else // Linux
    // TODO fopen
#endif // WIN32
}

void UARTInterface::Close()
{
    LogMessage(DEBUG_LVL, __FILENAME__, __LINE__, "Close file");
#ifdef __WIN32__
    ::CloseHandle(m_FileHandle);
#endif // __WIN32__
}

void UARTInterface::ReadData(char *buffer, int *bufferLen)
{
    int maxBufferLen = *bufferLen;
#ifdef __WIN32__
    ::ReadFile(m_FileHandle, buffer, maxBufferLen, reinterpret_cast<LPDWORD>(bufferLen), nullptr);
#else // Linux

#endif // WIN32
}

void UARTInterface::WriteData(const char *buffer, int *write)
{
#ifdef __WIN32__
    DWORD written;
    ::WriteFile(m_FileHandle, buffer, *write, &written, nullptr);
    *write = static_cast<int>(written);
#else // Linux

#endif // WIN32
}

bool UARTInterface::SetComParameters()
{
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

#endif // WIN32
}

char* UARTInterface::GetErrorMessage()
{
#ifdef __WIN32__
    FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, nullptr, GetLastError(),
                  MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), err, 255, nullptr);
    return err;
#else // __linux__

#endif // __WIN32__
}
