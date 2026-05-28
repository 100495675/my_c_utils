#include "my_c_utils/free.h"
#include "my_c_utils/vector.h"
#include <assert.h>

VECTOR_CONFIG(Int)

Int main(void)
{
  Vector(Int) vector = Vector_new(Int)();
  Result(Void, cref(Char)) r1 = Vector_push_back(Int)(&vector, 1);
  assert(Result_is_ok(Void, cref(Char))(&r1));
  Result(Void, cref(Char)) r2 = Vector_push_back(Int)(&vector, 2);
  assert(Result_is_ok(Void, cref(Char))(&r2));

  Int seen_sum = 0;
  Size seen_count = 0;

  for_each_ref(Vector(Int), item, &vector) {
    seen_sum += ref_deref(Int)(item);
    ++seen_count;
    if (ref_deref(Int)(item) == 2)
    {
      Result(Void, cref(Char)) r3 = Vector_push_back(Int)(&vector, 3);
      assert(Result_is_ok(Void, cref(Char))(&r3));
      Result(Void, cref(Char)) r4 = Vector_push_back(Int)(&vector, 4);
      assert(Result_is_ok(Void, cref(Char))(&r4));
    }
  }

  assert(seen_count == 4);
  assert(seen_sum == 10);
  assert(Vector_size(Int)(&vector) == 4);

  Vector_free(Int)(&vector);
  return 0;
}