/**
 * @brief This file serves as a wrapper for platform and architecture independent representation of data types.
 * @author Florian Frank
 */
#ifndef PLATFORM_INDEPENDENT_LIB_DATATYPE_DEFINES_H
#define PLATFORM_INDEPENDENT_LIB_DATATYPE_DEFINES_H

#include <stdint.h>

/** @brief Boolean data type implementation on C. **/
typedef uint8_t PIL_BOOL;

#define TRUE    1
#define FALSE   0

#endif //PLATFORM_INDEPENDENT_LIB_DATATYPE_DEFINES_H
