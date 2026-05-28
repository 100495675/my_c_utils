# Macro Template System Guide

This guide explains the design, instantiation, and usage of the prefix-free C11 macro template system used in `my_c_utils`.

---

## 1. Core Concepts

In standard C, achieving generic programming is challenging. `my_c_utils` implements a type-safe preprocessor template engine where:
1. **Specialized Types** are declared under concantenated names (e.g., `Vector_Int`).
2. **Generic Macros** act as type constructors (e.g., `Vector(Int)`) that expand directly to their specialized type names.
3. **Specialized Methods** are mapped via method constructors (e.g., `Vector_push_back(Int)`) expanding to the specialized function symbol (e.g., `Vector_Int_push_back`).

---

## 2. Invocation Syntax

Because macros resolve to the function name itself, you invoke them by passing **type arguments** inside the macro parentheses first, followed immediately by the **function arguments** in regular parentheses.

```c
// Conceptually: Container_Method(TypeArguments)(FunctionArguments)

// Example: Push an integer into a vector
Vector_push_back(Int)(&my_vec, 42);

// Example: Retrieve a value from a Hashmap
Result(ref_Int, cref_Char) res = Hashmap_get(String, Int)(&my_map, "key");
```

This syntax is highly consistent across all generic containers and utilities in the library.

---

## 3. Template Instantiation (`TEMPLATE_CONFIG`)

Before using a container or helper with a custom or built-in type, you must specialize the template for that type. This is done once per type configuration (usually in a header or at the top of a `.c` file):

```c
// Instantiates Vector for Point struct
TEMPLATE_CONFIG(Vector, Point);

// Instantiates Hashmap with String keys and Int values
TEMPLATE_CONFIG(Hashmap, String, Int);
```

---

## 4. Comprehensive Container Cheat Sheet

Below is a cheat sheet showing the exact syntax for each container and helper macro.

### A. Box (`Box_##T`)
*Provides a heap-allocated owning wrapper for a single value.*

* **Type**: `Box(T)`
* **Create a Box**: `Box_new(T)(value)` (returns `Box(T)`)
* **Dereference (Immutable)**: `cref_deref(T)(Box_deref(T)(&my_box))` (returns `T` by copy/value)
* **Dereference (Mutable)**: `*Box_deref_mut(T)(&my_box) = new_value;` (returns `T*` for writing)
* **Extract value (Unwrap)**: `Box_into_inner(T)(my_box)` (destroys box, returns owned `T`)
* **Free box**: `Box_free(T)(my_box)`

### B. Vector (`Vector_##T`)
*A dynamic array that grows automatically as needed.*

* **Type**: `Vector(T)`
* **Create**: `Vector_new(T)()` (returns `Vector(T)`)
* **Size**: `Vector_size(T)(&my_vec)` (returns `Size`)
* **Push back**: `Vector_push_back(T)(&my_vec, value)` (returns `Result(Void, cref_Char)`)
* **Pop back**: `Vector_pop_back(T)(&my_vec)` (returns `Result(T, cref_Char)`)
* **Get element**: `Vector_at(T)(&my_vec, index)` (returns `Result(ref(T), cref_Char)`)
* **Set element**: `Vector_set(T)(&my_vec, index, value)` (returns `Result(Void, cref_Char)`)
* **Sort**: `Vector_sort(T)(&my_vec, compare_fn)`
* **Free vector**: `Vector_free(T)(&my_vec)`

### C. Linked List (`List_##T`)
*A generic singly linked list.*

* **Type**: `List(T)`
* **Create**: `List_new(T)()` (returns `List(T)`)
* **Size**: `List_size(T)(&my_list)` (returns `Size`)
* **Push front**: `List_push_front(T)(&my_list, value)` (returns `Result(Void, cref_Char)`)
* **Push back**: `List_push_back(T)(&my_list, value)` (returns `Result(Void, cref_Char)`)
* **Pop front**: `List_pop_front(T)(&my_list)` (returns `Result(T, cref_Char)`)
* **Free list**: `List_free(T)(&my_list)`

### D. Hashmap (`Hashmap_##K_##V`)
*An open-addressing hash map with Robin Hood collision resolution.*

