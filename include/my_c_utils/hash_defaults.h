#ifndef MY_C_UTILS_HASH_DEFAULTS_H
#define MY_C_UTILS_HASH_DEFAULTS_H

#include "my_c_utils/tipos.h"

static inline Size my_c_utils_fnv1a(const void *data, Size size)
{
    const unsigned char *bytes = (const unsigned char *)data;
    Size hash = 2166136261U;
    for (Size i = 0; i < size; ++i)
    {
        hash ^= bytes[i];
        hash *= 16777619U;
    }
    return hash;
}

#define PRIMITIVE_HASH_EQUALS(T) \
    static inline Size T##_hash(T value) \
    { \
        return my_c_utils_fnv1a(&value, sizeof(T)); \
    } \
    static inline Bool T##_equals(T a, T b) \
    { \
        return a == b; \
    }

#endif // MY_C_UTILS_HASH_DEFAULTS_H
