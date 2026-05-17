# Roadmap

## Next steps

1. Add a dedicated test runner or task configuration for the files in `tests/`.
2. Add collision-heavy tests for the hash map, especially deletion after probing and resize behavior.
3. Add tests for nested containers, such as `List_Vector_Char` and `Hashmap_String_Vector_Int`.
4. Decide whether `debug` helpers should stay in the public headers or move behind optional compile-time flags.
5. Add a consistent include guard and naming convention audit for all public macros and types.
6. Consider optional clone/copy helpers for containers that need deep duplication.
7. Consider allocator hooks if the library should be embedded in constrained or custom-memory projects.

## Versioning target

The current code is at the stage where API contracts are still being stabilized. A good next milestone is a `v0.1.0` release focused on:

- stable ownership rules
- stable include layout
- assertion-based tests
- documented container semantics
