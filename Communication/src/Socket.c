#include "Socket.h"

// TODO Linux windows
#include <sys/socket.h> // helperFiles, recvfrom, sendto
#include <arpa/inet.h> // htons, inet_addr, inet_ntoa
#include <string.h> // memset
#include <unistd.h> // close

#if embedded
#include <lwip/tcpip.h>
#include <lwip/udp.h>
#include <lwip/igmp.h>
#endif // embedded

#ifdef __linux__

#include <sys/select.h> // fd_set, timeval, select
#include <ErrorHandler.h>

#endif // __linux__

/**
 *
 * @brief Creates a helperFiles, dependent on the keyServer config stored in the object.
 *        It can create UDP and TCP sockets using IPv4 or IPv6.
 * @param socketRet return value of the helperFiles.
 * @param protocol protocol used either TCP or UDP is supported.
 * @param ipVersion use IPv4 or IPv6.
 * @param ipAddress IP address to bind.
 * @param port port to bind.
 * @return 0 if no error occurred.
 */
PIL_ERROR_CODE
PIL_SOCKET_Create(PIL_SOCKET *socketRet, TransportProtocol protocol, InternetProtocol ipVersion, const char *ipAddress,
                  const uint16_t port)
{
    if (!socketRet)
        return PIL_INVALID_ARGUMENTS;

    if (!ipAddress)
    {
        PIL_SetLastErrorMsg(&socketRet->m_ErrorHandle, PIL_INVALID_ARGUMENTS, "Invalid IP");
        return PIL_INVALID_ARGUMENTS;
    }

    socketRet->m_protocol = (protocol == UDP) ? SOCK_DGRAM : SOCK_STREAM;
    socketRet->m_IPVersion = (ipVersion == IPv4) ? AF_INET : AF_INET6;

    // TODO ip validity check
    strcpy((char *) socketRet->m_IPAddress, ipAddress);
    socketRet->m_port = port;

#ifndef embedded
    socketRet->m_socket = socket(socketRet->m_protocol, socketRet->m_IPVersion, 0);
    if (socketRet->m_socket == -1)
    {
        socketRet->m_IsOpen = 0;
        PIL_SetLastError(&socketRet->m_ErrorHandle, PIL_ERRNO);
        return PIL_ERRNO;
    }
#else // lwip
    socketRet->conn = udp_new();
#endif // linux
    socketRet->m_IsOpen = 1;
    return PIL_NO_ERROR;
}

/**
 * @brief Closes a helperFiles if helperFiles is open.
 * @param socketRet handle corresponding to the helperFiles which should be closed.
 * @return 0 when no error occurred else return -1.
 */
PIL_ERROR_CODE PIL_SOCKET_Close(PIL_SOCKET *socketRet)
{
    if (!socketRet)
        return -1;

#ifndef embedded
    if (socketRet->m_IsOpen)
    {
        socketRet->m_IsOpen = 0;
        if (close(socketRet->m_socket) != 0)
        {
            PIL_SetLastError(&socketRet->m_ErrorHandle, PIL_ERRNO);
            return PIL_ERRNO;
        }
        return PIL_NO_ERROR;
    }
#else // lwip
    udp_remove(socketRet->conn);
#endif // linux
    return PIL_SOCKET_CLOSED;
}

/**
 * @brief Bind helperFiles on address given in keyserver config.
 * @param socketRet handle to bind the helperFiles.
 * @param reuseSock flag if SO_REUSEADDR should be used.
 * @return 0 if no error occurred else -1.
 */
