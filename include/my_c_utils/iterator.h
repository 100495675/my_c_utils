#ifndef MY_C_UTILS_ITERATOR_H
#define MY_C_UTILS_ITERATOR_H

/*
Functions the containes has to implement to be iterable:
- into_iter: creates an iterator from the container
- iter_next: returns the next item wrapped in a Result_Void, or an error if there are no more items
- iter_deref: returns the current item without advancing the iterator, wrapped in
    a Result_Void, or an error if the iterator is out of bounds
*/


// Reference version (borrowed pointer) - exposes immutable borrows.
#define for_each_ref(ContainerType, var_name, iterable)                                                                                                 \
    for (struct { __typeof__(MY_C_UTILS_CONCAT(ContainerType, _into_iter)(iterable)) it; \
                  __typeof__(MY_C_UTILS_CONCAT(iter_, MY_C_UTILS_CONCAT(ContainerType, _next))(NULL)) res; \
                  bool active; } _state = {.it = MY_C_UTILS_CONCAT(ContainerType, _into_iter)(iterable), .active = true};                                            \
         (_state.active = true) && (_state.res = MY_C_UTILS_CONCAT(iter_, MY_C_UTILS_CONCAT(ContainerType, _next))(&_state.it), !_state.res.is_error);) \
        for (__typeof__(_state.res.value) var_name = _state.res.value;                                                                                  \
             _state.active;                                                                                                                             \
             _state.active = false)

// Copy version (value by-copy)
#define for_each_copy(ContainerType, var_name, iterable)                                                                                                \
    for (struct { __typeof__(MY_C_UTILS_CONCAT(ContainerType, _into_iter)(iterable)) it; \
                  __typeof__(MY_C_UTILS_CONCAT(iter_, MY_C_UTILS_CONCAT(ContainerType, _next))(NULL)) res; \
                  bool active; } _state = {.it = MY_C_UTILS_CONCAT(ContainerType, _into_iter)(iterable), .active = true};                                            \
         (_state.active = true) && (_state.res = MY_C_UTILS_CONCAT(iter_, MY_C_UTILS_CONCAT(ContainerType, _next))(&_state.it), !_state.res.is_error);) \
        for (__typeof__(*_state.res.value) var_name = *_state.res.value;                                                                                \
             _state.active;                                                                                                                             \
             _state.active = false)

#endif