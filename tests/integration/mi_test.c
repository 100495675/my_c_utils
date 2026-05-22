#include "my_c_utils/box.h"
#include "my_c_utils/struct_config.h"
#include "my_c_utils/vector.h"
#include <assert.h>

VECTOR_CONFIG(Char)

STRUCT_CONFIG(Sub_dato,
              Int, numero,
              Vector_Char, texto)

Sub_dato Sub_dato_new(Int numero, Vector_Char texto)
{
  return (Sub_dato){.numero = numero, .texto = texto};
}

BOX_CONFIG(Sub_dato)

STRUCT_CONFIG(Mi_struct,
              Box_Sub_dato, dato,
              Int, otro_numero)

Mi_struct Mi_struct_new(Box_Sub_dato dato, Int otro_numero)
{
  return (Mi_struct){.dato = dato, .otro_numero = otro_numero};
}

VECTOR_CONFIG(Mi_struct)

Int main()
{
  Vector_Char texto = Vector_Char_new();
  for (const Char *c = "Hola"; *c; ++c)
  {
    Vector_Char_push_back(&texto, *c);
  }
  Mi_struct mi_struct = Mi_struct_new(Box_Sub_dato_new(Sub_dato_new(42, texto)), 7);
  Vector_Mi_struct vector = Vector_Mi_struct_new();

  Result_Void r1 = Vector_Mi_struct_push_back(&vector, mi_struct);
  assert(Result_Void_is_ok(&r1));
  Result_Void_free(&r1);

  Result_Void_ref_Mi_struct result = Vector_Mi_struct_at(&vector, 0);
  assert(Result_Void_ref_Mi_struct_is_ok(&result));
  Mi_struct *retrieved = Result_Void_ref_Mi_struct_unwrap(result);
  Result_Void_ref_Mi_struct_free(&result);

  assert(retrieved->otro_numero == 7);
  assert(Box_Sub_dato_deref(&retrieved->dato)->numero == 42);

  Vector_Mi_struct_free(&vector);

  return 0;
}