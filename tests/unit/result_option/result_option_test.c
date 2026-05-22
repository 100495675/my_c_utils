#include "my_c_utils/option.h"
#include "my_c_utils/result.h"
#include "my_c_utils/string_type.h"
#include <assert.h>

static inline String make_text(void)
{
  return string_dup("hello");
}

OPTION_CONFIG(String)

Int main(void)
{
  Result_Void_String ok = Result_Void_String_ok(make_text());
  Result_Void_String_free(&ok);

  Result_Void_String err = Result_Void_String_err("oops");
  Result_Void_String_free(&err);

  Option_String some = Option_String_some(make_text());
  Option_String_free(&some);

  Option_String none = Option_String_none();
  Option_String_free(&none);

  return 0;
}