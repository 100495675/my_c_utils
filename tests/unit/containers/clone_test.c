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
VECTOR_CONFIG(Vector_Int)

/* ========== Test: Clone Primitive Types ========== */

void test_clone_primitive_int(void)
{
    printf("TEST: Clone primitive Int... ");
    Int original = 42;
    Result_Int cloned = Int_clone(&original);
    assert(!cloned.is_error);
    assert(cloned.value == 42);
    printf("✓\n");
}

void test_clone_primitive_char(void)
{
    printf("TEST: Clone primitive Char... ");
    Char original = 'A';
    Result_Char cloned = Char_clone(&original);
    assert(!cloned.is_error);
    assert(cloned.value == 'A');
    printf("✓\n");
}

/* ========== Test: Vector Clone (Shallow Elements) ========== */

void test_clone_vector_int_empty(void)
{
    printf("TEST: Clone empty Vector_Int... ");
    Vector_Int original = Vector_Int_new();
    Result_Vector_Int result = Vector_Int_clone(&original);

    assert(!result.is_error);
    assert(result.value.size == 0);

    Vector_Int_free(&result.value);
    printf("✓\n");
}

void test_clone_vector_int_simple(void)
{
    printf("TEST: Clone Vector_Int with elements... ");
    Vector_Int original = Vector_Int_new();
    Vector_Int_push_back(&original, 10);
    Vector_Int_push_back(&original, 20);
    Vector_Int_push_back(&original, 30);

    Result_Vector_Int result = Vector_Int_clone(&original);
    assert(!result.is_error);
    assert(result.value.size == 3);

    Result_Int_ref v0 = Vector_Int_at(&result.value, 0);
    Result_Int_ref v1 = Vector_Int_at(&result.value, 1);
    Result_Int_ref v2 = Vector_Int_at(&result.value, 2);
    assert(!v0.is_error && *v0.value == 10);
    assert(!v1.is_error && *v1.value == 20);
    assert(!v2.is_error && *v2.value == 30);

    /* Verify clone is independent: modify original */
    Vector_Int_set(&original, 0, 999);
    Result_Int_ref cloned_v0 = Vector_Int_at(&result.value, 0);
    assert(!cloned_v0.is_error && *cloned_v0.value == 10);

    Vector_Int_free(&original);
    Vector_Int_free(&result.value);
    printf("✓\n");
}

/* ========== Test: Nested Vector Clone (Deep Cloning) ========== */

void test_clone_vector_of_vectors_empty(void)
{
    printf("TEST: Clone empty Vector_Vector_Int... ");
    Vector_Vector_Int original = Vector_Vector_Int_new();
    Result_Vector_Vector_Int result = Vector_Vector_Int_clone(&original);

    assert(!result.is_error);
    assert(result.value.size == 0);

    Vector_Vector_Int_free(&result.value);
    printf("✓\n");
}

void test_clone_vector_of_vectors_nested(void)
{
    printf("TEST: Clone Vector_Vector_Int with nested data... ");
    Vector_Vector_Int original = Vector_Vector_Int_new();

    /* Create nested vectors */
    Vector_Int inner1 = Vector_Int_new();
    Vector_Int_push_back(&inner1, 1);
    Vector_Int_push_back(&inner1, 2);
    Vector_Int_push_back(&inner1, 3);

    Vector_Int inner2 = Vector_Int_new();
    Vector_Int_push_back(&inner2, 10);
    Vector_Int_push_back(&inner2, 20);

    Vector_Vector_Int_push_back(&original, inner1);
    Vector_Vector_Int_push_back(&original, inner2);

    /* Clone the nested structure */
    Result_Vector_Vector_Int result = Vector_Vector_Int_clone(&original);
    assert(!result.is_error);
    assert(result.value.size == 2);

    /* Verify first nested vector */
    Result_Vector_Int_ref ref1 = Vector_Vector_Int_at(&result.value, 0);
    assert(!ref1.is_error);
    assert(ref1.value->size == 3);
    Result_Int_ref v1_0 = Vector_Int_at(ref1.value, 0);
    Result_Int_ref v1_1 = Vector_Int_at(ref1.value, 1);
    Result_Int_ref v1_2 = Vector_Int_at(ref1.value, 2);
    assert(!v1_0.is_error && *v1_0.value == 1);
    assert(!v1_1.is_error && *v1_1.value == 2);
    assert(!v1_2.is_error && *v1_2.value == 3);

    /* Verify second nested vector */
    Result_Vector_Int_ref ref2 = Vector_Vector_Int_at(&result.value, 1);
    assert(!ref2.is_error);
    assert(ref2.value->size == 2);
    Result_Int_ref v2_0 = Vector_Int_at(ref2.value, 0);
    Result_Int_ref v2_1 = Vector_Int_at(ref2.value, 1);
    assert(!v2_0.is_error && *v2_0.value == 10);
    assert(!v2_1.is_error && *v2_1.value == 20);

    /* Verify independence: modify original's nested vector */
    Result_Vector_Int_ref orig_ref1 = Vector_Vector_Int_at(&original, 0);
    assert(!orig_ref1.is_error);
    Vector_Int_set(orig_ref1.value, 0, 999);

    /* Cloned version should be unchanged */
    Result_Int_ref cloned_check =
        Vector_Int_at((Vector_Vector_Int_at(&result.value, 0)).value, 0);
    assert(!cloned_check.is_error && *cloned_check.value == 1);

    Vector_Vector_Int_free(&original);
    Vector_Vector_Int_free(&result.value);
    printf("✓\n");
}

/* ========== Test: NULL pointer handling ========== */

void test_clone_null_vector(void)
{
    printf("TEST: Clone NULL Vector pointer... ");
    Result_Vector_Int result = Vector_Int_clone(NULL);
    assert(result.is_error);
    printf("✓\n");
}

/* ========== Main ========== */

int main(void)
{
    printf("\n=== Deep Clone Tests ===\n\n");

    /* Primitive types */
    test_clone_primitive_int();
    test_clone_primitive_char();

    /* Vector cloning */
    test_clone_vector_int_empty();
    test_clone_vector_int_simple();

    /* Nested vector cloning (deep clone) */
    test_clone_vector_of_vectors_empty();
    test_clone_vector_of_vectors_nested();

    /* NULL pointer handling */
    test_clone_null_vector();

    printf("\n=== All clone tests passed! ===\n\n");
    return 0;
}

