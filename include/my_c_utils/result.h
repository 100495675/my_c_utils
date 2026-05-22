#ifndef MY_C_UTILS_RESULT_H
#define MY_C_UTILS_RESULT_H

#include "my_c_utils/tipos.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define RESULT_CONFIG(Type)                                                       \
    typedef struct                                                                \
    {                                                                             \
        union                                                                     \
        {                                                                         \
            Type value;                                                           \
            cref_Char error_message;                                              \
        };                                                                        \
        Bool is_error;                                                            \
    } Result_Void_##Type;                                                              \
    REF_EXPAND(Result_Void_##Type)                                                     \
                                                                                  \
    static inline Result_Void_##Type Result_Void_##Type##_ok(Type value)                    \
    {                                                                             \
        return (Result_Void_##Type){.value = value, .is_error = false};                \
    }                                                                             \
                                                                                  \
    static inline Result_Void_##Type Result_Void_##Type##_err(cref_Char error_message)      \
    {                                                                             \
        return (Result_Void_##Type){.error_message = error_message, .is_error = true}; \
    }                                                                             \
                                                                                  \
    static inline Type Result_Void_##Type##_unwrap(Result_Void_##Type result)               \
    {                                                                             \
        if (result.is_error)                                                      \
        {                                                                         \
            fputs("Unwrap on error: ", stderr);                                   \
            fputs(result.error_message, stderr);                                  \
            fputc('\n', stderr);                                                  \
            exit(1);                                                              \
        }                                                                         \
        return result.value;                                                      \
    }                                                                             \
                                                                                  \
    static inline Bool Result_Void_##Type##_is_err(cref_Result_Void_##Type result)          \
    {                                                                             \
        return result->is_error;                                                  \
    }                                                                             \
                                                                                  \
    static inline Bool Result_Void_##Type##_is_ok(cref_Result_Void_##Type result)           \
    {                                                                             \
        return !(result->is_error);                                               \
    }                                                                             \
                                                                                  \
    static inline cref_Char Result_Void_##Type##_unwrap_err(Result_Void_##Type result)      \
    {                                                                             \
        if (!result.is_error)                                                     \
        {                                                                         \
            perror("Result_Void is ok");                                               \
            exit(1);                                                              \
        }                                                                         \
        return result.error_message;                                              \
    }                                                                             \
                                                                                  \
    static inline void Result_Void_##Type##_free(ref_Result_Void_##Type result)             \
    {                                                                             \
        if (!result->is_error)                                                    \
        {                                                                         \
            Type##_free(&result->value);                                          \
        }                                                                         \
    }

typedef struct
{
    cref_Char error_message;
    Bool is_error;
} Result_Void;
REF_EXPAND(Result_Void)

static inline Result_Void Result_Void_ok()
{
    return (Result_Void){.error_message = NULL, .is_error = false};
}

static inline Result_Void Result_Void_err(cref_Char error_message)
{
    return (Result_Void){.error_message = error_message, .is_error = true};
}

static inline void Result_Void_unwrap(Result_Void result)
{
    if (result.is_error)
    {
        fputs("Unwrap on error: ", stderr);
        fputs(result.error_message, stderr);
        fputc('\n', stderr);
        exit(1);
    }
}

static inline Bool Result_Void_is_err(cref_Result_Void result)
{
    return result->is_error;
}

static inline Bool Result_Void_is_ok(cref_Result_Void result)
{
    return !(result->is_error);
}

static inline cref_Char Result_Void_unwrap_err(Result_Void result)
{
    if (!result.is_error)
    {
        perror("Result_Void is ok");
        exit(1);
    }
    return result.error_message;
}

static inline void Result_Void_free(ref_Result_Void result)
{
    (void)result;
}

#endif
