#include "my_c_utils/free.h"
#include "my_c_utils/vector.h"
#include <assert.h>


VECTOR_CONFIG(Int)

Int main(void)
{
  Vector(Int) vector = Vector_new(Int)();

  Result(Void, cref(Char)) r1 = Vector_push_back(Int)(&vector, 10);
  assert(Result_is_ok(Void, cref(Char))(&r1));
  Result_free(Void, cref(Char))(&r1);
  Result(Void, cref(Char)) r2 = Vector_push_back(Int)(&vector, 20);
  assert(Result_is_ok(Void, cref(Char))(&r2));
  Result_free(Void, cref(Char))(&r2);
  Result(Void, cref(Char)) r3 = Vector_push_back(Int)(&vector, 30);
  assert(Result_is_ok(Void, cref(Char))(&r3));
  Result_free(Void, cref(Char))(&r3);


  Int sum_ref = 0;
  Vector(Int) clone1 = Vector_clone(Int)(&vector);
  for_each_ref(Vector(Int), item, &clone1) {
    sum_ref += ref_deref(Int)(item);
  }
  assert(sum_ref == 60);
  Free(Int)(&sum_ref);

  Int sum_copy = 0;
  Vector(Int) clone2 = Vector_clone(Int)(&vector);
  for_each(Vector(Int), item, &clone2) {
    sum_copy += item;
  }
  assert(sum_copy == 60);

  Int sum_compat = 0;
  for_each_ref(Vector(Int), item, &vector) {
    sum_compat += ref_deref(Int)(item);
  }
  assert(sum_compat == 60);

  return 0;
}