#include "my_c_utils/free.h"
#include "my_c_utils/linked_list.h"
#include "my_c_utils/vector.h"
#include <assert.h>

VECTOR_CONFIG(Int)
typedef Vector_Int Vector_Int_list;
static inline void Vector_Int_list_free(Vector_Int_list *value)
{
  Vector_Int_free(value);
}

RESULT_CONFIG(Vector_Int_list)

LINKED_LIST_CONFIG(Vector_Int_list)

Int main(void)
{
  List_Vector_Int_list list = List_Vector_Int_list_new();

  Vector_Int first = Vector_Int_new();
  assert(Result_is_ok(Vector_Int_push_back(&first, 7)));
  assert(Result_is_ok(Vector_Int_push_back(&first, 8)));
  List_Vector_Int_list_push_back(&list, first);
  first = Vector_Int_new();

  Vector_Int second = Vector_Int_new();
  assert(Result_is_ok(Vector_Int_push_back(&second, 9)));
  List_Vector_Int_list_push_back(&list, second);
  second = Vector_Int_new();

  assert(List_Vector_Int_list_size(&list) == 2);

  Result_Vector_Int_list popped = List_Vector_Int_list_pop_back(&list);
  assert(Result_Vector_Int_list_is_ok(popped));
  Vector_Int_list popped_vector = Result_Vector_Int_list_unwrap(popped);
  assert(Vector_Int_size(&popped_vector) == 1);
  Vector_Int_list_free(&popped_vector);

  List_Vector_Int_list_free(&list);
  return 0;
}