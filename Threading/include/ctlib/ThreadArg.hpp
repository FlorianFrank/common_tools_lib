/**
 * @author Florian Frank
 * @copyright University of Passau - Chair of Computer Engineering
 */
#pragma once

#include <memory> // std::unique_ptr
#include <functional> // std::function

typedef void *(*ThreadFuncC)(void *);

namespace PIL {
    template<class T>
    /**
     * @brief Template class to pass thread arguments wrapping C++ objects to c thread functions.
     */
    class ThreadArgCXX {
    public:
        ThreadArgCXX<T>(std::function<void *(std::unique_ptr<T> &)> &function, std::unique_ptr<T> argument) :
                m_ThreadFunction(function), m_Argument(std::move(argument)),
                m_CThreadFunction([](void *thArg) -> void * {
                    auto threadArg = reinterpret_cast<ThreadArgCXX<T> *>(thArg);
                    return threadArg->m_ThreadFunction(threadArg->m_Argument);
                }) {
        }

        ThreadFuncC getCFunctionPtr() {
            return m_CThreadFunction;
        }
    private:
        /** Argument to pass to the C++ thread function. */
        std::unique_ptr<T> m_Argument;
        /** C thread function required to call the C++ function pointer from the c-thread. */
        void *(*m_CThreadFunction)(void *);
        /** C++ thread function called from the c thread function passing m_Argument to the thread.  */
        std::function<void *(std::unique_ptr<T> &)> m_ThreadFunction;
    };
}
