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
#include "my_c_utils/template.h"

// 1. User-Facing Macros (Prefix-free, template-compatible)
#define List(...) TEMPLATE_TYPE(List, __VA_ARGS__)
#define ref_List(...) TEMPLATE_TYPE(ref_List, __VA_ARGS__)
#define cref_List(...) TEMPLATE_TYPE(cref_List, __VA_ARGS__)

#define iter_List(...) TEMPLATE_TYPE(iter_List, __VA_ARGS__)
#define ref_iter_List(...) TEMPLATE_TYPE(ref_iter_List, __VA_ARGS__)
#define cref_iter_List(...) TEMPLATE_TYPE(cref_iter_List, __VA_ARGS__)

#define List_new(...)          TEMPLATE_METHOD(List, new, __VA_ARGS__)
#define List_push_back(...)    TEMPLATE_METHOD(List, push_back, __VA_ARGS__)
#define List_push_front(...)   TEMPLATE_METHOD(List, push_front, __VA_ARGS__)
#define List_pop_front(...)    TEMPLATE_METHOD(List, pop_front, __VA_ARGS__)
#define List_pop_back(...)     TEMPLATE_METHOD(List, pop_back, __VA_ARGS__)
#define List_free(...)         TEMPLATE_METHOD(List, free, __VA_ARGS__)
#define List_debug(...)        TEMPLATE_METHOD(List, debug, __VA_ARGS__)
#define List_size(...)         TEMPLATE_METHOD(List, size, __VA_ARGS__)
#define List_into_iter(...)    TEMPLATE_METHOD(List, into_iter, __VA_ARGS__)
#define List_clone(...)        TEMPLATE_METHOD(List, clone, __VA_ARGS__)

#define iter_List_deref(...)   TEMPLATE_METHOD(iter_List, deref, __VA_ARGS__)
#define iter_List_next(...)    TEMPLATE_METHOD(iter_List, next, __VA_ARGS__)
#define iter_List_free(...)    TEMPLATE_METHOD(iter_List, free, __VA_ARGS__)

// Backward compatibility alias for the manual LINKED_LIST_CONFIG
#define LINKED_LIST_CONFIG(T) TEMPLATE_List(T)

