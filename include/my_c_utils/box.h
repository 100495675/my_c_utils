#ifndef MY_C_UTILS_BOX_H
#define MY_C_UTILS_BOX_H

#include <stdio.h>
#include <stdlib.h>

#include "my_c_utils/allocator.h"
#include "my_c_utils/result.h"
#include "my_c_utils/primitives.h"

#define BOX_CONFIG(Type)                                                                      \
    typedef struct                                                                            \
    {                                                                                         \
        ref_##Type value;                                                                     \
    } Box_##Type;                                                                             \
    REF_EXPAND(Box_##Type)                                                                    \
    static inline void Box_##Type##_free(ref_Box_##Type self)                                 \
    {                                                                                         \
        if (self->value)                                                                      \
        {                                                                                     \
            Type##_free(self->value);                                                         \
            MY_C_UTILS_FREE(self->value);                                                     \
            self->value = NULL;                                                               \
        }                                                                                     \
    }                                                                                         \
                                                                                              \
    RESULT_CONFIG(Box_##Type)                                                                 \
                                                                                              \
    static inline Box_##Type Box_##Type##_new(Type value)                                     \
    {                                                                                         \
        ref_##Type boxed_value = MY_C_UTILS_MALLOC(sizeof(Type));                             \
        if (!boxed_value)                                                                     \
        {                                                                                     \
            perror("Memory allocation failed");                                              \
            exit(1);                                                                          \
        }                                                                                     \
        *boxed_value = value;                                                                 \
        return (Box_##Type){.value = boxed_value};                                            \
    }                                                                                         \
                                                                                              \
    static inline cref_##Type Box_##Type##_deref(cref_Box_##Type self)                        \
    {                                                                                         \
        return self->value;                                                                   \
    }                                                                                         \
                                                                                              \
    static inline ref_##Type Box_##Type##_deref_mut(ref_Box_##Type self)                     \
    {                                                                                         \
        return self->value;                                                                   \
    }                                                                                         \
                                                                                              \
    static inline Type Box_##Type##_into_inner(ref_Box_##Type self)                           \
    {                                                                                         \
        if (!self->value)                                                                     \
        {                                                                                     \
            fputs("Box is empty\n", stderr);                                                \
            exit(1);                                                                          \
        }                                                                                     \
        Type value = *self->value;                                                            \
        MY_C_UTILS_FREE(self->value);                                                         \
        self->value = NULL;                                                                   \
        return value;                                                                         \
    }                                                                                         \
                                                                                              \
    static inline Box_##Type Box_##Type##_clone(cref_Box_##Type src)                          \
    {                                                                                         \
        if (!src)                                                                             \
        {                                                                                     \
            perror("Cannot clone NULL Box pointer");                                        \
            exit(1);                                                                          \
        }                                                                                     \
        return Box_##Type##_new(Type##_clone(src->value));                                    \
    }

#endif