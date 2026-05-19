#ifndef MY_C_UTILS_LINKED_LIST_H
#define MY_C_UTILS_LINKED_LIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_c_utils/iterator.h"
#include "my_c_utils/result.h"
#include "my_c_utils/primitives.h"

#define LINKED_LIST_CONFIG(Type)                                                   \
  typedef struct Node_##Type                                                       \
  {                                                                                \
    Type data;                                                                     \
    struct Node_##Type *next; /* excepción de los ref_  */                         \
  } Node_##Type;                                                                   \
  REF_EXPAND(Node_##Type)                                                          \
                                                                                   \
  typedef struct                                                                   \
  {                                                                                \
    ref_Node_##Type head;                                                          \
    ref_Node_##Type tail;                                                          \
    Size size;                                                                     \
  } List_##Type;                                                                   \
  REF_EXPAND(List_##Type)                                                          \
                                                                                   \
  typedef struct                                                                   \
  {                                                                                \
    const ref_List_##Type list;                                                    \
    ref_Node_##Type node;                                                          \
  } iter_List_##Type;                                                              \
  REF_EXPAND(iter_List_##Type)                                                     \
                                                                                   \
  RESULT_CONFIG(ref_##Type)                                                        \
                                                                                   \
  static inline List_##Type List_##Type##_new()                                    \
  {                                                                                \
    return (List_##Type){.head = NULL, .tail = NULL, .size = 0};                   \
  }                                                                                \
                                                                                   \
  static inline void List_##Type##_push_back(ref_List_##Type self, Type value)     \
  {                                                                                \
    ref_Node_##Type new_node = malloc(sizeof(Node_##Type));                        \
    new_node->data = value;                                                        \
    new_node->next = NULL;                                                         \
    if (self->size == 0)                                                           \
    {                                                                              \
      self->head = new_node;                                                       \
    }                                                                              \
    else                                                                           \
    {                                                                              \
      self->tail->next = new_node;                                                 \
    }                                                                              \
    self->tail = new_node;                                                         \
    ++self->size;                                                                  \
  }                                                                                \
                                                                                   \
  static inline Result_##Type List_##Type##_pop_back(ref_List_##Type self)         \
  {                                                                                \
    if (self->size == 0)                                                           \
    {                                                                              \
      return Result_##Type##_err("Pop on empty list");                             \
    }                                                                              \
    ref_Node_##Type current = self->head;                                          \
    ref_Node_##Type previous = NULL;                                               \
    while (current->next != NULL)                                                  \
    {                                                                              \
      previous = current;                                                          \
      current = current->next;                                                     \
    }                                                                              \
    if (previous == NULL)                                                          \
    {                                                                              \
      self->head = NULL;                                                           \
      self->tail = NULL;                                                           \
    }                                                                              \
    else                                                                           \
    {                                                                              \
      previous->next = NULL;                                                       \
      self->tail = previous;                                                       \
    }                                                                              \
    Type data = current->data;                                                     \
    free(current);                                                                 \
    --self->size;                                                                  \
    return Result_##Type##_ok(data);                                               \
  }                                                                                \
                                                                                   \
  static inline void List_##Type##_free(ref_List_##Type self)                      \
  {                                                                                \
    ref_Node_##Type current = self->head;                                          \
    while (current != NULL)                                                        \
    {                                                                              \
      ref_Node_##Type next = current->next;                                        \
      Type##_free(&current->data);                                                 \
      free(current);                                                               \
      current = next;                                                              \
    }                                                                              \
    self->head = NULL;                                                             \
    self->tail = NULL;                                                             \
    self->size = 0;                                                                \
  }                                                                                \
                                                                                   \
  static inline void List_##Type##_debug(const ref_List_##Type self)               \
  {                                                                                \
    printf("Size: %zu\n", self->size);                                             \
    ref_Node_##Type current = self->head;                                          \
    Size index = 0;                                                                \
    while (current != NULL)                                                        \
    {                                                                              \
      printf("  [%zu]\n", index);                                                  \
      current = current->next;                                                     \
      ++index;                                                                     \
    }                                                                              \
  }                                                                                \
                                                                                   \
  static inline Size List_##Type##_size(const ref_List_##Type self)                \
  {                                                                                \
    return self->size;                                                             \
  }                                                                                \
                                                                                   \
  static inline iter_List_##Type List_##Type##_into_iter(                          \
      const ref_List_##Type self)                                                  \
  {                                                                                \
    return (iter_List_##Type){                                                     \
        .list = self,                                                              \
        .node = self->head};                                                       \
  }                                                                                \
                                                                                   \
  static inline Result_ref_##Type iter_List_##Type##_deref(                        \
      const ref_iter_List_##Type it)                                               \
  {                                                                                \
    if (it->node == NULL)                                                          \
    {                                                                              \
      return Result_ref_##Type##_err("Iterator out of bounds");                    \
    }                                                                              \
    return Result_ref_##Type##_ok(&it->node->data);                                \
  }                                                                                \
                                                                                   \
  static inline Result_ref_##Type iter_List_##Type##_next(ref_iter_List_##Type it) \
  {                                                                                \
    if (it->node == NULL)                                                          \
    {                                                                              \
      return Result_ref_##Type##_err("Iterator out of bounds");                    \
    }                                                                              \
    ref_Node_##Type cur = it->node;                                                \
    it->node = it->node->next;                                                     \
    return Result_ref_##Type##_ok(&cur->data);                                     \
  }                                                                                \
                                                                                   \
  RESULT_CONFIG(List_##Type)                                                       \
  static inline List_##Type List_##Type##_clone(const List_##Type *src) \
  {                                                                                \
    if (!src)                                                                      \
    {                                                                              \
      perror("Cannot clone NULL List pointer");                                   \
      exit(1);                                                                     \
    }                                                                              \
    List_##Type dest = List_##Type##_new();                                         \
    ref_Node_##Type node = src->head;                                              \
    while (node != NULL)                                                           \
    {                                                                              \
      Type cloned_value = Type##_clone(&node->data);                                \
      List_##Type##_push_back(&dest, cloned_value);                                \
      node = node->next;                                                           \
    }                                                                              \
    return dest;                                                                   \
  }

#endif
