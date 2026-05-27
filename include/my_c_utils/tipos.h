#ifndef MY_C_UTILS_TIPOS_H
#define MY_C_UTILS_TIPOS_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <wchar.h>
#include "my_c_utils/ref.h"

typedef bool Bool;
REF_EXPAND(Bool)
typedef char Char;
REF_EXPAND(Char)
typedef signed char SChar;
REF_EXPAND(SChar)
typedef unsigned char UChar;
REF_EXPAND(UChar)
typedef short Short;
REF_EXPAND(Short)
typedef unsigned short UShort;
REF_EXPAND(UShort)
typedef int Int;
REF_EXPAND(Int)
typedef unsigned int UInt;
REF_EXPAND(UInt)
typedef long Long;
REF_EXPAND(Long)
typedef unsigned long ULong;
REF_EXPAND(ULong)
typedef long long LongLong;
REF_EXPAND(LongLong)
typedef unsigned long long ULongLong;
REF_EXPAND(ULongLong)
typedef float Float;
REF_EXPAND(Float)
typedef double Double;
REF_EXPAND(Double)
typedef long double LongDouble;
REF_EXPAND(LongDouble)
typedef wchar_t WChar;
REF_EXPAND(WChar)

typedef int8_t I8;
REF_EXPAND(I8)
typedef uint8_t U8;
REF_EXPAND(U8)
typedef int16_t I16;
REF_EXPAND(I16)
typedef uint16_t U16;
REF_EXPAND(U16)
typedef int32_t I32;
REF_EXPAND(I32)
typedef uint32_t U32;
REF_EXPAND(U32)
typedef int64_t I64;
REF_EXPAND(I64)
typedef uint64_t U64;
REF_EXPAND(U64)
typedef intptr_t IntPtr;
REF_EXPAND(IntPtr)
typedef uintptr_t UIntPtr;
REF_EXPAND(UIntPtr)
typedef intmax_t IntMax;
REF_EXPAND(IntMax)
typedef uintmax_t UIntMax;
REF_EXPAND(UIntMax)
typedef ptrdiff_t PtrDiff;
REF_EXPAND(PtrDiff)
typedef size_t Size;
REF_EXPAND(Size)

#define MY_C_UTILS_CONCAT_IMPL(a, b) a##b
#define MY_C_UTILS_CONCAT(a, b) MY_C_UTILS_CONCAT_IMPL(a, b)

#endif