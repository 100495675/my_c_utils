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
   containers because Result_Void_ref_Int is pre-configured in primitives.h,
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
    Vector_Int vec = Vector_Int_new();
    
    // Insert in empty vector (index 0)
    Result_Void r = Vector_Int_insert_at(&vec, 0, 10);
    assert(Result_Void_is_ok(&r));
    assert(vec.size == 1);
    
    // Insert at end (index 1)
    r = Vector_Int_insert_at(&vec, 1, 30);
    assert(Result_Void_is_ok(&r));
    assert(vec.size == 2);
    
    // Insert in middle (index 1)
    r = Vector_Int_insert_at(&vec, 1, 20);
    assert(Result_Void_is_ok(&r));
    assert(vec.size == 3);
    
    // Insert at beginning (index 0)
    r = Vector_Int_insert_at(&vec, 0, 5);
    assert(Result_Void_is_ok(&r));
    assert(vec.size == 4);
    
    // Verify values: 5, 10, 20, 30
    assert(*Vector_Int_at(&vec, 0).value == 5);
    assert(*Vector_Int_at(&vec, 1).value == 10);
    assert(*Vector_Int_at(&vec, 2).value == 20);
    assert(*Vector_Int_at(&vec, 3).value == 30);
    
    // Bounds check
    r = Vector_Int_insert_at(&vec, 5, 99);
    assert(Result_Void_is_err(&r));
    
    Vector_Int_free(&vec);
    printf("✓\n");
}

void test_vector_remove_at(void)
{
    printf("TEST: Vector_Int_remove_at... ");
    Vector_Int vec = Vector_Int_new();
    Vector_Int_push_back(&vec, 10);
    Vector_Int_push_back(&vec, 20);
    Vector_Int_push_back(&vec, 30);
    Vector_Int_push_back(&vec, 40);
    
    // Remove from middle (index 1) -> 10, 30, 40
    Result_Void r = Vector_Int_remove_at(&vec, 1);
    assert(Result_Void_is_ok(&r));
    assert(vec.size == 3);
    assert(*Vector_Int_at(&vec, 0).value == 10);
    assert(*Vector_Int_at(&vec, 1).value == 30);
    assert(*Vector_Int_at(&vec, 2).value == 40);
    
    // Remove from beginning (index 0) -> 30, 40
    r = Vector_Int_remove_at(&vec, 0);
    assert(Result_Void_is_ok(&r));
    assert(vec.size == 2);
    assert(*Vector_Int_at(&vec, 0).value == 30);
    
    // Remove from end (index 1) -> 30
    r = Vector_Int_remove_at(&vec, 1);
    assert(Result_Void_is_ok(&r));
    assert(vec.size == 1);
    assert(*Vector_Int_at(&vec, 0).value == 30);
    
    // Bounds check
    r = Vector_Int_remove_at(&vec, 1);
    assert(Result_Void_is_err(&r));
    
    Vector_Int_free(&vec);
    printf("✓\n");
}

void test_vector_shrink_to_fit(void)
{
    printf("TEST: Vector_Int_shrink_to_fit... ");
    Vector_Int vec = Vector_Int_new();
    
    // Test on empty vector
    Result_Void r = Vector_Int_shrink_to_fit(&vec);
    assert(Result_Void_is_ok(&r));
    assert(vec.capacity == 0);
    
    Vector_Int_push_back(&vec, 10);
    Vector_Int_push_back(&vec, 20);
    assert(vec.capacity >= 2);
    
    Vector_Int_reserve(&vec, 100);
    assert(vec.capacity >= 100);
    
    r = Vector_Int_shrink_to_fit(&vec);
    assert(Result_Void_is_ok(&r));
    assert(vec.capacity == vec.size);
    assert(*Vector_Int_at(&vec, 0).value == 10);
    assert(*Vector_Int_at(&vec, 1).value == 20);
    
    Vector_Int_free(&vec);
    printf("✓\n");
}

void test_vector_sort(void)
{
    printf("TEST: Vector_Int_sort... ");
    Vector_Int vec = Vector_Int_new();
    Vector_Int_push_back(&vec, 30);
    Vector_Int_push_back(&vec, 10);
    Vector_Int_push_back(&vec, 40);
    Vector_Int_push_back(&vec, 20);
    
    Vector_Int_sort(&vec, compare_ints);
    
    assert(*Vector_Int_at(&vec, 0).value == 10);
    assert(*Vector_Int_at(&vec, 1).value == 20);
    assert(*Vector_Int_at(&vec, 2).value == 30);
    assert(*Vector_Int_at(&vec, 3).value == 40);
    
    Vector_Int_free(&vec);
    printf("✓\n");
}

/* ========== Test: Linked List Extensions ========== */

void test_linked_list_push_pop_front(void)
{
    printf("TEST: List_Int_push_front and pop_front... ");
    List_Int list = List_Int_new();
    
    // push_front
    Result_Void r = List_Int_push_front(&list, 10);
    assert(Result_Void_is_ok(&r));
    r = List_Int_push_front(&list, 20);
    assert(Result_Void_is_ok(&r));
    r = List_Int_push_front(&list, 30);
    assert(Result_Void_is_ok(&r));
    
    assert(list.size == 3);
    
    // pop_front
    Result_Void_Int pop_res = List_Int_pop_front(&list);
    assert(!pop_res.is_error);
    assert(pop_res.value == 30);
    
    pop_res = List_Int_pop_front(&list);
    assert(!pop_res.is_error);
    assert(pop_res.value == 20);
    
    pop_res = List_Int_pop_front(&list);
    assert(!pop_res.is_error);
    assert(pop_res.value == 10);
    
    assert(list.size == 0);
    
    // Pop on empty list
    pop_res = List_Int_pop_front(&list);
    assert(pop_res.is_error);
    
    List_Int_free(&list);
    printf("✓\n");
}

void test_linked_list_push_back_result(void)
{
    printf("TEST: List_Int_push_back returning Result_Void... ");
    List_Int list = List_Int_new();
    
    Result_Void r = List_Int_push_back(&list, 42);
    assert(Result_Void_is_ok(&r));
    assert(list.size == 1);
    
    Result_Void_Int pop_res = List_Int_pop_back(&list);
    assert(!pop_res.is_error);
    assert(pop_res.value == 42);
    
    List_Int_free(&list);
    printf("✓\n");
}

/* ========== Test: Hash Map Extensions ========== */

void test_hash_map_get_mut(void)
{
    printf("TEST: Hashmap_Int_Int_get_mut... ");
    Hashmap_Int_Int map = Hashmap_Int_Int_new(4);
    
    Result_Void r = Hashmap_Int_Int_add(&map, 10, 100);
    assert(Result_Void_is_ok(&r));
    
    // Retrieve mutable pointer and modify value
    Result_Void_ref_Int get_res = Hashmap_Int_Int_get_mut(&map, 10);
    assert(!get_res.is_error);
    assert(*get_res.value == 100);
    
    // Modify through the returned pointer
    *get_res.value = 999;
    
    // Get again to verify modification
    get_res = Hashmap_Int_Int_get(&map, 10);
    assert(!get_res.is_error);
    assert(*get_res.value == 999);
    
    Hashmap_Int_Int_free(&map);
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
