#ifndef MY_C_UTILS_FREE_H
#define MY_C_UTILS_FREE_H

#define TRIVIAL_FREE(T) \
    static inline void MY_C_UTILS_CONCAT(T, _free)(ref(T) self) { (void)self; }

#endif