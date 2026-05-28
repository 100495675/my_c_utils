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
#define lambda_impl(ret_t, params, body, unique_id) \
    ({ \
        ret_t unique_id params body \
        unique_id; \
    })

/**
 * @brief Utility macro to define a type-safe inline anonymous function (lambda) with an explicit return.
 * @param ret_t The return type of the function.
 * @param params The parameter list in parentheses, e.g. (Int x) or (Int acc, Int x).
 * @param body The function body block inside braces, e.g. { return x * 2; }.
 * @returns A pointer to the newly defined inline function.
 * @usage lambda(Int, (Int x), { return x * 2; })
 */
#define lambda(ret_t, params, body) \
    lambda_impl(ret_t, params, body, MY_C_UTILS_CONCAT(_lambda_, __COUNTER__))

// NOTE: These loop macros use a single-parenthesis notation: for_each(ContainerT, var_name, iterable).
// This is an intentional exception to the project's standard double-parentheses generic notation (e.g. collect(Dest, Src)(...))
// due to standard C preprocessor syntax limitations for control structures (cannot define macros inside macros or paste unclosed parentheses without compiler errors).

#define for_each_ref_impl(ContainerT, var_name, iterable, state_var) \
    for (struct { __typeof__(MY_C_UTILS_CONCAT(ContainerT, _into_iter)(iterable)) it; \
                  __typeof__(iter_next(ContainerT)(NULL)) res; \
                  bool active; } state_var = {.it = MY_C_UTILS_CONCAT(ContainerT, _into_iter)(iterable), .active = true}; \
         (state_var.active = true) && (state_var.res = iter_next(ContainerT)(&state_var.it), !state_var.res.is_error);) \
        for (__typeof__(state_var.res.value) var_name = state_var.res.value; \
             state_var.active; \
             state_var.active = false)

/**
 * @brief Loops over a container, automatically creating an iterator and exposing each element as a borrowed reference pointer (ref(T)).
 * @param ContainerT The container type (e.g. Vector(Int), List(Int)).
 * @param var_name Name of the local variable that will hold the ref(T) borrowed pointer.
 * @param iterable Pointer to the container instance to iterate over (e.g. &my_vec).
 * @usage for_each_ref(Vector(Int), item_ref, &my_vec) { ... }
 */
#define for_each_ref(ContainerT, var_name, iterable) \
    for_each_ref_impl(ContainerT, var_name, iterable, MY_C_UTILS_CONCAT(_state_, __COUNTER__))

#define for_each_impl(ContainerT, var_name, iterable, state_var) \
    for (struct { __typeof__(MY_C_UTILS_CONCAT(ContainerT, _into_iter)(iterable)) it; \
                  __typeof__(iter_next(ContainerT)(NULL)) res; \
                  bool active; } state_var = {.it = MY_C_UTILS_CONCAT(ContainerT, _into_iter)(iterable), .active = true}; \
         (state_var.active = true) && (state_var.res = iter_next(ContainerT)(&state_var.it), !state_var.res.is_error);) \
        for (__typeof__(*(state_var.res.value)) var_name = *(state_var.res.value); \
             state_var.active; \
             state_var.active = false)

/**
 * @brief Loops over a container, automatically creating an iterator and exposing each element by copy (value).
 * @param ContainerT The container type (e.g. Vector(Int), List(Int)).
 * @param var_name Name of the local variable that will hold the element copy.
 * @param iterable Pointer to the container instance to iterate over (e.g. &my_vec).
 * @usage for_each(Vector(Int), item, &my_vec) { ... }
 */
#define for_each(ContainerT, var_name, iterable) \
    for_each_impl(ContainerT, var_name, iterable, MY_C_UTILS_CONCAT(_state_, __COUNTER__))

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

/**
 * @brief Loops over a container and executes a callback function for each element (passed by reference).
 * @param SrcContainerT The source container/iterator type (e.g. Vector(Int), List(Int)).
 * @usage for_each_fn(Vector(Int))(&my_vec, callback_func);
 */
