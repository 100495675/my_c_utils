#ifndef RESULT_H
#define RESULT_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#define RESULT_CONFIG(Type)                                                    \
  typedef struct {                                                             \
    union {                                                                    \
      Type value;                                                              \
      char *error_message;                                                     \
    };                                                                         \
    bool is_error;                                                             \
  } Result_##Type;                                                             \
                                                                               \
  static inline Result_##Type Result_##Type##_ok(Type value) {                 \
    return (Result_##Type){.value = value, .is_error = false};                 \
  }                                                                            \
                                                                               \
  static inline Result_##Type Result_##Type##_err(char *error_message) {       \
    return (Result_##Type){.error_message = error_message, .is_error = true};  \
  }                                                                            \
                                                                               \
  static inline Type Result_##Type##_unwrap(Result_##Type result) {            \
    if (result.is_error) {                                                     \
      fprintf(stderr, "Unwrap on error: %s\n", result.error_message);          \
      exit(1);                                                                 \
    }                                                                          \
    return result.value;                                                       \
  }                                                                            \
                                                                               \
  static inline bool Result_##Type##_is_err(Result_##Type result) {            \
    return result.is_error;                                                    \
  }                                                                            \
                                                                               \
  static inline bool Result_##Type##_is_ok(Result_##Type result) {             \
    return !result.is_error;                                                   \
  }                                                                            \
                                                                               \
  static inline char *Result_##Type##_unwrap_err(Result_##Type result) {       \
    if (!result.is_error) {                                                    \
      perror("Result is ok");                                                  \
      exit(1);                                                                 \
    }                                                                          \
    return result.error_message;                                               \
  }

typedef struct {
  char *error_message;
  bool is_error;
} Result;

static inline Result Result_ok() { return (Result){.is_error = false}; }

static inline Result Result_err(char *error_message) {
  return (Result){.error_message = error_message, .is_error = true};
}

static inline void Result_unwrap(Result result) {
  if (result.is_error) {
    fprintf(stderr, "Unwrap on error: %s\n", result.error_message);
    exit(1);
  }
}

static inline bool Result_is_err(Result result) { return result.is_error; }

static inline bool Result_is_ok(Result result) { return !result.is_error; }

static inline char *Result_unwrap_err(Result result) {
  if (!result.is_error) {
    perror("Result is ok");
    exit(1);
  }
  return result.error_message;
}

#endif