PIL_ERROR_CODE PIL_SOCKET_Bind(PIL_SOCKET *socketRet, PIL_BOOL reuseSock)
{
    if (!socketRet)
        return PIL_INVALID_ARGUMENTS;

#ifndef embedded
    if(reuseSock)
    {
        // Allow reuse of local addresses
        uint32_t reuse = 1;
        int sockOptRet = setsockopt(socketRet->m_socket, SOL_SOCKET, SO_REUSEADDR, (char *) &reuse, sizeof(reuse));
        if (sockOptRet != 0)
        {
            PIL_SetLastError(&socketRet->m_ErrorHandle, PIL_ERRNO);
            return PIL_ERRNO;
        }
    }

    // Set transport protocol (UDP/TCP), IP-version (IPv4, IPv6) and the address of the helperFiles to bound on
    struct sockaddr_in address = {0};
    memset(&address, 0, sizeof(address));

    address.sin_family = socketRet->m_IPVersion;
    address.sin_addr.s_addr = inet_addr(socketRet->m_IPAddress);
    address.sin_port = htons(socketRet->m_port);

    int bindRet = bind(socketRet->m_socket, (struct sockaddr *) &address, sizeof(struct sockaddr));
    if (bindRet != 0)
    {
        PIL_SetLastError(&socketRet->m_ErrorHandle, PIL_ERRNO);
        return PIL_ERRNO;
    }
#else // lwip
    err_t ret = udp_bind(socketRet->conn, &socketRet->m_SrcAddr, socketRet->m_port);
    if(ret != 0)
    {
    PIL_SetLastError(&socketRet->m_ErrorHandle, PIL_ERRNO); // TODO
    return -1;
        }
#endif // Linux
    return PIL_NO_ERROR;
}

/**
 * @brief Perform listen operation on a helperFiles using a TCP connection.
 * @param socketRet helperFiles to listen.
 * @param sizeQueue size of the queue for listening operations.
 * @return 0 if no error occurs else -1.
 */
PIL_ERROR_CODE PIL_SOCKET_Listen(PIL_SOCKET *socketRet, uint32_t sizeQueue)
{
#ifndef embedded
    if (socketRet == NULL)
        return PIL_INVALID_ARGUMENTS;

    uint32_t listenRet = listen(socketRet->m_socket, sizeQueue);
    if (listenRet != 0)
    {
        PIL_SetLastError(&socketRet->m_ErrorHandle, PIL_ERRNO);
        return PIL_ERRNO;
    }
#endif // we use LWIP only for UDP
    return PIL_NO_ERROR;
}

/**
 * @brief This function accepts a message when using TCP connections.
 * @param socket helperFiles to accept.
 * @param ipAddr ipAddress of the new accepted connection.
 * @return 0 if no error occurs else -1.
 */
PIL_ERROR_CODE PIL_SOCKET_Accept(PIL_SOCKET *socket, char *ipAddr)
{
#ifndef embedded
    if (socket == NULL)
        return PIL_INVALID_ARGUMENTS;


    if (!ipAddr)
    {
        PIL_SetLastErrorMsg(&socket->m_ErrorHandle, PIL_INVALID_ARGUMENTS, "ipAddr pointer is NULL");
        return PIL_INVALID_ARGUMENTS;
    }

    struct sockaddr_in address;
    socklen_t addrLen = sizeof(address);
    int acceptRet = accept(socket->m_socket, (struct sockaddr *) &address, &addrLen);
    if (acceptRet < 0)
    {
        PIL_SetLastError(&socket->m_ErrorHandle, PIL_ERRNO);
        return PIL_ERRNO;
    }

    strcpy(ipAddr, inet_ntoa(address.sin_addr));
#else // our LWIP implementation only supports UDP!
    return 0;
#endif // Linux
    return PIL_NO_ERROR;
}

/**
 * @brief Connect function if a TCP helperFiles is used.
 * @param socket helperFiles on which the connect function is called.
 * @param ipAddr ip address of the connection.
 * @param port port of the new connection.
 * @return 0 if no error occured.
 */
PIL_ERROR_CODE PIL_SOCKET_Connect(PIL_SOCKET *socket, const char *ipAddr, uint16_t port)
{
    if (socket == NULL)
        return PIL_INVALID_ARGUMENTS;

    if(!ipAddr)
    {
        PIL_SetLastErrorMsg(&socket->m_ErrorHandle, PIL_INVALID_ARGUMENTS, "ipAddr pointer is NULL");
        return PIL_INVALID_ARGUMENTS;
    }

#ifndef embedded
    struct sockaddr_in address;
    address.sin_family = socket->m_IPVersion;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = inet_addr(ipAddr);

    int connectRet = connect(socket->m_socket, (struct sockaddr *) &address, sizeof(address));
    if (connectRet != 0)
    {
        PIL_SetLastError(&socket->m_ErrorHandle, PIL_ERRNO);
        return PIL_ERRNO;
    }
#endif // On LWIP we only support UDP!
    return PIL_NO_ERROR;
}

