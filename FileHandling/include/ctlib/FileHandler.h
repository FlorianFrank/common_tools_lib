//
// Created by florianfrank on 22.03.21.
//

#ifndef PLATFORMINDEPENDENTLIB_FILEHANDLER_H
#define PLATFORMINDEPENDENTLIB_FILEHANDLER_H

#include "ctlib/ErrorCodeDefines.h"
#include "ctlib/FileHandlingDefines.h"

PIL_ERROR_CODE PIL_OpenFile(PIL_FileHandle *fileHandle, const char* fileName, PIL_FilePermissions permission, PIL_FileMode fileMode);
PIL_ERROR_CODE PIL_CloseFile(PIL_FileHandle *fileHandle);
PIL_ERROR_CODE PIL_WriteFile(PIL_FileHandle *fileHandle, uint8_t* buffer, uint32_t *bufferLen, PIL_BOOL flush);
PIL_ERROR_CODE PIL_ReadFile(PIL_FileHandle *fileHandle, uint8_t* buffer, uint32_t *bufferLen);
PIL_ERROR_CODE PIL_ListFilesInDirectory(const char *path, uint8_t filter, PIL_FileListElem *listOfFiles, PIL_BOOL recursive);

PIL_ERROR_CODE PIL_WriteDataToFile(const char* fileName, uint8_t* buffer, uint32_t bufferLen, PIL_FileMode mode);
PIL_ERROR_CODE PIL_ReadDataFromFile(const char* fileName, uint8_t* buffer, uint32_t *bufferLen);


#endif //PLATFORMINDEPENDENTLIB_FILEHANDLER_H
