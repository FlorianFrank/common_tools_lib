//
// Created by florianfrank on 22.03.21.
//

#ifndef PLATFORMINDEPENDENTLIB_FILEHANDLINGDEFINES_H
#define PLATFORMINDEPENDENTLIB_FILEHANDLINGDEFINES_H
#include <stdio.h> // TODO noch entfernen
#include "DataTypeDefines.h"

#define MAX_PATH_LENGTH 512

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

typedef enum PIL_FILE_TYPE
{
    ALL = 254,
    UNKNOWN = 1,
    DIRECTORIES = 2,
    REGULAR_FILES = 4,
    SYMBOLIC_LINKS = 8,
    FIFO = 16,
    SOCKETS = 32,
    CHARACTER_DEVICE = 64
} PIL_FilterDirElements;

struct PIL_FileHandle
{
    FILE* fileHandle;
    enum PIL_FILE_TYPE type;
    char path[MAX_PATH_LENGTH];
    PIL_BOOL isOpen;
    int errCode;
} typedef PIL_FileHandle;

struct PIL_FileListElem
{
    struct PIL_FileListElem *previous;
    PIL_FileHandle *handle;
    struct PIL_FileListElem *next;

} typedef PIL_FileListElem;

#endif //PLATFORMINDEPENDENTLIB_FILEHANDLINGDEFINES_H
