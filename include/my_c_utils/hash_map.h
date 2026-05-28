#ifndef MY_C_UTILS_HASHMAP_H
#define MY_C_UTILS_HASHMAP_H

#include "my_c_utils/allocator.h"
#include "my_c_utils/option.h"
#include "my_c_utils/iterator.h"
#include "my_c_utils/result.h"
#include "my_c_utils/primitives.h"
#include "my_c_utils/template.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_LOAD_FACTOR 0.75

// 1. User-Facing Macros (Prefix-free, template-compatible)
#define Hashmap(...) TEMPLATE_TYPE(Hashmap, __VA_ARGS__)
#define ref_Hashmap(...) TEMPLATE_TYPE(ref_Hashmap, __VA_ARGS__)
#define cref_Hashmap(...) TEMPLATE_TYPE(cref_Hashmap, __VA_ARGS__)

#define iter_Hashmap(...) TEMPLATE_TYPE(iter_Hashmap, __VA_ARGS__)
#define ref_iter_Hashmap(...) TEMPLATE_TYPE(ref_iter_Hashmap, __VA_ARGS__)
#define cref_iter_Hashmap(...) TEMPLATE_TYPE(cref_iter_Hashmap, __VA_ARGS__)

#define Entry(...) TEMPLATE_TYPE(Entry, __VA_ARGS__)
#define ref_Entry(...) TEMPLATE_TYPE(ref_Entry, __VA_ARGS__)
#define cref_Entry(...) TEMPLATE_TYPE(cref_Entry, __VA_ARGS__)

#define Hashmap_new(...) TEMPLATE_METHOD(Hashmap, new, __VA_ARGS__)
#define Hashmap_add(...) TEMPLATE_METHOD(Hashmap, add, __VA_ARGS__)
#define Hashmap_free(...) TEMPLATE_METHOD(Hashmap, free, __VA_ARGS__)
#define Hashmap_get(...) TEMPLATE_METHOD(Hashmap, get, __VA_ARGS__)
#define Hashmap_get_mut(...) TEMPLATE_METHOD(Hashmap, get_mut, __VA_ARGS__)
#define Hashmap_pop(...) TEMPLATE_METHOD(Hashmap, pop, __VA_ARGS__)
#define Hashmap_contains(...) TEMPLATE_METHOD(Hashmap, contains, __VA_ARGS__)
#define Hashmap_remove(...) TEMPLATE_METHOD(Hashmap, remove, __VA_ARGS__)
#define Hashmap_stats(...) TEMPLATE_METHOD(Hashmap, stats, __VA_ARGS__)
#define Hashmap_debug(...) TEMPLATE_METHOD(Hashmap, debug, __VA_ARGS__)
#define Hashmap_into_iter(...) TEMPLATE_METHOD(Hashmap, into_iter, __VA_ARGS__)
#define Hashmap_clone(...) TEMPLATE_METHOD(Hashmap, clone, __VA_ARGS__)

#define iter_Hashmap_deref(...) TEMPLATE_METHOD(iter_Hashmap, deref, __VA_ARGS__)
#define iter_Hashmap_next(...) TEMPLATE_METHOD(iter_Hashmap, next, __VA_ARGS__)
#define iter_Hashmap_free(...) TEMPLATE_METHOD(iter_Hashmap, free, __VA_ARGS__)

#define Hashmap_insert_entry(...) TEMPLATE_METHOD(Hashmap, insert_entry, __VA_ARGS__)
#define Hashmap_find_index(...) TEMPLATE_METHOD(Hashmap, find_index, __VA_ARGS__)
#define Hashmap_shift_delete(...) TEMPLATE_METHOD(Hashmap, shift_delete, __VA_ARGS__)
#define Hashmap_resize(...) TEMPLATE_METHOD(Hashmap, resize, __VA_ARGS__)
#define Hashmap_resize_if_needed(...) TEMPLATE_METHOD(Hashmap, resize_if_needed, __VA_ARGS__)
#define Hashmap_free_impl(...) TEMPLATE_METHOD(Hashmap, free_impl, __VA_ARGS__)
#define Hashmap_seek_next(...) TEMPLATE_METHOD(Hashmap, seek_next, __VA_ARGS__)

