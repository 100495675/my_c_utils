#ifndef MY_C_UTILS_ITERATOR_H
#define MY_C_UTILS_ITERATOR_H

/*
Functions the containes has to implement to be iterable:
- into_iter: creates an iterator from the container
- iter_next: returns the next item wrapped in a Result_Void, or an error if there are no more items
- iter_deref: returns the current item without advancing the iterator, wrapped in
    a Result_Void, or an error if the iterator is out of bounds
*/

#define MY_C_UTILS_CONCAT_IMPL(a, b) a##b
#define MY_C_UTILS_CONCAT(a, b) MY_C_UTILS_CONCAT_IMPL(a, b)

// Reference version (borrowed pointer) - exposes immutable borrows.
#define for_each_ref_IMPL(container, ItemType, var_name, iterable, body, counter)                                    \
    Result_Void_ref_##ItemType MY_C_UTILS_CONCAT(_res_, counter);                                                         \
    iter_##container##_##ItemType MY_C_UTILS_CONCAT(_it_, counter) = container##_##ItemType##_into_iter(iterable);   \
    for (;;)                                                                                                         \
    {                                                                                                                \
        MY_C_UTILS_CONCAT(_res_, counter) = iter_##container##_##ItemType##_next(&MY_C_UTILS_CONCAT(_it_, counter)); \
        if (!Result_Void_ref_##ItemType##_is_ok(&MY_C_UTILS_CONCAT(_res_, counter)))                                      \
        {                                                                                                            \
            break;                                                                                                   \
        }                                                                                                            \
        cref_##ItemType var_name = Result_Void_ref_##ItemType##_unwrap(MY_C_UTILS_CONCAT(_res_, counter));                \
        body                                                                                                         \
    }                                                                                                                \
    Result_Void_ref_##ItemType##_free(&MY_C_UTILS_CONCAT(_res_, counter));                                                \
    iter_##container##_##ItemType##_free(&MY_C_UTILS_CONCAT(_it_, counter));

// Copy version (value by-copy)
#define for_each_copy_IMPL(container, ItemType, var_name, iterable, body, counter)                                   \
    Result_Void_ref_##ItemType MY_C_UTILS_CONCAT(_res_, counter);                                                         \
    iter_##container##_##ItemType MY_C_UTILS_CONCAT(_it_, counter) = container##_##ItemType##_into_iter(iterable);   \
    for (;;)                                                                                                         \
    {                                                                                                                \
        MY_C_UTILS_CONCAT(_res_, counter) = iter_##container##_##ItemType##_next(&MY_C_UTILS_CONCAT(_it_, counter)); \
        if (!Result_Void_ref_##ItemType##_is_ok(&MY_C_UTILS_CONCAT(_res_, counter)))                                      \
        {                                                                                                            \
            break;                                                                                                   \
        }                                                                                                            \
        ItemType var_name = *Result_Void_ref_##ItemType##_unwrap(MY_C_UTILS_CONCAT(_res_, counter));                      \
        body                                                                                                         \
    }                                                                                                                \
    Result_Void_ref_##ItemType##_free(&MY_C_UTILS_CONCAT(_res_, counter));                                                \
    iter_##container##_##ItemType##_free(&MY_C_UTILS_CONCAT(_it_, counter));

// Public macros
#define for_each_ref(ItemType, var_name, container, iterable, body) \
    for_each_ref_IMPL(container, ItemType, var_name, iterable, body, __COUNTER__)

#define for_each_copy(ItemType, var_name, container, iterable, body) \
    for_each_copy_IMPL(container, ItemType, var_name, iterable, body, __COUNTER__)

#endif