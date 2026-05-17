#include "my_c_utils/hash_map.h"
#include "my_c_utils/free.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef Char *String;

void String_free(String *s)
{
  free(*s);
}

static String string_dup(const Char *text)
{
  Size length = strlen(text);
  String copy = malloc(length + 1);
  assert(copy != NULL);
  for (Size i = 0; i <= length; ++i)
  {
    copy[i] = text[i];
  }
  return copy;
}

Size String_hash(String key)
{
  Size hash = 0;
  while (*key)
  {
    hash = (hash * 31) + (UChar)*key++;
  }
  return hash;
}

Bool String_equals(String a, String b)
{
  return strcmp(a, b) == 0;
}

HASH_MAP_CONFIG(String, Int)

static void assert_map_iteration(const Hashmap_String_Int *hm,
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

Int main()
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

  Result_Int_ref two_result = Hashmap_String_Int_get(&hm, "two");
  assert(Result_Int_ref_is_ok(two_result));
  assert(*Result_Int_ref_unwrap(two_result) == 20);

  Hashmap_String_Int_remove(&hm, "two");
  assert(!Hashmap_String_Int_contains(&hm, "two"));
  assert(Result_Int_ref_is_err(Hashmap_String_Int_get(&hm, "two")));

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
