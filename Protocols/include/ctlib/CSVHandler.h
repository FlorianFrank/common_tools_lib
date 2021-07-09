//
// Created by florianfrank on 14.06.21.
//

#ifndef PLATFORMINDEPENDENTLIB_CSV_HANDLER_H
#define PLATFORMINDEPENDENTLIB_CSV_HANDLER_H

#include <ctlib/DataTypeDefines.h>
#include "ctlib/ErrorCodeDefines.h"


struct PIL_CSVHandle {
    void* fileHandle;
    char delimiter;
    char seperator;
} typedef PIL_CSVHandle;

PIL_ERROR_CODE
PIL_OpenCSVFile(PIL_CSVHandle *csvHandle, const char *fileName, PIL_BOOL overwrite, char delimiter, char separator);
PIL_ERROR_CODE PIL_WriteLineArray(PIL_CSVHandle csvHandle, char **data, uint32_t *nrElements);
PIL_ERROR_CODE PIL_WriteLineString(PIL_CSVHandle csvHandle, char *data, uint32_t *nrElements);
//PIL_ERROR_CODE ReadNextLIne(char* data, uint32_t *bufferSize);
PIL_ERROR_CODE PIL_CloseCSVFile(PIL_CSVHandle csvHandle);


#endif //PLATFORMINDEPENDENTLIB_CSV_HANDLER_H
