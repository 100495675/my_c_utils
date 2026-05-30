#ifndef MY_C_UTILS_PAIR_H
#define MY_C_UTILS_PAIR_H

#include "my_c_utils/template.h"

/**
 * @brief Represents a generic pair of two values.
 * @usage Pair(A, B)
 */
#define Pair(A, B) TEMPLATE_TYPE(Pair, A, B)

/**
 * @brief Configuration macro to instantiate a type-safe Pair(A, B).
 * @param A The type of the first member.
 * @param B The type of the second member.
 */
#define PAIR_CONFIG(A, B) \
    typedef struct { \
        A first; \
        B second; \
    } Pair(A, B); \
    typedef Pair(A, B) *ref(Pair(A, B)); \
    typedef const Pair(A, B) *cref(Pair(A, B)); \
    static inline void TEMPLATE_METHOD(Free, free, ref(Pair(A, B)))(ref(Pair(A, B)) *self) { (void)self; } \
    static inline void TEMPLATE_METHOD(Free, free, cref(Pair(A, B)))(cref(Pair(A, B)) *self) { (void)self; } \
    static inline ref(Pair(A, B)) TEMPLATE_METHOD(Clone, clone, ref(Pair(A, B)))(const ref(Pair(A, B)) *self) { return *self; } \
    static inline cref(Pair(A, B)) TEMPLATE_METHOD(Clone, clone, cref(Pair(A, B)))(const cref(Pair(A, B)) *self) { return *self; } \
    static inline Pair(A, B) TEMPLATE_METHOD(Deref, deref, ref(Pair(A, B)))(ref(Pair(A, B)) self) { return *self; } \
    static inline Pair(A, B) TEMPLATE_METHOD(Deref, deref, cref(Pair(A, B)))(cref(Pair(A, B)) self) { return *self; }

#endif
