#include "my_c_utils/free.h"
#include "my_c_utils/hash_map.h"
#include "my_c_utils/string_type.h"
#include <assert.h>

HASH_MAP_CONFIG(String, Int)

static void assert_map_iteration(Hashmap(String, Int) *hm,
                                 Size expected_count,
                                 Int expected_sum)
{
  Size count = 0;
  Int sum = 0;

  for_each_ref(Hashmap(String, Int), item, hm) {
    sum += *item;
    ++count;
  }

  assert(count == expected_count);
  assert(sum == expected_sum);
}

Int main(void)
{
  Hashmap(String, Int) hm = Hashmap_new(String, Int)(4);

  Hashmap_add(String, Int)(&hm, string_dup("one"), 10);
  Hashmap_add(String, Int)(&hm, string_dup("two"), 20);
  Hashmap_add(String, Int)(&hm, string_dup("three"), 30);

  HashmapStats initial_stats = Hashmap_stats(String, Int)(&hm);
  assert(initial_stats.size == 3);
  assert(initial_stats.capacity == 4);
  assert(initial_stats.load_factor == 0.75);
  assert_map_iteration(&hm, 3, 60);

  assert(Hashmap_contains(String, Int)(&hm, "one"));
  assert(Hashmap_contains(String, Int)(&hm, "two"));
  assert(Hashmap_contains(String, Int)(&hm, "three"));

  Result(ref_Int, cref_Char) two_result = Hashmap_get(String, Int)(&hm, "two");
  assert(Result_is_ok(ref_Int, cref_Char)(&two_result));
  assert(*Result_unwrap(ref_Int, cref_Char)(two_result) == 20);

  Hashmap_remove(String, Int)(&hm, "two");
  assert(!Hashmap_contains(String, Int)(&hm, "two"));
  Result(ref_Int, cref_Char) two_result_after_removal = Hashmap_get(String, Int)(&hm, "two");
  assert(Result_is_err(ref_Int, cref_Char)(&two_result_after_removal));

  Result(Int, cref_Char) popped_three = Hashmap_pop(String, Int)(&hm, "three");
  assert(Result_is_ok(Int, cref_Char)(&popped_three));
  assert(Result_unwrap(Int, cref_Char)(popped_three) == 30);
  assert(!Hashmap_contains(String, Int)(&hm, "three"));

  Hashmap_add(String, Int)(&hm, string_dup("four"), 40);
  assert(Hashmap_contains(String, Int)(&hm, "four"));

  HashmapStats final_stats = Hashmap_stats(String, Int)(&hm);
  assert(final_stats.size == 2);
  assert(final_stats.capacity == 4);
  assert(final_stats.load_factor == 0.5);
  assert_map_iteration(&hm, 2, 50);

  Hashmap_free(String, Int)(&hm);
  return 0;
}