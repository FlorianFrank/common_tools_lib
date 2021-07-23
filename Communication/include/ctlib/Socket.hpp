#ifdef CXX
#pragma once
#include <string>
#include <vector>

extern "C"
{
    #include "ctlib/SocketDefines.h"
}

enum WaitRetValue {
    SUCCESS,
    TIMEOUT,
    ERROR
};

namespace PIL
{

    class Socket
    {
    public:
        Socket(TransportProtocol
        transportProtocol,
        InternetProtocol internetProtocol,
        const std::string &address,
        int port
        );

        ~Socket();

        PIL_ERROR_CODE Bind(PIL_BOOL reuse);

        PIL_ERROR_CODE Listen(int queueSize);

        PIL_ERROR_CODE Accept(char *ipAddr);

        WaitRetValue WaitTillDataAvailable(int timeOut);

        PIL_ERROR_CODE Connect(std::string &ipAddr, int port);

        PIL_ERROR_CODE Receive(uint8_t *buffer, uint32_t *bufferLen);

        PIL_ERROR_CODE ReceiveFrom(uint8_t *buffer, int *bufferLen, char *ipAddr, int port);

        PIL_ERROR_CODE Send(uint8_t *buffer, int *len);

        PIL_ERROR_CODE SendTo(std::string &destAddr, int port, uint8_t *buffer, int *bufferLen);

        std::string GetSenderIP();

        PIL_BOOL IsOpen();

        PIL_ERROR_CODE  CreateServerSocket(void (*receiveCallback)(PIL_SOCKET, char *));

        PIL_ERROR_CODE  ConnectToServer(std::string ipAddr, int destPort, void (*receiveCallback)(uint8_t *, uint32_t));

    private:
        int m_Port;
        std::string m_IPAddrress;
        TransportProtocol m_TransportProtocol;
        InternetProtocol m_InternetProtocol;
        PIL_ERROR_CODE m_LastError;

        PIL_SOCKET m_SocketRet;
        std::vector<PIL_SOCKET> m_SocketList;
        // TODO ring buffer
    };

}
#endif // CXX