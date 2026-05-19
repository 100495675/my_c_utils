#include "my_c_utils/box.h"
#include "my_c_utils/struct_config.h"
#include "../support/string_helpers.h"
#include <assert.h>

STRUCT_CONFIG(Sub_dato,
              Int, numero,
              String, texto)

BOX_CONFIG(Sub_dato)

STRUCT_CONFIG(Mi_struct,
              Box_Sub_dato, dato,
              Int, otro_numero)
/*
typedef struct
{
  Box_Sub_dato dato;
  Int otro_numero;
} Mi_struct;
typedef Mi_struct *ref_Mi_struct;
typedef ref_Mi_struct *ref_ref_Mi_struct;
typedef ref_ref_Mi_struct *ref_ref_ref_Mi_struct;
typedef ref_ref_ref_Mi_struct *ref_ref_ref_ref_Mi_struct;
typedef ref_ref_ref_ref_Mi_struct *ref_ref_ref_ref_ref_Mi_struct;
static inline void ref_Mi_struct_free(ref_ref_Mi_struct value) { (void)value; }
static inline void ref_ref_Mi_struct_free(ref_ref_ref_Mi_struct value) { (void)value; }
static inline void ref_ref_ref_Mi_struct_free(ref_ref_ref_ref_Mi_struct value) { (void)value; }
static inline void ref_ref_ref_ref_Mi_struct_free(ref_ref_ref_ref_ref_Mi_struct value) { (void)value; }
static inline void Mi_struct_free(ref_Mi_struct self)
{
  if (!self)
    return;
  Box_Sub_dato_free(&self->dato);
  Int_free(&self->otro_numero);
}
typedef struct
{
  union
  {
    Mi_struct value;
    const ref_Char error_message;
  };
  Bool is_error;
} Result_Mi_struct;
typedef Result_Mi_struct *ref_Result_Mi_struct;
typedef ref_Result_Mi_struct *ref_ref_Result_Mi_struct;
typedef ref_ref_Result_Mi_struct *ref_ref_ref_Result_Mi_struct;
typedef ref_ref_ref_Result_Mi_struct *ref_ref_ref_ref_Result_Mi_struct;
typedef ref_ref_ref_ref_Result_Mi_struct *ref_ref_ref_ref_ref_Result_Mi_struct;
static inline void ref_Result_Mi_struct_free(ref_ref_Result_Mi_struct value) { (void)value; }
static inline void ref_ref_Result_Mi_struct_free(ref_ref_ref_Result_Mi_struct value) { (void)value; }
static inline void ref_ref_ref_Result_Mi_struct_free(ref_ref_ref_ref_Result_Mi_struct value) { (void)value; }
static inline void ref_ref_ref_ref_Result_Mi_struct_free(ref_ref_ref_ref_ref_Result_Mi_struct value) { (void)value; }
static inline Result_Mi_struct Result_Mi_struct_ok(Mi_struct value) { return (Result_Mi_struct){.value = value, .is_error = 0}; }
static inline Result_Mi_struct Result_Mi_struct_err(const ref_Char error_message)
{
  return (Result_Mi_struct){.error_message = error_message, .is_error = 1};
}
static inline Mi_struct Result_Mi_struct_unwrap(Result_Mi_struct result)
{
  if (result.is_error)
  {
    fputs("Unwrap on error: ", stderr);
    fputs(result.error_message, stderr);
    fputc('\n', stderr);
    exit(1);
  }
  return result.value;
}
static inline Bool Result_Mi_struct_is_err(Result_Mi_struct result) { return result.is_error; }
static inline Bool Result_Mi_struct_is_ok(Result_Mi_struct result) { return !result.is_error; }
static inline ref_Char Result_Mi_struct_unwrap_err(Result_Mi_struct result)
{
  if (!result.is_error)
  {
    perror("Result is ok");
    exit(1);
  }
  return result.error_message;
}
static inline void Result_Mi_struct_free(ref_Result_Mi_struct result)
{
  if (!result->is_error)
  {
    Mi_struct_free(&result->value);
  }
}
static inline Mi_struct Mi_struct_clone(const Mi_struct *src)
{
  if (!src)
  {
    perror("Cannot clone NULL pointer");
    exit(1);
  }
  Mi_struct dest = {0};
  dest.dato = Box_Sub_dato_clone(&src->dato);
  dest.otro_numero = Int_clone(&src->otro_numero);
  return dest;
}
*/
Int main()
{
  return 0;
}