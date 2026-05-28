#ifndef MY_C_UTILS_STRING_TYPE_H
#define MY_C_UTILS_STRING_TYPE_H

#include <string.h>
#include "my_c_utils/allocator.h"
#include "my_c_utils/tipos.h"
#include "my_c_utils/result.h"

typedef Char *String;
REF_EXPAND(String)

static inline String string_dup(const Char *text)
{
    if (!text)
    {
        return NULL;
    }
    Size length = strlen(text);
    String copy = (String)MY_C_UTILS_MALLOC(length + 1);
    if (!copy)
    {
        return NULL;
    }
    memcpy(copy, text, length + 1);
    return copy;
}

static inline Size String_hash(String key)
{
    Size hash = 2166136261U;
    if (key)
    {
        while (*key)
        {
            hash ^= (unsigned char)*key++;
            hash *= 16777619U;
        }
    }
    return hash;
}

static inline Bool String_equals(String a, String b)
{
    if (a == b)
    {
        return true;
    }
    if (!a || !b)
    {
        return false;
    }
    return strcmp(a, b) == 0;
}

static inline void String_free(String *value)
{
    if (value && *value)
    {
        MY_C_UTILS_FREE(*value);
        *value = NULL;
    }
}

RESULT_CONFIG(String, cref(Char))
RESULT_CONFIG(ref_String, cref(Char))

static inline String String_clone(const String *src)
{
    if (!src || !*src)
    {
        return NULL;
    }
    return string_dup(*src);
}

#endif // MY_C_UTILS_STRING_TYPE_H
