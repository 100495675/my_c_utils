#include "my_c_utils/my_c_utils.h"
#include "../support/string_helpers.h"
#include <assert.h>

VECTOR_CONFIG(Int)
HASH_MAP_CONFIG(String, Vector_Int)

Int main(void)
{
  Hashmap_String_Vector_Int map = Hashmap_String_Vector_Int_new(4);

  Vector_Int alpha = Vector_Int_new();
  assert(Result_is_ok(Vector_Int_push_back(&alpha, 1)));
  assert(Result_is_ok(Vector_Int_push_back(&alpha, 2)));
  Hashmap_String_Vector_Int_add(&map, string_dup("alpha"), alpha);
  alpha = Vector_Int_new();

  Vector_Int beta = Vector_Int_new();
  assert(Result_is_ok(Vector_Int_push_back(&beta, 3)));
  Hashmap_String_Vector_Int_add(&map, string_dup("beta"), beta);
  beta = Vector_Int_new();

  Result_ref_Vector_Int alpha_result =
      Hashmap_String_Vector_Int_get(&map, "alpha");
  assert(Result_ref_Vector_Int_is_ok(alpha_result));
  assert(Vector_Int_size(Result_ref_Vector_Int_unwrap(alpha_result)) == 2);

  Result_Vector_Int popped_beta =
      Hashmap_String_Vector_Int_pop(&map, "beta");
  assert(Result_Vector_Int_is_ok(popped_beta));
  Vector_Int beta_copy = Result_Vector_Int_unwrap(popped_beta);
  assert(Vector_Int_size(&beta_copy) == 1);
  Vector_Int_free(&beta_copy);

  assert(!Hashmap_String_Vector_Int_contains(&map, "beta"));

  Hashmap_String_Vector_Int_free(&map);
  return 0;
}