#ifndef MY_C_UTILS_HASHMAP_H
#define MY_C_UTILS_HASHMAP_H

#include "my_c_utils/option.h"
#include "my_c_utils/iterator.h"
#include "my_c_utils/result.h"
#include "my_c_utils/primitives.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_LOAD_FACTOR 0.75

#define HASH_MAP_CONFIG(Key, Value)                                                     \
  RESULT_CONFIG(ref_##Value)                                                            \
  OPTION_CONFIG(ref_##Value)                                                            \
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
    const ref_Hashmap_##Key##_##Value map;                                              \
    Size current_index;                                                                 \
  } iter_Hashmap_##Key##_##Value;                                                       \
  REF_EXPAND(iter_Hashmap_##Key##_##Value)                                              \
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
  static inline void Hashmap_##Key##_##Value##_insert_entry(                            \
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
  static inline Size Hashmap_##Key##_##Value##_find_index(                              \
      ref_Hashmap_##Key##_##Value map, Key key)                                         \
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
  static inline void Hashmap_##Key##_##Value##_shift_delete(                            \
      ref_Hashmap_##Key##_##Value map, Size delete_index)                               \
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
  static inline Hashmap_##Key##_##Value Hashmap_##Key##_##Value##_new(                  \
      Size capacity)                                                                    \
  {                                                                                     \
    if (capacity == 0)                                                                  \
    {                                                                                   \
      perror("Capacity must be greater than 0");                                        \
      exit(1);                                                                          \
    }                                                                                   \
    ref_Entry_##Key##_##Value data =                                                    \
        calloc(capacity, sizeof(Entry_##Key##_##Value));                                \
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
  static inline void Hashmap_##Key##_##Value##_resize(                                  \
      ref_Hashmap_##Key##_##Value map, Size new_capacity)                               \
  {                                                                                     \
    ref_Entry_##Key##_##Value new_data =                                                \
        calloc(new_capacity, sizeof(Entry_##Key##_##Value));                            \
    if (!new_data)                                                                      \
    {                                                                                   \
      perror("Memory allocation failed");                                               \
      exit(1);                                                                          \
    }                                                                                   \
    for (Size i = 0; i < map->capacity; i++)                                            \
    {                                                                                   \
      if (map->data[i].filled)                                                          \
      {                                                                                 \
        Hashmap_##Key##_##Value##_insert_entry(new_data, new_capacity,                  \
                                               map->data[i]);                           \
      }                                                                                 \
    }                                                                                   \
    free(map->data);                                                                    \
    map->data = new_data;                                                               \
    map->capacity = new_capacity;                                                       \
  }                                                                                     \
                                                                                        \
  static inline void Hashmap_##Key##_##Value##_resize_if_needed(                        \
      ref_Hashmap_##Key##_##Value map)                                                  \
  {                                                                                     \
    Double load_factor = (Double)map->size / map->capacity;                             \
    if (load_factor >= DEFAULT_LOAD_FACTOR)                                             \
    {                                                                                   \
      Hashmap_##Key##_##Value##_resize(map, map->capacity * 2);                         \
    }                                                                                   \
  }                                                                                     \
                                                                                        \
  static inline void Hashmap_##Key##_##Value##_add(                                     \
      ref_Hashmap_##Key##_##Value map, Key key, Value value)                            \
  {                                                                                     \
    Size existing_index = Hashmap_##Key##_##Value##_find_index(map, key);               \
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
      return;                                                                           \
    }                                                                                   \
    Hashmap_##Key##_##Value##_resize_if_needed(map);                                    \
    Entry_##Key##_##Value new_entry = {.key = key,                                      \
                                       .value = value,                                  \
                                       .hash = Key##_hash(key),                         \
                                       .distance = 0,                                   \
                                       .filled = true};                                 \
    Hashmap_##Key##_##Value##_insert_entry(map->data, map->capacity,                    \
                                           new_entry);                                  \
    map->size++;                                                                        \
  }                                                                                     \
                                                                                        \
  static inline void Hashmap_##Key##_##Value##_free_impl(                               \
      ref_Hashmap_##Key##_##Value map)                                                  \
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
    free(map->data);                                                                    \
    map->data = NULL;                                                                   \
    map->size = 0;                                                                      \
    map->capacity = 0;                                                                  \
  }                                                                                     \
                                                                                        \
  static inline void Hashmap_##Key##_##Value##_free(                                    \
      ref_Hashmap_##Key##_##Value map)                                                  \
  {                                                                                     \
    Hashmap_##Key##_##Value##_free_impl(map);                                           \
  }                                                                                     \
                                                                                        \
  static inline Result_ref_##Value Hashmap_##Key##_##Value##_get(                       \
      ref_Hashmap_##Key##_##Value map, Key key)                                         \
  {                                                                                     \
    Size index = Hashmap_##Key##_##Value##_find_index(map, key);                        \
    if (index < map->capacity)                                                          \
    {                                                                                   \
      return Result_ref_##Value##_ok(&map->data[index].value);                          \
    }                                                                                   \
    return Result_ref_##Value##_err("no existe elemento con esa Key");                  \
  }                                                                                     \
                                                                                        \
  static inline Result_##Value Hashmap_##Key##_##Value##_pop(                           \
      ref_Hashmap_##Key##_##Value map, Key key)                                         \
  {                                                                                     \
    Size index = Hashmap_##Key##_##Value##_find_index(map, key);                        \
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
      Hashmap_##Key##_##Value##_shift_delete(map, index);                               \
      return Result_##Value##_ok(value);                                                \
    }                                                                                   \
    return Result_##Value##_err("no existe elemento con esa Key");                      \
  }                                                                                     \
                                                                                        \
  static inline Bool Hashmap_##Key##_##Value##_contains(                                \
      ref_Hashmap_##Key##_##Value map, Key key)                                         \
  {                                                                                     \
    return Hashmap_##Key##_##Value##_find_index(map, key) < map->capacity;              \
  }                                                                                     \
                                                                                        \
  static inline void Hashmap_##Key##_##Value##_remove(                                  \
      ref_Hashmap_##Key##_##Value map, Key key)                                         \
  {                                                                                     \
    Size index = Hashmap_##Key##_##Value##_find_index(map, key);                        \
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
      Hashmap_##Key##_##Value##_shift_delete(map, index);                               \
    }                                                                                   \
  }                                                                                     \
                                                                                        \
  static inline HashmapStats Hashmap_##Key##_##Value##_stats(                           \
      const ref_Hashmap_##Key##_##Value map)                                            \
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
  static inline Size Hashmap_##Key##_##Value##_seek_next(                               \
      const ref_Hashmap_##Key##_##Value map, Size start_index)                          \
  {                                                                                     \
    Size index = start_index;                                                           \
    while (index < map->capacity && !map->data[index].filled)                           \
    {                                                                                   \
      index++;                                                                          \
    }                                                                                   \
    return index;                                                                       \
  }                                                                                     \
                                                                                        \
  static inline void Hashmap_##Key##_##Value##_debug(                                   \
      const ref_Hashmap_##Key##_##Value map)                                            \
  {                                                                                     \
    HashmapStats stats = Hashmap_##Key##_##Value##_stats(map);                          \
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
  static inline iter_Hashmap_##Key##_##Value                                            \
  Hashmap_##Key##_##Value##_into_iter(                                                  \
      const ref_Hashmap_##Key##_##Value map)                                            \
  {                                                                                     \
    Size first_index = Hashmap_##Key##_##Value##_seek_next(map, 0);                     \
    return (iter_Hashmap_##Key##_##Value){.map = map,                                   \
                                            .current_index = first_index};              \
  }                                                                                     \
                                                                                        \
  static inline Result_ref_##Value iter_Hashmap_##Key##_##Value##_deref(                \
      const ref_iter_Hashmap_##Key##_##Value iterator)                                  \
  {                                                                                     \
    if (iterator->current_index >= iterator->map->capacity ||                           \
        !iterator->map->data[iterator->current_index].filled)                           \
    {                                                                                   \
      return Result_ref_##Value##_err("Iterator out of bounds");                        \
    }                                                                                   \
    return Result_ref_##Value##_ok(                                                     \
        &iterator->map->data[iterator->current_index].value);                           \
  }                                                                                     \
                                                                                        \
  static inline Result_ref_##Value iter_Hashmap_##Key##_##Value##_next(                 \
      ref_iter_Hashmap_##Key##_##Value iterator)                                        \
  {                                                                                     \
    if (iterator->current_index >= iterator->map->capacity ||                           \
        !iterator->map->data[iterator->current_index].filled)                           \
    {                                                                                   \
      return Result_ref_##Value##_err("Iterator out of bounds");                        \
    }                                                                                   \
    Size cur = iterator->current_index;                                                 \
    iterator->current_index =                                                           \
        Hashmap_##Key##_##Value##_seek_next(iterator->map,                              \
                                            iterator->current_index + 1);               \
    return Result_ref_##Value##_ok(&iterator->map->data[cur].value);                    \
  }                                                                                     \
                                                                                        \
  RESULT_CONFIG(Hashmap_##Key##_##Value)                                                \
    static inline Hashmap_##Key##_##Value Hashmap_##Key##_##Value##_clone(               \
      const Hashmap_##Key##_##Value *src)                                                \
  {                                                                                     \
    if (!src)                                                                            \
    {                                                                                   \
      perror("Cannot clone NULL Hashmap pointer");                                     \
      exit(1);                                                                           \
    }                                                                                   \
    Hashmap_##Key##_##Value dest = Hashmap_##Key##_##Value##_new(src->capacity);         \
    dest.key_free = src->key_free;                                                       \
    dest.value_free = src->value_free;                                                   \
    for (Size i = 0; i < src->capacity; ++i)                                             \
    {                                                                                   \
      if (!src->data[i].filled)                                                         \
        continue;                                                                       \
      Key key_clone = Key##_clone(&src->data[i].key);                                   \
      Value value_clone = Value##_clone(&src->data[i].value);                           \
      Hashmap_##Key##_##Value##_add(&dest, key_clone, value_clone);                     \
    }                                                                                   \
    return dest;                                                                         \
  }

#endif
