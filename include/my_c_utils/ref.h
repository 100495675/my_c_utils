#ifndef MY_C_UTILS_REF_H
#define MY_C_UTILS_REF_H

#include <stdlib.h>
#include "my_c_utils/free.h"

// ref_##Type is a mutable borrowed view.
// cref_##Type is an immutable borrowed view.
// Neither owns data and both remain no-op-compatible for generic cleanup code.
// 5 levels of indirection should be enough for anyone.
#define REF_EXPAND(Type)             \
    typedef Type *ref_##Type;        \
    typedef const Type *cref_##Type; \
    TRIVIAL_FREE(ref_##Type, ref_##Type *value)  \
    TRIVIAL_FREE(cref_##Type, cref_##Type *value)

#endif