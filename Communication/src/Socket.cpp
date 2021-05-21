//
// Created by florianfrank on 04.05.21.
//
#ifdef CXX

extern "C++" {

#include <Socket.hpp>

extern "C" {
#include "Socket.h"
}

#include <string>

namespace PIL
{

    Socket::Socket(TransportProtocol transportProtocol, InternetProtocol internetProtocol,
                           const std::string &address, int port) : m_TransportProtocol(transportProtocol),
                                                                   m_InternetProtocol(internetProtocol),
                                                                   m_IPAddrress(address), m_Port(port)
    {
        m_LastError = PIL_SOCKET_Create(&m_SocketRet, transportProtocol, internetProtocol, address.c_str(), port);
    }

    Socket::~Socket()
    {
        PIL_SOCKET_Close(&m_SocketRet);
    }

    bool Socket::Bind(PIL_BOOL reuse)
    {
        m_LastError = PIL_SOCKET_Bind(&m_SocketRet, reuse);
        return m_LastError == PIL_NO_ERROR ? true : false;
    }

    bool Socket::Listen(int queueSize)
    {
        m_LastError = PIL_SOCKET_Listen(&m_SocketRet, queueSize);
        return m_LastError == PIL_NO_ERROR ? true : false;
    }

    void Socket::Accept(char *ipAddr)
    {
        PIL_SOCKET sock;
        m_LastError = PIL_SOCKET_Accept(&m_SocketRet, ipAddr, &sock);
        m_SocketList.push_back(sock);
    }

    WaitRetValue Socket::WaitTillDataAvailable(int timeOut)
    {
        m_LastError = PIL_SOCKET_WaitTillDataAvail(&m_SocketRet, timeOut);
        switch (m_LastError)
        {
            case PIL_NO_ERROR:
                return SUCCESS;
            case PIL_SOCKET_TIMEOUT:
                return TIMEOUT;
            default:
                return ERROR;
        }
    }

    bool Socket::Connect(std::string &ipAddr, int port)
    {
        m_LastError = PIL_SOCKET_Connect(&m_SocketRet, ipAddr.c_str(), port);
        return m_LastError == PIL_NO_ERROR ? true : false;
    }

    bool Socket::Receive(uint8_t *buffer, uint32_t *bufferLen)
    {
        m_LastError = PIL_SOCKET_Receive(&m_SocketRet, buffer, bufferLen);
        return m_LastError == PIL_NO_ERROR ? true : false;
    }

    bool Socket::ReceiveFrom(uint8_t *buffer, int *bufferLen, char *ipAddr, int port)
    {
        m_LastError = PIL_SOCKET_ReceiveFrom(&m_SocketRet, buffer, reinterpret_cast<uint16_t *>(bufferLen), ipAddr,
                                             &port);
        return m_LastError == PIL_NO_ERROR ? true : false;

    }

    bool Socket::Send(uint8_t *buffer, int *len)
    {
        m_LastError = PIL_SOCKET_Send(&m_SocketRet, buffer, reinterpret_cast<uint32_t *>(len));
        return m_LastError == PIL_NO_ERROR ? true : false;
    }

    bool Socket::SendTo(std::string &destAddr, int port, uint8_t *buffer, int *bufferLen)
    {
        m_LastError = PIL_SOCKET_SendTo(&m_SocketRet, destAddr.c_str(), port, buffer,
                                        reinterpret_cast<uint32_t *>(bufferLen));
        return m_LastError == PIL_NO_ERROR ? true : false;
    }

    std::string Socket::GetSenderIP()
    {
        return PIL_SOCKET_GetSenderIP(&m_SocketRet);
    }

    PIL_BOOL Socket::IsOpen()
    {
        return PIL_SOCKET_IsOpen(&m_SocketRet);
    }

    bool Socket::CreateServerSocket(void (*receiveCallback)(PIL_SOCKET, char *))
    {
        m_LastError = PIL_SOCKET_Setup_ServerSocket(&m_SocketRet, m_Port, receiveCallback);
        return m_LastError == PIL_NO_ERROR ? true : false;
    }

    bool Socket::ConnectWithServer(std::string &ipAddr, int destPort, void (*receiveCallback)(PIL_SOCKET *, char *))
    {
        m_LastError = PIL_SOCKET_ConnectToServer(&m_SocketRet, ipAddr.c_str(), m_Port, destPort, receiveCallback);
        return m_LastError == PIL_NO_ERROR ? true : false;
    }

}
}
#endif // CXX
