//
// Created by florianfrank on 31.12.20.
//
#include <ErrorHandler.h>

#include <asm/errno.h>
#include "../include/Threading.h"

int lastThreadCtr = 0;

PIL_ERROR_CODE PIL_THREADING_CreateThread(ThreadHandle *threadHandle, void* (*functionPtr)(void*), void* argument)
{
    if(!threadHandle)
        return PIL_INVALID_ARGUMENTS;

    threadHandle->m_ThreadFunction = functionPtr;
    threadHandle->m_Argument = argument;
    threadHandle->m_ThreadID = lastThreadCtr;
    lastThreadCtr++;

    return PIL_NO_ERROR;
}

PIL_ERROR_CODE PIL_THREADING_RunThread(ThreadHandle *threadHandle)
{
    if (!threadHandle)
        return PIL_INVALID_ARGUMENTS;

    int ret = pthread_create(&threadHandle->m_Handle, NULL, threadHandle->m_ThreadFunction, threadHandle->m_Argument);
    if (ret == 0)
    {
        threadHandle->m_Running = TRUE;
        return PIL_NO_ERROR;
    }
    switch (ret)
    {
        case EAGAIN:
            PIL_SetLastErrorMsg(&threadHandle->m_ErrorHandle, PIL_INSUFFICIENT_RESOURCES,
                                "Insufficient resources to create another thread.");
            return PIL_INSUFFICIENT_RESOURCES;
        case EINVAL:
            PIL_SetLastErrorMsg(&threadHandle->m_ErrorHandle, PIL_INVALID_ARGUMENTS, "Invalid settings in attr");
            return PIL_INVALID_ARGUMENTS;
        case EPERM:
            PIL_SetLastErrorMsg(&threadHandle->m_ErrorHandle, PIL_INSUFFICIENT_PERMISSIONS,
                                "o permission to set the scheduling policy and parameters specified in attr.");
            return PIL_INSUFFICIENT_PERMISSIONS;
        default:
            PIL_SetLastError(&threadHandle->m_ErrorHandle, PIL_UNKNOWN_ERROR);
            return PIL_UNKNOWN_ERROR;
    }
}


PIL_ERROR_CODE PIL_THREADING_JoinThread(ThreadHandle *threadHandle, void **retValue)
{
    int ret = pthread_join(threadHandle->m_Handle, retValue);
    threadHandle->m_Running = FALSE;
    if(ret == 0)
        return PIL_NO_ERROR;

    switch (ret)
    {
        case EDEADLK:
            PIL_SetLastError(&threadHandle->m_ErrorHandle, PIL_DEADLOCK_DETECTED);
            return PIL_DEADLOCK_DETECTED;
        case EINVAL:
            PIL_SetLastError(&threadHandle->m_ErrorHandle, PIL_THREAD_NOT_JOINABLE);
            return PIL_THREAD_NOT_JOINABLE;
        case ESRCH:
            PIL_SetLastError(&threadHandle->m_ErrorHandle, PIL_THREAD_NOT_FOUND);
            return PIL_THREAD_NOT_FOUND;
        default:
            PIL_SetLastError(&threadHandle->m_ErrorHandle, PIL_UNKNOWN_ERROR);
            return PIL_UNKNOWN_ERROR;
    }
}

PIL_ERROR_CODE PIL_THREADING_AbortThread(ThreadHandle *threadHandle)
{

}