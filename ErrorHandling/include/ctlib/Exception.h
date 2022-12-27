//
// Created by florianfrank on 01.07.22.
//

#ifndef INSTRUMENT_CONTROL_LIB_EXCEPTION_H
#define INSTRUMENT_CONTROL_LIB_EXCEPTION_H

#include <ctlib/ErrorHandler.hpp>
#include <string>
#include <exception>

#define MAX_ERROR_MESSAGE_LENGTH 2048

namespace PIL {
    class Exception : public std::exception{
    public:
        explicit Exception(PIL_ErrorHandle errHandle);
        explicit Exception(PIL_ERROR_CODE errorCode);
        explicit Exception(PIL_ERROR_CODE errorCode, std::string errorMessage);
        explicit Exception(PIL_ERROR_CODE  errorCode, const char *fileName, unsigned int lineNumber, const std::string message, ...);


        explicit Exception(PIL_ERROR_CODE  errorCode, const char *fileName, unsigned int lineNumber, const std::string& message, va_list vaList);

        [[nodiscard]] const char *what() const noexcept override;

    private:
        PIL_ErrorHandle m_ErrorHandle{};
        std::string m_ErrorMessage;

        void setErrorMessageVarArgs(PIL_ERROR_CODE  errorCode, const char *fileName, unsigned int lineNumber, const std::string& message, va_list vaList);
    };
}


#endif //INSTRUMENT_CONTROL_LIB_EXCEPTION_H
