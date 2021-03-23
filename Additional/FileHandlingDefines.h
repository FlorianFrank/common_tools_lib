//
// Created by florianfrank on 22.03.21.
//

#ifndef PLATFORMINDEPENDENTLIB_FILEHANDLINGDEFINES_H
#define PLATFORMINDEPENDENTLIB_FILEHANDLINGDEFINES_H
#include <stdio.h> // TODO noch entfernen
#include "DataTypeDefines.h"

typedef enum PIL_FilePermissions
{
    READ = 0,
    WRITE = 1,
    READ_WRITE = 2,
}  PIL_FilePermissions;

typedef enum PIL_FileMode
{
    NONE = 0,
    CREATE_APPEND = 1,
    CREATE_TRUNCATE = 2
} PIL_FileMode;

struct PIL_FileHandle
{
  FILE* fileHandle;
  PIL_BOOL isOpen;
  int errCode;
} typedef PIL_FileHandle;

#endif //PLATFORMINDEPENDENTLIB_FILEHANDLINGDEFINES_H
