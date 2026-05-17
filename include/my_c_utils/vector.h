#ifndef MY_C_UTILS_VECTOR_H
#define MY_C_UTILS_VECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_c_utils/result.h"
#include "my_c_utils/iterator.h"

#define VECTOR_CONFIG(Type)                                                      \
    typedef struct                                                               \
    {                                                                            \
        Type *data;                                                              \
        Size size;                                                             \
        Size capacity;                                                         \
    } Vector_##Type;                                                             \
                                                                                 \
    typedef struct                                                               \
    {                                                                            \
        const Vector_##Type *vector;                                             \
        Size index;                                                            \
    } Vector_##Type##_iter;                                                      \
                                                                                 \
    RESULT_CONFIG(Type)                                                          \
    typedef Type *Type##_ref;                                                    \
    static inline void Type##_ref_free(Type##_ref *value)                        \
    {                                                                            \
        (void)value;                                                             \
    }                                                                            \
    RESULT_CONFIG(Type##_ref)                                                    \
                                                                                 \
    static inline Vector_##Type Vector_##Type##_new()                            \
    {                                                                            \
        return (Vector_##Type){.data = NULL, .size = 0, .capacity = 0};          \
    }                                                                            \
                                                                                 \
    static inline Result Vector_##Type##_push_back(Vector_##Type *self,          \
                                                   const Type value)             \
    {                                                                            \
        if (self->size == self->capacity)                                        \
        {                                                                        \
            self->capacity = self->capacity == 0 ? 1 : self->capacity * 2;       \
            Type *new_data = realloc(self->data, self->capacity * sizeof(Type)); \
            if (!new_data)                                                       \
            {                                                                    \
                return Result_err("Memory allocation failed");                   \
            }                                                                    \
            self->data = new_data;                                               \
        }                                                                        \
        self->data[self->size++] = value;                                        \
        return Result_ok();                                                      \
    }                                                                            \
                                                                                 \
    static inline Result_##Type##_ref Vector_##Type##_at(                        \
        const Vector_##Type *self, const Size index)                           \
    {                                                                            \
        if (index >= self->size)                                                 \
        {                                                                        \
            return Result_##Type##_ref_err("Index out of bounds");               \
        }                                                                        \
        return Result_##Type##_ref_ok(&self->data[index]);                       \
    }                                                                            \
                                                                                 \
    static inline Size Vector_##Type##_size(const Vector_##Type *self)         \
    {                                                                            \
        return self->size;                                                       \
    }                                                                            \
                                                                                 \
    static inline Result Vector_##Type##_set(                                    \
        Vector_##Type *self, const Size index, const Type value)               \
    {                                                                            \
        if (index >= self->size)                                                 \
        {                                                                        \
            return Result_err("Index out of bounds");                            \
        }                                                                        \
        Type##_free(&self->data[index]);                                         \
        self->data[index] = value;                                               \
        return Result_ok();                                                      \
    }                                                                            \
                                                                                 \
    static inline void Vector_##Type##_clear(Vector_##Type *self)                \
    {                                                                            \
        for (Size i = 0; i < self->size; ++i)                                  \
        {                                                                        \
            Type##_free(&self->data[i]);                                         \
        }                                                                        \
        self->size = 0;                                                          \
    }                                                                            \
                                                                                 \
    static inline void Vector_##Type##_free(Vector_##Type *self)                 \
    {                                                                            \
        for (Size i = 0; i < self->size; ++i)                                  \
        {                                                                        \
            Type##_free(&self->data[i]);                                         \
        }                                                                        \
        free(self->data);                                                        \
        self->data = NULL;                                                       \
        self->size = 0;                                                          \
        self->capacity = 0;                                                      \
    }                                                                            \
                                                                                 \
    static inline Result Vector_##Type##_reserve(Vector_##Type *self,            \
                                                 const Size capacity)          \
    {                                                                            \
        if (capacity > self->capacity)                                           \
        {                                                                        \
            Type *new_data = realloc(self->data, capacity * sizeof(Type));       \
            if (!new_data)                                                       \
            {                                                                    \
                return Result_err("Memory allocation failed");                   \
            }                                                                    \
            self->data = new_data;                                               \
            self->capacity = capacity;                                           \
        }                                                                        \
        return Result_ok();                                                      \
    }                                                                            \
                                                                                 \
    static inline Result_##Type Vector_##Type##_pop_back(Vector_##Type *self)    \
    {                                                                            \
        if (self->size == 0)                                                     \
        {                                                                        \
            return Result_##Type##_err("Vector is empty");                       \
        }                                                                        \
        return Result_##Type##_ok(self->data[--self->size]);                     \
    }                                                                            \
                                                                                 \
    static inline void Vector_##Type##_debug(const Vector_##Type *self)          \
    {                                                                            \
        printf("Vector: size=%zu, capacity=%zu{\n", self->size, self->capacity); \
        for (Size i = 0; i < self->size; ++i)                                  \
        {                                                                        \
            printf("  [%zu]\n", i);                                              \
        }                                                                        \
        printf("}\n");                                                           \
    }                                                                            \
                                                                                 \
    static inline Vector_##Type##_iter Vector_##Type##_into_iter(                \
        const Vector_##Type *self)                                               \
    {                                                                            \
        return (Vector_##Type##_iter){.vector = self, .index = 0};               \
    }                                                                            \
                                                                                 \
    static inline Result_##Type##_ref Vector_##Type##_iter_deref(                \
        const Vector_##Type##_iter *self)                                        \
    {                                                                            \
        if (self->index >= self->vector->size)                                   \
        {                                                                        \
            return Result_##Type##_ref_err("Iterator out of bounds");            \
        }                                                                        \
        return Result_##Type##_ref_ok(&(self->vector->data[self->index]));       \
    }                                                                            \
                                                                                 \
    static inline Result_##Type##_ref Vector_##Type##_iter_next(Vector_##Type##_iter *self)     \
    {                                                                            \
        if (self->index >= self->vector->size)                                   \
        {                                                                        \
            return Result_##Type##_ref_err("Iterator out of bounds");          \
        }                                                                        \
        return Result_##Type##_ref_ok(&self->vector->data[self->index++]);       \
    }                                                                            \


#endif
