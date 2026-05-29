#ifndef MY_C_UTILS_TEMPLATE_H
#define MY_C_UTILS_TEMPLATE_H

#include "my_c_utils/tipos.h"

// Argument counting and concatenation helper macros
#define MY_C_UTILS_PP_CAT(a, b) MY_C_UTILS_PP_CAT_I(a, b)
#define MY_C_UTILS_PP_CAT_I(a, b) a##b

#define MY_C_UTILS_NARGS(...) MY_C_UTILS_NARGS_I(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define MY_C_UTILS_NARGS_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...) N

#define MY_C_UTILS_VAR_CONCAT_1(a) a
#define MY_C_UTILS_VAR_CONCAT_2(a, b) MY_C_UTILS_CONCAT(a, b)
#define MY_C_UTILS_VAR_CONCAT_3(a, b, c) MY_C_UTILS_CONCAT(MY_C_UTILS_CONCAT(a, b), MY_C_UTILS_CONCAT(_, c))
#define MY_C_UTILS_VAR_CONCAT_4(a, b, c, d) MY_C_UTILS_CONCAT(MY_C_UTILS_VAR_CONCAT_3(a, b, c), MY_C_UTILS_CONCAT(_, d))
#define MY_C_UTILS_VAR_CONCAT_5(a, b, c, d, e) MY_C_UTILS_CONCAT(MY_C_UTILS_VAR_CONCAT_4(a, b, c, d), MY_C_UTILS_CONCAT(_, e))
#define MY_C_UTILS_VAR_CONCAT_6(a, b, c, d, e, f) MY_C_UTILS_CONCAT(MY_C_UTILS_VAR_CONCAT_5(a, b, c, d, e), MY_C_UTILS_CONCAT(_, f))

#define MY_C_UTILS_VAR_CONCAT(...) \
    MY_C_UTILS_PP_CAT(MY_C_UTILS_VAR_CONCAT_, MY_C_UTILS_NARGS(__VA_ARGS__))(__VA_ARGS__)

// Resolves to the specialized template type name (e.g., Pair_Int)
#define TEMPLATE_TYPE(Name, ...) MY_C_UTILS_VAR_CONCAT(Name##_, __VA_ARGS__)

// Resolves to the specialized template member method name (e.g., Pair_Int_new)
#define TEMPLATE_METHOD(Name, Method, ...) MY_C_UTILS_CONCAT(TEMPLATE_TYPE(Name, __VA_ARGS__), _##Method)

// Instantiates a registered template with the given type arguments
#define TEMPLATE_CONFIG(TemplateName, ...) MY_C_UTILS_CONCAT(TEMPLATE_, TemplateName)(__VA_ARGS__)

#define iter(Container) MY_C_UTILS_CONCAT(iter_, Container)
#define ref_iter(Container) MY_C_UTILS_CONCAT(ref_iter_, Container)
#define cref_iter(Container) MY_C_UTILS_CONCAT(cref_iter_, Container)

#define into_iter(Container) MY_C_UTILS_CONCAT(Container, _into_iter)
#define iter_next(Container) MY_C_UTILS_CONCAT(iter(Container), _next)
#define iter_deref(Container) MY_C_UTILS_CONCAT(iter(Container), _deref)
#define iter_free(Container) MY_C_UTILS_CONCAT(iter(Container), _free)

#define iter_val(Container) MY_C_UTILS_CONCAT(iter_val_, Container)
#define ref_iter_val(Container) MY_C_UTILS_CONCAT(ref_iter_val_, Container)
#define cref_iter_val(Container) MY_C_UTILS_CONCAT(cref_iter_val_, Container)

#define into_iter_val(Container) MY_C_UTILS_CONCAT(Container, _into_iter_val)
#define iter_val_next(...) TEMPLATE_METHOD(iter_val, next, __VA_ARGS__)
#define iter_val_deref(...) TEMPLATE_METHOD(iter_val, deref, __VA_ARGS__)
#define iter_val_free(...) TEMPLATE_METHOD(iter_val, free, __VA_ARGS__)

#define iter_mut(Container) MY_C_UTILS_CONCAT(iter_mut_, Container)
#define ref_iter_mut(Container) MY_C_UTILS_CONCAT(ref_iter_mut_, Container)
#define cref_iter_mut(Container) MY_C_UTILS_CONCAT(cref_iter_mut_, Container)

#define iter_mut_init(Container) MY_C_UTILS_CONCAT(Container, _iter_mut)
#define iter_mut_next(...) TEMPLATE_METHOD(iter_mut, next, __VA_ARGS__)
#define iter_mut_deref(...) TEMPLATE_METHOD(iter_mut, deref, __VA_ARGS__)
#define iter_mut_free(...) TEMPLATE_METHOD(iter_mut, free, __VA_ARGS__)

#define iter_const(Container) MY_C_UTILS_CONCAT(iter_const_, Container)
#define ref_iter_const(Container) MY_C_UTILS_CONCAT(ref_iter_const_, Container)
#define cref_iter_const(Container) MY_C_UTILS_CONCAT(cref_iter_const_, Container)

#define iter_const_init(Container) MY_C_UTILS_CONCAT(Container, _iter_const)
#define iter_const_next(...) TEMPLATE_METHOD(iter_const, next, __VA_ARGS__)
#define iter_const_deref(...) TEMPLATE_METHOD(iter_const, deref, __VA_ARGS__)
#define iter_const_free(...) TEMPLATE_METHOD(iter_const, free, __VA_ARGS__)

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

#endif
