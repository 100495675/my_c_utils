#ifndef MY_C_UTILS_CLONE_H
#define MY_C_UTILS_CLONE_H

#include "my_c_utils/tipos.h"
#include "my_c_utils/result.h"

/**
 * Clone Infrastructure
 *
 * Cloning provides explicit duplication semantics for values.
 * For primitive types, this is a trivial operation (copy by value).
 * For containers, clone functions are defined in their respective headers
 * (vector.h, linked_list.h, hash_map.h).
 *
 * Key rules:
 * 1. Clone functions receive the value by-value: Type value, not Type *ptr
 * 2. Trivial types return the clone result directly
 * 3. Container clones return Result_Container_Type (allocated, deep copy)
 * 4. Caller owns cloned containers and must call Type_free() when done
 * 5. Cloning is explicit (not implicit) to surface allocation costs
 */

/**
 * TRIVIAL_CLONE(Type)
 *
 * Generates a trivial clone function for primitive types.
 * Clone functions operate on pointers to values and return a Result_Type.
 *
 * Generated function:
 *   Result_Type Type_clone(const Type *src)
 *
 * Usage:
 *   TRIVIAL_CLONE(Int)
 *   TRIVIAL_CLONE(Char)
 *   TRIVIAL_CLONE(Bool)
 *   etc.
 */
#define TRIVIAL_CLONE(Type)                                          \
    RESULT_CONFIG(Type)                                              \
    static inline Result_##Type Type##_clone(const ref_##Type src)   \
    {                                                                \
        if (!src)                                                    \
        {                                                            \
            return Result_##Type##_err("Cannot clone NULL pointer"); \
        }                                                            \
        return Result_##Type##_ok(*src);                             \
    }

/* TRIVIAL_CLONE only defines the macro; primitive instantiations live in
    include/my_c_utils/primitives.h so projects can opt-in once per binary. */

#endif // MY_C_UTILS_CLONE_H
