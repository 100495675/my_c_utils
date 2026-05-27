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

#define MY_C_UTILS_STRUCT_FIELD_DECLARE(FieldType, FieldName) FieldType FieldName;
#define MY_C_UTILS_STRUCT_FIELD_FREE(FieldType, FieldName) FieldType##_free(&self->FieldName);
#define MY_C_UTILS_STRUCT_FIELD_CLONE(FieldType, FieldName) dest.FieldName = FieldType##_clone(&src->FieldName);

#define FREE_CONFIG(Type, ...)                                                 \
    static inline void Type##_free(ref_##Type self)                            \
    {                                                                          \
        if (!self)                                                             \
            return;                                                            \
        MY_C_UTILS_FOR_EACH_FIELD_2(MY_C_UTILS_STRUCT_FIELD_FREE, __VA_ARGS__) \
    }

#define CLONE_CONFIG(Type, ...)                                                 \
    static inline Type Type##_clone(cref_##Type src)                            \
    {                                                                           \
        if (!src)                                                               \
        {                                                                       \
            perror("Cannot clone NULL pointer");                                \
            exit(1);                                                            \
        }                                                                       \
        Type dest = {0};                                                        \
        MY_C_UTILS_FOR_EACH_FIELD_2(MY_C_UTILS_STRUCT_FIELD_CLONE, __VA_ARGS__) \
        return dest;                                                            \
    }

#define STRUCT_CONFIG(Type, ...)                                                  \
    typedef struct                                                                \
    {                                                                             \
        MY_C_UTILS_FOR_EACH_FIELD_2(MY_C_UTILS_STRUCT_FIELD_DECLARE, __VA_ARGS__) \
    } Type;                                                                       \
    REF_EXPAND(Type)                                                              \
    FREE_CONFIG(Type, __VA_ARGS__)                                                \
    RESULT_CONFIG(Type, cref_Char)                                                           \
    RESULT_CONFIG(ref_##Type, cref_Char)                                                     \
    CLONE_CONFIG(Type, __VA_ARGS__)

#endif