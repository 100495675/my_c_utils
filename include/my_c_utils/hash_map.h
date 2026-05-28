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

/**
 * @brief Represents a generic owning hash map using open addressing and Robin Hood collision resolution.
 * @usage Hashmap(K, V)
 */
#define Hashmap(...) TEMPLATE_TYPE(Hashmap, __VA_ARGS__)

/**
 * @brief Mutable borrowed pointer reference to Hashmap(K, V).
 * @usage ref_Hashmap(K, V)
 */
#define ref_Hashmap(...) TEMPLATE_TYPE(ref_Hashmap, __VA_ARGS__)

/**
 * @brief Immutable borrowed pointer reference to Hashmap(K, V).
 * @usage cref_Hashmap(K, V)
 */
#define cref_Hashmap(...) TEMPLATE_TYPE(cref_Hashmap, __VA_ARGS__)

/**
 * @brief Represents a generic iterator for Hashmap(K, V).
 * @usage iter_Hashmap(K, V)
 */
#define iter_Hashmap(...) TEMPLATE_TYPE(iter_Hashmap, __VA_ARGS__)

/**
 * @brief Mutable reference to a hash map iterator.
 */
#define ref_iter_Hashmap(...) TEMPLATE_TYPE(ref_iter_Hashmap, __VA_ARGS__)

/**
 * @brief Immutable reference to a hash map iterator.
 */
#define cref_iter_Hashmap(...) TEMPLATE_TYPE(cref_iter_Hashmap, __VA_ARGS__)

/**
 * @brief Represents an internal key-value entry in Hashmap.
 * @usage Entry(K, V)
 */
#define Entry(...) TEMPLATE_TYPE(Entry, __VA_ARGS__)

/**
 * @brief Mutable reference to a hash map entry.
 */
#define ref_Entry(...) TEMPLATE_TYPE(ref_Entry, __VA_ARGS__)

/**
 * @brief Immutable reference to a hash map entry.
 */
#define cref_Entry(...) TEMPLATE_TYPE(cref_Entry, __VA_ARGS__)

/**
 * @brief Constructs a new, empty Hashmap with the specified initial bucket count.
 * @param K The Key type.
 * @param V The Value type.
 * @param initial_capacity The initial number of buckets (e.g. 4, 8, 16).
 * @returns Hashmap(K, V)
 * @usage Hashmap(String, Int) map = Hashmap_new(String, Int)(4);
 */
#define Hashmap_new(...) TEMPLATE_METHOD(Hashmap, new, __VA_ARGS__)

/**
 * @brief Inserts or updates a key-value pair in the Hashmap.
 * @param K The Key type.
 * @param V The Value type.
 * @param self Pointer to the Hashmap instance (&my_map).
 * @param key The key of type K to add (ownership is transferred to map).
 * @param value The value of type V to add (copied into map).
 * @returns Result(Void, cref(Char))
 * @usage Hashmap_add(String, Int)(&my_map, string_dup("key"), 100);
 */
#define Hashmap_add(...) TEMPLATE_METHOD(Hashmap, add, __VA_ARGS__)

/**
 * @brief Destroys the Hashmap, freeing all populated keys, values, and bucket storage.
 * @param K The Key type.
 * @param V The Value type.
 * @param self Pointer to the Hashmap instance.
 * @usage Hashmap_free(String, Int)(&my_map);
 */
#define Hashmap_free(...) TEMPLATE_METHOD(Hashmap, free, __VA_ARGS__)

/**
 * @brief Retrieves a borrowed pointer (ref(V)) to the value associated with the key.
 * @param K The Key type.
 * @param V The Value type.
 * @param self Pointer to the Hashmap instance.
 * @param key The search key of type K.
 * @returns Result(ref(V), cref(Char))
 * @usage Result(ref_Int, cref(Char)) r = Hashmap_get(String, Int)(&my_map, "key");
 */
#define Hashmap_get(...) TEMPLATE_METHOD(Hashmap, get, __VA_ARGS__)

/**
 * @brief Retrieves a mutable borrowed pointer (ref(V)) to the value associated with the key.
 * @param K The Key type.
 * @param V The Value type.
 * @param self Pointer to the Hashmap instance.
 * @param key The search key of type K.
 * @returns Result(ref(V), cref(Char))
 * @usage *Result_unwrap(ref_Int, ...)(Hashmap_get_mut(String, Int)(&my_map, "key")) = 999;
 */
#define Hashmap_get_mut(...) TEMPLATE_METHOD(Hashmap, get_mut, __VA_ARGS__)

/**
 * @brief Removes the entry for the key, and returns its owned value.
 * @param K The Key type.
 * @param V The Value type.
 * @param self Pointer to the Hashmap instance.
 * @param key The key to pop.
 * @returns Result(V, cref(Char))
 * @usage Result(Int, cref(Char)) popped = Hashmap_pop(String, Int)(&my_map, "key");
 */
#define Hashmap_pop(...) TEMPLATE_METHOD(Hashmap, pop, __VA_ARGS__)

