#ifndef MY_C_UTILS_VECTOR_H
#define MY_C_UTILS_VECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_c_utils/allocator.h"
#include "my_c_utils/result.h"
#include "my_c_utils/iterator.h"
#include "my_c_utils/primitives.h"

#define VECTOR_CONFIG(Type)                                                                \
    typedef struct                                                                         \
    {                                                                                      \
        ref_##Type data;                                                                   \
        Size size;                                                                         \
        Size capacity;                                                                     \
    } Vector_##Type;                                                                       \
    REF_EXPAND(Vector_##Type)                                                              \
                                                                                           \
    typedef struct                                                                         \
    {                                                                                      \
        cref_Vector_##Type vector;                                                         \
        Size index;                                                                        \
    } iter_Vector_##Type;                                                                  \
    REF_EXPAND(iter_Vector_##Type)                                                         \
    TRIVIAL_FREE(iter_Vector_##Type, cref_iter_Vector_##Type value)                          \
                                                                                           \
                                                                                           \
    static inline Vector_##Type Vector_##Type##_new()                                      \
    {                                                                                      \
        return (Vector_##Type){.data = NULL, .size = 0, .capacity = 0};                    \
    }                                                                                      \
                                                                                           \
    static inline Result_Void Vector_##Type##_push_back(ref_Vector_##Type self,                 \
                                                   const Type value)                       \
    {                                                                                      \
        if (self->size == self->capacity)                                                  \
        {                                                                                  \
            self->capacity = self->capacity == 0 ? 1 : self->capacity * 2;                 \
            ref_##Type new_data = MY_C_UTILS_REALLOC(self->data, self->capacity * sizeof(Type)); \
            if (!new_data)                                                                 \
            {                                                                              \
                return Result_Void_err("Memory allocation failed");                             \
            }                                                                              \
            self->data = new_data;                                                         \
        }                                                                                  \
        self->data[self->size++] = value;                                                  \
        return Result_Void_ok();                                                                \
    }                                                                                      \
                                                                                           \
    static inline Result_Void_ref_##Type Vector_##Type##_at(                                    \
        cref_Vector_##Type self, const Size index)                                         \
    {                                                                                      \
        if (index >= self->size)                                                           \
        {                                                                                  \
            return Result_Void_ref_##Type##_err("Index out of bounds");                         \
        }                                                                                  \
        return Result_Void_ref_##Type##_ok(&self->data[index]);                                 \
    }                                                                                      \
                                                                                           \
    static inline Size Vector_##Type##_size(cref_Vector_##Type self)                       \
    {                                                                                      \
        return self->size;                                                                 \
    }                                                                                      \
                                                                                           \
    static inline Result_Void Vector_##Type##_set(                                              \
        ref_Vector_##Type self, const Size index, const Type value)                        \
    {                                                                                      \
        if (index >= self->size)                                                           \
        {                                                                                  \
            return Result_Void_err("Index out of bounds");                                      \
        }                                                                                  \
        Type##_free(&self->data[index]);                                                   \
        self->data[index] = value;                                                         \
        return Result_Void_ok();                                                                \
    }                                                                                      \
                                                                                           \
    static inline void Vector_##Type##_clear(ref_Vector_##Type self)                       \
    {                                                                                      \
        for (Size i = 0; i < self->size; ++i)                                              \
        {                                                                                  \
            Type##_free(&self->data[i]);                                                   \
        }                                                                                      \
        self->size = 0;                                                                    \
    }                                                                                      \
                                                                                           \
    static inline void Vector_##Type##_free(ref_Vector_##Type self)                        \
    {                                                                                      \
        for (Size i = 0; i < self->size; ++i)                                              \
        {                                                                                  \
            Type##_free(&self->data[i]);                                                   \
        }                                                                                  \
        MY_C_UTILS_FREE(self->data);                                                       \
        self->data = NULL;                                                                 \
        self->size = 0;                                                                    \
        self->capacity = 0;                                                                \
    }                                                                                      \
                                                                                           \
    static inline Result_Void Vector_##Type##_reserve(ref_Vector_##Type self,                   \
                                                 const Size capacity)                      \
    {                                                                                      \
        if (capacity > self->capacity)                                                     \
        {                                                                                  \
            ref_##Type new_data = MY_C_UTILS_REALLOC(self->data, capacity * sizeof(Type));  \
            if (!new_data)                                                                 \
            {                                                                              \
                return Result_Void_err("Memory allocation failed");                             \
            }                                                                              \
            self->data = new_data;                                                         \
            self->capacity = capacity;                                                     \
        }                                                                                  \
        return Result_Void_ok();                                                                \
    }                                                                                      \
                                                                                           \
    static inline Result_Void_##Type Vector_##Type##_pop_back(ref_Vector_##Type self)           \
    {                                                                                      \
        if (self->size == 0)                                                               \
        {                                                                                  \
            return Result_Void_##Type##_err("Vector is empty");                                 \
        }                                                                                  \
        return Result_Void_##Type##_ok(self->data[--self->size]);                               \
    }                                                                                      \
                                                                                           \
    static inline Result_Void Vector_##Type##_insert_at(ref_Vector_##Type self,                 \
                                                   Size index,                             \
                                                   Type value)                             \
    {                                                                                      \
        if (index > self->size)                                                            \
        {                                                                                  \
            return Result_Void_err("Index out of bounds");                                      \
        }                                                                                  \
        if (self->size == self->capacity)                                                  \
        {                                                                                  \
            self->capacity = self->capacity == 0 ? 1 : self->capacity * 2;                 \
            ref_##Type new_data = MY_C_UTILS_REALLOC(self->data, self->capacity * sizeof(Type));\
            if (!new_data)                                                                 \
            {                                                                              \
                return Result_Void_err("Memory allocation failed");                             \
            }                                                                              \
            self->data = new_data;                                                         \
        }                                                                                  \
        for (Size i = self->size; i > index; --i)                                          \
        {                                                                                  \
            self->data[i] = self->data[i - 1];                                             \
        }                                                                                  \
        self->data[index] = value;                                                         \
        self->size++;                                                                      \
        return Result_Void_ok();                                                                \
    }                                                                                      \
                                                                                           \
    static inline Result_Void Vector_##Type##_remove_at(ref_Vector_##Type self, Size index)     \
    {                                                                                      \
        if (index >= self->size)                                                           \
        {                                                                                  \
            return Result_Void_err("Index out of bounds");                                      \
        }                                                                                  \
        Type##_free(&self->data[index]);                                                   \
        for (Size i = index; i < self->size - 1; ++i)                                      \
        {                                                                                  \
            self->data[i] = self->data[i + 1];                                             \
        }                                                                                  \
        self->size--;                                                                      \
        return Result_Void_ok();                                                                \
    }                                                                                      \
                                                                                           \
    static inline Result_Void Vector_##Type##_shrink_to_fit(ref_Vector_##Type self)             \
    {                                                                                      \
        if (self->capacity > self->size)                                                   \
        {                                                                                  \
            if (self->size == 0)                                                           \
            {                                                                              \
                MY_C_UTILS_FREE(self->data);                                               \
                self->data = NULL;                                                         \
                self->capacity = 0;                                                        \
            }                                                                                  \
            else                                                                           \
            {                                                                              \
                ref_##Type new_data = MY_C_UTILS_REALLOC(self->data, self->size * sizeof(Type));\
                if (!new_data)                                                             \
                {                                                                          \
                    return Result_Void_err("Memory allocation failed");                         \
                }                                                                          \
                self->data = new_data;                                                     \
                self->capacity = self->size;                                               \
            }                                                                              \
        }                                                                                  \
        return Result_Void_ok();                                                                \
    }                                                                                      \
                                                                                           \
    static inline void Vector_##Type##_sort(ref_Vector_##Type self,                        \
                                            int (*compare)(const void *, const void *))    \
    {                                                                                      \
        if (self->data && self->size > 1)                                                  \
        {                                                                                  \
            qsort(self->data, self->size, sizeof(Type), compare);                          \
        }                                                                                  \
    }                                                                                      \
                                                                                           \
    static inline void Vector_##Type##_debug(cref_Vector_##Type self)                      \
    {                                                                                      \
        printf("Vector: size=%zu, capacity=%zu{\n", self->size, self->capacity);           \
        for (Size i = 0; i < self->size; ++i)                                              \
        {                                                                                  \
            printf("  [%zu]\n", i);                                                        \
        }                                                                                  \
        printf("}\n");                                                                     \
    }                                                                                      \
                                                                                           \
    static inline iter_Vector_##Type Vector_##Type##_into_iter(                            \
        cref_Vector_##Type self)                                                           \
    {                                                                                      \
        return (iter_Vector_##Type){.vector = self, .index = 0};                           \
    }                                                                                      \
                                                                                           \
    static inline Result_Void_ref_##Type iter_Vector_##Type##_deref(                            \
        cref_iter_Vector_##Type self)                                                      \
    {                                                                                      \
        if (self->index >= self->vector->size)                                             \
        {                                                                                  \
            return Result_Void_ref_##Type##_err("Iterator out of bounds");                      \
        }                                                                                  \
        return Result_Void_ref_##Type##_ok(&(self->vector->data[self->index]));                 \
    }                                                                                      \
                                                                                           \
    static inline Result_Void_ref_##Type iter_Vector_##Type##_next(ref_iter_Vector_##Type self) \
    {                                                                                      \
        if (self->index >= self->vector->size)                                             \
        {                                                                                  \
            return Result_Void_ref_##Type##_err("Iterator out of bounds");                      \
        }                                                                                  \
        return Result_Void_ref_##Type##_ok(&self->vector->data[self->index++]);                 \
    }                                                                                      \
                                                                                           \
    RESULT_CONFIG(Vector_##Type)                                                           \
    RESULT_CONFIG(ref_Vector_##Type)                                                       \
    static inline Vector_##Type Vector_##Type##_clone(cref_Vector_##Type src)              \
    {                                                                                      \
        if (!src)                                                                          \
        {                                                                                  \
            perror("Cannot clone NULL Vector pointer");                                    \
            exit(1);                                                                       \
        }                                                                                  \
        Vector_##Type dest = Vector_##Type##_new();                                        \
        for (Size i = 0; i < src->size; ++i)                                               \
        {                                                                                  \
            Type cloned_value = Type##_clone(&src->data[i]);                               \
            Result_Void push_res = Vector_##Type##_push_back(&dest, cloned_value);              \
            if (Result_Void_is_err(&push_res))                                                  \
            {                                                                              \
                Vector_##Type##_free(&dest);                                               \
                perror("Memory allocation failed during vector clone");                    \
                exit(1);                                                                   \
            }                                                                              \
        }                                                                                  \
        return dest;                                                                       \
    }
#endif
