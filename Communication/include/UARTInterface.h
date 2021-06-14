/**
 * @file File contains a wrapper class for UART communication on different systems.
 * @addtogroup ASOACryptMiddleware
 */

#ifndef PIL_UART_INTERFACE
#define PIL_UART_INTERFACE

#include "UARTDefines.h"


PIL_ERROR_CODE PIL_UART_CreateUartInterface(PIL_UART_Config *config, const char *interface, uint32_t baudrate);
PIL_ERROR_CODE PIL_UART_Open(PIL_UART_Config *config, PIL_BOOL nonBlocking);
PIL_ERROR_CODE PIL_UART_Close(PIL_UART_Config *config);
PIL_ERROR_CODE PIL_UART_ReadData(PIL_UART_Config *config, char* buffer, int *bufferLen);
PIL_ERROR_CODE PIL_UART_WriteData(PIL_UART_Config *config, const char *buffer, const int *write);
PIL_ERROR_CODE PIL_UART_SetComParameters(PIL_UART_Config *config);

#endif //PIL_UART_INTERFACE
