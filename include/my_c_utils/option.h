#ifndef MY_C_UTILS_OPTION_H
#define MY_C_UTILS_OPTION_H

#include "my_c_utils/tipos.h"
#include "my_c_utils/template.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Represents an optional value that may or may not be filled/present.
 * @usage Option(T)
 */
#define Option(...) TEMPLATE_TYPE(Option, __VA_ARGS__)

/**
 * @brief Mutable borrowed pointer reference to Option(T).
 * @usage ref_Option(T)
 */
#define ref_Option(...) TEMPLATE_TYPE(ref_Option, __VA_ARGS__)

/**
 * @brief Immutable borrowed pointer reference to Option(T).
 * @usage cref_Option(T)
 */
#define cref_Option(...) TEMPLATE_TYPE(cref_Option, __VA_ARGS__)

/**
 * @brief Wraps a value of type T inside a filled Option(T).
 * @param T The type of the value.
 * @param value The value to store.
 * @returns Option(T)
 * @usage Option(Int) opt = Option_some(Int)(42);
 */
#define Option_some(...)    TEMPLATE_METHOD(Option, some, __VA_ARGS__)

/**
 * @brief Constructs an empty/none Option(T).
 * @param T The type of the absent value.
 * @returns Option(T)
 * @usage Option(Int) opt = Option_none(Int)();
 */
#define Option_none(...)    TEMPLATE_METHOD(Option, none, __VA_ARGS__)

/**
 * @brief Unwraps a filled Option(T), returning its inner T value. Panics and aborts if empty.
 * @param T The type of the value.
 * @param self The Option(T) instance.
 * @returns T
 * @usage Int value = Option_unwrap(Int)(opt);
 */
#define Option_unwrap(...)  TEMPLATE_METHOD(Option, unwrap, __VA_ARGS__)

/**
 * @brief Checks if the Option(T) is empty (None).
 * @param T The type of the value.
 * @param self A pointer to the Option(T) instance (&opt).
 * @returns Bool
 * @usage Bool empty = Option_is_none(Int)(&opt);
 */
#define Option_is_none(...) TEMPLATE_METHOD(Option, is_none, __VA_ARGS__)

/**
 * @brief Checks if the Option(T) is filled (Some).
 * @param T The type of the value.
 * @param self A pointer to the Option(T) instance (&opt).
 * @returns Bool
 * @usage Bool filled = Option_is_some(Int)(&opt);
 */
#define Option_is_some(...) TEMPLATE_METHOD(Option, is_some, __VA_ARGS__)

/**
 * @brief Frees the inner value stored in the Option if it is filled.
 * @param T The type of the value.
 * @param self A pointer to the Option(T) instance (&opt).
 * @usage Option_free(Int)(&opt);
 */
#define Option_free(...)    TEMPLATE_METHOD(Option, free, __VA_ARGS__)

// Backward compatibility alias for the manual OPTION_CONFIG
#define OPTION_CONFIG(T) TEMPLATE_Option(T)

// 2. Template Definition Macro
#define TEMPLATE_Option(T) \
    typedef struct \
    { \
        T value; \
        Bool filled; \
    } Option(T); \
    typedef Option(T) *ref_Option(T); \
    typedef const Option(T) *cref_Option(T); \
    static inline void TEMPLATE_METHOD(ref_Option, free, T)(ref_Option(T) *value) { (void)value; } \
    static inline void TEMPLATE_METHOD(cref_Option, free, T)(cref_Option(T) *value) { (void)value; } \
    \
    static inline Option(T) Option_some(T)(T value) \
    { \
        return (Option(T)){.value = value, .filled = true}; \
    } \
    \
    static inline Option(T) Option_none(T)() \
    { \
        return (Option(T)){.filled = false}; \
    } \
    \
    static inline T Option_unwrap(T)(Option(T) self) \
    { \
        if (!self.filled) \
        { \
            perror("Option is none"); \
            exit(1); \
        } \
        return self.value; \
    } \
    \
    static inline Bool Option_is_none(T)(cref_Option(T) self) \
    { \
        return !(self->filled); \
    } \
    \
    static inline Bool Option_is_some(T)(cref_Option(T) self) \
    { \
        return self->filled; \
    } \
    \
    static inline void Option_free(T)(ref_Option(T) self) \
    { \
        if (self->filled) \
        { \
            MY_C_UTILS_CONCAT(T, _free)(&self->value); \
        } \
    }

#endif
