//
// Created by florianfrank on 22.03.21.
//

#ifndef PLATFORMINDEPENDENTLIB_FILEHANDLINGDEFINES_H
#define PLATFORMINDEPENDENTLIB_FILEHANDLINGDEFINES_H
#include <stdio.h> // TODO noch entfernen
#include "DataTypeDefines.h"

typedef enum FilePermissions
{
    READ = 0,
    WRITE = 1,
    READ_WRITE = 2,
}  FilePermissions;

typedef enum FileMode
{
    NONE = 0,
    CREATE_APPEND = 1,
    CREATE_TRUNCATE = 2
} FileMode;

struct FileHandle
{
  FILE* fileHandle;
  PIL_BOOL isOpen;
} typedef FileHandle;

#endif //PLATFORMINDEPENDENTLIB_FILEHANDLINGDEFINES_H
