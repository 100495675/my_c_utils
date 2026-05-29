#include "my_c_utils/my_c_utils.h"
#include "my_c_utils/string_type.h"
#include <assert.h>

VECTOR_CONFIG(Int)
RESULT_CONFIG(cref(Vector(Int)), cref(Char))
HASH_MAP_CONFIG(String, Vector(Int))

Int main(void)
{
    Hashmap(String, Vector(Int)) map = Hashmap_new(String, Vector(Int))(4);

    Vector(Int) alpha = Vector_new(Int)();

    Result(Void, cref(Char)) r1 = Vector_push_back(Int)(&alpha, 1);
    assert(Result_is_ok(Void, cref(Char))(&r1));
    Result_free(Void, cref(Char))(&r1);

    Result(Void, cref(Char)) r2 = Vector_push_back(Int)(&alpha, 2);
    assert(Result_is_ok(Void, cref(Char))(&r2));
    Result_free(Void, cref(Char))(&r2);

    Hashmap_add(String, Vector(Int))(&map, string_dup("alpha"), alpha);
    alpha = Vector_new(Int)();

    Vector(Int) beta = Vector_new(Int)();

    Result(Void, cref(Char)) r3 = Vector_push_back(Int)(&beta, 3);
    assert(Result_is_ok(Void, cref(Char))(&r3));
    Result_free(Void, cref(Char))(&r3);
    Hashmap_add(String, Vector(Int))(&map, string_dup("beta"), beta);
    beta = Vector_new(Int)();

    Result(ref_Vector(Int), cref(Char)) alpha_result =
        Hashmap_get(String, Vector(Int))(&map, "alpha");
    assert(Result_is_ok(ref_Vector(Int), cref(Char))(&alpha_result));
    assert(Vector_size(Int)(Result_unwrap(ref_Vector(Int), cref(Char))(alpha_result)) == 2);

    Result(Vector(Int), cref(Char)) popped_beta =
        Hashmap_pop(String, Vector(Int))(&map, "beta");
    assert(Result_is_ok(Vector(Int), cref(Char))(&popped_beta));
    Vector(Int) beta_copy = Result_unwrap(Vector(Int), cref(Char))(popped_beta);
    assert(Vector_size(Int)(&beta_copy) == 1);
    Vector_free(Int)(&beta_copy);

    assert(!Hashmap_contains(String, Vector(Int))(&map, "beta"));

    Hashmap_free(String, Vector(Int))(&map);
    return 0;
}