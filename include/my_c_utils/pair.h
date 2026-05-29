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
    typedef Pair(A, B) *TEMPLATE_TYPE(ref, Pair(A, B)); \
    typedef const Pair(A, B) *TEMPLATE_TYPE(cref, Pair(A, B)); \
    static inline void TEMPLATE_METHOD(ref, free, Pair(A, B))(TEMPLATE_TYPE(ref, Pair(A, B)) *self) { (void)self; } \
    static inline void TEMPLATE_METHOD(cref, free, Pair(A, B))(TEMPLATE_TYPE(cref, Pair(A, B)) *self) { (void)self; } \
    static inline TEMPLATE_TYPE(ref, Pair(A, B)) TEMPLATE_METHOD(ref, clone, Pair(A, B))(const TEMPLATE_TYPE(ref, Pair(A, B)) *self) { return *self; } \
    static inline TEMPLATE_TYPE(cref, Pair(A, B)) TEMPLATE_METHOD(cref, clone, Pair(A, B))(const TEMPLATE_TYPE(cref, Pair(A, B)) *self) { return *self; } \
    static inline Pair(A, B) TEMPLATE_METHOD(ref, deref, Pair(A, B))(TEMPLATE_TYPE(ref, Pair(A, B)) self) { return *self; } \
    static inline Pair(A, B) TEMPLATE_METHOD(cref, deref, Pair(A, B))(TEMPLATE_TYPE(cref, Pair(A, B)) self) { return *self; }

#endif
