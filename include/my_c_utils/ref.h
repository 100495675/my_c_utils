#ifndef MY_C_UTILS_REF_H
#define MY_C_UTILS_REF_H

#define MY_C_UTILS_CONCAT_IMPL(a, b) a##b
#define MY_C_UTILS_CONCAT(a, b) MY_C_UTILS_CONCAT_IMPL(a, b)

#include <stdlib.h>
#include "my_c_utils/free.h"

// ref_##T is a mutable borrowed view.
// cref_##T is an immutable borrowed view.
// Neither owns data and both remain no-op-compatible for generic cleanup code.
// The free functions for these types are defined to do nothing, as they are non-owning references
#define REF_EXPAND(T)                                               \
    typedef T *ref_##T;                                             \
    typedef const T *cref_##T;                                      \
                                                                    \
    void MY_C_UTILS_CONCAT(ref_##T, _free)(ref_##T *self) { (void)self; } \
    void MY_C_UTILS_CONCAT(cref_##T, _free)(cref_##T *self) { (void)self; }

#endif