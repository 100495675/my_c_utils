#ifndef MY_C_UTILS_BOX_H
#define MY_C_UTILS_BOX_H

#include <stdio.h>
#include <stdlib.h>

#include "my_c_utils/allocator.h"
#include "my_c_utils/result.h"
#include "my_c_utils/primitives.h"

#define Box(Type) MY_C_UTILS_CONCAT(Box_, Type)
#define ref_Box(Type) MY_C_UTILS_CONCAT(ref_Box_, Type)
#define cref_Box(Type) MY_C_UTILS_CONCAT(cref_Box_, Type)

#define Box_new(Type)          MY_C_UTILS_CONCAT(Box_, MY_C_UTILS_CONCAT(Type, _new))
#define Box_free(Type)         MY_C_UTILS_CONCAT(Box_, MY_C_UTILS_CONCAT(Type, _free))
#define Box_deref(Type)        MY_C_UTILS_CONCAT(Box_, MY_C_UTILS_CONCAT(Type, _deref))
#define Box_deref_mut(Type)    MY_C_UTILS_CONCAT(Box_, MY_C_UTILS_CONCAT(Type, _deref_mut))
#define Box_into_inner(Type)   MY_C_UTILS_CONCAT(Box_, MY_C_UTILS_CONCAT(Type, _into_inner))
#define Box_clone(Type)        MY_C_UTILS_CONCAT(Box_, MY_C_UTILS_CONCAT(Type, _clone))

#define BOX_CONFIG(Type) BOX_CONFIG_IMPL(Type)

#define BOX_CONFIG_IMPL(Type)                                                                      \
    typedef struct                                                                            \
    {                                                                                         \
        ref_##Type value;                                                                     \
    } Box_##Type;                                                                             \
    REF_EXPAND(Box_##Type)                                                                    \
    static inline void Box_free(Type)(ref_Box(Type) self)                                 \
    {                                                                                         \
        if (self->value)                                                                      \
        {                                                                                     \
            Type##_free(self->value);                                                         \
            MY_C_UTILS_FREE(self->value);                                                     \
            self->value = NULL;                                                               \
        }                                                                                     \
    }                                                                                         \
    RESULT_CONFIG(Box_##Type, cref_Char)                                                                 \
                                                                                              \
    static inline Box_##Type Box_new(Type)(Type value)                                     \
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
    static inline cref_##Type Box_deref(Type)(cref_Box(Type) self)                        \
    {                                                                                         \
        return self->value;                                                                   \
    }                                                                                         \
                                                                                              \
    static inline ref_##Type Box_deref_mut(Type)(ref_Box(Type) self)                     \
    {                                                                                         \
        return self->value;                                                                   \
    }                                                                                         \
                                                                                              \
    static inline Type Box_into_inner(Type)(ref_Box(Type) self)                           \
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
    static inline Box_##Type Box_clone(Type)(cref_Box(Type) src)                          \
    {                                                                                         \
        if (!src)                                                                             \
        {                                                                                     \
            perror("Cannot clone NULL Box pointer");                                        \
            exit(1);                                                                          \
        }                                                                                     \
        return Box_new(Type)(Type##_clone(src->value));                                    \
    }

#endif