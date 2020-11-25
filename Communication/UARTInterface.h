//
// Created by frank55 on 10.11.2020.
//

#ifndef AUTOMATIC_TESTER_UARTINTERFACE_H
#define AUTOMATIC_TESTER_UARTINTERFACE_H


#ifdef __WIN32__
#include <ntdef.h>
typedef HANDLE FileHandle;
#else // Linux
typedef int FileHandle;
#endif // WIN32

class UARTInterface
{
public:
    UARTInterface(const char *interface, int baudrate);
    bool Open();
    void Close();
    void ReadData(char* buffer, int *bufferLen);
    void WriteData(const char *buffer, int *write);
    bool SetComParameters();
private:
    int m_Baudrate;
    char m_Interface[30]{};
    FileHandle m_FileHandle{};

    char* GetErrorMessage();

};


#endif //AUTOMATIC_TESTER_UARTINTERFACE_H
