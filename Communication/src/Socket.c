#include "ctlib/Socket.h"
#if PIL_THREADING
#include "ctlib/Threading.h"
#endif // PIL_THREADING
#include "ctlib/ErrorHandler.h"

#include <stdlib.h> // malloc
#include "assert.h"
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
#include <malloc.h>


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
    {
        return PIL_INVALID_ARGUMENTS;
    }
    socketRet->m_AcceptThreadHandle = NULL;
    socketRet->m_IsConnected = FALSE;
    socketRet->m_IsOpen = FALSE;

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
    socketRet->m_socket = socket(socketRet->m_IPVersion, socketRet->m_protocol, 0);
    if (socketRet->m_socket == -1)
    {
        socketRet->m_IsOpen = FALSE;
        PIL_SetLastError(&socketRet->m_ErrorHandle, PIL_ERRNO);
        return PIL_ERRNO;
    }
#else // lwip
    socketRet->conn = udp_new();
#endif // linux
    socketRet->m_IsOpen = TRUE;
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
        return PIL_INVALID_ARGUMENTS;

    if(socketRet->m_callbackActive == TRUE)
    {
        PIL_SOCKET_UnregisterCallbackFunction(socketRet);
    }

#ifndef embedded
    if (socketRet->m_IsOpen)
    {
        socketRet->m_IsOpen = FALSE;
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

    if(socketRet->m_AcceptThreadHandle)
    {
        PIL_THREADING_JoinThread(socketRet->m_AcceptThreadHandle, NULL);
    }

    PIL_ERROR_CODE ret =  (close(socketRet->m_socket) == 0) ?  PIL_NO_ERROR : PIL_ERRNO;
    free(socketRet);
    return ret;
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
    struct sockaddr_in bindAddr;

    bindAddr.sin_family = socketRet->m_IPVersion;
    if(strcmp(socketRet->m_IPAddress, "127.0.0.1") == 0 || strcmp(socketRet->m_IPAddress, "localhost") == 0)
        bindAddr.sin_addr.s_addr = INADDR_ANY;
    else
        bindAddr.sin_addr.s_addr = inet_addr(socketRet->m_IPAddress);
    bindAddr.sin_port = htons(socketRet->m_port);

    int bindRet = bind(socketRet->m_socket, (struct sockaddr *) &bindAddr,  sizeof(bindAddr));
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
PIL_ERROR_CODE PIL_SOCKET_Accept(PIL_SOCKET *socket, char *ipAddr, PIL_SOCKET *newHandle)
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

    memcpy(newHandle, socket, sizeof(PIL_SOCKET));
    newHandle->m_socket = acceptRet;

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
    inet_aton(ipAddr, &address.sin_addr);

    int connectRet = connect(socket->m_socket, (struct sockaddr *) &address, sizeof(address));
    if (connectRet != 0)
    {
        PIL_SetLastError(&socket->m_ErrorHandle, PIL_ERRNO);
        return PIL_ERRNO;
    }
#endif // On LWIP we only support UDP!
    socket->m_IsConnected = TRUE;
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
    FD_SET(socketRet->m_socket, &readFD);

    timeout.tv_usec = (timeoutMS % 1000) * 1000;
    timeout.tv_sec = (timeoutMS - (timeoutMS % 1000)) / 1000;
    int ret = select(socketRet->m_socket + 1, &readFD, NULL, NULL, &timeout);
    if (ret == -1)
        return PIL_SetLastError(&socketRet->m_ErrorHandle, PIL_ERRNO);
    if(ret == 0)
        return PIL_TIMEOUT;
    return PIL_NO_ERROR;
#else
    return 0;
#endif // !embedded
}

/**
 * @brief Blocking receive function. Receives data on the provided socket.
 * @param socketRet socket on which data should be received.
 * @param buffer buffer in which the incomming data should be stored.
 * @param bufferLen the maximum number of the buffer must be passed to this function.
 *        Afterwards, it contains the amount of data which was received.
 * @return
 */
