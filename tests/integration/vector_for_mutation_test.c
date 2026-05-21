#include "my_c_utils/free.h"
#include "my_c_utils/vector.h"
#include <assert.h>

VECTOR_CONFIG(Int)

Int main(void)
{
  Vector_Int vector = Vector_Int_new();
  Result r1 = Vector_Int_push_back(&vector, 1);
  assert(Result_is_ok(&r1));
  Result r2 = Vector_Int_push_back(&vector, 2);
  assert(Result_is_ok(&r2));

  Int seen_sum = 0;
  Size seen_count = 0;

  for_each_ref(Int, item, Vector, &vector, {
    seen_sum += *item;
    ++seen_count;
    if (*item == 2)
    {
      Result r3 = Vector_Int_push_back(&vector, 3);
      assert(Result_is_ok(&r3));
      Result r4 = Vector_Int_push_back(&vector, 4);
      assert(Result_is_ok(&r4));
    }
  });

  assert(seen_count == 4);
  assert(seen_sum == 10);
  assert(Vector_Int_size(&vector) == 4);

  Vector_Int_free(&vector);
  return 0;
}