#define for_each_fn(SrcContainerT) \
    ({ \
        typedef __typeof__(*(iter_next(SrcContainerT)((iter(SrcContainerT)*)0).value)) _SrcT; \
        void _for_each_fn_impl(SrcContainerT* _c, void (*_f)(const _SrcT*)) { \
            iter(SrcContainerT) _s = MY_C_UTILS_CONCAT(SrcContainerT, _into_iter)(_c); \
            __typeof__(iter_next(SrcContainerT)(&_s)) _res = iter_next(SrcContainerT)(&_s); \
            while (!_res.is_error) { \
                _f(_res.value); \
                _res = iter_next(SrcContainerT)(&_s); \
            } \
        } \
        _for_each_fn_impl; \
    })

#define Mapped(...) TEMPLATE_TYPE(Mapped, __VA_ARGS__)
#define iter_Mapped(...) TEMPLATE_TYPE(iter_Mapped, __VA_ARGS__)

/**
 * @brief Configuration macro to instantiate a type-safe lazy mapping iterator.
 * @param SrcContainer The source container type (e.g. Vector(Int)).
 * @param DestT The destination element type (e.g. Int, Double).
 */
#define MAPPED_CONFIG(SrcContainer, DestT) \
    typedef struct { \
        iter(SrcContainer) source; \
        DestT (*func)(const __typeof__(*(iter_next(SrcContainer)((iter(SrcContainer)*)0).value))*); \
        DestT current_value; \
    } iter_Mapped(SrcContainer, DestT); \
    \
    static inline void TEMPLATE_METHOD(iter_Mapped, free, SrcContainer, DestT)(const iter_Mapped(SrcContainer, DestT)* self) { \
        (void)self; \
    } \
    \
    static inline Result(ref(DestT), cref(Char)) TEMPLATE_METHOD(iter_Mapped, next, SrcContainer, DestT)(iter_Mapped(SrcContainer, DestT)* self) { \
        __typeof__(iter_next(SrcContainer)(&self->source)) res = iter_next(SrcContainer)(&self->source); \
        if (res.is_error) { \
            return Result_err(ref(DestT), cref(Char))(res.error); \
        } \
        self->current_value = self->func(res.value); \
        return Result_ok(ref(DestT), cref(Char))(&self->current_value); \
    } \
    \
    static inline Result(ref(DestT), cref(Char)) TEMPLATE_METHOD(iter_Mapped, deref, SrcContainer, DestT)(const iter_Mapped(SrcContainer, DestT)* self) { \
        __typeof__(iter_deref(SrcContainer)(&self->source)) res = iter_deref(SrcContainer)(&self->source); \
        if (res.is_error) { \
            return Result_err(ref(DestT), cref(Char))(res.error); \
        } \
        iter_Mapped(SrcContainer, DestT)* self_mut = (iter_Mapped(SrcContainer, DestT)*)self; \
        self_mut->current_value = self->func(res.value); \
        return Result_ok(ref(DestT), cref(Char))(&self_mut->current_value); \
    }

/**
 * @brief Maps each element of an iterator lazily using a mapping function, returning a new Mapped iterator.
 * @param SrcContainer The source container type (e.g. Vector(Int)).
 * @param DestT The destination element type (e.g. Int, Double).
 * @usage map(Vector(Int), Int)(my_iter, double_func)
 */
#define map(SrcContainer, DestT) \
    ({ \
        typedef __typeof__(*(iter_next(SrcContainer)((iter(SrcContainer)*)0).value)) _SrcT; \
        iter_Mapped(SrcContainer, DestT) _map_impl(iter(SrcContainer) _s, DestT (*_f)(const _SrcT*)) { \
            return (iter_Mapped(SrcContainer, DestT)){.source = _s, .func = _f}; \
        } \
        _map_impl; \
    })

/**
 * @brief Filters elements of a source iterator using a predicate and appends those that match to a destination container.
 * @param DestContainerT The destination container type (e.g. Vector(Int), List(Int)).
 * @param SrcContainerT The source container/iterator type (e.g. Vector(Int), List(Int)).
 * @usage filter(Vector(Int), List(Int))(&my_vec, my_list_iter, is_even)
 */
#define filter(DestContainerT, SrcContainerT) \
    ({ \
        typedef __typeof__(*(iter_next(SrcContainerT)((iter(SrcContainerT)*)0).value)) _SrcT; \
        void _filter_impl(DestContainerT* _d, iter(SrcContainerT) _s, bool (*_predicate)(const _SrcT*)) { \
            __typeof__(iter_next(SrcContainerT)(&_s)) _res = iter_next(SrcContainerT)(&_s); \
            while (!_res.is_error) { \
                if (_predicate(_res.value)) { \
                    (void)MY_C_UTILS_CONCAT(DestContainerT, _push_back)(_d, *(_res.value)); \
                } \
                _res = iter_next(SrcContainerT)(&_s); \
            } \
        } \
        _filter_impl; \
    })

