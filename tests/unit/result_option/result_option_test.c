#include "my_c_utils/primitives.h"
#include "my_c_utils/option.h"
#include "my_c_utils/result.h"
#include "my_c_utils/string_type.h"
#include <assert.h>

static inline String make_text(void)
{
  return string_dup("hello");
}

OPTION_CONFIG(String)

RESULT_CONFIG(Int, Int)

Int main(void)
{
  // Test new simplified C++/Rust two-parameter Result(String, cref(Char))
  Result(String, cref(Char)) ok = Result_ok(String, cref(Char))(make_text());
  assert(Result_is_ok(String, cref(Char))(&ok));
  String ok_val = Result_unwrap(String, cref(Char))(ok);
  assert(strcmp(ok_val, "hello") == 0);
  String_free(&ok_val);

  Result(String, cref(Char)) err = Result_err(String, cref(Char))("oops");
  assert(Result_is_err(String, cref(Char))(&err));
  assert(strcmp(Result_unwrap_err(String, cref(Char))(err), "oops") == 0);

  // Test new C++/Rust two-parameter Result(Int, Int)
  Result(Int, Int) res_ok = Result_ok(Int, Int)(42);
  assert(Result_is_ok(Int, Int)(&res_ok));
  assert(!Result_is_err(Int, Int)(&res_ok));
  assert(Result_unwrap(Int, Int)(res_ok) == 42);

  Result(Int, Int) res_err = Result_err(Int, Int)(99);
  assert(!Result_is_ok(Int, Int)(&res_err));
  assert(Result_is_err(Int, Int)(&res_err));
  assert(Result_unwrap_err(Int, Int)(res_err) == 99);

  // Test Option
  Option(String) some = Option_some(String)(make_text());
  assert(Option_is_some(String)(&some));
  assert(!Option_is_none(String)(&some));
  String some_val = Option_unwrap(String)(some);
  assert(strcmp(some_val, "hello") == 0);
  String_free(&some_val);

  Option(String) some2 = Option_some(String)(make_text());
  assert(Option_is_some(String)(&some2));
  Option_free(String)(&some2);

  Option(String) none = Option_none(String)();
  assert(Option_is_none(String)(&none));
  assert(!Option_is_some(String)(&none));
  Option_free(String)(&none);

  return 0;
}