#include "my_c_utils/option.h"
#include "my_c_utils/result.h"
#include <assert.h>
#include <stdio.h>
#include <math.h>
#include <string.h>

static inline void Int_free(Int *val)
{
    (void)val;
}

static inline void Double_free(Double *val)
{
    (void)val;
}

// 1. Instantiate Option and Result Templates first
OPTION_CONFIG(Int)
OPTION_CONFIG(Double)
OPTION_MAP_CONFIG(Int, Double)
OPTION_AND_THEN_CONFIG(Int, Double)

RESULT_CONFIG(Int, cref_Char)
RESULT_CONFIG(Double, cref_Char)
RESULT_MAP_CONFIG(Int, cref_Char, Double)
RESULT_AND_THEN_CONFIG(Int, cref_Char, Double)

// 2. Helper mapping functions for Option
static inline Double int_to_double(Int x)
{
    return (Double)x * 1.5;
}

static inline Option(Double) int_to_opt_double(Int x)
{
    if (x == 0)
    {
        return Option_none(Double)();
    }
    return Option_some(Double)((Double)x * 1.5);
}

// 3. Helper mapping functions for Result
static inline Double res_int_to_double(Int x)
{
    return (Double)x * 2.5;
}

static inline Result(Double, cref_Char) res_int_to_res_double(Int x)
{
    if (x == 0)
    {
        return Result_err(Double, cref_Char)("Cannot map zero");
    }
    return Result_ok(Double, cref_Char)((Double)x * 2.5);
}

// 4. Test Functions
void test_option_unwrap_or(void)
{
    printf("TEST: Option_unwrap_or... ");
    Option(Int) some = Option_some(Int)(42);
    Option(Int) none = Option_none(Int)();

    assert(Option_unwrap_or(Int)(some, 100) == 42);
    assert(Option_unwrap_or(Int)(none, 100) == 100);

    Option_free(Int)(&some);
    Option_free(Int)(&none);
    printf("✓\n");
}

void test_option_map(void)
{
    printf("TEST: Option_map... ");
    Option(Int) some = Option_some(Int)(10);
    Option(Int) none = Option_none(Int)();

    // Map Some: 10 * 1.5 = 15.0
    Option(Double) mapped_some = Option_map(Int, Double)(some, int_to_double);
    assert(Option_is_some(Double)(&mapped_some));
    assert(fabs(Option_unwrap(Double)(mapped_some) - 15.0) < 1e-9);

    // Map None
    Option(Double) mapped_none = Option_map(Int, Double)(none, int_to_double);
    assert(Option_is_none(Double)(&mapped_none));

    Option_free(Int)(&some);
    Option_free(Int)(&none);
    Option_free(Double)(&mapped_some);
    Option_free(Double)(&mapped_none);
    printf("✓\n");
}

void test_option_and_then(void)
{
    printf("TEST: Option_and_then... ");
    Option(Int) some_non_zero = Option_some(Int)(10);
    Option(Int) some_zero = Option_some(Int)(0);
    Option(Int) none = Option_none(Int)();

    // non-zero mapping: 10 * 1.5 = 15.0
    Option(Double) mapped_ok = Option_and_then(Int, Double)(some_non_zero, int_to_opt_double);
    assert(Option_is_some(Double)(&mapped_ok));
    assert(fabs(Option_unwrap(Double)(mapped_ok) - 15.0) < 1e-9);

    // zero mapping (returns None)
    Option(Double) mapped_zero = Option_and_then(Int, Double)(some_zero, int_to_opt_double);
    assert(Option_is_none(Double)(&mapped_zero));

    // none mapping
    Option(Double) mapped_none = Option_and_then(Int, Double)(none, int_to_opt_double);
    assert(Option_is_none(Double)(&mapped_none));

    Option_free(Int)(&some_non_zero);
    Option_free(Int)(&some_zero);
    Option_free(Int)(&none);
    Option_free(Double)(&mapped_ok);
    Option_free(Double)(&mapped_zero);
    Option_free(Double)(&mapped_none);
    printf("✓\n");
}

