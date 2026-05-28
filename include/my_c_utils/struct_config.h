#ifndef MY_C_UTILS_STRUCT_CONFIG_H
#define MY_C_UTILS_STRUCT_CONFIG_H

#include <stdio.h>
#include <stdlib.h>

#include "my_c_utils/free.h"
#include "my_c_utils/result.h"

#define MY_C_UTILS_PP_CAT(a, b) MY_C_UTILS_PP_CAT_I(a, b)
#define MY_C_UTILS_PP_CAT_I(a, b) a##b

#define MY_C_UTILS_NARGS(...) MY_C_UTILS_NARGS_I(__VA_ARGS__, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1, 0)
#define MY_C_UTILS_NARGS_I(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, N, ...) N

#define MY_C_UTILS_FOR_EACH_FIELD_2(Macro, ...) \
    MY_C_UTILS_PP_CAT(MY_C_UTILS_FOR_EACH_FIELD_2_, MY_C_UTILS_NARGS(__VA_ARGS__))(Macro, __VA_ARGS__)

#define MY_C_UTILS_FOR_EACH_FIELD_2_0(Macro, ...)
#define MY_C_UTILS_FOR_EACH_FIELD_2_2(Macro, T1, N1) Macro(T1, N1)
#define MY_C_UTILS_FOR_EACH_FIELD_2_4(Macro, T1, N1, T2, N2) \
    Macro(T1, N1)                                            \
        Macro(T2, N2)
#define MY_C_UTILS_FOR_EACH_FIELD_2_6(Macro, T1, N1, T2, N2, T3, N3) \
    Macro(T1, N1)                                                    \
        Macro(T2, N2)                                                \
            Macro(T3, N3)
#define MY_C_UTILS_FOR_EACH_FIELD_2_8(Macro, T1, N1, T2, N2, T3, N3, T4, N4) \
    Macro(T1, N1)                                                            \
        Macro(T2, N2)                                                        \
            Macro(T3, N3)                                                    \
                Macro(T4, N4)
#define MY_C_UTILS_FOR_EACH_FIELD_2_10(Macro, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5) \
    Macro(T1, N1)                                                                     \
        Macro(T2, N2)                                                                 \
            Macro(T3, N3)                                                             \
                Macro(T4, N4)                                                         \
                    Macro(T5, N5)
#define MY_C_UTILS_FOR_EACH_FIELD_2_12(Macro, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6) \
    Macro(T1, N1)                                                                             \
        Macro(T2, N2)                                                                         \
            Macro(T3, N3)                                                                     \
                Macro(T4, N4)                                                                 \
                    Macro(T5, N5)                                                             \
                        Macro(T6, N6)
#define MY_C_UTILS_FOR_EACH_FIELD_2_14(Macro, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
    Macro(T1, N1)                                                                                     \
        Macro(T2, N2)                                                                                 \
            Macro(T3, N3)                                                                             \
                Macro(T4, N4)                                                                         \
                    Macro(T5, N5)                                                                     \
                        Macro(T6, N6)                                                                 \
                            Macro(T7, N7)
#define MY_C_UTILS_FOR_EACH_FIELD_2_16(Macro, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8) \
    Macro(T1, N1)                                                                                             \
        Macro(T2, N2)                                                                                         \
            Macro(T3, N3)                                                                                     \
                Macro(T4, N4)                                                                                 \
                    Macro(T5, N5)                                                                             \
                        Macro(T6, N6)                                                                         \
                            Macro(T7, N7)                                                                     \
                                Macro(T8, N8)

#define MY_C_UTILS_FOR_EACH_PARAM_0(Macro, ...)
#define MY_C_UTILS_FOR_EACH_PARAM_2(Macro, T1, N1) Macro(T1, N1)
#define MY_C_UTILS_FOR_EACH_PARAM_4(Macro, T1, N1, T2, N2) \
    Macro(T1, N1), Macro(T2, N2)
#define MY_C_UTILS_FOR_EACH_PARAM_6(Macro, T1, N1, T2, N2, T3, N3) \
    Macro(T1, N1), Macro(T2, N2), Macro(T3, N3)
#define MY_C_UTILS_FOR_EACH_PARAM_8(Macro, T1, N1, T2, N2, T3, N3, T4, N4) \
    Macro(T1, N1), Macro(T2, N2), Macro(T3, N3), Macro(T4, N4)
