#ifndef MY_C_UTILS_REF_H
#define MY_C_UTILS_REF_H

#define MY_C_UTILS_CONCAT_IMPL(a, b) a##b
#define MY_C_UTILS_CONCAT(a, b) MY_C_UTILS_CONCAT_IMPL(a, b)

#include <stdlib.h>
#include "my_c_utils/free.h"

// ref_##Type is a mutable borrowed view.
// cref_##Type is an immutable borrowed view.
// Neither owns data and both remain no-op-compatible for generic cleanup code.
#define REF_EXPAND(Type)             \
    typedef Type *ref_##Type;        \
    typedef const Type *cref_##Type; \
    TRIVIAL_FREE(ref_##Type, ref_##Type *value)  \
    TRIVIAL_FREE(cref_##Type, cref_##Type *value)

#endif