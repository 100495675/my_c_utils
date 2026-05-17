#include "my_c_utils/vector.h"
#include "my_c_utils/free.h"
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

Int main()
{
  Vector_Char vector = Vector_Char_new();
  assert(Vector_Char_size(&vector) == 0);

  assert(Result_is_ok(Vector_Char_push_back(&vector, 'a')));
  assert(Result_is_ok(Vector_Char_push_back(&vector, 'b')));
  assert(Result_is_ok(Vector_Char_push_back(&vector, 'c')));
  assert(Vector_Char_size(&vector) == 3);
  assert(char_count(vector) == 3);

  Result_Char_ref first = Vector_Char_at(&vector, 0);
  assert(Result_Char_ref_is_ok(first));
  assert(*Result_Char_ref_unwrap(first) == 'a');

  assert(Result_is_ok(Vector_Char_set(&vector, 1, 'z')));
  assert(*Result_Char_ref_unwrap(Vector_Char_at(&vector, 1)) == 'z');

  Result_Char popped = Vector_Char_pop_back(&vector);
  assert(Result_Char_is_ok(popped));
  assert(Result_Char_unwrap(popped) == 'c');
  assert(Vector_Char_size(&vector) == 2);

  assert(Result_is_ok(Vector_Char_reserve(&vector, 8)));
  assert(Vector_Char_size(&vector) == 2);

  Vector_Char_clear(&vector);
  assert(Vector_Char_size(&vector) == 0);

  Vector_Char_free(&vector);
  return 0;
}
