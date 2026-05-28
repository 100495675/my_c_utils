#ifndef MY_C_UTILS_OPTION_H
#define MY_C_UTILS_OPTION_H

#include "my_c_utils/tipos.h"
#include "my_c_utils/template.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// 1. User-Facing Macros (Prefix-free, template-compatible)
#define Option(...) TEMPLATE_TYPE(Option, __VA_ARGS__)
#define ref_Option(...) TEMPLATE_TYPE(ref_Option, __VA_ARGS__)
#define cref_Option(...) TEMPLATE_TYPE(cref_Option, __VA_ARGS__)

#define Option_some(...)    TEMPLATE_METHOD(Option, some, __VA_ARGS__)
#define Option_none(...)    TEMPLATE_METHOD(Option, none, __VA_ARGS__)
#define Option_unwrap(...)  TEMPLATE_METHOD(Option, unwrap, __VA_ARGS__)
#define Option_is_none(...) TEMPLATE_METHOD(Option, is_none, __VA_ARGS__)
#define Option_is_some(...) TEMPLATE_METHOD(Option, is_some, __VA_ARGS__)
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
