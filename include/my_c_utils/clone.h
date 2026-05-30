#ifndef MY_C_UTILS_CLONE_H
#define MY_C_UTILS_CLONE_H

#include <stdio.h>
#include <stdlib.h>

#include "my_c_utils/tipos.h"
#include "my_c_utils/template.h"

/**
 * Clone Infrastructure
 *
 * Cloning provides explicit duplication semantics for values.
 * For primitive types, this is a trivial operation (copy by value).
 * For containers, clone functions are defined in their respective headers
 * (vector.h, linked_list.h, hash_map.h).
 *
 * Key rules:
 * 1. Clone functions receive the source by reference and return the cloned value directly.
 * 2. Primitive clones copy the value directly.
 * 3. Container clones deep-copy and return the owned result.
 * 4. Clone failures abort immediately on invalid input or allocation failure.
 * 5. Cloning is explicit (not implicit) to surface allocation costs.
 */

/**
 * @brief Trait definition to deep-clone/duplicate a value of a type.
 * @usage Clone(T)(&instance)
 */
#define Clone(...) TEMPLATE_METHOD(Clone, clone, __VA_ARGS__)

/**
 * TRIVIAL_CLONE(T)
 *
 * Generates a trivial clone function for primitive types.
 * Clone functions return the cloned value directly.
 *
 * Generated function:
 *   T T_clone(const T *self)
 *
 * Usage:
 *   TRIVIAL_CLONE(Int)
 *   TRIVIAL_CLONE(Char)
 *   TRIVIAL_CLONE(Bool)
 *   etc.
 */
#define TRIVIAL_CLONE(T)                              \
    static inline T TEMPLATE_METHOD(Clone, clone, T)(const T *self)  \
    {                                                 \
        if (!self)                                    \
        {                                             \
            perror("Cannot clone NULL pointer");     \
            exit(1);                                  \
        }                                             \
        return *self;                                 \
    }

/* TRIVIAL_CLONE only defines the macro; primitive instantiations live in
    include/my_c_utils/primitives.h so projects can opt-in once per binary. */

#endif // MY_C_UTILS_CLONE_H
