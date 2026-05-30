#ifndef MY_C_UTILS_DEFAULT_H
#define MY_C_UTILS_DEFAULT_H

#include "my_c_utils/template.h"

/**
 * @brief Trait definition to retrieve the default value or constructor for a type.
 * @usage Default(T)()
 */
#define Default(...) TEMPLATE_METHOD(Default, default, __VA_ARGS__)

/**
 * @brief Configures/registers the Default trait for a primitive or custom type.
 * @usage DEFAULT_CONFIG(Int, 0)
 */
#define DEFAULT_CONFIG(T, ...) \
    static inline T TEMPLATE_METHOD(Default, default, T)(void) \
    { \
        return __VA_ARGS__; \
    }

#endif // MY_C_UTILS_DEFAULT_H
