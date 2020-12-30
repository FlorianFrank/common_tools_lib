/**
 * @file File contains a wrapper class Socked, which wraps the platform dependent implementation of sockets.
 * @addtogroup ASOACryptMiddleware
 */

#ifndef PIL_SOCKET_H
#define PIL_SOCKET_H

#include "DataTypeDefines.h"
#include "ErrorCodeDefines.h"

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

enum TransportProtocol
{
    UDP = 0,
    TCP = 1
} typedef TransportProtocol;

enum InternetProtocol
{
    IPv4 = 0,
    IPv6 = 1
} typedef InternetProtocol;

PIL_ERROR_CODE PIL_SOCKET_Create(PIL_SOCKET *socketRet, TransportProtocol protocol,
                                 InternetProtocol ipVersion, const char* ipAddress, u_int16_t port);

PIL_ERROR_CODE PIL_SOCKET_Close(PIL_SOCKET *socketRet);

PIL_ERROR_CODE PIL_SOCKET_Bind(PIL_SOCKET *socketRet, PIL_BOOL reuseSock);

PIL_ERROR_CODE PIL_SOCKET_Listen(PIL_SOCKET *socketRet, uint32_t sizeQueue);

PIL_ERROR_CODE PIL_SOCKET_Accept(PIL_SOCKET *socketRet, char *ipAddr);

PIL_ERROR_CODE PIL_SOCKET_Connect(PIL_SOCKET *socketRet, const char *ipAddr, uint16_t port);

PIL_ERROR_CODE PIL_SOCKET_WaitTillDataAvail(PIL_SOCKET *socketRet, uint32_t timeoutMS);

PIL_ERROR_CODE PIL_SOCKET_Receive(PIL_SOCKET *socketRet , uint8_t *buffer, uint16_t *bufferLen);

PIL_ERROR_CODE PIL_SOCKET_ReceiveFrom(PIL_SOCKET *socketRet , uint8_t *buffer, uint16_t *bufferLen, char *ipAddr, int *port);

PIL_ERROR_CODE PIL_SOCKET_Send(PIL_SOCKET *socketRet, uint8_t *buffer, uint32_t *bufferLen);

PIL_ERROR_CODE PIL_SOCKET_SendTo(PIL_SOCKET *socketRet, const char* destAddr, uint16_t port, uint8_t *buffer, uint32_t *bufferLen);

const char *PIL_SOCKET_GetSenderIP(PIL_SOCKET *socketRet);

PIL_BOOL PIL_SOCKET_IsOpen(PIL_SOCKET *socketRet);


/**
* @}
* @}
*/

#endif //PIL_SOCKET_H
