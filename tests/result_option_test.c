#include "my_c_utils/option.h"
#include "my_c_utils/result.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>

typedef Char *String;

static String string_dup(const Char *text)
{
    Size length = strlen(text);
    String copy = malloc(length + 1);
    assert(copy != NULL);
    for (Size i = 0; i <= length; ++i)
    {
        copy[i] = text[i];
    }
    return copy;
}

void String_free(String *value) { free(*value); }

RESULT_CONFIG(String)
OPTION_CONFIG(String)

Int main(void)
{
    Result_String ok = Result_String_ok(string_dup("hello"));
    Result_String_free(&ok);

    Result_String err = Result_String_err("oops");
    Result_String_free(&err);

    Option_String some = Option_String_some(string_dup("world"));
    Option_String_free(&some);

    Option_String none = Option_String_none();
    Option_String_free(&none);

    return 0;
}