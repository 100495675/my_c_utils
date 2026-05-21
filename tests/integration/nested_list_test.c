#include "my_c_utils/my_c_utils.h"
#include <assert.h>

VECTOR_CONFIG(Int)
LINKED_LIST_CONFIG(Vector_Int)

Int main(void)
{
  List_Vector_Int list = List_Vector_Int_new();

  Vector_Int first = Vector_Int_new();
  Result r1 = Vector_Int_push_back(&first, 7);
  assert(Result_is_ok(&r1));
  Result_free(&r1);
  Result r2 = Vector_Int_push_back(&first, 8);
  assert(Result_is_ok(&r2));
  Result_free(&r2);
  List_Vector_Int_push_back(&list, first);
  first = Vector_Int_new();

  Vector_Int second = Vector_Int_new();

  Result r3 = Vector_Int_push_back(&second, 9);
  assert(Result_is_ok(&r3));
  Result_free(&r3);
  List_Vector_Int_push_back(&list, second);
  second = Vector_Int_new();

  assert(List_Vector_Int_size(&list) == 2);

  Result_Vector_Int popped = List_Vector_Int_pop_back(&list);
  assert(Result_Vector_Int_is_ok(&popped));
  Vector_Int popped_vector = Result_Vector_Int_unwrap(popped);
  assert(Vector_Int_size(&popped_vector) == 1);
  Vector_Int_free(&popped_vector);

  List_Vector_Int_free(&list);
  return 0;
}