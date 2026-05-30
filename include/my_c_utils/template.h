#ifndef MY_C_UTILS_TEMPLATE_H
#define MY_C_UTILS_TEMPLATE_H

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

#endif // MY_C_UTILS_TEMPLATE_H
