//
// Created by florianfrank on 14.06.21.
//
#include "ctlib/CSVHandler.h"
#include "ctlib/FileHandler.h"

#include <malloc.h>
#include <string.h>

PIL_ERROR_CODE
PIL_OpenCSVFile(PIL_CSVHandle *csvHandle, const char *fileName, PIL_BOOL overwrite, char delimiter, char separator)
{
    if(!csvHandle)
        return PIL_INVALID_ARGUMENTS;

    csvHandle->delimiter = delimiter;
    csvHandle->seperator = separator;
    csvHandle->fileHandle = malloc(sizeof(PIL_FileHandle));

    PIL_FileMode fileMode = CREATE_APPEND;
    if(overwrite)
        fileMode = CREATE_TRUNCATE;

    return PIL_OpenFile(csvHandle->fileHandle, fileName, READ_WRITE, fileMode);
}


PIL_ERROR_CODE PIL_WriteLineArray(PIL_CSVHandle csvHandle, char **data, uint32_t *nrElements)
{
    if (!data || !nrElements)
        return PIL_INVALID_ARGUMENTS;

    // TODO hardcoded line length
    char buffer[2048];
    memset(buffer, 0x00, 2048);

    for (int i = 0; i < *nrElements; i++)
    {
        if((strlen(buffer) + strlen(data[i]) + 1) > 2048)
            return PIL_INSUFFICIENT_RESOURCES;

        if(i < *nrElements - 1)
            sprintf(&buffer[strlen(buffer)], "%s%c", data[i], csvHandle.delimiter);
        else
            sprintf(&buffer[strlen(buffer)], "%s%c", data[i], csvHandle.seperator);

    }

    *nrElements = strlen(buffer);
    return PIL_WriteFile(csvHandle.fileHandle, (uint8_t*)buffer, nrElements, TRUE);
}

PIL_ERROR_CODE PIL_WriteLineString(PIL_CSVHandle csvHandle, char *data, uint32_t *nrElements)
{
    if (!data || !nrElements)
        return PIL_INVALID_ARGUMENTS;

    // TODO buffersize
    char buffer[2048];
    if(csvHandle.delimiter == '!')
        sprintf(buffer, "%s", data);
    else
        sprintf(buffer, "%s%c", data, csvHandle.delimiter);

    *nrElements = strlen(buffer);
    return PIL_WriteFile(csvHandle.fileHandle, (uint8_t*)buffer, nrElements, TRUE);
}

//PIL_ERROR_CODE ReadNextLIne(char* data, uint32_t *bufferSize);


PIL_ERROR_CODE PIL_CloseCSVFile(PIL_CSVHandle csvHandle)
{
    PIL_ERROR_CODE ret = PIL_CloseFile(csvHandle.fileHandle);
    free(csvHandle.fileHandle);
    return ret;
}
