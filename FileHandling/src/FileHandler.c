//
// Created by florianfrank on 22.03.21.
//

#include <stdio.h>
#include "../include/FileHandler.h"

PIL_ERROR_CODE OpenFile(FileHandle *fileHandle, const char* fileName, FilePermissions permission, FileMode fileMode)
{
    if(!fileHandle)
        return PIL_INVALID_ARGUMENTS;

    const char *p;
    if(permission == READ && fileMode == NONE)
        p = "r";
    else if(permission == READ_WRITE && fileMode == NONE)
        p = "r+";
    else if(permission == WRITE && fileMode == CREATE_TRUNCATE)
        p = "w";
    else if(permission == WRITE && fileMode == CREATE_APPEND)
        p = "a";
    else if(permission == READ_WRITE && fileMode == CREATE_TRUNCATE)
        p = "w+";
    else if(permission == READ_WRITE && fileMode == CREATE_APPEND)
        p = "a+";
    else
        return PIL_INVALID_ARGUMENTS;


    fileHandle->fileHandle = fopen(fileName, p);
    if(!fileHandle->fileHandle)
        return PIL_ERRNO;

    fileHandle->isOpen = TRUE;
    return PIL_NO_ERROR;
}

PIL_ERROR_CODE CloseFile(FileHandle *fileHandle)
{
    if(!fileHandle)
        return PIL_INVALID_ARGUMENTS;

    fileHandle->isOpen = FALSE;
    int ret = fclose(fileHandle->fileHandle);
    if(ret != 0)
        return PIL_ERRNO;

    return PIL_NO_ERROR;
}

PIL_ERROR_CODE WriteFile(FileHandle *fileHandle, uint8_t* buffer, uint32_t *bufferLen)
{
    if(!fileHandle)
        return PIL_INVALID_ARGUMENTS;

    if(!fileHandle->isOpen)
        return PIL_INTERFACE_CLOSED;

    int ret = fwrite(buffer, 1, *bufferLen, fileHandle->fileHandle);
    if(ret < 0)
        return PIL_ERRNO;
    if(ret < *bufferLen)
        return PIL_ONLY_PARTIALLY_READ_WRITTEN;


    return PIL_NO_ERROR;
}

PIL_ERROR_CODE ReadFile(FileHandle *fileHandle, uint8_t* buffer, uint32_t *bufferLen)
{
    if(!fileHandle)
        return PIL_INVALID_ARGUMENTS;

    if(!fileHandle->isOpen)
        return PIL_INTERFACE_CLOSED;

    int ret = fread(buffer, 1, *bufferLen, fileHandle->fileHandle);
    if(ret < 0)
        return PIL_ERRNO;
    if(ret < *bufferLen)
        return PIL_ONLY_PARTIALLY_READ_WRITTEN;

    return PIL_NO_ERROR;
}
