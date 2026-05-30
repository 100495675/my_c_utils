#include "my_c_utils/primitives.h"
#include "my_c_utils/result.h"

Int main(void)
{
  Result_unwrap(Int, cref(Char))(Result_err(Int, cref(Char))("boom"));
  return 0;
}