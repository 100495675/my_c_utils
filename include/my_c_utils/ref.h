#ifndef MY_C_UTILS_REF_H
#define MY_C_UTILS_REF_H

#define MY_C_UTILS_CONCAT_IMPL(a, b) a##b
#define MY_C_UTILS_CONCAT(a, b) MY_C_UTILS_CONCAT_IMPL(a, b)

#include <stdlib.h>
#include <stdio.h>
#include "my_c_utils/free.h"

// ref_##T is a mutable borrowed view.
// cref_##T is an immutable borrowed view.
// Neither owns data and both remain no-op-compatible for generic cleanup code.
// The free functions for these types are defined to do nothing, as they are non-owning references
#define REF_EXPAND(T)                                               \
    typedef T *ref_##T;                                             \
    typedef const T *cref_##T;                                      \
    void MY_C_UTILS_CONCAT(ref_##T, _free)(ref_##T *self) { (void)self; } \
    void MY_C_UTILS_CONCAT(cref_##T, _free)(cref_##T *self) { (void)self; } \
    static inline ref_##T MY_C_UTILS_CONCAT(ref_##T, _clone)(const ref_##T *self) \
    { \
        if (!self) { \
            perror("Cannot clone NULL pointer"); \
            exit(1); \
        } \
        return *self; \
    } \
    static inline cref_##T MY_C_UTILS_CONCAT(cref_##T, _clone)(const cref_##T *self) \
    { \
        if (!self) { \
            perror("Cannot clone NULL pointer"); \
            exit(1); \
        } \
        return *self; \
    } \
    static inline T MY_C_UTILS_CONCAT(ref_##T, _deref)(ref_##T self) \
    { \
        if (!self) { \
            perror("Cannot dereference NULL pointer"); \
            exit(1); \
        } \
        return *self; \
    } \
    static inline T MY_C_UTILS_CONCAT(cref_##T, _deref)(cref_##T self) \
    { \
        if (!self) { \
            perror("Cannot dereference NULL pointer"); \
            exit(1); \
        } \
        return *self; \
    }

/**
 * @brief Represents a mutable borrowed pointer (T*). Non-owning and compatible with container generic cleanup APIs.
 * @usage ref(T)
 */
#define ref(...) TEMPLATE_TYPE(ref, __VA_ARGS__)

/**
 * @brief Represents an immutable borrowed pointer (const T*). Non-owning and compatible with container generic cleanup APIs.
 * @usage cref(T)
 */
#define cref(...) TEMPLATE_TYPE(cref, __VA_ARGS__)

/**
 * @brief Clones the mutable reference wrapper (by copying the pointer address).
 * @param T The type of the referenced value.
 * @param self A pointer to the ref(T) wrapper (&my_ref).
 * @returns ref(T)
 * @usage ref(Int) cloned = ref_clone(Int)(&my_ref);
 */
#define ref_clone(...) TEMPLATE_METHOD(ref, clone, __VA_ARGS__)

/**
 * @brief Clones the immutable reference wrapper (by copying the pointer address).
 * @param T The type of the referenced value.
 * @param self A pointer to the cref(T) wrapper (&my_cref).
 * @returns cref(T)
 * @usage cref(Int) cloned = cref_clone(Int)(&my_cref);
 */
#define cref_clone(...) TEMPLATE_METHOD(cref, clone, __VA_ARGS__)

/**
 * @brief Dereferences the mutable pointer, returning the T value by copy/value. Panics and aborts if NULL.
 * @param T The type of the referenced value.
 * @param self The ref(T) instance.
 * @returns T
 * @usage Int value = ref_deref(Int)(my_ref);
 */
#define ref_deref(...) TEMPLATE_METHOD(ref, deref, __VA_ARGS__)

/**
 * @brief Dereferences the immutable pointer, returning the T value by copy/value. Panics and aborts if NULL.
 * @param T The type of the referenced value.
 * @param self The cref(T) instance.
 * @returns T
 * @usage Int value = cref_deref(Int)(my_cref);
 */
#define cref_deref(...) TEMPLATE_METHOD(cref, deref, __VA_ARGS__)

#endif