#include "my_c_utils/free.h"
#include "my_c_utils/hash_map.h"
#include <assert.h>
#include <stdio.h>


HASH_MAP_CONFIG(Int, Int)

Int main(void)
{
  Hashmap(Int, Int) hm = Hashmap_new(Int, Int)(4);

  const Int N = 2000;
  for (Int i = 0; i < N; ++i)
  {
    Hashmap_add(Int, Int)(&hm, i, i * 2);
  }

  for (Int i = 0; i < N; ++i)
  {
    assert(Hashmap_contains(Int, Int)(&hm, i));
    Result(ref_Int, cref_Char) r = Hashmap_get(Int, Int)(&hm, i);
    assert(Result_is_ok(ref_Int, cref_Char)(&r));
    assert(ref_deref(Int)(Result_unwrap(ref_Int, cref_Char)(r)) == i * 2);
  }

  for (Int i = 0; i < N; i += 3)
  {
    Result(Int, cref_Char) popped = Hashmap_pop(Int, Int)(&hm, i);
    assert(Result_is_ok(Int, cref_Char)(&popped));
  }

  Hashmap_free(Int, Int)(&hm);
  return 0;
}