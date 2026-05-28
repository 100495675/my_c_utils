
#include "my_c_utils/my_c_utils.h"
#include <assert.h>

// 1. Declaramos los constructores de tipos para el preprocesador
#define Point(...) TEMPLATE_TYPE(Point, __VA_ARGS__)
#define ref_Point(...) TEMPLATE_TYPE(ref_Point, __VA_ARGS__)
#define cref_Point(...) TEMPLATE_TYPE(cref_Point, __VA_ARGS__)
#define Point_free(...) TEMPLATE_METHOD(Point, free, __VA_ARGS__)
#define Point_clone(...) TEMPLATE_METHOD(Point, clone, __VA_ARGS__)
#define Point_new(...) TEMPLATE_METHOD(Point, new, __VA_ARGS__)

// 2. Definimos la plantilla genérica usando STRUCT_CONFIG
#define TEMPLATE_Point(T)   \
    STRUCT_CONFIG(Point(T), \
                  T, x,     \
                  T, y)

// 3. ¡Instanciamos para el tipo que queramos!
TEMPLATE_CONFIG(Point, Int);
TEMPLATE_CONFIG(Point, Double);

Int main(){
    Point(Int) p1 = Point_new(Int)(3, 4);
    Point(Double) p2 = Point_new(Double)(1.5, 2.5);

    printf("Point<Int>: (%d, %d)\n", p1.x, p1.y);
    printf("Point<Double>: (%.2f, %.2f)\n", p2.x, p2.y);

    // 4. Probar la clonación autogenerada por CLONE_CONFIG
    Point(Int) p1_clone = Point_clone(Int)(&p1);
    assert(p1_clone.x == 3 && p1_clone.y == 4);
    printf("Point<Int> clonado correctamente: (%d, %d)\n", p1_clone.x, p1_clone.y);

    // 5. Probar el tipo Result(Point(Int), cref_Char) autogenerado por RESULT_CONFIG
    Result(Point(Int), cref_Char) res = Result_ok(Point(Int), cref_Char)(p1_clone);
    assert(Result_is_ok(Point(Int), cref_Char)(&res));

    Point(Int) p1_unwrapped = Result_unwrap(Point(Int), cref_Char)(res);
    assert(p1_unwrapped.x == 3 && p1_unwrapped.y == 4);
    printf("Point<Int> extraído de Result correctamente: (%d, %d)\n", p1_unwrapped.x, p1_unwrapped.y);

    // Liberamos todos los elementos de manera segura
    Point_free(Int)(&p1);
    Point_free(Int)(&p1_unwrapped);
    Point_free(Double)(&p2);
    
    printf("¡Todas las características autogeneradas por STRUCT_CONFIG validadas! ... ✓\n");

    // 6. Probar ref(T) y cref(T) como genéricos
    Int val = 42;
    ref(Int) r_val = &val;
    cref(Int) cr_val = &val;
    assert(ref_deref(Int)(r_val) == 42);
    assert(cref_deref(Int)(cr_val) == 42);

    // Probar ref_clone y cref_clone
    ref(Int) r_val_clone = ref_clone(Int)(&r_val);
    assert(r_val_clone == r_val);
    assert(ref_deref(Int)(r_val_clone) == 42);

    cref(Int) cr_val_clone = cref_clone(Int)(&cr_val);
    assert(cr_val_clone == cr_val);
    assert(cref_deref(Int)(cr_val_clone) == 42);

    printf("¡Trivial clone y deref para ref(Int) y cref(Int) validados! ... ✓\n");
    return 0;
}