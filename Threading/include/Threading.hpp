//
// Created by florianfrank on 30.05.21.
//

#ifndef PLATFORMINDEPENDENTLIB_THREADING_HPP
#define PLATFORMINDEPENDENTLIB_THREADING_HPP

#include "ErrorHandler.hpp"

extern "C" {
#include "Threading.h"
};


namespace PIL
{

    class Threading : public ErrorHandler
    {
    public:
        Threading(void *(*threadFunction)(void *), void *argument);

        void Run();

        void Detach();

        void Join();

        void Abort();

    private:
        ThreadHandle *m_ThreadHandle;
        void *m_argument;
        void *m_RetValue;
        void *(*m_ThreadFunction)(void *function);

    };
}


#endif //PLATFORMINDEPENDENTLIB_THREADING_HPP
