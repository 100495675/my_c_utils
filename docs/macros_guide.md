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
Result(ref_Int, cref(Char)) res = Hashmap_get(String, Int)(&my_map, "key");
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
* **Push back**: `Vector_push_back(T)(&my_vec, value)` (returns `Result(Void, cref(Char))`)
* **Pop back**: `Vector_pop_back(T)(&my_vec)` (returns `Result(T, cref(Char))`)
* **Get element**: `Vector_at(T)(&my_vec, index)` (returns `Result(ref(T), cref(Char))`)
* **Set element**: `Vector_set(T)(&my_vec, index, value)` (returns `Result(Void, cref(Char))`)
* **Sort**: `Vector_sort(T)(&my_vec, compare_fn)`
* **Free vector**: `Vector_free(T)(&my_vec)`

### C. Linked List (`List_##T`)
*A generic singly linked list.*

* **Type**: `List(T)`
* **Create**: `List_new(T)()` (returns `List(T)`)
* **Size**: `List_size(T)(&my_list)` (returns `Size`)
* **Push front**: `List_push_front(T)(&my_list, value)` (returns `Result(Void, cref(Char))`)
* **Push back**: `List_push_back(T)(&my_list, value)` (returns `Result(Void, cref(Char))`)
* **Pop front**: `List_pop_front(T)(&my_list)` (returns `Result(T, cref(Char))`)
* **Free list**: `List_free(T)(&my_list)`

### D. Hashmap (`Hashmap_##K_##V`)
*An open-addressing hash map with Robin Hood collision resolution.*

* **Type**: `Hashmap(K, V)`
* **Create**: `Hashmap_new(K, V)(initial_capacity)` (returns `Hashmap(K, V)`)
* **Insert/Add**: `Hashmap_add(K, V)(&my_map, key, value)` (returns `Result(Void, cref(Char))`)
* **Retrieve (Borrowed)**: `Hashmap_get(K, V)(&my_map, key)` (returns `Result(ref(V), cref(Char))`)
* **Retrieve (Mutable)**: `Hashmap_get_mut(K, V)(&my_map, key)` (returns `Result(ref(V), cref(Char))`)
* **Remove/Delete**: `Hashmap_remove(K, V)(&my_map, key)`
* **Pop (Extract)**: `Hashmap_pop(K, V)(&my_map, key)` (returns `Result(V, cref(Char))`)
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

## 7. Iterator Collect System

*Provides modern, container-agnostic mechanisms to collect elements from any standard iterator into any linear container (e.g., Vector, List).*

### A. In-Place Collection: `collect`
*Populates an existing destination container with elements drawn from an active iterator.*

* **Syntax**: `collect(DestContainerT, SrcContainerT)(dest_ptr, src_iter)`
* **Example**:
  ```c
  List(Int) my_list = ...;
  Vector(Int) my_vec = Vector_new(Int)();
  
  iter_List(Int) it = List_into_iter(Int)(&my_list);
  collect(Vector(Int), List(Int))(&my_vec, it);
  ```

### B. Inline Expression Collection: `collect_new`
*Constructs, populates, and returns a new container inline from an active iterator.*

* **Syntax**: `collect_new(DestContainerT, SrcContainerT)(src_iter)`
* **Example**:
  ```c
  List(Int) my_list = ...;
  
  // We can pass the iterator directly by value, even inline!
  Vector(Int) my_vec = collect_new(Vector(Int), List(Int))(List_into_iter(Int)(&my_list));
  ```

---

## 8. Iterator Loop Macros

> [!IMPORTANT]
> **Notation Exception**: These loop macros use a single-parenthesis calling syntax (e.g., `for_each(Vector(Int), item, &vec)`). This is a necessary and intentional exception to the project's standard double-parentheses generic notation (like `collect(Vector(Int), List(Int))(...)`) due to standard C preprocessor syntax limitations for control structures (since C cannot define macros inside other macros or cleanly parse unclosed parentheses for loops without compilation errors).

*Provides highly ergonomic, type-safe loop control structures that automatically generate iterators from containers under a single-parenthesis calling syntax.*

### A. Copy Loop: `for_each`
*Loops over a container, automatically creating an iterator and exposing each element by copy (value).*

