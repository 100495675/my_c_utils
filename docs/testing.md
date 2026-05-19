# Testing Strategy

## Goals

The test suite is organized to validate the library as a real reusable C project, not only as a collection of smoke tests.

## Structure

- `tests/unit/result_option/`: success and failure cases for `Result` and `Option`.
- `tests/expected_fail/`: binaries that are expected to terminate with a non-zero exit status.
- `tests/unit/vector/`: vector boundary and mutation behavior.
- `tests/unit/linked_list/`: linked list push/pop and iteration behavior.
- `tests/unit/hash_map/`: insertion, collision handling, resize, deletion, and retrieval.
- `tests/integration/`: nested container cases and iterator behavior across modules.
- `tests/support/`: shared helpers used by multiple test programs.

## Covered cases

### Result and Option

- unwrap of valid values
- free on success and on error states
- typed payloads with primitive and pointer-backed values
- expected-failure programs for unwrap-on-error and unwrap-on-none

### Vector

- empty vector
- push, pop, reserve, set, and clear
- iteration by reference and by copy
- mutation during iteration

### Linked list

- empty list
- single-element and multi-element lists
- pop_back behavior
- iteration over all elements

### Hash map

- insert and lookup
- replace existing keys
- collision chains
- deletion after probing
- resize growth
- nested value types

### Integration

- nested vectors
- list-of-vectors
- hash map values that are vectors
- reference and copy iteration macros

## Running tests

Use `make` to compile and execute the full suite.

- `make test-pass` runs unit and integration success cases.
- `make test-fail` runs expected-failure binaries and confirms they exit non-zero.
- `make clean` removes generated binaries.
