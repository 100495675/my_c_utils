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
            const ref_Char error_message;                                         \
        };                                                                        \
        Bool is_error;                                                            \
    } Result_##Type;                                                              \
    REF_EXPAND(Result_##Type)                                                     \
                                                                                  \
    static inline Result_##Type Result_##Type##_ok(Type value)                    \
    {                                                                             \
        return (Result_##Type){.value = value, .is_error = false};                \
    }                                                                             \
                                                                                  \
    static inline Result_##Type Result_##Type##_err(const ref_Char error_message) \
    {                                                                             \
        return (Result_##Type){.error_message = error_message, .is_error = true}; \
    }                                                                             \
                                                                                  \
    static inline Type Result_##Type##_unwrap(Result_##Type result)               \
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
    static inline Bool Result_##Type##_is_err(Result_##Type result)               \
    {                                                                             \
        return result.is_error;                                                   \
    }                                                                             \
                                                                                  \
    static inline Bool Result_##Type##_is_ok(Result_##Type result)                \
    {                                                                             \
        return !result.is_error;                                                  \
    }                                                                             \
                                                                                  \
    static inline const ref_Char Result_##Type##_unwrap_err(Result_##Type result) \
    {                                                                             \
        if (!result.is_error)                                                     \
        {                                                                         \
            perror("Result is ok");                                               \
            exit(1);                                                              \
        }                                                                         \
        return result.error_message;                                              \
    }                                                                             \
                                                                                  \
    static inline void Result_##Type##_free(ref_Result_##Type result)             \
    {                                                                             \
        if (!result->is_error)                                                    \
        {                                                                         \
            Type##_free(&result->value);                                          \
        }                                                                         \
    }

typedef const ref_Char Result;
REF_EXPAND(Result)

static inline Result Result_ok() { return NULL; }
static inline Result Result_err(const ref_Char error_message) { return error_message; }

static inline void Result_unwrap(Result result)
{
    if (result)
    {
        fputs("Unwrap on error: ", stderr);
        fputs(result, stderr);
        fputc('\n', stderr);
        exit(1);
    }
}

static inline Bool Result_is_err(Result result) { return result; }
static inline Bool Result_is_ok(Result result) { return !result; }

static inline const ref_Char Result_unwrap_err(Result result)
{
    if (!result)
    {
        perror("Result is ok");
        exit(1);
    }
    return result;
}

static inline void Result_free(ref_Result result)
{
    (void)result;
}

#endif
