#include "my_c_utils/my_c_utils.h"
#include <assert.h>
#include <stdio.h>

VECTOR_CONFIG(Int)
LINKED_LIST_CONFIG(Int)
HASH_MAP_CONFIG(String, Int)
OPTION_CONFIG(Int)

int main(void)
{
  printf("=== Starting Triple Iterator Interface Tests ===\n");

  // Setup vector: [10, 20, 30]
  Vector(Int) vec = Vector_new(Int)();
  (void)Vector_push_back(Int)(&vec, 10);
  (void)Vector_push_back(Int)(&vec, 20);
  (void)Vector_push_back(Int)(&vec, 30);

  // Setup list: [100, 200, 300]
  List(Int) list = List_new(Int)();
  (void)List_push_back(Int)(&list, 100);
  (void)List_push_back(Int)(&list, 200);
  (void)List_push_back(Int)(&list, 300);

  // 1. Vector: Value Iterator (_val) and for_each_val
  {
    printf("TEST 1: Vector Consuming Value Iterator (_val)... ");
    Int sum = 0;
    for_each_val(Vector(Int), item, Vector_clone(Int)(&vec)) {
      sum += item;
    }
    assert(sum == 60);
    printf("✓\n");
  }

  // 2. Vector: Mutable Reference Iterator (_mut) and for_each_mut
  {
    printf("TEST 2: Vector Mutable Reference Iterator (_mut)... ");
    Vector(Int) local_vec = Vector_clone(Int)(&vec);
    for_each_mut(Vector(Int), item_ref, &local_vec) {
      *item_ref = ref_deref(Int)(item_ref) * 2;
    }
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&local_vec, 0))) == 20);
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&local_vec, 1))) == 40);
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&local_vec, 2))) == 60);
    Vector_free(Int)(&local_vec);
    printf("✓\n");
  }

  // 3. Vector: Constant Reference Iterator (_const) and for_each_const
  {
    printf("TEST 3: Vector Constant Reference Iterator (_const)... ");
    Int sum = 0;
    for_each_const(Vector(Int), item_cref, &vec) {
      sum += cref_deref(Int)(item_cref);
    }
    assert(sum == 60);
    printf("✓\n");
  }

  // 4. Linked List: Value Iterator (_val) and for_each_val
  {
    printf("TEST 4: LinkedList Consuming Value Iterator (_val)... ");
    Int sum = 0;
    for_each_val(List(Int), item, List_clone(Int)(&list)) {
      sum += item;
    }
    assert(sum == 600);
    printf("✓\n");
  }

  // 5. Linked List: Mutable Reference Iterator (_mut) and for_each_mut
  {
    printf("TEST 5: LinkedList Mutable Reference Iterator (_mut)... ");
    List(Int) local_list = List_clone(Int)(&list);
    for_each_mut(List(Int), item_ref, &local_list) {
      *item_ref = ref_deref(Int)(item_ref) * 2;
    }
    // Convert to vector to assert easily
    Vector(Int) v_collected = Vector_new(Int)();
    collect(Vector(Int), List(Int))(&v_collected, List_into_iter(Int)(local_list));
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&v_collected, 0))) == 200);
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&v_collected, 1))) == 400);
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&v_collected, 2))) == 600);
    Vector_free(Int)(&v_collected);
    printf("✓\n");
  }

  // 6. Linked List: Constant Reference Iterator (_const) and for_each_const
  {
    printf("TEST 6: LinkedList Constant Reference Iterator (_const)... ");
    Int sum = 0;
    for_each_const(List(Int), item_cref, &list) {
      sum += cref_deref(Int)(item_cref);
    }
    assert(sum == 600);
    printf("✓\n");
  }

  // 7. Hashmap: _val, _mut, _const
  {
    printf("TEST 7: Hashmap Consuming Value Iterator (_val)... ");
    Hashmap(String, Int) hm = Hashmap_new(String, Int)(4);
    (void)Hashmap_add(String, Int)(&hm, string_dup("one"), 10);
    (void)Hashmap_add(String, Int)(&hm, string_dup("two"), 20);
    (void)Hashmap_add(String, Int)(&hm, string_dup("three"), 30);

    Int sum = 0;
    for_each_val(Hashmap(String, Int), item, hm) {
      sum += item;
    }
    assert(sum == 60);
    printf("✓\n");
  }
  {
    printf("TEST 8: Hashmap Mutable Reference Iterator (_mut)... ");
    Hashmap(String, Int) hm = Hashmap_new(String, Int)(4);
    (void)Hashmap_add(String, Int)(&hm, string_dup("one"), 10);
    (void)Hashmap_add(String, Int)(&hm, string_dup("two"), 20);

    for_each_mut(Hashmap(String, Int), item_ref, &hm) {
      *item_ref = ref_deref(Int)(item_ref) * 2;
    }

    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Hashmap_get(String, Int)(&hm, "one"))) == 20);
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Hashmap_get(String, Int)(&hm, "two"))) == 40);

    Hashmap_free(String, Int)(&hm);
    printf("✓\n");
  }
  {
    printf("TEST 9: Hashmap Constant Reference Iterator (_const)... ");
    Hashmap(String, Int) hm = Hashmap_new(String, Int)(4);
    (void)Hashmap_add(String, Int)(&hm, string_dup("one"), 10);
    (void)Hashmap_add(String, Int)(&hm, string_dup("two"), 20);

    Int sum = 0;
    for_each_const(Hashmap(String, Int), item_cref, &hm) {
      sum += cref_deref(Int)(item_cref);
    }
    assert(sum == 30);
    Hashmap_free(String, Int)(&hm);
    printf("✓\n");
  }

  // 10. Option: _val, _mut, _const
  {
    printf("TEST 10: Option Consuming Value Iterator (_val)... ");
    Option(Int) opt_some = Option_some(Int)(42);
    Option(Int) opt_none = Option_none(Int)();

    Int sum = 0;
    for_each_val(Option(Int), item, opt_some) {
      sum += item;
    }
    assert(sum == 42);

    Int count = 0;
    for_each_val(Option(Int), item, opt_none) {
      (void)item;
      count++;
    }
    assert(count == 0);
    printf("✓\n");
  }
  {
    printf("TEST 11: Option Mutable Reference Iterator (_mut)... ");
    Option(Int) opt = Option_some(Int)(10);
    for_each_mut(Option(Int), item_ref, &opt) {
      *item_ref = 100;
    }
    assert(Option_unwrap(Int)(opt) == 100);
    Option_free(Int)(&opt);
    printf("✓\n");
  }
  {
    printf("TEST 12: Option Constant Reference Iterator (_const)... ");
    Option(Int) opt = Option_some(Int)(200);
    Int val = 0;
    for_each_const(Option(Int), item_cref, &opt) {
      val = cref_deref(Int)(item_cref);
    }
    assert(val == 200);
    Option_free(Int)(&opt);
    printf("✓\n");
  }

  // 13. Result: _val, _mut, _const
  {
    printf("TEST 13: Result Consuming Value Iterator (_val)... ");
    Result(Int, cref(Char)) res_ok = Result_ok(Int, cref(Char))(42);
    Result(Int, cref(Char)) res_err = Result_err(Int, cref(Char))("error");

    Int sum = 0;
    for_each_val(Result(Int, cref(Char)), item, res_ok) {
      sum += item;
    }
    assert(sum == 42);

    Int count = 0;
    for_each_val(Result(Int, cref(Char)), item, res_err) {
      (void)item;
      count++;
    }
    assert(count == 0);
    printf("✓\n");
  }
  {
    printf("TEST 14: Result Mutable Reference Iterator (_mut)... ");
    Result(Int, cref(Char)) res = Result_ok(Int, cref(Char))(10);
    for_each_mut(Result(Int, cref(Char)), item_ref, &res) {
      *item_ref = 100;
    }
    assert(Result_unwrap(Int, cref(Char))(res) == 100);
    Result_free(Int, cref(Char))(&res);
    printf("✓\n");
  }
  {
    printf("TEST 15: Result Constant Reference Iterator (_const)... ");
    Result(Int, cref(Char)) res = Result_ok(Int, cref(Char))(200);
    Int val = 0;
    for_each_const(Result(Int, cref(Char)), item_cref, &res) {
      val = cref_deref(Int)(item_cref);
    }
    assert(val == 200);
    Result_free(Int, cref(Char))(&res);
    printf("✓\n");
  }

  Vector_free(Int)(&vec);
  List_free(Int)(&list);

  printf("=== All Triple Iterator Interface Tests Passed! ===\n");
  return 0;
}
