#include "my_c_utils/free.h"
#include "my_c_utils/result.h"

static inline void Int_free(Int *value)
{
  (void)value;
}

RESULT_CONFIG(Int, cref(Char))

Int main(void)
{
  Result_unwrap(Int, cref(Char))(Result_err(Int, cref(Char))("boom"));
  return 0;
}