/**
 * @brief Constructs a new destination container and populates it with elements of a source iterator that match a predicate.
 * @param DestContainerT The destination container type to create (e.g. Vector(Int), List(Int)).
 * @param SrcContainerT The source container/iterator type (e.g. Vector(Int), List(Int)).
 * @returns A populated DestContainerT instance.
 * @usage Vector(Int) vec = filter_new(Vector(Int), List(Int))(my_list_iter, is_even);
 */
#define filter_new(DestContainerT, SrcContainerT) \
    ({ \
        typedef __typeof__(*(iter_next(SrcContainerT)((iter(SrcContainerT)*)0).value)) _SrcT; \
        DestContainerT _filter_new_impl(iter(SrcContainerT) _s, bool (*_predicate)(const _SrcT*)) { \
            DestContainerT _dest = MY_C_UTILS_CONCAT(DestContainerT, _new)(); \
            filter(DestContainerT, SrcContainerT)(&_dest, _s, _predicate); \
            return _dest; \
        } \
        _filter_new_impl; \
    })

/**
 * @brief Folds/reduces the elements of an iterator into a single value, starting with an initial accumulator value.
 * @param SrcContainerT The source container/iterator type (e.g. Vector(Int), List(Int)).
 * @param AccT The type of the accumulator.
 * @usage Int sum = fold(Vector(Int), Int)(0, my_vec_iter, add_func);
 */
#define fold(SrcContainerT, AccT) \
    ({ \
        typedef __typeof__(*(iter_next(SrcContainerT)((iter(SrcContainerT)*)0).value)) _SrcT; \
        AccT _fold_impl(AccT _init, iter(SrcContainerT) _s, AccT (*_f)(AccT, const _SrcT*)) { \
            AccT _acc = _init; \
            __typeof__(iter_next(SrcContainerT)(&_s)) _res = iter_next(SrcContainerT)(&_s); \
            while (!_res.is_error) { \
                _acc = _f(_acc, _res.value); \
                _res = iter_next(SrcContainerT)(&_s); \
            } \
            return _acc; \
        } \
        _fold_impl; \
    })

/**
 * @brief Returns true if at least one element of the iterator matches the predicate.
 * @param SrcContainerT The source container/iterator type (e.g. Vector(Int), List(Int)).
 * @usage bool has_negative = any(Vector(Int))(my_vec_iter, is_negative);
 */
#define any(SrcContainerT) \
    ({ \
        typedef __typeof__(*(iter_next(SrcContainerT)((iter(SrcContainerT)*)0).value)) _SrcT; \
        bool _any_impl(iter(SrcContainerT) _s, bool (*_predicate)(const _SrcT*)) { \
            __typeof__(iter_next(SrcContainerT)(&_s)) _res = iter_next(SrcContainerT)(&_s); \
            while (!_res.is_error) { \
                if (_predicate(_res.value)) { \
                    return true; \
                } \
                _res = iter_next(SrcContainerT)(&_s); \
            } \
            return false; \
        } \
        _any_impl; \
    })

/**
 * @brief Returns true if all elements of the iterator match the predicate.
 * @param SrcContainerT The source container/iterator type (e.g. Vector(Int), List(Int)).
 * @usage bool all_positive = all(Vector(Int))(my_vec_iter, is_positive);
 */
#define all(SrcContainerT) \
    ({ \
        typedef __typeof__(*(iter_next(SrcContainerT)((iter(SrcContainerT)*)0).value)) _SrcT; \
        bool _all_impl(iter(SrcContainerT) _s, bool (*_predicate)(const _SrcT*)) { \
            __typeof__(iter_next(SrcContainerT)(&_s)) _res = iter_next(SrcContainerT)(&_s); \
            while (!_res.is_error) { \
                if (!_predicate(_res.value)) { \
                    return false; \
                } \
                _res = iter_next(SrcContainerT)(&_s); \
            } \
            return true; \
        } \
        _all_impl; \
    })

#endif