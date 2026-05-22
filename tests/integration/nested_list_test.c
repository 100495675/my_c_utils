#include "my_c_utils/my_c_utils.h"
#include <assert.h>

VECTOR_CONFIG(Int)
LINKED_LIST_CONFIG(Vector_Int)

Int main(void)
{
  List_Vector_Int list = List_Vector_Int_new();

  Vector_Int first = Vector_Int_new();
  Result_Void r1 = Vector_Int_push_back(&first, 7);
  assert(Result_Void_is_ok(&r1));
  Result_Void_free(&r1);
  Result_Void r2 = Vector_Int_push_back(&first, 8);
  assert(Result_Void_is_ok(&r2));
  Result_Void_free(&r2);
  List_Vector_Int_push_back(&list, first);
  first = Vector_Int_new();

  Vector_Int second = Vector_Int_new();

  Result_Void r3 = Vector_Int_push_back(&second, 9);
  assert(Result_Void_is_ok(&r3));
  Result_Void_free(&r3);
  List_Vector_Int_push_back(&list, second);
  second = Vector_Int_new();

  assert(List_Vector_Int_size(&list) == 2);

  Result_Void_Vector_Int popped = List_Vector_Int_pop_back(&list);
  assert(Result_Void_Vector_Int_is_ok(&popped));
  Vector_Int popped_vector = Result_Void_Vector_Int_unwrap(popped);
  assert(Vector_Int_size(&popped_vector) == 1);
  Vector_Int_free(&popped_vector);

  List_Vector_Int_free(&list);
  return 0;
}