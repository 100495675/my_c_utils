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

#define Vector(Type) MY_C_UTILS_CONCAT(Vector_, Type)
#define ref_Vector(Type) MY_C_UTILS_CONCAT(ref_Vector_, Type)
#define cref_Vector(Type) MY_C_UTILS_CONCAT(cref_Vector_, Type)

#define iter_Vector(Type) MY_C_UTILS_CONCAT(iter_Vector_, Type)
#define ref_iter_Vector(Type) MY_C_UTILS_CONCAT(ref_iter_Vector_, Type)
#define cref_iter_Vector(Type) MY_C_UTILS_CONCAT(cref_iter_Vector_, Type)

#define Vector_new(Type)          MY_C_UTILS_CONCAT(Vector_, MY_C_UTILS_CONCAT(Type, _new))
#define Vector_push_back(Type)    MY_C_UTILS_CONCAT(Vector_, MY_C_UTILS_CONCAT(Type, _push_back))
#define Vector_at(Type)           MY_C_UTILS_CONCAT(Vector_, MY_C_UTILS_CONCAT(Type, _at))
#define Vector_size(Type)         MY_C_UTILS_CONCAT(Vector_, MY_C_UTILS_CONCAT(Type, _size))
#define Vector_set(Type)          MY_C_UTILS_CONCAT(Vector_, MY_C_UTILS_CONCAT(Type, _set))
#define Vector_clear(Type)        MY_C_UTILS_CONCAT(Vector_, MY_C_UTILS_CONCAT(Type, _clear))
#define Vector_free(Type)         MY_C_UTILS_CONCAT(Vector_, MY_C_UTILS_CONCAT(Type, _free))
#define Vector_reserve(Type)      MY_C_UTILS_CONCAT(Vector_, MY_C_UTILS_CONCAT(Type, _reserve))
#define Vector_pop_back(Type)     MY_C_UTILS_CONCAT(Vector_, MY_C_UTILS_CONCAT(Type, _pop_back))
#define Vector_insert_at(Type)    MY_C_UTILS_CONCAT(Vector_, MY_C_UTILS_CONCAT(Type, _insert_at))
#define Vector_remove_at(Type)    MY_C_UTILS_CONCAT(Vector_, MY_C_UTILS_CONCAT(Type, _remove_at))
#define Vector_shrink_to_fit(Type) MY_C_UTILS_CONCAT(Vector_, MY_C_UTILS_CONCAT(Type, _shrink_to_fit))
#define Vector_sort(Type)         MY_C_UTILS_CONCAT(Vector_, MY_C_UTILS_CONCAT(Type, _sort))
#define Vector_debug(Type)        MY_C_UTILS_CONCAT(Vector_, MY_C_UTILS_CONCAT(Type, _debug))
#define Vector_into_iter(Type)    MY_C_UTILS_CONCAT(Vector_, MY_C_UTILS_CONCAT(Type, _into_iter))
#define Vector_clone(Type)        MY_C_UTILS_CONCAT(Vector_, MY_C_UTILS_CONCAT(Type, _clone))

#define iter_Vector_deref(Type)   MY_C_UTILS_CONCAT(iter_Vector_, MY_C_UTILS_CONCAT(Type, _deref))
#define iter_Vector_next(Type)    MY_C_UTILS_CONCAT(iter_Vector_, MY_C_UTILS_CONCAT(Type, _next))
#define iter_Vector_free(Type)    MY_C_UTILS_CONCAT(iter_Vector_, MY_C_UTILS_CONCAT(Type, _free))

#define VECTOR_CONFIG(Type) VECTOR_CONFIG_IMPL(Type)

