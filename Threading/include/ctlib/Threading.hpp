//
// Created by florianfrank on 30.05.21.
//

#ifndef PLATFORMINDEPENDENTLIB_THREADING_HPP
#define PLATFORMINDEPENDENTLIB_THREADING_HPP

#include "ctlib/ErrorHandler.hpp"

extern "C" {
#include "Threading.h"
}


namespace PIL
{

    class Threading : public ErrorHandler
    {
    public:
        Threading(std::function<void*(void *)> &function, void *argument);
        ~Threading();

        void Run();

        void Detach();

        void Join();

        void Abort();

    private:
        ThreadHandle *m_ThreadHandle;
        void *m_Argument;
        void *m_RetValue{};
        std::function<void*(void*)> m_ThreadFunction;
    };
}


#endif //PLATFORMINDEPENDENTLIB_THREADING_HPP
