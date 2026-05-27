#include "my_c_utils/free.h"
#include "my_c_utils/vector.h"
#include <assert.h>

VECTOR_CONFIG(Int)

Int main(void)
{
  Vector(Int) vector = Vector_new(Int)();
  Result(Void, cref_Char) r1 = Vector_push_back(Int)(&vector, 1);
  assert(Result_is_ok(Void, cref_Char)(&r1));

  iter(Vector(Int)) it = into_iter(Vector(Int))(&vector);
  Result(ref_Int, cref_Char) first = iter_next(Vector(Int))(&it);
  (void)Result_unwrap(ref_Int, cref_Char)(first);
  Result(ref_Int, cref_Char) exhausted = iter_next(Vector(Int))(&it);
  Result_unwrap(ref_Int, cref_Char)(exhausted);

  Vector_free(Int)(&vector);
  return 0;
}