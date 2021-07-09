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

        bool Bind(PIL_BOOL reuse);

        bool Listen(int queueSize);

        void Accept(char *ipAddr);

        WaitRetValue WaitTillDataAvailable(int timeOut);

        bool Connect(std::string &ipAddr, int port);

        bool Receive(uint8_t *buffer, uint32_t *bufferLen);

        bool ReceiveFrom(uint8_t *buffer, int *bufferLen, char *ipAddr, int port);

        bool Send(uint8_t *buffer, int *len);

        bool SendTo(std::string &destAddr, int port, uint8_t *buffer, int *bufferLen);

        std::string GetSenderIP();

        PIL_BOOL IsOpen();

        bool CreateServerSocket(void (*receiveCallback)(PIL_SOCKET, char *));

        bool ConnectToServer(std::string ipAddr, int destPort, void (*receiveCallback)(uint8_t *, uint32_t));

    private:
        int m_Port;
        std::string m_IPAddrress;
        TransportProtocol m_TransportProtocol;
        InternetProtocol m_InternetProtocol;
        PIL_ERROR_CODE m_LastError;

        PIL_SOCKET m_SocketRet{};
        std::vector<PIL_SOCKET> m_SocketList;
        // TODO ring buffer
    };

}
#endif // CXX