// Backward compatibility alias for the manual HASH_MAP_CONFIG
#define HASH_MAP_CONFIG(K, V) TEMPLATE_Hashmap(K, V)

// 2. Template Definition Macro
#define TEMPLATE_Hashmap(K, V)                                                                                        \
  typedef struct                                                                                                      \
  {                                                                                                                   \
    K key;                                                                                                            \
    V value;                                                                                                          \
    Size hash;                                                                                                        \
    Size distance;                                                                                                    \
    Bool filled;                                                                                                      \
  } Entry(K, V);                                                                                                      \
  typedef Entry(K, V) * ref_Entry(K, V);                                                                              \
  typedef const Entry(K, V) * cref_Entry(K, V);                                                                       \
  static inline void TEMPLATE_METHOD(ref_Entry, free, K, V)(ref_Entry(K, V) * value) { (void)value; }                 \
  static inline void TEMPLATE_METHOD(cref_Entry, free, K, V)(cref_Entry(K, V) * value) { (void)value; }               \
                                                                                                                      \
  typedef struct                                                                                                      \
  {                                                                                                                   \
    Size size;                                                                                                        \
    Size capacity;                                                                                                    \
    ref_Entry(K, V) data;                                                                                             \
  } Hashmap(K, V);                                                                                                    \
  typedef Hashmap(K, V) * ref_Hashmap(K, V);                                                                          \
  typedef const Hashmap(K, V) * cref_Hashmap(K, V);                                                                   \
  static inline void TEMPLATE_METHOD(ref_Hashmap, free, K, V)(ref_Hashmap(K, V) * value) { (void)value; }             \
  static inline void TEMPLATE_METHOD(cref_Hashmap, free, K, V)(cref_Hashmap(K, V) * value) { (void)value; }           \
                                                                                                                      \
  typedef struct                                                                                                      \
  {                                                                                                                   \
    cref_Hashmap(K, V) map;                                                                                           \
    Size current_index;                                                                                               \
  } iter_Hashmap(K, V);                                                                                               \
  typedef iter_Hashmap(K, V) * ref_iter_Hashmap(K, V);                                                                \
  typedef const iter_Hashmap(K, V) * cref_iter_Hashmap(K, V);                                                         \
  static inline void TEMPLATE_METHOD(ref_iter_Hashmap, free, K, V)(ref_iter_Hashmap(K, V) * value) { (void)value; }   \
  static inline void TEMPLATE_METHOD(cref_iter_Hashmap, free, K, V)(cref_iter_Hashmap(K, V) * value) { (void)value; } \
  static inline void iter_Hashmap_free(K, V)(cref_iter_Hashmap(K, V) value)                                           \
  {                                                                                                                   \
    (void)value;                                                                                                      \
  }                                                                                                                   \
                                                                                                                      \
  typedef struct                                                                                                      \
  {                                                                                                                   \
    Size size;                                                                                                        \
    Size capacity;                                                                                                    \
    Double load_factor;                                                                                               \
    Size collisions;                                                                                                  \
  } HashmapStats;                                                                                                     \
  typedef HashmapStats *ref_HashmapStats;                                                                             \
  typedef const HashmapStats *cref_HashmapStats;                                                                      \
  static inline void ref_HashmapStats_free(ref_HashmapStats *value) { (void)value; }                                  \
  static inline void cref_HashmapStats_free(cref_HashmapStats *value) { (void)value; }                                \
                                                                                                                      \
  static inline void Hashmap_insert_entry(K, V)(                                                                      \
      ref_Entry(K, V) data, Size capacity,                                                                            \
      Entry(K, V) entry)                                                                                              \
  {                                                                                                                   \
    Size index = entry.hash % capacity;                                                                               \
    while (true)                                                                                                      \
    {                                                                                                                 \
      if (!data[index].filled)                                                                                        \
      {                                                                                                               \
        data[index] = entry;                                                                                          \
        return;                                                                                                       \
      }                                                                                                               \
      if (data[index].distance < entry.distance)                                                                      \
      {                                                                                                               \
        Entry(K, V) temp = data[index];                                                                               \
        data[index] = entry;                                                                                          \
        entry = temp;                                                                                                 \
      }                                                                                                               \
      index = (index + 1) % capacity;                                                                                 \
      entry.distance++;                                                                                               \
    }                                                                                                                 \
  }                                                                                                                   \
                                                                                                                      \
  static inline Size Hashmap_find_index(K, V)(                                                                        \
      ref_Hashmap(K, V) self, K key)                                                                                  \
  {                                                                                                                   \
    Size key_hash = K##_hash(key);                                                                                    \
    Size index = key_hash % self->capacity;                                                                           \
    Size distance = 0;                                                                                                \
    while (self->data[index].filled)                                                                                  \
    {                                                                                                                 \
      if (self->data[index].hash == key_hash &&                                                                       \
          K##_equals(self->data[index].key, key))                                                                     \
      {                                                                                                               \
        return index;                                                                                                 \
      }                                                                                                               \
      if (self->data[index].distance < distance)                                                                      \
      {                                                                                                               \
        break;                                                                                                        \
      }                                                                                                               \
      index = (index + 1) % self->capacity;                                                                           \
      distance++;                                                                                                     \
    }                                                                                                                 \
    return self->capacity;                                                                                            \
  }                                                                                                                   \
                                                                                                                      \
  static inline void Hashmap_shift_delete(K, V)(                                                                      \
      ref_Hashmap(K, V) self, Size delete_index)                                                                      \
  {                                                                                                                   \
    Size hole = delete_index;                                                                                         \
    Size index = (hole + 1) % self->capacity;                                                                         \
    while (self->data[index].filled && self->data[index].distance > 0)                                                \
    {                                                                                                                 \
      self->data[hole] = self->data[index];                                                                           \
      self->data[hole].distance--;                                                                                    \
      hole = index;                                                                                                   \
      index = (index + 1) % self->capacity;                                                                           \
    }                                                                                                                 \
    self->data[hole].filled = false;                                                                                  \
    self->data[hole].distance = 0;                                                                                    \
  }                                                                                                                   \
                                                                                                                      \
  static inline Hashmap(K, V) Hashmap_new(K, V)(                                                                      \
      Size capacity)                                                                                                  \
  {                                                                                                                   \
    if (capacity == 0)                                                                                                \
    {                                                                                                                 \
      perror("Capacity must be greater than 0");                                                                      \
      exit(1);                                                                                                        \
    }                                                                                                                 \
    ref_Entry(K, V) data =                                                                                            \
        MY_C_UTILS_CALLOC(capacity, sizeof(Entry(K, V)));                                                             \
    if (!data)                                                                                                        \
    {                                                                                                                 \
      perror("Memory allocation failed");                                                                             \
      exit(1);                                                                                                        \
    }                                                                                                                 \
    return (Hashmap(K, V)){                                                                                           \
        .size = 0,                                                                                                    \
        .capacity = capacity,                                                                                         \
        .data = data};                                                                                                \
  }                                                                                                                   \
                                                                                                                      \
  static inline Result(Void, cref_Char) Hashmap_resize(K, V)(                                                         \
      ref_Hashmap(K, V) self, Size new_capacity)                                                                      \
  {                                                                                                                   \
    ref_Entry(K, V) new_data =                                                                                        \
        MY_C_UTILS_CALLOC(new_capacity, sizeof(Entry(K, V)));                                                         \
    if (!new_data)                                                                                                    \
    {                                                                                                                 \
      return Result_err(Void, cref_Char)("Memory allocation failed");                                                 \
    }                                                                                                                 \
    for (Size i = 0; i < self->capacity; i++)                                                                         \
    {                                                                                                                 \
      if (self->data[i].filled)                                                                                       \
      {                                                                                                               \
        Entry(K, V) entry = self->data[i];                                                                            \
        entry.distance = 0;                                                                                           \
        Hashmap_insert_entry(K, V)(new_data, new_capacity,                                                            \
                                   entry);                                                                            \
      }                                                                                                               \
    }                                                                                                                 \
    MY_C_UTILS_FREE(self->data);                                                                                      \
    self->data = new_data;                                                                                            \
    self->capacity = new_capacity;                                                                                    \
    return Result_ok(Void, cref_Char)((Void){});                                                                      \
  }                                                                                                                   \
                                                                                                                      \
  static inline Result(Void, cref_Char) Hashmap_resize_if_needed(K, V)(                                               \
      ref_Hashmap(K, V) self)                                                                                         \
  {                                                                                                                   \
    Double load_factor = (Double)self->size / self->capacity;                                                         \
    if (load_factor >= DEFAULT_LOAD_FACTOR)                                                                           \
    {                                                                                                                 \
      return Hashmap_resize(K, V)(self, self->capacity * 2);                                                          \
    }                                                                                                                 \
    return Result_ok(Void, cref_Char)((Void){});                                                                      \
  }                                                                                                                   \
                                                                                                                      \
  static inline Result(Void, cref_Char) Hashmap_add(K, V)(                                                            \
      ref_Hashmap(K, V) self, K key, V value)                                                                         \
  {                                                                                                                   \
    Size existing_index = Hashmap_find_index(K, V)(self, key);                                                        \
    if (existing_index < self->capacity)                                                                              \
    {                                                                                                                 \
      MY_C_UTILS_CONCAT(K, _free)(&self->data[existing_index].key);                                                   \
      MY_C_UTILS_CONCAT(V, _free)(&self->data[existing_index].value);                                                 \
      self->data[existing_index].key = key;                                                                           \
      self->data[existing_index].value = value;                                                                       \
      self->data[existing_index].hash = K##_hash(key);                                                                \
      return Result_ok(Void, cref_Char)((Void){});                                                                    \
    }                                                                                                                 \
    Result(Void, cref_Char) resize_res = Hashmap_resize_if_needed(K, V)(self);                                        \
    if (Result_is_err(Void, cref_Char)(&resize_res))                                                                  \
    {                                                                                                                 \
      return resize_res;                                                                                              \
    }                                                                                                                 \
    Entry(K, V) new_entry = {.key = key,                                                                              \
                             .value = value,                                                                          \
                             .hash = K##_hash(key),                                                                   \
                             .distance = 0,                                                                           \
                             .filled = true};                                                                         \
    Hashmap_insert_entry(K, V)(self->data, self->capacity,                                                            \
                               new_entry);                                                                            \
    self->size++;                                                                                                     \
    return Result_ok(Void, cref_Char)((Void){});                                                                      \
  }                                                                                                                   \
                                                                                                                      \
  static inline void Hashmap_free(K, V)(                                                                              \
      ref_Hashmap(K, V) self)                                                                                         \
  {                                                                                                                   \
    for (Size i = 0; i < self->capacity; i++)                                                                         \
    {                                                                                                                 \
      if (self->data[i].filled)                                                                                       \
      {                                                                                                               \
        MY_C_UTILS_CONCAT(K, _free)(&self->data[i].key);                                                              \
        MY_C_UTILS_CONCAT(V, _free)(&self->data[i].value);                                                            \
      }                                                                                                               \
    }                                                                                                                 \
    MY_C_UTILS_FREE(self->data);                                                                                      \
    self->data = NULL;                                                                                                \
    self->size = 0;                                                                                                   \
    self->capacity = 0;                                                                                               \
  }                                                                                                                   \
                                                                                                                      \
  static inline Result(ref_##V, cref_Char) Hashmap_get(K, V)(                                                         \
      ref_Hashmap(K, V) self, K key)                                                                                  \
  {                                                                                                                   \
    Size index = Hashmap_find_index(K, V)(self, key);                                                                 \
    if (index < self->capacity)                                                                                       \
    {                                                                                                                 \
      return Result_ok(ref_##V, cref_Char)(&self->data[index].value);                                                 \
    }                                                                                                                 \
    return Result_err(ref_##V, cref_Char)("no existe elemento con esa Key");                                          \
  }                                                                                                                   \
                                                                                                                      \
  static inline Result(ref_##V, cref_Char) Hashmap_get_mut(K, V)(                                                     \
      ref_Hashmap(K, V) self, K key)                                                                                  \
  {                                                                                                                   \
    return Hashmap_get(K, V)(self, key);                                                                              \
  }                                                                                                                   \
                                                                                                                      \
  static inline Result(V, cref_Char) Hashmap_pop(K, V)(                                                               \
      ref_Hashmap(K, V) self, K key)                                                                                  \
  {                                                                                                                   \
    Size index = Hashmap_find_index(K, V)(self, key);                                                                 \
    if (index < self->capacity)                                                                                       \
    {                                                                                                                 \
      V value = self->data[index].value;                                                                              \
      MY_C_UTILS_CONCAT(K, _free)(&self->data[index].key);                                                            \
      self->size--;                                                                                                   \
      Hashmap_shift_delete(K, V)(self, index);                                                                        \
      return Result_ok(V, cref_Char)(value);                                                                          \
    }                                                                                                                 \
    return Result_err(V, cref_Char)("no existe elemento con esa Key");                                                \
  }                                                                                                                   \
                                                                                                                      \
  static inline Bool Hashmap_contains(K, V)(                                                                          \
      ref_Hashmap(K, V) self, K key)                                                                                  \
  {                                                                                                                   \
    return Hashmap_find_index(K, V)(self, key) < self->capacity;                                                      \
  }                                                                                                                   \
                                                                                                                      \
  static inline void Hashmap_remove(K, V)(                                                                            \
      ref_Hashmap(K, V) self, K key)                                                                                  \
  {                                                                                                                   \
    Size index = Hashmap_find_index(K, V)(self, key);                                                                 \
    if (index < self->capacity)                                                                                       \
    {                                                                                                                 \
      MY_C_UTILS_CONCAT(K, _free)(&self->data[index].key);                                                            \
      MY_C_UTILS_CONCAT(V, _free)(&self->data[index].value);                                                          \
      self->size--;                                                                                                   \
      Hashmap_shift_delete(K, V)(self, index);                                                                        \
    }                                                                                                                 \
  }                                                                                                                   \
                                                                                                                      \
  static inline HashmapStats Hashmap_stats(K, V)(                                                                     \
      cref_Hashmap(K, V) self)                                                                                        \
  {                                                                                                                   \
    HashmapStats stats = {.size = self->size,                                                                         \
                          .capacity = self->capacity,                                                                 \
                          .load_factor = (Double)self->size / self->capacity,                                         \
                          .collisions = 0};                                                                           \
    for (Size i = 0; i < self->capacity; i++)                                                                         \
    {                                                                                                                 \
      if (self->data[i].filled && self->data[i].distance > 0)                                                         \
      {                                                                                                               \
        stats.collisions++;                                                                                           \
      }                                                                                                               \
    }                                                                                                                 \
    return stats;                                                                                                     \
  }                                                                                                                   \
                                                                                                                      \
  static inline Size Hashmap_seek_next(K, V)(                                                                         \
      cref_Hashmap(K, V) self, Size start_index)                                                                      \
  {                                                                                                                   \
    Size index = start_index;                                                                                         \
    while (index < self->capacity && !self->data[index].filled)                                                       \
    {                                                                                                                 \
      index++;                                                                                                        \
    }                                                                                                                 \
    return index;                                                                                                     \
  }                                                                                                                   \
                                                                                                                      \
  static inline void Hashmap_debug(K, V)(                                                                             \
      cref_Hashmap(K, V) self)                                                                                        \
  {                                                                                                                   \
    HashmapStats stats = Hashmap_stats(K, V)(self);                                                                   \
    printf("size: %zu, capacity: %zu, load factor: %f, collisions: %zu\n",                                            \
           stats.size, stats.capacity, stats.load_factor, stats.collisions);                                          \
    printf("{\n");                                                                                                    \
    for (Size i = 0; i < self->capacity; i++)                                                                         \
    {                                                                                                                 \
      if (self->data[i].filled)                                                                                       \
      {                                                                                                               \
        printf("  index: %zu, distance: %zu, hash: %zu\n", i,                                                         \
               self->data[i].distance, self->data[i].hash);                                                           \
      }                                                                                                               \
    }                                                                                                                 \
    printf("}\n");                                                                                                    \
  }                                                                                                                   \
                                                                                                                      \
  static inline iter_Hashmap(K, V)                                                                                    \
      Hashmap_into_iter(K, V)(                                                                                        \
          cref_Hashmap(K, V) self)                                                                                    \
  {                                                                                                                   \
    Size first_index = Hashmap_seek_next(K, V)(self, 0);                                                              \
    return (iter_Hashmap(K, V)){.map = self,                                                                          \
                                .current_index = first_index};                                                        \
  }                                                                                                                   \
                                                                                                                      \
  static inline Result(ref_##V, cref_Char) iter_Hashmap_deref(K, V)(                                                  \
      cref_iter_Hashmap(K, V) self)                                                                                   \
  {                                                                                                                   \
    if (self->current_index >= self->map->capacity ||                                                                 \
        !self->map->data[self->current_index].filled)                                                                 \
    {                                                                                                                 \
      return Result_err(ref_##V, cref_Char)("Iterator out of bounds");                                                \
    }                                                                                                                 \
    return Result_ok(ref_##V, cref_Char)(                                                                             \
        &self->map->data[self->current_index].value);                                                                 \
  }                                                                                                                   \
                                                                                                                      \
  static inline Result(ref_##V, cref_Char) iter_Hashmap_next(K, V)(                                                   \
      ref_iter_Hashmap(K, V) self)                                                                                    \
  {                                                                                                                   \
    if (self->current_index >= self->map->capacity ||                                                                 \
        !self->map->data[self->current_index].filled)                                                                 \
    {                                                                                                                 \
      return Result_err(ref_##V, cref_Char)("Iterator out of bounds");                                                \
    }                                                                                                                 \
    Size cur = self->current_index;                                                                                   \
    self->current_index =                                                                                             \
        Hashmap_seek_next(K, V)(self->map,                                                                            \
                                self->current_index + 1);                                                             \
    return Result_ok(ref_##V, cref_Char)(&self->map->data[cur].value);                                                \
  }                                                                                                                   \
  RESULT_CONFIG(Hashmap(K, V), cref_Char)                                                                             \
  RESULT_CONFIG(ref_Hashmap(K, V), cref_Char)                                                                         \
  static inline Hashmap(K, V) Hashmap_clone(K, V)(                                                                    \
      cref_Hashmap(K, V) self)                                                                                        \
  {                                                                                                                   \
    if (!self)                                                                                                        \
    {                                                                                                                 \
      perror("Cannot clone NULL Hashmap pointer");                                                                    \
      exit(1);                                                                                                        \
    }                                                                                                                 \
    Hashmap(K, V) dest = Hashmap_new(K, V)(self->capacity);                                                           \
    for (Size i = 0; i < self->capacity; ++i)                                                                         \
    {                                                                                                                 \
      if (!self->data[i].filled)                                                                                      \
        continue;                                                                                                     \
      K key_clone = K##_clone(&self->data[i].key);                                                                    \
      V value_clone = V##_clone(&self->data[i].value);                                                                \
      Result(Void, cref_Char) add_res = Hashmap_add(K, V)(&dest, key_clone, value_clone);                             \
      if (Result_is_err(Void, cref_Char)(&add_res))                                                                   \
      {                                                                                                               \
        Hashmap_free(K, V)(&dest);                                                                                    \
        perror("Memory allocation failed during hashmap clone");                                                      \
        exit(1);                                                                                                      \
      }                                                                                                               \
    }                                                                                                                 \
    return dest;                                                                                                      \
  }

#endif
