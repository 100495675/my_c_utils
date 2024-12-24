#ifndef VECTOR_H
#define VECTOR_H

#include "result.h"

#define VECTOR_CONFIG(Type)                                                    \
  typedef struct {                                                             \
    Type *data;                                                                \
    size_t size;                                                               \
    size_t capacity;                                                           \
  } Vector_##Type;                                                             \
                                                                               \
  typedef struct {                                                             \
    Vector_##Type vector;                                                      \
    size_t index;                                                              \
  } Vector_##Type##_iter;                                                      \
                                                                               \
  RESULT_CONFIG(Type)                                                          \
  typedef Type *Type##_ref;                                                    \
  RESULT_CONFIG(Type##_ref)                                                    \
                                                                               \
  /* Create a new vector */                                                    \
  static inline Vector_##Type Vector_##Type##_new() {                          \
    return (Vector_##Type){.data = NULL, .size = 0, .capacity = 0};            \
  }                                                                            \
                                                                               \
  /* Add an element to the end of the vector */                                \
  static inline Result Vector_##Type##_push_back(Vector_##Type *self,          \
                                                 const Type value) {           \
    if (self->size == self->capacity) {                                        \
      self->capacity = self->capacity == 0 ? 1 : self->capacity * 2;           \
      Type *new_data = realloc(self->data, self->capacity * sizeof(Type));     \
      if (!new_data) {                                                         \
        return Result_err("Memory allocation failed");                         \
      }                                                                        \
      self->data = new_data;                                                   \
    }                                                                          \
    self->data[self->size++] = value;                                          \
    return Result_ok();                                                        \
  }                                                                            \
                                                                               \
  /* Get the element at the specified index */                                 \
  static inline Result_##Type##_ref Vector_##Type##_at(                        \
      const Vector_##Type *self, const size_t index) {                         \
    if (index >= self->size) {                                                 \
      return Result_##Type##_ref_err("Index out of bounds");                   \
    }                                                                          \
    return Result_##Type##_ref_ok(&self->data[index]);                         \
  }                                                                            \
                                                                               \
  /* Get the size of the vector */                                             \
  static inline size_t Vector_##Type##_size(const Vector_##Type *self) {       \
    return self->size;                                                         \
  }                                                                            \
                                                                               \
  /* Set the element at the specified index */                                 \
  static inline Result Vector_##Type##_set(                                    \
      Vector_##Type *self, const size_t index, const Type value) {             \
    if (index >= self->size) {                                                 \
      return Result_err("Index out of bounds");                                \
    }                                                                          \
    Type##_free(self->data[index]);                                            \
    self->data[index] = value;                                                 \
    return Result_ok();                                                        \
  }                                                                            \
                                                                               \
  /* Clear the vector */                                                       \
  static inline void Vector_##Type##_clear(Vector_##Type *self) {              \
    for (size_t i = 0; i < self->size; ++i) {                                  \
      Type##_free(self->data[i]);                                              \
    }                                                                          \
    self->size = 0;                                                            \
  }                                                                            \
                                                                               \
  /* Free the memory allocated for the vector */                               \
  static inline void Vector_##Type##_free(Vector_##Type *self) {               \
    for (size_t i = 0; i < self->size; ++i) {                                  \
      Type##_free(self->data[i]);                                              \
    }                                                                          \
    free(self->data);                                                          \
    self->data = NULL;                                                         \
    self->size = 0;                                                            \
    self->capacity = 0;                                                        \
  }                                                                            \
                                                                               \
  /* Reserve memory for the vector */                                          \
  static inline Result Vector_##Type##_reserve(Vector_##Type *self,            \
                                               const size_t capacity) {        \
    if (capacity > self->capacity) {                                           \
      Type *new_data = realloc(self->data, capacity * sizeof(Type));           \
      if (!new_data) {                                                         \
        return Result_err("Memory allocation failed");                         \
      }                                                                        \
      self->data = new_data;                                                   \
      self->capacity = capacity;                                               \
    }                                                                          \
    return Result_ok();                                                        \
  }                                                                            \
                                                                               \
  /* Remove the last element from the vector */                                \
  static inline Result_##Type Vector_##Type##_pop_back(Vector_##Type *self) {  \
    if (self->size == 0) {                                                     \
      return Result_##Type##_err("Vector is empty");                           \
    }                                                                          \
    return Result_##Type##_ok(self->data[--self->size]);                       \
  }                                                                            \
                                                                               \
  /* Print the vector */                                                       \
  static inline void Vector_##Type##_debug(const Vector_##Type *self) {        \
    printf("Vector: size=%zu, capacity=%zu{\n", self->size, self->capacity);   \
    for (size_t i = 0; i < self->size; ++i) {                                  \
      printf("  %d,\n", self->data[i]);                                        \
    }                                                                          \
    printf("}\n");                                                             \
  }                                                                            \
                                                                               \
  /* Create an iterator for the vector */                                      \
  static inline Vector_##Type##_iter Vector_##Type##_into_iter(                \
      const Vector_##Type *self) {                                             \
    return (Vector_##Type##_iter){.vector = *self, .index = 0};                \
  }                                                                            \
                                                                               \
  /* Dereference the iterator to get the current element */                    \
  static inline Result_##Type##_ref Vector_##Type##_iter_deref(                \
      const Vector_##Type##_iter *self) {                                      \
    if (self->index >= self->vector.size) {                                    \
      return Result_##Type##_ref_err("Iterator out of bounds");                \
    }                                                                          \
    return Result_##Type##_ref_ok(&(self->vector.data[self->index]));          \
  }                                                                            \
                                                                               \
  /* Move the iterator to the next element */                                  \
  static inline void Vector_##Type##_iter_next(Vector_##Type##_iter *self) {   \
    self->index++;                                                             \
  }                                                                            \
                                                                               \
  /* Check if two iterators are equal */                                       \
  static inline bool Vector_##Type##_iter_eq(                                  \
      const Vector_##Type##_iter *self, const Vector_##Type##_iter other) {    \
    return self->vector.data == other.vector.data &&                           \
           self->index == other.index;                                         \
  }

#define vector_for(Type, var_name, vector_ref, body)                           \
  for (Vector_##Type##_iter _it = Vector_##Type##_into_iter(vector_ref);       \
       Result_##Type##_ref_is_ok(Vector_##Type##_iter_deref(&_it));            \
       Vector_##Type##_iter_next(&_it)) {                                      \
    Type *var_name = &(_it.vector.data[_it.index]);                            \
    body                                                                       \
  }

#endif