PIL_ERROR_CODE PIL_SOCKET_Receive(PIL_SOCKET *socketRet, uint8_t *buffer, uint32_t *bufferLen)
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
 * @brief This function should only called internally.
 *        It constantly receives data from a socket and forwards it to the user defined callback function.
 * @param handle argument consists of the corresponding socket, such as a callback function.
 * @return NULL
 */
void* PIL_ReceiveThreadFunction(void *handle)
{
    assert(handle);
    ReceiveThreadCallbackArg *arg = (ReceiveThreadCallbackArg*) handle;
    assert(arg->socket && arg->receiveCallback);

    uint8_t buffer[DEFAULT_SOCK_BUFF_SIZE];
    uint32_t len = DEFAULT_SOCK_BUFF_SIZE;

    while(arg->socket->m_callbackActive)
    {
        PIL_ERROR_CODE  ret = PIL_SOCKET_WaitTillDataAvail(arg->socket, DEFAULT_TIMEOUT_MS);
        if(ret != PIL_NO_ERROR && ret != PIL_TIMEOUT)
        {
            PIL_SetLastError(&arg->socket->m_ErrorHandle, ret);
            return NULL;
        }

        if(ret == PIL_NO_ERROR)
        {
            ret = PIL_SOCKET_Receive(arg->socket, buffer, &len);
            if(ret == PIL_NO_ERROR)
            {
                arg->receiveCallback(buffer, len);
            }
        }
    }
    return NULL;
}

#ifdef PIL_THREADING
/**
 * @brief
 * @param socketRet
 * @param callback
 * @return
 */
PIL_ERROR_CODE PIL_SOCKET_RegisterCallbackFunction(PIL_SOCKET *socketRet, void (*callback)(uint8_t *, uint32_t))
{
    if(!socketRet || !callback)
        return PIL_INVALID_ARGUMENTS;

    // Initialize thread argument
    socketRet->m_callbackThreadArg = malloc(sizeof(ReceiveThreadCallbackArg));
    socketRet->m_callbackThreadArg ->socket = socketRet;
    socketRet->m_callbackThreadArg ->receiveCallback = callback;

    socketRet->m_callbackThreadHandle = malloc(sizeof(ThreadHandle));
    socketRet->m_callbackThreadHandle->m_ThreadArgument.m_ThreadArgument = (void*)&socketRet->m_callbackThreadArg ;
    socketRet->m_callbackThreadHandle->m_ThreadArgument.m_ThreadFunction = PIL_ReceiveThreadFunction;
    PIL_ERROR_CODE createSockRet = PIL_THREADING_CreateThread(socketRet->m_callbackThreadHandle, PIL_ReceiveThreadFunction, (void*)socketRet->m_callbackThreadArg);
    if (createSockRet == PIL_NO_ERROR)
    {
        socketRet->m_callbackActive = TRUE;
        createSockRet = PIL_THREADING_RunThread(socketRet->m_callbackThreadHandle, FALSE);
    }

    if (createSockRet != PIL_NO_ERROR)
    {
        free(socketRet->m_callbackThreadHandle);
        socketRet->m_callbackThreadHandle = NULL;
        free(socketRet->m_callbackThreadArg);
        socketRet->m_callbackThreadArg = NULL;
        return createSockRet;
    }
        return PIL_NO_ERROR;
}


/**
 * @brief This function unregisters the previously created callback function.
 * @param socketRet socket for which the callback was registered.
 * @return PIL_NO_ERROR on success.
 */
PIL_ERROR_CODE PIL_SOCKET_UnregisterCallbackFunction(PIL_SOCKET *socketRet)
{
    if(!socketRet)
        return PIL_INVALID_ARGUMENTS;

    socketRet->m_callbackActive = FALSE;
    if(!socketRet->m_callbackThreadHandle)
    {
        return PIL_INVALID_ARGUMENTS;
    }
        PIL_ERROR_CODE ret = PIL_THREADING_JoinThread(socketRet->m_callbackThreadHandle, NULL);
        free(socketRet->m_callbackThreadHandle);
        socketRet->m_callbackThreadHandle = NULL;
        free(socketRet->m_callbackThreadArg);
        socketRet->m_callbackThreadArg = NULL;
        if(ret != PIL_NO_ERROR)
        {
            return PIL_THREAD_NOT_JOINABLE;
        }
    return PIL_NO_ERROR;
}

