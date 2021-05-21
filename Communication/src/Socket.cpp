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

PIL_Socket::PIL_Socket(TransportProtocol transportProtocol, InternetProtocol internetProtocol, std::string &address,
                       int port)
{
    PIL_ERROR_CODE ret = PIL_SOCKET_Create(&m_SocketRet, transportProtocol, internetProtocol, address.c_str(), port);
    if (ret != PIL_NO_ERROR)
    {
        // ERROR Handling
    }
}

PIL_Socket::~PIL_Socket()
{
    PIL_ERROR_CODE ret = PIL_SOCKET_Close(&m_SocketRet);
    if (ret != PIL_NO_ERROR)
    {}
    // Exception
}

void PIL_Socket::Bind(PIL_BOOL reuse)
{
    PIL_ERROR_CODE ret = PIL_SOCKET_Bind(&m_SocketRet, reuse);
    if (ret != PIL_NO_ERROR)
    {
        // Exception
    }
}

void PIL_Socket::Listen(int queueSize)
{
    PIL_ERROR_CODE ret = PIL_SOCKET_Listen(&m_SocketRet, queueSize);
    if (ret != PIL_NO_ERROR)
    {
        // Exception
    }
}

void PIL_Socket::Accept(char *ipAddr)
{

    // TODO add to list
    //PIL_ERROR_CODE ret = PIL_SOCKET_Accept(&m_SocketRet, ipAddr, PIL_SOCKET *newHandle);
}

void PIL_Socket::WaitTillDataAvailable(int timeOut)
{
    PIL_ERROR_CODE ret = PIL_SOCKET_WaitTillDataAvail(&m_SocketRet, timeOut);
    if (ret != PIL_NO_ERROR)
    {
        // TODO error handling
    }

}

void PIL_Socket::Connect(std::string &ipAddr, int port)
{
    PIL_ERROR_CODE ret = PIL_SOCKET_Connect(&m_SocketRet, ipAddr.c_str(), port);
    if (ret != PIL_NO_ERROR)
    {

    }
}

void PIL_Socket::Receive(uint8_t *buffer, uint32_t *bufferLen)
{
    PIL_ERROR_CODE ret = PIL_SOCKET_Receive(&m_SocketRet, buffer, bufferLen);
    if (ret != PIL_NO_ERROR)
    {

    }
}

void PIL_Socket::ReceiveFrom(uint8_t *buffer, int *bufferLen, char *ipAddr, int port)
{
    // TODO
    PIL_ERROR_CODE ret = PIL_SOCKET_ReceiveFrom(&m_SocketRet, buffer, reinterpret_cast<uint16_t *>(bufferLen),
                                                ipAddr, &port);
    if (ret != PIL_NO_ERROR)
    {
        // Error handling
    }
}

void PIL_Socket::Send(uint8_t *buffer, int *len)
{
    PIL_ERROR_CODE ret = PIL_SOCKET_Send(&m_SocketRet, buffer, reinterpret_cast<uint32_t *>(len));
    if (ret != PIL_NO_ERROR)
    {
        //
    }

}

void PIL_Socket::SendTo(std::string &destAddr, int port, uint8_t *buffer, int *bufferLen)
{
    PIL_ERROR_CODE ret = PIL_SOCKET_SendTo(&m_SocketRet, destAddr.c_str(), port, buffer,
                                           reinterpret_cast<uint32_t *>(bufferLen));
    if (ret != PIL_NO_ERROR)
    {
        // Ret
    }
}

std::string PIL_Socket::GetSenderIP()
{
    return PIL_SOCKET_GetSenderIP(&m_SocketRet);
}

PIL_BOOL PIL_Socket::IsOpen()
{
    return PIL_SOCKET_IsOpen(&m_SocketRet);
}

void PIL_Socket::CreateServerSocket(int port, void (*receiveCallback)(PIL_SOCKET, char *))
{
    PIL_ERROR_CODE ret = PIL_SOCKET_Setup_ServerSocket(&m_SocketRet, port, receiveCallback);
    if (ret != PIL_NO_ERROR)
    {
        // Return
    }

}

void PIL_Socket::ConnectWithServer(std::string &ipAddr, int srcPort, int destPort,
                                   void (*receiveCallback)(struct PIL_SOCKET *retHandle, char *ip))
{
    PIL_ERROR_CODE ret = PIL_SOCKET_ConnectToServer(&m_SocketRet, ipAddr.c_str(), srcPort, destPort, receiveCallback);
    if (ret != PIL_NO_ERROR)
    {
        // Return
    }
}
}
#endif // CXX
