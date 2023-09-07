//
// Created by florianfrank on 31.12.20.
//

#ifndef PLATFORMINDEPENDENTLIB_THREADDEFINES_H
#define PLATFORMINDEPENDENTLIB_THREADDEFINES_H

#if !defined(_MSC_VER)
#include <pthread.h>
#else 
#include <windows.h>
#endif

#include "DataTypeDefines.h"
#include "ctlib/ErrorCodeDefines.h"

struct
{
    void* (*m_ThreadFunction)(void *);
    void* m_ThreadArgument;
    volatile PIL_BOOL m_Running;
    PIL_BOOL m_Loop;
} typedef PIL_ThreadArgument;


struct
{
#if defined(_MSC_VER)
    HANDLE m_Handle;
    DWORD m_ThreadID;
#else
    pthread_t m_Handle;
    uint32_t m_ThreadID;
#endif
    PIL_ThreadArgument m_ThreadArgument;
    PIL_BOOL m_Running;
    PIL_ErrorHandle m_ErrorHandle;
} typedef ThreadHandle;

#endif //PLATFORMINDEPENDENTLIB_THREADDEFINES_H
