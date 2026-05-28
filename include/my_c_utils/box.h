#ifndef MY_C_UTILS_BOX_H
#define MY_C_UTILS_BOX_H

#include <stdio.h>
#include <stdlib.h>

#include "my_c_utils/allocator.h"
#include "my_c_utils/result.h"
#include "my_c_utils/primitives.h"
#include "my_c_utils/template.h"

// 1. User-Facing Macros (Prefix-free, fully template-compatible)
#define Box(...) TEMPLATE_TYPE(Box, __VA_ARGS__)
#define ref_Box(...) TEMPLATE_TYPE(ref_Box, __VA_ARGS__)
#define cref_Box(...) TEMPLATE_TYPE(cref_Box, __VA_ARGS__)

#define Box_new(...) TEMPLATE_METHOD(Box, new, __VA_ARGS__)
#define Box_free(...) TEMPLATE_METHOD(Box, free, __VA_ARGS__)
#define Box_deref(...) TEMPLATE_METHOD(Box, deref, __VA_ARGS__)
#define Box_deref_mut(...) TEMPLATE_METHOD(Box, deref_mut, __VA_ARGS__)
#define Box_into_inner(...) TEMPLATE_METHOD(Box, into_inner, __VA_ARGS__)
#define Box_clone(...) TEMPLATE_METHOD(Box, clone, __VA_ARGS__)

// Backward compatibility alias for the manual BOX_CONFIG
#define BOX_CONFIG(T) TEMPLATE_Box(T)

// 2. Template Definition Macro
#define TEMPLATE_Box(T) \
    typedef struct { \
        ref_##T value; \
    } Box(T); \
    typedef Box(T) *ref_Box(T); \
    typedef const Box(T) *cref_Box(T); \
    static inline void TEMPLATE_METHOD(ref_Box, free, T)(ref_Box(T) *value) { (void)value; } \
    static inline void TEMPLATE_METHOD(cref_Box, free, T)(cref_Box(T) *value) { (void)value; } \
    static inline void Box_free(T)(ref_Box(T) self) { \
        if (self->value) { \
            T##_free(self->value); \
            MY_C_UTILS_FREE(self->value); \
            self->value = NULL; \
        } \
    } \
    RESULT_CONFIG(Box(T), cref_Char) \
    static inline Box(T) Box_new(T)(T value) { \
        ref_##T boxed_value = MY_C_UTILS_MALLOC(sizeof(T)); \
        if (!boxed_value) { \
            perror("Memory allocation failed"); \
            exit(1); \
        } \
        *boxed_value = value; \
        return (Box(T)){.value = boxed_value}; \
    } \
    static inline cref_##T Box_deref(T)(cref_Box(T) self) { \
        return self->value; \
    } \
    static inline ref_##T Box_deref_mut(T)(ref_Box(T) self) { \
        return self->value; \
    } \
    static inline T Box_into_inner(T)(ref_Box(T) self) { \
        if (!self->value) { \
            fputs("Box is empty\n", stderr); \
            exit(1); \
        } \
        T value = *self->value; \
        MY_C_UTILS_FREE(self->value); \
        self->value = NULL; \
        return value; \
    } \
    static inline Box(T) Box_clone(T)(cref_Box(T) src) { \
        if (!src) { \
            perror("Cannot clone NULL Box pointer"); \
            exit(1); \
        } \
        return Box_new(T)(T##_clone(src->value)); \
    }

#endif