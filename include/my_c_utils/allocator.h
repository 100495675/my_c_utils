#ifndef MY_C_UTILS_ALLOCATOR_H
#define MY_C_UTILS_ALLOCATOR_H

#include <stdlib.h>

#ifndef MY_C_UTILS_MALLOC
#define MY_C_UTILS_MALLOC(size) malloc(size)
#endif

#ifndef MY_C_UTILS_REALLOC
#define MY_C_UTILS_REALLOC(ptr, size) realloc(ptr, size)
#endif

#ifndef MY_C_UTILS_CALLOC
#define MY_C_UTILS_CALLOC(num, size) calloc(num, size)
#endif

#ifndef MY_C_UTILS_FREE
#define MY_C_UTILS_FREE(ptr) free(ptr)
#endif

#endif // MY_C_UTILS_ALLOCATOR_H
