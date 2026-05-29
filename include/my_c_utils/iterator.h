#ifndef MY_C_UTILS_ITERATOR_H
#define MY_C_UTILS_ITERATOR_H

#include "my_c_utils/template.h"
#include "my_c_utils/pair.h"

/*
Functions the containes has to implement to be iterable:
- into_iter: creates an iterator from the container
- iter_next: returns the next item wrapped in a Result_Void, or an error if there are no more items
- iter_deref: returns the current item without advancing the iterator, wrapped in
    a Result_Void, or an error if the iterator is out of bounds
*/

// Iterator macros moved to template.h to allow global access for result.h and option.h
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
    for (struct { __typeof__(MY_C_UTILS_CONCAT(ContainerT, _into_iter)(*iterable)) it; \
                  __typeof__(iter_next(ContainerT)(NULL)) res; \
                  bool active; } state_var = {.it = MY_C_UTILS_CONCAT(ContainerT, _into_iter)(*iterable), .active = true}; \
         (state_var.active = true) && \
         (state_var.res = iter_next(ContainerT)(&state_var.it), \
          !state_var.res.is_error ? true : (iter_free(ContainerT)(&state_var.it), false));) \
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
    for (struct { __typeof__(MY_C_UTILS_CONCAT(ContainerT, _into_iter)(*iterable)) it; \
                  __typeof__(iter_next(ContainerT)(NULL)) res; \
                  bool active; } state_var = {.it = MY_C_UTILS_CONCAT(ContainerT, _into_iter)(*iterable), .active = true}; \
         (state_var.active = true) && \
         (state_var.res = iter_next(ContainerT)(&state_var.it), \
          !state_var.res.is_error ? true : (iter_free(ContainerT)(&state_var.it), false));) \
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

#define for_each_val_impl(ContainerT, var_name, iterable, state_var) \
    for (struct { iter_val(ContainerT) it; \
                  __typeof__(iter_val_next(ContainerT)(NULL)) res; \
                  bool active; } state_var = {.it = into_iter_val(ContainerT)(iterable), .active = true}; \
         (state_var.active = true) && \
         (state_var.res = iter_val_next(ContainerT)(&state_var.it), \
          !state_var.res.is_error ? true : (iter_val_free(ContainerT)(&state_var.it), false));) \
        for (__typeof__(state_var.res.value) var_name = state_var.res.value; \
             state_var.active; \
             state_var.active = false)

/**
 * @brief Loops over a container, consuming it and exposing each element directly by value (T).
 * @param ContainerT The container type (e.g. Vector(Int), List(Int)).
 * @param var_name Name of the local variable that will hold the element.
 * @param iterable The container instance itself passed by value (e.g. my_vec or Vector_clone(Int)(&my_vec)).
 * @usage for_each_val(Vector(Int), item, Vector_clone(Int)(&my_vec)) { ... }
 */
#define for_each_val(ContainerT, var_name, iterable) \
    for_each_val_impl(ContainerT, var_name, iterable, MY_C_UTILS_CONCAT(_state_, __COUNTER__))

#define for_each_mut_impl(ContainerT, var_name, iterable, state_var) \
    for (struct { iter_mut(ContainerT) it; \
                  __typeof__(iter_mut_next(ContainerT)(NULL)) res; \
                  bool active; } state_var = {.it = iter_mut_init(ContainerT)(iterable), .active = true}; \
         (state_var.active = true) && \
         (state_var.res = iter_mut_next(ContainerT)(&state_var.it), \
          !state_var.res.is_error ? true : (iter_mut_free(ContainerT)(&state_var.it), false));) \
        for (__typeof__(state_var.res.value) var_name = state_var.res.value; \
             state_var.active; \
             state_var.active = false)

/**
 * @brief Loops over a container, borrowing it mutably and exposing each element as a mutable borrowed pointer (ref(T) / T*).
 * @param ContainerT The container type (e.g. Vector(Int), List(Int)).
 * @param var_name Name of the local variable that will hold the ref(T) mutable pointer.
 * @param iterable Pointer to the container instance (e.g. &my_vec).
 * @usage for_each_mut(Vector(Int), item_ref, &my_vec) { *item_ref = 42; }
 */
#define for_each_mut(ContainerT, var_name, iterable) \
    for_each_mut_impl(ContainerT, var_name, iterable, MY_C_UTILS_CONCAT(_state_, __COUNTER__))

