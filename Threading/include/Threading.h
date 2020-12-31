//
// Created by florianfrank on 31.12.20.
//

#ifndef PLATFORMINDEPENDENTLIB_THREADING_H
#define PLATFORMINDEPENDENTLIB_THREADING_H

#include "ErrorCodeDefines.h"
#include "ThreadingDefines.h"


PIL_ERROR_CODE PIL_THREADING_CreateThread(ThreadHandle *threadHandle, void* (*functionPtr)(void*));
PIL_ERROR_CODE PIL_THREADING_JoinThread(ThreadHandle *threadHandle);
PIL_ERROR_CODE PIL_THREADING_AbortThread(ThreadHandle *threadHandle);

#endif //PLATFORMINDEPENDENTLIB_THREADING_H
