#ifndef MY_C_UTILS_RESULT_H
#define MY_C_UTILS_RESULT_H

#include "my_c_utils/tipos.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define Result(Type, Err) MY_C_UTILS_CONCAT(Result_, MY_C_UTILS_CONCAT(Type, MY_C_UTILS_CONCAT(_, Err)))
#define ref_Result(Type, Err) MY_C_UTILS_CONCAT(ref_Result_, MY_C_UTILS_CONCAT(Type, MY_C_UTILS_CONCAT(_, Err)))
#define cref_Result(Type, Err) MY_C_UTILS_CONCAT(cref_Result_, MY_C_UTILS_CONCAT(Type, MY_C_UTILS_CONCAT(_, Err)))

#define Result_is_ok(Type, Err) MY_C_UTILS_CONCAT(Result(Type, Err), _is_ok)
#define Result_is_err(Type, Err) MY_C_UTILS_CONCAT(Result(Type, Err), _is_err)
#define Result_unwrap(Type, Err) MY_C_UTILS_CONCAT(Result(Type, Err), _unwrap)
#define Result_unwrap_err(Type, Err) MY_C_UTILS_CONCAT(Result(Type, Err), _unwrap_err)
#define Result_free(Type, Err) MY_C_UTILS_CONCAT(Result(Type, Err), _free)
#define Result_ok(Type, Err) MY_C_UTILS_CONCAT(Result(Type, Err), _ok)
#define Result_err(Type, Err) MY_C_UTILS_CONCAT(Result(Type, Err), _err)

#define RESULT_CONFIG(Type, Err) RESULT_CONFIG_IMPL(Type, Err)

#define RESULT_CONFIG_IMPL(Type, Err)                                             \
    typedef struct                                                                \
    {                                                                             \
        union                                                                     \
        {                                                                         \
            Type value;                                                           \
            Err error;                                                            \
        };                                                                        \
        Bool is_error;                                                            \
    } Result_##Type##_##Err;                                                      \
    REF_EXPAND(Result_##Type##_##Err)                                             \
                                                                                  \
    static inline Result_##Type##_##Err Result_##Type##_##Err##_ok(Type value)    \
    {                                                                             \
        return (Result_##Type##_##Err){.value = value, .is_error = false};        \
    }                                                                             \
                                                                                  \
    static inline Result_##Type##_##Err Result_##Type##_##Err##_err(Err error)    \
    {                                                                             \
        return (Result_##Type##_##Err){.error = error, .is_error = true};         \
    }                                                                             \
                                                                                  \
    static inline Type Result_##Type##_##Err##_unwrap(Result_##Type##_##Err result)\
    {                                                                             \
        if (result.is_error)                                                      \
        {                                                                         \
            fputs("Unwrap on error\n", stderr);                                   \
            exit(1);                                                              \
        }                                                                         \
        return result.value;                                                      \
    }                                                                             \
                                                                                  \
    static inline Bool Result_##Type##_##Err##_is_err(cref_Result_##Type##_##Err result)\
    {                                                                             \
        return result->is_error;                                                  \
    }                                                                             \
                                                                                  \
    static inline Bool Result_##Type##_##Err##_is_ok(cref_Result_##Type##_##Err result)\
    {                                                                             \
        return !(result->is_error);                                               \
    }                                                                             \
                                                                                  \
    static inline Err Result_##Type##_##Err##_unwrap_err(Result_##Type##_##Err result)\
    {                                                                             \
        if (!result.is_error)                                                     \
        {                                                                         \
            perror("Result is ok");                                               \
            exit(1);                                                              \
        }                                                                         \
        return result.error;                                                      \
    }                                                                             \
                                                                                  \
    static inline void Result_##Type##_##Err##_free(ref_Result_##Type##_##Err result)\
    {                                                                             \
        if (!result->is_error)                                                    \
        {                                                                         \
            Type##_free(&result->value);                                          \
        }                                                                             \
    }                                                                             \

typedef struct {} Void;
static inline void Void_free(Void *value)
{
    (void)value;
}

RESULT_CONFIG(Void, cref_Char)

#endif