* **Type**: `Hashmap(K, V)`
* **Create**: `Hashmap_new(K, V)(initial_capacity)` (returns `Hashmap(K, V)`)
* **Insert/Add**: `Hashmap_add(K, V)(&my_map, key, value)` (returns `Result(Void, cref_Char)`)
* **Retrieve (Borrowed)**: `Hashmap_get(K, V)(&my_map, key)` (returns `Result(ref(V), cref_Char)`)
* **Retrieve (Mutable)**: `Hashmap_get_mut(K, V)(&my_map, key)` (returns `Result(ref(V), cref_Char)`)
* **Remove/Delete**: `Hashmap_remove(K, V)(&my_map, key)`
* **Pop (Extract)**: `Hashmap_pop(K, V)(&my_map, key)` (returns `Result(V, cref_Char)`)
* **Free map**: `Hashmap_free(K, V)(&my_map)`

---

## 5. Result and Option Monad Macros

### A. Option (`Option_##T`)
*Safe wrapper for optional values that may or may not be present.*

* **Type**: `Option(T)`
* **Some**: `Option_some(T)(value)`
* **None**: `Option_none(T)()`
* **Is Some**: `Option_is_some(T)(&opt)` (returns `Bool`)
* **Is None**: `Option_is_none(T)(&opt)` (returns `Bool`)
* **Unwrap**: `Option_unwrap(T)(opt)` (returns `T` or aborts with panic if None)
* **Unwrap Or (Default)**: `Option_unwrap_or(T)(opt, default_value)` (returns `T`)
* **Map**: `Option_map(T, U)(opt, mapping_fn)` (returns `Option(U)`)
  * *Note: Requires `OPTION_MAP_CONFIG(T, U)` instantiation.*
* **And Then (Flat Map)**: `Option_and_then(T, U)(opt, monadic_fn)` (returns `Option(U)`)
  * *Note: Requires `OPTION_AND_THEN_CONFIG(T, U)` instantiation.*
* **Free**: `Option_free(T)(&opt)`

### B. Result (`Result_##T_##E`)
*Fallible operations returning success value `T` or error `E`.*

* **Type**: `Result(T, E)`
* **Ok (Success)**: `Result_ok(T, E)(value)`
* **Err (Failure)**: `Result_err(T, E)(error)`
* **Is Ok**: `Result_is_ok(T, E)(&res)` (returns `Bool`)
* **Is Err**: `Result_is_err(T, E)(&res)` (returns `Bool`)
* **Unwrap**: `Result_unwrap(T, E)(res)` (returns `T` or panics if Err)
* **Unwrap Error**: `Result_unwrap_err(T, E)(res)` (returns `E` or panics if Ok)
* **Unwrap Or (Default)**: `Result_unwrap_or(T, E)(res, default_value)` (returns `T`)
* **Map**: `Result_map(T, E, U)(res, mapping_fn)` (returns `Result(U, E)`)
  * *Note: Requires `RESULT_MAP_CONFIG(T, E, U)` instantiation.*
* **And Then (Flat Map)**: `Result_and_then(T, E, U)(res, monadic_fn)` (returns `Result(U, E)`)
  * *Note: Requires `RESULT_AND_THEN_CONFIG(T, E, U)` instantiation.*
* **Free**: `Result_free(T, E)(&res)`

---

## 6. Reference Wrapping (`ref` and `cref`)

*Provides trivial non-owning pointers designed to work seamlessly with container APIs.*

* **Mutable reference type**: `ref(T)` (expands to `T*`)
* **Immutable reference type**: `cref(T)` (expands to `const T*`)
* **Clone reference**: `ref_clone(T)(&my_ref)` / `cref_clone(T)(&my_cref)`
* **Dereference**: `ref_deref(T)(my_ref)` / `cref_deref(T)(my_cref)` (returns `T` by copy/value)

---

## 7. Interactive Example

Below is a complete, working example using several templates together:

```c
#include "my_c_utils/my_c_utils.h"
#include <assert.h>

int main() {
    // 1. Initialize a vector of Option(Int)
    // Instantiations are pre-registered for primitive types in umbrella headers
    Vector(Option(Int)) vec = Vector_new(Option(Int))();

    // 2. Push elements
    Vector_push_back(Option(Int))(&vec, Option_some(Int)(10));
    Vector_push_back(Option(Int))(&vec, Option_none(Int)());

    // 3. Inspect and unwrap
    Result(ref(Option(Int)), cref_Char) item_res = Vector_at(Option(Int))(&vec, 0);
    assert(Result_is_ok(ref(Option(Int)), cref_Char)(&item_res));
    
    // Dereference to get the Option(Int) copy
    Option(Int) opt = ref_deref(Option(Int))(Result_unwrap(ref(Option(Int)), cref_Char)(item_res));
    assert(Option_is_some(Int)(&opt));
    assert(Option_unwrap(Int)(opt) == 10);

    // 4. Cleanup
    Vector_free(Option(Int))(&vec);
    return 0;
}
```
