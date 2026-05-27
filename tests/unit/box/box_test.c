#include "my_c_utils/box.h"
#include "my_c_utils/vector.h"
#include <assert.h>

VECTOR_CONFIG(Int)
BOX_CONFIG(Int)
BOX_CONFIG(Vector(Int))

Int main(void)
{
  Box(Int) number_box = Box_new(Int)(7);
  assert(*Box_deref(Int)(&number_box) == 7);

  *Box_deref_mut(Int)(&number_box) = 9;
  assert(*Box_deref(Int)(&number_box) == 9);

  Box(Int) cloned_number = Box_clone(Int)(&number_box);
  assert(*Box_deref(Int)(&cloned_number) == 9);

  *Box_deref_mut(Int)(&number_box) = 11;
  assert(*Box_deref(Int)(&cloned_number) == 9);

  Box_free(Int)(&number_box);
  Box_free(Int)(&cloned_number);

  Vector(Int) vector = Vector_new(Int)();
  Result(Void, cref_Char) r1 = Vector_push_back(Int)(&vector, 1);
  assert(Result_is_ok(Void, cref_Char)(&r1));
  Result(Void, cref_Char) r2 = Vector_push_back(Int)(&vector, 2);
  assert(Result_is_ok(Void, cref_Char)(&r2));

  Box(Vector(Int)) vector_box = Box_new(Vector(Int))(vector);
  vector = Vector_new(Int)();
  assert(Vector_size(Int)(Box_deref(Vector(Int))(&vector_box)) == 2);

  Box(Vector(Int)) cloned_vector_box = Box_clone(Vector(Int))(&vector_box);
  assert(Vector_size(Int)(Box_deref(Vector(Int))(&cloned_vector_box)) == 2);

  Result(ref_Int, cref_Char) original_first =
      Vector_at(Int)(Box_deref(Vector(Int))(&vector_box), 0);
  Result(ref_Int, cref_Char) cloned_first =
      Vector_at(Int)(Box_deref(Vector(Int))(&cloned_vector_box), 0);
  assert(Result_is_ok(ref_Int, cref_Char)(&original_first));
  assert(Result_is_ok(ref_Int, cref_Char)(&cloned_first));
  assert(*Result_unwrap(ref_Int, cref_Char)(original_first) == 1);
  assert(*Result_unwrap(ref_Int, cref_Char)(cloned_first) == 1);

  Result(Void, cref_Char) r3 = Vector_set(Int)(Box_deref_mut(Vector(Int))(&vector_box), 0, 99);
  assert(Result_is_ok(Void, cref_Char)(&r3));
  Result(Void, cref_Char) r4 = Vector_set(Int)(Box_deref_mut(Vector(Int))(&vector_box), 0, 99);
  assert(Result_is_ok(Void, cref_Char)(&r4));
  assert(*Result_unwrap(ref_Int, cref_Char)(Vector_at(Int)(Box_deref(Vector(Int))(&vector_box), 0)) == 99);
  assert(*Result_unwrap(ref_Int, cref_Char)(Vector_at(Int)(Box_deref(Vector(Int))(&cloned_vector_box), 0)) == 1);

  Box_free(Vector(Int))(&vector_box);
  Box_free(Vector(Int))(&cloned_vector_box);
  Vector_free(Int)(&vector);
  return 0;
}