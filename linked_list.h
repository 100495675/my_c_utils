#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "result.h"

#define LINKED_LIST_CONFIG(Type)                                               \
  typedef struct Node_##Type {                                                 \
    Type data;                                                                 \
    struct Node_##Type *next;                                                  \
  } Node_##Type;                                                               \
                                                                               \
  typedef struct {                                                             \
    Node_##Type *head;                                                         \
    Node_##Type *tail;                                                         \
    size_t size;                                                               \
  } List_##Type;                                                               \
                                                                               \
  typedef struct {                                                             \
    List_##Type linked_list;                                                   \
    Node_##Type *node;                                                         \
  } List_##Type##_iter;                                                        \
                                                                               \
  RESULT_CONFIG(Type)                                                          \
                                                                               \
  static inline List_##Type List_##Type##_new() {                              \
    return (List_##Type){.head = NULL, .tail = NULL, .size = 0};               \
  }                                                                            \
                                                                               \
  static inline void List_##Type##_push_back(List_##Type *self, Type value) {  \
    Node_##Type *new_node = malloc(sizeof(Node_##Type));                       \
    new_node->data = value;                                                    \
    new_node->next = NULL;                                                     \
    if (self->size == 0) {                                                     \
      self->head = new_node;                                                   \
    } else {                                                                   \
      self->tail->next = new_node;                                             \
    }                                                                          \
    self->tail = new_node;                                                     \
    ++self->size;                                                              \
  }                                                                            \
                                                                               \
  static inline Result_##Type List_##Type##_pop_back(List_##Type *self) {      \
    if (self->size == 0) {                                                     \
      return Result_##Type##_err("Pop on empty list");                         \
    }                                                                          \
    Node_##Type *current = self->head;                                         \
    Node_##Type *previous = NULL;                                              \
    while (current->next != NULL) {                                            \
      previous = current;                                                      \
      current = current->next;                                                 \
    }                                                                          \
    if (previous == NULL) {                                                    \
      self->head = NULL;                                                       \
      self->tail = NULL;                                                       \
    } else {                                                                   \
      previous->next = NULL;                                                   \
      self->tail = previous;                                                   \
    }                                                                          \
    Type data = current->data;                                                 \
    free(current);                                                             \
    --self->size;                                                              \
    return Result_##Type##_ok(data);                                           \
  }                                                                            \
                                                                               \
  static inline void List_##Type##_free(List_##Type *self) {                   \
    Node_##Type *current = self->head;                                         \
    while (current != NULL) {                                                  \
      Node_##Type *next = current->next;                                       \
      Type##_free(&current->data);                                             \
      free(current);                                                           \
      current = next;                                                          \
    }                                                                          \
    self->head = NULL;                                                         \
    self->tail = NULL;                                                         \
    self->size = 0;                                                            \
  }                                                                            \
                                                                               \
  static inline void List_##Type##_debug(List_##Type *self) {                  \
    printf("Size: %zu\n", self->size);                                         \
    Node_##Type *current = self->head;                                         \
    while (current != NULL) {                                                  \
      printf("%d\n", current->data);                                           \
      current = current->next;                                                 \
    }                                                                          \
  }                                                                            \
                                                                               \
  static inline size_t List_##Type##_size(List_##Type *self) {                 \
    return self->size;                                                         \
  }

#endif