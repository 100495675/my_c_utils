#ifndef MY_C_UTILS_FREE_H
#define MY_C_UTILS_FREE_H

#include "my_c_utils/template.h"

/**
 * @brief Trait definition to free/drop resources owned by a type.
 * @usage Free(T)(&instance)
 */
#define Free(...) TEMPLATE_METHOD(Free, free, __VA_ARGS__)

#define TRIVIAL_FREE(T) \
    static inline void TEMPLATE_METHOD(Free, free, T)(T *self) { (void)self; }

#endif // MY_C_UTILS_FREE_H