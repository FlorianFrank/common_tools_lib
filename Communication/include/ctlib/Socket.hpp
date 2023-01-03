#ifdef PIL_CXX
#pragma once
#include <string>
#include <vector>
#include <functional>
#include <memory>

extern "C"
{
    #include "ctlib/SocketDefines.h"
}

enum WaitRetValue {
    PIL_SOCK_SUCCESS,
    PIL_SOCK_TIMEOUT,
    PIL_SOCK_ERROR
};

namespace PIL
{

#define MAX_BUF_LEN 2048

    class Socket
    {
    public:
        Socket(TransportProtocol transportProtocol, InternetProtocol internetProtocol, const std::string &address,
               int port, uint16_t timeoutInMS);

        Socket(PIL_SOCKET *socket, std::string &ip, uint16_t port);

        ~Socket();

        struct ReceiveCallbackArg {
            explicit ReceiveCallbackArg(std::function<void(std::shared_ptr<PIL::Socket>&, std::string&)>& c): m_ReceiveCallback(c)
            {
            }
            std::function<void(std::shared_ptr<PIL::Socket>&, std::string&)> &m_ReceiveCallback;
            std::shared_ptr<PIL::Socket> m_Socket = {};
        };

        PIL_ERROR_CODE Close();

        PIL_ERROR_CODE Bind(PIL_BOOL reuse);

        PIL_ERROR_CODE Listen(int queueSize);

        PIL_ERROR_CODE Accept(char *ipAddr);

        WaitRetValue WaitTillDataAvailable();

        PIL_ERROR_CODE Connect(std::string &ipAddr, int port);

        PIL_ERROR_CODE Receive(std::string &retBuffer);

        PIL_ERROR_CODE Receive(uint8_t *buffer, uint32_t *bufferLen);

        PIL_ERROR_CODE ReceiveFrom(uint8_t *buffer, int *bufferLen, char *ipAddr, int port);

        PIL_ERROR_CODE Send(const uint8_t *buffer, int *len);

        PIL_ERROR_CODE Send(std::string &message);

        PIL_ERROR_CODE SendTo(std::string &destAddr, int port, uint8_t *buffer, int *bufferLen);

        std::string GetSenderIP();

        PIL_BOOL IsOpen();

        PIL_ERROR_CODE CreateServerSocket(std::function<void(std::shared_ptr<PIL::Socket>&)> &receiveCallback);

        PIL_ERROR_CODE ConnectToServer(std::string &ipAddr, int destPort, std::function<void(std::shared_ptr<Socket>& , std::string &)> &receiveCallback);

        PIL_ERROR_CODE RegisterReceiveCallbackFunction(ReceiveCallbackArg& additionalArg);

        PIL_ERROR_CODE UnregisterCallbackFunction();

        PIL_ERROR_CODE GetInterfaceInfos(InterfaceInfoList *interfaceInfos);


        inline PIL_ERROR_CODE GetLastError() { return m_LastError; };

        TransportProtocol GetTransportProtocol() const { return m_TransportProtocol; }

        InternetProtocol GetInternetProtocol() const { return m_InternetProtocol; }




    private:
        uint16_t m_Port;
        std::string m_IPAddrress;
        TransportProtocol m_TransportProtocol;
        InternetProtocol m_InternetProtocol;
        uint16_t m_TimeoutInMS;
        PIL_ERROR_CODE m_LastError;

        PIL_SOCKET m_SocketRet{};
        std::vector<PIL_SOCKET> m_SocketList;

        PIL_ERROR_CODE RegisterAcceptCallback(std::function<void(std::shared_ptr<PIL::Socket>&)> &f);
    };

}
#endif // PIL_CXX