/**
 * @brief Wait until timeout if data is available at the helperFiles.
 * @param timeoutMS timeout to wait until data is available.
 * @return -1 if error occurs, 0 if no data is available and timeout occurs else data to read is available.
 */
PIL_ERROR_CODE PIL_SOCKET_WaitTillDataAvail(PIL_SOCKET *socketRet, uint32_t timeoutMS)
{
#ifndef embedded
    if (!socketRet)
        return -1;

    fd_set readFD = {0};
    struct timeval timeout = {0};

    // set select write handle to zero
    FD_ZERO(&readFD);
    FD_SET(0u, &readFD); // NOLINT(hicpp-signed-bitwise)

    timeout.tv_usec = (timeoutMS % 1000) * 1000;
    timeout.tv_sec = (timeoutMS - (timeoutMS % 1000)) / 1000;
    int ret = select(socketRet->m_socket + 1, &readFD, NULL, NULL, &timeout);
    if (ret == -1)
        PIL_SetLastError(&socketRet->m_ErrorHandle, PIL_ERRNO);
    return ret;
#else
    return 0;
#endif // !embedded
}

PIL_ERROR_CODE PIL_SOCKET_Receive(PIL_SOCKET *socketRet, uint8_t *buffer, uint16_t *bufferLen)
{
    if (!socketRet)
        return PIL_INVALID_ARGUMENTS;

    uint32_t ret = recv(socketRet->m_socket, buffer, *bufferLen, 0);
    if (ret == -1)
    {
        PIL_SetLastError(&socketRet->m_ErrorHandle, PIL_ERRNO);
        return PIL_ERRNO;
    }

    *bufferLen = ret;
    return PIL_NO_ERROR;
}


/**
 * @brief Receive Data in blocking mode. Stores IPAddress and port of sending participant.
 * (WaitTillDataAvail must be called previously to avoid deadlocks)
 * @param buffer m_ReceiveBuffer to store the received data.
 * @param bufferLen size of the m_ReceiveBuffer, after method call, this variable contains the size of the sent data.
 * @return 0 if no error is occurred else -1 is returned.
 */
PIL_ERROR_CODE PIL_SOCKET_ReceiveFrom(PIL_SOCKET *socketRet, uint8_t *buffer, uint16_t *bufferLen, char *ipAddr, int *port)
{
    if (!socketRet)
        return PIL_INVALID_ARGUMENTS;

#ifndef embedded
    struct sockaddr_in addr;
    memset(&addr, 0x00, sizeof(struct sockaddr_in));
    addr.sin_family = socketRet->m_protocol;
    addr.sin_port = htons(socketRet->m_port);
    addr.sin_addr.s_addr = INADDR_ANY;
    socklen_t senderAddrLen = sizeof(addr);


    int ret = recvfrom(socketRet->m_socket, buffer, *bufferLen, MSG_WAITALL, (struct sockaddr *) &addr, &senderAddrLen);
    if (ret < 0)
    {
        PIL_SetLastError(&socketRet->m_ErrorHandle, PIL_ERRNO);
        return PIL_ERRNO;
    }
    if (ipAddr != NULL)
        strcpy(ipAddr, inet_ntoa(addr.sin_addr));
    if (port != NULL)
        *port = ntohs(addr.sin_port);

    *bufferLen = ret;
#else // LWIP
    struct udp_pcb buff;
    err_t ret = udp_recv(buff, 10); // TODO
    if(ret != 0)
    {
        PIL_SetLastError(socketRet);
        return -1;
    }
#endif // LINUX
    return 0;
}


