#include "my_c_utils/box.h"
#include "my_c_utils/struct_config.h"
#include "my_c_utils/vector.h"
#include <assert.h>

VECTOR_CONFIG(Char)

STRUCT_CONFIG(Sub_dato,
              Int, numero,
              Vector(Char), texto)


BOX_CONFIG(Sub_dato)

STRUCT_CONFIG(Mi_struct,
              Box(Sub_dato), dato,
              Int, otro_numero)


VECTOR_CONFIG(Mi_struct)

Int main()
{
  Vector(Char) texto = Vector_new(Char)();
  for (const Char *c = "Hola"; *c; ++c)
  {
    Vector_push_back(Char)(&texto, *c);
  }
  Mi_struct mi_struct = Mi_struct_new(Box_new(Sub_dato)(Sub_dato_new(42, texto)), 7);
  Vector(Mi_struct) vector = Vector_new(Mi_struct)();

  Result(Void, cref_Char) r1 = Vector_push_back(Mi_struct)(&vector, mi_struct);
  assert(Result_is_ok(Void, cref_Char)(&r1));
  Result_free(Void, cref_Char)(&r1);

  Result(ref_Mi_struct, cref_Char) result = Vector_at(Mi_struct)(&vector, 0);
  assert(Result_is_ok(ref_Mi_struct, cref_Char)(&result));
  Mi_struct *retrieved = Result_unwrap(ref_Mi_struct, cref_Char)(result);
  Result_free(ref_Mi_struct, cref_Char)(&result);

  assert(retrieved->otro_numero == 7);
  assert(Box_deref(Sub_dato)(&retrieved->dato)->numero == 42);

  Vector_free(Mi_struct)(&vector);

  return 0;
}