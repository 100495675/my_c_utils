#include "my_c_utils/free.h"
#include "my_c_utils/vector.h"
#include <assert.h>

VECTOR_CONFIG(Int)
RESULT_CONFIG(cref(Vector(Int)), cref(Char))
VECTOR_CONFIG(Vector(Int))

static void assert_nested_iteration(const Vector(Vector(Int)) *outer,
                                    Size expected_count,
                                    Int expected_sum)
{
  Size count = 0;
  Int sum = 0;

  Vector(Vector(Int)) outer_clone = Vector_clone(Vector(Int))(outer);
  for_each_ref(Vector(Vector(Int)), inner, &outer_clone) {
    Vector(Int) inner_clone = Vector_clone(Int)(inner);
    for_each_ref(Vector(Int), item, &inner_clone) {
      sum += ref_deref(Int)(item);
      ++count;
    }
  }

  assert(count == expected_count);
  assert(sum == expected_sum);
}

Int main(void)
{
  Vector(Vector(Int)) outer = Vector_new(Vector(Int))();

  Vector(Int) first = Vector_new(Int)();
  Result(Void, cref(Char)) r1 = Vector_push_back(Int)(&first, 1);
  assert(Result_is_ok(Void, cref(Char))(&r1));
  Result(Void, cref(Char)) r2 = Vector_push_back(Int)(&first, 2);
  assert(Result_is_ok(Void, cref(Char))(&r2));
  
  Result(Void, cref(Char)) r3 = Vector_push_back(Vector(Int))(&outer, first);
  assert(Result_is_ok(Void, cref(Char))(&r3));
  first = Vector_new(Int)();

  
  Vector(Int) second = Vector_new(Int)();
  Result(Void, cref(Char)) r4 = Vector_push_back(Int)(&second, 3);
  assert(Result_is_ok(Void, cref(Char))(&r4));

  Result(Void, cref(Char)) r5 = Vector_push_back(Vector(Int))(&outer, second);
  assert(Result_is_ok(Void, cref(Char))(&r5));
  second = Vector_new(Int)();

  assert_nested_iteration(&outer, 3, 6);

  Vector_free(Vector(Int))(&outer);
  return 0;
}