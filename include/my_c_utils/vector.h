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
#include "my_c_utils/template.h"

// 1. User-Facing Macros (Prefix-free, template-engine compatible)
#define Vector(...) TEMPLATE_TYPE(Vector, __VA_ARGS__)
#define ref_Vector(...) TEMPLATE_TYPE(ref_Vector, __VA_ARGS__)
#define cref_Vector(...) TEMPLATE_TYPE(cref_Vector, __VA_ARGS__)

#define iter_Vector(...) TEMPLATE_TYPE(iter_Vector, __VA_ARGS__)
#define ref_iter_Vector(...) TEMPLATE_TYPE(ref_iter_Vector, __VA_ARGS__)
#define cref_iter_Vector(...) TEMPLATE_TYPE(cref_iter_Vector, __VA_ARGS__)

#define Vector_new(...)          TEMPLATE_METHOD(Vector, new, __VA_ARGS__)
#define Vector_push_back(...)    TEMPLATE_METHOD(Vector, push_back, __VA_ARGS__)
#define Vector_at(...)           TEMPLATE_METHOD(Vector, at, __VA_ARGS__)
#define Vector_size(...)         TEMPLATE_METHOD(Vector, size, __VA_ARGS__)
#define Vector_set(...)          TEMPLATE_METHOD(Vector, set, __VA_ARGS__)
#define Vector_clear(...)        TEMPLATE_METHOD(Vector, clear, __VA_ARGS__)
#define Vector_free(...)         TEMPLATE_METHOD(Vector, free, __VA_ARGS__)
#define Vector_reserve(...)      TEMPLATE_METHOD(Vector, reserve, __VA_ARGS__)
#define Vector_pop_back(...)     TEMPLATE_METHOD(Vector, pop_back, __VA_ARGS__)
#define Vector_insert_at(...)    TEMPLATE_METHOD(Vector, insert_at, __VA_ARGS__)
#define Vector_remove_at(...)    TEMPLATE_METHOD(Vector, remove_at, __VA_ARGS__)
#define Vector_shrink_to_fit(...) TEMPLATE_METHOD(Vector, shrink_to_fit, __VA_ARGS__)
#define Vector_sort(...)         TEMPLATE_METHOD(Vector, sort, __VA_ARGS__)
#define Vector_debug(...)        TEMPLATE_METHOD(Vector, debug, __VA_ARGS__)
#define Vector_into_iter(...)    TEMPLATE_METHOD(Vector, into_iter, __VA_ARGS__)
#define Vector_clone(...)        TEMPLATE_METHOD(Vector, clone, __VA_ARGS__)

#define iter_Vector_deref(...)   TEMPLATE_METHOD(iter_Vector, deref, __VA_ARGS__)
#define iter_Vector_next(...)    TEMPLATE_METHOD(iter_Vector, next, __VA_ARGS__)
#define iter_Vector_free(...)    TEMPLATE_METHOD(iter_Vector, free, __VA_ARGS__)

// Backward compatibility alias for the manual VECTOR_CONFIG
#define VECTOR_CONFIG(T) TEMPLATE_Vector(T)

