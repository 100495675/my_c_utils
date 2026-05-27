#ifndef MY_C_UTILS_OPTION_H
#define MY_C_UTILS_OPTION_H

#include "my_c_utils/tipos.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define Option(Type) MY_C_UTILS_CONCAT(Option_, Type)
#define ref_Option(Type) MY_C_UTILS_CONCAT(ref_Option_, Type)
#define cref_Option(Type) MY_C_UTILS_CONCAT(cref_Option_, Type)

#define Option_some(Type) MY_C_UTILS_CONCAT(Option(Type), _some)
#define Option_none(Type) MY_C_UTILS_CONCAT(Option(Type), _none)
#define Option_unwrap(Type) MY_C_UTILS_CONCAT(Option(Type), _unwrap)
#define Option_is_none(Type) MY_C_UTILS_CONCAT(Option(Type), _is_none)
#define Option_is_some(Type) MY_C_UTILS_CONCAT(Option(Type), _is_some)
#define Option_free(Type) MY_C_UTILS_CONCAT(Option(Type), _free)

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
