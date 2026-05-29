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
#define Option_into_iter_val(...) TEMPLATE_METHOD(Option, into_iter_val, __VA_ARGS__)
#define Option_iter_mut(...)      TEMPLATE_METHOD(Option, iter_mut, __VA_ARGS__)
#define Option_iter_const(...)    TEMPLATE_METHOD(Option, iter_const, __VA_ARGS__)
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
#define Option_free(...)    TEMPLATE_METHOD(Option, free, __VA_ARGS__)

/**
 * @brief Maps an Option(T) to Option(U) by applying a function to the contained value.
 * @param T Source type.
 * @param U Destination type.
 * @param self The Option(T) instance.
 * @param fn Mapping function U (*fn)(T).
 * @returns Option(U)
 * @usage Option_map(Int, Double)(opt, int_to_double)
 */
#define Option_map(...)      TEMPLATE_METHOD(Option, map, __VA_ARGS__)

/**
 * @brief Applies a function returning Option(U) to the contained value of Option(T).
 * @param T Source type.
 * @param U Destination type.
 * @param self The Option(T) instance.
 * @param fn Monadic function Option(U) (*fn)(T).
 * @returns Option(U)
 * @usage Option_and_then(Int, Double)(opt, int_to_opt_double)
 */
#define Option_and_then(...) TEMPLATE_METHOD(Option, and_then, __VA_ARGS__)

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
    static inline void TEMPLATE_METHOD(ref_Option, free, T)(ref_Option(T) *value) { (void)value; } \
    static inline void TEMPLATE_METHOD(cref_Option, free, T)(cref_Option(T) *value) { (void)value; } \
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
    static inline Bool Option_is_none(T)(cref_Option(T) self) \
    { \
        return !(self->filled); \
    } \
    \
    static inline Bool Option_is_some(T)(cref_Option(T) self) \
    { \
        return self->filled; \
    } \
    \
    static inline void Option_free(T)(ref_Option(T) self) \
    { \
        if (self->filled) \
        { \
            MY_C_UTILS_CONCAT(T, _free)(&self->value); \
        } \
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
    static inline void iter_val_free(Option(T))(cref_iter_val(Option(T)) value) \
    { \
        if (!value->consumed) \
        { \
            Option_free(T)((Option(T)*)&value->opt); \
        } \
    } \
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
        ref_Option(T) opt; \
        bool consumed; \
    } iter_mut(Option(T)); \
    typedef iter_mut(Option(T)) *ref_iter_mut(Option(T)); \
    typedef const iter_mut(Option(T)) *cref_iter_mut(Option(T)); \
    static inline void TEMPLATE_METHOD(ref_iter_mut_Option, free, T)(ref_iter_mut(Option(T)) *value) { (void)value; } \
    static inline void TEMPLATE_METHOD(cref_iter_mut_Option, free, T)(cref_iter_mut(Option(T)) *value) { (void)value; } \
    static inline void iter_mut_free(Option(T))(cref_iter_mut(Option(T)) value) { (void)value; } \
    \
    typedef struct \
    { \
        cref_Option(T) opt; \
        bool consumed; \
    } iter_const(Option(T)); \
    typedef iter_const(Option(T)) *ref_iter_const(Option(T)); \
    typedef const iter_const(Option(T)) *cref_iter_const(Option(T)); \
    static inline void TEMPLATE_METHOD(ref_iter_const_Option, free, T)(ref_iter_const(Option(T)) *value) { (void)value; } \
    static inline void TEMPLATE_METHOD(cref_iter_const_Option, free, T)(cref_iter_const(Option(T)) *value) { (void)value; } \
    static inline void iter_const_free(Option(T))(cref_iter_const(Option(T)) value) { (void)value; } \
    \
    static inline iter_val(Option(T)) Option_into_iter_val(T)( \
        Option(T) self) \
    { \
        return (iter_val(Option(T))){.opt = self, .consumed = false}; \
    } \
    \
    static inline Option_iter_val_Result(T) iter_val_deref(Option(T))( \
        cref_iter_val(Option(T)) self) \
    { \
        if (self->consumed || !Option_is_some(T)((Option(T)*)&self->opt)) \
        { \
            return (Option_iter_val_Result(T)){.is_error = true}; \
        } \
        return (Option_iter_val_Result(T)){.value = self->opt.value, .is_error = false}; \
    } \
    \
    static inline Option_iter_val_Result(T) iter_val_next(Option(T))(ref_iter_val(Option(T)) self) \
    { \
        if (self->consumed || !Option_is_some(T)(&self->opt)) \
        { \
            return (Option_iter_val_Result(T)){.is_error = true}; \
        } \
        self->consumed = true; \
        return (Option_iter_val_Result(T)){.value = self->opt.value, .is_error = false}; \
    } \
    \
    static inline iter_mut(Option(T)) Option_iter_mut(T)( \
        ref_Option(T) self) \
    { \
        return (iter_mut(Option(T))){.opt = self, .consumed = false}; \
    } \
    \
    static inline Option_iter_mut_Result(T) iter_mut_deref(Option(T))( \
        cref_iter_mut(Option(T)) self) \
    { \
        if (self->consumed || !Option_is_some(T)((Option(T)*)self->opt)) \
        { \
            return (Option_iter_mut_Result(T)){.value = NULL, .is_error = true}; \
        } \
        return (Option_iter_mut_Result(T)){.value = &(self->opt->value), .is_error = false}; \
    } \
    \
    static inline Option_iter_mut_Result(T) iter_mut_next(Option(T))(ref_iter_mut(Option(T)) self) \
    { \
        if (self->consumed || !Option_is_some(T)(self->opt)) \
        { \
            return (Option_iter_mut_Result(T)){.value = NULL, .is_error = true}; \
        } \
        self->consumed = true; \
        return (Option_iter_mut_Result(T)){.value = &(self->opt->value), .is_error = false}; \
    } \
    \
    static inline iter_const(Option(T)) Option_iter_const(T)( \
        cref_Option(T) self) \
    { \
        return (iter_const(Option(T))){.opt = self, .consumed = false}; \
    } \
    \
    static inline Option_iter_const_Result(T) iter_const_deref(Option(T))( \
        cref_iter_const(Option(T)) self) \
    { \
        if (self->consumed || !Option_is_some(T)((Option(T)*)self->opt)) \
        { \
            return (Option_iter_const_Result(T)){.value = NULL, .is_error = true}; \
        } \
        return (Option_iter_const_Result(T)){.value = &(self->opt->value), .is_error = false}; \
    } \
    static inline Option_iter_const_Result(T) iter_const_next(Option(T))(ref_iter_const(Option(T)) self) \
    { \
        if (self->consumed || !Option_is_some(T)((Option(T)*)self->opt)) \
        { \
            return (Option_iter_const_Result(T)){.value = NULL, .is_error = true}; \
        } \
        self->consumed = true; \
        return (Option_iter_const_Result(T)){.value = &(self->opt->value), .is_error = false}; \
    } \
    \
    static inline Result(Void, cref(Char)) Option_push_back(T)(ref_Option(T) self, T value) \
    { \
        if (!self->filled) \
        { \
            *self = Option_some(T)(value); \
        } \
        else \
        { \
            MY_C_UTILS_CONCAT(T, _free)(&value); \
        } \
        return Result_ok(Void, cref(Char))((Void){}); \
    } \
    \
    static inline Option(T) TEMPLATE_METHOD(Default, default, Option(T))(void) \
    { \
        return Option_none(T)(); \
    }

#define OPTION_MAP_CONFIG(T, U) \
    static inline Option(U) TEMPLATE_METHOD(Option, map, T, U)(Option(T) self, U (*fn)(T)) \
    { \
        if (self.filled) { \
            return Option_some(U)(fn(self.value)); \
        } \
        return Option_none(U)(); \
    }

#define OPTION_AND_THEN_CONFIG(T, U) \
    static inline Option(U) TEMPLATE_METHOD(Option, and_then, T, U)(Option(T) self, Option(U) (*fn)(T)) \
    { \
        if (self.filled) { \
            return fn(self.value); \
        } \
        return Option_none(U)(); \
    }



#endif
