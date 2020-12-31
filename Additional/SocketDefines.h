//
// Created by florianfrank on 31.12.20.
//

#ifndef PLATFORMINDEPENDENTLIB_SOCKETDEFINES_H
#define PLATFORMINDEPENDENTLIB_SOCKETDEFINES_H

#include <stdint.h>
#include <zconf.h>

#ifndef embedded
#include <sys/socket.h>
#else // LWIP
#include <lwip/tcpip.h>
#include <lwip/udp.h>
#endif // !embedded

#include <stdint.h>

struct PIL_SOCKET
{
#ifndef embedded // Linux
    /** Socket handle. */
    int32_t m_socket;
#else // lwip
    struct udp_pcb *conn;
#endif // Linux

    u_int16_t m_port;

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
    int m_IsOpen;
    PIL_ErrorHandle m_ErrorHandle;
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




#endif //PLATFORMINDEPENDENTLIB_SOCKETDEFINES_H
