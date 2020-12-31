//
// Created by florianfrank on 31.12.20.
//

#ifndef PLATFORMINDEPENDENTLIB_THREADDEFINES_H
#define PLATFORMINDEPENDENTLIB_THREADDEFINES_H

#include <pthread.h>

struct
{
    pthread_t m_Handle;
    uint32_t m_ThreadID;

    void* (*m_ThreadHandle)(void *);
} typedef ThreadHandle;

#endif //PLATFORMINDEPENDENTLIB_THREADDEFINES_H
