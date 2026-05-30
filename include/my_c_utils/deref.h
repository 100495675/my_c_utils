#ifndef MY_C_UTILS_DEREF_H
#define MY_C_UTILS_DEREF_H

#include "my_c_utils/template.h"

/**
 * @brief Trait definitions to dereference pointer/reference wrappers.
 * @usage Deref(Box(Int))(&my_box)
 */
#define Deref(...)    TEMPLATE_METHOD(Deref, deref, __VA_ARGS__)
#define DerefMut(...) TEMPLATE_METHOD(DerefMut, deref_mut, __VA_ARGS__)

#endif // MY_C_UTILS_DEREF_H
