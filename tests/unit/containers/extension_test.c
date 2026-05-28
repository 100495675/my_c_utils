#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "my_c_utils/tipos.h"
#include "my_c_utils/free.h"
#include "my_c_utils/vector.h"
#include "my_c_utils/linked_list.h"
#include "my_c_utils/hash_map.h"
#include "my_c_utils/result.h"

/* Generate configs for container types we'll use in testing.
   We can now safely use the SAME element type (Int) across multiple
   containers because Result(ref_Int, cref_Char) is pre-configured in primitives.h,
   eliminating any duplicate symbol macro configuration conflicts! */
VECTOR_CONFIG(Int)
LINKED_LIST_CONFIG(Int)
HASH_MAP_CONFIG(Int, Int)

/* Comparison function for qsort / Vector_Int_sort */
int compare_ints(const void *a, const void *b)
{
    int int_a = *(const int *)a;
    int int_b = *(const int *)b;
    return (int_a > int_b) - (int_a < int_b);
}

/* ========== Test: Vector Extensions ========== */

void test_vector_insert_at(void)
{
    printf("TEST: Vector_Int_insert_at... ");
    Vector(Int) vec = Vector_new(Int)();
    
    // Insert in empty vector (index 0)
    Result(Void, cref_Char) r = Vector_insert_at(Int)(&vec, 0, 10);
    assert(Result_is_ok(Void, cref_Char)(&r));
    assert(vec.size == 1);
    
    // Insert at end (index 1)
    r = Vector_insert_at(Int)(&vec, 1, 30);
    assert(Result_is_ok(Void, cref_Char)(&r));
    assert(vec.size == 2);
    
    // Insert in middle (index 1)
    r = Vector_insert_at(Int)(&vec, 1, 20);
    assert(Result_is_ok(Void, cref_Char)(&r));
    assert(vec.size == 3);
    
    // Insert at beginning (index 0)
    r = Vector_insert_at(Int)(&vec, 0, 5);
    assert(Result_is_ok(Void, cref_Char)(&r));
    assert(vec.size == 4);
    
    // Verify values: 5, 10, 20, 30
    assert(ref_deref(Int)(Result_unwrap(ref_Int, cref_Char)(Vector_at(Int)(&vec, 0))) == 5);
    assert(ref_deref(Int)(Result_unwrap(ref_Int, cref_Char)(Vector_at(Int)(&vec, 1))) == 10);
    assert(ref_deref(Int)(Result_unwrap(ref_Int, cref_Char)(Vector_at(Int)(&vec, 2))) == 20);
    assert(ref_deref(Int)(Result_unwrap(ref_Int, cref_Char)(Vector_at(Int)(&vec, 3))) == 30);
    
    // Bounds check
    r = Vector_insert_at(Int)(&vec, 5, 99);
    assert(Result_is_err(Void, cref_Char)(&r));
    
    Vector_free(Int)(&vec);
    printf("✓\n");
}

void test_vector_remove_at(void)
{
    printf("TEST: Vector_Int_remove_at... ");
    Vector(Int) vec = Vector_new(Int)();
    Vector_push_back(Int)(&vec, 10);
    Vector_push_back(Int)(&vec, 20);
    Vector_push_back(Int)(&vec, 30);
    Vector_push_back(Int)(&vec, 40);
    
    // Remove from middle (index 1) -> 10, 30, 40
    Result(Void, cref_Char) r = Vector_remove_at(Int)(&vec, 1);
    assert(Result_is_ok(Void, cref_Char)(&r));
    assert(vec.size == 3);
    assert(ref_deref(Int)(Result_unwrap(ref_Int, cref_Char)(Vector_at(Int)(&vec, 0))) == 10);
    assert(ref_deref(Int)(Result_unwrap(ref_Int, cref_Char)(Vector_at(Int)(&vec, 1))) == 30);
    assert(ref_deref(Int)(Result_unwrap(ref_Int, cref_Char)(Vector_at(Int)(&vec, 2))) == 40);
    
    // Remove from beginning (index 0) -> 30, 40
    r = Vector_remove_at(Int)(&vec, 0);
    assert(Result_is_ok(Void, cref_Char)(&r));
    assert(vec.size == 2);
    assert(ref_deref(Int)(Result_unwrap(ref_Int, cref_Char)(Vector_at(Int)(&vec, 0))) == 30);
    
    // Remove from end (index 1) -> 30
    r = Vector_remove_at(Int)(&vec, 1);
    assert(Result_is_ok(Void, cref_Char)(&r));
    assert(vec.size == 1);
    assert(ref_deref(Int)(Result_unwrap(ref_Int, cref_Char)(Vector_at(Int)(&vec, 0))) == 30);
    
    // Bounds check
    r = Vector_remove_at(Int)(&vec, 1);
    assert(Result_is_err(Void, cref_Char)(&r));
    
    Vector_free(Int)(&vec);
    printf("✓\n");
}

void test_vector_shrink_to_fit(void)
{
    printf("TEST: Vector_Int_shrink_to_fit... ");
    Vector(Int) vec = Vector_new(Int)();
    
    // Test on empty vector
    Result(Void, cref_Char) r = Vector_shrink_to_fit(Int)(&vec);
    assert(Result_is_ok(Void, cref_Char)(&r));
    assert(vec.capacity == 0);
    
    Vector_push_back(Int)(&vec, 10);
    Vector_push_back(Int)(&vec, 20);
    assert(vec.capacity >= 2);
    
    Vector_reserve(Int)(&vec, 100);
    assert(vec.capacity >= 100);
    
    r = Vector_shrink_to_fit(Int)(&vec);
    assert(Result_is_ok(Void, cref_Char)(&r));
    assert(vec.capacity == vec.size);
    assert(ref_deref(Int)(Result_unwrap(ref_Int, cref_Char)(Vector_at(Int)(&vec, 0))) == 10);
    assert(ref_deref(Int)(Result_unwrap(ref_Int, cref_Char)(Vector_at(Int)(&vec, 1))) == 20);
    
    Vector_free(Int)(&vec);
    printf("✓\n");
}

