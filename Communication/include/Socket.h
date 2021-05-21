/**
 * @file File contains a wrapper class Socked, which wraps the platform dependent implementation of sockets.
 * @addtogroup ASOACryptMiddleware
 */

#ifndef PIL_SOCKET_H
#define PIL_SOCKET_H

#include "DataTypeDefines.h"
#include "ErrorCodeDefines.h"
#include "SocketDefines.h"


PIL_ERROR_CODE PIL_SOCKET_Create(PIL_SOCKET *socketRet, TransportProtocol protocol,
                                 InternetProtocol ipVersion, const char* ipAddress, u_int16_t port);

PIL_ERROR_CODE PIL_SOCKET_Close(PIL_SOCKET *socketRet);

PIL_ERROR_CODE PIL_SOCKET_Bind(PIL_SOCKET *socketRet, PIL_BOOL reuseSock);

PIL_ERROR_CODE PIL_SOCKET_Listen(PIL_SOCKET *socketRet, uint32_t sizeQueue);

PIL_ERROR_CODE PIL_SOCKET_Accept(PIL_SOCKET *socket, char *ipAddr, PIL_SOCKET *newHandle);

PIL_ERROR_CODE PIL_SOCKET_Connect(PIL_SOCKET *socketRet, const char *ipAddr, uint16_t port);

PIL_ERROR_CODE PIL_SOCKET_WaitTillDataAvail(PIL_SOCKET *socketRet, uint32_t timeoutMS);

PIL_ERROR_CODE PIL_SOCKET_Receive(PIL_SOCKET *socketRet , uint8_t *buffer, uint32_t *bufferLen);

PIL_ERROR_CODE PIL_SOCKET_ReceiveFrom(PIL_SOCKET *socketRet , uint8_t *buffer, uint16_t *bufferLen, char *ipAddr, int *port);

PIL_ERROR_CODE PIL_SOCKET_Send(PIL_SOCKET *socketRet, uint8_t *buffer, uint32_t *bufferLen);

PIL_ERROR_CODE PIL_SOCKET_SendTo(PIL_SOCKET *socketRet, const char* destAddr, uint16_t port, uint8_t *buffer, uint32_t *bufferLen);

const char *PIL_SOCKET_GetSenderIP(PIL_SOCKET *socketRet);

PIL_BOOL PIL_SOCKET_IsOpen(PIL_SOCKET *socketRet);

PIL_ERROR_CODE PIL_SOCKET_Setup_ServerSocket(PIL_SOCKET *socket, uint16_t port, void (*receive)(struct PIL_SOCKET retHandle, char* ip));

PIL_ERROR_CODE PIL_SOCKET_ConnectToServer(PIL_SOCKET *socket, const char *ipAddr, uint16_t srcPort, uint16_t destPort,
                                          void (*receiveCallback)(struct PIL_SOCKET *retHandle, char *ip));

/**
 * Event based functions require Threading support.
 */
#ifdef PIL_THREADING
PIL_ERROR_CODE PIL_SOCKET_RegisterCallbackFunction(PIL_SOCKET *socketRet, void (*callback)(uint8_t *buffer,
                                                                                           uint32_t len));
PIL_ERROR_CODE PIL_SOCKET_UnregisterCallbackFunction(PIL_SOCKET *socketRet);
#endif // PIL_THREADING




/**
* @}
* @}
*/

#endif //PIL_SOCKET_H
