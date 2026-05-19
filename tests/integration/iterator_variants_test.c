#include "my_c_utils/free.h"
#include "my_c_utils/vector.h"
#include <assert.h>


VECTOR_CONFIG(Int)

Int main(void)
{
  Vector_Int vector = Vector_Int_new();

  assert(Result_is_ok(Vector_Int_push_back(&vector, 10)));
  assert(Result_is_ok(Vector_Int_push_back(&vector, 20)));
  assert(Result_is_ok(Vector_Int_push_back(&vector, 30)));

  Int sum_ref = 0;
  for_each_ref(Int, item, Vector, &vector, {
    sum_ref += *item;
  });
  assert(sum_ref == 60);

  Int sum_copy = 0;
  for_each_copy(Int, item, Vector, &vector, {
    sum_copy += item;
  });
  assert(sum_copy == 60);

  Int sum_compat = 0;
  for_each_ref(Int, item, Vector, &vector, {
    sum_compat += *item;
  });
  assert(sum_compat == 60);

  Vector_Int_free(&vector);
  return 0;
}