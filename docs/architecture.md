# Architecture

## Purpose

my_c_utils is a header-only C11 library that provides reusable generic containers and explicit ownership helpers.

## Layout

- `include/my_c_utils/tipos.h`: canonical aliases for primitive C types used by the project.
- `include/my_c_utils/free.h`: trivial destructors for primitive aliases.
- `include/my_c_utils/clone.h`: direct-return clone helpers for primitives.
- `include/my_c_utils/struct_config.h`: central macro for declaring structs plus `free` and `clone` helpers.
- `include/my_c_utils/core.h`: umbrella for types, free/clone, result/option, iterator, and struct helpers.
- `include/my_c_utils/containers.h`: umbrella for box, vector, linked list, and hash map helpers.
- `include/my_c_utils/my_c_utils.h`: single public entry point that includes core helpers and containers.
- `include/my_c_utils/result.h`: fallible operations with typed success values.
- `include/my_c_utils/option.h`: optional values with typed payloads.
- `include/my_c_utils/iterator.h`: `for_each_ref` and `for_each_copy` loop macros.
- `include/my_c_utils/box.h`: owning heap wrapper for a single value.
- `include/my_c_utils/vector.h`: generic dynamic array.
- `include/my_c_utils/linked_list.h`: generic singly linked list.
- `include/my_c_utils/hash_map.h`: generic open-addressing hash map.

## Ownership model

- `ref_##Type` is the mutable borrowed alias family.
- `cref_##Type` is the immutable borrowed alias family.
- Borrowed helpers exist only to keep references compatible with the generic destructor API; they never release memory.
- Containers own the values they store.
- `push_back` and `add` copy values into storage.
- `set`, `remove`, `clear`, and `free` release owned elements before replacing or dropping them.
- `pop` returns a copy and transfers that copy to the caller.
- `get` and `at` return borrowed references wrapped in typed `Result` values.
- `Box_Type` owns a heap-allocated `Type` value and provides `deref`, `into_inner`, and `free` helpers.
- `STRUCT_CONFIG(Type, field_type, field_name, ...)` declares the struct and generates `Type_free` and `Type_clone` under a single rule set.
- Read-only APIs should use `cref_##Type` so immutable borrows are visible at the call site.
- `Type_clone(const cref_##Type src)` returns a cloned value directly; it aborts on invalid input or allocation failure instead of returning `Result`.
- `STRUCT_CONFIG` is intended for plain data structs whose field list fully defines ownership.
- Container templates keep manual struct declarations when their invariants require specialized `free`, `clone`, or iterator state.

## Iterator contract

- Iteration is explicit and ownership-aware.
- `into_iter` creates a cursor.
- `iter_next` returns the next borrowed element wrapped in `Result_##Type##_ref` and advances the cursor.
- Exhaustion is reported as an error result.
- `for_each_ref` exposes borrowed pointers.
- `for_each_copy` exposes copied values.

## Type aliases

The project code uses the aliases from `tipos.h` consistently:

- `Int`, `UInt`, `Long`, `ULong`, `LongLong`, `ULongLong`
- `Char`, `SChar`, `UChar`, `WChar`
- `Float`, `Double`, `LongDouble`
- `Bool`, `Size`, `PtrDiff`, `IntPtr`, `UIntPtr`, `IntMax`, `UIntMax`

## Design constraints

- The library favors explicit ownership over implicit lifetime management.
- Nested containers are supported when nested types provide compatible destructors.
- The API remains header-only so consumers can compose templates-like specializations per type.
- Public types should be declared through `STRUCT_CONFIG` whenever possible so the associated `free` and `clone` helpers cannot drift from the struct layout.