/**
 * @brief Checks if the Hashmap contains the specified key.
 * @param K The Key type.
 * @param V The Value type.
 * @param self Pointer to the Hashmap instance.
 * @param key The search key of type K.
 * @returns Bool
 * @usage Bool exists = Hashmap_contains(String, Int)(&my_map, "key");
 */
#define Hashmap_contains(...) TEMPLATE_METHOD(Hashmap, contains, __VA_ARGS__)

/**
 * @brief Removes the entry associated with the key, destroying its key and value.
 * @param K The Key type.
 * @param V The Value type.
 * @param self Pointer to the Hashmap instance.
 * @param key The key to remove.
 * @usage Hashmap_remove(String, Int)(&my_map, "key");
 */
#define Hashmap_remove(...) TEMPLATE_METHOD(Hashmap, remove, __VA_ARGS__)

/**
 * @brief Returns map statistics (size, capacity, load factor).
 * @returns HashmapStats
 * @usage HashmapStats stats = Hashmap_stats(String, Int)(&my_map);
 */
#define Hashmap_stats(...) TEMPLATE_METHOD(Hashmap, stats, __VA_ARGS__)

/**
 * @brief Prints debug information about the Hashmap buckets.
 */
#define Hashmap_debug(...) TEMPLATE_METHOD(Hashmap, debug, __VA_ARGS__)

/**
 * @brief Creates an iterator starting at the first bucket of the map.
 * @returns iter_Hashmap(K, V)
 * @usage iter_Hashmap(String, Int) it = Hashmap_into_iter(String, Int)(&my_map);
 */
#define Hashmap_into_iter(...) TEMPLATE_METHOD(Hashmap, into_iter, __VA_ARGS__)

/**
 * @brief Clones the Hashmap, creating a completely independent deep copy.
 * @returns Hashmap(K, V)
 * @usage Hashmap(String, Int) cloned = Hashmap_clone(String, Int)(&my_map);
 */
#define Hashmap_clone(...) TEMPLATE_METHOD(Hashmap, clone, __VA_ARGS__)

/**
 * @brief Returns the value (ref(V)) of the current populated entry in the iterator.
 * @returns Result(ref(V), cref(Char))
 * @usage Result(ref_Int, cref(Char)) r = iter_Hashmap_deref(String, Int)(&it);
 */
#define iter_Hashmap_deref(...) TEMPLATE_METHOD(iter_Hashmap, deref, __VA_ARGS__)

/**
 * @brief Advances the iterator to the next populated entry.
 * @returns Result(ref(V), cref(Char))
 * @usage Result(ref_Int, cref(Char)) r = iter_Hashmap_next(String, Int)(&it);
 */
#define iter_Hashmap_next(...) TEMPLATE_METHOD(iter_Hashmap, next, __VA_ARGS__)

