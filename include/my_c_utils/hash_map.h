#ifndef MY_C_UTILS_HASHMAP_H
#define MY_C_UTILS_HASHMAP_H

#include "my_c_utils/option.h"
#include "my_c_utils/iterator.h"
#include "my_c_utils/result.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define DEFAULT_LOAD_FACTOR 0.75

#define HASH_MAP_CONFIG(Key, Value)                                          \
                                                                             \
  RESULT_CONFIG(Value)                                                       \
  typedef Value *Value##_ref;                                                \
  static inline void Value##_ref_free(Value##_ref *value)                    \
  {                                                                          \
    (void)value;                                                             \
  }                                                                          \
  RESULT_CONFIG(Value##_ref)                                                 \
  OPTION_CONFIG(Value##_ref)                                                 \
                                                                             \
  typedef struct                                                             \
  {                                                                          \
    Key key;                                                                 \
    Value value;                                                             \
    Size hash;                                                             \
    Size distance;                                                         \
    Bool filled;                                                             \
  } Entry_##Key##_##Value;                                                   \
                                                                             \
  typedef struct                                                             \
  {                                                                          \
    Size size;                                                             \
    Size capacity;                                                         \
    Entry_##Key##_##Value *data;                                             \
    void (*key_free)(Key *);                                                 \
    void (*value_free)(Value *);                                             \
  } Hashmap_##Key##_##Value;                                                 \
                                                                             \
  typedef struct                                                             \
  {                                                                          \
    const Hashmap_##Key##_##Value *map;                                      \
    Size current_index;                                                    \
  } Hashmap_##Key##_##Value##_iter;                                          \
                                                                             \
  typedef struct                                                             \
  {                                                                          \
    Size size;                                                             \
    Size capacity;                                                         \
    Double load_factor;                                                      \
    Size collisions;                                                       \
  } HashmapStats;                                                            \
                                                                             \
  static inline void Hashmap_##Key##_##Value##_insert_entry(                 \
      Entry_##Key##_##Value *data, Size capacity,                          \
      Entry_##Key##_##Value entry)                                           \
  {                                                                          \
    Size index = entry.hash % capacity;                                    \
    while (true)                                                             \
    {                                                                        \
      if (!data[index].filled)                                               \
      {                                                                      \
        data[index] = entry;                                                 \
        return;                                                              \
      }                                                                      \
      if (data[index].distance < entry.distance)                             \
      {                                                                      \
        Entry_##Key##_##Value temp = data[index];                            \
        data[index] = entry;                                                 \
        entry = temp;                                                        \
      }                                                                      \
      index = (index + 1) % capacity;                                        \
      entry.distance++;                                                      \
    }                                                                        \
  }                                                                          \
                                                                             \
  static inline Size Hashmap_##Key##_##Value##_find_index(                 \
      Hashmap_##Key##_##Value *map, Key key)                                 \
  {                                                                          \
    Size key_hash = Key##_hash(key);                                       \
    Size index = key_hash % map->capacity;                                 \
    Size distance = 0;                                                     \
    while (map->data[index].filled)                                          \
    {                                                                        \
      if (map->data[index].hash == key_hash &&                               \
          Key##_equals(map->data[index].key, key))                           \
      {                                                                      \
        return index;                                                        \
      }                                                                      \
      if (map->data[index].distance < distance)                              \
      {                                                                      \
        break;                                                               \
      }                                                                      \
      index = (index + 1) % map->capacity;                                   \
      distance++;                                                            \
    }                                                                        \
    return map->capacity;                                                    \
  }                                                                          \
                                                                             \
  static inline void Hashmap_##Key##_##Value##_shift_delete(                 \
      Hashmap_##Key##_##Value *map, Size delete_index)                     \
  {                                                                          \
    Size hole = delete_index;                                              \
    Size index = (hole + 1) % map->capacity;                               \
    while (map->data[index].filled && map->data[index].distance > 0)         \
    {                                                                        \
      map->data[hole] = map->data[index];                                    \
      map->data[hole].distance--;                                            \
      hole = index;                                                          \
      index = (index + 1) % map->capacity;                                   \
    }                                                                        \
    map->data[hole].filled = false;                                          \
    map->data[hole].distance = 0;                                            \
  }                                                                          \
                                                                             \
  static inline Hashmap_##Key##_##Value Hashmap_##Key##_##Value##_new(       \
      Size capacity)                                                       \
  {                                                                          \
    if (capacity == 0)                                                       \
    {                                                                        \
      perror("Capacity must be greater than 0");                             \
      exit(1);                                                               \
    }                                                                        \
    Entry_##Key##_##Value *data =                                            \
        calloc(capacity, sizeof(Entry_##Key##_##Value));                     \
    if (!data)                                                               \
    {                                                                        \
      perror("Memory allocation failed");                                    \
      exit(1);                                                               \
    }                                                                        \
    return (Hashmap_##Key##_##Value){                                        \
        .size = 0,                                                           \
        .capacity = capacity,                                                \
        .data = data,                                                        \
        .key_free = NULL,                                                    \
        .value_free = NULL};                                                 \
  }                                                                          \
                                                                             \
  static inline void Hashmap_##Key##_##Value##_resize(                       \
      Hashmap_##Key##_##Value *map, Size new_capacity)                     \
  {                                                                          \
    Entry_##Key##_##Value *new_data =                                        \
        calloc(new_capacity, sizeof(Entry_##Key##_##Value));                 \
    if (!new_data)                                                           \
    {                                                                        \
      perror("Memory allocation failed");                                    \
      exit(1);                                                               \
    }                                                                        \
    for (Size i = 0; i < map->capacity; i++)                               \
    {                                                                        \
      if (map->data[i].filled)                                               \
      {                                                                      \
        Hashmap_##Key##_##Value##_insert_entry(new_data, new_capacity,       \
                                               map->data[i]);                \
      }                                                                      \
    }                                                                        \
    free(map->data);                                                         \
    map->data = new_data;                                                    \
    map->capacity = new_capacity;                                            \
  }                                                                          \
                                                                             \
  static inline void Hashmap_##Key##_##Value##_resize_if_needed(             \
      Hashmap_##Key##_##Value *map)                                          \
  {                                                                          \
    Double load_factor = (Double)map->size / map->capacity;                  \
    if (load_factor >= DEFAULT_LOAD_FACTOR)                                  \
    {                                                                        \
      Hashmap_##Key##_##Value##_resize(map, map->capacity * 2);              \
    }                                                                        \
  }                                                                          \
                                                                             \
  static inline void Hashmap_##Key##_##Value##_add(                          \
      Hashmap_##Key##_##Value *map, Key key, Value value)                    \
  {                                                                          \
    Size existing_index = Hashmap_##Key##_##Value##_find_index(map, key);  \
    if (existing_index < map->capacity)                                      \
    {                                                                        \
      if (map->key_free)                                                     \
      {                                                                      \
        map->key_free(&map->data[existing_index].key);                       \
      }                                                                      \
      else                                                                   \
      {                                                                      \
        Key##_free(&map->data[existing_index].key);                          \
      }                                                                      \
      if (map->value_free)                                                   \
      {                                                                      \
        map->value_free(&map->data[existing_index].value);                   \
      }                                                                      \
      else                                                                   \
      {                                                                      \
        Value##_free(&map->data[existing_index].value);                      \
      }                                                                      \
      map->data[existing_index].key = key;                                   \
      map->data[existing_index].value = value;                               \
      map->data[existing_index].hash = Key##_hash(key);                      \
      return;                                                                \
    }                                                                        \
    Hashmap_##Key##_##Value##_resize_if_needed(map);                         \
    Entry_##Key##_##Value new_entry = {.key = key,                           \
                                       .value = value,                       \
                                       .hash = Key##_hash(key),              \
                                       .distance = 0,                        \
                                       .filled = true};                      \
    Hashmap_##Key##_##Value##_insert_entry(map->data, map->capacity,         \
                                           new_entry);                       \
    map->size++;                                                             \
  }                                                                          \
                                                                             \
  static inline void Hashmap_##Key##_##Value##_free_impl(                    \
      Hashmap_##Key##_##Value *map)                                          \
  {                                                                          \
    for (Size i = 0; i < map->capacity; i++)                               \
    {                                                                        \
      if (map->data[i].filled)                                               \
      {                                                                      \
        if (map->key_free)                                                   \
        {                                                                    \
          map->key_free(&map->data[i].key);                                  \
        }                                                                    \
        else                                                                 \
        {                                                                    \
          Key##_free(&map->data[i].key);                                     \
        }                                                                    \
        if (map->value_free)                                                 \
        {                                                                    \
          map->value_free(&map->data[i].value);                              \
        }                                                                    \
        else                                                                 \
        {                                                                    \
          Value##_free(&map->data[i].value);                                 \
        }                                                                    \
      }                                                                      \
    }                                                                        \
    free(map->data);                                                         \
    map->data = NULL;                                                        \
    map->size = 0;                                                           \
    map->capacity = 0;                                                       \
  }                                                                          \
                                                                             \
  static inline void Hashmap_##Key##_##Value##_free(                         \
      Hashmap_##Key##_##Value *map)                                          \
  {                                                                          \
    Hashmap_##Key##_##Value##_free_impl(map);                                \
  }                                                                          \
                                                                             \
  static inline Result_##Value##_ref Hashmap_##Key##_##Value##_get(          \
      Hashmap_##Key##_##Value *map, Key key)                                 \
  {                                                                          \
    Size index = Hashmap_##Key##_##Value##_find_index(map, key);           \
    if (index < map->capacity)                                               \
    {                                                                        \
      return Result_##Value##_ref_ok(&map->data[index].value);               \
    }                                                                        \
    return Result_##Value##_ref_err("no existe elemento con esa Key");       \
  }                                                                          \
                                                                             \
  static inline Result_##Value Hashmap_##Key##_##Value##_pop(                \
      Hashmap_##Key##_##Value *map, Key key)                                 \
  {                                                                          \
    Size index = Hashmap_##Key##_##Value##_find_index(map, key);           \
    if (index < map->capacity)                                               \
    {                                                                        \
      Value value = map->data[index].value;                                  \
      if (map->key_free)                                                     \
      {                                                                      \
        map->key_free(&map->data[index].key);                                \
      }                                                                      \
      else                                                                   \
      {                                                                      \
        Key##_free(&map->data[index].key);                                   \
      }                                                                      \
      map->size--;                                                           \
      Hashmap_##Key##_##Value##_shift_delete(map, index);                    \
      return Result_##Value##_ok(value);                                     \
    }                                                                        \
    return Result_##Value##_err("no existe elemento con esa Key");           \
  }                                                                          \
                                                                             \
  static inline Bool Hashmap_##Key##_##Value##_contains(                     \
      Hashmap_##Key##_##Value *map, Key key)                                 \
  {                                                                          \
    return Hashmap_##Key##_##Value##_find_index(map, key) < map->capacity;   \
  }                                                                          \
                                                                             \
  static inline void Hashmap_##Key##_##Value##_remove(                       \
      Hashmap_##Key##_##Value *map, Key key)                                 \
  {                                                                          \
    Size index = Hashmap_##Key##_##Value##_find_index(map, key);           \
    if (index < map->capacity)                                               \
    {                                                                        \
      if (map->key_free)                                                     \
      {                                                                      \
        map->key_free(&map->data[index].key);                                \
      }                                                                      \
      else                                                                   \
      {                                                                      \
        Key##_free(&map->data[index].key);                                   \
      }                                                                      \
      if (map->value_free)                                                   \
      {                                                                      \
        map->value_free(&map->data[index].value);                            \
      }                                                                      \
      else                                                                   \
      {                                                                      \
        Value##_free(&map->data[index].value);                               \
      }                                                                      \
      map->size--;                                                           \
      Hashmap_##Key##_##Value##_shift_delete(map, index);                    \
    }                                                                        \
  }                                                                          \
                                                                             \
  static inline HashmapStats Hashmap_##Key##_##Value##_stats(                \
      const Hashmap_##Key##_##Value *map)                                    \
  {                                                                          \
    HashmapStats stats = {.size = map->size,                                 \
                          .capacity = map->capacity,                         \
                          .load_factor = (Double)map->size / map->capacity,  \
                          .collisions = 0};                                  \
    for (Size i = 0; i < map->capacity; i++)                               \
    {                                                                        \
      if (map->data[i].filled && map->data[i].distance > 0)                  \
      {                                                                      \
        stats.collisions++;                                                  \
      }                                                                      \
    }                                                                        \
    return stats;                                                            \
  }                                                                          \
                                                                             \
  static inline Size Hashmap_##Key##_##Value##_iter_seek_next(             \
      const Hashmap_##Key##_##Value *map, Size start_index)                \
  {                                                                          \
    Size index = start_index;                                              \
    while (index < map->capacity && !map->data[index].filled)                \
    {                                                                        \
      index++;                                                               \
    }                                                                        \
    return index;                                                            \
  }                                                                          \
                                                                             \
  static inline void Hashmap_##Key##_##Value##_debug(                        \
      const Hashmap_##Key##_##Value *map)                                    \
  {                                                                          \
    HashmapStats stats = Hashmap_##Key##_##Value##_stats(map);               \
    printf("size: %zu, capacity: %zu, load factor: %f, collisions: %zu\n",   \
           stats.size, stats.capacity, stats.load_factor, stats.collisions); \
    printf("{\n");                                                           \
    for (Size i = 0; i < map->capacity; i++)                               \
    {                                                                        \
      if (map->data[i].filled)                                               \
      {                                                                      \
        printf("  index: %zu, distance: %zu, hash: %zu\n", i,                \
               map->data[i].distance, map->data[i].hash);                    \
      }                                                                      \
    }                                                                        \
    printf("}\n");                                                           \
  }                                                                          \
                                                                             \
  static inline Hashmap_##Key##_##Value##_iter                               \
  Hashmap_##Key##_##Value##_into_iter(                                        \
      const Hashmap_##Key##_##Value *map)                                    \
  {                                                                          \
    Size first_index = Hashmap_##Key##_##Value##_iter_seek_next(map, 0);    \
    return (Hashmap_##Key##_##Value##_iter){.map = map,                       \
                                            .current_index = first_index};     \
  }                                                                          \
                                                                             \
  static inline Result_##Value##_ref Hashmap_##Key##_##Value##_iter_deref(   \
      const Hashmap_##Key##_##Value##_iter *iterator)                        \
  {                                                                          \
    if (iterator->current_index >= iterator->map->capacity ||                \
        !iterator->map->data[iterator->current_index].filled)                \
    {                                                                        \
      return Result_##Value##_ref_err("Iterator out of bounds");            \
    }                                                                        \
    return Result_##Value##_ref_ok(                                          \
        &iterator->map->data[iterator->current_index].value);                \
  }                                                                          \
                                                                             \
  static inline Result_##Value##_ref Hashmap_##Key##_##Value##_iter_next(                    \
      Hashmap_##Key##_##Value##_iter *iterator)                              \
  {                                                                          \
    if (iterator->current_index >= iterator->map->capacity ||                \
        !iterator->map->data[iterator->current_index].filled)                \
    {                                                                        \
      return Result_##Value##_ref_err("Iterator out of bounds");          \
    }                                                                        \
    Size cur = iterator->current_index;                                     \
    iterator->current_index =                                               \
        Hashmap_##Key##_##Value##_iter_seek_next(iterator->map,               \
                                                 iterator->current_index + 1); \
    return Result_##Value##_ref_ok(&iterator->map->data[cur].value);          \
  }

#endif