* **Syntax**: `for_each(ContainerT, var_name, iterable) { ... }`
* **Example**:
  ```c
  Vector(Int) my_vec = ...;
  
  Int sum = 0;
  for_each(Vector(Int), item, &my_vec) {
      sum += item;
  }
  ```

### B. Reference Loop: `for_each_ref`
*Loops over a container, automatically creating an iterator and exposing each element as a borrowed reference pointer (`ref(T)`).*

* **Syntax**: `for_each_ref(ContainerT, var_name, iterable) { ... }`
* **Example**:
  ```c
  Vector(Int) my_vec = ...;
  
  Int sum_ref = 0;
  for_each_ref(Vector(Int), item_ref, &my_vec) {
      sum_ref += ref_deref(Int)(item_ref);
  }
  ```

---

## 9. Functional Iterator Operators

Modern C programming supports programming in a functional style using type-safe high-order iterator macros. These macros support the project's standard double-parenthesis notation and are fully compatible with static functions.

To facilitate inline anonymous function declaration without verbose nested function syntax or name collisions, the library provides a utility **`lambda`** macro.

### The `lambda` Utility Macro
*Declares a type-safe inline anonymous function (lambda) with an explicit return, protected from variable collisions using `__COUNTER__`.*

* **Syntax**: `lambda(return_type, (parameter_list), { body_with_explicit_return })`
* **Features**:
  * **Explicit Returns**: The user writes `return` explicitly inside the body block.
  * **Any Number of Parameters**: Supports 0, 1, 2, or more parameters (e.g. `(Int x)` or `(Int acc, Int x)`).
  * **Collision-Free**: Generates unique names at compile time using `__COUNTER__`.

---

### A. Functional Loop: `for_each_fn`
*Iterates over a container and executes a callback function or inline lambda for each element.*

* **Syntax**: `for_each_fn(ContainerT)(container_ptr, callback)`
* **Example (with `lambda`)**:
  ```c
  Vector(Int) my_vec = ...;
  for_each_fn(Vector(Int))(&my_vec, lambda(void, (const Int* x), {
      printf("Value: %d\n", *x);
  }));
  ```

### B. Element Mapping: `map`
*Maps elements of a source iterator lazily using a mapping function or lambda, returning a lazy `Mapped` iterator.*

* **Syntax**: `map(SrcContainer, DestT)(src_iter, mapping_func)`
* **Example (with lazy evaluation & collection)**:
  ```c
  Vector(Int) my_vec = ...; // [10, 20, 30, 40]
  
  // Create a lazy mapped iterator doubling each element
  iter_Mapped(Vector(Int), Int) doubled_iter = map(Vector(Int), Int)(
      Vector_into_iter(Int)(my_vec),
      lambda(Int, (cref(Int) x), { return cref_deref(Int)(x) * 2; })
  );
  
  // Eagerly collect mapped elements into a new vector
  Vector(Int) doubled = collect_new(Vector(Int), Mapped(Vector(Int), Int))(doubled_iter);
  ```

### C. Element Filtering: `filter` and `filter_new`
*Filters elements of an iterator lazily using a predicate function or lambda, returning a lazy `Filtered` iterator.*
* `filter` returns a lazy `Filtered` iterator wrapper.
* `filter_new` is an eager helper that constructs a new destination container, lazily filters, and populates it inline using `collect_new` under the hood.

* **Syntax**:
  * `filter(SrcContainerT)(src_iter, predicate)`
  * `filter_new(DestContainerT, SrcContainerT)(src_iter, predicate)`
* **Example (with `lambda`)**:
  ```c
  Vector(Int) my_vec = ...; // [1, 2, 3, 4]
  
  // Filter even elements lazily
  iter_Filtered(Vector(Int)) evens_iter = filter(Vector(Int))(
      Vector_into_iter(Int)(Vector_clone(Int)(&my_vec)),
      lambda(bool, (cref(Int) x), { return cref_deref(Int)(x) % 2 == 0; })
  );
  
  // Eagerly collect filtered elements into an existing vector
  Vector(Int) filtered = Vector_new(Int)();
  collect(Vector(Int), Filtered(Vector(Int)))(&filtered, evens_iter);
  
  // Alternatively, filter and collect inline using the eager filter_new helper
  Vector(Int) evens = filter_new(Vector(Int), Vector(Int))(
      Vector_into_iter(Int)(my_vec),
      lambda(bool, (cref(Int) x), { return cref_deref(Int)(x) % 2 == 0; })
  );
  ```

