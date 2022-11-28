//
// Created by florianfrank on 31.05.22.
//
#pragma once

extern "C" {
    #include "Logging.h"
}

#include <string>

namespace PIL {

#ifdef WIN32
#undef ERROR
#endif //

    enum Level {
      INFO = INFO_LVL,
      DEBUG = DEBUG_LVL,
      WARNING = WARNING_LVL,
      ERROR = ERROR_LVL
    };

    class Logging {
    public:
        Logging();
        Logging(Level logLevel, std::string *fileName);
        ~Logging();

        void CloseLogFile();

        void LogMessage(Level level, const char* fileName, unsigned int lineNumber, std::string message, ...);
        void Log(Level level, const char* fileName, unsigned int lineNumber, std::string message);
    private:
        static ::Level PILLOGDefinesToCLogDefines(Level);

    };
}