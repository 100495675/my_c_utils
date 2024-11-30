#include "vector.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

typedef char *String;

void my_free_string(String s) { free(s); }

VECTOR_CONFIG(String, my_free_sring)

int main() {
  Vector_String v = vector_String_new();

  result_unwrap(vector_String_reserve(&v, 3));

  result_unwrap(vector_String_push_back(&v, strdup("string1")));
  result_unwrap(vector_String_push_back(&v, strdup("string2")));
  result_unwrap(vector_String_push_back(&v, strdup("string3")));

  for (Vector_String_iter it = vector_String_iter(&v);
       result_String_is_ok(vector_String_iter_deref(&it));
       vector_String_iter_next(&it)) {

    printf("%s\n", result_String_unwrap(vector_String_iter_deref(&it)));
  }

  printf("Element at index 1: %s\n",
         *result_String_ref_unwrap(vector_String_at(&v, 1)));

  vector_String_free(&v);
  return 0;
}