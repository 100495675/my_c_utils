#include "my_c_utils/free.h"
#include "my_c_utils/vector.h"
#include <assert.h>

VECTOR_CONFIG(Int)

Int main(void)
{
  Vector_Int vector = Vector_Int_new();
  Result(Void, cref_Char) r1 = Vector_Int_push_back(&vector, 1);
  assert(Result_is_ok(Void, cref_Char)(&r1));

  iter_Vector_Int it = Vector_Int_into_iter(&vector);
  Result(ref_Int, cref_Char) first = iter_Vector_Int_next(&it);
  (void)Result_unwrap(ref_Int, cref_Char)(first);
  Result(ref_Int, cref_Char) exhausted = iter_Vector_Int_next(&it);
  Result_unwrap(ref_Int, cref_Char)(exhausted);

  Vector_Int_free(&vector);
  return 0;
}