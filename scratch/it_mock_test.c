#include <stdio.h>
#include <stdbool.h>

// Mock of our framework's types and macros to test the pure preprocessor logic
typedef struct {
    int value;
    bool is_error;
} Result_Int;

typedef struct {
    int current;
} iter_Vector_Int;

Result_Int iter_Vector_Int_next(iter_Vector_Int* self) {
    if (self->current < 3) {
        return (Result_Int){.value = ++self->current, .is_error = false};
    }
    return (Result_Int){.is_error = true};
}

#define iter(T) iter_##T
#define iter_next(T) iter_##T##_next

// Mock Mapped iterator
typedef struct {
    iter_Vector_Int source;
    int (*func)(const int*);
} iter_Mapped_Vector_Int_Int;

Result_Int iter_Mapped_Vector_Int_Int_next(iter_Mapped_Vector_Int_Int* self) {
    Result_Int res = iter_Vector_Int_next(&self->source);
    if (!res.is_error) {
        res.value = self->func(&res.value);
    }
    return res;
}

// Mock map macro
#define map(SrcContainer, DestT) \
    ({ \
        iter_Mapped_##SrcContainer##_##DestT _map_impl(iter(SrcContainer) _s, DestT (*_f)(const int*)) { \
            return (iter_Mapped_##SrcContainer##_##DestT){.source = _s, .func = _f}; \
        } \
        _map_impl; \
    })

// Core pipeline macros (same as pipeline)
#define it_pipe_2(input, step1) \
    ({ \
        __typeof__(input) _val = (input); \
        __typeof__(step1) _temp = (step1); \
        _temp; \
    })

#define it_pipe_3(input, step1, step2) \
    ({ \
        __typeof__(input) _val = (input); \
        ({ \
            __typeof__(step1) _temp = (step1); \
            __typeof__(_temp) _val = _temp; \
            __typeof__(step2) _temp2 = (step2); \
            _temp2; \
        }); \
    })

// Proposed it_map descriptor macro!
#define it_map(mapper) \
    ({ \
        typedef __typeof__(*(iter_next(Vector_Int)((__typeof__(_val)*)0)).value) _SrcT; \
        typedef __typeof__(mapper((const _SrcT*)0)) _DestT; \
        map(Vector_Int, _DestT)(_val, mapper); \
    })

int double_val(const int* x) {
    return (*x) * 2;
}

int main(void) {
    iter_Vector_Int vec_iter = {.current = 0};

    // Test the it_pipe with the it_map descriptor!
    iter_Mapped_Vector_Int_Int res = it_pipe_2(
        vec_iter,
        it_map(double_val)
    );

    Result_Int r = iter_Mapped_Vector_Int_Int_next(&res);
    printf("First value: %d\n", r.value); // Should be 2
    r = iter_Mapped_Vector_Int_Int_next(&res);
    printf("Second value: %d\n", r.value); // Should be 4
    return 0;
}
