/**
 * @copyright University of Passau - Chair of Computer Engineering
 * @author Florian Frank
 */
#pragma once

#include "ctlib/ErrorHandler.hpp"
#include "ctlib/ThreadArg.hpp"
#include "ctlib/Exception.h"
#include "ctlib/Logging.h"

extern "C" {
#include "Threading.h"
}

namespace PIL
{
    template <typename T>
    class Threading : public ErrorHandler
    {
    public:
        Threading(std::function<void*(std::unique_ptr<T>&)> &function, std::unique_ptr<T> &argument);
        explicit Threading(std::unique_ptr<ThreadArgCXX<T>> &threadArg);

        PIL_ERROR_CODE Run(bool loop = FALSE);

        __attribute__((unused)) PIL_ERROR_CODE Detach();

        PIL_ERROR_CODE Join();

        PIL_ERROR_CODE Abort();

    private:
        std::unique_ptr<ThreadArgCXX<T>> m_ThreadArg;
        std::unique_ptr<ThreadHandle> m_ThreadHandle;
        void *m_RetValue{};
    };
}

template<typename T>
PIL::Threading<T>::Threading(std::function<void*(std::unique_ptr<T>&)> &function, std::unique_ptr<T> &argument) : ErrorHandler(),
            m_ThreadArg(std::make_unique<ThreadArgCXX<T>>(function, std::move(argument))), m_ThreadHandle(std::make_unique<ThreadHandle>()) {
    m_LastError = PIL_THREADING_CreateThread(m_ThreadHandle.get(), m_ThreadArg->getCFunctionPtr(),
                                             reinterpret_cast<void*>(m_ThreadArg.get()));
#ifdef PIL_EXCEPTION_HANDLING
    if(m_LastError != PIL_NO_ERROR)
        throw PIL::Exception(m_LastError, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
}

template <typename T>
PIL::Threading<T>::Threading(std::unique_ptr<ThreadArgCXX<T>> &threadArg): ErrorHandler(), m_ThreadArg(std::move(threadArg)),
m_ThreadHandle(std::unique_ptr<ThreadHandle>()) {
    m_LastError = PIL_THREADING_CreateThread(m_ThreadHandle.get(), m_ThreadArg.getCFunctionPtr(),
                                             reinterpret_cast<void*>(m_ThreadArg.get()));
#ifdef PIL_EXCEPTION_HANDLING
    if(m_LastError != PIL_NO_ERROR)
        throw PIL::Exception(m_LastError, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
}

template <typename T>
PIL_ERROR_CODE PIL::Threading<T>::Run(bool loop)
{
    auto errCode = PIL_THREADING_RunThread(m_ThreadHandle.get(), loop); // TODO fix
#ifdef PIL_EXCEPTION_HANDLING
    if(errCode != PIL_NO_ERROR)
        throw PIL::Exception(errCode, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
    return errCode;
}

template <typename T>
PIL_ERROR_CODE PIL::Threading<T>::Join()
{
    auto errCode = PIL_THREADING_JoinThread(m_ThreadHandle.get(), &m_RetValue);
#ifdef PIL_EXCEPTION_HANDLING
    if(errCode != PIL_NO_ERROR)
        throw PIL::Exception(errCode, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
    return errCode;
}

template <typename T>
PIL_ERROR_CODE PIL::Threading<T>::Abort()
{
    auto errCode = PIL_THREADING_AbortThread(m_ThreadHandle.get());
#ifdef PIL_EXCEPTION_HANDLING
    if(errCode != PIL_NO_ERROR)
        throw PIL::Exception(errCode, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
    return errCode;

}

template <typename T>
__attribute__((unused)) PIL_ERROR_CODE PIL::Threading<T>::Detach()
{
    auto errCode = PIL_THREADING_Detach(m_ThreadHandle.get());
#ifdef PIL_EXCEPTION_HANDLING
    if(errCode != PIL_NO_ERROR)
        throw PIL::Exception(errCode, __FILENAME__, __LINE__);
#endif // PIL_EXCEPTION_HANDLING
    return errCode;

}