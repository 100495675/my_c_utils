#ifndef MY_C_UTILS_ITERATOR_H
#define MY_C_UTILS_ITERATOR_H

#include "my_c_utils/template.h"

/*
Functions the containes has to implement to be iterable:
- into_iter: creates an iterator from the container
- iter_next: returns the next item wrapped in a Result_Void, or an error if there are no more items
- iter_deref: returns the current item without advancing the iterator, wrapped in
    a Result_Void, or an error if the iterator is out of bounds
*/

#define iter(Container) MY_C_UTILS_CONCAT(iter_, Container)
#define ref_iter(Container) MY_C_UTILS_CONCAT(ref_iter_, Container)
#define cref_iter(Container) MY_C_UTILS_CONCAT(cref_iter_, Container)

#define into_iter(Container) MY_C_UTILS_CONCAT(Container, _into_iter)
#define iter_next(Container) MY_C_UTILS_CONCAT(iter(Container), _next)
#define iter_deref(Container) MY_C_UTILS_CONCAT(iter(Container), _deref)
#define iter_free(Container) MY_C_UTILS_CONCAT(iter(Container), _free)

// NOTE: These loop macros use a single-parenthesis notation: for_each(ContainerT, var_name, iterable).
// This is an intentional exception to the project's standard double-parentheses generic notation (e.g. collect(Dest, Src)(...))
// due to standard C preprocessor syntax limitations for control structures (cannot define macros inside macros or paste unclosed parentheses without compiler errors).

/**
 * @brief Loops over a container, automatically creating an iterator and exposing each element as a borrowed reference pointer (ref(T)).
 * @param ContainerT The container type (e.g. Vector(Int), List(Int)).
 * @param var_name Name of the local variable that will hold the ref(T) borrowed pointer.
 * @param iterable Pointer to the container instance to iterate over (e.g. &my_vec).
 * @usage for_each_ref(Vector(Int), item_ref, &my_vec) { ... }
 */
#define for_each_ref(ContainerT, var_name, iterable) \
    for (struct { __typeof__(MY_C_UTILS_CONCAT(ContainerT, _into_iter)(iterable)) it; \
                  __typeof__(iter_next(ContainerT)(NULL)) res; \
                  bool active; } _state = {.it = MY_C_UTILS_CONCAT(ContainerT, _into_iter)(iterable), .active = true}; \
         (_state.active = true) && (_state.res = iter_next(ContainerT)(&_state.it), !_state.res.is_error);) \
        for (__typeof__(_state.res.value) var_name = _state.res.value; \
             _state.active; \
             _state.active = false)

/**
 * @brief Loops over a container, automatically creating an iterator and exposing each element by copy (value).
 * @param ContainerT The container type (e.g. Vector(Int), List(Int)).
 * @param var_name Name of the local variable that will hold the element copy.
 * @param iterable Pointer to the container instance to iterate over (e.g. &my_vec).
 * @usage for_each(Vector(Int), item, &my_vec) { ... }
 */
#define for_each(ContainerT, var_name, iterable) \
    for (struct { __typeof__(MY_C_UTILS_CONCAT(ContainerT, _into_iter)(iterable)) it; \
                  __typeof__(iter_next(ContainerT)(NULL)) res; \
                  bool active; } _state = {.it = MY_C_UTILS_CONCAT(ContainerT, _into_iter)(iterable), .active = true}; \
         (_state.active = true) && (_state.res = iter_next(ContainerT)(&_state.it), !_state.res.is_error);) \
        for (__typeof__(*(_state.res.value)) var_name = *(_state.res.value); \
             _state.active; \
             _state.active = false)

/**
 * @brief Collects all remaining elements from a source iterator and appends them to a destination container.
 * @param DestContainerT The destination container type (e.g. Vector(Int), List(Int)).
 * @param SrcContainerT The source container/iterator type (e.g. Vector(Int), List(Int)).
 * @usage collect(Vector(Int), List(Int))(&my_vec, my_list_iter)
 */
#define collect(DestContainerT, SrcContainerT) \
    ({ \
        void _collect_impl(DestContainerT* _d, iter(SrcContainerT) _s) { \
            __typeof__(iter_next(SrcContainerT)(&_s)) _res = iter_next(SrcContainerT)(&_s); \
            while (!_res.is_error) { \
                __typeof__(_res.value) _item_ref = _res.value; \
                __typeof__(*(_item_ref)) _item = *(_item_ref); \
                (void)MY_C_UTILS_CONCAT(DestContainerT, _push_back)(_d, _item); \
                _res = iter_next(SrcContainerT)(&_s); \
            } \
        } \
        _collect_impl; \
    })

/**
 * @brief Constructs a new destination container and collects all remaining elements from an iterator into it, returning the container.
 * @param DestContainerT The destination container type to create (e.g. Vector(Int), List(Int)).
 * @param SrcContainerT The source container/iterator type (e.g. Vector(Int), List(Int)).
 * @returns A populated DestContainerT instance.
 * @usage Vector(Int) vec = collect_new(Vector(Int), List(Int))(my_list_iter);
 */
#define collect_new(DestContainerT, SrcContainerT) \
    ({ \
        DestContainerT _collect_new_impl(iter(SrcContainerT) _s) { \
            DestContainerT _dest = MY_C_UTILS_CONCAT(DestContainerT, _new)(); \
            collect(DestContainerT, SrcContainerT)(&_dest, _s); \
            return _dest; \
        } \
        _collect_new_impl; \
    })

#endif