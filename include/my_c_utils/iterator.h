#ifndef MY_C_UTILS_ITERATOR_H
#define MY_C_UTILS_ITERATOR_H

/*
Functions the containes has to implement to be iterable:
- into_iter: creates an iterator from the container
- iter_next: returns the next item wrapped in a Result, or an error if there are no more items
- iter_deref: returns the current item without advancing the iterator, wrapped in
    a Result, or an error if the iterator is out of bounds
*/

#define MY_C_UTILS_CONCAT_IMPL(a, b) a##b
#define MY_C_UTILS_CONCAT(a, b) MY_C_UTILS_CONCAT_IMPL(a, b)

// Reference version (pointer) - original behavior
#define for_each_ref_IMPL(ItemType, var_name, container, iterable, body, counter)   \
    for (container##_##ItemType##_iter MY_C_UTILS_CONCAT(_it_, counter) =                \
             container##_##ItemType##_into_iter(iterable);                               \
         ; /* loop until iter_next signals no more items */)                             \
    {                                                                                     \
        Result_##ItemType##_ref MY_C_UTILS_CONCAT(_res_, counter) =                       \
            container##_##ItemType##_iter_next(&MY_C_UTILS_CONCAT(_it_, counter));       \
        if (!Result_##ItemType##_ref_is_ok(MY_C_UTILS_CONCAT(_res_, counter)))           \
        {                                                                                 \
            break;                                                                        \
        }                                                                                 \
        ItemType *var_name = Result_##ItemType##_ref_unwrap(MY_C_UTILS_CONCAT(_res_, counter)); \
        body                                                                              \
    }

// Copy version (value by-copy)
#define for_each_copy_IMPL(ItemType, var_name, container, iterable, body, counter)  \
    for (container##_##ItemType##_iter MY_C_UTILS_CONCAT(_it_, counter) =                \
             container##_##ItemType##_into_iter(iterable);                               \
         ; )                                                                             \
    {                                                                                     \
        Result_##ItemType##_ref MY_C_UTILS_CONCAT(_res_, counter) =                       \
            container##_##ItemType##_iter_next(&MY_C_UTILS_CONCAT(_it_, counter));       \
        if (!Result_##ItemType##_ref_is_ok(MY_C_UTILS_CONCAT(_res_, counter)))           \
        {                                                                                 \
            break;                                                                        \
        }                                                                                 \
        ItemType var_name = *Result_##ItemType##_ref_unwrap(MY_C_UTILS_CONCAT(_res_, counter)); \
        body                                                                              \
    }

// Public macros
#define for_each_ref(ItemType, var_name, container, iterable, body)                      \
    for_each_ref_IMPL(ItemType, var_name, container, iterable, body, __COUNTER__)

#define for_each_copy(ItemType, var_name, container, iterable, body)                     \
    for_each_copy_IMPL(ItemType, var_name, container, iterable, body, __COUNTER__)

#endif