#include "my_c_utils/vector.h"
#include "my_c_utils/free.h"
#include <assert.h>

VECTOR_CONFIG(Int)

Int main(void) {
  Vector_Int vector = Vector_Int_new();
  assert(Result_is_ok(Vector_Int_push_back(&vector, 1)));
  assert(Result_is_ok(Vector_Int_push_back(&vector, 2)));

  Int seen_sum = 0;
  Size seen_count = 0;

  for_each_ref(Int, item, Vector, &vector, {
    seen_sum += *item;
    ++seen_count;
    if (*item == 2) {
      assert(Result_is_ok(Vector_Int_push_back(&vector, 3)));
      assert(Result_is_ok(Vector_Int_push_back(&vector, 4)));
    }
  });

  assert(seen_count == 4);
  assert(seen_sum == 10);
  assert(Vector_Int_size(&vector) == 4);

  Vector_Int_free(&vector);
  return 0;
}
