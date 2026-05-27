#include "my_c_utils/my_c_utils.h"
#include <assert.h>

VECTOR_CONFIG(Int)
LINKED_LIST_CONFIG(Vector(Int))

Int main(void)
{
  List(Vector(Int)) list = List_new(Vector(Int))();

  Vector(Int) first = Vector_new(Int)();
  Result(Void, cref_Char) r1 = Vector_push_back(Int)(&first, 7);
  assert(Result_is_ok(Void, cref_Char)(&r1));
  Result_free(Void, cref_Char)(&r1);
  Result(Void, cref_Char) r2 = Vector_push_back(Int)(&first, 8);
  assert(Result_is_ok(Void, cref_Char)(&r2));
  Result_free(Void, cref_Char)(&r2);
  List_push_back(Vector(Int))(&list, first);
  first = Vector_new(Int)();

  Vector(Int) second = Vector_new(Int)();

  Result(Void, cref_Char) r3 = Vector_push_back(Int)(&second, 9);
  assert(Result_is_ok(Void, cref_Char)(&r3));
  Result_free(Void, cref_Char)(&r3);
  List_push_back(Vector(Int))(&list, second);
  second = Vector_new(Int)();

  assert(List_size(Vector(Int))(&list) == 2);

  Result(Vector(Int), cref_Char) popped = List_pop_back(Vector(Int))(&list);
  assert(Result_is_ok(Vector(Int), cref_Char)(&popped));
  Vector(Int) popped_vector = Result_unwrap(Vector(Int), cref_Char)(popped);
  assert(Vector_size(Int)(&popped_vector) == 1);
  Vector_free(Int)(&popped_vector);

  List_free(Vector(Int))(&list);
  return 0;
}