#ifndef OPTION_H
#define OPTION_H

#define OPTION_CONFIG(Type)                                        \
  typedef struct                                                   \
  {                                                                \
    Type value;                                                    \
    bool filled;                                                   \
  } Option_##Type;                                                 \
                                                                   \
  static inline Option_##Type option_##Type##_some(Type value)     \
  {                                                                \
    return (Option_##Type){.value = value, .filled = true};        \
  }                                                                \
                                                                   \
  static inline Option_##Type option_##Type##_none()               \
  {                                                                \
    return (Option_##Type){.filled = false};                       \
  }                                                                \
                                                                   \
  static inline Type option_##Type##_unwrap(Option_##Type option)  \
  {                                                                \
    if (!option.filled)                                            \
    {                                                              \
      perror("Option is none");                                    \
      exit(1);                                                     \
    }                                                              \
    return option.value;                                           \
  }                                                                \
                                                                   \
  static inline bool option_##Type##_is_none(Option_##Type option) \
  {                                                                \
    return !option.filled;                                         \
  }                                                                \
                                                                   \
  static inline bool option_##Type##_is_some(Option_##Type option) \
  {                                                                \
    return option.filled;                                          \
  }

#endif