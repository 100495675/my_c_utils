# Architecture

## Purpose

my_c_utils is a header-only C11 library that provides reusable generic containers and explicit ownership helpers.

## Layout

- `include/my_c_utils/tipos.h`: canonical aliases for primitive C types used by the project.
- `include/my_c_utils/free.h`: trivial destructors for primitive aliases.
- `include/my_c_utils/result.h`: fallible operations with typed success values.
- `include/my_c_utils/option.h`: optional values with typed payloads.
- `include/my_c_utils/iterator.h`: `for_each_ref` and `for_each_copy` loop macros.
- `include/my_c_utils/vector.h`: generic dynamic array.
- `include/my_c_utils/linked_list.h`: generic singly linked list.
- `include/my_c_utils/hash_map.h`: generic open-addressing hash map.

## Ownership model

- Containers own the values they store.
- `push_back` and `add` copy values into storage.
- `set`, `remove`, `clear`, and `free` release owned elements before replacing or dropping them.
- `pop` returns a copy and transfers that copy to the caller.
- `get` and `at` return borrowed references wrapped in typed `Result` values.

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
