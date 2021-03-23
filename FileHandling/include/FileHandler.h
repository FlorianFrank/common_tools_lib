//
// Created by florianfrank on 22.03.21.
//

#ifndef PLATFORMINDEPENDENTLIB_FILEHANDLER_H
#define PLATFORMINDEPENDENTLIB_FILEHANDLER_H

#include "ErrorCodeDefines.h"
#include "FileHandlingDefines.h"

PIL_ERROR_CODE PIL_OpenFile(PIL_FileHandle *fileHandle, const char* fileName, PIL_FilePermissions permission, PIL_FileMode fileMode);
PIL_ERROR_CODE PIL_CloseFile(PIL_FileHandle *fileHandle);
PIL_ERROR_CODE PIL_WriteFile(PIL_FileHandle *fileHandle, uint8_t* buffer, uint32_t *bufferLen);
PIL_ERROR_CODE PIL_ReadFile(PIL_FileHandle *fileHandle, uint8_t* buffer, const uint32_t *bufferLen);

PIL_ERROR_CODE PIL_WriteDataToFile(const char* fileName, uint8_t* buffer, uint32_t bufferLen, PIL_FileMode mode);


#endif //PLATFORMINDEPENDENTLIB_FILEHANDLER_H
