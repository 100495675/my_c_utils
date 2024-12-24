#include "hash_map.h"
#include <stdio.h>
#include <string.h>

typedef int Int;
typedef char *String;

size_t String_hash(String key) {
  size_t hash = 0;
  while (*key) {
    hash = (hash * 31) + *key++;
  }
  return hash;
}

bool String_equals(String a, String b) { return a == b; }

void String_free(String s) {}
void Int_free(Int i) {}

HASH_MAP_CONFIG(String, Int)

int main() {
  Hashmap_String_Int hm = Hashmap_String_Int_new(10);
  Hashmap_String_Int_add(&hm, "one", 10);
  Hashmap_String_Int_add(&hm, "two", 20);
  Hashmap_String_Int_add(&hm, "three", 30);

  Hashmap_String_Int_debug(&hm);
  printf("Value: %d\n",
         *Result_Int_ref_unwrap(Hashmap_String_Int_get(&hm, "two")));

  Hashmap_String_Int_remove(&hm, "three");
  Hashmap_String_Int_debug(&hm);

  Hashmap_String_Int_remove(&hm, "two");
  Hashmap_String_Int_debug(&hm);

  Hashmap_String_Int_free(&hm);
}