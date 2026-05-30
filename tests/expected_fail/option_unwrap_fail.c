#include "my_c_utils/primitives.h"
#include "my_c_utils/option.h"

OPTION_CONFIG(Int)

Int main(void)
{
  Option_unwrap(Int)(Option_none(Int)());
  return 0;
}