#include "my_c_utils/my_c_utils.h"
#include <assert.h>
#include <stdio.h>

VECTOR_CONFIG(Int)

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
  int (*double_val)(int) = lambda(int, (int x), { return x * 2; });
  assert(double_val(5) == 10);
  assert(double_val(25) == 50);
  printf("✓\n");

  // 2. Test map and map_new with lambdas
  printf("TEST 2: map and map_new with lambdas... ");
  Vector(Int) mapped_in_place = Vector_new(Int)();
  map(Vector(Int), Vector(Int))(&mapped_in_place, Vector_into_iter(Int)(&vec), lambda(Int, (Int x), { return x + 5; }));
  assert(Vector_size(Int)(&mapped_in_place) == 4);
  assert(*Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&mapped_in_place, 0)) == 15);
  assert(*Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&mapped_in_place, 3)) == 45);
  Vector_free(Int)(&mapped_in_place);

  Vector(Int) mapped_inline = map_new(Vector(Int), Vector(Int))(Vector_into_iter(Int)(&vec), lambda(Int, (Int x), { return x * 10; }));
  assert(Vector_size(Int)(&mapped_inline) == 4);
  assert(*Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&mapped_inline, 0)) == 100);
  assert(*Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&mapped_inline, 3)) == 400);
  Vector_free(Int)(&mapped_inline);
  printf("✓\n");

  // 3. Test filter and filter_new with lambdas
  printf("TEST 3: filter and filter_new with lambdas... ");
  Vector(Int) filtered_in_place = Vector_new(Int)();
  filter(Vector(Int), Vector(Int))(&filtered_in_place, Vector_into_iter(Int)(&vec), lambda(bool, (Int x), { return x > 25; }));
  assert(Vector_size(Int)(&filtered_in_place) == 2);
  assert(*Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&filtered_in_place, 0)) == 30);
  assert(*Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&filtered_in_place, 1)) == 40);
  Vector_free(Int)(&filtered_in_place);

  Vector(Int) filtered_inline = filter_new(Vector(Int), Vector(Int))(Vector_into_iter(Int)(&vec), lambda(bool, (Int x), { return x < 25; }));
  assert(Vector_size(Int)(&filtered_inline) == 2);
  assert(*Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&filtered_inline, 0)) == 10);
  assert(*Result_unwrap(ref(Int), cref(Char))(Vector_at(Int)(&filtered_inline, 1)) == 20);
  Vector_free(Int)(&filtered_inline);
  printf("✓\n");

  // 4. Test fold (reduction) with two-parameter lambdas
  printf("TEST 4: fold with lambdas... ");
  Int sum_value = fold(Vector(Int), Int)(0, Vector_into_iter(Int)(&vec), lambda(Int, (Int acc, Int x), { return acc + x; }));
  assert(sum_value == 100); // 10 + 20 + 30 + 40

  Int folded_subtraction = fold(Vector(Int), Int)(1000, Vector_into_iter(Int)(&vec), lambda(Int, (Int acc, Int x), { return acc - x; }));
  assert(folded_subtraction == 900); // 1000 - 100
  printf("✓\n");

  // 5. Test any and all predicates with lambdas
  printf("TEST 5: any and all predicates with lambdas... ");
  bool contains_thirty = any(Vector(Int))(Vector_into_iter(Int)(&vec), lambda(bool, (Int x), { return x == 30; }));
  assert(contains_thirty);

  bool contains_hundred = any(Vector(Int))(Vector_into_iter(Int)(&vec), lambda(bool, (Int x), { return x == 100; }));
  assert(!contains_hundred);

  bool all_are_positive = all(Vector(Int))(Vector_into_iter(Int)(&vec), lambda(bool, (Int x), { return x > 0; }));
  assert(all_are_positive);

  bool all_are_above_fifteen = all(Vector(Int))(Vector_into_iter(Int)(&vec), lambda(bool, (Int x), { return x > 15; }));
  assert(!all_are_above_fifteen);
  printf("✓\n");

  // 6. Test functional callback looping (for_each_fn) with side effects
  printf("TEST 6: for_each_fn callback loop with lambdas... ");
  global_sum = 0;
  for_each_fn(Vector(Int))(&vec, lambda(void, (Int x), { global_sum += x; }));
  assert(global_sum == 100);
  printf("✓\n");

  Vector_free(Int)(&vec);
  printf("=== All Lambda & Functional Iterator Unit Tests Passed! ===\n");
  return 0;
}
