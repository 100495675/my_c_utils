#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my_c_utils/tipos.h"
#include "my_c_utils/free.h"
#include "my_c_utils/vector.h"
#include "my_c_utils/linked_list.h"
#include "my_c_utils/hash_map.h"
#include "my_c_utils/clone.h"
#include "my_c_utils/result.h"

/* Generate configs for all container types we'll use */
VECTOR_CONFIG(Int)

/* Vector<Vector<Int>> - nested deep cloning */
VECTOR_CONFIG(Vector(Int))

/* ========== Test: Clone Primitive Ts ========== */

void test_clone_primitive_int(void)
{
    printf("TEST: Clone primitive Int... ");
    Int original = 42;
    Int cloned = Clone(Int)(&original);
    assert(cloned == 42);
    printf("✓\n");
}

void test_clone_primitive_char(void)
{
    printf("TEST: Clone primitive Char... ");
    Char original = 'A';
    Char cloned = Clone(Char)(&original);
    assert(cloned == 'A');
    printf("✓\n");
}

/* ========== Test: Vector Clone (Shallow Elements) ========== */

void test_clone_Vector_Int_empty(void)
{
    printf("TEST: Clone empty Vector(Int)... ");
    Vector(Int) original = Vector_new(Int)();
    Vector(Int) result = Vector_clone(Int)(&original);

    assert(result.size == 0);

    Vector_free(Int)(&result);
    printf("✓\n");
}

void test_clone_Vector_Int_simple(void)
{
    printf("TEST: Clone Vector(Int) with elements... ");
    Vector(Int) original = Vector_new(Int)();
    Vector_push_back(Int)(&original, 10);
    Vector_push_back(Int)(&original, 20);
    Vector_push_back(Int)(&original, 30);

    Vector(Int) result = Vector_clone(Int)(&original);
    assert(result.size == 3);

    Result(ref_Int, cref(Char)) v0 = Vector_at(Int)(&result, 0);
    Result(ref_Int, cref(Char)) v1 = Vector_at(Int)(&result, 1);
    Result(ref_Int, cref(Char)) v2 = Vector_at(Int)(&result, 2);
    assert(Result_is_ok(ref_Int, cref(Char))(&v0) && ref_deref(Int)(Result_unwrap(ref_Int, cref(Char))(v0)) == 10);
    assert(Result_is_ok(ref_Int, cref(Char))(&v1) && ref_deref(Int)(Result_unwrap(ref_Int, cref(Char))(v1)) == 20);
    assert(Result_is_ok(ref_Int, cref(Char))(&v2) && ref_deref(Int)(Result_unwrap(ref_Int, cref(Char))(v2)) == 30);

    /* Verify clone is independent: modify original */
    Vector_set(Int)(&original, 0, 999);
    Result(ref_Int, cref(Char)) cloned_v0 = Vector_at(Int)(&result, 0);
    assert(Result_is_ok(ref_Int, cref(Char))(&cloned_v0) && ref_deref(Int)(Result_unwrap(ref_Int, cref(Char))(cloned_v0)) == 10);

    Vector_free(Int)(&original);
    Vector_free(Int)(&result);
    printf("✓\n");
}

/* ========== Test: Nested Vector Clone (Deep Cloning) ========== */

void test_clone_vector_of_vectors_empty(void)
{
    printf("TEST: Clone empty Vector_Vector(Int)... ");
    Vector(Vector(Int)) original = Vector_new(Vector(Int))();
    Vector(Vector(Int)) result = Vector_clone(Vector(Int))(&original);

    assert(result.size == 0);

    Vector_free(Vector(Int))(&result);
    printf("✓\n");
}

