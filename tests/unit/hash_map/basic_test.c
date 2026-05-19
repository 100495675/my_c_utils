#include "my_c_utils/free.h"
#include "my_c_utils/hash_map.h"
#include "../../support/string_helpers.h"
#include <assert.h>

HASH_MAP_CONFIG(String, Int)

static void assert_map_iteration(Hashmap_String_Int *hm,
                                 Size expected_count,
                                 Int expected_sum)
{
  Size count = 0;
  Int sum = 0;

  for_each_ref(Int, item, Hashmap_String, hm, {
    sum += *item;
    ++count;
  });

  assert(count == expected_count);
  assert(sum == expected_sum);
}

Int main(void)
{
  Hashmap_String_Int hm = Hashmap_String_Int_new(4);

  Hashmap_String_Int_add(&hm, string_dup("one"), 10);
  Hashmap_String_Int_add(&hm, string_dup("two"), 20);
  Hashmap_String_Int_add(&hm, string_dup("three"), 30);

  HashmapStats initial_stats = Hashmap_String_Int_stats(&hm);
  assert(initial_stats.size == 3);
  assert(initial_stats.capacity == 4);
  assert(initial_stats.load_factor == 0.75);
  assert_map_iteration(&hm, 3, 60);

  assert(Hashmap_String_Int_contains(&hm, "one"));
  assert(Hashmap_String_Int_contains(&hm, "two"));
  assert(Hashmap_String_Int_contains(&hm, "three"));

  Result_ref_Int two_result = Hashmap_String_Int_get(&hm, "two");
  assert(Result_ref_Int_is_ok(two_result));
  assert(*Result_ref_Int_unwrap(two_result) == 20);

  Hashmap_String_Int_remove(&hm, "two");
  assert(!Hashmap_String_Int_contains(&hm, "two"));
  assert(Result_ref_Int_is_err(Hashmap_String_Int_get(&hm, "two")));

  Result_Int popped_three = Hashmap_String_Int_pop(&hm, "three");
  assert(Result_Int_is_ok(popped_three));
  assert(Result_Int_unwrap(popped_three) == 30);
  assert(!Hashmap_String_Int_contains(&hm, "three"));

  Hashmap_String_Int_add(&hm, string_dup("four"), 40);
  assert(Hashmap_String_Int_contains(&hm, "four"));

  HashmapStats final_stats = Hashmap_String_Int_stats(&hm);
  assert(final_stats.size == 2);
  assert(final_stats.capacity == 4);
  assert(final_stats.load_factor == 0.5);
  assert_map_iteration(&hm, 2, 50);

  Hashmap_String_Int_free(&hm);
  return 0;
}