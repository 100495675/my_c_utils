#include "my_c_utils/core.h"
#include "my_c_utils/vector.h"
#include <assert.h>
#include <stdio.h>

// 0. Declare dependencies for Stack
VECTOR_CONFIG(Int)

// 1. Generic Struct Template: Stack(T)
// Wraps a Vector(T) inside a struct defined by STRUCT_CONFIG
#define Stack(...) TEMPLATE_TYPE(Stack, __VA_ARGS__)
#define ref_Stack(...) TEMPLATE_TYPE(ref_Stack, __VA_ARGS__)
#define cref_Stack(...) TEMPLATE_TYPE(cref_Stack, __VA_ARGS__)

#define Stack_new(...) TEMPLATE_METHOD(Stack, new, __VA_ARGS__)
#define Stack_push(...) TEMPLATE_METHOD(Stack, push, __VA_ARGS__)
#define Stack_pop(...) TEMPLATE_METHOD(Stack, pop, __VA_ARGS__)
#define Stack_free(...) TEMPLATE_METHOD(Stack, free, __VA_ARGS__)

#define TEMPLATE_Stack(T) \
    STRUCT_CONFIG(Stack(T), \
                  Vector(T), items) \
    static inline Result(Void, cref_Char) Stack_push(T)(ref_Stack(T) self, T value) { \
        return Vector_push_back(T)(&self->items, value); \
    } \
    static inline Result(T, cref_Char) Stack_pop(T)(ref_Stack(T) self) { \
        return Vector_pop_back(T)(&self->items); \
    }

// 2. Instantiate template configurations
TEMPLATE_CONFIG(Stack, Int);

int main(void) {
    printf("=== Template Engine Unit Tests ===\n\n");

    // Test 1: Stack<Int> wrapping Vector<Int>
    Stack(Int) stack = Stack_new(Int)(Vector_new(Int)());
    
    Result(Void, cref_Char) r_push1 = Stack_push(Int)(&stack, 100);
    assert(Result_is_ok(Void, cref_Char)(&r_push1));
    Result(Void, cref_Char) r_push2 = Stack_push(Int)(&stack, 200);
    assert(Result_is_ok(Void, cref_Char)(&r_push2));
    printf("TEST: Stack_push(Int) pushed 100 and 200 ... ✓\n");

    Result(Int, cref_Char) r_pop1 = Stack_pop(Int)(&stack);
    assert(Result_is_ok(Int, cref_Char)(&r_pop1));
    assert(Result_unwrap(Int, cref_Char)(r_pop1) == 200);

    Result(Int, cref_Char) r_pop2 = Stack_pop(Int)(&stack);
    assert(Result_is_ok(Int, cref_Char)(&r_pop2));
    assert(Result_unwrap(Int, cref_Char)(r_pop2) == 100);
    printf("TEST: Stack_pop(Int) popped 200 then 100 (LIFO order) ... ✓\n");

    // Free resources cleanly
    Stack_free(Int)(&stack);
    printf("TEST: Custom template cleanup via free methods ... ✓\n");

    printf("\n=== All template tests passed! ===\n");
    return 0;
}