#define for_each_const_impl(ContainerT, var_name, iterable, state_var) \
    for (struct { iter_const(ContainerT) it; \
                  __typeof__(iter_const_next(ContainerT)(NULL)) res; \
                  bool active; } state_var = {.it = iter_const_init(ContainerT)(iterable), .active = true}; \
         (state_var.active = true) && \
         (state_var.res = iter_const_next(ContainerT)(&state_var.it), \
          !state_var.res.is_error ? true : (iter_const_free(ContainerT)(&state_var.it), false));) \
        for (__typeof__(state_var.res.value) var_name = state_var.res.value; \
             state_var.active; \
             state_var.active = false)

/**
 * @brief Loops over a container, borrowing it immutably and exposing each element as an immutable borrowed pointer (cref(T) / const T*).
 * @param ContainerT The container type (e.g. Vector(Int), List(Int)).
 * @param var_name Name of the local variable that will hold the cref(T) immutable pointer.
 * @param iterable Pointer to the container instance (e.g. &my_vec).
 * @usage for_each_const(Vector(Int), item_ref, &my_vec) { printf("%d\n", *item_ref); }
 */
#define for_each_const(ContainerT, var_name, iterable) \
    for_each_const_impl(ContainerT, var_name, iterable, MY_C_UTILS_CONCAT(_state_, __COUNTER__))

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
            iter_free(SrcContainerT)(&_s); \
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
            DestContainerT _dest = Default(DestContainerT)(); \
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
            iter(SrcContainerT) _s = MY_C_UTILS_CONCAT(SrcContainerT, _into_iter)(*_c); \
            __typeof__(iter_next(SrcContainerT)(&_s)) _res = iter_next(SrcContainerT)(&_s); \
            while (!_res.is_error) { \
                _f(_res.value); \
                _res = iter_next(SrcContainerT)(&_s); \
            } \
            iter_free(SrcContainerT)(&_s); \
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
        iter_free(SrcContainer)((iter(SrcContainer)*)&self->source); \
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

#define Filtered(...) TEMPLATE_TYPE(Filtered, __VA_ARGS__)
#define iter_Filtered(...) TEMPLATE_TYPE(iter_Filtered, __VA_ARGS__)

/**
 * @brief Configuration macro to instantiate a type-safe lazy filtering iterator.
 * @param SrcContainer The source container type (e.g. Vector(Int)).
 * @param T The element type of the container (e.g. Int).
 */
#define FILTERED_CONFIG(SrcContainer, T) \
    typedef struct { \
        iter(SrcContainer) source; \
        bool (*predicate)(const T*); \
    } iter_Filtered(SrcContainer); \
    \
    static inline void TEMPLATE_METHOD(iter_Filtered, free, SrcContainer)(const iter_Filtered(SrcContainer)* self) { \
        iter_free(SrcContainer)((iter(SrcContainer)*)&self->source); \
    } \
    \
    static inline Result(ref(T), cref(Char)) TEMPLATE_METHOD(iter_Filtered, next, SrcContainer)(iter_Filtered(SrcContainer)* self) { \
        __typeof__(iter_next(SrcContainer)(&self->source)) res = iter_next(SrcContainer)(&self->source); \
        while (!res.is_error) { \
            if (self->predicate(res.value)) { \
                return Result_ok(ref(T), cref(Char))(res.value); \
            } \
            res = iter_next(SrcContainer)(&self->source); \
        } \
        return Result_err(ref(T), cref(Char))(res.error); \
    } \
    \
    static inline Result(ref(T), cref(Char)) TEMPLATE_METHOD(iter_Filtered, deref, SrcContainer)(const iter_Filtered(SrcContainer)* self) { \
        __typeof__(iter_deref(SrcContainer)(&self->source)) res = iter_deref(SrcContainer)(&self->source); \
        if (res.is_error) { \
            return Result_err(ref(T), cref(Char))(res.error); \
        } \
        return Result_ok(ref(T), cref(Char))(res.value); \
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
 * @brief Filters elements of an iterator lazily using a predicate function, returning a new Filtered iterator.
 * @param SrcContainer The source container type (e.g. Vector(Int)).
 * @usage filter(Vector(Int))(my_iter, predicate_func)
 */
#define filter(SrcContainer) \
    ({ \
        typedef __typeof__(*(iter_next(SrcContainer)((iter(SrcContainer)*)0).value)) _SrcT; \
        iter_Filtered(SrcContainer) _filter_impl(iter(SrcContainer) _s, bool (*_predicate)(const _SrcT*)) { \
            return (iter_Filtered(SrcContainer)){.source = _s, .predicate = _predicate}; \
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
            return collect_new(DestContainerT, Filtered(SrcContainerT))(filter(SrcContainerT)(_s, _predicate)); \
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
            iter_free(SrcContainerT)(&_s); \
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
                    iter_free(SrcContainerT)(&_s); \
                    return true; \
                } \
                _res = iter_next(SrcContainerT)(&_s); \
            } \
            iter_free(SrcContainerT)(&_s); \
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
                    iter_free(SrcContainerT)(&_s); \
                    return false; \
                } \
                _res = iter_next(SrcContainerT)(&_s); \
            } \
            iter_free(SrcContainerT)(&_s); \
            return true; \
        } \
        _all_impl; \
    })

