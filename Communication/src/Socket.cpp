/**
 * @copyright University of Passau - Chair of Computer Engineering
 * @author Florian Frank
 */
#include "ctlib/Exception.h"
#include "ctlib/Logging.h"

#ifdef PIL_CXX
#include <ctlib/Socket.hpp>
extern "C" {
#include "ctlib/Socket.h"
}

#include "ctlib/Threading.hpp"
#include "ctlib/ExceptionHandler.h"

#include <cassert> // assert

namespace PIL
{
    Socket::Socket(): m_IPAddress(""), m_Port(0),
    m_TransportProtocol(TCP), m_InternetProtocol(IPv4), m_TimeoutInMS(0){

    }

    Socket::Socket(std::shared_ptr<PIL_SOCKET> &socket, std::string &ip, uint16_t port) :
                m_CSocketHandle(socket), m_IPAddress(ip), m_Port(port),
                m_TransportProtocol(TCP), m_InternetProtocol(IPv4), m_TimeoutInMS(0){
    }

    Socket::Socket(TransportProtocol transportProtocol, InternetProtocol internetProtocol, const std::string &address,
                   int port, uint16_t timeoutInMS) : m_TransportProtocol(transportProtocol),
                                                     m_InternetProtocol(internetProtocol), m_IPAddress(address), m_CSocketHandle(std::make_unique<PIL_SOCKET>()),
                                                     m_Port(port), m_TimeoutInMS(timeoutInMS){
        auto ret = PIL_SOCKET_Create(m_CSocketHandle.get(), transportProtocol, internetProtocol, address.c_str(), port);
    if(ret != PIL_NO_ERROR){
#ifdef PIL_EXCEPTION_HANDLING
    throw PIL::Exception(ret, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        }
    }

    Socket::~Socket(){
        Disconnect();
    }

    PIL_ERROR_CODE Socket::Disconnect(){
        auto retCode = UnregisterAllCallbackFunctions();
        if(retCode != PIL_NO_ERROR){
#ifdef PIL_EXCEPTION_HANDLING
            throw PIL::Exception(retCode, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
            return retCode;
        }
        retCode = PIL_SOCKET_Close(m_CSocketHandle.get());
#ifdef PIL_EXCEPTION_HANDLING
        if (retCode != PIL_NO_ERROR)
            throw PIL::Exception(retCode, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return retCode;
    }


    PIL_ERROR_CODE Socket::Bind(PIL_BOOL reuse){
        auto ret = PIL_SOCKET_Bind(m_CSocketHandle.get(), reuse);
#ifdef PIL_EXCEPTION_HANDLING
        if(ret != PIL_NO_ERROR)
            throw PIL::Exception(ret, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return ret;
    }

    PIL_ERROR_CODE Socket::Listen(int queueSize){
        auto ret = PIL_SOCKET_Listen(m_CSocketHandle.get(), queueSize);
#ifdef PIL_EXCEPTION_HANDLING
        if(ret != PIL_NO_ERROR)
            throw PIL::Exception(ret, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return ret;
    }

    PIL_ERROR_CODE Socket::Accept(char *ipAddr){
        std::unique_ptr<PIL_SOCKET> sock = std::make_unique<PIL_SOCKET>();
        auto ret = PIL_SOCKET_Accept(m_CSocketHandle.get(), ipAddr, sock.get());
        if (ret != PIL_NO_ERROR){
#ifdef PIL_EXCEPTION_HANDLING
            throw PIL::Exception(ret, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
            return ret;
        }
        m_SocketList.push_back(std::move(sock));
        return ret;
    }

    WaitRetValue Socket::WaitTillDataAvailable(){
        auto ret = PIL_SOCKET_WaitTillDataAvail(m_CSocketHandle.get(), m_TimeoutInMS);
        switch (ret){
            case PIL_NO_ERROR:
                return PIL_SOCK_SUCCESS;
            case PIL_TIMEOUT:
                return PIL_SOCK_TIMEOUT;
            default:{
#ifdef PIL_EXCEPTION_HANDLING
                throw PIL::Exception(ret, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
                return PIL_SOCK_ERROR;
            }
        }
    }

    PIL_ERROR_CODE Socket::Connect(std::string &ipAddr, int port){
        auto ret = PIL_SOCKET_Connect(m_CSocketHandle.get(), ipAddr.c_str(), port, m_TimeoutInMS);
#ifdef PIL_EXCEPTION_HANDLING
        if(ret != PIL_NO_ERROR)
            throw PIL::Exception(ret, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return ret;
    }

    PIL_ERROR_CODE Socket::Receive(uint8_t *buffer, uint32_t *bufferLen){
        auto ret = PIL_SOCKET_Receive(m_CSocketHandle.get(), buffer, bufferLen, m_TimeoutInMS);
#ifdef PIL_EXCEPTION_HANDLING
        if(ret != PIL_NO_ERROR)
            throw PIL::Exception(ret, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return ret;
    }

    PIL_ERROR_CODE Socket::Receive(std::string &retBuffer){
        uint32_t bufferLen = MAX_BUF_LEN;
        uint8_t buffer[2048];
        auto ret = Receive(buffer, &bufferLen);
        retBuffer = std::string((char *)buffer, bufferLen);
        return ret;
    }

    PIL_ERROR_CODE Socket::ReceiveFrom(uint8_t *buffer, int *bufferLen, char *ipAddr, int port){
        auto ret = PIL_SOCKET_ReceiveFrom(m_CSocketHandle.get(), buffer,
                                          reinterpret_cast<uint16_t *>(bufferLen), ipAddr, &port);
#ifdef PIL_EXCEPTION_HANDLING
        if(ret != PIL_NO_ERROR)
            throw PIL::Exception(ret, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return ret;

    }

    PIL_ERROR_CODE Socket::Send(std::string &message){
        int len = static_cast<int>(message.length());
        return Send(reinterpret_cast<const uint8_t*>(message.c_str()), &len);
    }

    PIL_ERROR_CODE Socket::Send(const uint8_t *buffer, int *len){
        auto ret  = PIL_SOCKET_Send(m_CSocketHandle.get(), buffer, reinterpret_cast<uint32_t *>(len));
#ifdef PIL_EXCEPTION_HANDLING
        if(ret != PIL_NO_ERROR)
            throw PIL::Exception(ret, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return ret;
    }

    PIL_ERROR_CODE Socket::SendTo(std::string &destAddr, int port, uint8_t *buffer, int *bufferLen){
        auto ret = PIL_SOCKET_SendTo(m_CSocketHandle.get(), destAddr.c_str(), port, buffer,
                                        reinterpret_cast<uint32_t *>(bufferLen));
#ifdef PIL_EXCEPTION_HANDLING
        if(ret != PIL_NO_ERROR)
            throw PIL::Exception(ret, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return ret;
    }

    std::string Socket::GetSenderIPAddress(){
        const char *senderIP = PIL_SOCKET_GetSenderIP(m_CSocketHandle.get());
#ifdef PIL_EXCEPTION_HANDLING
        if(!senderIP)
            throw PIL::Exception(PIL_ERRNO, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return senderIP;
    }

    PIL_BOOL Socket::IsOpen(){
        return PIL_SOCKET_IsOpen(m_CSocketHandle.get());
    }

    void* PIL_AcceptThreadFunctionCXX(std::unique_ptr<Socket::ThreadAcceptArg> &arg){
        assert(arg);

        char ipAddr[MAX_IP_LEN];

        std::unique_ptr<PIL::Socket> socketPtr = std::make_unique<PIL::Socket>();
        do {
            if(!arg->argC.socket->m_IsOpen){
                return arg.get();
            }
            auto retHandle = std::make_shared<PIL_SOCKET>();
            retHandle->m_IsOpen = TRUE;
            retHandle->m_ReceiveCallback = FALSE;

            int ret = PIL_SOCKET_Accept(arg->argC.socket, ipAddr, retHandle.get());
            if(ret == PIL_INTERFACE_CLOSED)
                return arg.get();
            if(ret != PIL_NO_ERROR){
#ifdef PIL_EXCEPTION_HANDLING
                throw PIL::Exception(PIL_ERRNO, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
                return nullptr;
            }
            retHandle->m_IsOpen = TRUE;
            retHandle->m_IsConnected = TRUE;
            std::string ipStr = ipAddr;

            socketPtr->setSocketHandle(retHandle);
            socketPtr->setIPAddress(ipStr);
            socketPtr->setPort(retHandle->m_port);

            arg->acceptCallback(socketPtr);
        }while(arg->argC.socket->m_IsOpen);
        return arg.get();
    }

    PIL_ERROR_CODE Socket::RegisterAcceptCallback(std::function<void(std::unique_ptr<PIL::Socket>&)> &f){
        auto threadArg = std::make_unique<ThreadAcceptArg>();
        threadArg->argC.socket = m_CSocketHandle.get();
        threadArg->argC.acceptCallback = nullptr;
        threadArg->acceptCallback = f;
        std::function<void*(std::unique_ptr<ThreadAcceptArg>&)> threadFunc = PIL_AcceptThreadFunctionCXX;
        m_AcceptThread = std::make_unique<PIL::Threading<ThreadAcceptArg>>(threadFunc, threadArg);
        auto ret = m_AcceptThread->Run();
#ifdef PIL_EXCEPTION_HANDLING
        if(ret != PIL_NO_ERROR)
            throw PIL::Exception(ret, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return ret;
    }


    PIL_ERROR_CODE Socket::CreateServerSocket(std::function<void(std::unique_ptr<PIL::Socket>&)> &receiveCallback){
    auto ret = PIL_SOCKET_Setup_ServerSocket(m_CSocketHandle.get(), m_Port, nullptr /*Use C++ callback*/);
    if(ret != PIL_NO_ERROR)
        return ret;
    ret = RegisterAcceptCallback(receiveCallback);
    if(ret != PIL_NO_ERROR){
#ifdef PIL_EXCEPTION_HANDLING
        throw PIL::Exception(ret, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        }
        return ret;
    }

    PIL_ERROR_CODE
    Socket::ConnectToServer(std::string &ipAddr, int destPort, std::function<void(std::shared_ptr<Socket>& , std::string &)> &receiveCallback){
        auto functionPtr = [](PIL_SOCKET* socket, uint8_t *buffer, uint32_t bufferLen, void* additionalArg){
            if(!additionalArg){
#ifdef PIL_EXCEPTION_HANDLING
                throw PIL::Exception(PIL_INVALID_ARGUMENTS, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
                return;
            }
            std::string ip = socket->m_IPAddress;

            // Create new socket object now managed by shared_ptr
            auto sock = new PIL_SOCKET;
            *sock = *socket;

            std::shared_ptr<PIL_SOCKET> s = std::shared_ptr<PIL_SOCKET>(sock);

            // Set old socket to closed!
            socket->m_IsOpen = FALSE;
            socket->m_IsConnected = FALSE;

            s->m_IsOpen = TRUE;
            s->m_ReceiveCallback = FALSE;

            auto socketCXX = std::make_shared<PIL::Socket>(s, ip, socket->m_port);
            std::string value = std::string((char *)buffer, bufferLen);
            auto *callbackFunction = reinterpret_cast<std::unique_ptr<ReceiveCallbackArg>*>(additionalArg);
            (*callbackFunction)->m_ReceiveCallback(socketCXX, value);
        };

        m_ReceiveCallback = std::move(std::make_unique<ReceiveCallbackArg>(receiveCallback));
        auto ret = PIL_SOCKET_ConnectToServer(m_CSocketHandle.get(), ipAddr.c_str(),
                                              m_Port, destPort, m_TimeoutInMS, functionPtr,
                                              &m_ReceiveCallback);
#ifdef PIL_EXCEPTION_HANDLING
        if(ret != PIL_NO_ERROR)
            throw PIL::Exception(ret, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return ret;
    }


    PIL_ERROR_CODE Socket::RegisterReceiveCallbackFunction(ReceiveCallbackArg& additionalArg){
        auto callback = [](PIL_SOCKET *socket, uint8_t *buffer,
                uint32_t bufferLen, void* additionalArg){
            std::string ip = socket->m_IPAddress;
            std::string value = std::string((char *)buffer, bufferLen);
            auto *arg = reinterpret_cast<ReceiveCallbackArg*>(additionalArg);
            auto s = std::shared_ptr<PIL_SOCKET>(socket);
            arg->m_Socket = std::make_shared<PIL::Socket>(s, ip, socket->m_port);
            arg->m_ReceiveCallback(arg->m_Socket, value);
        };

        auto ret = PIL_SOCKET_RegisterReceiveCallbackFunction(m_CSocketHandle.get(), callback, &additionalArg);
#ifdef PIL_EXCEPTION_HANDLING
        if(ret != PIL_NO_ERROR)
            throw PIL::Exception(ret, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return ret;
    }

    PIL_ERROR_CODE Socket::UnregisterAllCallbackFunctions(){
        if(m_CSocketHandle == nullptr)
            return PIL_NO_ERROR;
        auto ret = PIL_SOCKET_UnregisterReceiveCallbackFunction(m_CSocketHandle.get());
#ifdef PIL_EXCEPTION_HANDLING
        if(ret != PIL_NO_ERROR)
            throw PIL::Exception(ret, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return ret;
    }


    PIL_ERROR_CODE Socket::GetInterfaceInfos(InterfaceInfoList *interfaceInfos){
        auto ret = ::GetInterfaceInfos(m_CSocketHandle.get(), interfaceInfos);
#ifdef PIL_EXCEPTION_HANDLING
        if(ret != PIL_NO_ERROR)
            throw PIL::Exception(ret, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
        return ret;
    }

    void Socket::setPort(uint16_t mPort)
    {
        m_Port = mPort;
    }

    void Socket::setIPAddress(const std::string &mIpAddress)
    {
        m_IPAddress = mIpAddress;
    }

    void Socket::setSocketHandle(const std::shared_ptr<PIL_SOCKET> &mCSocketHandle)
    {
        m_CSocketHandle = mCSocketHandle;
    }

}
#endif // PIL_CXX