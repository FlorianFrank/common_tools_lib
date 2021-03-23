#include <stdio.h>
#include <errno.h>
#include "../include/FileHandler.h"

/**
 * @brief This function opens a file for reading and writing
 * @param fileHandle fileHandle returned for further actions, like reading and writing.
 * @param fileName path and name of the file to open.
 * @param permission permission of the file. (Read/write/Read_Write)
 * @param fileMode Mode when opening in write mode. (Append, create and truncate)
 * @return PIL_NO_ERROR when no error occurs.
 */
PIL_ERROR_CODE PIL_OpenFile(PIL_FileHandle *fileHandle, const char* fileName, PIL_FilePermissions permission, PIL_FileMode fileMode)
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
    {
        fileHandle->errCode = errno;
        if(fileHandle->errCode == 2)
            return PIL_NO_SUCH_FILE;
        return PIL_ERRNO;
    }

    fileHandle->errCode = 0;
    fileHandle->isOpen = TRUE;
    return PIL_NO_ERROR;
}

/**
 * @brief This function closes an open file.
 * @param fileHandle file handle corresponding to the file to close.
 * @return PIL_NO_ERROR if no error occurs.
 */
PIL_ERROR_CODE PIL_CloseFile(PIL_FileHandle *fileHandle)
{
    if(!fileHandle)
        return PIL_INVALID_ARGUMENTS;

    fileHandle->isOpen = FALSE;
    int ret = fclose(fileHandle->fileHandle);
    if(ret != 0)
        return PIL_ERRNO;

    return PIL_NO_ERROR;
}

/**
 * @brief This function writes the data passed by the buffer argument to the file.
 * @param fileHandle file handle returned by the PIL_OpenFile function.
 * @param buffer buffer to write.
 * @param bufferLen number of byte to write. Also returns the number actually written.
 * @return PIL_NO_ERROR if no error occurs.
 */
PIL_ERROR_CODE PIL_WriteFile(PIL_FileHandle *fileHandle, uint8_t* buffer, uint32_t *bufferLen)
{
    if(!fileHandle)
        return PIL_INVALID_ARGUMENTS;

    if(!fileHandle->isOpen)
        return PIL_INTERFACE_CLOSED;

    size_t ret = fwrite(buffer, 1, *bufferLen, fileHandle->fileHandle);
    if(ret < *bufferLen)
    {
        *bufferLen = ret;
        fileHandle->errCode = ferror(fileHandle->fileHandle);
        return PIL_ONLY_PARTIALLY_READ_WRITTEN;
    }

    return PIL_NO_ERROR;
}

/**
 * @brief This function reads data from a file.
 * @param fileHandle file handle returned by the PIL_OpenFile function.
 * @param buffer return buffer contains the read data.
 * @param bufferLen maximum length of the data which can be written into the buffer.
 * @return PIL_NO_ERROR if no error occurs.
 */
PIL_ERROR_CODE PIL_ReadFile(PIL_FileHandle *fileHandle, uint8_t* buffer, uint32_t *bufferLen)
{
    if(!fileHandle)
        return PIL_INVALID_ARGUMENTS;

    if(!fileHandle->isOpen)
        return PIL_INTERFACE_CLOSED;

    int sizeToWrite = *bufferLen;
    size_t ret = fread(buffer, 1, *bufferLen, fileHandle->fileHandle);
    if(ret < sizeToWrite)
    {
        *bufferLen = ret;
        fileHandle->errCode = ferror(fileHandle->fileHandle);
        return PIL_ONLY_PARTIALLY_READ_WRITTEN;
    }

    return PIL_NO_ERROR;
}

/**
 * @brief This function creates a file, writes the data passed in the buffer argument to a file and closes the file.
 * @param fileName path and name of the file.
 * @param buffer the buffer contains the data to write.
 * @param bufferLen length of the buffer to write
 * @param mode mode of the operation (create, truncate, append)
 * @return PIL_NO_ERROR if no error occurs
 */
PIL_ERROR_CODE PIL_WriteDataToFile(const char* fileName, uint8_t* buffer, uint32_t bufferLen, PIL_FileMode mode)
{
    if(!fileName || !buffer)
        return PIL_INVALID_ARGUMENTS;

    PIL_FileHandle fileHandle;
    PIL_ERROR_CODE ret = PIL_OpenFile(&fileHandle, fileName, READ_WRITE, mode);
    if(ret != PIL_NO_ERROR)
        return ret;

    ret = PIL_WriteFile(&fileHandle, buffer, &bufferLen);
    if (ret != PIL_NO_ERROR)
        return ret;

    return PIL_CloseFile(&fileHandle);
}

/**
 * @brief This function opens a file, reads from the file and closes the file handle afterwards.
 * @param fileName path and name of the file to read.
 * @param buffer buffer containing the read data.
 * @param bufferLen
 * @return
 */
PIL_ERROR_CODE PIL_ReadDataFromFile(const char* fileName, uint8_t* buffer, uint32_t* bufferLen)
{
    if(!fileName || !buffer || !bufferLen || *bufferLen < 1)
        return PIL_INVALID_ARGUMENTS;

    PIL_FileHandle fileHandle;
    PIL_ERROR_CODE ret = PIL_OpenFile(&fileHandle, fileName, READ, NONE);
    if(ret != PIL_NO_ERROR)
        return ret;

    ret = PIL_ReadFile(&fileHandle, buffer, bufferLen);
    if(ret != PIL_NO_ERROR)
        return ret;

    return PIL_CloseFile(&fileHandle);
}
