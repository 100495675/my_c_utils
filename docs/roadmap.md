# Roadmap

## Near-term priorities

1. Keep the structured test layout growing by module: `tests/unit/` for isolated behavior and `tests/integration/` for nested and cross-container cases.
2. Keep expanding the expected-failure suite under `tests/expected_fail/`, especially `Result` unwraps on error, `Option` unwraps on none, and iterator exhaustion.
3. Keep the dedicated success/failure runners aligned with CI and editor tasks.
4. Decide whether public debug helpers should remain in the core headers or move behind compile-time flags.
5. Add allocator hooks if the library should be embedded in constrained or custom-memory projects.
6. Add deep clone helpers for nested containers that need explicit duplication semantics.

## Structural follow-ups

1. Split test support into reusable helpers for strings, temporary files, and subprocess assertions.
2. Keep documentation synchronized with ownership and iterator contracts whenever a public macro changes.
3. Keep the public headers layered so most consumers can include a single umbrella header instead of learning internal dependencies like `tipos.h`.
4. Consider a tagged release once the current header layout, ownership rules, and iterator contract stop moving.

## Release target

A sensible next milestone is a `v0.1.0` release focused on:

- stable include layout
- stable ownership rules
- explicit iterator semantics
- a documented, recursive test suite
