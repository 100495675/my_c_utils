#include "my_c_utils/free.h"
#include "my_c_utils/vector.h"
#include <assert.h>

VECTOR_CONFIG(Int)
VECTOR_CONFIG(Vector_Int)

static void assert_nested_iteration(const Vector_Vector_Int *outer,
                                    Size expected_count,
                                    Int expected_sum)
{
  Size count = 0;
  Int sum = 0;

  for_each_ref(Vector_Int, inner, Vector, outer, {
    for_each_ref(Int, item, Vector, inner, {
      sum += *item;
      ++count;
    });
  });

  assert(count == expected_count);
  assert(sum == expected_sum);
}

Int main(void)
{
  Vector_Vector_Int outer = Vector_Vector_Int_new();

  Vector_Int first = Vector_Int_new();
  assert(Result_is_ok(Vector_Int_push_back(&first, 1)));
  assert(Result_is_ok(Vector_Int_push_back(&first, 2)));
  assert(Result_is_ok(Vector_Vector_Int_push_back(&outer, first)));
  first = Vector_Int_new();

  Vector_Int second = Vector_Int_new();
  assert(Result_is_ok(Vector_Int_push_back(&second, 3)));
  assert(Result_is_ok(Vector_Vector_Int_push_back(&outer, second)));
  second = Vector_Int_new();

  assert_nested_iteration(&outer, 3, 6);

  Vector_Vector_Int_free(&outer);
  return 0;
}