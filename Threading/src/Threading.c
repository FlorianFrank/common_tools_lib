//
// Created by florianfrank on 31.12.20.
//
#include <ctlib/ErrorHandler.h>

#include <errno.h>
#include "ctlib/Threading.h"

int lastThreadCtr = 0;


void* ThreadFunction(void *threadArgs)
{
    PIL_ThreadArgument *pilThreadArgument = (void*)threadArgs;
    do{
        pilThreadArgument->m_ThreadFunction(pilThreadArgument->m_ThreadArgument);
    }while(pilThreadArgument->m_Loop && pilThreadArgument->m_Running);
    return NULL;
}

PIL_ERROR_CODE PIL_THREADING_CreateThread(ThreadHandle *threadHandle, void* (*functionPtr)(void*), void* argument)
{
    if(!threadHandle)
        return PIL_INVALID_ARGUMENTS;

    threadHandle->m_ThreadArgument.m_ThreadFunction = functionPtr;
    threadHandle->m_ThreadArgument.m_ThreadArgument = argument;
    threadHandle->m_ThreadArgument.m_Running = TRUE;
    threadHandle->m_ThreadID = lastThreadCtr;
    lastThreadCtr++;

    return PIL_NO_ERROR;
}

PIL_ERROR_CODE PIL_THREADING_RunThread(ThreadHandle *threadHandle, PIL_BOOL loop)
{
    if (!threadHandle)
        return PIL_INVALID_ARGUMENTS;

    threadHandle->m_ThreadArgument.m_Loop = loop;

    int ret = pthread_create(&threadHandle->m_Handle, NULL, ThreadFunction, &threadHandle->m_ThreadArgument);
    if (ret != 0)
    {

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
    threadHandle->m_Running = TRUE;
    return PIL_NO_ERROR;
}

PIL_ERROR_CODE PIL_THREADING_Detach(ThreadHandle *threadHandle)
{
    if(!threadHandle)
        return PIL_INVALID_ARGUMENTS;

    int ret = pthread_detach(threadHandle->m_Handle);
    if(ret != 0)
    {
        switch (ret)
        {
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
    return PIL_NO_ERROR;
}


PIL_ERROR_CODE PIL_THREADING_JoinThread(ThreadHandle *threadHandle, void **retValue)
{
    threadHandle->m_ThreadArgument.m_Running = FALSE;
    int ret = pthread_join(threadHandle->m_Handle, retValue);
    if(ret != 0)
    {
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
    threadHandle->m_Running = FALSE;
    return PIL_NO_ERROR;
}

PIL_ERROR_CODE PIL_THREADING_AbortThread(ThreadHandle *threadHandle)
{
    return PIL_NO_ERROR;
}
