#ifndef MY_C_UTILS_HASHMAP_H
#define MY_C_UTILS_HASHMAP_H

#include "my_c_utils/allocator.h"
#include "my_c_utils/option.h"
#include "my_c_utils/iterator.h"
#include "my_c_utils/result.h"
#include "my_c_utils/primitives.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_LOAD_FACTOR 0.75

#define Hashmap(Key, Value) MY_C_UTILS_CONCAT(Hashmap_, MY_C_UTILS_CONCAT(Key, MY_C_UTILS_CONCAT(_, Value)))
#define ref_Hashmap(Key, Value) MY_C_UTILS_CONCAT(ref_Hashmap_, MY_C_UTILS_CONCAT(Key, MY_C_UTILS_CONCAT(_, Value)))
#define cref_Hashmap(Key, Value) MY_C_UTILS_CONCAT(cref_Hashmap_, MY_C_UTILS_CONCAT(Key, MY_C_UTILS_CONCAT(_, Value)))

#define iter_Hashmap(Key, Value) MY_C_UTILS_CONCAT(iter_Hashmap_, MY_C_UTILS_CONCAT(Key, MY_C_UTILS_CONCAT(_, Value)))
#define ref_iter_Hashmap(Key, Value) MY_C_UTILS_CONCAT(ref_iter_Hashmap_, MY_C_UTILS_CONCAT(Key, MY_C_UTILS_CONCAT(_, Value)))
#define cref_iter_Hashmap(Key, Value) MY_C_UTILS_CONCAT(cref_iter_Hashmap_, MY_C_UTILS_CONCAT(Key, MY_C_UTILS_CONCAT(_, Value)))

#define Hashmap_new(Key, Value)           MY_C_UTILS_CONCAT(Hashmap(Key, Value), _new)
#define Hashmap_add(Key, Value)           MY_C_UTILS_CONCAT(Hashmap(Key, Value), _add)
#define Hashmap_free(Key, Value)          MY_C_UTILS_CONCAT(Hashmap(Key, Value), _free)
#define Hashmap_get(Key, Value)           MY_C_UTILS_CONCAT(Hashmap(Key, Value), _get)
#define Hashmap_get_mut(Key, Value)       MY_C_UTILS_CONCAT(Hashmap(Key, Value), _get_mut)
#define Hashmap_pop(Key, Value)           MY_C_UTILS_CONCAT(Hashmap(Key, Value), _pop)
#define Hashmap_contains(Key, Value)      MY_C_UTILS_CONCAT(Hashmap(Key, Value), _contains)
#define Hashmap_remove(Key, Value)        MY_C_UTILS_CONCAT(Hashmap(Key, Value), _remove)
#define Hashmap_stats(Key, Value)         MY_C_UTILS_CONCAT(Hashmap(Key, Value), _stats)
#define Hashmap_debug(Key, Value)         MY_C_UTILS_CONCAT(Hashmap(Key, Value), _debug)
#define Hashmap_into_iter(Key, Value)     MY_C_UTILS_CONCAT(Hashmap(Key, Value), _into_iter)
#define Hashmap_clone(Key, Value)         MY_C_UTILS_CONCAT(Hashmap(Key, Value), _clone)

#define iter_Hashmap_deref(Key, Value)    MY_C_UTILS_CONCAT(iter_Hashmap(Key, Value), _deref)
#define iter_Hashmap_next(Key, Value)     MY_C_UTILS_CONCAT(iter_Hashmap(Key, Value), _next)

