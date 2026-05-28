#include "my_c_utils/free.h"
#include "my_c_utils/linked_list.h"
#include <assert.h>

LINKED_LIST_CONFIG(Int)

Int main(void)
{
  List(Int) ll = List_new(Int)();
  assert(List_size(Int)(&ll) == 0);
  List_push_back(Int)(&ll, 10);
  List_push_back(Int)(&ll, 20);
  List_push_back(Int)(&ll, 30);
  assert(List_size(Int)(&ll) == 3);

  Result(Int, cref(Char)) result = List_pop_back(Int)(&ll);
  assert(Result_is_ok(Int, cref(Char))(&result));
  assert(Result_unwrap(Int, cref(Char))(result) == 30);

  Result(Int, cref(Char)) second = List_pop_back(Int)(&ll);
  assert(Result_is_ok(Int, cref(Char))(&second));
  assert(Result_unwrap(Int, cref(Char))(second) == 20);

  Result(Int, cref(Char)) third = List_pop_back(Int)(&ll);
  assert(Result_is_ok(Int, cref(Char))(&third));
  assert(Result_unwrap(Int, cref(Char))(third) == 10);

  assert(List_size(Int)(&ll) == 0);

  List_push_back(Int)(&ll, 1);
  List_push_back(Int)(&ll, 2);
  List_push_back(Int)(&ll, 3);

  Int sum = 0;
  Size count = 0;
  for_each_ref(List(Int), item, &ll) {
    sum += ref_deref(Int)(item);
    ++count;
  }

  assert(count == 3);
  assert(sum == 6);

  return 0;
}