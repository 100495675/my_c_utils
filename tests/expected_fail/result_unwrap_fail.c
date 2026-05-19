#include "my_c_utils/free.h"
#include "my_c_utils/result.h"

static inline void Int_free(Int *value)
{
  (void)value;
}

RESULT_CONFIG(Int)

Int main(void)
{
  Result_Int_unwrap(Result_Int_err("boom"));
  return 0;
}