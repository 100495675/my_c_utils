#include "my_c_utils/free.h"
#include "my_c_utils/vector.h"
#include <assert.h>

VECTOR_CONFIG(Int)

Int main(void)
{
  Vector_Int vector = Vector_Int_new();

  Vector_Int_iter it = Vector_Int_into_iter(&vector);
  Result_Int_ref out = Vector_Int_iter_next(&it);
  assert(Result_Int_ref_is_err(out));

  assert(Result_is_ok(Vector_Int_push_back(&vector, 1)));
  it = Vector_Int_into_iter(&vector);
  out = Vector_Int_iter_next(&it);
  assert(Result_Int_ref_is_ok(out));
  assert(*Result_Int_ref_unwrap(out) == 1);
  out = Vector_Int_iter_next(&it);
  assert(Result_Int_ref_is_err(out));

  Vector_Int_free(&vector);
  return 0;
}