// 2. Template Definition Macro
#define TEMPLATE_List(T) \
  typedef struct TEMPLATE_TYPE(Node, T) \
  { \
    T data; \
    struct TEMPLATE_TYPE(Node, T) *next; \
  } TEMPLATE_TYPE(Node, T); \
  typedef TEMPLATE_TYPE(Node, T) *TEMPLATE_TYPE(ref_Node, T); \
  typedef const TEMPLATE_TYPE(Node, T) *TEMPLATE_TYPE(cref_Node, T); \
  static inline void TEMPLATE_METHOD(ref_Node, free, T)(TEMPLATE_TYPE(ref_Node, T) *value) { (void)value; } \
  static inline void TEMPLATE_METHOD(cref_Node, free, T)(TEMPLATE_TYPE(cref_Node, T) *value) { (void)value; } \
  \
  typedef struct \
  { \
    TEMPLATE_TYPE(ref_Node, T) head; \
    TEMPLATE_TYPE(ref_Node, T) tail; \
    Size size; \
  } List(T); \
  typedef List(T) *ref_List(T); \
  typedef const List(T) *cref_List(T); \
  static inline void TEMPLATE_METHOD(ref_List, free, T)(ref_List(T) *value) { (void)value; } \
  static inline void TEMPLATE_METHOD(cref_List, free, T)(cref_List(T) *value) { (void)value; } \
  \
  typedef struct \
  { \
    cref_List(T) list; \
    TEMPLATE_TYPE(ref_Node, T) node; \
  } iter_List(T); \
  typedef iter_List(T) *ref_iter_List(T); \
  typedef const iter_List(T) *cref_iter_List(T); \
  static inline void TEMPLATE_METHOD(ref_iter_List, free, T)(ref_iter_List(T) *value) { (void)value; } \
  static inline void TEMPLATE_METHOD(cref_iter_List, free, T)(cref_iter_List(T) *value) { (void)value; } \
  static inline void iter_List_free(T)(cref_iter_List(T) value) \
  { \
    (void)value; \
  } \
  \
  static inline List(T) List_new(T)() \
  { \
    return (List(T)){.head = NULL, .tail = NULL, .size = 0}; \
  } \
  \
  static inline Result(Void, cref_Char) List_push_back(T)(ref_List(T) self, T value) \
  { \
    TEMPLATE_TYPE(ref_Node, T) new_node = MY_C_UTILS_MALLOC(sizeof(TEMPLATE_TYPE(Node, T))); \
    if (!new_node) \
    { \
      return Result_err(Void, cref_Char)("Memory allocation failed"); \
    } \
    new_node->data = value; \
    new_node->next = NULL; \
    if (self->size == 0) \
    { \
      self->head = new_node; \
    } \
    else \
    { \
      self->tail->next = new_node; \
    } \
    self->tail = new_node; \
    ++self->size; \
    return Result_ok(Void, cref_Char)((Void){}); \
  } \
  \
  static inline Result(Void, cref_Char) List_push_front(T)(ref_List(T) self, T value) \
  { \
    TEMPLATE_TYPE(ref_Node, T) new_node = MY_C_UTILS_MALLOC(sizeof(TEMPLATE_TYPE(Node, T))); \
    if (!new_node) \
    { \
      return Result_err(Void, cref_Char)("Memory allocation failed"); \
    } \
    new_node->data = value; \
    new_node->next = self->head; \
    self->head = new_node; \
    if (self->size == 0) \
    { \
      self->tail = new_node; \
    } \
    ++self->size; \
    return Result_ok(Void, cref_Char)((Void){}); \
  } \
  \
  static inline Result(T, cref_Char) List_pop_front(T)(ref_List(T) self) \
  { \
    if (self->size == 0) \
    { \
      return Result_err(T, cref_Char)("Pop on empty list"); \
    } \
    TEMPLATE_TYPE(ref_Node, T) temp = self->head; \
    self->head = self->head->next; \
    if (self->head == NULL) \
    { \
      self->tail = NULL; \
    } \
    T data = temp->data; \
    MY_C_UTILS_FREE(temp); \
    --self->size; \
    return Result_ok(T, cref_Char)(data); \
  } \
  \
  static inline Result(T, cref_Char) List_pop_back(T)(ref_List(T) self) \
  { \
    if (self->size == 0) \
    { \
      return Result_err(T, cref_Char)("Pop on empty list"); \
    } \
    TEMPLATE_TYPE(ref_Node, T) current = self->head; \
    TEMPLATE_TYPE(ref_Node, T) previous = NULL; \
    while (current->next != NULL) \
    { \
      previous = current; \
      current = current->next; \
    } \
    if (previous == NULL) \
    { \
      self->head = NULL; \
      self->tail = NULL; \
    } \
    else \
    { \
      previous->next = NULL; \
      self->tail = previous; \
    } \
    T data = current->data; \
    MY_C_UTILS_FREE(current); \
    --self->size; \
    return Result_ok(T, cref_Char)(data); \
  } \
  \
  static inline void List_free(T)(ref_List(T) self) \
  { \
    TEMPLATE_TYPE(ref_Node, T) current = self->head; \
    while (current != NULL) \
    { \
      TEMPLATE_TYPE(ref_Node, T) next = current->next; \
      T##_free(&current->data); \
      MY_C_UTILS_FREE(current); \
      current = next; \
    } \
    self->head = NULL; \
    self->tail = NULL; \
    self->size = 0; \
  } \
  \
  static inline void List_debug(T)(cref_List(T) self) \
  { \
    printf("Size: %zu\n", self->size); \
    TEMPLATE_TYPE(ref_Node, T) current = self->head; \
    Size index = 0; \
    while (current != NULL) \
    { \
      printf("  [%zu]\n", index); \
      current = current->next; \
      ++index; \
    } \
  } \
  \
  static inline Size List_size(T)(cref_List(T) self) \
  { \
    return self->size; \
  } \
  \
  static inline iter_List(T) List_into_iter(T)( \
      cref_List(T) self) \
  { \
    return (iter_List(T)){ \
        .list = self, \
        .node = self->head}; \
  } \
  \
  static inline Result(ref_##T, cref_Char) iter_List_deref(T)( \
      cref_iter_List(T) self) \
  { \
    if (self->node == NULL) \
    { \
      return Result_err(ref_##T, cref_Char)("Iterator out of bounds"); \
    } \
    return Result_ok(ref_##T, cref_Char)(&self->node->data); \
  } \
  \
  static inline Result(ref_##T, cref_Char) iter_List_next(T)(ref_iter_List(T) self) \
  { \
    if (self->node == NULL) \
    { \
      return Result_err(ref_##T, cref_Char)("Iterator out of bounds"); \
    } \
    TEMPLATE_TYPE(ref_Node, T) cur = self->node; \
    self->node = self->node->next; \
    return Result_ok(ref_##T, cref_Char)(&cur->data); \
  } \
  RESULT_CONFIG(List(T), cref_Char) \
  RESULT_CONFIG(ref_List(T), cref_Char) \
  static inline List(T) List_clone(T)(cref_List(T) self) \
  { \
    if (!self) \
    { \
      perror("Cannot clone NULL List pointer"); \
      exit(1); \
    } \
    List(T) dest = List_new(T)(); \
    TEMPLATE_TYPE(ref_Node, T) node = self->head; \
    while (node != NULL) \
    { \
      T cloned_value = T##_clone(&node->data); \
      Result(Void, cref_Char) push_res = List_push_back(T)(&dest, cloned_value); \
      if (Result_is_err(Void, cref_Char)(&push_res)) \
      { \
        List_free(T)(&dest); \
        perror("Memory allocation failed during list clone"); \
        exit(1); \
      } \
      node = node->next; \
    } \
    return dest; \
  }

#endif
