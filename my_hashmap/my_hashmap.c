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

//#define INIT_NULL_STRING(var_name) string_init_from_cstr(&(var_name), "");

void run_tests(hashmap(string, int) *map, string s, string s2, string s3) 
{
    int value;
    hashmap_insert_or_assign(string, int)(map, s, 22);
    hashmap_insert_or_assign(string, int)(map, s2, 64);
    hashmap_insert_or_assign(string, int)(map, s, 28);
    if (!hashmap_at(string, int)(map, s, &value)) { 
        printf("at function broken.\n");
        exit(1); 
    }
    printf("Retrieved value: %d\n", value);

    if (!hashmap_at(string, int)(map, s2, &value)) { exit(1); }
    printf("Retrieved value: %d\n", value);

    if (!hashmap_resize(string, int)(map, INITIAL_HASHMAP_CAP * 2)) {
        printf("Couldn't resize.\n");
    } else {
        if (!hashmap_at(string, int)(map, s, &value)) { 
            printf("Couldn't grab value: %s\n", s.buffer);
        } else {
            printf("Retrieved value: %d\n", value);
        }
    }

    printf("contains s: %s\n", hashmap_contains(string, int)(map, s) ? "true" : "false");
    printf("contains s3: %s\n", hashmap_contains(string, int)(map, s3) ? "true" : "false");

    printf("count s: %zu\n", hashmap_count(string, int)(map, s));
}

int main(void) {
    hashmap(string, int) map;
    hashmap_init(string, int)(&map, hash_string, string_equals);
    hashmap_set_free_key_func(string, int)(&map, string_free);
    hashmap_set_init_key_func(string, int)(&map, string_init_no_return);
    hashmap_set_copy_key_func(string, int)(&map, string_copy);

    string s;
    string_init_from_cstr(&s, "Hello, World!");
    string s2;
    string_init_from_cstr(&s2, "Another example string.");
    string s3;
    string_init_from_cstr(&s3, "Not in the map.");

    run_tests(&map, s, s2, s3);

//    if (!hashmap_remove(string, int)(&map, s)) {
//        printf("Didn't remove element.\n");
//    } else { printf("Removed element.\n"); }
//
////    run_tests(&s, &s2, &s3);
//    run_tests(&map, s, s2, s3);

    hashmap_free(string, int)(&map);
}
