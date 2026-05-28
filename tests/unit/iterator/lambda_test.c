#include "my_c_utils/my_c_utils.h"
#include <assert.h>
#include <stdio.h>

VECTOR_CONFIG(Int)
MAPPED_CONFIG(Vector(Int), Int)
FILTERED_CONFIG(Vector(Int), Int)

static Int global_sum = 0;

int main(void)
{
  printf("=== Starting Lambda & Functional Iterator Unit Tests ===\n");

  // Setup a vector with [10, 20, 30, 40]
  Vector(Int) vec = Vector_new(Int)();
  (void)Vector_push_back(Int)(&vec, 10);
  (void)Vector_push_back(Int)(&vec, 20);
  (void)Vector_push_back(Int)(&vec, 30);
  (void)Vector_push_back(Int)(&vec, 40);

  // 1. Test basic lambda macro expansion and evaluation directly
  printf("TEST 1: Direct Lambda Evaluation... ");
  ref(Int) my_ref = &global_sum;
  global_sum = 25;
  Int (*read_val)(cref(Int)) = lambda(Int, (cref(Int) x), { return cref_deref(Int)(x); });
  assert(read_val(my_ref) == 25);
  printf("✓\n");

  // 2. Test lazy map and collect
  printf("TEST 2: Lazy map and collect... ");
  iter_Vector(Int) it = Vector_into_iter(Int)(vec);
  iter_Mapped(Vector(Int), Int) mapped_it = map(Vector(Int), Int)(it, lambda(Int, (cref(Int) x), { return cref_deref(Int)(x) * 2; }));
  
  // The original vector itself must NOT be mutated yet!
  assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&vec, 0))) == 10);
  assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&vec, 1))) == 20);
  assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&vec, 2))) == 30);
  assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&vec, 3))) == 40);

  // Collect the mapped iterator into a new vector
  Vector(Int) collected = collect_new(Vector(Int), Mapped(Vector(Int), Int))(mapped_it);

  // Verify the collected mapped values
  assert(Vector_size(Int)(&collected) == 4);
  assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&collected, 0))) == 20);
  assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&collected, 1))) == 40);
  assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&collected, 2))) == 60);
  assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&collected, 3))) == 80);



  vec = collected;
  printf("✓\n");

  // 3. Test filter and filter_new with lambdas (by reference)
  printf("TEST 3: filter and filter_new with lambdas... ");
  Vector(Int) filtered_in_place = Vector_new(Int)();
  collect(Vector(Int), Filtered(Vector(Int)))(&filtered_in_place, filter(Vector(Int))(Vector_into_iter(Int)(Vector_clone(Int)(&vec)), lambda(bool, (cref(Int) x), { return cref_deref(Int)(x) > 50; })));
  assert(Vector_size(Int)(&filtered_in_place) == 2);
  assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&filtered_in_place, 0))) == 60);
  assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&filtered_in_place, 1))) == 80);
  Vector_free(Int)(&filtered_in_place);

  Vector(Int) filtered_inline = filter_new(Vector(Int), Vector(Int))(Vector_into_iter(Int)(Vector_clone(Int)(&vec)), lambda(bool, (cref(Int) x), { return cref_deref(Int)(x) < 50; }));
  assert(Vector_size(Int)(&filtered_inline) == 2);
  assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&filtered_inline, 0))) == 20);
  assert(ref_deref(Int)(Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&filtered_inline, 1))) == 40);
  Vector_free(Int)(&filtered_inline);
  printf("✓\n");

  // 4. Test fold (reduction) with two-parameter lambdas (element by reference)
  printf("TEST 4: fold with lambdas... ");
  Int sum_value = fold(Vector(Int), Int)(0, Vector_into_iter(Int)(Vector_clone(Int)(&vec)), lambda(Int, (Int acc, cref(Int) x), { return acc + cref_deref(Int)(x); }));
  assert(sum_value == 200); // 20 + 40 + 60 + 80

  Int folded_subtraction = fold(Vector(Int), Int)(1000, Vector_into_iter(Int)(Vector_clone(Int)(&vec)), lambda(Int, (Int acc, cref(Int) x), { return acc - cref_deref(Int)(x); }));
  assert(folded_subtraction == 800); // 1000 - 200
  printf("✓\n");

  // 5. Test any and all predicates with lambdas (by reference)
  printf("TEST 5: any and all predicates with lambdas... ");
  bool contains_sixty = any(Vector(Int))(Vector_into_iter(Int)(Vector_clone(Int)(&vec)), lambda(bool, (cref(Int) x), { return cref_deref(Int)(x) == 60; }));
  assert(contains_sixty);

  bool contains_hundred = any(Vector(Int))(Vector_into_iter(Int)(Vector_clone(Int)(&vec)), lambda(bool, (cref(Int) x), { return cref_deref(Int)(x) == 100; }));
  assert(!contains_hundred);

  bool all_are_positive = all(Vector(Int))(Vector_into_iter(Int)(Vector_clone(Int)(&vec)), lambda(bool, (cref(Int) x), { return cref_deref(Int)(x) > 0; }));
  assert(all_are_positive);

  bool all_are_above_thirty = all(Vector(Int))(Vector_into_iter(Int)(Vector_clone(Int)(&vec)), lambda(bool, (cref(Int) x), { return cref_deref(Int)(x) > 30; }));
  assert(!all_are_above_thirty);
  printf("✓\n");

  // 6. Test functional callback looping (for_each_fn) with side effects (by reference)
  printf("TEST 6: for_each_fn callback loop with lambdas... ");
  global_sum = 0;
  for_each_fn(Vector(Int))(&vec, lambda(void, (cref(Int) x), { global_sum += cref_deref(Int)(x); }));
  assert(global_sum == 200);
  printf("✓\n");

  printf("=== All Lambda & Functional Iterator Unit Tests Passed! ===\n");
  return 0;
}
