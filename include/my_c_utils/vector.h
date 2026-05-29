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

/**
 * @brief Represents a generic, owning dynamic array that grows automatically.
 * @usage Vector(T)
 */
#define Vector(...) TEMPLATE_TYPE(Vector, __VA_ARGS__)

/**
 * @brief Mutable borrowed pointer reference to Vector(T).
 * @usage ref_Vector(T)
 */
#define ref_Vector(...) TEMPLATE_TYPE(ref_Vector, __VA_ARGS__)

/**
 * @brief Immutable borrowed pointer reference to Vector(T).
 * @usage cref_Vector(T)
 */
#define cref_Vector(...) TEMPLATE_TYPE(cref_Vector, __VA_ARGS__)

/**
 * @brief Represents a generic iterator for Vector(T).
 * @usage iter_Vector(T)
 */
#define iter_Vector(...) TEMPLATE_TYPE(iter_Vector, __VA_ARGS__)

/**
 * @brief Mutable reference to a vector iterator.
 */
#define ref_iter_Vector(...) TEMPLATE_TYPE(ref_iter_Vector, __VA_ARGS__)

/**
 * @brief Immutable reference to a vector iterator.
 */
#define cref_iter_Vector(...) TEMPLATE_TYPE(cref_iter_Vector, __VA_ARGS__)

/**
 * @brief Constructs a new, empty dynamic Vector.
 * @param T The type of the elements.
 * @returns Vector(T)
 * @usage Vector(Int) vec = Vector_new(Int)();
 */
#define Vector_new(...)          TEMPLATE_METHOD(Vector, new, __VA_ARGS__)

/**
 * @brief Appends an element to the end of the Vector.
 * @param T The element type.
 * @param self Pointer to the Vector instance (&my_vec).
 * @param value The value of type T to append (copied into Vector).
 * @returns Result(Void, cref(Char))
 * @usage Result(Void, cref(Char)) r = Vector_push_back(Int)(&my_vec, 42);
 */
#define Vector_push_back(...)    TEMPLATE_METHOD(Vector, push_back, __VA_ARGS__)

/**
 * @brief Borrows an element at the specified index.
 * @param T The element type.
 * @param self Pointer to the Vector instance (&my_vec).
 * @param index The target index.
 * @returns Result(ref(T), cref(Char))
 * @usage Result(ref_Int, cref(Char)) r = Vector_at(Int)(&my_vec, 0);
 */
#define Vector_at(...)           TEMPLATE_METHOD(Vector, at, __VA_ARGS__)

/**
 * @brief Returns the current number of elements stored in the Vector.
 * @param T The element type.
 * @param self Pointer to the Vector instance (&my_vec).
 * @returns Size
 * @usage Size size = Vector_size(Int)(&my_vec);
 */
#define Vector_size(...)         TEMPLATE_METHOD(Vector, size, __VA_ARGS__)

/**
 * @brief Updates/replaces the element at the specified index.
 * @param T The element type.
 * @param self Pointer to the Vector instance (&my_vec).
 * @param index The target index.
 * @param value The new value of type T to assign (overwriting the old one).
 * @returns Result(Void, cref(Char))
 * @usage Vector_set(Int)(&my_vec, 0, 999);
 */
#define Vector_set(...)          TEMPLATE_METHOD(Vector, set, __VA_ARGS__)

/**
 * @brief Clears the Vector, destroying all elements and setting size to 0.
 * @param T The element type.
 * @param self Pointer to the Vector instance (&my_vec).
 * @usage Vector_clear(Int)(&my_vec);
 */
#define Vector_clear(...)        TEMPLATE_METHOD(Vector, clear, __VA_ARGS__)

/**
 * @brief Destroys the Vector, freeing its underlying memory and destroying all elements.
 * @param T The element type.
 * @param self Pointer to the Vector instance (&my_vec).
 * @usage Vector_free(Int)(&my_vec);
 */
#define Vector_free(...)         TEMPLATE_METHOD(Vector, free, __VA_ARGS__)

/**
 * @brief Reserves memory to guarantee a minimum capacity.
 * @param T The element type.
 * @param self Pointer to the Vector instance (&my_vec).
 * @param capacity The desired minimum capacity.
 * @returns Result(Void, cref(Char))
 * @usage Vector_reserve(Int)(&my_vec, 100);
 */
#define Vector_reserve(...)      TEMPLATE_METHOD(Vector, reserve, __VA_ARGS__)

