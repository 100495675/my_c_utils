#include "my_c_utils/linked_list.h"
#include "my_c_utils/free.h"
#include <assert.h>

LINKED_LIST_CONFIG(Int)

Int main()
{
  List_Int ll = List_Int_new();
  assert(List_Int_size(&ll) == 0);
  List_Int_push_back(&ll, 10);
  List_Int_push_back(&ll, 20);
  List_Int_push_back(&ll, 30);
  assert(List_Int_size(&ll) == 3);

  Result_Int result = List_Int_pop_back(&ll);
  if (Result_Int_is_ok(result))
  {
    assert(Result_Int_unwrap(result) == 30);
  }

  assert(List_Int_size(&ll) == 2);

  Result_Int second = List_Int_pop_back(&ll);
  assert(Result_Int_is_ok(second));
  assert(Result_Int_unwrap(second) == 20);

  Result_Int third = List_Int_pop_back(&ll);
  assert(Result_Int_is_ok(third));
  assert(Result_Int_unwrap(third) == 10);

  assert(List_Int_size(&ll) == 0);

  List_Int_push_back(&ll, 1);
  List_Int_push_back(&ll, 2);
  List_Int_push_back(&ll, 3);

  Int sum = 0;
  Size count = 0;
  for_each_ref(Int, item, List, &ll, {
    sum += *item;
    ++count;
  });

  assert(count == 3);
  assert(sum == 6);

  List_Int_free(&ll);
  return 0;
}
