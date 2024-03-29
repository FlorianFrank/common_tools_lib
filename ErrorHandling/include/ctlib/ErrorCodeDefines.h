//
// Created by florianfrank on 29.12.20.
//

#ifndef PIL_ERROR_CODE_DEFINES_H
#define PIL_ERROR_CODE_DEFINES_H

#include <stdint.h> // uint32_t

/** Maximum error message length. */
#define MAX_ERROR_MSG_LEN 128

/**
 * @brief Enum of different error codes.
 */
enum
{
    /** No error occurred. */
    PIL_NO_ERROR,
    /**  **/
    PIL_INVALID_ARGUMENTS,
    /** An error occured and an errno error code was set. **/
    PIL_ERRNO,
    /** A timeout during an operation occurred. */
    PIL_TIMEOUT,
    /** Error when performing an operation on a closed interface. */
    PIL_INTERFACE_CLOSED,
    /** Error when baudrate cannot be set. E.g. when the approximation function has a higher deviation then 2 %. */
    PIL_INVALID_BAUDRATE,
    /* Error insufficient resources. */
    PIL_INSUFFICIENT_RESOURCES,
    PIL_NO_DATA_RECEIVED,
    /** Error, insufficient permissions to perform a certain operation. */
    PIL_INSUFFICIENT_PERMISSIONS,
    /** Error, when deadlock is detected in a multithreaded application. */
    PIL_DEADLOCK_DETECTED,
    /** Error while joining a thread. */
    PIL_THREAD_NOT_JOINABLE,
    /** Error performing an operation on on a not initialized thread. */
    PIL_THREAD_NOT_FOUND,
    /** Error entire file could not be written. */
    PIL_ONLY_PARTIALLY_READ_WRITTEN,
    /** Error no such file or directory. */
    PIL_NO_SUCH_FILE,

     /** Invalid error code. */
    PIL_UNKNOWN_ERROR,

    PIL_XML_PARSING_ERROR,

    PIL_ITEM_IN_ERROR_QUEUE,

    PIL_DOUBLE_INITIALIZE

} typedef PIL_ERROR_CODE;

/**
 * @brief Struct storing the error code, errno, if errno is set as well as the error message as string.
 */
struct
{
    /** Error code of the returning function. */
    PIL_ERROR_CODE m_ErrorCode;
    /** Sets errno code when m_ErrorCode is PIL_ERRNO. */
    uint32_t m_ErrnoCode;
    /** String error message. */
    char m_ErrorMessage[MAX_ERROR_MSG_LEN];
} typedef PIL_ErrorHandle;

#endif //PIL_ERROR_CODE_DEFINES_H
