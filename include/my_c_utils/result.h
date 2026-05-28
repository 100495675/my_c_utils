#ifndef MY_C_UTILS_RESULT_H
#define MY_C_UTILS_RESULT_H

#include "my_c_utils/tipos.h"
#include "my_c_utils/template.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// 1. User-Facing Macros (Prefix-free, template-compatible)
#define Result(...) TEMPLATE_TYPE(Result, __VA_ARGS__)
#define ref_Result(...) TEMPLATE_TYPE(ref_Result, __VA_ARGS__)
#define cref_Result(...) TEMPLATE_TYPE(cref_Result, __VA_ARGS__)

#define Result_is_ok(...)      TEMPLATE_METHOD(Result, is_ok, __VA_ARGS__)
#define Result_is_err(...)     TEMPLATE_METHOD(Result, is_err, __VA_ARGS__)
#define Result_unwrap(...)     TEMPLATE_METHOD(Result, unwrap, __VA_ARGS__)
#define Result_unwrap_err(...) TEMPLATE_METHOD(Result, unwrap_err, __VA_ARGS__)
#define Result_free(...)       TEMPLATE_METHOD(Result, free, __VA_ARGS__)
#define Result_ok(...)         TEMPLATE_METHOD(Result, ok, __VA_ARGS__)
#define Result_err(...)        TEMPLATE_METHOD(Result, err, __VA_ARGS__)

// Backward compatibility alias for the manual RESULT_CONFIG
#define RESULT_CONFIG(T, E) TEMPLATE_Result(T, E)

// 2. Template Definition Macro
#define TEMPLATE_Result(T, E) \
    typedef struct \
    { \
        union \
        { \
            T value; \
            E error; \
        }; \
        Bool is_error; \
    } Result(T, E); \
    typedef Result(T, E) *ref_Result(T, E); \
    typedef const Result(T, E) *cref_Result(T, E); \
    static inline void TEMPLATE_METHOD(ref_Result, free, T, E)(ref_Result(T, E) *value) { (void)value; } \
    static inline void TEMPLATE_METHOD(cref_Result, free, T, E)(cref_Result(T, E) *value) { (void)value; } \
    \
    static inline Result(T, E) Result_ok(T, E)(T value) \
    { \
        return (Result(T, E)){.value = value, .is_error = false}; \
    } \
    \
    static inline Result(T, E) Result_err(T, E)(E error) \
    { \
        return (Result(T, E)){.error = error, .is_error = true}; \
    } \
    \
    static inline T Result_unwrap(T, E)(Result(T, E) self) \
    { \
        if (self.is_error) \
        { \
            fputs("Unwrap on error\n", stderr); \
            exit(1); \
        } \
        return self.value; \
    } \
    \
    static inline Bool Result_is_err(T, E)(cref_Result(T, E) self) \
    { \
        return self->is_error; \
    } \
    \
    static inline Bool Result_is_ok(T, E)(cref_Result(T, E) self) \
    { \
        return !(self->is_error); \
    } \
    \
    static inline E Result_unwrap_err(T, E)(Result(T, E) self) \
    { \
        if (!self.is_error) \
        { \
            perror("Result is ok"); \
            exit(1); \
        } \
        return self.error; \
    } \
    \
    static inline void Result_free(T, E)(ref_Result(T, E) self) \
    { \
        if (!self->is_error) \
        { \
            MY_C_UTILS_CONCAT(T, _free)(&self->value); \
        } \
    }

typedef struct {} Void;
static inline void Void_free(Void *value)
{
    (void)value;
}

RESULT_CONFIG(Void, cref_Char)

#endif