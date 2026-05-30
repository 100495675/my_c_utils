#ifndef MY_C_UTILS_BOX_H
#define MY_C_UTILS_BOX_H

#include <stdio.h>
#include <stdlib.h>

#include "my_c_utils/allocator.h"
#include "my_c_utils/result.h"
#include "my_c_utils/primitives.h"
#include "my_c_utils/template.h"

/**
 * @brief Represents an owning, heap-allocated wrapper for a single value.
 * @details Resolves to the specialized template type (e.g. Box_Int).
 * @usage Box(T)
 */
#define Box(...) TEMPLATE_TYPE(Box, __VA_ARGS__)

/**
 * @brief Mutable borrowed pointer reference to Box(T).
 * @usage ref_Box(T)
 */
#define ref_Box(...) TEMPLATE_TYPE(ref_Box, __VA_ARGS__)

/**
 * @brief Immutable borrowed pointer reference to Box(T).
 * @usage cref_Box(T)
 */
#define cref_Box(...) TEMPLATE_TYPE(cref_Box, __VA_ARGS__)

/**
 * @brief Allocates heap memory for T and creates an owning Box(T).
 * @param T The type of the value to box.
 * @param value The value of type T to store.
 * @returns Box(T)
 * @usage Box_new(Int)(42)
 */
#define Box_new(...) TEMPLATE_METHOD(Box, new, __VA_ARGS__)

/**
 * @brief Frees the heap-allocated memory and destroys the boxed value.
 * @param T The type of the boxed value.
 * @param self The Box(T) instance.
 * @usage Box_free(Int)(my_box)
 */
#define Box_free(...) Free(Box(__VA_ARGS__))

/**
 * @brief Returns an immutable borrow (cref(T)) pointing to the inner value.
 * @param T The type of the boxed value.
 * @param self A pointer to the Box(T) instance (ref_Box(T) or &Box(T)).
 * @returns cref(T)
 * @usage cref_deref(Int)(Box_deref(Int)(&my_box))
 */
#define Box_deref(...) Deref(Box(__VA_ARGS__))

/**
 * @brief Returns a mutable borrow (ref(T)) pointing to the inner value.
 * @param T The type of the boxed value.
 * @param self A pointer to the Box(T) instance (ref_Box(T) or &Box(T)).
 * @returns ref(T)
 * @usage *Box_deref_mut(Int)(&my_box) = 99;
 */
#define Box_deref_mut(...) DerefMut(Box(__VA_ARGS__))

/**
 * @brief Extracts the inner T value from the Box, destroying the Box and transferring ownership.
 * @param T The type of the boxed value.
 * @param self The Box(T) instance.
 * @returns T
 * @usage Int value = Box_into_inner(Int)(my_box)
 */
#define Box_into_inner(...) TEMPLATE_METHOD(Box, into_inner, __VA_ARGS__)

/**
 * @brief Clones the Box(T) by cloning its inner value.
 * @param T The type of the boxed value.
 * @param self A pointer to the Box(T) to clone (&Box(T)).
 * @returns Box(T)
 * @usage Box(Int) cloned = Box_clone(Int)(&my_box)
 */
#define Box_clone(...) Clone(Box(__VA_ARGS__))

// Backward compatibility alias for the manual BOX_CONFIG
#define BOX_CONFIG(T) TEMPLATE_Box(T)

// 2. Template Definition Macro
#define TEMPLATE_Box(T) \
    typedef struct { \
        ref(T) value; \
    } Box(T); \
    typedef Box(T) *ref_Box(T); \
    typedef const Box(T) *cref_Box(T); \
    static inline void TEMPLATE_METHOD(Free, free, ref(Box(T)))(ref(Box(T)) *self) { (void)self; } \
    static inline void TEMPLATE_METHOD(Free, free, cref(Box(T)))(cref(Box(T)) *self) { (void)self; } \
    static inline ref(Box(T)) TEMPLATE_METHOD(Clone, clone, ref(Box(T)))(const ref(Box(T)) *self) { return *self; } \
    static inline cref(Box(T)) TEMPLATE_METHOD(Clone, clone, cref(Box(T)))(const cref(Box(T)) *self) { return *self; } \
    static inline Box(T) TEMPLATE_METHOD(Deref, deref, ref(Box(T)))(ref(Box(T)) self) { return *self; } \
    static inline Box(T) TEMPLATE_METHOD(Deref, deref, cref(Box(T)))(cref(Box(T)) self) { return *self; } \
    \
    static inline Box(T) Box_new(T)(T value) { \
        ref(T) boxed_value = MY_C_UTILS_MALLOC(sizeof(T)); \
        if (!boxed_value) { \
            perror("Memory allocation failed"); \
            exit(1); \
        } \
        *boxed_value = value; \
        return (Box(T)){.value = boxed_value}; \
    } \
    static inline T Box_into_inner(T)(ref(Box(T)) self) { \
        if (!self->value) { \
            fputs("Box is empty\n", stderr); \
            exit(1); \
        } \
        T value = *self->value; \
        MY_C_UTILS_FREE(self->value); \
        self->value = NULL; \
        return value; \
    } \
    static inline void TEMPLATE_METHOD(Free, free, Box(T))(ref(Box(T)) self) { \
        if (self->value) { \
            Free(T)(self->value); \
            MY_C_UTILS_FREE(self->value); \
            self->value = NULL; \
        } \
    } \
    static inline Box(T) TEMPLATE_METHOD(Clone, clone, Box(T))(cref(Box(T)) self) { \
        if (!self) { \
            perror("Cannot clone NULL Box pointer"); \
            exit(1); \
        } \
        return Box_new(T)(Clone(T)(self->value)); \
    } \
    static inline cref(T) TEMPLATE_METHOD(Deref, deref, Box(T))(cref(Box(T)) self) { \
        return self->value; \
    } \
    static inline ref(T) TEMPLATE_METHOD(DerefMut, deref_mut, Box(T))(ref(Box(T)) self) { \
        return self->value; \
    } \
    RESULT_CONFIG(Box(T), cref(Char)) \
    RESULT_CONFIG(ref(Box(T)), cref(Char))

#endif