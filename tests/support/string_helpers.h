#ifndef MY_C_UTILS_TESTS_STRING_HELPERS_H
#define MY_C_UTILS_TESTS_STRING_HELPERS_H

#include "my_c_utils/free.h"
#include "my_c_utils/result.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef Char *String;
REF_EXPAND(String)

static inline String string_dup(const Char *text)
{
  Size length = strlen(text);
  String copy = (String)malloc(length + 1);
  assert(copy != NULL);
  for (Size i = 0; i <= length; ++i)
  {
    copy[i] = text[i];
  }
  return copy;
}

static inline Size String_hash(String key)
{
  Size hash = 0;
  while (*key)
  {
    hash = (hash * 31) + (unsigned char)*key++;
  }
  return hash;
}

static inline Bool String_equals(String a, String b)
{
  return strcmp(a, b) == 0;
}

static inline void String_free(String *value)
{
  free(*value);
}

RESULT_CONFIG(String)

static inline String String_clone(const String *src)
{
  if (!src)
  {
    perror("Cannot clone NULL pointer");
    exit(1);
  }
  return string_dup(*src);
}

#endif