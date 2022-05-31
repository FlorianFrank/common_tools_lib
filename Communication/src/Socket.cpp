//
// Created by florianfrank on 04.05.21.
//
#ifdef CXX

extern "C++" {

#include <ctlib/Socket.hpp>

extern "C" {
#include "ctlib/Socket.h"
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
        Close();
    }

    PIL_ERROR_CODE Socket::Close()
    {
        m_LastError = PIL_SOCKET_Close(&m_SocketRet);
        return m_LastError;
    }


    PIL_ERROR_CODE Socket::Bind(PIL_BOOL reuse)
    {
        m_LastError = PIL_SOCKET_Bind(&m_SocketRet, reuse);
        return m_LastError;
    }

    PIL_ERROR_CODE Socket::Listen(int queueSize)
    {
        m_LastError = PIL_SOCKET_Listen(&m_SocketRet, queueSize);
        return m_LastError;
    }

    PIL_ERROR_CODE Socket::Accept(char *ipAddr)
    {
        PIL_SOCKET sock;
        m_LastError = PIL_SOCKET_Accept(&m_SocketRet, ipAddr, &sock);
        if(m_LastError != PIL_NO_ERROR)
            return m_LastError;
        m_SocketList.push_back(sock);
        return m_LastError;
    }

    WaitRetValue Socket::WaitTillDataAvailable(int timeOut)
    {
        m_LastError = PIL_SOCKET_WaitTillDataAvail(&m_SocketRet, timeOut);
        switch (m_LastError)
        {
            case PIL_NO_ERROR:
                return SUCCESS;
            case PIL_TIMEOUT:
                return TIMEOUT;
            default:
                return ERROR;
        }
    }

    PIL_ERROR_CODE Socket::Connect(std::string &ipAddr, int port)
    {
        m_LastError = PIL_SOCKET_Connect(&m_SocketRet, ipAddr.c_str(), port);
        return m_LastError;
    }

    PIL_ERROR_CODE Socket::Receive(uint8_t *buffer, uint32_t *bufferLen)
    {
        m_LastError = PIL_SOCKET_Receive(&m_SocketRet, buffer, bufferLen);
        return m_LastError;
    }

    PIL_ERROR_CODE Socket::ReceiveFrom(uint8_t *buffer, int *bufferLen, char *ipAddr, int port)
    {
        m_LastError = PIL_SOCKET_ReceiveFrom(&m_SocketRet, buffer, reinterpret_cast<uint16_t *>(bufferLen), ipAddr,
                                             &port);
        return m_LastError;

    }

    PIL_ERROR_CODE Socket::Send(uint8_t *buffer, int *len)
    {
        m_LastError = PIL_SOCKET_Send(&m_SocketRet, buffer, reinterpret_cast<uint32_t *>(len));
        return m_LastError;
    }

    PIL_ERROR_CODE Socket::SendTo(std::string &destAddr, int port, uint8_t *buffer, int *bufferLen)
    {
        m_LastError = PIL_SOCKET_SendTo(&m_SocketRet, destAddr.c_str(), port, buffer,
                                        reinterpret_cast<uint32_t *>(bufferLen));
        return m_LastError;
    }

    std::string Socket::GetSenderIP()
    {
        return PIL_SOCKET_GetSenderIP(&m_SocketRet);
    }

    PIL_BOOL Socket::IsOpen()
    {
        return PIL_SOCKET_IsOpen(&m_SocketRet);
    }

    PIL_ERROR_CODE Socket::CreateServerSocket(void (*receiveCallback)(PIL_SOCKET, char *))
    {
        m_LastError = PIL_SOCKET_Setup_ServerSocket(&m_SocketRet, m_Port, receiveCallback);
        return m_LastError;
    }

    PIL_ERROR_CODE  Socket::ConnectToServer(std::string ipAddr, int destPort, void (*receiveCallback)(uint8_t *, uint32_t))
    {
        m_LastError = PIL_SOCKET_ConnectToServer(&m_SocketRet, ipAddr.c_str(), m_Port, destPort, receiveCallback);
        return m_LastError;
    }
}
}
#endif // CXX