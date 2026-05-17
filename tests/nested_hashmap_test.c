#include "my_c_utils/hash_map.h"
#include "my_c_utils/vector.h"
#include "my_c_utils/free.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef Char *String;

static String string_dup(const Char *text) {
  Size length = strlen(text);
  String copy = malloc(length + 1);
  assert(copy != NULL);
  for (Size i = 0; i <= length; ++i) {
    copy[i] = text[i];
  }
  return copy;
}

Size String_hash(String key) {
  Size hash = 0;
  while (*key) {
    hash = (hash * 31) + (UChar)*key++;
  }
  return hash;
}

Bool String_equals(String a, String b) { return strcmp(a, b) == 0; }

void String_free(String *value) { free(*value); }

VECTOR_CONFIG(Int)

typedef Vector_Int Vector_Int_map;
static inline void Vector_Int_map_free(Vector_Int_map *value) {
  Vector_Int_free(value);
}

HASH_MAP_CONFIG(String, Vector_Int_map)

Int main(void) {
  Hashmap_String_Vector_Int_map map = Hashmap_String_Vector_Int_map_new(4);

  Vector_Int alpha = Vector_Int_new();
  assert(Result_is_ok(Vector_Int_push_back(&alpha, 1)));
  assert(Result_is_ok(Vector_Int_push_back(&alpha, 2)));
  Hashmap_String_Vector_Int_map_add(&map, string_dup("alpha"), alpha);
  alpha = Vector_Int_new();

  Vector_Int beta = Vector_Int_new();
  assert(Result_is_ok(Vector_Int_push_back(&beta, 3)));
  Hashmap_String_Vector_Int_map_add(&map, string_dup("beta"), beta);
  beta = Vector_Int_new();

  Result_Vector_Int_map_ref alpha_result =
      Hashmap_String_Vector_Int_map_get(&map, "alpha");
  assert(Result_Vector_Int_map_ref_is_ok(alpha_result));
  assert(Vector_Int_size(Result_Vector_Int_map_ref_unwrap(alpha_result)) == 2);

  Result_Vector_Int_map popped_beta =
      Hashmap_String_Vector_Int_map_pop(&map, "beta");
  assert(Result_Vector_Int_map_is_ok(popped_beta));
  Vector_Int beta_copy = Result_Vector_Int_map_unwrap(popped_beta);
  assert(Vector_Int_size(&beta_copy) == 1);
  Vector_Int_map_free(&beta_copy);

  assert(!Hashmap_String_Vector_Int_map_contains(&map, "beta"));

  Hashmap_String_Vector_Int_map_free(&map);
  return 0;
}