void test_clone_vector_of_vectors_nested(void)
{
    printf("TEST: Clone Vector_Vector(Int) with nested data... ");
    Vector(Vector(Int)) original = Vector_new(Vector(Int))();

    /* Create nested vectors */
    Vector(Int) inner1 = Vector_new(Int)();
    Vector_push_back(Int)(&inner1, 1);
    Vector_push_back(Int)(&inner1, 2);
    Vector_push_back(Int)(&inner1, 3);

    Vector(Int) inner2 = Vector_new(Int)();
    Vector_push_back(Int)(&inner2, 10);
    Vector_push_back(Int)(&inner2, 20);

    Vector_push_back(Vector(Int))(&original, inner1);
    Vector_push_back(Vector(Int))(&original, inner2);

    /* Clone the nested structure */
    Vector(Vector(Int)) result = Vector_clone(Vector(Int))(&original);
    assert(result.size == 2);

    /* Verify first nested vector */
    Result(ref_Vector(Int), cref(Char)) ref1 = Vector_at(Vector(Int))(&result, 0);
    assert(Result_is_ok(ref_Vector(Int), cref(Char))(&ref1));
    assert(Result_unwrap(ref_Vector(Int), cref(Char))(ref1)->size == 3);
    Result(ref_Int, cref(Char)) v1_0 = Vector_at(Int)(Result_unwrap(ref_Vector(Int), cref(Char))(ref1), 0);
    Result(ref_Int, cref(Char)) v1_1 = Vector_at(Int)(Result_unwrap(ref_Vector(Int), cref(Char))(ref1), 1);
    Result(ref_Int, cref(Char)) v1_2 = Vector_at(Int)(Result_unwrap(ref_Vector(Int), cref(Char))(ref1), 2);
    assert(Result_is_ok(ref_Int, cref(Char))(&v1_0) && ref_deref(Int)(Result_unwrap(ref_Int, cref(Char))(v1_0)) == 1);
    assert(Result_is_ok(ref_Int, cref(Char))(&v1_1) && ref_deref(Int)(Result_unwrap(ref_Int, cref(Char))(v1_1)) == 2);
    assert(Result_is_ok(ref_Int, cref(Char))(&v1_2) && ref_deref(Int)(Result_unwrap(ref_Int, cref(Char))(v1_2)) == 3);

    /* Verify second nested vector */
    Result(ref_Vector(Int), cref(Char)) ref2 = Vector_at(Vector(Int))(&result, 1);
    assert(Result_is_ok(ref_Vector(Int), cref(Char))(&ref2));
    assert(Result_unwrap(ref_Vector(Int), cref(Char))(ref2)->size == 2);
    Result(ref_Int, cref(Char)) v2_0 = Vector_at(Int)(Result_unwrap(ref_Vector(Int), cref(Char))(ref2), 0);
    Result(ref_Int, cref(Char)) v2_1 = Vector_at(Int)(Result_unwrap(ref_Vector(Int), cref(Char))(ref2), 1);
    assert(Result_is_ok(ref_Int, cref(Char))(&v2_0) && ref_deref(Int)(Result_unwrap(ref_Int, cref(Char))(v2_0)) == 10);
    assert(Result_is_ok(ref_Int, cref(Char))(&v2_1) && ref_deref(Int)(Result_unwrap(ref_Int, cref(Char))(v2_1)) == 20);

    /* Verify independence: modify original's nested vector */
    Result(ref_Vector(Int), cref(Char)) orig_ref1 = Vector_at(Vector(Int))(&original, 0);
    assert(Result_is_ok(ref_Vector(Int), cref(Char))(&orig_ref1));
    Vector_set(Int)(Result_unwrap(ref_Vector(Int), cref(Char))(orig_ref1), 0, 999);

    /* Cloned version should be unchanged */
    Result(ref_Int, cref(Char)) cloned_check =
        Vector_at(Int)(Result_unwrap(ref_Vector(Int), cref(Char))(Vector_at(Vector(Int))(&result, 0)), 0);
    assert(Result_is_ok(ref_Int, cref(Char))(&cloned_check) && ref_deref(Int)(Result_unwrap(ref_Int, cref(Char))(cloned_check)) == 1);

    Vector_free(Vector(Int))(&original);
    Vector_free(Vector(Int))(&result);
    printf("✓\n");
}

/* ========== Test: NULL pointer handling ========== */

void test_clone_null_vector(void)
{
    printf("TEST: Clone NULL Vector pointer... ");
    printf("skipped (clone now aborts on NULL)\n");
}

/* ========== Main ========== */

int main(void)
{
    printf("\n=== Deep Clone Tests ===\n\n");

    /* Primitive types */
    test_clone_primitive_int();
    test_clone_primitive_char();

    /* Vector cloning */
    test_clone_Vector_Int_empty();
    test_clone_Vector_Int_simple();

    /* Nested vector cloning (deep clone) */
    test_clone_vector_of_vectors_empty();
    test_clone_vector_of_vectors_nested();

    printf("\n=== All clone tests passed! ===\n\n");
    return 0;
}
