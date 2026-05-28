#include "my_c_utils/my_c_utils.h"
#include <assert.h>

VECTOR_CONFIG(Int)
LINKED_LIST_CONFIG(Int)

int main(void)
{
  printf("=== Starting Collect System Tests ===\n");

  // ==========================================
  // Test 1: In-place Collect from List to Vector
  // ==========================================
  {
    printf("TEST 1: In-place Collect from List to Vector... ");
    List(Int) list = List_new(Int)();
    
    Result(Void, cref(Char)) r1 = List_push_back(Int)(&list, 10);
    assert(Result_is_ok(Void, cref(Char))(&r1));
    Result_free(Void, cref(Char))(&r1);
    
    Result(Void, cref(Char)) r2 = List_push_back(Int)(&list, 20);
    assert(Result_is_ok(Void, cref(Char))(&r2));
    Result_free(Void, cref(Char))(&r2);
    
    Result(Void, cref(Char)) r3 = List_push_back(Int)(&list, 30);
    assert(Result_is_ok(Void, cref(Char))(&r3));
    Result_free(Void, cref(Char))(&r3);

    Vector(Int) vec = Vector_new(Int)();
    iter_List(Int) it = List_into_iter(Int)(&list);

    collect(Vector(Int), List(Int))(&vec, it);

    assert(Vector_size(Int)(&vec) == 3);
    
    Result(ref(Int), cref(Char)) v0 = Vector_at(Int)(&vec, 0);
    assert(Result_is_ok(ref(Int), cref(Char))(&v0));
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(v0)) == 10);
    
    Result(ref(Int), cref(Char)) v1 = Vector_at(Int)(&vec, 1);
    assert(Result_is_ok(ref(Int), cref(Char))(&v1));
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(v1)) == 20);
    
    Result(ref(Int), cref(Char)) v2 = Vector_at(Int)(&vec, 2);
    assert(Result_is_ok(ref(Int), cref(Char))(&v2));
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(v2)) == 30);

    Vector_free(Int)(&vec);
    List_free(Int)(&list);
    printf("✓\n");
  }

  // ==========================================
  // Test 2: Inline collect_new from List to Vector
  // ==========================================
  {
    printf("TEST 2: Inline collect_new from List to Vector... ");
    List(Int) list = List_new(Int)();
    
    Result(Void, cref(Char)) r1 = List_push_back(Int)(&list, 15);
    assert(Result_is_ok(Void, cref(Char))(&r1));
    Result_free(Void, cref(Char))(&r1);
    
    Result(Void, cref(Char)) r2 = List_push_back(Int)(&list, 25);
    assert(Result_is_ok(Void, cref(Char))(&r2));
    Result_free(Void, cref(Char))(&r2);

    iter_List(Int) it = List_into_iter(Int)(&list);
    Vector(Int) vec = collect_new(Vector(Int), List(Int))(it);

    assert(Vector_size(Int)(&vec) == 2);
    
    Result(ref(Int), cref(Char)) v0 = Vector_at(Int)(&vec, 0);
    assert(Result_is_ok(ref(Int), cref(Char))(&v0));
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(v0)) == 15);
    
    Result(ref(Int), cref(Char)) v1 = Vector_at(Int)(&vec, 1);
    assert(Result_is_ok(ref(Int), cref(Char))(&v1));
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(v1)) == 25);

    Vector_free(Int)(&vec);
    List_free(Int)(&list);
    printf("✓\n");
  }

  // ==========================================
  // Test 3: In-place Collect from Vector to List
  // ==========================================
  {
    printf("TEST 3: In-place Collect from Vector to List... ");
    Vector(Int) vec = Vector_new(Int)();
    
    Result(Void, cref(Char)) r1 = Vector_push_back(Int)(&vec, 100);
    assert(Result_is_ok(Void, cref(Char))(&r1));
    Result_free(Void, cref(Char))(&r1);
    
    Result(Void, cref(Char)) r2 = Vector_push_back(Int)(&vec, 200);
    assert(Result_is_ok(Void, cref(Char))(&r2));
    Result_free(Void, cref(Char))(&r2);

    List(Int) list = List_new(Int)();
    iter_Vector(Int) it = Vector_into_iter(Int)(&vec);

    collect(List(Int), Vector(Int))(&list, it);

    assert(List_size(Int)(&list) == 2);

    Int sum = 0;
    for_each(List(Int), item, &list) {
      sum += item;
    }
    assert(sum == 300);

    List_free(Int)(&list);
    Vector_free(Int)(&vec);
    printf("✓\n");
  }

  // ==========================================
  // Test 4: Inline collect_new from Vector to List
  // ==========================================
  {
    printf("TEST 4: Inline collect_new from Vector to List... ");
    Vector(Int) vec = Vector_new(Int)();
    
    Result(Void, cref(Char)) r1 = Vector_push_back(Int)(&vec, 1000);
    assert(Result_is_ok(Void, cref(Char))(&r1));
    Result_free(Void, cref(Char))(&r1);
    
    Result(Void, cref(Char)) r2 = Vector_push_back(Int)(&vec, 2000);
    assert(Result_is_ok(Void, cref(Char))(&r2));
    Result_free(Void, cref(Char))(&r2);

    iter_Vector(Int) it = Vector_into_iter(Int)(&vec);
    List(Int) list = collect_new(List(Int), Vector(Int))(it);

    assert(List_size(Int)(&list) == 2);

    Int sum = 0;
    for_each(List(Int), item, &list) {
      sum += item;
    }
    assert(sum == 3000);

    List_free(Int)(&list);
    Vector_free(Int)(&vec);
    printf("✓\n");
  }

  // ==========================================
  // Test 5: Collect Inline Iterator Creation
  // ==========================================
  {
    printf("TEST 5: Collect inline iterator creation... ");
    List(Int) list = List_new(Int)();
    
    Result(Void, cref(Char)) r1 = List_push_back(Int)(&list, 70);
    assert(Result_is_ok(Void, cref(Char))(&r1));
    Result_free(Void, cref(Char))(&r1);
    
    Result(Void, cref(Char)) r2 = List_push_back(Int)(&list, 80);
    assert(Result_is_ok(Void, cref(Char))(&r2));
    Result_free(Void, cref(Char))(&r2);

    // Call collect_new passing List_into_iter directly!
    Vector(Int) vec = collect_new(Vector(Int), List(Int))(List_into_iter(Int)(&list));

    assert(Vector_size(Int)(&vec) == 2);
    
    Result(ref(Int), cref(Char)) v0 = Vector_at(Int)(&vec, 0);
    assert(Result_is_ok(ref(Int), cref(Char))(&v0));
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(v0)) == 70);
    
    Result(ref(Int), cref(Char)) v1 = Vector_at(Int)(&vec, 1);
    assert(Result_is_ok(ref(Int), cref(Char))(&v1));
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(v1)) == 80);

    Vector_free(Int)(&vec);
    List_free(Int)(&list);
    printf("✓\n");
  }

  // ==========================================
  // Test 6: Single-Parenthesis for_each Loops
  // ==========================================
  {
    printf("TEST 6: Single-Parenthesis for_each Loops... ");
    Vector(Int) vec = Vector_new(Int)();
    
    Result(Void, cref(Char)) r1 = Vector_push_back(Int)(&vec, 400);
    assert(Result_is_ok(Void, cref(Char))(&r1));
    Result_free(Void, cref(Char))(&r1);
    
    Result(Void, cref(Char)) r2 = Vector_push_back(Int)(&vec, 500);
    assert(Result_is_ok(Void, cref(Char))(&r2));
    Result_free(Void, cref(Char))(&r2);

    Int sum = 0;
    for_each(Vector(Int), item, &vec) {
      sum += item;
    }
    assert(sum == 900);

    Int sum_ref = 0;
    for_each_ref(Vector(Int), item_ref, &vec) {
      sum_ref += ref_deref(Int)(item_ref);
    }
    assert(sum_ref == 900);

    Vector_free(Int)(&vec);
    printf("✓\n");
  }

  printf("=== All Collect System Tests Passed! ===\n");
  return 0;
}
