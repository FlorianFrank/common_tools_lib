//
// Created by florianfrank on 31.05.22.
//
#pragma once

extern "C" {
    #include "Logging.h"
}

#include <string>

namespace PIL {
    class Logging {
    public:
        Logging(Level logLevel, std::string &fileName);
        ~Logging();

        void CloseLogFile();

        void LogMessage(Level level, std::string &fileName, unsigned int lineNumber, std::string& message, ...);

    };
}