// 2. Template Definition Macro
#define TEMPLATE_Vector(T) \
    typedef struct \
    { \
        ref_##T data; \
        Size size; \
        Size capacity; \
    } Vector(T); \
    typedef Vector(T) *ref_Vector(T); \
    typedef const Vector(T) *cref_Vector(T); \
    static inline void TEMPLATE_METHOD(ref_Vector, free, T)(ref_Vector(T) *value) { (void)value; } \
    static inline void TEMPLATE_METHOD(cref_Vector, free, T)(cref_Vector(T) *value) { (void)value; } \
    \
    typedef struct \
    { \
        cref_Vector(T) vector; \
        Size index; \
    } iter_Vector(T); \
    typedef iter_Vector(T) *ref_iter_Vector(T); \
    typedef const iter_Vector(T) *cref_iter_Vector(T); \
    static inline void TEMPLATE_METHOD(ref_iter_Vector, free, T)(ref_iter_Vector(T) *value) { (void)value; } \
    static inline void TEMPLATE_METHOD(cref_iter_Vector, free, T)(cref_iter_Vector(T) *value) { (void)value; } \
    static inline void iter_Vector_free(T)(cref_iter_Vector(T) value) \
    { \
        (void)value; \
    } \
    \
    static inline Vector(T) Vector_new(T)() \
    { \
        return (Vector(T)){.data = NULL, .size = 0, .capacity = 0}; \
    } \
    \
    static inline Result(Void, cref_Char) Vector_push_back(T)(ref_Vector(T) self, \
                                                   const T value) \
    { \
        if (self->size == self->capacity) \
        { \
            self->capacity = self->capacity == 0 ? 1 : self->capacity * 2; \
            ref_##T new_data = MY_C_UTILS_REALLOC(self->data, self->capacity * sizeof(T)); \
            if (!new_data) \
            { \
                return Result_err(Void, cref_Char)("Memory allocation failed"); \
            } \
            self->data = new_data; \
        } \
        self->data[self->size++] = value; \
        return Result_ok(Void, cref_Char)((Void){}); \
    } \
    \
    static inline Result(ref_##T, cref_Char) Vector_at(T)( \
        cref_Vector(T) self, const Size index) \
    { \
        if (index >= self->size) \
        { \
            return Result_err(ref_##T, cref_Char)("Index out of bounds"); \
        } \
        return Result_ok(ref_##T, cref_Char)(&self->data[index]); \
    } \
    \
    static inline Size Vector_size(T)(cref_Vector(T) self) \
    { \
        return self->size; \
    } \
    \
    static inline Result(Void, cref_Char) Vector_set(T)( \
        ref_Vector(T) self, const Size index, const T value) \
    { \
        if (index >= self->size) \
        { \
            return Result_err(Void, cref_Char)("Index out of bounds"); \
        } \
        T##_free(&self->data[index]); \
        self->data[index] = value; \
        return Result_ok(Void, cref_Char)((Void){}); \
    } \
    \
    static inline void Vector_clear(T)(ref_Vector(T) self) \
    { \
        for (Size i = 0; i < self->size; ++i) \
        { \
            T##_free(&self->data[i]); \
        } \
        self->size = 0; \
    } \
    \
    static inline void Vector_free(T)(ref_Vector(T) self) \
    { \
        for (Size i = 0; i < self->size; ++i) \
        { \
            T##_free(&self->data[i]); \
        } \
        MY_C_UTILS_FREE(self->data); \
        self->data = NULL; \
        self->size = 0; \
        self->capacity = 0; \
    } \
    \
    static inline Result(Void, cref_Char) Vector_reserve(T)(ref_Vector(T) self, \
                                                 const Size capacity) \
    { \
        if (capacity > self->capacity) \
        { \
            ref_##T new_data = MY_C_UTILS_REALLOC(self->data, capacity * sizeof(T)); \
            if (!new_data) \
            { \
                return Result_err(Void, cref_Char)("Memory allocation failed"); \
            } \
            self->data = new_data; \
            self->capacity = capacity; \
        } \
        return Result_ok(Void, cref_Char)((Void){}); \
    } \
    \
    static inline Result(T, cref_Char) Vector_pop_back(T)(ref_Vector(T) self) \
    { \
        if (self->size == 0) \
        { \
            return Result_err(T, cref_Char)("Vector is empty"); \
        } \
        return Result_ok(T, cref_Char)(self->data[--self->size]); \
    } \
    \
    static inline Result(Void, cref_Char) Vector_insert_at(T)(ref_Vector(T) self, \
                                                   Size index, \
                                                    T value) \
    { \
        if (index > self->size) \
        { \
            return Result_err(Void, cref_Char)("Index out of bounds"); \
        } \
        if (self->size == self->capacity) \
        { \
            self->capacity = self->capacity == 0 ? 1 : self->capacity * 2; \
            ref_##T new_data = MY_C_UTILS_REALLOC(self->data, self->capacity * sizeof(T));\
            if (!new_data) \
            { \
                return Result_err(Void, cref_Char)("Memory allocation failed"); \
            } \
            self->data = new_data; \
        } \
        for (Size i = self->size; i > index; --i) \
        { \
            self->data[i] = self->data[i - 1]; \
        } \
        self->data[index] = value; \
        self->size++; \
        return Result_ok(Void, cref_Char)((Void){}); \
    } \
    \
    static inline Result(Void, cref_Char) Vector_remove_at(T)(ref_Vector(T) self, Size index) \
    { \
        if (index >= self->size) \
        { \
            return Result_err(Void, cref_Char)("Index out of bounds"); \
        } \
        T##_free(&self->data[index]); \
        for (Size i = index; i < self->size - 1; ++i) \
        { \
            self->data[i] = self->data[i + 1]; \
        } \
        self->size--; \
        return Result_ok(Void, cref_Char)((Void){}); \
    } \
    \
    static inline Result(Void, cref_Char) Vector_shrink_to_fit(T)(ref_Vector(T) self) \
    { \
        if (self->capacity > self->size) \
        { \
            if (self->size == 0) \
            { \
                MY_C_UTILS_FREE(self->data); \
                self->data = NULL; \
                self->capacity = 0; \
            } \
            else \
            { \
                ref_##T new_data = MY_C_UTILS_REALLOC(self->data, self->size * sizeof(T));\
                if (!new_data) \
                { \
                    return Result_err(Void, cref_Char)("Memory allocation failed"); \
                } \
                self->data = new_data; \
                self->capacity = self->size; \
            } \
        } \
        return Result_ok(Void, cref_Char)((Void){}); \
    } \
    \
    static inline void Vector_sort(T)(ref_Vector(T) self, \
                                            int (*compare)(const void *, const void *)) \
    { \
        if (self->data && self->size > 1) \
        { \
            qsort(self->data, self->size, sizeof(T), compare); \
        } \
    } \
    \
    static inline void Vector_debug(T)(cref_Vector(T) self) \
    { \
        printf("Vector: size=%zu, capacity=%zu{\n", self->size, self->capacity); \
        for (Size i = 0; i < self->size; ++i) \
        { \
            printf("  [%zu]\n", i); \
        } \
        printf("}\n"); \
    } \
    \
    static inline iter_Vector(T) Vector_into_iter(T)( \
        cref_Vector(T) self) \
    { \
        return (iter_Vector(T)){.vector = self, .index = 0}; \
    } \
    \
    static inline Result(ref_##T, cref_Char) iter_Vector_deref(T)( \
        cref_iter_Vector(T) self) \
    { \
        if (self->index >= self->vector->size) \
        { \
            return Result_err(ref_##T, cref_Char)("Iterator out of bounds"); \
        } \
        return Result_ok(ref_##T, cref_Char)(&(self->vector->data[self->index])); \
    } \
    \
    static inline Result(ref_##T, cref_Char) iter_Vector_next(T)(ref_iter_Vector(T) self) \
    { \
        if (self->index >= self->vector->size) \
        { \
            return Result_err(ref_##T, cref_Char)("Iterator out of bounds"); \
        } \
        return Result_ok(ref_##T, cref_Char)(&self->vector->data[self->index++]); \
    } \
    RESULT_CONFIG(Vector(T), cref_Char) \
    RESULT_CONFIG(ref_Vector(T), cref_Char) \
    static inline Vector(T) Vector_clone(T)(cref_Vector(T) self) \
    { \
        if (!self) \
        { \
            perror("Cannot clone NULL Vector pointer"); \
            exit(1); \
        } \
        Vector(T) dest = Vector_new(T)(); \
        for (Size i = 0; i < self->size; ++i) \
        { \
            T cloned_value = T##_clone(&self->data[i]); \
            Result(Void, cref_Char) push_res = Vector_push_back(T)(&dest, cloned_value); \
            if (Result_is_err(Void, cref_Char)(&push_res)) \
            { \
                Vector_free(T)(&dest); \
                perror("Memory allocation failed during vector clone"); \
                exit(1); \
            } \
        } \
        return dest; \
    }

#endif
