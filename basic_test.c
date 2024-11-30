#include "hash_map.h"
#include <stdio.h>
#include <string.h>

size_t hash(int key)
{
    return key;
}

HASH_MAP_CONFIG(int, int, hash)

//! TODO: robin hood 

int main()
{
    Hashmap_int_int hm = hashmap_int_int_new(10);
    hashmap_int_int_add(&hm, 1, 10);
    hashmap_int_int_add(&hm, 11, 20);
    hashmap_int_int_add(&hm, 2, 30);

    hashmap_int_int_debug(&hm);
    printf("Value: %d\n", *result_int_ref_unwrap(hashmap_int_int_get(&hm, 2)));

    hashmap_int_int_remove(&hm, 11);
    hashmap_int_int_debug(&hm);

    hashmap_int_int_remove(&hm, 2); // Fails
    hashmap_int_int_debug(&hm);

    hashmap_int_int_free(&hm);
}