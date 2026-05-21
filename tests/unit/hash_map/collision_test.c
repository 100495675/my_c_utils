#include "my_c_utils/free.h"
#include "my_c_utils/hash_map.h"
#include "../../support/string_helpers.h"
#include <assert.h>

static String make_key(Int value)
{
  Char digits[16];
  Size digits_len = 0;

  do
  {
    digits[digits_len++] = (Char)('0' + (value % 10));
    value /= 10;
  } while (value > 0);

  const Size prefix_len = 4;
  String key = (String)malloc(prefix_len + digits_len + 1);
  assert(key != NULL);

  key[0] = 'k';
  key[1] = 'e';
  key[2] = 'y';
  key[3] = '-';

  for (Size i = 0; i < digits_len; ++i)
  {
    key[prefix_len + i] = digits[digits_len - 1 - i];
  }
  key[prefix_len + digits_len] = '\0';
  return key;
}

HASH_MAP_CONFIG(String, Int)

Int main(void)
{
  Hashmap_String_Int hm = Hashmap_String_Int_new(8);

  for (Int i = 0; i < 200; ++i)
  {
    Hashmap_String_Int_add(&hm, make_key(i), i);
  }

  for (Int i = 0; i < 200; ++i)
  {
    String key = make_key(i);
    assert(Hashmap_String_Int_contains(&hm, key));
    Result_ref_Int r = Hashmap_String_Int_get(&hm, key);
    assert(Result_ref_Int_is_ok(&r));
    assert(*Result_ref_Int_unwrap(r) == i);
    free(key);
  }

  for (Int i = 0; i < 100; ++i)
  {
    String key = make_key(i);
    Hashmap_String_Int_remove(&hm, key);
    free(key);
  }

  for (Int i = 0; i < 100; ++i)
  {
    String key = make_key(i);
    assert(!Hashmap_String_Int_contains(&hm, key));
    free(key);
  }

  for (Int i = 100; i < 200; ++i)
  {
    String key = make_key(i);
    assert(Hashmap_String_Int_contains(&hm, key));
    Result_ref_Int r = Hashmap_String_Int_get(&hm, key);
    assert(Result_ref_Int_is_ok(&r));
    assert(*Result_ref_Int_unwrap(r) == i);
    free(key);
  }

  Hashmap_String_Int_free(&hm);
  return 0;
}