#define Take(...) TEMPLATE_TYPE(Take, __VA_ARGS__)
#define iter_Take(...) TEMPLATE_TYPE(iter_Take, __VA_ARGS__)

/**
 * @brief Configuration macro to instantiate a type-safe lazy taking iterator.
 * @param SrcContainer The source container type (e.g. Vector(Int)).
 * @param T The element type (e.g. Int).
 */
#define TAKE_CONFIG(SrcContainer, T) \
    typedef struct { \
        iter(SrcContainer) source; \
        Size limit; \
        Size count; \
    } iter_Take(SrcContainer); \
    \
    static inline void TEMPLATE_METHOD(iter_Take, free, SrcContainer)(const iter_Take(SrcContainer)* self) { \
        iter_free(SrcContainer)((iter(SrcContainer)*)&self->source); \
    } \
    \
    static inline Result(ref(T), cref(Char)) TEMPLATE_METHOD(iter_Take, next, SrcContainer)(iter_Take(SrcContainer)* self) { \
        if (self->count >= self->limit) { \
            return Result_err(ref(T), cref(Char))("Take limit reached"); \
        } \
        __typeof__(iter_next(SrcContainer)(&self->source)) res = iter_next(SrcContainer)(&self->source); \
        if (res.is_error) { \
            return Result_err(ref(T), cref(Char))(res.error); \
        } \
        self->count++; \
        return Result_ok(ref(T), cref(Char))(res.value); \
    } \
    \
    static inline Result(ref(T), cref(Char)) TEMPLATE_METHOD(iter_Take, deref, SrcContainer)(const iter_Take(SrcContainer)* self) { \
        if (self->count >= self->limit) { \
            return Result_err(ref(T), cref(Char))("Take limit reached"); \
        } \
        __typeof__(iter_deref(SrcContainer)(&self->source)) res = iter_deref(SrcContainer)(&self->source); \
        if (res.is_error) { \
            return Result_err(ref(T), cref(Char))(res.error); \
        } \
        return Result_ok(ref(T), cref(Char))(res.value); \
    }

/**
 * @brief Lazily takes the first N elements from an iterator.
 * @param SrcContainer The source container type.
 * @usage take(Vector(Int))(my_iter, 5)
 */
#define take(SrcContainer) \
    ({ \
        iter_Take(SrcContainer) _take_impl(iter(SrcContainer) _s, Size _n) { \
            return (iter_Take(SrcContainer)){.source = _s, .limit = _n, .count = 0}; \
        } \
        _take_impl; \
    })

#define Skip(...) TEMPLATE_TYPE(Skip, __VA_ARGS__)
#define iter_Skip(...) TEMPLATE_TYPE(iter_Skip, __VA_ARGS__)

/**
 * @brief Configuration macro to instantiate a type-safe lazy skipping iterator.
 * @param SrcContainer The source container type (e.g. Vector(Int)).
 * @param T The element type (e.g. Int).
 */
#define SKIP_CONFIG(SrcContainer, T) \
    typedef struct { \
        iter(SrcContainer) source; \
    } iter_Skip(SrcContainer); \
    \
    static inline void TEMPLATE_METHOD(iter_Skip, free, SrcContainer)(const iter_Skip(SrcContainer)* self) { \
        iter_free(SrcContainer)((iter(SrcContainer)*)&self->source); \
    } \
    \
    static inline Result(ref(T), cref(Char)) TEMPLATE_METHOD(iter_Skip, next, SrcContainer)(iter_Skip(SrcContainer)* self) { \
        return iter_next(SrcContainer)(&self->source); \
    } \
    \
    static inline Result(ref(T), cref(Char)) TEMPLATE_METHOD(iter_Skip, deref, SrcContainer)(const iter_Skip(SrcContainer)* self) { \
        return iter_deref(SrcContainer)(&self->source); \
    }

