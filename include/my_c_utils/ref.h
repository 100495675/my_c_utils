#ifndef MY_C_UTILS_REF_H
#define MY_C_UTILS_REF_H

#include <stdlib.h>
#include "my_c_utils/free.h"

// ref_##Type is a borrowed view. It does not own data and its free function is a no-op.
// 5 levels of indirection should be enough for anyone.
#define REF_EXPAND(Type)                                        \
    typedef Type *ref_##Type;                                   \
    typedef ref_##Type *ref_ref_##Type;                         \
    typedef ref_ref_##Type *ref_ref_ref_##Type;                 \
    typedef ref_ref_ref_##Type *ref_ref_ref_ref_##Type;         \
    typedef ref_ref_ref_ref_##Type *ref_ref_ref_ref_ref_##Type; \
    TRIVIAL_FREE(ref_##Type)                                    \
    TRIVIAL_FREE(ref_ref_##Type)                                \
    TRIVIAL_FREE(ref_ref_ref_##Type)                            \
    TRIVIAL_FREE(ref_ref_ref_ref_##Type)

#endif