#define VECTOR_CONFIG_IMPL(Type)                                                                \
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
    TRIVIAL_FREE(iter_Vector_##Type, cref_iter_Vector_##Type value)                        \
                                                                                           \
                                                                                           \
    static inline Vector_##Type Vector_new(Type)()                                      \
    {                                                                                      \
        return (Vector_##Type){.data = NULL, .size = 0, .capacity = 0};                    \
    }                                                                                      \
                                                                                           \
    static inline Result(Void, cref_Char) Vector_push_back(Type)(ref_Vector(Type) self,                 \
                                                   const Type value)                       \
    {                                                                                      \
        if (self->size == self->capacity)                                                  \
        {                                                                                  \
            self->capacity = self->capacity == 0 ? 1 : self->capacity * 2;                 \
            ref_##Type new_data = MY_C_UTILS_REALLOC(self->data, self->capacity * sizeof(Type)); \
            if (!new_data)                                                                 \
            {                                                                              \
                return Result_err(Void, cref_Char)("Memory allocation failed");                             \
            }                                                                              \
            self->data = new_data;                                                         \
        }                                                                                  \
        self->data[self->size++] = value;                                                  \
        return Result_ok(Void, cref_Char)((Void){});                                                                \
    }                                                                                      \
                                                                                           \
    static inline Result(ref_##Type, cref_Char) Vector_at(Type)(                                    \
        cref_Vector(Type) self, const Size index)                                         \
    {                                                                                      \
        if (index >= self->size)                                                           \
        {                                                                                  \
            return Result_err(ref_##Type, cref_Char)("Index out of bounds");                         \
        }                                                                                  \
        return Result_ok(ref_##Type, cref_Char)(&self->data[index]);                                 \
    }                                                                                      \
                                                                                           \
    static inline Size Vector_size(Type)(cref_Vector(Type) self)                       \
    {                                                                                      \
        return self->size;                                                                 \
    }                                                                                      \
                                                                                           \
    static inline Result(Void, cref_Char) Vector_set(Type)(                                              \
        ref_Vector(Type) self, const Size index, const Type value)                        \
    {                                                                                      \
        if (index >= self->size)                                                           \
        {                                                                                  \
            return Result_err(Void, cref_Char)("Index out of bounds");                                      \
        }                                                                                  \
        Type##_free(&self->data[index]);                                                   \
        self->data[index] = value;                                                         \
        return Result_ok(Void, cref_Char)((Void){});                                                                \
    }                                                                                      \
                                                                                           \
    static inline void Vector_clear(Type)(ref_Vector(Type) self)                       \
    {                                                                                      \
        for (Size i = 0; i < self->size; ++i)                                              \
        {                                                                                  \
            Type##_free(&self->data[i]);                                                   \
        }                                                                                      \
        self->size = 0;                                                                    \
    }                                                                                      \
                                                                                           \
    static inline void Vector_free(Type)(ref_Vector(Type) self)                        \
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
    static inline Result(Void, cref_Char) Vector_reserve(Type)(ref_Vector(Type) self,                   \
                                                 const Size capacity)                      \
    {                                                                                      \
        if (capacity > self->capacity)                                                     \
        {                                                                                  \
            ref_##Type new_data = MY_C_UTILS_REALLOC(self->data, capacity * sizeof(Type));  \
            if (!new_data)                                                                 \
            {                                                                              \
                return Result_err(Void, cref_Char)("Memory allocation failed");                             \
            }                                                                              \
            self->data = new_data;                                                         \
            self->capacity = capacity;                                                     \
        }                                                                                  \
        return Result_ok(Void, cref_Char)((Void){});                                                                \
    }                                                                                      \
                                                                                           \
    static inline Result(Type, cref_Char) Vector_pop_back(Type)(ref_Vector(Type) self)           \
    {                                                                                      \
        if (self->size == 0)                                                               \
        {                                                                                  \
            return Result_err(Type, cref_Char)("Vector is empty");                                 \
        }                                                                                  \
        return Result_ok(Type, cref_Char)(self->data[--self->size]);                               \
    }                                                                                      \
                                                                                           \
    static inline Result(Void, cref_Char) Vector_insert_at(Type)(ref_Vector(Type) self,                 \
                                                   Size index,                             \
                                                    Type value)                             \
    {                                                                                      \
        if (index > self->size)                                                            \
        {                                                                                  \
            return Result_err(Void, cref_Char)("Index out of bounds");                                      \
        }                                                                                  \
        if (self->size == self->capacity)                                                  \
        {                                                                                  \
            self->capacity = self->capacity == 0 ? 1 : self->capacity * 2;                 \
            ref_##Type new_data = MY_C_UTILS_REALLOC(self->data, self->capacity * sizeof(Type));\
            if (!new_data)                                                                 \
            {                                                                              \
                return Result_err(Void, cref_Char)("Memory allocation failed");                             \
            }                                                                              \
            self->data = new_data;                                                         \
        }                                                                                  \
        for (Size i = self->size; i > index; --i)                                          \
        {                                                                                  \
            self->data[i] = self->data[i - 1];                                             \
        }                                                                                  \
        self->data[index] = value;                                                         \
        self->size++;                                                                      \
        return Result_ok(Void, cref_Char)((Void){});                                                                \
    }                                                                                      \
                                                                                           \
    static inline Result(Void, cref_Char) Vector_remove_at(Type)(ref_Vector(Type) self, Size index)     \
    {                                                                                      \
        if (index >= self->size)                                                           \
        {                                                                                  \
            return Result_err(Void, cref_Char)("Index out of bounds");                                      \
        }                                                                                  \
        Type##_free(&self->data[index]);                                                   \
        for (Size i = index; i < self->size - 1; ++i)                                      \
        {                                                                                  \
            self->data[i] = self->data[i + 1];                                             \
        }                                                                                  \
        self->size--;                                                                      \
        return Result_ok(Void, cref_Char)((Void){});                                                                \
    }                                                                                      \
                                                                                           \
    static inline Result(Void, cref_Char) Vector_shrink_to_fit(Type)(ref_Vector(Type) self)             \
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
                    return Result_err(Void, cref_Char)("Memory allocation failed");                         \
                }                                                                          \
                self->data = new_data;                                                     \
                self->capacity = self->size;                                               \
            }                                                                              \
        }                                                                                  \
        return Result_ok(Void, cref_Char)((Void){});                                                                \
    }                                                                                      \
                                                                                           \
    static inline void Vector_sort(Type)(ref_Vector(Type) self,                        \
                                            int (*compare)(const void *, const void *))    \
    {                                                                                      \
        if (self->data && self->size > 1)                                                  \
        {                                                                                  \
            qsort(self->data, self->size, sizeof(Type), compare);                          \
        }                                                                                  \
    }                                                                                      \
                                                                                           \
    static inline void Vector_debug(Type)(cref_Vector(Type) self)                      \
    {                                                                                      \
        printf("Vector: size=%zu, capacity=%zu{\n", self->size, self->capacity);           \
        for (Size i = 0; i < self->size; ++i)                                              \
        {                                                                                  \
            printf("  [%zu]\n", i);                                                        \
        }                                                                                  \
        printf("}\n");                                                                     \
    }                                                                                      \
                                                                                           \
    static inline iter_Vector(Type) Vector_into_iter(Type)(                            \
        cref_Vector(Type) self)                                                           \
    {                                                                                      \
        return (iter_Vector(Type)){.vector = self, .index = 0};                           \
    }                                                                                      \
                                                                                           \
    static inline Result(ref_##Type, cref_Char) iter_Vector_deref(Type)(                            \
        cref_iter_Vector(Type) self)                                                      \
    {                                                                                      \
        if (self->index >= self->vector->size)                                             \
        {                                                                                  \
            return Result_err(ref_##Type, cref_Char)("Iterator out of bounds");                      \
        }                                                                                  \
        return Result_ok(ref_##Type, cref_Char)(&(self->vector->data[self->index]));                 \
    }                                                                                      \
                                                                                           \
    static inline Result(ref_##Type, cref_Char) iter_Vector_next(Type)(ref_iter_Vector(Type) self) \
    {                                                                                      \
        if (self->index >= self->vector->size)                                             \
        {                                                                                  \
            return Result_err(ref_##Type, cref_Char)("Iterator out of bounds");                      \
        }                                                                                  \
        return Result_ok(ref_##Type, cref_Char)(&self->vector->data[self->index++]);                 \
    }                                                                                      \
    RESULT_CONFIG(Vector_##Type, cref_Char)                                                           \
    RESULT_CONFIG(ref_Vector_##Type, cref_Char)                                                       \
    static inline Vector_##Type Vector_clone(Type)(cref_Vector(Type) src)              \
    {                                                                                      \
        if (!src)                                                                          \
        {                                                                                  \
            perror("Cannot clone NULL Vector pointer");                                    \
            exit(1);                                                                       \
        }                                                                                  \
        Vector(Type) dest = Vector_new(Type)();                                        \
        for (Size i = 0; i < src->size; ++i)                                               \
        {                                                                                  \
            Type cloned_value = Type##_clone(&src->data[i]);                               \
            Result(Void, cref_Char) push_res = Vector_push_back(Type)(&dest, cloned_value);              \
            if (Result_is_err(Void, cref_Char)(&push_res))                                                  \
            {                                                                              \
                Vector_free(Type)(&dest);                                               \
                perror("Memory allocation failed during vector clone");                    \
                exit(1);                                                                   \
            }                                                                              \
        }                                                                                  \
        return dest;                                                                       \
    }
#endif