/**
 * @brief Lazily skips the first N elements from an iterator.
 * @param SrcContainer The source container type.
 * @usage skip(Vector(Int))(my_iter, 3)
 */
#define skip(SrcContainer) \
    ({ \
        iter_Skip(SrcContainer) _skip_impl(iter(SrcContainer) _s, Size _n) { \
            for (Size _i = 0; _i < _n; _i++) { \
                __typeof__(iter_next(SrcContainer)(&_s)) _res = iter_next(SrcContainer)(&_s); \
                if (_res.is_error) { \
                    break; \
                } \
            } \
            return (iter_Skip(SrcContainer)){.source = _s}; \
        } \
        _skip_impl; \
    })

#define Enumerate(...) TEMPLATE_TYPE(Enumerate, __VA_ARGS__)
#define iter_Enumerate(...) TEMPLATE_TYPE(iter_Enumerate, __VA_ARGS__)

/**
 * @brief Configuration macro to instantiate a type-safe lazy enumerating iterator.
 * @param SrcContainer The source container type (e.g. Vector(Int)).
 * @param T The element type (e.g. Int).
 */
#define ENUMERATE_CONFIG(SrcContainer, T) \
    PAIR_CONFIG(Size, ref(T)) \
    RESULT_CONFIG(ref(Pair(Size, ref(T))), cref(Char)) \
    typedef struct { \
        iter(SrcContainer) source; \
        Size index; \
        Pair(Size, ref(T)) current_value; \
    } iter_Enumerate(SrcContainer, T); \
    \
    static inline void TEMPLATE_METHOD(iter_Enumerate, free, SrcContainer, T)(const iter_Enumerate(SrcContainer, T)* self) { \
        iter_free(SrcContainer)((iter(SrcContainer)*)&self->source); \
    } \
    \
    static inline Result(ref(Pair(Size, ref(T))), cref(Char)) TEMPLATE_METHOD(iter_Enumerate, next, SrcContainer, T)(iter_Enumerate(SrcContainer, T)* self) { \
        __typeof__(iter_next(SrcContainer)(&self->source)) res = iter_next(SrcContainer)(&self->source); \
        if (res.is_error) { \
            return Result_err(ref(Pair(Size, ref(T))), cref(Char))(res.error); \
        } \
        self->current_value.first = self->index; \
        self->current_value.second = res.value; \
        self->index++; \
        return Result_ok(ref(Pair(Size, ref(T))), cref(Char))(&self->current_value); \
    } \
    \
    static inline Result(ref(Pair(Size, ref(T))), cref(Char)) TEMPLATE_METHOD(iter_Enumerate, deref, SrcContainer, T)(const iter_Enumerate(SrcContainer, T)* self) { \
        __typeof__(iter_deref(SrcContainer)(&self->source)) res = iter_deref(SrcContainer)(&self->source); \
        if (res.is_error) { \
            return Result_err(ref(Pair(Size, ref(T))), cref(Char))(res.error); \
        } \
        iter_Enumerate(SrcContainer, T)* self_mut = (iter_Enumerate(SrcContainer, T)*)self; \
        self_mut->current_value.first = self->index; \
        self_mut->current_value.second = res.value; \
        return Result_ok(ref(Pair(Size, ref(T))), cref(Char))(&self_mut->current_value); \
    }

/**
 * @brief Lazily pairs each element of an iterator with its index, returning a Pair(Size, ref(T)).
 * @param SrcContainer The source container type.
 * @param T The element type of the container.
 * @usage enumerate(Vector(Int), Int)(my_iter)
 */
#define enumerate(SrcContainer, T) \
    ({ \
        iter_Enumerate(SrcContainer, T) _enumerate_impl(iter(SrcContainer) _s) { \
            return (iter_Enumerate(SrcContainer, T)){.source = _s, .index = 0}; \
        } \
        _enumerate_impl; \
    })

#define Zip(...) TEMPLATE_TYPE(Zip, __VA_ARGS__)
#define iter_Zip(...) TEMPLATE_TYPE(iter_Zip, __VA_ARGS__)

