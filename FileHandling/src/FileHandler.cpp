/**
 * @brief This File contains a C++ implementation wrapping the Filehandler file.
 * @author Florian Frank
 * @copyright University of Passau
 */
#include "ctlib/FileHandler.hpp"
#include "ctlib/ExceptionHandler.h"

PIL::FileHandler::FileHandler(std::string &fileName, PIL_FilePermissions permissions, PIL_FileMode mode):
m_FileName(fileName), m_Permissions(permissions), m_Mode(mode)
{
    m_LastError = PIL_OpenFile(&m_Handle, fileName.c_str(), m_Permissions, m_Mode);
}


PIL::FileHandler::~FileHandler()
{
    CloseFile();
}


PIL_ERROR_CODE PIL::FileHandler::CloseFile()
{
    auto ret = PIL_CloseFile(&m_Handle);
#ifdef PIL_EXCEPTION_HANDLING
    if(ret != PIL_NO_ERROR)
        throw ExceptionHandler(ret);
#endif // PIL_EXCEPTION_HANDLING
    return ret;
}


PIL_ERROR_CODE PIL::FileHandler::WriteFile(std::string &buffer, bool flush)
{
    uint32_t bufferLen = buffer.length();
    auto ret = PIL_WriteFile(&m_Handle, reinterpret_cast<uint8_t *>(const_cast<char *>(buffer.c_str())), &bufferLen,
                             (flush) ? TRUE : FALSE);
#ifdef PIL_EXCEPTION_HANDLING
    if(ret != PIL_NO_ERROR)
        throw ExceptionHandler(ret);
#endif // PIL_EXCEPTION_HANDLING
    return ret;
}


PIL_ERROR_CODE PIL::FileHandler::ReadFile(std::string *buffer)
{
    uint32_t buffLen = 1024; // TODO
    uint8_t cBuff[1024];
    auto ret = PIL_ReadFile(&m_Handle, cBuff, &buffLen);

#ifdef PIL_EXCEPTION_HANDLING
    if(ret != PIL_NO_ERROR)
        throw ExceptionHandler(ret);
#endif // PIL_EXCEPTION_HANDLING
    if(!buffer)
    {
#ifdef PIL_EXCEPTION_HANDLING
        if(ret != PIL_NO_ERROR)
        throw ExceptionHandler(PIL_INVALID_ARGUMENTS);
#endif // PIL_EXCEPTION_HANDLING
        return PIL_INVALID_ARGUMENTS;
    }
    *buffer = std::string(reinterpret_cast<char *>(cBuff), buffLen);
    return ret;
}


/*static*/ PIL_ERROR_CODE
PIL::FileHandler::ListFilesInDirectory(std::string &path, uint8_t filter, PIL_FileListElem *listOfFiles, bool recursive)
{
    auto ret = PIL_ListFilesInDirectory(path.c_str(), filter, listOfFiles, (recursive) ? TRUE : FALSE);
#ifdef PIL_EXCEPTION_HANDLING
    if(ret != PIL_NO_ERROR)
        throw ExceptionHandler(PIL_INVALID_ARGUMENTS);
#endif // PIL_EXCEPTION_HANDLING
    return ret;
}
