#ifndef MY_C_UTILS_OPTION_H
#define MY_C_UTILS_OPTION_H

#include "my_c_utils/tipos.h"
#include "my_c_utils/template.h"
#include "my_c_utils/result.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * @brief Represents an optional value that may or may not be filled/present.
 * @usage Option(T)
 */
#define Option(...) TEMPLATE_TYPE(Option, __VA_ARGS__)

/**
 * @brief Mutable borrowed pointer reference to Option(T).
 * @usage ref_Option(T)
 */
#define ref_Option(...) TEMPLATE_TYPE(ref_Option, __VA_ARGS__)

/**
 * @brief Immutable borrowed pointer reference to Option(T).
 * @usage cref_Option(T)
 */
#define cref_Option(...) TEMPLATE_TYPE(cref_Option, __VA_ARGS__)

#define Option_iter_val_Result(...) TEMPLATE_TYPE(Option_iter_val_Result, __VA_ARGS__)
#define Option_iter_mut_Result(...) TEMPLATE_TYPE(Option_iter_mut_Result, __VA_ARGS__)
#define Option_iter_const_Result(...) TEMPLATE_TYPE(Option_iter_const_Result, __VA_ARGS__)

/**
 * @brief Wraps a value of type T inside a filled Option(T).
 * @param T The type of the value.
 * @param value The value to store.
 * @returns Option(T)
 * @usage Option(Int) opt = Option_some(Int)(42);
 */
#define Option_some(...)    TEMPLATE_METHOD(Option, some, __VA_ARGS__)

/**
 * @brief Constructs an empty/none Option(T).
 * @param T The type of the absent value.
 * @returns Option(T)
 * @usage Option(Int) opt = Option_none(Int)();
 */
#define Option_none(...)    TEMPLATE_METHOD(Option, none, __VA_ARGS__)
#define Option_into_iter_val(...) IntoIterator(Option(__VA_ARGS__))
#define Option_into_iter(...)     Option_into_iter_val(__VA_ARGS__)
#define Option_iter_mut(...)      IntoIteratorMut(Option(__VA_ARGS__))
#define Option_iter_const(...)    IntoIteratorConst(Option(__VA_ARGS__))

#define iter_Option_next(...)       Iterator_next(iter_val(Option(__VA_ARGS__)))
#define iter_Option_deref(...)      Iterator_deref(iter_val(Option(__VA_ARGS__)))
#define iter_Option_free(...)       Iterator_free(iter_val(Option(__VA_ARGS__)))
#define Option_push_back(...) TEMPLATE_METHOD(Option, push_back, __VA_ARGS__)
#define Option_new(...)       Option_none(__VA_ARGS__)

/**
 * @brief Unwraps a filled Option(T), returning its inner T value. Panics and aborts if empty.
 * @param T The type of the value.
 * @param self The Option(T) instance.
 * @returns T
 * @usage Int value = Option_unwrap(Int)(opt);
 */
#define Option_unwrap(...)  TEMPLATE_METHOD(Option, unwrap, __VA_ARGS__)

/**
 * @brief Checks if the Option(T) is empty (None).
 * @param T The type of the value.
 * @param self A pointer to the Option(T) instance (&opt).
 * @returns Bool
 * @usage Bool empty = Option_is_none(Int)(&opt);
 */
#define Option_is_none(...) TEMPLATE_METHOD(Option, is_none, __VA_ARGS__)

/**
 * @brief Checks if the Option(T) is filled (Some).
 * @param T The type of the value.
 * @param self A pointer to the Option(T) instance (&opt).
 * @returns Bool
 * @usage Bool filled = Option_is_some(Int)(&opt);
 */
#define Option_is_some(...) TEMPLATE_METHOD(Option, is_some, __VA_ARGS__)

/**
 * @brief Frees the inner value stored in the Option if it is filled.
 * @param T The type of the value.
 * @param self A pointer to the Option(T) instance (&opt).
 * @usage Option_free(Int)(&opt);
 */
#define Option_free(...)    Free(Option(__VA_ARGS__))

/**
 * @brief Maps an Option(T) to Option(U) by applying a function to the contained value.
 * @param T Source type.
 * @param U Destination type.
 * @param self The Option(T) instance.
 * @param fn Mapping function U (*fn)(T).
 * @returns Option(U)
 * @usage Option_map(Int, Double)(opt, int_to_double)
 */
#define Option_map(T, U) \
    ({ \
        Option(U) _map_fn(Option(T) self, U (*fn)(T)) { \
            if (self.filled) { \
                return Option_some(U)(fn(self.value)); \
            } \
            return Option_none(U)(); \
        } \
        _map_fn; \
    })

/**
 * @brief Applies a function returning Option(U) to the contained value of Option(T).
 * @param T Source type.
 * @param U Destination type.
 * @param self The Option(T) instance.
 * @param fn Monadic function Option(U) (*fn)(T).
 * @returns Option(U)
 * @usage Option_and_then(Int, Double)(opt, int_to_opt_double)
 */