/**
 * @brief Configuration macro to instantiate a type-safe lazy zipping iterator.
 * @param SrcContainer1 The first source container type.
 * @param SrcContainer2 The second source container type.
 * @param T1 The first element type.
 * @param T2 The second element type.
 */
#define ZIP_CONFIG(SrcContainer1, SrcContainer2, T1, T2) \
    PAIR_CONFIG(ref(T1), ref(T2)) \
    RESULT_CONFIG(ref(Pair(ref(T1), ref(T2))), cref(Char)) \
    typedef struct { \
        iter(SrcContainer1) source1; \
        iter(SrcContainer2) source2; \
        Pair(ref(T1), ref(T2)) current_value; \
    } iter_Zip(SrcContainer1, SrcContainer2, T1, T2); \
    \
    static inline void TEMPLATE_METHOD(iter_Zip, free, SrcContainer1, SrcContainer2, T1, T2)(const iter_Zip(SrcContainer1, SrcContainer2, T1, T2)* self) { \
        iter_free(SrcContainer1)((iter(SrcContainer1)*)&self->source1); \
        iter_free(SrcContainer2)((iter(SrcContainer2)*)&self->source2); \
    } \
    \
    static inline Result(ref(Pair(ref(T1), ref(T2))), cref(Char)) TEMPLATE_METHOD(iter_Zip, next, SrcContainer1, SrcContainer2, T1, T2)(iter_Zip(SrcContainer1, SrcContainer2, T1, T2)* self) { \
        __typeof__(iter_next(SrcContainer1)(&self->source1)) res1 = iter_next(SrcContainer1)(&self->source1); \
        __typeof__(iter_next(SrcContainer2)(&self->source2)) res2 = iter_next(SrcContainer2)(&self->source2); \
        if (res1.is_error || res2.is_error) { \
            return Result_err(ref(Pair(ref(T1), ref(T2))), cref(Char))("Zip end reached"); \
        } \
        self->current_value.first = res1.value; \
        self->current_value.second = res2.value; \
        return Result_ok(ref(Pair(ref(T1), ref(T2))), cref(Char))(&self->current_value); \
    } \
    \
    static inline Result(ref(Pair(ref(T1), ref(T2))), cref(Char)) TEMPLATE_METHOD(iter_Zip, deref, SrcContainer1, SrcContainer2, T1, T2)(const iter_Zip(SrcContainer1, SrcContainer2, T1, T2)* self) { \
        __typeof__(iter_deref(SrcContainer1)(&self->source1)) res1 = iter_deref(SrcContainer1)(&self->source1); \
        __typeof__(iter_deref(SrcContainer2)(&self->source2)) res2 = iter_deref(SrcContainer2)(&self->source2); \
        if (res1.is_error || res2.is_error) { \
            return Result_err(ref(Pair(ref(T1), ref(T2))), cref(Char))("Zip end reached"); \
        } \
        iter_Zip(SrcContainer1, SrcContainer2, T1, T2)* self_mut = (iter_Zip(SrcContainer1, SrcContainer2, T1, T2)*)self; \
        self_mut->current_value.first = res1.value; \
        self_mut->current_value.second = res2.value; \
        return Result_ok(ref(Pair(ref(T1), ref(T2))), cref(Char))(&self_mut->current_value); \
    }

/**
 * @brief Lazily pairs elements from two iterators concurrently, returning a Pair(ref(T1), ref(T2)).
 * @param SrcContainer1 The first source container type.
 * @param SrcContainer2 The second source container type.
 * @param T1 The first element type.
 * @param T2 The second element type.
 * @usage zip(List(Int), Vector(Int), Int, Int)(my_list_iter, my_vec_iter)
 */
#define zip(SrcContainer1, SrcContainer2, T1, T2) \
    ({ \
        iter_Zip(SrcContainer1, SrcContainer2, T1, T2) _zip_impl(iter(SrcContainer1) _s1, iter(SrcContainer2) _s2) { \
            return (iter_Zip(SrcContainer1, SrcContainer2, T1, T2)){.source1 = _s1, .source2 = _s2}; \
        } \
        _zip_impl; \
    })

#define pipeline_1(input) (input)

#define pipeline_2(input, step1) \
    ({ \
        __typeof__(input) _val = (input); \
        __typeof__(step1) _temp = (step1); \
        _temp; \
    })