/**
 * @brief Removes and returns the last element of the Vector.
 * @param T The element type.
 * @param self Pointer to the Vector instance (&my_vec).
 * @returns Result(T, cref(Char))
 * @usage Result(Int, cref(Char)) popped = Vector_pop_back(Int)(&my_vec);
 */
#define Vector_pop_back(...)     TEMPLATE_METHOD(Vector, pop_back, __VA_ARGS__)

/**
 * @brief Inserts an element at a specific index, shifting subsequent elements.
 * @param T The element type.
 * @param self Pointer to the Vector instance (&my_vec).
 * @param index The target index to insert at.
 * @param value The value to insert.
 * @returns Result(Void, cref(Char))
 * @usage Vector_insert_at(Int)(&my_vec, 0, 5);
 */
#define Vector_insert_at(...)    TEMPLATE_METHOD(Vector, insert_at, __VA_ARGS__)

/**
 * @brief Removes the element at the specified index, shifting subsequent elements.
 * @param T The element type.
 * @param self Pointer to the Vector instance (&my_vec).
 * @param index The target index to remove.
 * @returns Result(Void, cref(Char))
 * @usage Vector_remove_at(Int)(&my_vec, 1);
 */
#define Vector_remove_at(...)    TEMPLATE_METHOD(Vector, remove_at, __VA_ARGS__)

/**
 * @brief Reallocates the Vector storage to match exactly its current size.
 * @param T The element type.
 * @param self Pointer to the Vector instance (&my_vec).
 * @returns Result(Void, cref(Char))
 * @usage Vector_shrink_to_fit(Int)(&my_vec);
 */
#define Vector_shrink_to_fit(...) TEMPLATE_METHOD(Vector, shrink_to_fit, __VA_ARGS__)

/**
 * @brief Sorts the Vector elements using a comparison function.
 * @param T The element type.
 * @param self Pointer to the Vector instance (&my_vec).
 * @param compare The comparison function pointer.
 * @usage Vector_sort(Int)(&my_vec, compare_ints);
 */
#define Vector_sort(...)         TEMPLATE_METHOD(Vector, sort, __VA_ARGS__)

/**
 * @brief Prints a debug representation of the Vector.
 * @param T The element type.
 * @param self Pointer to the Vector instance (&my_vec).
 */
#define Vector_debug(...)        TEMPLATE_METHOD(Vector, debug, __VA_ARGS__)

/**
 * @brief Creates an iterator starting at the first element.
 * @param T The element type.
 * @param self Pointer to the Vector instance.
 * @returns iter_Vector(T)
 * @usage iter_Vector(Int) it = Vector_into_iter(Int)(&my_vec);
 */
#define Vector_into_iter(...)    TEMPLATE_METHOD(Vector, into_iter, __VA_ARGS__)
#define Vector_into_iter_val(...) TEMPLATE_METHOD(Vector, into_iter_val, __VA_ARGS__)
#define Vector_iter_mut(...)     TEMPLATE_METHOD(Vector, iter_mut, __VA_ARGS__)
#define Vector_iter_const(...)   TEMPLATE_METHOD(Vector, iter_const, __VA_ARGS__)

#define Vector_iter_mut_Result(...) TEMPLATE_TYPE(Vector_iter_mut_Result, __VA_ARGS__)
#define Vector_iter_const_Result(...) TEMPLATE_TYPE(Vector_iter_const_Result, __VA_ARGS__)

/**
 * @brief Clones the Vector, returning an independent deep copy of it.
 * @param T The element type.
 * @param self Pointer to the Vector to clone.
 * @returns Vector(T)
 * @usage Vector(Int) cloned = Vector_clone(Int)(&my_vec);
 */
#define Vector_clone(...)        TEMPLATE_METHOD(Vector, clone, __VA_ARGS__)

/**
 * @brief Returns the current element of the iterator without advancing.
 * @returns Result(ref(T), cref(Char))
 * @usage Result(ref_Int, cref(Char)) r = iter_Vector_deref(Int)(&it);
 */
#define iter_Vector_deref(...)   TEMPLATE_METHOD(iter_Vector, deref, __VA_ARGS__)

/**
 * @brief Advances the iterator and returns the next element.
 * @returns Result(ref(T), cref(Char))
 * @usage Result(ref_Int, cref(Char)) r = iter_Vector_next(Int)(&it);
 */
#define iter_Vector_next(...)    TEMPLATE_METHOD(iter_Vector, next, __VA_ARGS__)

/**
 * @brief Destroys and cleans up the iterator.
 * @usage iter_Vector_free(Int)(&it);
 */
#define iter_Vector_free(...)    TEMPLATE_METHOD(iter_Vector, free, __VA_ARGS__)

