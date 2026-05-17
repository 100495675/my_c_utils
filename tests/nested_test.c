#include "my_c_utils/vector.h"
#include "my_c_utils/free.h"
#include <assert.h>

VECTOR_CONFIG(Int)
VECTOR_CONFIG(Vector_Int)

Int main(void) {
  Vector_Vector_Int outer = Vector_Vector_Int_new();

  Vector_Int first = Vector_Int_new();
  assert(Result_is_ok(Vector_Int_push_back(&first, 1)));
  assert(Result_is_ok(Vector_Int_push_back(&first, 2)));
  assert(Result_is_ok(Vector_Vector_Int_push_back(&outer, first)));
  first = Vector_Int_new();

  Vector_Int second = Vector_Int_new();
  assert(Result_is_ok(Vector_Int_push_back(&second, 3)));
  assert(Result_is_ok(Vector_Vector_Int_push_back(&outer, second)));
  second = Vector_Int_new();

  assert(Vector_Vector_Int_size(&outer) == 2);

  Vector_Vector_Int_free(&outer);
  return 0;
}
