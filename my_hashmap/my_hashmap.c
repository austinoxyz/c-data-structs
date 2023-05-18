#include <assert.h>

#include "my_string.h"

#define K_TYPE string
#define V_TYPE int
#include "my_hashmap.h"
#undef K_TYPE
#undef V_TYPE

// http://www.cse.yorku.ca/~oz/hash.html
unsigned long djb2(char *str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
        hash = ((hash << 5) + hash) + c;
    return hash;
}

unsigned long hash_string(string s) {
    return djb2(s.buffer);
}

// #define INIT_NULL_STRING(var_name) string_init_from_cstr(&(var_name), "");

void run_tests(hashmap(string, int) *map, string s, string s2, string s3) 
{
    printf("s: %s\n", s.buffer);
    printf("s2: %s\n", s2.buffer);
    printf("s3: %s\n", s3.buffer);

    printf("\n");

    bool success;
    printf("insert: \n\tkey=`s`\n\tvalue=%d\n", 22);
    success = hashmap_insert(string, int)(map, s, 22);
    assert(success);

    printf("insert: \n\tkey=`s2`\n\tvalue=%d\n", 64);
    success = hashmap_insert(string, int)(map, s2, 64);
    assert(success);

    printf("insert: \n\tkey=`s`\n\tvalue=%d\n", 28);
    success = hashmap_insert(string, int)(map, s, 28);
    assert(success);

    printf("\n");

    int value;
    printf("hashmap_at: \n\tkey=%s\n", s.buffer);
    success = hashmap_at(string, int)(map, s, &value);
    printf("Retrieved value: %d\n", value);

    printf("\n");

    printf("hashmap_at: \n\tkey=%s\n", s2.buffer);
    success = hashmap_at(string, int)(map, s2, &value);
    printf("Retrieved value: %d\n", value);

    printf("\n");

    success = hashmap_contains(string, int)(map, s);
    printf("contains s: %s\n", success ? "true" : "false");
    assert(success);

    success = hashmap_contains(string, int)(map, s3);
    printf("contains s3: %s\n", success ? "true" : "false");
    assert(!success);

    printf("\n");

    printf("count s: %zu\n", hashmap_count(string, int)(map, s));
    printf("\n");
}

int main(void) 
{
    hashmap(string, int) map;
    hashmap_init(string, int)(&map);

    map.key_free = string_free;
    map.key_init = string_init_no_return;
    map.key_copy = string_copy;
    map.key_hash = hash_string;
    map.key_equals = string_equals;
    //map->key_compare = string_compare;

    string s;
    string_init_from_cstr(&s, "Hello, World!");
    string s2;
    string_init_from_cstr(&s2, "Another example string.");
    string s3;
    string_init_from_cstr(&s3, "Not in the map.");

    run_tests(&map, s, s2, s3);

    if (!hashmap_remove(string, int)(&map, s)) {
        printf("Didn't remove element.\n");
    } else { printf("Removed element.\n"); }

    run_tests(&map, s, s2, s3);

    hashmap_free(string, int)(&map);
}
