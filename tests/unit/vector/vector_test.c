#include "my_c_utils/free.h"
#include "my_c_utils/vector.h"
#include <assert.h>

VECTOR_CONFIG(Char)

static Size char_count(Vector_Char vector)
{
  Size count = 0;
  for_each_ref(Char, it, Vector, &vector, {
    (void)it;
    ++count;
  });
  return count;
}

Int main(void)
{
  Vector_Char vector = Vector_Char_new();
  assert(Vector_Char_size(&vector) == 0);

  Result_Void r1 = Vector_Char_push_back(&vector, 'a');
  assert(Result_Void_is_ok(&r1));
  Result_Void r2 = Vector_Char_push_back(&vector, 'b');
  assert(Result_Void_is_ok(&r2));
  Result_Void r3 = Vector_Char_push_back(&vector, 'c');
  assert(Result_Void_is_ok(&r3));

  assert(Vector_Char_size(&vector) == 3);
  assert(char_count(vector) == 3);

  Result_Void_ref_Char first = Vector_Char_at(&vector, 0);
  assert(Result_Void_ref_Char_is_ok(&first));
  assert(*Result_Void_ref_Char_unwrap(first) == 'a');

  Result_Void r4 = Vector_Char_set(&vector, 1, 'z');
  assert(Result_Void_is_ok(&r4));
  assert(*Result_Void_ref_Char_unwrap(Vector_Char_at(&vector, 1)) == 'z');

  Result_Void_Char popped = Vector_Char_pop_back(&vector);
  assert(Result_Void_Char_is_ok(&popped));
  assert(Result_Void_Char_unwrap(popped) == 'c');
  assert(Vector_Char_size(&vector) == 2);

  Result_Void r5 = Vector_Char_reserve(&vector, 8);
  assert(Result_Void_is_ok(&r5));
  assert(Vector_Char_size(&vector) == 2);

  Vector_Char_clear(&vector);
  assert(Vector_Char_size(&vector) == 0);

  Vector_Char_free(&vector);
  return 0;
}