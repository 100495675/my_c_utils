#ifndef MY_C_UTILS_REF_H
#define MY_C_UTILS_REF_H

// 5 levels of indirection should be enough for anyone
#define REF_EXPAND(Type)                                \
    typedef Type *ref_##Type;                           \
    typedef ref_##Type *ref_ref_##Type;                 \
    typedef ref_ref_##Type *ref_ref_ref_##Type;         \
    typedef ref_ref_ref_##Type *ref_ref_ref_ref_##Type; \
    typedef ref_ref_ref_ref_##Type *ref_ref_ref_ref_ref_##Type;

#endif