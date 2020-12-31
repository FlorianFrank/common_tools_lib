//
// Created by florianfrank on 31.12.20.
//

#ifndef PLATFORMINDEPENDENTLIB_THREADDEFINES_H
#define PLATFORMINDEPENDENTLIB_THREADDEFINES_H

#include <pthread.h>
#include "DataTypeDefines.h"

struct
{
    pthread_t m_Handle;
    uint32_t m_ThreadID;
    void* (*m_ThreadFunction)(void *);
    void *m_Argument;
    PIL_BOOL m_Running;
    PIL_ErrorHandle m_ErrorHandle;
} typedef ThreadHandle;

#endif //PLATFORMINDEPENDENTLIB_THREADDEFINES_H
