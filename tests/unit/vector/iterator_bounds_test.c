#include "my_c_utils/free.h"
#include "my_c_utils/vector.h"
#include <assert.h>

VECTOR_CONFIG(Int)

Int main(void)
{
  Vector(Int) vector = Vector_new(Int)();

  iter(Vector(Int)) it = into_iter(Vector(Int))(Vector_clone(Int)(&vector));
  Result(ref_Int, cref(Char)) out = iter_next(Vector(Int))(&it);
  assert(Result_is_err(ref_Int, cref(Char))(&out));
  iter_free(Vector(Int))(&it);

  Result(Void, cref(Char)) r1 = Vector_push_back(Int)(&vector, 1);
  assert(Result_is_ok(Void, cref(Char))(&r1));
  iter(Vector(Int)) it2 = into_iter(Vector(Int))(vector);
  Result(ref_Int, cref(Char)) out2 = iter_next(Vector(Int))(&it2);
  assert(Result_is_ok(ref_Int, cref(Char))(&out2));
  assert(ref_deref(Int)(Result_unwrap(ref_Int, cref(Char))(out2)) == 1);
  Result(ref_Int, cref(Char)) out3 = iter_next(Vector(Int))(&it2);
  assert(Result_is_err(ref_Int, cref(Char))(&out3));
  iter_free(Vector(Int))(&it2);

  return 0;
}