#define Hashmap_insert_entry(Key, Value)  MY_C_UTILS_CONCAT(Hashmap(Key, Value), _insert_entry)
#define Hashmap_find_index(Key, Value)    MY_C_UTILS_CONCAT(Hashmap(Key, Value), _find_index)
#define Hashmap_shift_delete(Key, Value)  MY_C_UTILS_CONCAT(Hashmap(Key, Value), _shift_delete)
#define Hashmap_resize(Key, Value)        MY_C_UTILS_CONCAT(Hashmap(Key, Value), _resize)
#define Hashmap_resize_if_needed(Key, Value) MY_C_UTILS_CONCAT(Hashmap(Key, Value), _resize_if_needed)
#define Hashmap_free_impl(Key, Value)     MY_C_UTILS_CONCAT(Hashmap(Key, Value), _free_impl)
#define Hashmap_seek_next(Key, Value)     MY_C_UTILS_CONCAT(Hashmap(Key, Value), _seek_next)

#define HASH_MAP_CONFIG(Key, Value) HASH_MAP_CONFIG_IMPL(Key, Value)

#define HASH_MAP_CONFIG_IMPL(Key, Value)                                                     \
                                                                                        \
  typedef struct                                                                        \
  {                                                                                     \
    Key key;                                                                            \
    Value value;                                                                        \
    Size hash;                                                                          \
    Size distance;                                                                      \
    Bool filled;                                                                        \
  } Entry_##Key##_##Value;                                                              \
  REF_EXPAND(Entry_##Key##_##Value)                                                     \
                                                                                        \
  typedef struct                                                                        \
  {                                                                                     \
    Size size;                                                                          \
    Size capacity;                                                                      \
    ref_Entry_##Key##_##Value data;                                                     \
    void (*key_free)(ref_##Key);                                                        \
    void (*value_free)(ref_##Value);                                                    \
  } Hashmap_##Key##_##Value;                                                            \
  REF_EXPAND(Hashmap_##Key##_##Value)                                                   \
                                                                                        \
  typedef struct                                                                        \
  {                                                                                     \
    cref_Hashmap_##Key##_##Value map;                                                   \
    Size current_index;                                                                 \
  } iter_Hashmap_##Key##_##Value;                                                       \
  REF_EXPAND(iter_Hashmap_##Key##_##Value)                                              \
  TRIVIAL_FREE(iter_Hashmap_##Key##_##Value, cref_iter_Hashmap_##Key##_##Value value)   \
                                                                                        \
  typedef struct                                                                        \
  {                                                                                     \
    Size size;                                                                          \
    Size capacity;                                                                      \
    Double load_factor;                                                                 \
    Size collisions;                                                                    \
  } HashmapStats;                                                                       \
  REF_EXPAND(HashmapStats)                                                              \
                                                                                        \
  static inline void Hashmap_insert_entry(Key, Value)(                            \
      ref_Entry_##Key##_##Value data, Size capacity,                                    \
      Entry_##Key##_##Value entry)                                                      \
  {                                                                                     \
    Size index = entry.hash % capacity;                                                 \
    while (true)                                                                        \
    {                                                                                   \
      if (!data[index].filled)                                                          \
      {                                                                                 \
        data[index] = entry;                                                            \
        return;                                                                         \
      }                                                                                 \
      if (data[index].distance < entry.distance)                                        \
      {                                                                                 \
        Entry_##Key##_##Value temp = data[index];                                       \
        data[index] = entry;                                                            \
        entry = temp;                                                                   \
      }                                                                                 \
      index = (index + 1) % capacity;                                                   \
      entry.distance++;                                                                 \
    }                                                                                   \
  }                                                                                     \
                                                                                        \
  static inline Size Hashmap_find_index(Key, Value)(                              \
      ref_Hashmap(Key, Value) map, Key key)                                         \
  {                                                                                     \
    Size key_hash = Key##_hash(key);                                                    \
    Size index = key_hash % map->capacity;                                              \
    Size distance = 0;                                                                  \
    while (map->data[index].filled)                                                     \
    {                                                                                   \
      if (map->data[index].hash == key_hash &&                                          \
          Key##_equals(map->data[index].key, key))                                      \
      {                                                                                 \
        return index;                                                                   \
      }                                                                                 \
      if (map->data[index].distance < distance)                                         \
      {                                                                                 \
        break;                                                                          \
      }                                                                                 \
      index = (index + 1) % map->capacity;                                              \
      distance++;                                                                       \
    }                                                                                   \
    return map->capacity;                                                               \
  }                                                                                     \
                                                                                        \
  static inline void Hashmap_shift_delete(Key, Value)(                            \
      ref_Hashmap(Key, Value) map, Size delete_index)                               \
  {                                                                                     \
    Size hole = delete_index;                                                           \
    Size index = (hole + 1) % map->capacity;                                            \
    while (map->data[index].filled && map->data[index].distance > 0)                    \
    {                                                                                   \
      map->data[hole] = map->data[index];                                               \
      map->data[hole].distance--;                                                       \
      hole = index;                                                                     \
      index = (index + 1) % map->capacity;                                              \
    }                                                                                   \
    map->data[hole].filled = false;                                                     \
    map->data[hole].distance = 0;                                                       \
  }                                                                                     \
                                                                                        \
  static inline Hashmap_##Key##_##Value Hashmap_new(Key, Value)(                  \
      Size capacity)                                                                    \
  {                                                                                     \
    if (capacity == 0)                                                                  \
    {                                                                                   \
      perror("Capacity must be greater than 0");                                        \
      exit(1);                                                                          \
    }                                                                                   \
    ref_Entry_##Key##_##Value data =                                                    \
        MY_C_UTILS_CALLOC(capacity, sizeof(Entry_##Key##_##Value));                     \
    if (!data)                                                                          \
    {                                                                                   \
      perror("Memory allocation failed");                                               \
      exit(1);                                                                          \
    }                                                                                   \
    return (Hashmap_##Key##_##Value){                                                   \
        .size = 0,                                                                      \
        .capacity = capacity,                                                           \
        .data = data,                                                                   \
        .key_free = NULL,                                                               \
        .value_free = NULL};                                                            \
  }                                                                                     \
                                                                                        \
  static inline Result(Void, cref_Char) Hashmap_resize(Key, Value)(                                \
      ref_Hashmap(Key, Value) map, Size new_capacity)                               \
  {                                                                                     \
    ref_Entry_##Key##_##Value new_data =                                                \
        MY_C_UTILS_CALLOC(new_capacity, sizeof(Entry_##Key##_##Value));                 \
    if (!new_data)                                                                      \
    {                                                                                   \
      return Result_err(Void, cref_Char)("Memory allocation failed");                                    \
    }                                                                                   \
    for (Size i = 0; i < map->capacity; i++)                                            \
    {                                                                                   \
      if (map->data[i].filled)                                                          \
      {                                                                                 \
        Entry_##Key##_##Value entry = map->data[i];                                     \
        entry.distance = 0;                                                             \
        Hashmap_insert_entry(Key, Value)(new_data, new_capacity,                  \
                                                entry);                                  \
      }                                                                                 \
    }                                                                                   \
    MY_C_UTILS_FREE(map->data);                                                         \
    map->data = new_data;                                                               \
    map->capacity = new_capacity;                                                       \
    return Result_ok(Void, cref_Char)((Void){});                                                                 \
  }                                                                                     \
                                                                                        \
  static inline Result(Void, cref_Char) Hashmap_resize_if_needed(Key, Value)(                      \
      ref_Hashmap(Key, Value) map)                                                  \
  {                                                                                     \
    Double load_factor = (Double)map->size / map->capacity;                             \
    if (load_factor >= DEFAULT_LOAD_FACTOR)                                             \
    {                                                                                   \
      return Hashmap_resize(Key, Value)(map, map->capacity * 2);                  \
    }                                                                                   \
    return Result_ok(Void, cref_Char)((Void){});                                                                 \
  }                                                                                     \
                                                                                        \
  static inline Result(Void, cref_Char) Hashmap_add(Key, Value)(                                   \
      ref_Hashmap(Key, Value) map, Key key, Value value)                            \
  {                                                                                     \
    Size existing_index = Hashmap_find_index(Key, Value)(map, key);               \
    if (existing_index < map->capacity)                                                 \
    {                                                                                   \
      if (map->key_free)                                                                \
      {                                                                                 \
        map->key_free(&map->data[existing_index].key);                                  \
      }                                                                                 \
      else                                                                              \
      {                                                                                 \
        Key##_free(&map->data[existing_index].key);                                     \
      }                                                                                 \
      if (map->value_free)                                                              \
      {                                                                                 \
        map->value_free(&map->data[existing_index].value);                              \
      }                                                                                 \
      else                                                                              \
      {                                                                                 \
        Value##_free(&map->data[existing_index].value);                                 \
      }                                                                                 \
      map->data[existing_index].key = key;                                              \
      map->data[existing_index].value = value;                                          \
      map->data[existing_index].hash = Key##_hash(key);                                 \
      return Result_ok(Void, cref_Char)((Void){});                                                               \
    }                                                                                   \
    Result(Void, cref_Char) resize_res = Hashmap_resize_if_needed(Key, Value)(map);                \
    if (Result_is_err(Void, cref_Char)(&resize_res))                                                     \
    {                                                                                   \
      return resize_res;                                                                \
    }                                                                                   \
    Entry_##Key##_##Value new_entry = {.key = key,                                      \
                                       .value = value,                                  \
                                       .hash = Key##_hash(key),                         \
                                       .distance = 0,                                   \
                                       .filled = true};                                 \
    Hashmap_insert_entry(Key, Value)(map->data, map->capacity,                    \
                                           new_entry);                                  \
    map->size++;                                                                        \
    return Result_ok(Void, cref_Char)((Void){});                                                                 \
  }                                                                                     \
                                                                                        \
  static inline void Hashmap_free_impl(Key, Value)(                               \
      ref_Hashmap(Key, Value) map)                                                  \
  {                                                                                     \
    for (Size i = 0; i < map->capacity; i++)                                            \
    {                                                                                   \
      if (map->data[i].filled)                                                          \
      {                                                                                 \
        if (map->key_free)                                                              \
        {                                                                               \
          map->key_free(&map->data[i].key);                                             \
        }                                                                               \
        else                                                                            \
        {                                                                               \
          Key##_free(&map->data[i].key);                                                \
        }                                                                               \
        if (map->value_free)                                                            \
        {                                                                               \
          map->value_free(&map->data[i].value);                                         \
        }                                                                               \
        else                                                                            \
        {                                                                               \
          Value##_free(&map->data[i].value);                                            \
        }                                                                               \
      }                                                                                 \
    }                                                                                   \
    MY_C_UTILS_FREE(map->data);                                                         \
    map->data = NULL;                                                                   \
    map->size = 0;                                                                      \
    map->capacity = 0;                                                                  \
  }                                                                                     \
                                                                                        \
  static inline void Hashmap_free(Key, Value)(                                    \
      ref_Hashmap(Key, Value) map)                                                  \
  {                                                                                     \
    Hashmap_free_impl(Key, Value)(map);                                           \
  }                                                                                     \
                                                                                        \
  static inline Result(ref_##Value, cref_Char) Hashmap_get(Key, Value)(                       \
      ref_Hashmap(Key, Value) map, Key key)                                         \
  {                                                                                     \
    Size index = Hashmap_find_index(Key, Value)(map, key);                        \
    if (index < map->capacity)                                                          \
    {                                                                                   \
      return Result_ok(ref_##Value, cref_Char)(&map->data[index].value);                          \
    }                                                                                   \
    return Result_err(ref_##Value, cref_Char)("no existe elemento con esa Key");                  \
  }                                                                                     \
                                                                                        \
  static inline Result(ref_##Value, cref_Char) Hashmap_get_mut(Key, Value)(                   \
      ref_Hashmap(Key, Value) map, Key key)                                         \
  {                                                                                     \
    return Hashmap_get(Key, Value)(map, key);                                     \
  }                                                                                     \
                                                                                        \
  static inline Result(Value, cref_Char) Hashmap_pop(Key, Value)(                           \
      ref_Hashmap(Key, Value) map, Key key)                                         \
  {                                                                                     \
    Size index = Hashmap_find_index(Key, Value)(map, key);                        \
    if (index < map->capacity)                                                          \
    {                                                                                   \
      Value value = map->data[index].value;                                             \
      if (map->key_free)                                                                \
      {                                                                                 \
        map->key_free(&map->data[index].key);                                           \
      }                                                                                 \
      else                                                                              \
      {                                                                                 \
        Key##_free(&map->data[index].key);                                              \
      }                                                                                 \
      map->size--;                                                                      \
      Hashmap_shift_delete(Key, Value)(map, index);                               \
      return Result_ok(Value, cref_Char)(value);                                                \
    }                                                                                   \
    return Result_err(Value, cref_Char)("no existe elemento con esa Key");                      \
  }                                                                                     \
                                                                                        \
  static inline Bool Hashmap_contains(Key, Value)(                                \
      ref_Hashmap(Key, Value) map, Key key)                                         \
  {                                                                                     \
    return Hashmap_find_index(Key, Value)(map, key) < map->capacity;              \
  }                                                                                     \
                                                                                        \
  static inline void Hashmap_remove(Key, Value)(                                  \
      ref_Hashmap(Key, Value) map, Key key)                                         \
  {                                                                                     \
    Size index = Hashmap_find_index(Key, Value)(map, key);                        \
    if (index < map->capacity)                                                          \
    {                                                                                   \
      if (map->key_free)                                                                \
      {                                                                                 \
        map->key_free(&map->data[index].key);                                           \
      }                                                                                 \
      else                                                                              \
      {                                                                                 \
        Key##_free(&map->data[index].key);                                              \
      }                                                                                 \
      if (map->value_free)                                                              \
      {                                                                                 \
        map->value_free(&map->data[index].value);                                       \
      }                                                                                 \
      else                                                                              \
      {                                                                                 \
        Value##_free(&map->data[index].value);                                          \
      }                                                                                 \
      map->size--;                                                                      \
      Hashmap_shift_delete(Key, Value)(map, index);                               \
    }                                                                                   \
  }                                                                                     \
                                                                                        \
  static inline HashmapStats Hashmap_stats(Key, Value)(                           \
      cref_Hashmap(Key, Value) map)                                                 \
  {                                                                                     \
    HashmapStats stats = {.size = map->size,                                            \
                          .capacity = map->capacity,                                    \
                          .load_factor = (Double)map->size / map->capacity,             \
                          .collisions = 0};                                             \
    for (Size i = 0; i < map->capacity; i++)                                            \
    {                                                                                   \
      if (map->data[i].filled && map->data[i].distance > 0)                             \
      {                                                                                 \
        stats.collisions++;                                                             \
      }                                                                                 \
    }                                                                                   \
    return stats;                                                                       \
  }                                                                                     \
                                                                                        \
  static inline Size Hashmap_seek_next(Key, Value)(                               \
      cref_Hashmap(Key, Value) map, Size start_index)                               \
  {                                                                                     \
    Size index = start_index;                                                           \
    while (index < map->capacity && !map->data[index].filled)                           \
    {                                                                                   \
      index++;                                                                          \
    }                                                                                   \
    return index;                                                                       \
  }                                                                                     \
                                                                                        \
  static inline void Hashmap_debug(Key, Value)(                                   \
      cref_Hashmap(Key, Value) map)                                                 \
  {                                                                                     \
    HashmapStats stats = Hashmap_stats(Key, Value)(map);                          \
    printf("size: %zu, capacity: %zu, load factor: %f, collisions: %zu\n",              \
           stats.size, stats.capacity, stats.load_factor, stats.collisions);            \
    printf("{\n");                                                                      \
    for (Size i = 0; i < map->capacity; i++)                                            \
    {                                                                                   \
      if (map->data[i].filled)                                                          \
      {                                                                                 \
        printf("  index: %zu, distance: %zu, hash: %zu\n", i,                           \
               map->data[i].distance, map->data[i].hash);                               \
      }                                                                                 \
    }                                                                                   \
    printf("}\n");                                                                      \
  }                                                                                     \
                                                                                        \
  static inline iter_Hashmap(Key, Value)                                            \
  Hashmap_into_iter(Key, Value)(                                                  \
      cref_Hashmap(Key, Value) map)                                                 \
  {                                                                                     \
    Size first_index = Hashmap_seek_next(Key, Value)(map, 0);                     \
    return (iter_Hashmap(Key, Value)){.map = map,                                   \
                                            .current_index = first_index};              \
  }                                                                                     \
                                                                                        \
  static inline Result(ref_##Value, cref_Char) iter_Hashmap_deref(Key, Value)(                \
      cref_iter_Hashmap(Key, Value) iterator)                                       \
  {                                                                                     \
    if (iterator->current_index >= iterator->map->capacity ||                           \
        !iterator->map->data[iterator->current_index].filled)                           \
    {                                                                                   \
      return Result_err(ref_##Value, cref_Char)("Iterator out of bounds");                        \
    }                                                                                   \
    return Result_ok(ref_##Value, cref_Char)(                                                     \
        &iterator->map->data[iterator->current_index].value);                           \
  }                                                                                     \
                                                                                        \
  static inline Result(ref_##Value, cref_Char) iter_Hashmap_next(Key, Value)(                 \
      ref_iter_Hashmap(Key, Value) iterator)                                        \
  {                                                                                     \
    if (iterator->current_index >= iterator->map->capacity ||                           \
        !iterator->map->data[iterator->current_index].filled)                           \
    {                                                                                   \
      return Result_err(ref_##Value, cref_Char)("Iterator out of bounds");                        \
    }                                                                                   \
    Size cur = iterator->current_index;                                                 \
    iterator->current_index =                                                           \
        Hashmap_seek_next(Key, Value)(iterator->map,                              \
                                            iterator->current_index + 1);               \
    return Result_ok(ref_##Value, cref_Char)(&iterator->map->data[cur].value);                    \
  }                                                                                     \
  RESULT_CONFIG(Hashmap_##Key##_##Value, cref_Char)                                                \
  RESULT_CONFIG(ref_Hashmap_##Key##_##Value, cref_Char)                                            \
  static inline Hashmap_##Key##_##Value Hashmap_clone(Key, Value)(                \
      const Hashmap(Key, Value) *src)                                               \
  {                                                                                     \
    if (!src)                                                                           \
    {                                                                                   \
      perror("Cannot clone NULL Hashmap pointer");                                      \
      exit(1);                                                                           \
    }                                                                                   \
    Hashmap(Key, Value) dest = Hashmap_new(Key, Value)(src->capacity);        \
    dest.key_free = src->key_free;                                                       \
    dest.value_free = src->value_free;                                                   \
    for (Size i = 0; i < src->capacity; ++i)                                             \
    {                                                                                   \
      if (!src->data[i].filled)                                                         \
        continue;                                                                       \
      Key key_clone = Key##_clone(&src->data[i].key);                                   \
      Value value_clone = Value##_clone(&src->data[i].value);                           \
      Result(Void, cref_Char) add_res = Hashmap_add(Key, Value)(&dest, key_clone, value_clone);    \
      if (Result_is_err(Void, cref_Char)(&add_res))                                                      \
      {                                                                                 \
        Hashmap_free(Key, Value)(&dest);                                          \
        perror("Memory allocation failed during hashmap clone");                        \
        exit(1);                                                                        \
      }                                                                                 \
    }                                                                                   \
    return dest;                                                                         \
  }
#endif
