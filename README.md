# my_c_utils

Header-only C11 utilities for generic containers, typed result/option helpers, and explicit ownership-aware iteration.

## What it provides

- Generic result helpers.
- Generic option helpers.
- A dynamic vector.
- A singly linked list.
- An open-addressing hash map.
- Reference and copy-based iteration macros.
- A single umbrella header for users who do not want to manage individual includes.

## Project layout

- `include/my_c_utils/`: public headers.
- `tests/`: executable tests organized by unit and integration scope.
- `docs/`: architecture, ownership, testing, and roadmap notes.

## Core contracts

- Containers own stored values.
- `Type_free(Type *value)` destroys owned elements.
- `get` and `at` return borrowed references.
- `pop` returns owned copies.
- `for_each_ref` exposes borrowed pointers.
- `for_each_copy` exposes copied values.

## Entry points

Most users should include [include/my_c_utils/my_c_utils.h](include/my_c_utils/my_c_utils.h) and let it pull in the core helpers, containers, and struct macro layer in the correct order.

Lower-level headers remain available when a translation unit wants a narrower dependency surface:

- [include/my_c_utils/core.h](include/my_c_utils/core.h) for types, result/option, iterator, free/clone, and struct macros.
- [include/my_c_utils/containers.h](include/my_c_utils/containers.h) for box/vector/list/hash map.

## Build and test

```sh
make
make test-pass
make test-fail
make clean
```

## Documentation

- [Architecture](docs/architecture.md)
- [Ownership rules](docs/ownership.md)
- [Testing strategy](docs/testing.md)
- [Roadmap](docs/roadmap.md)
