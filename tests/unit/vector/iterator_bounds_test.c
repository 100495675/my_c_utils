#include "my_c_utils/free.h"
#include "my_c_utils/vector.h"
#include <assert.h>

VECTOR_CONFIG(Int)

Int main(void)
{
  Vector_Int vector = Vector_Int_new();

  iter_Vector_Int it = Vector_Int_into_iter(&vector);
  Result_ref_Int out = iter_Vector_Int_next(&it);
  assert(Result_ref_Int_is_err(&out));

  Result r1 = Vector_Int_push_back(&vector, 1);
  assert(Result_is_ok(&r1));
  iter_Vector_Int it2 = Vector_Int_into_iter(&vector);
  Result_ref_Int out2 = iter_Vector_Int_next(&it2);
  assert(Result_ref_Int_is_ok(&out2));
  assert(*Result_ref_Int_unwrap(out2) == 1);
  Result_ref_Int out3 = iter_Vector_Int_next(&it2);
  assert(Result_ref_Int_is_err(&out3));

  Vector_Int_free(&vector);
  return 0;
}