/**
 * @brief Method sends data to participant stored in m_SrcAddr (set by receive function).
 * @param buffer m_ReceiveBuffer to send.
 * @param bufferLen length of the m_ReceiveBuffer, after this method, the sent data are stored in this variable.
 * @return 0 if no error is occurred else -1 is returned.
 */
PIL_ERROR_CODE PIL_SOCKET_Send(PIL_SOCKET *socketRet, uint8_t *buffer, uint32_t *bufferLen)
{
    if (!socketRet)
        return -1;

#ifndef embedded
    //  socklen_t senderAddrLen = sizeof(socketRet->m_SrcAddr);
    int ret = send(socketRet->m_socket, buffer, *bufferLen, 0/*, &socketRet->m_SrcAddr, senderAddrLen*/);
    if (ret < 0)
    {
        PIL_SetLastError(&socketRet->m_ErrorHandle, PIL_ERRNO);
        return -1;
    }
    *bufferLen = ret;
#else // LWIP
    struct pbuf *newBuff;
    newBuff = pbuf_alloc(PBUF_TRANSPORT, *bufferLen, PBUF_RAM);
    err_t ret = udp_send(socketRet->conn, newBuff);
    if(ret != 0)
    {
        PIL_SetLastError(socketRet);
        return -1;
        }
#endif // Linux
    return 0;
}

/**
 * @brief Method sends data to participant stored in m_SrcAddr (set by receive function).
 * @param buffer m_ReceiveBuffer to send.
 * @param bufferLen length of the m_ReceiveBuffer, after this method, the sent data are stored in this variable.
 * @return 0 if no error is occurred else -1 is returned.
 */
PIL_ERROR_CODE
PIL_SOCKET_SendTo(PIL_SOCKET *socketRet, const char *destAddr, const uint16_t port, uint8_t *buffer, uint32_t *bufferLen)
{
#ifndef embedded
    if (destAddr == NULL || buffer == NULL)
        return -1;

    ((struct sockaddr_in *) &socketRet->m_SrcAddr)->sin_family = socketRet->m_IPVersion;
    ((struct sockaddr_in *) &socketRet->m_SrcAddr)->sin_addr.s_addr = inet_addr(destAddr);
    ((struct sockaddr_in *) &socketRet->m_SrcAddr)->sin_port = htons(port);

    socklen_t senderAddrLen = sizeof(socketRet->m_SrcAddr);
    int ret = sendto(socketRet->m_socket, buffer, *bufferLen, 0, &socketRet->m_SrcAddr, senderAddrLen);
    if (ret < 0)
    {
        PIL_SetLastError(&socketRet->m_ErrorHandle, PIL_ERRNO);
        return PIL_ERRNO;
    }
    *bufferLen = ret;
#else // LWIP
    struct pbuf *newbuff;
    newbuff = pbuf_alloc(PBUF_TRANSPORT, *bufferLen, PBUF_RAM);
    if(ipaddr_aton(&socketRet->m_SrcAddr, &socketRet->m_SrcAddr) != 0)
        return -1;
    err_t ret = udp_sendto(socketRet->conn, newbuff, &socketRet->m_SrcAddr, socketRet->m_port);
    if(ret != 0)
            return -1;
#endif // Linux
    return 0;
}

/**
 * @brief Method can be called after receive operation and
 * returns the IP-address of the sender of the last received package.
 * @return IP-address of the sender.
 */
const char *PIL_SOCKET_GetSenderIP(PIL_SOCKET *socketRet)
{
#ifndef embedded
    if (!socketRet)
        return NULL;

    return inet_ntoa(((struct sockaddr_in *) &socketRet->m_SrcAddr)->sin_addr);
#endif // Not supported by LWIP
}

/**
 * @brief This function returns if the socket is open or not.
 * @param socketRet socket handle to check if the socket is open.
 * @return true if open else false.
 */
PIL_BOOL PIL_SOCKET_IsOpen(PIL_SOCKET *socketRet)
{
    if(!socketRet)
        return FALSE;
    return socketRet->m_IsOpen;
}
