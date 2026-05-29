# Future Architectural Proposals & Features for `my_c_utils`

This document details five highly advanced, standard-compliant design directions that represent the logical evolution of the `my_c_utils` library, building directly upon our monadic Option/Result combinators, value-consuming ownership, and lazy iterator pipelines.

---

## 1. Advanced Lazy Iterator Adapters (`take`, `skip`, `enumerate`, `zip`)

### Background
With `map` and `filter` now fully refactored as lazy iterator adapters (using `iter_Mapped` and `iter_Filtered` structs), the iterator pipeline is extremely powerful. We can expand this laziness with new intermediate adapters commonly found in functional pipelines (Rust, Python, Haskell).

### Proposed Designs

* **`take(N)` / `skip(N)`**: Lazy adapters that restrict or skip iteration steps.
  - `take` stops emitting values after `N` elements, calling `iter_free` on the inner iterator early.
  - `skip` immediately advances the inner iterator `N` times in its constructor, then continues lazily.
* **`enumerate`**: Lazily pairs each element with its 0-indexed count, returning a `Pair(Size, T)`.
* **`zip`**: Lazily takes elements from two separate iterators concurrently and pairs them into a `Pair(T, U)`.

```c
// Example: Lazy take
iter_Take(Vector(Int)) taken = take(Vector(Int))(Vector_into_iter(Int)(vec), 5);
Vector(Int) first_five = collect_new(Vector(Int), Take(Vector(Int)))(taken);
```

### Advantages
* **Finite Streams**: Allows processing infinite or massive streams by binding them lazily to a specific size.
* **Complex Transformations**: Eliminates manual index counting or dual-loop boilerplate.

---

## 2. Declarative Pipeline Composition (The Pipe Operator `|`)

### Background
Currently, chaining lazy adapters requires deeply nested preprocessor declarations:
```c
collect_new(Vector(Int), Mapped(Filtered(Vector(Int)), Int))(
    map(Filtered(Vector(Int)), Int)(
        filter(Vector(Int))(Vector_into_iter(Int)(vec), is_even),
        double_val
    )
);
```
While extremely fast (zero-cost abstraction at compile time), the nested parentheses make readability difficult.

### Proposed Design
Introduce a **Pipeline Composition Macro** (`pipe!`) or a pseudo-fluent API using C preprocessor concatenation, enabling a clean, sequential flow of transformations:

```c
// Declarative composition pipeline
Vector(Int) result = pipeline(
    Vector_into_iter(Int)(vec),
    filter(Vector(Int)), is_even,
    map(Filtered(Vector(Int)), Int), double_val,
    collect_new(Vector(Int), Mapped(Filtered(Vector(Int)), Int))
);
```
Alternatively, leveraging statement expressions:
```c
Vector(Int) result = it_pipe(
    Vector_into_iter(Int)(vec),
    it_filter(is_even),
    it_map(double_val),
    it_collect(Vector(Int))
);
```

### Advantages
* **High Readability**: Sequentially maps logic from top to bottom or left to right, rather than inside-out.
* **Zero Overhead**: Expands directly to our existing nested macro call stack at compile time.

---

## 3. Explicit Move Semantics & Zero-Copy Ownership Transfer

### Background
Currently, when pushing a struct into a vector or list (`Vector_push_back`), the data is copied by value (`*dest = value`). If the element itself is a heap-owning container (like `Vector(Vector(Int))`), this shallow copy creates double-free risks or requires manual deep cloning (`Vector_clone`).

### Proposed Design
Introduce **Move Semantics** at the library level using a standard `move()` macro helper.
Moving a struct transfers its pointers to the destination and clears the source struct fields (setting capacity and data pointer to `NULL`), neutralizing double-frees without deep copy costs:

```c
#define move(x) \
    ({ \
        __typeof__(x) _tmp = x; \
        memset(&(x), 0, sizeof(x)); \
        _tmp; \
    })

// Usage: Zero-copy transfer of an owning nested vector
Vector(Int) inner = Vector_new(Int)();
Vector_push_back(Int)(&inner, 42);

Vector(Vector(Int)) outer = Vector_new(Vector(Int))();
Vector_push_back(Vector(Int))(&outer, move(inner)); // zero-copy move! inner is now cleared and safe to go out of scope.
```

### Advantages
* **Perfect Performance**: Replaces expensive heap allocations and element deep-copy loops with simple pointer copies and CPU register swaps.
* **Compile-Time Safety**: Standardizes resource-moving contracts in plain C.

---

## 4. Pluggable Memory Allocators (`allocator_api`)

### Background
Currently, `malloc`, `realloc`, and `free` are hardcoded under the hood of all dynamic containers. For embedded environments, high-performance engines, and real-time systems, controlling memory layout via custom arenas, pools, or stack allocators is crucial.

### Proposed Design
Define an implicit or explicit **Allocator API**:
```c
typedef struct {
    void* (*alloc)(void* ctx, Size size);
    void* (*realloc)(void* ctx, void* ptr, Size new_size);
    void (*free)(void* ctx, void* ptr);
    void* context;
} Allocator;
```
Containers would gain alternative constructors (`_in` variants):
```c
Allocator stack_alloc = stack_allocator_init(buffer, sizeof(buffer));

// Instantiates a vector that allocates solely from the stack buffer
Vector(Int) vec = Vector_new_in(Int)(&stack_alloc);
```

### Advantages
* **Zero Fragmentation**: Using Arena allocators eliminates fragmentation and speeds up allocation to O(1).
* **Freestanding Support**: Operates perfectly in environments lacking a standard libc heap.

---

## 5. Reference Counted Smart Pointers (`Rc(T)` and `Arc(T)`)

### Background
We have `Box(T)` for single-owner heap data, and `ref(T)`/`cref(T)` for borrowing. Building complex, cyclic graph structures (like trees, graphs, or multi-threaded shared states) under our owning model is difficult without shared ownership.

### Proposed Design
Introduce generic reference-counted pointer wrappers via template configurations:
* `Rc(T)`: Reference-counted heap wrapper for single-threaded usage (simple non-atomic ref count).
* `Arc(T)`: Atomic reference-counted heap wrapper for safe thread sharing, utilizing C11 `stdatomic.h`.

```c
// Instantiates a shared integer pointer on the heap
Arc(Int) shared_ptr = Arc_new(Int)(42);

// Spawns shared owners
Arc(Int) owner2 = Arc_clone(Int)(&shared_ptr); // Increments counter atomically

// The value 42 is automatically freed ONLY when both shared_ptr and owner2 are destroyed.
```

### Advantages
* **Bulletproof Safety**: Solves shared-data life management, eliminating memory leaks or dangling pointers in complex graphs.
* **Deterministic Destructors**: Combines perfectly with the existing container cleanup model.