#define Option_and_then(T, U) \
    ({ \
        Option(U) _and_then_fn(Option(T) self, Option(U) (*fn)(T)) { \
            if (self.filled) { \
                return fn(self.value); \
            } \
            return Option_none(U)(); \
        } \
        _and_then_fn; \
    })

/**
 * @brief Returns the contained value or a default value.
 * @param T Element type.
 * @param self The Option(T) instance.
 * @param default_value Default value to return if empty.
 * @returns T
 * @usage Int val = Option_unwrap_or(Int)(opt, 0)
 */
#define Option_unwrap_or(...) TEMPLATE_METHOD(Option, unwrap_or, __VA_ARGS__)

// Backward compatibility alias for the manual OPTION_CONFIG
#define OPTION_CONFIG(T) TEMPLATE_Option(T)

// 2. Template Definition Macro
#define TEMPLATE_Option(T) \
    typedef struct \
    { \
        T value; \
        Bool filled; \
    } Option(T); \
    typedef Option(T) *ref_Option(T); \
    typedef const Option(T) *cref_Option(T); \
    static inline void TEMPLATE_METHOD(Free, free, ref(Option(T)))(ref(Option(T)) *value) { (void)value; } \
    static inline void TEMPLATE_METHOD(Free, free, cref(Option(T)))(cref(Option(T)) *value) { (void)value; } \
    static inline ref(Option(T)) TEMPLATE_METHOD(Clone, clone, ref(Option(T)))(const ref(Option(T)) *self) { return *self; } \
    static inline cref(Option(T)) TEMPLATE_METHOD(Clone, clone, cref(Option(T)))(const cref(Option(T)) *self) { return *self; } \
    static inline Option(T) TEMPLATE_METHOD(Deref, deref, ref(Option(T)))(ref(Option(T)) self) { return *self; } \
    static inline Option(T) TEMPLATE_METHOD(Deref, deref, cref(Option(T)))(cref(Option(T)) self) { return *self; } \
    \
    static inline Option(T) Option_some(T)(T value) \
    { \
        return (Option(T)){.value = value, .filled = true}; \
    } \
    \
    static inline Option(T) Option_none(T)() \
    { \
        return (Option(T)){.filled = false}; \
    } \
    \
    static inline T Option_unwrap(T)(Option(T) self) \
    { \
        if (!self.filled) \
        { \
            perror("Option is none"); \
            exit(1); \
        } \
        return self.value; \
    } \
    \
    static inline Bool Option_is_none(T)(cref(Option(T)) self) \
    { \
        return !(self->filled); \
    } \
    \
    static inline Bool Option_is_some(T)(cref(Option(T)) self) \
    { \
        return self->filled; \
    } \
    \
    static inline T Option_unwrap_or(T)(Option(T) self, T default_value) \
    { \
        return self.filled ? self.value : default_value; \
    } \
    \
    typedef struct \
    { \
        Option(T) opt; \
        bool consumed; \
    } iter_val(Option(T)); \
    typedef iter_val(Option(T)) *ref_iter_val(Option(T)); \
    typedef const iter_val(Option(T)) *cref_iter_val(Option(T)); \
    static inline void TEMPLATE_METHOD(ref_iter_val_Option, free, T)(ref_iter_val(Option(T)) *value) { (void)value; } \
    static inline void TEMPLATE_METHOD(cref_iter_val_Option, free, T)(cref_iter_val(Option(T)) *value) { (void)value; } \
    \
    typedef struct \
    { \
        T value; \
        bool is_error; \
    } Option_iter_val_Result(T); \
    \
    typedef struct \
    { \
        T *value; \
        bool is_error; \
    } Option_iter_mut_Result(T); \
    typedef struct \
    { \
        const T *value; \
        bool is_error; \
    } Option_iter_const_Result(T); \
    \
    typedef struct \
    { \
        ref(Option(T)) opt; \
        bool consumed; \
    } iter_mut(Option(T)); \
    typedef iter_mut(Option(T)) *ref_iter_mut(Option(T)); \
    typedef const iter_mut(Option(T)) *cref_iter_mut(Option(T)); \
    static inline void TEMPLATE_METHOD(ref_iter_mut_Option, free, T)(ref_iter_mut(Option(T)) *value) { (void)value; } \
    static inline void TEMPLATE_METHOD(cref_iter_mut_Option, free, T)(cref_iter_mut(Option(T)) *value) { (void)value; } \
    \
    typedef struct \
    { \
        cref(Option(T)) opt; \
        bool consumed; \
    } iter_const(Option(T)); \
    typedef iter_const(Option(T)) *ref_iter_const(Option(T)); \
    typedef const iter_const(Option(T)) *cref_iter_const(Option(T)); \
    static inline void TEMPLATE_METHOD(ref_iter_const_Option, free, T)(ref_iter_const(Option(T)) *value) { (void)value; } \
    static inline void TEMPLATE_METHOD(cref_iter_const_Option, free, T)(cref_iter_const(Option(T)) *value) { (void)value; } \
    \
    static inline Result(Void, cref(Char)) Option_push_back(T)(ref(Option(T)) self, T value) \
    { \
        if (!self->filled) \
        { \
            *self = Option_some(T)(value); \
        } \
        else \
        { \
            Free(T)(&value); \
        } \
        return Result_ok(Void, cref(Char))((Void){}); \
    } \
    \
    static inline Option(T) TEMPLATE_METHOD(Default, default, Option(T))(void) \
    { \
        return Option_none(T)(); \
    } \
    static inline void TEMPLATE_METHOD(Free, free, Option(T))(ref(Option(T)) self) \
    { \
        if (self->filled) \
        { \
            Free(T)(&self->value); \
        } \
    } \
    static inline iter_val(Option(T)) TEMPLATE_METHOD(IntoIterator, into_iter_val, Option(T))(Option(T) self) \
    { \
        return (iter_val(Option(T))){.opt = self, .consumed = false}; \
    } \
    static inline iter_mut(Option(T)) TEMPLATE_METHOD(IntoIteratorMut, into_iter_mut, Option(T))(ref(Option(T)) self) \
    { \
        return (iter_mut(Option(T))){.opt = self, .consumed = false}; \
    } \
    static inline iter_const(Option(T)) TEMPLATE_METHOD(IntoIteratorConst, into_iter_const, Option(T))(cref(Option(T)) self) \
    { \
        return (iter_const(Option(T))){.opt = self, .consumed = false}; \
    } \
    static inline Option_iter_val_Result(T) TEMPLATE_METHOD(Iterator, next, iter_val(Option(T)))(iter_val(Option(T)) *self) \
    { \
        if (self->consumed || !self->opt.filled) \
        { \
            return (Option_iter_val_Result(T)){.value = self->opt.value, .is_error = true}; \
        } \
        self->consumed = true; \
        return (Option_iter_val_Result(T)){.value = self->opt.value, .is_error = false}; \
    } \
    static inline Option_iter_val_Result(T) TEMPLATE_METHOD(Iterator, deref, iter_val(Option(T)))(const iter_val(Option(T)) *self) \
    { \
        if (self->consumed || !self->opt.filled) \
        { \
            return (Option_iter_val_Result(T)){.value = self->opt.value, .is_error = true}; \
        } \
        return (Option_iter_val_Result(T)){.value = self->opt.value, .is_error = false}; \
    } \
    static inline void TEMPLATE_METHOD(Iterator, free, iter_val(Option(T)))(iter_val(Option(T)) *self) \
    { \
        if (!self->consumed) \
        { \
            Free(Option(T))(&self->opt); \
        } \
    } \
    static inline Option_iter_mut_Result(T) TEMPLATE_METHOD(Iterator, next, iter_mut(Option(T)))(iter_mut(Option(T)) *self) \
    { \
        if (self->consumed || !self->opt->filled) \
        { \
            return (Option_iter_mut_Result(T)){.value = NULL, .is_error = true}; \
        } \
        self->consumed = true; \
        return (Option_iter_mut_Result(T)){.value = &(self->opt->value), .is_error = false}; \
    } \
    static inline Option_iter_mut_Result(T) TEMPLATE_METHOD(Iterator, deref, iter_mut(Option(T)))(const iter_mut(Option(T)) *self) \
    { \
        if (self->consumed || !self->opt->filled) \
        { \
            return (Option_iter_mut_Result(T)){.value = NULL, .is_error = true}; \
        } \
        return (Option_iter_mut_Result(T)){.value = &(self->opt->value), .is_error = false}; \
    } \
    static inline void TEMPLATE_METHOD(Iterator, free, iter_mut(Option(T)))(iter_mut(Option(T)) *self) \
    { \
        (void)self; \
    } \
    static inline Option_iter_const_Result(T) TEMPLATE_METHOD(Iterator, next, iter_const(Option(T)))(iter_const(Option(T)) *self) \
    { \
        if (self->consumed || !self->opt->filled) \
        { \
            return (Option_iter_const_Result(T)){.value = NULL, .is_error = true}; \
        } \
        self->consumed = true; \
        return (Option_iter_const_Result(T)){.value = &(self->opt->value), .is_error = false}; \
    } \
    static inline Option_iter_const_Result(T) TEMPLATE_METHOD(Iterator, deref, iter_const(Option(T)))(const iter_const(Option(T)) *self) \
    { \
        if (self->consumed || !self->opt->filled) \
        { \
            return (Option_iter_const_Result(T)){.value = NULL, .is_error = true}; \
        } \
        return (Option_iter_const_Result(T)){.value = &(self->opt->value), .is_error = false}; \
    } \
    static inline void TEMPLATE_METHOD(Iterator, free, iter_const(Option(T)))(iter_const(Option(T)) *self) \
    { \
        (void)self; \
    }

#include "my_c_utils/iterator.h"

#endif