void test_vector_sort(void)
{
    printf("TEST: Vector_Int_sort... ");
    Vector(Int) vec = Vector_new(Int)();
    Vector_push_back(Int)(&vec, 30);
    Vector_push_back(Int)(&vec, 10);
    Vector_push_back(Int)(&vec, 40);
    Vector_push_back(Int)(&vec, 20);
    
    Vector_sort(Int)(&vec, compare_ints);
    
    assert(ref_deref(Int)(Result_unwrap(ref_Int, cref_Char)(Vector_at(Int)(&vec, 0))) == 10);
    assert(ref_deref(Int)(Result_unwrap(ref_Int, cref_Char)(Vector_at(Int)(&vec, 1))) == 20);
    assert(ref_deref(Int)(Result_unwrap(ref_Int, cref_Char)(Vector_at(Int)(&vec, 2))) == 30);
    assert(ref_deref(Int)(Result_unwrap(ref_Int, cref_Char)(Vector_at(Int)(&vec, 3))) == 40);
    
    Vector_free(Int)(&vec);
    printf("✓\n");
}

/* ========== Test: Linked List Extensions ========== */

void test_linked_list_push_pop_front(void)
{
    printf("TEST: List_Int_push_front and pop_front... ");
    List(Int) list = List_new(Int)();
    
    // push_front
    Result(Void, cref_Char) r = List_push_front(Int)(&list, 10);
    assert(Result_is_ok(Void, cref_Char)(&r));
    r = List_push_front(Int)(&list, 20);
    assert(Result_is_ok(Void, cref_Char)(&r));
    r = List_push_front(Int)(&list, 30);
    assert(Result_is_ok(Void, cref_Char)(&r));
    
    assert(list.size == 3);
    
    // pop_front
    Result(Int, cref_Char) pop_res = List_pop_front(Int)(&list);
    assert(Result_is_ok(Int, cref_Char)(&pop_res));
    assert(Result_unwrap(Int, cref_Char)(pop_res) == 30);
    
    pop_res = List_pop_front(Int)(&list);
    assert(Result_is_ok(Int, cref_Char)(&pop_res));
    assert(Result_unwrap(Int, cref_Char)(pop_res) == 20);
    
    pop_res = List_pop_front(Int)(&list);
    assert(Result_is_ok(Int, cref_Char)(&pop_res));
    assert(Result_unwrap(Int, cref_Char)(pop_res) == 10);
    
    assert(list.size == 0);
    
    // Pop on empty list
    pop_res = List_pop_front(Int)(&list);
    assert(Result_is_err(Int, cref_Char)(&pop_res));
    
    List_free(Int)(&list);
    printf("✓\n");
}

void test_linked_list_push_back_result(void)
{
    printf("TEST: List_Int_push_back returning Result_Void... ");
    List(Int) list = List_new(Int)();
    
    Result(Void, cref_Char) r = List_push_back(Int)(&list, 42);
    assert(Result_is_ok(Void, cref_Char)(&r));
    assert(list.size == 1);
    
    Result(Int, cref_Char) pop_res = List_pop_back(Int)(&list);
    assert(Result_is_ok(Int, cref_Char)(&pop_res));
    assert(Result_unwrap(Int, cref_Char)(pop_res) == 42);
    
    List_free(Int)(&list);
    printf("✓\n");
}

/* ========== Test: Hash Map Extensions ========== */

void test_hash_map_get_mut(void)
{
    printf("TEST: Hashmap_Int_Int_get_mut... ");
    Hashmap(Int, Int) map = Hashmap_new(Int, Int)(4);
    
    Result(Void, cref_Char) r = Hashmap_add(Int, Int)(&map, 10, 100);
    assert(Result_is_ok(Void, cref_Char)(&r));
    
    // Retrieve mutable pointer and modify value
    Result(ref_Int, cref_Char) get_res = Hashmap_get_mut(Int, Int)(&map, 10);
    assert(Result_is_ok(ref_Int, cref_Char)(&get_res));
    assert(ref_deref(Int)(Result_unwrap(ref_Int, cref_Char)(get_res)) == 100);
    
    // Modify through the returned pointer
    *Result_unwrap(ref_Int, cref_Char)(get_res) = 999;
    
    // Get again to verify modification
    get_res = Hashmap_get(Int, Int)(&map, 10);
    assert(Result_is_ok(ref_Int, cref_Char)(&get_res));
    assert(ref_deref(Int)(Result_unwrap(ref_Int, cref_Char)(get_res)) == 999);
    
    Hashmap_free(Int, Int)(&map);
    printf("✓\n");
}

int main(void)
{
    printf("=== Extension Tests ===\n\n");
    
    test_vector_insert_at();
    test_vector_remove_at();
    test_vector_shrink_to_fit();
    test_vector_sort();
    
    test_linked_list_push_pop_front();
    test_linked_list_push_back_result();
    
    test_hash_map_get_mut();
    
    printf("\n=== All extension tests passed! ===\n");
    return 0;
}
