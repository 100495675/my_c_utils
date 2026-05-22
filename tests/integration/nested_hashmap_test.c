#include "my_c_utils/my_c_utils.h"
#include "my_c_utils/string_type.h"
#include <assert.h>

VECTOR_CONFIG(Int)
HASH_MAP_CONFIG(String, Vector_Int)

Int main(void)
{
    Hashmap_String_Vector_Int map = Hashmap_String_Vector_Int_new(4);

    Vector_Int alpha = Vector_Int_new();

    Result_Void r1 = Vector_Int_push_back(&alpha, 1);
    assert(Result_Void_is_ok(&r1));
    Result_Void_free(&r1);

    Result_Void r2 = Vector_Int_push_back(&alpha, 2);
    assert(Result_Void_is_ok(&r2));
    Result_Void_free(&r2);

    Hashmap_String_Vector_Int_add(&map, string_dup("alpha"), alpha);
    alpha = Vector_Int_new();

    Vector_Int beta = Vector_Int_new();

    Result_Void r3 = Vector_Int_push_back(&beta, 3);
    assert(Result_Void_is_ok(&r3));
    Result_Void_free(&r3);
    Hashmap_String_Vector_Int_add(&map, string_dup("beta"), beta);
    beta = Vector_Int_new();

    Result_Void_ref_Vector_Int alpha_result =
        Hashmap_String_Vector_Int_get(&map, "alpha");
    assert(Result_Void_ref_Vector_Int_is_ok(&alpha_result));
    assert(Vector_Int_size(Result_Void_ref_Vector_Int_unwrap(alpha_result)) == 2);

    Result_Void_Vector_Int popped_beta =
        Hashmap_String_Vector_Int_pop(&map, "beta");
    assert(Result_Void_Vector_Int_is_ok(&popped_beta));
    Vector_Int beta_copy = Result_Void_Vector_Int_unwrap(popped_beta);
    assert(Vector_Int_size(&beta_copy) == 1);
    Vector_Int_free(&beta_copy);

    assert(!Hashmap_String_Vector_Int_contains(&map, "beta"));

    Hashmap_String_Vector_Int_free(&map);
    return 0;
}