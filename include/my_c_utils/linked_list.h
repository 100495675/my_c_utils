#ifndef MY_C_UTILS_LINKED_LIST_H
#define MY_C_UTILS_LINKED_LIST_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#include "my_c_utils/allocator.h"
#include "my_c_utils/iterator.h"
#include "my_c_utils/result.h"
#include "my_c_utils/primitives.h"

#define List(Type) MY_C_UTILS_CONCAT(List_, Type)
#define ref_List(Type) MY_C_UTILS_CONCAT(ref_List_, Type)
#define cref_List(Type) MY_C_UTILS_CONCAT(cref_List_, Type)

#define iter_List(Type) MY_C_UTILS_CONCAT(iter_List_, Type)
#define ref_iter_List(Type) MY_C_UTILS_CONCAT(ref_iter_List_, Type)
#define cref_iter_List(Type) MY_C_UTILS_CONCAT(cref_iter_List_, Type)

#define List_new(Type)          MY_C_UTILS_CONCAT(List_, MY_C_UTILS_CONCAT(Type, _new))
#define List_push_back(Type)    MY_C_UTILS_CONCAT(List_, MY_C_UTILS_CONCAT(Type, _push_back))
#define List_push_front(Type)   MY_C_UTILS_CONCAT(List_, MY_C_UTILS_CONCAT(Type, _push_front))
#define List_pop_front(Type)    MY_C_UTILS_CONCAT(List_, MY_C_UTILS_CONCAT(Type, _pop_front))
#define List_pop_back(Type)     MY_C_UTILS_CONCAT(List_, MY_C_UTILS_CONCAT(Type, _pop_back))
#define List_free(Type)         MY_C_UTILS_CONCAT(List_, MY_C_UTILS_CONCAT(Type, _free))
#define List_debug(Type)        MY_C_UTILS_CONCAT(List_, MY_C_UTILS_CONCAT(Type, _debug))
#define List_size(Type)         MY_C_UTILS_CONCAT(List_, MY_C_UTILS_CONCAT(Type, _size))
#define List_into_iter(Type)    MY_C_UTILS_CONCAT(List_, MY_C_UTILS_CONCAT(Type, _into_iter))
#define List_clone(Type)        MY_C_UTILS_CONCAT(List_, MY_C_UTILS_CONCAT(Type, _clone))

#define iter_List_deref(Type)   MY_C_UTILS_CONCAT(iter_List_, MY_C_UTILS_CONCAT(Type, _deref))
#define iter_List_next(Type)    MY_C_UTILS_CONCAT(iter_List_, MY_C_UTILS_CONCAT(Type, _next))

#define LINKED_LIST_CONFIG(Type) LINKED_LIST_CONFIG_IMPL(Type)

