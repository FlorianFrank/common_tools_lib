//
// Created by florianfrank on 31.12.20.
//

#ifndef PLATFORMINDEPENDENTLIB_SOCKETDEFINES_H
#define PLATFORMINDEPENDENTLIB_SOCKETDEFINES_H

#include <stdint.h>
#ifdef __WIN32__

#else
#include <zconf.h>
#endif // __WIN32__

#ifndef embedded

#ifdef __linux__
#include <sys/socket.h>
#endif // __linux

#ifdef __WIN32__
#include <winsock.h>
#endif // __WIN32__

#else // LWIP
#include <lwip/tcpip.h>
#include <lwip/udp.h>
#endif // !embedded

#include <stdint.h>
#include "ThreadingDefines.h"


struct ReceiveThreadCallbackArg typedef ReceiveThreadCallbackArg;

struct PIL_SOCKET
{
#ifndef embedded // Linux
    /** Socket handle. */
    int32_t m_socket;
#else // lwip
    struct udp_pcb *conn;
#endif // Linux

    uint16_t m_port;

    char m_IPAddress[128];

    int m_protocol;

    int m_IPVersion;

#ifndef embedded // Linux
    /** SocketAddr is used to identify the
     * sender of an received datagram or message. */
    struct sockaddr m_SrcAddr;
#else // LWIP
    ip_addr_t m_SrcAddr;
#endif // !embedded
    ThreadHandle *m_AcceptThreadHandle;
    volatile PIL_BOOL m_IsOpen;
    PIL_BOOL m_IsConnected;
    PIL_ErrorHandle m_ErrorHandle;

    ThreadHandle *m_callbackThreadHandle;
    ReceiveThreadCallbackArg *m_callbackThreadArg;
    PIL_BOOL m_callbackActive;

} typedef PIL_SOCKET;

enum
{
    UDP = 0,
    TCP = 1
} typedef TransportProtocol;

enum
{
    IPv4 = 0,
    IPv6 = 1
} typedef InternetProtocol;

struct ThreadArg{
    struct PIL_SOCKET socket;
    void (*receiveCallback)(struct PIL_SOCKET retHandle, char* ip);
} typedef ThreadArg;

struct ReceiveThreadCallbackArg {
    PIL_SOCKET *socket;
    void (*receiveCallback)(uint8_t* buffer, uint32_t len);
} typedef ReceiveThreadCallbackArg;

#define DEFAULT_QUEUE_SIZE 10
#define DEFAULT_TIMEOUT_MS 500
#define DEFAULT_SOCK_BUFF_SIZE 4096
#define MAX_IP_LEN 39 // Max size of an IP-address in textual representation (e.g. 0000:0000:0000:0000:0000:0000:0000:0000)



#endif //PLATFORMINDEPENDENTLIB_SOCKETDEFINES_H
