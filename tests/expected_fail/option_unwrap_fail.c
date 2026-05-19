#include "my_c_utils/free.h"
#include "my_c_utils/option.h"

static inline void Int_free(Int *value)
{
  (void)value;
}

OPTION_CONFIG(Int)

Int main(void)
{
  Option_Int_unwrap(Option_Int_none());
  return 0;
}