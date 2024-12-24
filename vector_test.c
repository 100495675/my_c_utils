#include "linked_list.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef char Char;

void Char_free(Char value) {
  printf("Freeing %c\n", value);
  return;
}

VECTOR_CONFIG(Char)
LINKED_LIST_CONFIG(Vector_Char)

int main() {
  Vector_Char s1 = Vector_Char_new();
  Vector_Char_push_back(&s1, 'a');
  Vector_Char_push_back(&s1, 'b');
  Vector_Char_push_back(&s1, 'c');
  Vector_Char_push_back(&s1, 'd');

  Vector_Char s2 = Vector_Char_new();
  Vector_Char_push_back(&s2, 'x');
  Vector_Char_push_back(&s2, 'y');
  Vector_Char_push_back(&s2, 'z');

  List_Vector_Char ll = List_Vector_Char_new();
  List_Vector_Char_push_back(&ll, s1);
  List_Vector_Char_push_back(&ll, s2);

  Result_Vector_Char result = List_Vector_Char_pop_back(&ll);
  if (Result_Vector_Char_is_ok(result)) {
    Vector_Char popped = Result_Vector_Char_unwrap(result);
    Vector_Char_debug(&popped);
  }

  vector_for(Char, it, &s1, printf("  %c,\n", *it););

  vector_for(Char, it, &s2, printf("  %c,\n", *it););

  List_Vector_Char_free(&ll);

  return 0;
}