#ifndef MY_C_UTILS_FREE_H
#define MY_C_UTILS_FREE_H

#include "my_c_utils/tipos.h"
// Unicamente no se usa el trivial_free es para structs con punteros,
// los punteros prestados (borrowed ref) a una variable que tiene dueño no necesitan ser liberados
#define TRIVIAL_FREE(Type)                      \
    static inline void Type##_free(Type *value) \
    {                                           \
        (void)value;                            \
    }

TRIVIAL_FREE(Bool)
TRIVIAL_FREE(Char)
TRIVIAL_FREE(SChar)
TRIVIAL_FREE(UChar)
TRIVIAL_FREE(Short)
TRIVIAL_FREE(UShort)
TRIVIAL_FREE(Int)
TRIVIAL_FREE(UInt)
TRIVIAL_FREE(Long)
TRIVIAL_FREE(ULong)
TRIVIAL_FREE(LongLong)
TRIVIAL_FREE(ULongLong)
TRIVIAL_FREE(Float)
TRIVIAL_FREE(Double)
TRIVIAL_FREE(LongDouble)
TRIVIAL_FREE(WChar)

TRIVIAL_FREE(I8)
TRIVIAL_FREE(U8)
TRIVIAL_FREE(I16)
TRIVIAL_FREE(U16)
TRIVIAL_FREE(I32)
TRIVIAL_FREE(U32)
TRIVIAL_FREE(I64)
TRIVIAL_FREE(U64)
TRIVIAL_FREE(IntPtr)
TRIVIAL_FREE(UIntPtr)
TRIVIAL_FREE(IntMax)
TRIVIAL_FREE(UIntMax)
TRIVIAL_FREE(PtrDiff)
TRIVIAL_FREE(Size)

#undef TRIVIAL_FREE

#endif