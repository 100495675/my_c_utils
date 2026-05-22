#include "my_c_utils/box.h"
#include "my_c_utils/vector.h"
#include <assert.h>

VECTOR_CONFIG(Int)
BOX_CONFIG(Int)
BOX_CONFIG(Vector_Int)

Int main(void)
{
  Box_Int number_box = Box_Int_new(7);
  assert(*Box_Int_deref(&number_box) == 7);

  *Box_Int_deref_mut(&number_box) = 9;
  assert(*Box_Int_deref(&number_box) == 9);

  Box_Int cloned_number = Box_Int_clone(&number_box);
  assert(*Box_Int_deref(&cloned_number) == 9);

  *Box_Int_deref_mut(&number_box) = 11;
  assert(*Box_Int_deref(&cloned_number) == 9);

  Box_Int_free(&number_box);
  Box_Int_free(&cloned_number);

  Vector_Int vector = Vector_Int_new();
  Result_Void r1 = Vector_Int_push_back(&vector, 1);
  assert(Result_Void_is_ok(&r1));
  Result_Void r2 = Vector_Int_push_back(&vector, 2);
  assert(Result_Void_is_ok(&r2));

  Box_Vector_Int vector_box = Box_Vector_Int_new(vector);
  vector = Vector_Int_new();
  assert(Vector_Int_size(Box_Vector_Int_deref(&vector_box)) == 2);

  Box_Vector_Int cloned_vector_box = Box_Vector_Int_clone(&vector_box);
  assert(Vector_Int_size(Box_Vector_Int_deref(&cloned_vector_box)) == 2);

  Result_Void_ref_Int original_first =
      Vector_Int_at(Box_Vector_Int_deref(&vector_box), 0);
  Result_Void_ref_Int cloned_first =
      Vector_Int_at(Box_Vector_Int_deref(&cloned_vector_box), 0);
  assert(Result_Void_ref_Int_is_ok(&original_first));
  assert(Result_Void_ref_Int_is_ok(&cloned_first));
  assert(*Result_Void_ref_Int_unwrap(original_first) == 1);
  assert(*Result_Void_ref_Int_unwrap(cloned_first) == 1);

  Result_Void r3 = Vector_Int_set(Box_Vector_Int_deref_mut(&vector_box), 0, 99);
  assert(Result_Void_is_ok(&r3));
  Result_Void r4 = Vector_Int_set(Box_Vector_Int_deref_mut(&vector_box), 0, 99);
  assert(Result_Void_is_ok(&r4));
  assert(*Result_Void_ref_Int_unwrap(Vector_Int_at(Box_Vector_Int_deref(&vector_box), 0)) == 99);
  assert(*Result_Void_ref_Int_unwrap(Vector_Int_at(Box_Vector_Int_deref(&cloned_vector_box), 0)) == 1);

  Box_Vector_Int_free(&vector_box);
  Box_Vector_Int_free(&cloned_vector_box);
  Vector_Int_free(&vector);
  return 0;
}