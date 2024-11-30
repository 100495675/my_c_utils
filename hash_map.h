#ifndef HASHMAP_H
#define HASHMAP_H

#include "option.h"
#include "result.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_LOAD_FACTOR 0.75

#define HASH_MAP_CONFIG_FULL(Key, Value, hash, equals_function, free_key,      \
                             free_value)                                       \
  static inline void noop_key(Key key) {};                                     \
  static inline void noop_value(Value value) {};                               \
  static inline bool default_equals(Key a, Key b) { return a == b; }           \
  void (*free_key_function)(Key) = free_key ? free_key : noop_key;             \
  void (*free_value_function)(Value) = free_value ? free_value : noop_value;   \
  bool (*equals)(Key, Key) =                                                   \
      equals_function ? equals_function : default_equals;                      \
                                                                               \
  RESULT_CONFIG(Value)                                                         \
  typedef Value *Value##_ref;                                                  \
  RESULT_CONFIG(Value##_ref)                                                   \
  OPTION_CONFIG(Value##_ref)                                                   \
                                                                               \
  typedef struct {                                                             \
    Key key;                                                                   \
    Value value;                                                               \
    size_t hash;                                                               \
    size_t distance;                                                           \
    bool filled;                                                               \
  } Entry_##Key##_##Value;                                                     \
                                                                               \
  typedef struct {                                                             \
    size_t size;                                                               \
    size_t capacity;                                                           \
    Entry_##Key##_##Value *data;                                               \
  } Hashmap_##Key##_##Value;                                                   \
                                                                               \
  typedef struct {                                                             \
    Hashmap_##Key##_##Value *map;                                              \
    size_t current_index;                                                      \
  } HashmapIterator_##Key##_##Value;                                           \
                                                                               \
  typedef struct {                                                             \
    size_t size;                                                               \
    size_t capacity;                                                           \
    double load_factor;                                                        \
    size_t collisions;                                                         \
  } HashmapStats;                                                              \
                                                                               \
  static inline Hashmap_##Key##_##Value hashmap_##Key##_##Value##_new(         \
      size_t capacity) {                                                       \
    if (capacity == 0) {                                                       \
      perror("Capacity must be greater than 0");                               \
      exit(1);                                                                 \
    }                                                                          \
    return (Hashmap_##Key##_##Value){                                          \
        .size = 0,                                                             \
        .capacity = capacity,                                                  \
        .data = calloc(capacity, sizeof(Entry_##Key##_##Value))};              \
  }                                                                            \
                                                                               \
  static inline void hashmap_##Key##_##Value##_resize(                         \
      Hashmap_##Key##_##Value *map, size_t new_capacity) {                     \
    Entry_##Key##_##Value *new_data =                                          \
        calloc(new_capacity, sizeof(Entry_##Key##_##Value));                   \
    for (size_t i = 0; i < map->capacity; i++) {                               \
      if (map->data[i].filled) {                                               \
        size_t index = map->data[i].hash % new_capacity;                       \
        size_t distance = 0;                                                   \
        while (new_data[index].filled) {                                       \
          if (new_data[index].distance < distance) {                           \
            Entry_##Key##_##Value temp = new_data[index];                      \
            new_data[index] = map->data[i];                                    \
            map->data[i] = temp;                                               \
            distance = new_data[index].distance;                               \
          }                                                                    \
          index = (index + 1) % new_capacity;                                  \
          distance++;                                                          \
        }                                                                      \
        new_data[index] = map->data[i];                                        \
        new_data[index].distance = distance;                                   \
      }                                                                        \
    }                                                                          \
    free(map->data);                                                           \
    map->data = new_data;                                                      \
    map->capacity = new_capacity;                                              \
  }                                                                            \
                                                                               \
  static inline void hashmap_##Key##_##Value##_resize_if_needed(               \
      Hashmap_##Key##_##Value *map) {                                          \
    double load_factor = (double)map->size / map->capacity;                    \
    if (load_factor >= DEFAULT_LOAD_FACTOR) {                                  \
      hashmap_##Key##_##Value##_resize(map, map->capacity * 2);                \
    }                                                                          \
  }                                                                            \
                                                                               \
  static inline void hashmap_##Key##_##Value##_add(                            \
      Hashmap_##Key##_##Value *map, Key key, Value value) {                    \
    hashmap_##Key##_##Value##_resize_if_needed(map);                           \
    size_t key_hash = hash(key);                                               \
    size_t index = key_hash % map->capacity;                                   \
    size_t distance = 0;                                                       \
    Entry_##Key##_##Value new_entry = {.key = key,                             \
                                       .value = value,                         \
                                       .hash = key_hash,                       \
                                       .distance = distance,                   \
                                       .filled = true};                        \
    while (map->data[index].filled) {                                          \
      if (map->data[index].hash == key_hash &&                                 \
          equals(map->data[index].key, key)) {                                 \
        free_key_function(map->data[index].key);                               \
        free_value_function(map->data[index].value);                           \
        map->data[index] = new_entry;                                          \
        return;                                                                \
      }                                                                        \
      if (map->data[index].distance < distance) {                              \
        Entry_##Key##_##Value temp = map->data[index];                         \
        map->data[index] = new_entry;                                          \
        new_entry = temp;                                                      \
      }                                                                        \
      index = (index + 1) % map->capacity;                                     \
      distance++;                                                              \
      new_entry.distance = distance;                                           \
    }                                                                          \
    map->data[index] = new_entry;                                              \
    map->size++;                                                               \
  }                                                                            \
                                                                               \
  static inline void hashmap_##Key##_##Value##_free(                           \
      Hashmap_##Key##_##Value *map) {                                          \
    for (size_t i = 0; i < map->capacity; i++) {                               \
      if (map->data[i].filled) {                                               \
        free_key_function(map->data[i].key);                                   \
        free_value_function(map->data[i].value);                               \
      }                                                                        \
    }                                                                          \
    free(map->data);                                                           \
  }                                                                            \
                                                                               \
  /* mut ref */                                                                \
  static inline Result_##Value##_ref hashmap_##Key##_##Value##_get(            \
      Hashmap_##Key##_##Value *map, Key key) {                                 \
    size_t key_hash = hash(key);                                               \
    size_t index = key_hash % map->capacity;                                   \
    size_t distance = 0;                                                       \
    while (map->data[index].filled) {                                          \
      if (map->data[index].hash == key_hash &&                                 \
          equals(map->data[index].key, key)) {                                 \
        return result_##Value##_ref_ok(&map->data[index].value);               \
      }                                                                        \
      if (map->data[index].distance < distance) {                              \
        break;                                                                 \
      }                                                                        \
      index = (index + 1) % map->capacity;                                     \
      distance++;                                                              \
    }                                                                          \
    return result_##Value##_ref_err("no existe elemento con esa Key");         \
  }                                                                            \
                                                                               \
  /* transfer ownership */                                                     \
  static inline Result_##Value hashmap_##Key##_##Value##_pop(                  \
      Hashmap_##Key##_##Value *map, Key key) {                                 \
    size_t key_hash = hash(key);                                               \
    size_t index = key_hash % map->capacity;                                   \
    size_t distance = 0;                                                       \
    while (map->data[index].filled) {                                          \
      if (map->data[index].hash == key_hash &&                                 \
          equals(map->data[index].key, key)) {                                 \
        Value value = map->data[index].value;                                  \
        map->data[index].filled = false;                                       \
        map->size--;                                                           \
        free_key_function(map->data[index].key);                               \
        return result_##Value##_ok(value);                                     \
      }                                                                        \
      if (map->data[index].distance < distance) {                              \
        break;                                                                 \
      }                                                                        \
      index = (index + 1) % map->capacity;                                     \
      distance++;                                                              \
    }                                                                          \
    return result_##Value##_err("no existe elemento con esa Key");             \
  }                                                                            \
                                                                               \
  static inline bool hashmap_##Key##_##Value##_contains(                       \
      Hashmap_##Key##_##Value *map, Key key) {                                 \
    size_t key_hash = hash(key);                                               \
    size_t index = key_hash % map->capacity;                                   \
    size_t distance = 0;                                                       \
    while (map->data[index].filled) {                                          \
      if (map->data[index].hash == key_hash &&                                 \
          equals(map->data[index].key, key)) {                                 \
        return true;                                                           \
      }                                                                        \
      if (map->data[index].distance < distance) {                              \
        break;                                                                 \
      }                                                                        \
      index = (index + 1) % map->capacity;                                     \
      distance++;                                                              \
    }                                                                          \
    return false;                                                              \
  }                                                                            \
                                                                               \
  static inline void hashmap_##Key##_##Value##_remove(                         \
      Hashmap_##Key##_##Value *map, Key key) {                                 \
    size_t key_hash = hash(key);                                               \
    size_t index = key_hash % map->capacity;                                   \
    size_t distance = 0;                                                       \
    while (map->data[index].filled) {                                          \
      if (map->data[index].hash == key_hash &&                                 \
          equals(map->data[index].key, key)) {                                 \
        map->data[index].filled = false;                                       \
        map->size--;                                                           \
        free_key_function(map->data[index].key);                               \
        free_value_function(map->data[index].value);                           \
        return;                                                                \
      }                                                                        \
      if (map->data[index].distance < distance) {                              \
        break;                                                                 \
      }                                                                        \
      index = (index + 1) % map->capacity;                                     \
      distance++;                                                              \
    }                                                                          \
  }                                                                            \
                                                                               \
  static inline void hashmap_##Key##_##Value##_clear(                          \
      Hashmap_##Key##_##Value *map) {                                          \
    for (size_t i = 0; i < map->capacity; i++) {                               \
      if (map->data[i].filled) {                                               \
        free_key_function(map->data[i].key);                                   \
        free_value_function(map->data[i].value);                               \
        map->data[i].filled = false;                                           \
      }                                                                        \
    }                                                                          \
    map->size = 0;                                                             \
  }                                                                            \
                                                                               \
  static inline HashmapStats hashmap_##Key##_##Value##_stats(                  \
      Hashmap_##Key##_##Value *map) {                                          \
    HashmapStats stats = {.size = map->size,                                   \
                          .capacity = map->capacity,                           \
                          .load_factor = (double)map->size / map->capacity,    \
                          .collisions = 0};                                    \
    for (size_t i = 0; i < map->capacity; i++) {                               \
      if (map->data[i].filled) {                                               \
        size_t index = map->data[i].hash % map->capacity;                      \
        if (index != i) {                                                      \
          stats.collisions++;                                                  \
        }                                                                      \
      }                                                                        \
    }                                                                          \
    return stats;                                                              \
  }                                                                            \
                                                                               \
  static inline void hashmap_##Key##_##Value##_debug(                          \
      Hashmap_##Key##_##Value *map) {                                          \
    HashmapStats stats = hashmap_##Key##_##Value##_stats(map);                 \
    printf("size: %zu, capacity: %zu, load factor: %f, collisions: %zu\n",     \
           stats.size, stats.capacity, stats.load_factor, stats.collisions);   \
    printf("{\n");                                                             \
    for (size_t i = 0; i < map->capacity; i++) {                               \
      if (map->data[i].filled) {                                               \
        printf("  index: %zu, key: %d, value: %d, distance: %zu, hash: %zu\n", \
               i, map->data[i].key, map->data[i].value, map->data[i].distance, \
               map->data[i].hash);                                             \
      }                                                                        \
    }                                                                          \
    printf("}\n");                                                             \
  }                                                                            \
                                                                               \
  static inline HashmapIterator_##Key##_##Value                                \
      hashmap_iterator_##Key##_##Value##_new(Hashmap_##Key##_##Value *map) {   \
    return (HashmapIterator_##Key##_##Value){.map = map, .current_index = 0};  \
  }                                                                            \
                                                                               \
  static inline bool hashmap_iterator_##Key##_##Value##_has_next(              \
      HashmapIterator_##Key##_##Value *iterator) {                             \
    while (iterator->current_index < iterator->map->capacity) {                \
      if (iterator->map->data[iterator->current_index].filled) {               \
        return true;                                                           \
      }                                                                        \
      iterator->current_index++;                                               \
    }                                                                          \
    return false;                                                              \
  }                                                                            \
                                                                               \
  static inline Option_##Value##_ref hashmap_iterator_##Key##_##Value##_next(  \
      HashmapIterator_##Key##_##Value *iterator) {                             \
    while (iterator->current_index < iterator->map->capacity) {                \
      if (iterator->map->data[iterator->current_index].filled) {               \
        return option_##Value##_ref_some(                                      \
            &iterator->map->data[iterator->current_index++].value);            \
      }                                                                        \
      iterator->current_index++;                                               \
    }                                                                          \
    return option_##Value##_ref_none();                                        \
  }

#define COUNT_ARGS(_1, _2, _3, _4, _5, _6, COUNT, ...) COUNT
#define EXPAND_ARGS(args) COUNT_ARGS args
#define COUNT(...) EXPAND_ARGS((__VA_ARGS__, 6, 5, 4, 3, 2, 1))

#define HASH_MAP_CONFIG_3(a, b, c)                                             \
  HASH_MAP_CONFIG_FULL(a, b, c, NULL, NULL, NULL)
#define HASH_MAP_CONFIG_4(a, b, c, d)                                          \
  HASH_MAP_CONFIG_FULL(a, b, c, d, NULL, NULL)
#define HASH_MAP_CONFIG_6(a, b, c, d, e, f)                                    \
  HASH_MAP_CONFIG_FULL(a, b, c, d, e, f)

#define HASH_MAP_CHOOSER2(count) HASH_MAP_CONFIG_##count
#define HASH_MAP_CHOOSER(count) HASH_MAP_CHOOSER2(count)
#define HASH_MAP_CONFIG(...) HASH_MAP_CHOOSER(COUNT(__VA_ARGS__))(__VA_ARGS__)

#endif