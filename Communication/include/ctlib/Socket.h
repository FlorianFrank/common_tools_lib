/**
 * @file Platform independent implementation of sockets written in C.
 * @addtogroup Communication
 * @addtogroup Sockets
 */

#ifndef PIL_SOCKET_H
#define PIL_SOCKET_H

#include "ctlib/DataTypeDefines.h"
#include "ctlib/ErrorCodeDefines.h"
#include "ctlib/SocketDefines.h"


PIL_ERROR_CODE PIL_SOCKET_Create(PIL_SOCKET *socketRet, TransportProtocol protocol,
                                 InternetProtocol ipVersion, const char* ipAddress, uint16_t port);

PIL_ERROR_CODE PIL_SOCKET_Close(PIL_SOCKET *socketRet);

PIL_ERROR_CODE PIL_SOCKET_Bind(PIL_SOCKET *socketRet, PIL_BOOL reuseSock);

PIL_ERROR_CODE PIL_SOCKET_Listen(PIL_SOCKET *socketRet, uint32_t sizeQueue);

PIL_ERROR_CODE PIL_SOCKET_Accept(PIL_SOCKET *socket, char *ipAddr, PIL_SOCKET *newHandle);

PIL_ERROR_CODE PIL_SOCKET_Connect(PIL_SOCKET *socketRet, const char *ipAddr, uint16_t port, uint16_t timeoutInMs);

PIL_ERROR_CODE PIL_SOCKET_WaitTillDataAvail(PIL_SOCKET *socketRet, uint32_t timeoutMS);

PIL_ERROR_CODE PIL_SOCKET_Receive(PIL_SOCKET *socketRet , uint8_t *buffer, uint32_t *bufferLen, uint16_t timeoutInMS);

PIL_ERROR_CODE PIL_SOCKET_ReceiveFrom(PIL_SOCKET *socketRet , uint8_t *buffer, uint16_t *bufferLen, char *ipAddr, int *port);

PIL_ERROR_CODE PIL_SOCKET_Send(PIL_SOCKET *socketRet, const uint8_t *buffer, uint32_t *bufferLen);

PIL_ERROR_CODE PIL_SOCKET_SendTo(PIL_SOCKET *socketRet, const char* destAddr, uint16_t port, uint8_t *buffer, uint32_t *bufferLen);

const char *PIL_SOCKET_GetSenderIP(PIL_SOCKET *socketRet);

PIL_BOOL PIL_SOCKET_IsOpen(PIL_SOCKET *socketRet);

PIL_ERROR_CODE PIL_SOCKET_RegisterAcceptCallback(PIL_SOCKET *socket, void (*callback)(PIL_SOCKET, char *));

PIL_ERROR_CODE PIL_SOCKET_Setup_ServerSocket(PIL_SOCKET *socket, uint16_t port, void (*acceptCallback)(struct PIL_SOCKET retHandle, char* ip));

PIL_ERROR_CODE PIL_SOCKET_ConnectToServer(PIL_SOCKET *socket, const char *ipAddr, uint16_t srcPort, uint16_t destPort, uint16_t timeoutInMs,
                                          void (*receiveCallback)(PIL_SOCKET* socket, uint8_t *retHandle, uint32_t ip, void*), void* additionalArgument);

struct timeval PIL_SOCKET_TransformMSInTimeVal(uint16_t timeoutInMS);

PIL_ERROR_CODE GetInterfaceInfos(PIL_SOCKET *socket, InterfaceInfoList *interfaceInfos);

/**
 * Event based functions require Threading support.
 */
//#ifdef PIL_THREADING
PIL_ERROR_CODE PIL_SOCKET_RegisterReceiveCallbackFunction(PIL_SOCKET *socketRet,
                                                          void (*callback)(PIL_SOCKET* socket, uint8_t *buffer, uint32_t len,
                                                                    void *), void *additional);

PIL_ERROR_CODE PIL_SOCKET_UnregisterReceiveCallbackFunction(PIL_SOCKET *socketRet);

/**
 * @}
 * @}
*/

#endif //PIL_SOCKET_H
