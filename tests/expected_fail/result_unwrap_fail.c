#include "my_c_utils/free.h"
#include "my_c_utils/result.h"

static inline void Int_free(Int *value)
{
  (void)value;
}

RESULT_CONFIG(Int)

Int main(void)
{
  Result_Void_Int_unwrap(Result_Void_Int_err("boom"));
  return 0;
}