#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char *String;

void my_free_string(String s) {
  printf("Freeing %s\n", s);
  free(s);
}

VECTOR_CONFIG(String, my_free_string)

int main() {
  Vector_String v = vector_String_new();

  result_unwrap(vector_String_push_back(&v, strdup("string1")));
  result_unwrap(vector_String_push_back(&v, strdup("string2")));
  result_unwrap(vector_String_push_back(&v, strdup("string3")));

  vector_for(String, x, &v) printf("%s\n", *x);
}

vector_String_free(&v);
return 0;
}