/**
 * @brief Destroys and cleans up the iterator.
 * @usage iter_Hashmap_free(String, Int)(&it);
 */
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
  static inline void TEMPLATE_METHOD(Hashmap, free, K, V)(ref_Hashmap(K, V) self); \
  \
  typedef struct                                                                                                      \
  {                                                                                                                   \
    Hashmap(K, V) map;                                                                                                \
    Size current_index;                                                                                               \
  } iter_Hashmap(K, V);                                                                                               \
  typedef iter_Hashmap(K, V) * ref_iter_Hashmap(K, V);                                                                \
  typedef const iter_Hashmap(K, V) * cref_iter_Hashmap(K, V);                                                         \
  static inline void TEMPLATE_METHOD(ref_iter_Hashmap, free, K, V)(ref_iter_Hashmap(K, V) * value) { (void)value; }   \
  static inline void TEMPLATE_METHOD(cref_iter_Hashmap, free, K, V)(cref_iter_Hashmap(K, V) * value) { (void)value; } \
  static inline void iter_Hashmap_free(K, V)(cref_iter_Hashmap(K, V) value)                                           \
  {                                                                                                                   \
    TEMPLATE_METHOD(Hashmap, free, K, V)((ref_Hashmap(K, V))&value->map);                                             \
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
  static inline Result(Void, cref(Char)) Hashmap_resize(K, V)(                                                         \
      ref_Hashmap(K, V) self, Size new_capacity)                                                                      \
  {                                                                                                                   \
    ref_Entry(K, V) new_data =                                                                                        \
        MY_C_UTILS_CALLOC(new_capacity, sizeof(Entry(K, V)));                                                         \
    if (!new_data)                                                                                                    \
    {                                                                                                                 \
      return Result_err(Void, cref(Char))("Memory allocation failed");                                                 \
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
    return Result_ok(Void, cref(Char))((Void){});                                                                      \
  }                                                                                                                   \
                                                                                                                      \
  static inline Result(Void, cref(Char)) Hashmap_resize_if_needed(K, V)(                                               \
      ref_Hashmap(K, V) self)                                                                                         \
  {                                                                                                                   \
    Double load_factor = (Double)self->size / self->capacity;                                                         \
    if (load_factor >= DEFAULT_LOAD_FACTOR)                                                                           \
    {                                                                                                                 \
      return Hashmap_resize(K, V)(self, self->capacity * 2);                                                          \
    }                                                                                                                 \
    return Result_ok(Void, cref(Char))((Void){});                                                                      \
  }                                                                                                                   \
                                                                                                                      \
  static inline Result(Void, cref(Char)) Hashmap_add(K, V)(                                                            \
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
      return Result_ok(Void, cref(Char))((Void){});                                                                    \
    }                                                                                                                 \
    Result(Void, cref(Char)) resize_res = Hashmap_resize_if_needed(K, V)(self);                                        \
    if (Result_is_err(Void, cref(Char))(&resize_res))                                                                  \
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
    return Result_ok(Void, cref(Char))((Void){});                                                                      \
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
  static inline Result(ref(V), cref(Char)) Hashmap_get(K, V)(                                                         \
      ref_Hashmap(K, V) self, K key)                                                                                  \
  {                                                                                                                   \
    Size index = Hashmap_find_index(K, V)(self, key);                                                                 \
    if (index < self->capacity)                                                                                       \
    {                                                                                                                 \
      return Result_ok(ref(V), cref(Char))(&self->data[index].value);                                                 \
    }                                                                                                                 \
    return Result_err(ref(V), cref(Char))("no existe elemento con esa Key");                                          \
  }                                                                                                                   \
                                                                                                                      \
  static inline Result(ref(V), cref(Char)) Hashmap_get_mut(K, V)(                                                      \
      ref(Hashmap(K, V)) self, K key)                                                                                  \
  {                                                                                                                   \
    return Hashmap_get(K, V)(self, key);                                                                              \
  }                                                                                                                   \
                                                                                                                      \
  static inline Result(V, cref(Char)) Hashmap_pop(K, V)(                                                               \
      ref(Hashmap(K, V)) self, K key)                                                                                  \
  {                                                                                                                   \
    Size index = Hashmap_find_index(K, V)(self, key);                                                                 \
    if (index < self->capacity)                                                                                       \
    {                                                                                                                 \
      V value = self->data[index].value;                                                                              \
      MY_C_UTILS_CONCAT(K, _free)(&self->data[index].key);                                                            \
      self->size--;                                                                                                   \
      Hashmap_shift_delete(K, V)(self, index);                                                                        \
      return Result_ok(V, cref(Char))(value);                                                                          \
    }                                                                                                                 \
    return Result_err(V, cref(Char))("no existe elemento con esa Key");                                                \
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
          Hashmap(K, V) self)                                                                                          \
  {                                                                                                                   \
    Size first_index = Hashmap_seek_next(K, V)(&self, 0);                                                             \
    return (iter_Hashmap(K, V)){.map = self,                                                                          \
                                .current_index = first_index};                                                        \
  }                                                                                                                   \
                                                                                                                       \
  static inline Result(ref(V), cref(Char)) iter_Hashmap_deref(K, V)(                                                  \
      cref_iter_Hashmap(K, V) self)                                                                                   \
  {                                                                                                                   \
    if (self->current_index >= self->map.capacity ||                                                                  \
        !self->map.data[self->current_index].filled)                                                                  \
    {                                                                                                                 \
      return Result_err(ref(V), cref(Char))("Iterator out of bounds");                                                \
    }                                                                                                                 \
    return Result_ok(ref(V), cref(Char))(                                                                             \
        (ref(V))&self->map.data[self->current_index].value);                                                           \
  }                                                                                                                   \
                                                                                                                       \
  static inline Result(ref(V), cref(Char)) iter_Hashmap_next(K, V)(                                                   \
      ref_iter_Hashmap(K, V) self)                                                                                    \
  {                                                                                                                   \
    if (self->current_index >= self->map.capacity ||                                                                  \
        !self->map.data[self->current_index].filled)                                                                  \
    {                                                                                                                 \
      return Result_err(ref(V), cref(Char))("Iterator out of bounds");                                                \
    }                                                                                                                 \
    Size cur = self->current_index;                                                                                   \
    self->current_index =                                                                                             \
        Hashmap_seek_next(K, V)(&self->map,                                                                           \
                                self->current_index + 1);                                                             \
    return Result_ok(ref(V), cref(Char))(&self->map.data[cur].value);                                                 \
  }                                                                                                                   \
  RESULT_CONFIG(Hashmap(K, V), cref(Char))                                                                             \
  RESULT_CONFIG(ref(Hashmap(K, V)), cref(Char)) \
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
      Result(Void, cref(Char)) add_res = Hashmap_add(K, V)(&dest, key_clone, value_clone);                             \
      if (Result_is_err(Void, cref(Char))(&add_res))                                                                   \
      {                                                                                                               \
        Hashmap_free(K, V)(&dest);                                                                                    \
        perror("Memory allocation failed during hashmap clone");                                                      \
        exit(1);                                                                                                      \
      }                                                                                                               \
    }                                                                                                                 \
    return dest;                                                                                                      \
  }

#endif
