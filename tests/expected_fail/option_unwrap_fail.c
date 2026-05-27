#include "my_c_utils/free.h"
#include "my_c_utils/option.h"

static inline void Int_free(Int *value)
{
  (void)value;
}

OPTION_CONFIG(Int)

Int main(void)
{
  Option_unwrap(Int)(Option_none(Int)());
  return 0;
}