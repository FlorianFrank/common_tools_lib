/**
 * @copyright University of Passau - Chair of Computer Engineering
 * @author Florian Frank
 */
#ifdef PIL_CXX
#pragma once

extern "C" {
#include "ctlib/SocketDefines.h"
}
#include "ctlib/Threading.hpp"

#include <vector> // std::vector
#include <functional> // std::function
#include <memory> // std::shared_ptr

namespace PIL
{
#define MAX_BUF_LEN 2048

    class Socket
    {
    public:

        struct ReceiveCallbackArg {
            explicit ReceiveCallbackArg(std::function<void(std::shared_ptr<PIL::Socket>&, std::string&)>& c) : m_ReceiveCallback(c) {
            }
            std::function<void(std::shared_ptr<PIL::Socket>&, std::string&)>& m_ReceiveCallback;
            std::shared_ptr<PIL::Socket> m_Socket = {};
        };

        /**
         * @brief Workaround to pass std::functions to C-acceptCallback function.
         */
        struct ThreadAcceptArg
        {
            /** Old C-threading function. */
            AcceptThreadArgC argC = {};
            /** Function pointer to C++ function returning PIL::Socket object. */
            std::function<void(std::unique_ptr<PIL::Socket>&)> acceptCallback = {};
        };

        Socket();
        Socket(TransportProtocol transportProtocol, InternetProtocol internetProtocol, const std::string& address,
            int port, uint16_t timeoutInMS);
        Socket(std::shared_ptr<PIL_SOCKET>& socket, std::string& ip, uint16_t port);
        ~Socket();

        PIL_ERROR_CODE Bind(PIL_BOOL reuse);
        PIL_ERROR_CODE Listen(int queueSize);
        PIL_ERROR_CODE Accept(char* ipAddr);

        WaitRetValue WaitTillDataAvailable();

        PIL_ERROR_CODE Connect(std::string& ipAddr, int port);
        PIL_ERROR_CODE Disconnect();


        PIL_ERROR_CODE Receive(std::string& retBuffer);
        PIL_ERROR_CODE Receive(uint8_t* buffer, uint32_t* bufferLen);
        PIL_ERROR_CODE ReceiveFrom(uint8_t* buffer, int* bufferLen, char* ipAddr, int port);

        PIL_ERROR_CODE Send(const uint8_t* buffer, int* len);
        PIL_ERROR_CODE Send(std::string& message);
        PIL_ERROR_CODE SendTo(std::string& destAddr, int port, uint8_t* buffer, int* bufferLen);

        PIL_ERROR_CODE CreateServerSocket(std::function<void(std::unique_ptr<PIL::Socket>&)>& receiveCallback);
        PIL_ERROR_CODE ConnectToServer(std::string& ipAddr, int destPort, std::function<void(std::shared_ptr<Socket>&, std::string&)>& receiveCallback);

        PIL_ERROR_CODE RegisterReceiveCallbackFunction(ReceiveCallbackArg& additionalArg);
        PIL_ERROR_CODE UnregisterAllCallbackFunctions();

        std::string GetSenderIPAddress();
        PIL_ERROR_CODE GetInterfaceInfos(InterfaceInfoList* interfaceInfos);
        PIL_BOOL IsOpen();
        TransportProtocol GetTransportProtocol() const { return m_TransportProtocol; }
        InternetProtocol GetInternetProtocol() const { return m_InternetProtocol; }
        void setPort(uint16_t mPort);
        void setIPAddress(const std::string& mIpAddress);
        void setSocketHandle(const std::shared_ptr<PIL_SOCKET>& mCSocketHandle);

    private:
        uint16_t m_Port;
        std::string m_IPAddress;
        TransportProtocol m_TransportProtocol;
        InternetProtocol m_InternetProtocol;
        uint16_t m_TimeoutInMS;


        std::shared_ptr<PIL_SOCKET> m_CSocketHandle;
        std::vector<std::unique_ptr<PIL_SOCKET>> m_SocketList;
        // std::unique_ptr<ThreadAcceptArg> m_ThreadArg;
        std::unique_ptr<PIL::Threading<ThreadAcceptArg>> m_AcceptThread;
        std::unique_ptr<ReceiveCallbackArg> m_ReceiveCallback;
        PIL_ERROR_CODE RegisterAcceptCallback(std::function<void(std::unique_ptr<PIL::Socket>&)>& f);
    };

}
#endif // PIL_CXX