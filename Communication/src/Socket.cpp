/**
 * @brief This class contains
 */


#include <functional>
#include <cassert>
#include <iostream>
#include <memory>
#include "ctlib/Exception.h"
#include "ctlib/Logging.h"

#ifdef PIL_CXX
extern "C++" {
#include <ctlib/Socket.hpp>
extern "C" {
#include "ctlib/Socket.h"
}

#include "ctlib/Threading.hpp"
#include "ctlib/ExceptionHandler.h"
#include <string>

namespace PIL
{
    /**
     * @brief Workaround to pass std::functions to C-acceptCallback function.
     */
    struct ThreadArgCXX {
        /** Old C-threading function. */
        ThreadArg argC = {};
        /** Function pointer to C++ function returning PIL::Socket object. */
        std::function<void(std::shared_ptr<PIL::Socket>&)> acceptCallback = {};
    };

    Socket::Socket(PIL_SOCKET* socket, std::string &ip, uint16_t port) : m_SocketRet(*socket), m_IPAddress(ip), m_Port(port),
    m_TransportProtocol(TCP), m_InternetProtocol(IPv4), m_TimeoutInMS(0), m_LastError(PIL_NO_ERROR){
    }

    Socket::Socket(TransportProtocol transportProtocol, InternetProtocol internetProtocol, const std::string &address,
                   int port, uint16_t timeoutInMS) : m_TransportProtocol(transportProtocol),
                                                     m_InternetProtocol(internetProtocol), m_IPAddress(address),
                                                     m_Port(port), m_TimeoutInMS(timeoutInMS), m_LastError(PIL_NO_ERROR){
        m_LastError = PIL_SOCKET_Create(&m_SocketRet, transportProtocol, internetProtocol, address.c_str(), port);
    }

    Socket::~Socket(){
        Disconnect();
    }

    PIL_ERROR_CODE Socket::Disconnect()
    {
        auto retCode = UnregisterCallbackFunction();
        if(retCode != PIL_NO_ERROR){
#ifdef PIL_EXCEPTION_HANDLING
            throw PIL::Exception(m_LastError, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
            return retCode;
        }
        retCode = PIL_SOCKET_Close(&m_SocketRet);
#ifdef PIL_EXCEPTION_HANDLING
        if (retCode != PIL_NO_ERROR)
            throw PIL::Exception(retCode, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return retCode;
    }


    PIL_ERROR_CODE Socket::Bind(PIL_BOOL reuse)
    {
        m_LastError = PIL_SOCKET_Bind(&m_SocketRet, reuse);
#ifdef PIL_EXCEPTION_HANDLING
        if(m_LastError != PIL_NO_ERROR)
            throw PIL::Exception(m_LastError, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return m_LastError;
    }

    PIL_ERROR_CODE Socket::Listen(int queueSize)
    {
        m_LastError = PIL_SOCKET_Listen(&m_SocketRet, queueSize);
#ifdef PIL_EXCEPTION_HANDLING
        if(m_LastError != PIL_NO_ERROR)
            throw PIL::Exception(m_LastError, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return m_LastError;
    }

    PIL_ERROR_CODE Socket::Accept(char *ipAddr)
    {
        PIL_SOCKET sock;
        m_LastError = PIL_SOCKET_Accept(&m_SocketRet, ipAddr, &sock);
        if (m_LastError != PIL_NO_ERROR)
        {
#ifdef PIL_EXCEPTION_HANDLING
            throw PIL::Exception(m_LastError, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
            return m_LastError;
        }
        m_SocketList.push_back(sock);
        return m_LastError;
    }

    WaitRetValue Socket::WaitTillDataAvailable()
    {
        m_LastError = PIL_SOCKET_WaitTillDataAvail(&m_SocketRet, m_TimeoutInMS);
        switch (m_LastError)
        {
            case PIL_NO_ERROR:
                return PIL_SOCK_SUCCESS;
            case PIL_TIMEOUT:
                return PIL_SOCK_TIMEOUT;
            default:
            {
#ifdef PIL_EXCEPTION_HANDLING
                throw PIL::Exception(m_LastError, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
                return PIL_SOCK_ERROR;
            }
        }
    }

    PIL_ERROR_CODE Socket::Connect(std::string &ipAddr, int port)
    {
        m_LastError = PIL_SOCKET_Connect(&m_SocketRet, ipAddr.c_str(), port, m_TimeoutInMS);
#ifdef PIL_EXCEPTION_HANDLING
        if(m_LastError != PIL_NO_ERROR)
            throw PIL::Exception(m_LastError, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return m_LastError;
    }

    PIL_ERROR_CODE Socket::Receive(uint8_t *buffer, uint32_t *bufferLen)
    {
        m_LastError = PIL_SOCKET_Receive(&m_SocketRet, buffer, bufferLen, m_TimeoutInMS);
#ifdef PIL_EXCEPTION_HANDLING
        if(m_LastError != PIL_NO_ERROR)
            throw PIL::Exception(m_LastError, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return m_LastError;
    }

    PIL_ERROR_CODE Socket::Receive(std::string &retBuffer)
    {
        uint32_t bufferLen = MAX_BUF_LEN;
        uint8_t buffer[2048];
        auto ret = Receive(buffer, &bufferLen);
        retBuffer = std::string((char *)buffer, bufferLen);
        return ret;
    }

    PIL_ERROR_CODE Socket::ReceiveFrom(uint8_t *buffer, int *bufferLen, char *ipAddr, int port)
    {
        m_LastError = PIL_SOCKET_ReceiveFrom(&m_SocketRet, buffer, reinterpret_cast<uint16_t *>(bufferLen), ipAddr,
                                             &port);
#ifdef PIL_EXCEPTION_HANDLING
        if(m_LastError != PIL_NO_ERROR)
            throw PIL::Exception(m_LastError, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return m_LastError;

    }

    PIL_ERROR_CODE Socket::Send(std::string &message){
        int len = static_cast<int>(message.length());
        return Send(reinterpret_cast<const uint8_t*>(message.c_str()), &len);
    }

    PIL_ERROR_CODE Socket::Send(const uint8_t *buffer, int *len)
    {
        m_LastError = PIL_SOCKET_Send(&m_SocketRet, buffer, reinterpret_cast<uint32_t *>(len));
#ifdef PIL_EXCEPTION_HANDLING
        if(m_LastError != PIL_NO_ERROR)
            throw PIL::Exception(m_LastError, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return m_LastError;
    }

    PIL_ERROR_CODE Socket::SendTo(std::string &destAddr, int port, uint8_t *buffer, int *bufferLen)
    {
        m_LastError = PIL_SOCKET_SendTo(&m_SocketRet, destAddr.c_str(), port, buffer,
                                        reinterpret_cast<uint32_t *>(bufferLen));
#ifdef PIL_EXCEPTION_HANDLING
        if(m_LastError != PIL_NO_ERROR)
            throw PIL::Exception(m_LastError, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return m_LastError;
    }

    std::string Socket::GetSenderIP()
    {
        const char *senderIP = PIL_SOCKET_GetSenderIP(&m_SocketRet);
#ifdef PIL_EXCEPTION_HANDLING
        if(!senderIP)
            throw PIL::Exception(m_LastError, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return senderIP;
    }

    PIL_BOOL Socket::IsOpen()
    {
        return PIL_SOCKET_IsOpen(&m_SocketRet);
    }

    void* PIL_AcceptThreadFunctionCXX(void* value)
    {
        std::shared_ptr<PIL::Socket> socketPtr;
        assert(value);

        auto *arg = static_cast<ThreadArgCXX *>(value);
        char ipAddr[MAX_IP_LEN];

        do {
            PIL_SOCKET retHandle;
            int ret = PIL_SOCKET_Accept(&arg->argC.socket, ipAddr, &retHandle);
            if(ret != PIL_NO_ERROR){
#ifdef PIL_EXCEPTION_HANDLING
                throw PIL::Exception(PIL_ERRNO, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
                return nullptr;
            }
            retHandle.m_IsConnected = TRUE;
            std::string ipStr = ipAddr;
            socketPtr = std::move(std::make_shared<PIL::Socket>(&retHandle, ipStr, retHandle.m_port));
            arg->acceptCallback(socketPtr);
        }while(arg->argC.socket.m_IsOpen);
        return arg;
    }

    PIL_ERROR_CODE Socket::RegisterAcceptCallback(std::function<void(std::shared_ptr<PIL::Socket>&)> &f)
    {
        auto *arg = new ThreadArgCXX;
        arg->argC.socket = m_SocketRet;
        arg->argC.acceptCallback = nullptr;
        arg->acceptCallback = f;

        std::function<void*(void*)> threadFunc = PIL_AcceptThreadFunctionCXX;
        PIL::Threading threading(threadFunc, arg);
        threading.Run();

        return PIL_NO_ERROR;
    }


    PIL_ERROR_CODE Socket::CreateServerSocket(std::function<void(std::shared_ptr<PIL::Socket>&)> &receiveCallback)
    {

    m_LastError = PIL_SOCKET_Setup_ServerSocket(&m_SocketRet, m_Port, nullptr);
    if(m_LastError != PIL_NO_ERROR)
        return m_LastError;
    auto ret = RegisterAcceptCallback(receiveCallback);
    if(ret != PIL_NO_ERROR)
    {
#ifdef PIL_EXCEPTION_HANDLING
        throw PIL::Exception(m_LastError, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
    }
        return ret;
        return m_LastError;
    }

    PIL_ERROR_CODE
    Socket::ConnectToServer(std::string &ipAddr, int destPort, std::function<void(std::shared_ptr<Socket>& , std::string &)> &receiveCallback)
    {
        auto functionPtr = [](PIL_SOCKET* socket, uint8_t *buffer, uint32_t bufferLen, void* additionalArg){
            if(!additionalArg){
#ifdef PIL_EXCEPTION_HANDLING
                throw PIL::Exception(PIL_INVALID_ARGUMENTS, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
                return;
            }
            std::string ip = socket->m_IPAddress;
            std::shared_ptr<PIL::Socket> socketCXX(new PIL::Socket(socket, ip, socket->m_port));
            std::string value = std::string((char *)buffer, bufferLen);
            auto *arg = reinterpret_cast<ReceiveCallbackArg*>(additionalArg);
            arg->m_ReceiveCallback(socketCXX, value);
        };

        m_LastError = PIL_SOCKET_ConnectToServer(&m_SocketRet, ipAddr.c_str(), m_Port, destPort, m_TimeoutInMS, functionPtr, &receiveCallback);
#ifdef PIL_EXCEPTION_HANDLING
        if(m_LastError != PIL_NO_ERROR)
            throw PIL::Exception(m_LastError, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return m_LastError;
    }


    PIL_ERROR_CODE Socket::RegisterReceiveCallbackFunction(ReceiveCallbackArg& additionalArg)
    {
        auto callback = [](PIL_SOCKET *socket, uint8_t *buffer, uint32_t bufferLen, void* additionalArg){
            std::string ip = socket->m_IPAddress;
            std::string value = std::string((char *)buffer, bufferLen);
            auto *arg = reinterpret_cast<ReceiveCallbackArg*>(additionalArg);
            arg->m_Socket = std::move(std::make_shared<PIL::Socket>(socket, ip, socket->m_port));
            arg->m_ReceiveCallback(arg->m_Socket, value);
        };

        auto ret = PIL_SOCKET_RegisterReceiveCallbackFunction(&m_SocketRet, callback, &additionalArg);
#ifdef PIL_EXCEPTION_HANDLING
        if(ret != PIL_NO_ERROR)
            throw PIL::Exception(m_LastError, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
            return ret;
    }

    PIL_ERROR_CODE Socket::UnregisterCallbackFunction()
    {
        auto ret = PIL_SOCKET_UnregisterCallbackFunction(&m_SocketRet);
#ifdef PIL_EXCEPTION_HANDLING
        if(ret != PIL_NO_ERROR)
            throw PIL::Exception(m_LastError, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return ret;
    }


    PIL_ERROR_CODE Socket::GetInterfaceInfos(InterfaceInfoList *interfaceInfos)
    {
        auto ret = ::GetInterfaceInfos(&m_SocketRet, interfaceInfos);
#ifdef PIL_EXCEPTION_HANDLING
        if(ret != PIL_NO_ERROR)
            throw PIL::Exception(m_LastError, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return ret;
    }

}
}
#endif // PIL_CXX