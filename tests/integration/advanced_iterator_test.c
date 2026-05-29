#include "my_c_utils/my_c_utils.h"
#include <assert.h>
#include <stdio.h>

VECTOR_CONFIG(Int)
LINKED_LIST_CONFIG(Int)

TAKE_CONFIG(Vector(Int), Int)
SKIP_CONFIG(Vector(Int), Int)
ENUMERATE_CONFIG(Vector(Int), Int)
ZIP_CONFIG(List(Int), Vector(Int), Int, Int)
FILTERED_CONFIG(Skip(Vector(Int)), Int)
MAPPED_CONFIG(Filtered(Skip(Vector(Int))), Int)


int main(void)
{
  printf("=== Starting Advanced Lazy Iterator Adapter Tests ===\n");

  // Setup vector: [10, 20, 30, 40, 50]
  Vector(Int) vec = Vector_new(Int)();
  (void)Vector_push_back(Int)(&vec, 10);
  (void)Vector_push_back(Int)(&vec, 20);
  (void)Vector_push_back(Int)(&vec, 30);
  (void)Vector_push_back(Int)(&vec, 40);
  (void)Vector_push_back(Int)(&vec, 50);

  // Setup list: [100, 200, 300]
  List(Int) list = List_new(Int)();
  (void)List_push_back(Int)(&list, 100);
  (void)List_push_back(Int)(&list, 200);
  (void)List_push_back(Int)(&list, 300);

  // 1. Test Lazy take
  {
    printf("TEST 1: Lazy take... ");
    iter_Vector(Int) it = Vector_into_iter(Int)(Vector_clone(Int)(&vec));
    iter_Take(Vector(Int)) take_it = take(Vector(Int))(it, 3);

    Vector(Int) taken = collect_new(Vector(Int), Take(Vector(Int)))(take_it);
    assert(Vector_size(Int)(&taken) == 3);
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&taken, 0))) == 10);
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&taken, 1))) == 20);
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&taken, 2))) == 30);

    Vector_free(Int)(&taken);
    printf("✓\n");
  }

  // 2. Test Lazy skip
  {
    printf("TEST 2: Lazy skip... ");
    iter_Vector(Int) it = Vector_into_iter(Int)(Vector_clone(Int)(&vec));
    iter_Skip(Vector(Int)) skip_it = skip(Vector(Int))(it, 2);

    Vector(Int) skipped = collect_new(Vector(Int), Skip(Vector(Int)))(skip_it);
    assert(Vector_size(Int)(&skipped) == 3);
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&skipped, 0))) == 30);
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&skipped, 1))) == 40);
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&skipped, 2))) == 50);

    Vector_free(Int)(&skipped);
    printf("✓\n");
  }

  // 3. Test Lazy enumerate
  {
    printf("TEST 3: Lazy enumerate... ");
    iter_Vector(Int) it = Vector_into_iter(Int)(Vector_clone(Int)(&vec));
    iter_Enumerate(Vector(Int), Int) enum_it = enumerate(Vector(Int), Int)(it);

    Size expected_idx = 0;
    Int expected_val = 10;
    
    Result(ref(Pair(Size, ref(Int))), cref(Char)) res = iter_next(Enumerate(Vector(Int), Int))(&enum_it);
    while (!res.is_error) {
      ref(Pair(Size, ref(Int))) pair_ref = Result_unwrap(ref(Pair(Size, ref(Int))), cref(Char))(res);
      Pair(Size, ref(Int)) pair = ref_deref(Pair(Size, ref(Int)))(pair_ref);
      assert(pair.first == expected_idx);
      assert(ref_deref(Int)(pair.second) == expected_val);
      
      expected_idx++;
      expected_val += 10;
      res = iter_next(Enumerate(Vector(Int), Int))(&enum_it);
    }
    assert(expected_idx == 5);
    iter_free(Enumerate(Vector(Int), Int))(&enum_it);
    printf("✓\n");
  }

  // 4. Test Lazy zip
  {
    printf("TEST 4: Lazy zip... ");
    iter_List(Int) it1 = List_into_iter(Int)(List_clone(Int)(&list));
    iter_Vector(Int) it2 = Vector_into_iter(Int)(Vector_clone(Int)(&vec));
    
    iter_Zip(List(Int), Vector(Int), Int, Int) zip_it = zip(List(Int), Vector(Int), Int, Int)(it1, it2);

    Int expected_v1 = 100;
    Int expected_v2 = 10;
    Size count = 0;

    Result(ref(Pair(ref(Int), ref(Int))), cref(Char)) res = iter_next(Zip(List(Int), Vector(Int), Int, Int))(&zip_it);
    while (!res.is_error) {
      ref(Pair(ref(Int), ref(Int))) pair_ref = Result_unwrap(ref(Pair(ref(Int), ref(Int))), cref(Char))(res);
      Pair(ref(Int), ref(Int)) pair = ref_deref(Pair(ref(Int), ref(Int)))(pair_ref);
      assert(ref_deref(Int)(pair.first) == expected_v1);
      assert(ref_deref(Int)(pair.second) == expected_v2);

      expected_v1 += 100;
      expected_v2 += 10;
      count++;
      res = iter_next(Zip(List(Int), Vector(Int), Int, Int))(&zip_it);
    }
    // Zip must terminate when list runs out (3 elements), even though vector has 5
    assert(count == 3);
    iter_free(Zip(List(Int), Vector(Int), Int, Int))(&zip_it);
    printf("✓\n");
  }

  // 5. Test Declarative pipeline composition
  {
    printf("TEST 5: Declarative pipeline composition... ");
    Vector(Int) pipeline_res = pipeline(
        Vector_into_iter(Int)(Vector_clone(Int)(&vec)),
        skip(Vector(Int))(_val, 1),
        filter(Skip(Vector(Int)))(_val, lambda(bool, (cref(Int) x), { return cref_deref(Int)(x) > 20; })),
        map(Filtered(Skip(Vector(Int))), Int)(_val, lambda(Int, (cref(Int) x), { return cref_deref(Int)(x) * 2; })),
        collect_new(Vector(Int), Mapped(Filtered(Skip(Vector(Int))), Int))(_val)
    );

    assert(Vector_size(Int)(&pipeline_res) == 3);
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&pipeline_res, 0))) == 60);
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&pipeline_res, 1))) == 80);
    assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&pipeline_res, 2))) == 100);

    Vector_free(Int)(&pipeline_res);
    printf("✓\n");
  }

  Vector_free(Int)(&vec);
  List_free(Int)(&list);

  printf("=== All Advanced Lazy Iterator Adapter Tests Passed! ===\n");
  return 0;
}
