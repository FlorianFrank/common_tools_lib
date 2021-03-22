//
// Created by florianfrank on 22.03.21.
//

#ifndef PLATFORMINDEPENDENTLIB_FILEHANDLER_H
#define PLATFORMINDEPENDENTLIB_FILEHANDLER_H

#include "ErrorCodeDefines.h"
#include "FileHandlingDefines.h"

PIL_ERROR_CODE OpenFile(FileHandle *fileHandle, const char* fileName, FilePermissions permission, FileMode fileMode);
PIL_ERROR_CODE CloseFile(FileHandle *fileHandle);
PIL_ERROR_CODE WriteFile(FileHandle *fileHandle, uint8_t* buffer, uint32_t *bufferLen);
PIL_ERROR_CODE ReadFile(FileHandle *fileHandle, uint8_t* buffer, uint32_t *bufferLen);
#endif //PLATFORMINDEPENDENTLIB_FILEHANDLER_H