// Backward compatibility alias for the manual VECTOR_CONFIG
#define VECTOR_CONFIG(T) TEMPLATE_Vector(T)

// 2. Template Definition Macro
#define TEMPLATE_Vector(T) \
    typedef struct \
    { \
        ref(T) data; \
        Size size; \
        Size capacity; \
    } Vector(T); \
    typedef Vector(T) *ref_Vector(T); \
    typedef const Vector(T) *cref_Vector(T); \
    static inline void TEMPLATE_METHOD(ref_Vector, free, T)(ref_Vector(T) *value) { (void)value; } \
    static inline void TEMPLATE_METHOD(cref_Vector, free, T)(cref_Vector(T) *value) { (void)value; } \
    \
    static inline void TEMPLATE_METHOD(Vector, free, T)(ref_Vector(T) self); \
    \
    typedef struct \
    { \
        Vector(T) vector; \
        Size index; \
    } iter_Vector(T); \
    typedef iter_Vector(T) *ref_iter_Vector(T); \
    typedef const iter_Vector(T) *cref_iter_Vector(T); \
    static inline void TEMPLATE_METHOD(ref_iter_Vector, free, T)(ref_iter_Vector(T) *value) { (void)value; } \
    static inline void TEMPLATE_METHOD(cref_iter_Vector, free, T)(cref_iter_Vector(T) *value) { (void)value; } \
    static inline void iter_Vector_free(T)(cref_iter_Vector(T) value) \
    { \
        TEMPLATE_METHOD(Vector, free, T)((ref_Vector(T))&value->vector); \
    } \
    \
    static inline Vector(T) Vector_new(T)() \
    { \
        return (Vector(T)){.data = NULL, .size = 0, .capacity = 0}; \
    } \
    \
    static inline Result(Void, cref(Char)) Vector_push_back(T)(ref_Vector(T) self, \
                                                   const T value) \
    { \
        if (self->size == self->capacity) \
        { \
            self->capacity = self->capacity == 0 ? 1 : self->capacity * 2; \
            ref(T) new_data = MY_C_UTILS_REALLOC(self->data, self->capacity * sizeof(T)); \
            if (!new_data) \
            { \
                return Result_err(Void, cref(Char))("Memory allocation failed"); \
            } \
            self->data = new_data; \
        } \
        self->data[self->size++] = value; \
        return Result_ok(Void, cref(Char))((Void){}); \
    } \
    \
    static inline Result(ref(T), cref(Char)) Vector_at(T)( \
        cref_Vector(T) self, const Size index) \
    { \
        if (index >= self->size) \
        { \
            return Result_err(ref(T), cref(Char))("Index out of bounds"); \
        } \
        return Result_ok(ref(T), cref(Char))(&self->data[index]); \
    } \
    \
    static inline Size Vector_size(T)(cref_Vector(T) self) \
    { \
        return self->size; \
    } \
    \
    static inline Result(Void, cref(Char)) Vector_set(T)( \
        ref_Vector(T) self, const Size index, const T value) \
    { \
        if (index >= self->size) \
        { \
            return Result_err(Void, cref(Char))("Index out of bounds"); \
        } \
        T##_free(&self->data[index]); \
        self->data[index] = value; \
        return Result_ok(Void, cref(Char))((Void){}); \
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
    static inline Result(Void, cref(Char)) Vector_reserve(T)(ref_Vector(T) self, \
                                                 const Size capacity) \
    { \
        if (capacity > self->capacity) \
        { \
            ref(T) new_data = MY_C_UTILS_REALLOC(self->data, capacity * sizeof(T)); \
            if (!new_data) \
            { \
                return Result_err(Void, cref(Char))("Memory allocation failed"); \
            } \
            self->data = new_data; \
            self->capacity = capacity; \
        } \
        return Result_ok(Void, cref(Char))((Void){}); \
    } \
    \
    static inline Result(T, cref(Char)) Vector_pop_back(T)(ref_Vector(T) self) \
    { \
        if (self->size == 0) \
        { \
            return Result_err(T, cref(Char))("Vector is empty"); \
        } \
        return Result_ok(T, cref(Char))(self->data[--self->size]); \
    } \
    \
    static inline Result(Void, cref(Char)) Vector_insert_at(T)(ref_Vector(T) self, \
                                                   Size index, \
                                                    T value) \
    { \
        if (index > self->size) \
        { \
            return Result_err(Void, cref(Char))("Index out of bounds"); \
        } \
        if (self->size == self->capacity) \
        { \
            self->capacity = self->capacity == 0 ? 1 : self->capacity * 2; \
            ref(T) new_data = MY_C_UTILS_REALLOC(self->data, self->capacity * sizeof(T));\
            if (!new_data) \
            { \
                return Result_err(Void, cref(Char))("Memory allocation failed"); \
            } \
            self->data = new_data; \
        } \
        for (Size i = self->size; i > index; --i) \
        { \
            self->data[i] = self->data[i - 1]; \
        } \
        self->data[index] = value; \
        self->size++; \
        return Result_ok(Void, cref(Char))((Void){}); \
    } \
    \
    static inline Result(Void, cref(Char)) Vector_remove_at(T)(ref_Vector(T) self, Size index) \
    { \
        if (index >= self->size) \
        { \
            return Result_err(Void, cref(Char))("Index out of bounds"); \
        } \
        T##_free(&self->data[index]); \
        for (Size i = index; i < self->size - 1; ++i) \
        { \
            self->data[i] = self->data[i + 1]; \
        } \
        self->size--; \
        return Result_ok(Void, cref(Char))((Void){}); \
    } \
    \
    static inline Result(Void, cref(Char)) Vector_shrink_to_fit(T)(ref_Vector(T) self) \
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
                ref(T) new_data = MY_C_UTILS_REALLOC(self->data, self->size * sizeof(T));\
                if (!new_data) \
                { \
                    return Result_err(Void, cref(Char))("Memory allocation failed"); \
                } \
                self->data = new_data; \
                self->capacity = self->size; \
            } \
        } \
        return Result_ok(Void, cref(Char))((Void){}); \
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
        Vector(T) self) \
    { \
        return (iter_Vector(T)){.vector = self, .index = 0}; \
    } \
    \
    static inline Result(ref(T), cref(Char)) iter_Vector_deref(T)( \
        cref_iter_Vector(T) self) \
    { \
        if (self->index >= self->vector.size) \
        { \
            return Result_err(ref(T), cref(Char))("Iterator out of bounds"); \
        } \
        return Result_ok(ref(T), cref(Char))((ref(T))&(self->vector.data[self->index])); \
    } \
    \
    static inline Result(ref(T), cref(Char)) iter_Vector_next(T)(ref_iter_Vector(T) self) \
    { \
        if (self->index >= self->vector.size) \
        { \
            return Result_err(ref(T), cref(Char))("Iterator out of bounds"); \
        } \
        return Result_ok(ref(T), cref(Char))(&self->vector.data[self->index++]); \
    } \
    \
    RESULT_CONFIG(Vector(T), cref(Char)) \
    RESULT_CONFIG(ref(Vector(T)), cref(Char)) \
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
            Result(Void, cref(Char)) push_res = Vector_push_back(T)(&dest, cloned_value); \
            if (Result_is_err(Void, cref(Char))(&push_res)) \
            { \
                Vector_free(T)(&dest); \
                perror("Memory allocation failed during vector clone"); \
                exit(1); \
            } \
        } \
        return dest; \
    } \
    \
    typedef struct \
    { \
        Vector(T) vector; \
        Size index; \
    } iter_val(Vector(T)); \
    typedef iter_val(Vector(T)) *ref_iter_val(Vector(T)); \
    typedef const iter_val(Vector(T)) *cref_iter_val(Vector(T)); \
    static inline void TEMPLATE_METHOD(ref_iter_val(Vector(T)), free, T)(ref_iter_val(Vector(T)) *value) { (void)value; } \
    static inline void TEMPLATE_METHOD(cref_iter_val(Vector(T)), free, T)(cref_iter_val(Vector(T)) *value) { (void)value; } \
    static inline void iter_val_free(Vector(T))(cref_iter_val(Vector(T)) value) \
    { \
        for (Size _i = value->index; _i < value->vector.size; ++_i) \
        { \
            MY_C_UTILS_CONCAT(T, _free)(&value->vector.data[_i]); \
        } \
        TEMPLATE_METHOD(Vector, free, T)((ref_Vector(T))&value->vector); \
    } \
    \
    typedef struct \
    { \
        T *value; \
        bool is_error; \
    } Vector_iter_mut_Result(T); \
    typedef struct \
    { \
        const T *value; \
        bool is_error; \
    } Vector_iter_const_Result(T); \
    \
    typedef struct \
    { \
        ref_Vector(T) vector; \
        Size index; \
    } iter_mut(Vector(T)); \
    typedef iter_mut(Vector(T)) *ref_iter_mut(Vector(T)); \
    typedef const iter_mut(Vector(T)) *cref_iter_mut(Vector(T)); \
    static inline void TEMPLATE_METHOD(ref_iter_mut(Vector(T)), free, T)(ref_iter_mut(Vector(T)) *value) { (void)value; } \
    static inline void TEMPLATE_METHOD(cref_iter_mut(Vector(T)), free, T)(cref_iter_mut(Vector(T)) *value) { (void)value; } \
    static inline void iter_mut_free(Vector(T))(cref_iter_mut(Vector(T)) value) { (void)value; } \
    \
    typedef struct \
    { \
        cref_Vector(T) vector; \
        Size index; \
    } iter_const(Vector(T)); \
    typedef iter_const(Vector(T)) *ref_iter_const(Vector(T)); \
    typedef const iter_const(Vector(T)) *cref_iter_const(Vector(T)); \
    static inline void TEMPLATE_METHOD(ref_iter_const(Vector(T)), free, T)(ref_iter_const(Vector(T)) *value) { (void)value; } \
    static inline void TEMPLATE_METHOD(cref_iter_const(Vector(T)), free, T)(cref_iter_const(Vector(T)) *value) { (void)value; } \
    static inline void iter_const_free(Vector(T))(cref_iter_const(Vector(T)) value) { (void)value; } \
    \
    static inline iter_val(Vector(T)) Vector_into_iter_val(T)( \
        Vector(T) self) \
    { \
        return (iter_val(Vector(T))){.vector = self, .index = 0}; \
    } \
    \
    static inline Result(T, cref(Char)) iter_val_deref(Vector(T))( \
        cref_iter_val(Vector(T)) self) \
    { \
        if (self->index >= self->vector.size) \
        { \
            return Result_err(T, cref(Char))("Iterator out of bounds"); \
        } \
        return Result_ok(T, cref(Char))(self->vector.data[self->index]); \
    } \
    \
    static inline Result(T, cref(Char)) iter_val_next(Vector(T))(ref_iter_val(Vector(T)) self) \
    { \
        if (self->index >= self->vector.size) \
        { \
            return Result_err(T, cref(Char))("Iterator out of bounds"); \
        } \
        return Result_ok(T, cref(Char))(self->vector.data[self->index++]); \
    } \
    \
    static inline iter_mut(Vector(T)) Vector_iter_mut(T)( \
        ref_Vector(T) self) \
    { \
        return (iter_mut(Vector(T))){.vector = self, .index = 0}; \
    } \
    \
    static inline Vector_iter_mut_Result(T) iter_mut_deref(Vector(T))( \
        cref_iter_mut(Vector(T)) self) \
    { \
        if (self->index >= self->vector->size) \
        { \
            return (Vector_iter_mut_Result(T)){.value = NULL, .is_error = true}; \
        } \
        return (Vector_iter_mut_Result(T)){.value = &(self->vector->data[self->index]), .is_error = false}; \
    } \
    \
    static inline Vector_iter_mut_Result(T) iter_mut_next(Vector(T))(ref_iter_mut(Vector(T)) self) \
    { \
        if (self->index >= self->vector->size) \
        { \
            return (Vector_iter_mut_Result(T)){.value = NULL, .is_error = true}; \
        } \
        return (Vector_iter_mut_Result(T)){.value = &self->vector->data[self->index++], .is_error = false}; \
    } \
    \
    static inline iter_const(Vector(T)) Vector_iter_const(T)( \
        cref_Vector(T) self) \
    { \
        return (iter_const(Vector(T))){.vector = self, .index = 0}; \
    } \
    \
    static inline Vector_iter_const_Result(T) iter_const_deref(Vector(T))( \
        cref_iter_const(Vector(T)) self) \
    { \
        if (self->index >= self->vector->size) \
        { \
            return (Vector_iter_const_Result(T)){.value = NULL, .is_error = true}; \
        } \
        return (Vector_iter_const_Result(T)){.value = &(self->vector->data[self->index]), .is_error = false}; \
    } \
    \
    static inline Vector_iter_const_Result(T) iter_const_next(Vector(T))(ref_iter_const(Vector(T)) self) \
    { \
        if (self->index >= self->vector->size) \
        { \
            return (Vector_iter_const_Result(T)){.value = NULL, .is_error = true}; \
        } \
        return (Vector_iter_const_Result(T)){.value = &self->vector->data[self->index++], .is_error = false}; \
    } \
    \
    static inline Vector(T) TEMPLATE_METHOD(Default, default, Vector(T))(void) \
    { \
        return Vector_new(T)(); \
    }

#endif
