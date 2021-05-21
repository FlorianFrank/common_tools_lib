//
// Created by florianfrank on 04.05.21.
//
#ifdef CXX
#pragma once
#include <string>
#include <vector>

extern "C"
{
    #include "SocketDefines.h"
}

class PIL_Socket
{
public:
    PIL_Socket(TransportProtocol transportProtocol, InternetProtocol internetProtocol, std::string &address, int port);
    ~PIL_Socket();

    void Bind(PIL_BOOL reuse);
    void Listen(int queueSize);
    void Accept(char *ipAddr);
    void WaitTillDataAvailable(int timeOut);
    void Connect(std::string &ipAddr, int port);
    void Receive(uint8_t *buffer, uint32_t *bufferLen);
    void ReceiveFrom(uint8_t *buffer, int *bufferLen, std::string &ipAddr, int port);
    void Send(uint8_t *buffer, int *len);
    void SendTo(std::string &destAddr, int port, uint8_t* buffer, int *bufferLen);
    std::string GetSenderIP();
    PIL_BOOL IsOpen();
    void CreateServerSocket(int port, void (*receiveCallback)(struct PIL_SOCKET *retHandle, char* ip));
    void ConnectWithServer(std::string &ipAddr, int port);

private:
    PIL_SOCKET m_SocketRet{};
    std::vector<PIL_Socket> m_SocketList;
    // TODO ring buffer
};
#endif // CXX