#endif // THREADING



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
        return PIL_INVALID_ARGUMENTS;

    if(!socketRet->m_IsOpen)
        return PIL_INTERFACE_CLOSED;

    if(socketRet->m_protocol == TCP && !socketRet->m_IsConnected)
        return PIL_INTERFACE_CLOSED;

#ifndef embedded
    //  socklen_t senderAddrLen = sizeof(socketRet->m_SrcAddr);
    int ret = send(socketRet->m_socket, (char*)buffer, *bufferLen, 0/*, &socketRet->m_SrcAddr, senderAddrLen*/);
    if (ret < 0)
    {
        PIL_SetLastError(&socketRet->m_ErrorHandle, PIL_ERRNO);
        return PIL_ERRNO;
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
    return PIL_NO_ERROR;
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

void* PIL_AcceptThreadFunction(void* value)
{
    assert(value);

    ThreadArg *arg = value;
    char ipAddr[MAX_IP_LEN];
    PIL_SOCKET retHandle;
    do {
        int ret = PIL_SOCKET_Accept(&arg->socket, ipAddr, &retHandle);
        if(ret != PIL_NO_ERROR)
            return NULL;
        retHandle.m_IsConnected = TRUE;
        arg->receiveCallback(retHandle, ipAddr);
    }while(arg->socket.m_IsOpen);
    return NULL;
}

PIL_ERROR_CODE PIL_SOCKET_Setup_ServerSocket(PIL_SOCKET *socket, uint16_t port, void (*receiveCallback)(struct PIL_SOCKET retHandle, char* ip))
{
    if(!receiveCallback)
        return PIL_INVALID_ARGUMENTS;

    PIL_ERROR_CODE ret = PIL_SOCKET_Create(socket, TCP, IPv4, "127.0.0.1", port);
    if(ret != PIL_NO_ERROR)
        return ret;

    ret = PIL_SOCKET_Bind(socket, TRUE);
    if(ret != PIL_NO_ERROR)
        return ret;

    ret = PIL_SOCKET_Listen(socket, DEFAULT_QUEUE_SIZE);
    if(ret != PIL_NO_ERROR)
        return ret;

    ThreadArg *arg = malloc(sizeof(struct ThreadArg));
    arg->socket = *socket;
    arg->receiveCallback = receiveCallback;

    socket->m_AcceptThreadHandle = malloc(sizeof(ThreadHandle));
    PIL_ERROR_CODE  socketRet = PIL_THREADING_CreateThread(socket->m_AcceptThreadHandle, PIL_AcceptThreadFunction, arg);
    if(socketRet == PIL_NO_ERROR)
    {
        socketRet = PIL_THREADING_RunThread(socket->m_AcceptThreadHandle, FALSE);
    }

    if(socketRet != PIL_NO_ERROR)
    {
        free(socket->m_AcceptThreadHandle);
        socket->m_AcceptThreadHandle = NULL;
        return socketRet;
    }

    return PIL_NO_ERROR;
}

PIL_ERROR_CODE PIL_SOCKET_ConnectToServer(PIL_SOCKET *socket, const char *ipAddr, uint16_t srcPort, uint16_t destPort,
                                          void (*receiveCallback)(uint8_t*, uint32_t))
{
    PIL_ERROR_CODE ret = PIL_SOCKET_Create(socket, TCP, IPv4, "127.0.0.1", srcPort);
    if(ret != PIL_NO_ERROR)
        return ret;

    ret = PIL_SOCKET_Connect(socket, ipAddr, destPort);
    if(ret != PIL_NO_ERROR)
        return ret;
    
    return PIL_SOCKET_RegisterCallbackFunction(socket, receiveCallback);
}