#define pipeline_3(input, step1, step2) \
    ({ \
        __typeof__(input) _val = (input); \
        ({ \
            __typeof__(step1) _temp = (step1); \
            __typeof__(_temp) _val = _temp; \
            __typeof__(step2) _temp2 = (step2); \
            _temp2; \
        }); \
    })

#define pipeline_4(input, step1, step2, step3) \
    ({ \
        __typeof__(input) _val = (input); \
        ({ \
            __typeof__(step1) _temp = (step1); \
            __typeof__(_temp) _val = _temp; \
            ({ \
                __typeof__(step2) _temp = (step2); \
                __typeof__(_temp) _val = _temp; \
                __typeof__(step3) _temp2 = (step3); \
                _temp2; \
            }); \
        }); \
    })

#define pipeline_5(input, step1, step2, step3, step4) \
    ({ \
        __typeof__(input) _val = (input); \
        ({ \
            __typeof__(step1) _temp = (step1); \
            __typeof__(_temp) _val = _temp; \
            ({ \
                __typeof__(step2) _temp = (step2); \
                __typeof__(_temp) _val = _temp; \
                ({ \
                    __typeof__(step3) _temp = (step3); \
                    __typeof__(_temp) _val = _temp; \
                    __typeof__(step4) _temp2 = (step4); \
                    _temp2; \
                }); \
            }); \
        }); \
    })

#define pipeline_6(input, step1, step2, step3, step4, step5) \
    ({ \
        __typeof__(input) _val = (input); \
        ({ \
            __typeof__(step1) _temp = (step1); \
            __typeof__(_temp) _val = _temp; \
            ({ \
                __typeof__(step2) _temp = (step2); \
                __typeof__(_temp) _val = _temp; \
                ({ \
                    __typeof__(step3) _temp = (step3); \
                    __typeof__(_temp) _val = _temp; \
                    ({ \
                        __typeof__(step4) _temp = (step4); \
                        __typeof__(_temp) _val = _temp; \
                        __typeof__(step5) _temp2 = (step5); \
                        _temp2; \
                    }); \
                }); \
            }); \
        }); \
    })

#define pipeline_7(input, step1, step2, step3, step4, step5, step6) \
    ({ \
        __typeof__(input) _val = (input); \
        ({ \
            __typeof__(step1) _temp = (step1); \
            __typeof__(_temp) _val = _temp; \
            ({ \
                __typeof__(step2) _temp = (step2); \
                __typeof__(_temp) _val = _temp; \
                ({ \
                    __typeof__(step3) _temp = (step3); \
                    __typeof__(_temp) _val = _temp; \
                    ({ \
                        __typeof__(step4) _temp = (step4); \
                        __typeof__(_temp) _val = _temp; \
                        ({ \
                            __typeof__(step5) _temp = (step5); \
                            __typeof__(_temp) _val = _temp; \
                            __typeof__(step6) _temp2 = (step6); \
                            _temp2; \
                        }); \
                    }); \
                }); \
            }); \
        }); \
    })

#define pipeline_8(input, step1, step2, step3, step4, step5, step6, step7) \
    ({ \
        __typeof__(input) _val = (input); \
        ({ \
            __typeof__(step1) _temp = (step1); \
            __typeof__(_temp) _val = _temp; \
            ({ \
                __typeof__(step2) _temp = (step2); \
                __typeof__(_temp) _val = _temp; \
                ({ \
                    __typeof__(step3) _temp = (step3); \
                    __typeof__(_temp) _val = _temp; \
                    ({ \
                        __typeof__(step4) _temp = (step4); \
                        __typeof__(_temp) _val = _temp; \
                        ({ \
                            __typeof__(step5) _temp = (step5); \
                            __typeof__(_temp) _val = _temp; \
                            ({ \
                                __typeof__(step6) _temp = (step6); \
                                __typeof__(_temp) _val = _temp; \
                                __typeof__(step7) _temp2 = (step7); \
                                _temp2; \
                            }); \
                        }); \
                    }); \
                }); \
            }); \
        }); \
    })

/**
 * @brief Thread-safe, type-safe declarative pipeline composition macro (The Pipe Operator).
 * @usage Vector(Int) res = pipeline(input, step1(_val), step2(_val), ...);
 */
#define pipeline(...) \
    MY_C_UTILS_PP_CAT(pipeline_, MY_C_UTILS_NARGS(__VA_ARGS__))(__VA_ARGS__)

#endif