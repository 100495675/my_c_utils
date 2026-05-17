# my_c_utils

A small header-only C11 utility library for reusable generic containers and value/result helpers.

## Structure

- `include/my_c_utils/` public headers for consumers
- `tests/` assertion-based executable tests and examples
- `docs/` design notes, ownership rules, and roadmap

## Public modules

- `result.h` generic error/value return helpers
- `option.h` optional value helpers
- `vector.h` generic dynamic array
- `linked_list.h` generic singly linked list
- `hash_map.h` generic open-addressing hash map

## Ownership model

Containers own the values they store.

- Stored elements are copied into the container.
- `Type_free(Type value)` destroys owned elements.
- `Type_ref` and `Value_ref` are borrowed pointers into container storage.
- `pop_back`/`pop` transfer a copy of the stored value back to the caller.
- `set`, `remove`, `free`, and `clear` destroy owned elements before replacing or releasing them.

This makes nested containers possible as long as each nested type exposes a compatible destructor.

## Build

Compile tests with the public include directory on the include path:

```sh
gcc -std=c11 -Wall -Wextra -pedantic -Iinclude tests/vector_test.c -o /tmp/vector_test
gcc -std=c11 -Wall -Wextra -pedantic -Iinclude tests/linked_test.c -o /tmp/linked_test
gcc -std=c11 -Wall -Wextra -pedantic -Iinclude tests/basic_test.c -o /tmp/basic_test
```

## Notes

- The hash map uses open addressing with backward-shift deletion.
- Iteration is currently container-specific, not a shared iterator base.
- The current public API favors header-only reuse and explicit ownership hooks.
