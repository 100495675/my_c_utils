#ifndef MY_C_UTILS_RESULT_H
#define MY_C_UTILS_RESULT_H

#include "my_c_utils/tipos.h"
#include "my_c_utils/template.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Represents a typed monad for fallible operations containing either a success (T) or error (E).
 * @usage Result(T, E)
 */
#define Result(...) TEMPLATE_TYPE(Result, __VA_ARGS__)

/**
 * @brief Mutable borrowed pointer reference to Result(T, E).
 * @usage ref_Result(T, E)
 */
#define ref_Result(...) TEMPLATE_TYPE(ref_Result, __VA_ARGS__)

/**
 * @brief Immutable borrowed pointer reference to Result(T, E).
 * @usage cref_Result(T, E)
 */
#define cref_Result(...) TEMPLATE_TYPE(cref_Result, __VA_ARGS__)

/**
 * @brief Checks if the Result contains a success value (is Ok).
 * @param T Success type.
 * @param E Error type.
 * @param self Pointer to the Result instance (&res).
 * @returns Bool
 * @usage Bool success = Result_is_ok(Int, cref(Char))(&res);
 */
#define Result_is_ok(...)      TEMPLATE_METHOD(Result, is_ok, __VA_ARGS__)

/**
 * @brief Checks if the Result contains a failure error (is Err).
 * @param T Success type.
 * @param E Error type.
 * @param self Pointer to the Result instance (&res).
 * @returns Bool
 * @usage Bool failure = Result_is_err(Int, cref(Char))(&res);
 */
#define Result_is_err(...)     TEMPLATE_METHOD(Result, is_err, __VA_ARGS__)

/**
 * @brief Unwraps a successful Result, returning its T value. Panics and aborts if it is Err.
 * @param T Success type.
 * @param E Error type.
 * @param self The Result instance.
 * @returns T
 * @usage Int value = Result_unwrap(Int, cref(Char))(res);
 */
#define Result_unwrap(...)     TEMPLATE_METHOD(Result, unwrap, __VA_ARGS__)

/**
 * @brief Unwraps a failed Result, returning its error E value. Panics and aborts if it is Ok.
 * @param T Success type.
 * @param E Error type.
 * @param self The Result instance.
 * @returns E
 * @usage cref(Char) err = Result_unwrap_err(Int, cref(Char))(res);
 */
#define Result_unwrap_err(...) TEMPLATE_METHOD(Result, unwrap_err, __VA_ARGS__)

/**
 * @brief Frees the inner success or error value depending on the Result status.
 * @param T Success type.
 * @param E Error type.
 * @param self Pointer to the Result instance (&res).
 * @usage Result_free(Int, cref(Char))(&res);
 */
#define Result_free(...)       TEMPLATE_METHOD(Result, free, __VA_ARGS__)

/**
 * @brief Constructs a successful Result containing a T value.
 * @param T Success type.
 * @param E Error type.
 * @param value The success value of type T to store.
 * @returns Result(T, E)
 * @usage Result(Int, cref(Char)) res = Result_ok(Int, cref(Char))(42);
 */
#define Result_ok(...)         TEMPLATE_METHOD(Result, ok, __VA_ARGS__)

/**
 * @brief Constructs a failed Result containing an error E value.
 * @param T Success type.
 * @param E Error type.
 * @param error The error value of type E to store.
 * @returns Result(T, E)
 * @usage Result(Int, cref(Char)) res = Result_err(Int, cref(Char))("An error occurred");
 */
#define Result_err(...)        TEMPLATE_METHOD(Result, err, __VA_ARGS__)

/**
 * @brief Maps a Result(T, E) to Result(U, E) by applying a function to the success value T.
 * @param T Success type.
 * @param E Error type.
 * @param U Destination success type.
 * @param self The Result(T, E) instance.
 * @param fn Mapping function U (*fn)(T).
 * @returns Result(U, E)
 * @usage Result_map(Int, cref(Char), Double)(res, int_to_double)
 */
#define Result_map(...)      TEMPLATE_METHOD(Result, map, __VA_ARGS__)

/**
 * @brief Applies a function returning Result(U, E) to the success value T of Result(T, E).
 * @param T Success type.
 * @param E Error type.
 * @param U Destination success type.
 * @param self The Result(T, E) instance.
 * @param fn Monadic function Result(U, E) (*fn)(T).
 * @returns Result(U, E)
 * @usage Result_and_then(Int, cref(Char), Double)(res, int_to_res_double)
 */
#define Result_and_then(...) TEMPLATE_METHOD(Result, and_then, __VA_ARGS__)

/**
 * @brief Returns the success value or a default value.
 * @param T Success type.
 * @param E Error type.
 * @param self The Result(T, E) instance.
 * @param default_value Default value to return if it is Err.
 * @returns T
 * @usage Int val = Result_unwrap_or(Int, cref(Char))(res, 0)
 */
#define Result_unwrap_or(...) TEMPLATE_METHOD(Result, unwrap_or, __VA_ARGS__)

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
    } \
    \
    static inline T Result_unwrap_or(T, E)(Result(T, E) self, T default_value) \
    { \
        return !self.is_error ? self.value : default_value; \
    }

#define RESULT_MAP_CONFIG(T, E, U) \
    static inline Result(U, E) TEMPLATE_METHOD(Result, map, T, E, U)(Result(T, E) self, U (*fn)(T)) \
    { \
        if (!self.is_error) { \
            return Result_ok(U, E)(fn(self.value)); \
        } \
        return Result_err(U, E)(self.error); \
    }

#define RESULT_AND_THEN_CONFIG(T, E, U) \
    static inline Result(U, E) TEMPLATE_METHOD(Result, and_then, T, E, U)(Result(T, E) self, Result(U, E) (*fn)(T)) \
    { \
        if (!self.is_error) { \
            return fn(self.value); \
        } \
        return Result_err(U, E)(self.error); \
    }

typedef struct {} Void;
static inline void Void_free(Void *value)
{
    (void)value;
}

RESULT_CONFIG(Void, cref(Char))

#endif