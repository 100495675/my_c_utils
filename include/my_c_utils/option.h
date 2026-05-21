#ifndef MY_C_UTILS_OPTION_H
#define MY_C_UTILS_OPTION_H

#include "my_c_utils/tipos.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define OPTION_CONFIG(Type)                                               \
    typedef struct                                                        \
    {                                                                     \
        Type value;                                                       \
        Bool filled;                                                      \
    } Option_##Type;                                                      \
    REF_EXPAND(Option_##Type)                                             \
                                                                          \
    static inline Option_##Type Option_##Type##_some(Type value)          \
    {                                                                     \
        return (Option_##Type){.value = value, .filled = true};           \
    }                                                                     \
                                                                          \
    static inline Option_##Type Option_##Type##_none()                    \
    {                                                                     \
        return (Option_##Type){.filled = false};                          \
    }                                                                     \
                                                                          \
    static inline Type Option_##Type##_unwrap(Option_##Type option)       \
    {                                                                     \
        if (!option.filled)                                               \
        {                                                                 \
            perror("Option is none");                                     \
            exit(1);                                                      \
        }                                                                 \
        return option.value;                                              \
    }                                                                     \
                                                                          \
    static inline Bool Option_##Type##_is_none(cref_Option_##Type option) \
    {                                                                     \
        return !(option->filled);                                         \
    }                                                                     \
                                                                          \
    static inline Bool Option_##Type##_is_some(cref_Option_##Type option) \
    {                                                                     \
        return option->filled;                                            \
    }                                                                     \
                                                                          \
    static inline void Option_##Type##_free(ref_Option_##Type option)     \
    {                                                                     \
        if (option->filled)                                               \
        {                                                                 \
            Type##_free(&option->value);                                  \
        }                                                                 \
    }

#endif