#define LINKED_LIST_CONFIG_IMPL(Type)                                                   \
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
    cref_List_##Type list;                                                         \
    ref_Node_##Type node;                                                          \
  } iter_List_##Type;                                                              \
  REF_EXPAND(iter_List_##Type)                                                     \
  TRIVIAL_FREE(iter_List_##Type, cref_iter_List_##Type value)                      \
                                                                                   \
  static inline List_##Type List_new(Type)()                                    \
  {                                                                                \
    return (List_##Type){.head = NULL, .tail = NULL, .size = 0};                   \
  }                                                                                \
                                                                                   \
  static inline Result(Void, cref_Char) List_push_back(Type)(ref_List(Type) self, Type value)   \
  {                                                                                \
    ref_Node_##Type new_node = MY_C_UTILS_MALLOC(sizeof(Node_##Type));             \
    if (!new_node)                                                                 \
    {                                                                              \
      return Result_err(Void, cref_Char)("Memory allocation failed");                               \
    }                                                                              \
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
    return Result_ok(Void, cref_Char)((Void){});                                                            \
  }                                                                                \
                                                                                   \
  static inline Result(Void, cref_Char) List_push_front(Type)(ref_List(Type) self, Type value)  \
  {                                                                                \
    ref_Node_##Type new_node = MY_C_UTILS_MALLOC(sizeof(Node_##Type));             \
    if (!new_node)                                                                 \
    {                                                                              \
      return Result_err(Void, cref_Char)("Memory allocation failed");                               \
    }                                                                              \
    new_node->data = value;                                                        \
    new_node->next = self->head;                                                   \
    self->head = new_node;                                                         \
    if (self->size == 0)                                                           \
    {                                                                              \
      self->tail = new_node;                                                       \
    }                                                                              \
    ++self->size;                                                                  \
    return Result_ok(Void, cref_Char)((Void){});                                                            \
  }                                                                                \
                                                                                   \
  static inline Result(Type, cref_Char) List_pop_front(Type)(ref_List(Type) self)        \
  {                                                                                \
    if (self->size == 0)                                                           \
    {                                                                              \
      return Result_err(Type, cref_Char)("Pop on empty list");                             \
    }                                                                              \
    ref_Node_##Type temp = self->head;                                             \
    self->head = self->head->next;                                                 \
    if (self->head == NULL)                                                        \
    {                                                                              \
      self->tail = NULL;                                                           \
    }                                                                              \
    Type data = temp->data;                                                        \
    MY_C_UTILS_FREE(temp);                                                         \
    --self->size;                                                                  \
    return Result_ok(Type, cref_Char)(data);                                               \
  }                                                                                \
                                                                                   \
  static inline Result(Type, cref_Char) List_pop_back(Type)(ref_List(Type) self)         \
  {                                                                                \
    if (self->size == 0)                                                           \
    {                                                                              \
      return Result_err(Type, cref_Char)("Pop on empty list");                             \
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
    MY_C_UTILS_FREE(current);                                                      \
    --self->size;                                                                  \
    return Result_ok(Type, cref_Char)(data);                                               \
  }                                                                                \
                                                                                   \
  static inline void List_free(Type)(ref_List(Type) self)                      \
  {                                                                                \
    ref_Node_##Type current = self->head;                                          \
    while (current != NULL)                                                        \
    {                                                                              \
      ref_Node_##Type next = current->next;                                        \
      Type##_free(&current->data);                                                 \
      MY_C_UTILS_FREE(current);                                                    \
      current = next;                                                              \
    }                                                                              \
    self->head = NULL;                                                             \
    self->tail = NULL;                                                             \
    self->size = 0;                                                                \
  }                                                                                \
                                                                                   \
  static inline void List_debug(Type)(cref_List(Type) self)                    \
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
  static inline Size List_size(Type)(cref_List(Type) self)                     \
  {                                                                                \
    return self->size;                                                             \
  }                                                                                \
                                                                                   \
  static inline iter_List(Type) List_into_iter(Type)(                          \
      cref_List(Type) self)                                                       \
  {                                                                                \
    return (iter_List(Type)){                                                     \
        .list = self,                                                              \
        .node = self->head};                                                       \
  }                                                                                \
                                                                                   \
  static inline Result(ref_##Type, cref_Char) iter_List_deref(Type)(                        \
      cref_iter_List(Type) it)                                                    \
  {                                                                                \
    if (it->node == NULL)                                                          \
    {                                                                              \
      return Result_err(ref_##Type, cref_Char)("Iterator out of bounds");                    \
    }                                                                              \
    return Result_ok(ref_##Type, cref_Char)(&it->node->data);                                \
  }                                                                                \
                                                                                   \
  static inline Result(ref_##Type, cref_Char) iter_List_next(Type)(ref_iter_List(Type) it) \
  {                                                                                \
    if (it->node == NULL)                                                          \
    {                                                                              \
      return Result_err(ref_##Type, cref_Char)("Iterator out of bounds");                    \
    }                                                                              \
    ref_Node_##Type cur = it->node;                                                \
    it->node = it->node->next;                                                     \
    return Result_ok(ref_##Type, cref_Char)(&cur->data);                                     \
  }                                                                                \
  RESULT_CONFIG(List_##Type, cref_Char)                                                       \
  RESULT_CONFIG(ref_List_##Type, cref_Char)                                                   \
  static inline List_##Type List_clone(Type)(cref_List(Type) src)              \
  {                                                                                \
    if (!src)                                                                      \
    {                                                                              \
      perror("Cannot clone NULL List pointer");                                    \
      exit(1);                                                                     \
    }                                                                              \
    List(Type) dest = List_new(Type)();                                        \
    ref_Node_##Type node = src->head;                                              \
    while (node != NULL)                                                           \
    {                                                                              \
      Type cloned_value = Type##_clone(&node->data);                               \
      Result(Void, cref_Char) push_res = List_push_back(Type)(&dest, cloned_value);              \
      if (Result_is_err(Void, cref_Char)(&push_res))                                                \
      {                                                                            \
        List_free(Type)(&dest);                                                 \
        perror("Memory allocation failed during list clone");                      \
        exit(1);                                                                   \
      }                                                                            \
      node = node->next;                                                           \
    }                                                                              \
    return dest;                                                                   \
  }
#endif
