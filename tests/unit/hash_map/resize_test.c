#include "my_c_utils/free.h"
#include "my_c_utils/hash_map.h"
#include <assert.h>
#include <stdio.h>

static Size Int_hash(Int key) { return (Size)key; }
static Bool Int_equals(Int a, Int b) { return a == b; }

HASH_MAP_CONFIG(Int, Int)

Int main(void)
{
  Hashmap_Int_Int hm = Hashmap_Int_Int_new(4);

  const Int N = 2000;
  for (Int i = 0; i < N; ++i)
  {
    Hashmap_Int_Int_add(&hm, i, i * 2);
  }

  for (Int i = 0; i < N; ++i)
  {
    assert(Hashmap_Int_Int_contains(&hm, i));
    Result_ref_Int r = Hashmap_Int_Int_get(&hm, i);
    assert(Result_ref_Int_is_ok(&r));
    assert(*Result_ref_Int_unwrap(r) == i * 2);
  }

  for (Int i = 0; i < N; i += 3)
  {
    Result_Int popped = Hashmap_Int_Int_pop(&hm, i);
    assert(Result_Int_is_ok(&popped));
  }

  Hashmap_Int_Int_free(&hm);
  return 0;
}