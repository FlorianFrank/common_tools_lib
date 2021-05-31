//
// Created by florianfrank on 30.05.21.
//
#ifdef CXX


#include "Threading.hpp"


PIL::Threading::Threading(void *(*threadFunction)(void *), void *argument) : ErrorHandler(),
m_ThreadFunction(threadFunction), m_argument(argument)
{
    m_LastError = PIL_THREADING_CreateThread(m_ThreadHandle, m_ThreadFunction, argument);
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