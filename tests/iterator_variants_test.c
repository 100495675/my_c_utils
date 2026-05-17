#include "my_c_utils/vector.h"
#include "my_c_utils/free.h"
#include <assert.h>

VECTOR_CONFIG(Int)

Int main()
{
  Vector_Int vector = Vector_Int_new();
  
  assert(Result_is_ok(Vector_Int_push_back(&vector, 10)));
  assert(Result_is_ok(Vector_Int_push_back(&vector, 20)));
  assert(Result_is_ok(Vector_Int_push_back(&vector, 30)));

  // Test for_each_ref (pointer version)
  Int sum_ref = 0;
  for_each_ref(Int, item, Vector, &vector, {
    sum_ref += *item;  // Must dereference pointer
  });
  assert(sum_ref == 60);

  // Test for_each_copy (value copy version)
  Int sum_copy = 0;
  for_each_copy(Int, item, Vector, &vector, {
    sum_copy += item;  // Direct value, no dereference
  });
  assert(sum_copy == 60);

  // Test backward compatibility: for_each_ref works like before
  Int sum_compat = 0;
  for_each_ref(Int, item, Vector, &vector, {
    sum_compat += *item;  // Must dereference (ref version)
  });
  assert(sum_compat == 60);

  Vector_Int_free(&vector);
  return 0;
}
