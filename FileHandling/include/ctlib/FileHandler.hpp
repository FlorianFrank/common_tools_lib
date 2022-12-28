//
// Created by florianfrank on 04.07.22.
//

#ifndef INSTRUMENT_CONTROL_LIB_FILEHANDLER_HPP
#define INSTRUMENT_CONTROL_LIB_FILEHANDLER_HPP

extern "C" {
#include "FileHandler.h"
};

#include <string>

namespace PIL {
    class FileHandler
    {
    public:
        FileHandler(std::string &fileName, PIL_FilePermissions permissions, PIL_FileMode mode);
        ~FileHandler();

        PIL_ERROR_CODE CloseFile();
        PIL_ERROR_CODE WriteFile(std::string &buffer, bool flush);
        PIL_ERROR_CODE ReadFile(std::string *buffer);
        static PIL_ERROR_CODE ListFilesInDirectory(std::string &path, uint8_t filter, PIL_FileListElem *listOfFiles, bool recursive);

    private:
        std::string m_FileName;
        PIL_FilePermissions m_Permissions;
        PIL_FileMode m_Mode;
        PIL_FileHandle m_Handle;
        PIL_ERROR_CODE m_LastError;
    };
};
#endif //INSTRUMENT_CONTROL_LIB_FILEHANDLER_HPP
