//
// Created by florianfrank on 30.05.21.
//
#ifdef PIL_CXX


#include <iostream>
#include <functional>
#include "ctlib/Threading.hpp"


PIL::Threading::Threading(std::function<void*(void*)> &function, void *argument) : ErrorHandler(),
                                                                                   m_ThreadFunction(function), m_Argument(argument)
{
    m_ThreadHandle = new ThreadHandle;
    auto functionPtr = m_ThreadFunction.target<void* (*)(void*)>();
    m_LastError = PIL_THREADING_CreateThread(m_ThreadHandle, *functionPtr, argument);
}

PIL::Threading::~Threading()
{
        delete m_ThreadHandle;
}

void PIL::Threading::Run()
{
    m_LastError = PIL_THREADING_RunThread(m_ThreadHandle, FALSE); // TODO fix
}

void PIL::Threading::Join()
{
    m_LastError = PIL_THREADING_JoinThread(m_ThreadHandle, &m_RetValue);
}

void PIL::Threading::Abort()
{
    m_LastError = PIL_THREADING_AbortThread(m_ThreadHandle);
}

void PIL::Threading::Detach()
{
    m_LastError = PIL_THREADING_Detach(m_ThreadHandle);
}




#endif // CXX