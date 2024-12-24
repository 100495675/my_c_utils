#include "linked_list.h"
#include "result.h"
#include <stdio.h>
#include <stdlib.h>

void Int_free(int *value) {
  printf("Freeing %d\n", *value);
  return;
}

typedef int Int;

LINKED_LIST_CONFIG(Int)

int main() {
  List_Int ll = List_Int_new();
  List_Int_push_back(&ll, 10);
  List_Int_push_back(&ll, 20);
  List_Int_push_back(&ll, 30);

  Result_Int result = List_Int_pop_back(&ll);
  if (Result_Int_is_ok(result)) {
    printf("Popped value: %d\n", Result_Int_unwrap(result));
  }

  List_Int_debug(&ll);

  List_Int_free(&ll);

  return 0;
}