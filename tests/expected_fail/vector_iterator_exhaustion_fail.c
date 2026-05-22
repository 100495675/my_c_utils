#include "my_c_utils/free.h"
#include "my_c_utils/vector.h"
#include <assert.h>

VECTOR_CONFIG(Int)

Int main(void)
{
  Vector_Int vector = Vector_Int_new();
  Result_Void r1 = Vector_Int_push_back(&vector, 1);
  assert(Result_Void_is_ok(&r1));

  iter_Vector_Int it = Vector_Int_into_iter(&vector);
  Result_Void_ref_Int first = iter_Vector_Int_next(&it);
  (void)Result_Void_ref_Int_unwrap(first);
  Result_Void_ref_Int exhausted = iter_Vector_Int_next(&it);
  Result_Void_ref_Int_unwrap(exhausted);

  Vector_Int_free(&vector);
  return 0;
}