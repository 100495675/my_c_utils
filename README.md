# my_c_utils

Header-only C11 utilities for generic containers, typed result/option helpers, and explicit ownership-aware iteration.

## What it provides

- Generic result helpers.
- Generic option helpers.
- A dynamic vector.
- A singly linked list.
- An open-addressing hash map.
- Reference and copy-based iteration macros.

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
