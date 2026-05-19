#ifndef MY_C_UTILS_FREE_H
#define MY_C_UTILS_FREE_H

// Unicamente no se usa el trivial_free es para structs con punteros,
// los punteros prestados (borrowed ref) a una variable que tiene dueño no necesitan ser liberados
#define TRIVIAL_FREE(TypeName, ParamDecl) \
    static inline void TypeName##_free(ParamDecl) \
    { \
        (void)value; \
    }

/* free.h now only defines the TRIVIAL_FREE macro; primitive instantiations
   are provided by include/my_c_utils/primitives.h so they can be included
   once per binary. */

#endif