#include "my_c_utils/free.h"
#include "my_c_utils/vector.h"
#include <assert.h>

VECTOR_CONFIG(Char)

static Size char_count(Vector(Char) vector)
{
  Size count = 0;
  for_each_ref(Vector(Char), it, &vector) {
    (void)it;
    ++count;
  }
  return count;
}

Int main(void)
{
  Vector(Char) vector = Vector_new(Char)();
  assert(Vector_size(Char)(&vector) == 0);

  Result(Void, cref(Char)) r1 = Vector_push_back(Char)(&vector, 'a');
  assert(Result_is_ok(Void, cref(Char))(&r1));
  Result(Void, cref(Char)) r2 = Vector_push_back(Char)(&vector, 'b');
  assert(Result_is_ok(Void, cref(Char))(&r2));
  Result(Void, cref(Char)) r3 = Vector_push_back(Char)(&vector, 'c');
  assert(Result_is_ok(Void, cref(Char))(&r3));

  assert(Vector_size(Char)(&vector) == 3);
  assert(char_count(vector) == 3);

  Result(ref_Char, cref(Char)) first = Vector_at(Char)(&vector, 0);
  assert(Result_is_ok(ref_Char, cref(Char))(&first));
  assert(ref_deref(Char)(Result_unwrap(ref_Char, cref(Char))(first)) == 'a');

  Result(Void, cref(Char)) r4 = Vector_set(Char)(&vector, 1, 'z');
  assert(Result_is_ok(Void, cref(Char))(&r4));
  assert(ref_deref(Char)(Result_unwrap(ref_Char, cref(Char))(Vector_at(Char)(&vector, 1))) == 'z');

  Result(Char, cref(Char)) popped = Vector_pop_back(Char)(&vector);
  assert(Result_is_ok(Char, cref(Char))(&popped));
  assert(Result_unwrap(Char, cref(Char))(popped) == 'c');
  assert(Vector_size(Char)(&vector) == 2);

  Result(Void, cref(Char)) r5 = Vector_reserve(Char)(&vector, 8);
  assert(Result_is_ok(Void, cref(Char))(&r5));
  assert(Vector_size(Char)(&vector) == 2);

  Vector_clear(Char)(&vector);
  assert(Vector_size(Char)(&vector) == 0);

  Vector_free(Char)(&vector);
  return 0;
}