void test_result_unwrap_or(void)
{
    printf("TEST: Result_unwrap_or... ");
    Result(Int, cref_Char) ok = Result_ok(Int, cref_Char)(42);
    Result(Int, cref_Char) err = Result_err(Int, cref_Char)("oops");

    assert(Result_unwrap_or(Int, cref_Char)(ok, 100) == 42);
    assert(Result_unwrap_or(Int, cref_Char)(err, 100) == 100);

    Result_free(Int, cref_Char)(&ok);
    Result_free(Int, cref_Char)(&err);
    printf("✓\n");
}

void test_result_map(void)
{
    printf("TEST: Result_map... ");
    Result(Int, cref_Char) ok = Result_ok(Int, cref_Char)(10);
    Result(Int, cref_Char) err = Result_err(Int, cref_Char)("oops");

    // Map Ok: 10 * 2.5 = 25.0
    Result(Double, cref_Char) mapped_ok = Result_map(Int, cref_Char, Double)(ok, res_int_to_double);
    assert(Result_is_ok(Double, cref_Char)(&mapped_ok));
    assert(fabs(Result_unwrap(Double, cref_Char)(mapped_ok) - 25.0) < 1e-9);

    // Map Err
    Result(Double, cref_Char) mapped_err = Result_map(Int, cref_Char, Double)(err, res_int_to_double);
    assert(Result_is_err(Double, cref_Char)(&mapped_err));
    assert(strcmp(Result_unwrap_err(Double, cref_Char)(mapped_err), "oops") == 0);

    Result_free(Int, cref_Char)(&ok);
    Result_free(Int, cref_Char)(&err);
    Result_free(Double, cref_Char)(&mapped_ok);
    Result_free(Double, cref_Char)(&mapped_err);
    printf("✓\n");
}

void test_result_and_then(void)
{
    printf("TEST: Result_and_then... ");
    Result(Int, cref_Char) ok_non_zero = Result_ok(Int, cref_Char)(10);
    Result(Int, cref_Char) ok_zero = Result_ok(Int, cref_Char)(0);
    Result(Int, cref_Char) err = Result_err(Int, cref_Char)("oops");

    // non-zero map: 10 * 2.5 = 25.0
    Result(Double, cref_Char) mapped_ok = Result_and_then(Int, cref_Char, Double)(ok_non_zero, res_int_to_res_double);
    assert(Result_is_ok(Double, cref_Char)(&mapped_ok));
    assert(fabs(Result_unwrap(Double, cref_Char)(mapped_ok) - 25.0) < 1e-9);

    // zero map (returns Err)
    Result(Double, cref_Char) mapped_zero = Result_and_then(Int, cref_Char, Double)(ok_zero, res_int_to_res_double);
    assert(Result_is_err(Double, cref_Char)(&mapped_zero));
    assert(strcmp(Result_unwrap_err(Double, cref_Char)(mapped_zero), "Cannot map zero") == 0);

    // err map
    Result(Double, cref_Char) mapped_err = Result_and_then(Int, cref_Char, Double)(err, res_int_to_res_double);
    assert(Result_is_err(Double, cref_Char)(&mapped_err));
    assert(strcmp(Result_unwrap_err(Double, cref_Char)(mapped_err), "oops") == 0);

    Result_free(Int, cref_Char)(&ok_non_zero);
    Result_free(Int, cref_Char)(&ok_zero);
    Result_free(Int, cref_Char)(&err);
    Result_free(Double, cref_Char)(&mapped_ok);
    Result_free(Double, cref_Char)(&mapped_zero);
    Result_free(Double, cref_Char)(&mapped_err);
    printf("✓\n");
}

int main(void)
{
    printf("=== Monadic Combinator Tests ===\n\n");

    test_option_unwrap_or();
    test_option_map();
    test_option_and_then();

    test_result_unwrap_or();
    test_result_map();
    test_result_and_then();

    printf("\n=== All monadic combinator tests passed! ===\n");
    return 0;
}