#define MY_C_UTILS_FOR_EACH_PARAM_10(Macro, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5) \
    Macro(T1, N1), Macro(T2, N2), Macro(T3, N3), Macro(T4, N4), Macro(T5, N5)
#define MY_C_UTILS_FOR_EACH_PARAM_12(Macro, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6) \
    Macro(T1, N1), Macro(T2, N2), Macro(T3, N3), Macro(T4, N4), Macro(T5, N5), Macro(T6, N6)
#define MY_C_UTILS_FOR_EACH_PARAM_14(Macro, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7) \
    Macro(T1, N1), Macro(T2, N2), Macro(T3, N3), Macro(T4, N4), Macro(T5, N5), Macro(T6, N6), Macro(T7, N7)
#define MY_C_UTILS_FOR_EACH_PARAM_16(Macro, T1, N1, T2, N2, T3, N3, T4, N4, T5, N5, T6, N6, T7, N7, T8, N8) \
    Macro(T1, N1), Macro(T2, N2), Macro(T3, N3), Macro(T4, N4), Macro(T5, N5), Macro(T6, N6), Macro(T7, N7), Macro(T8, N8)

#define MY_C_UTILS_FOR_EACH_PARAM(Macro, ...) \
    MY_C_UTILS_PP_CAT(MY_C_UTILS_FOR_EACH_PARAM_, MY_C_UTILS_NARGS(__VA_ARGS__))(Macro, __VA_ARGS__)

#define MY_C_UTILS_STRUCT_PARAM_DECLARE(FieldT, FieldName) FieldT FieldName
#define MY_C_UTILS_STRUCT_MEMBER_ASSIGN(FieldT, FieldName) .FieldName = FieldName

#define MY_C_UTILS_STRUCT_FIELD_DECLARE(FieldT, FieldName) FieldT FieldName;
#define MY_C_UTILS_STRUCT_FIELD_FREE(FieldT, FieldName) MY_C_UTILS_CONCAT(FieldT, _free)(&self->FieldName);
#define MY_C_UTILS_STRUCT_FIELD_CLONE(FieldT, FieldName) dest.FieldName = MY_C_UTILS_CONCAT(FieldT, _clone)(&src->FieldName);

#define NEW_CONFIG(T, ...) \
    static inline T MY_C_UTILS_CONCAT(T, _new)(MY_C_UTILS_FOR_EACH_PARAM(MY_C_UTILS_STRUCT_PARAM_DECLARE, __VA_ARGS__)) \
    { \
        return (T){ MY_C_UTILS_FOR_EACH_PARAM(MY_C_UTILS_STRUCT_MEMBER_ASSIGN, __VA_ARGS__) }; \
    }

#define FREE_CONFIG(T, ...)                                                 \
    static inline void MY_C_UTILS_CONCAT(T, _free)(ref(T) self)         \
    {                                                                          \
        if (!self)                                                             \
            return;                                                            \
        MY_C_UTILS_FOR_EACH_FIELD_2(MY_C_UTILS_STRUCT_FIELD_FREE, __VA_ARGS__) \
    }

#define CLONE_CONFIG(T, ...)                                                 \
    static inline T MY_C_UTILS_CONCAT(T, _clone)(cref(T) src)         \
    {                                                                           \
        if (!src)                                                               \
        {                                                                       \
            perror("Cannot clone NULL pointer");                                \
            exit(1); \
        } \
        T dest = {0};                                                        \
        MY_C_UTILS_FOR_EACH_FIELD_2(MY_C_UTILS_STRUCT_FIELD_CLONE, __VA_ARGS__) \
        return dest;                                                            \
    }

#define STRUCT_CONFIG(T, ...)                                                  \
    typedef struct                                                                \
    {                                                                             \
        MY_C_UTILS_FOR_EACH_FIELD_2(MY_C_UTILS_STRUCT_FIELD_DECLARE, __VA_ARGS__) \
    } T;                                                                       \
    REF_EXPAND(T)                                                              \
    FREE_CONFIG(T, __VA_ARGS__)                                                \
    RESULT_CONFIG(T, cref(Char))                                                \
    RESULT_CONFIG(ref(T), cref(Char))                                          \
    CLONE_CONFIG(T, __VA_ARGS__)                                               \
    NEW_CONFIG(T, __VA_ARGS__)

#endif