### D. Accumulator / Reduction: `fold`
*Folds/reduces the elements of an iterator into a single accumulated value.*

* **Syntax**: `fold(SrcContainerT, AccT)(acc_init, src_iter, fold_func)`
* **Example (with `lambda`)**:
  ```c
  Vector(Int) my_vec = ...;
  
  // Sum elements with a base value of 100
  Int sum = fold(Vector(Int), Int)(
      100, 
      Vector_into_iter(Int)(&my_vec),
      lambda(Int, (Int acc, const Int* x), { return acc + *x; })
  );
  ```

### E. Predicate Checks: `any` and `all`
*Evaluates existential and universal predicates over the elements of an iterator.*
* `any` returns true if at least one element matches the predicate.
* `all` returns true if all elements match the predicate.

* **Syntax**:
  * `any(SrcContainerT)(src_iter, predicate)`
  * `all(SrcContainerT)(src_iter, predicate)`
* **Example (with `lambda`)**:
  ```c
  Vector(Int) my_vec = ...;
  
  bool has_negative = any(Vector(Int))(
      Vector_into_iter(Int)(&my_vec),
      lambda(bool, (const Int* x), { return *x < 0; })
  );
  ```

---

## 10. Declarative Pipeline Composition (The Pipe Operator)

*Provides a thread-safe, type-safe declarative pipeline composition system using GCC statement expression nested shadowing scopes. This allows fluent chaining of lazy iterators, container transformations, and inline lambdas in standard C without verbose nested function calls or type-changing variables.*

To overcome standard C limitations where a variable's type cannot be changed within the same scope, `pipeline` nestedly declares and shadows a special local identifier `_val` at each stage. Inside each inner statement expression scope, `_val` is shadowed sequentially with a new deduced type, allowing the output of each pipeline step to serve as the input for the next step.

### A. The `pipeline` Macro
*Combines an input container/iterator and any number of pipeline steps (up to 8) into a single unified statement expression.*

* **Syntax**: `pipeline(input, step1, step2, ...)`
* **The `_val` Placeholder**: Within each step expression, use the keyword `_val` to represent the output from the immediate preceding step.
* **Example**:
  ```c
  Vector(Int) pipeline_res = pipeline(
      Vector_into_iter(Int)(Vector_clone(Int)(&vec)),
      skip(Vector(Int))(_val, 1),
      filter(Skip(Vector(Int)))(_val, lambda(bool, (cref(Int) x), { return cref_deref(Int)(x) > 20; })),
      map(Filtered(Skip(Vector(Int))), Int)(_val, lambda(Int, (cref(Int) x), { return cref_deref(Int)(x) * 2; })),
      collect_new(Vector(Int), Mapped(Filtered(Skip(Vector(Int))), Int))(_val)
  );
  ```

### B. Features & Compile-Time Safety
* **Zero Runtime Overhead**: Designed completely via standard C preprocessor structures; compiles directly down to standard nested blocks, optimized by the compiler without any performance or allocation overhead.
* **100% Memory Safe**: Fully integrates with our memory-consuming ownership model. Intermediate iterators are automatically consumed and cleaned up at the end of the pipeline when the final `collect` or `collect_new` step calls `iter_free` on the pipeline.
* **Lambda Compatible**: Fully compatible with inline anonymous functions (`lambda`) at any filtering or mapping stage.

---

## 11. Interactive Example

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
    Result(ref(Option(Int)), cref(Char)) item_res = Vector_at(Option(Int))(&vec, 0);
    assert(Result_is_ok(ref(Option(Int)), cref(Char))(&item_res));
    
    // Dereference to get the Option(Int) copy
    Option(Int) opt = ref_deref(Option(Int))(Result_unwrap(ref(Option(Int)), cref(Char))(item_res));
    assert(Option_is_some(Int)(&opt));
    assert(Option_unwrap(Int)(opt) == 10);

    // 4. Cleanup
    Vector_free(Option(Int))(&vec);
    return 0;
}
```
