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

void string_copy(string *lhs, string *rhs) {
    if (lhs->buffer == NULL) {
        string_init_from_string(lhs, rhs);
    } else {
//        lhs->size = rhs->size;
//        lhs->capacity = rhs->capacity;
//        lhs->buffer = rhs->buffer;
        string_free(lhs);
        string_init_from_string(lhs, rhs);
    }
}

#define setup_test_strings  \
    string s;                                              \
    string_init_from_cstr(&s, "Hello, World!");            \
    string s2;                                             \
    string_init_from_cstr(&s2, "Another example string."); \
    string s3;                                             \
    string_init_from_cstr(&s3, "Not in the map.");

#define setup_map \
    hashmap(string, int) map;                                     \
    hashmap_init(string, int)(&map, hash_string, string_equals);  \
    hashmap_set_free_key_func(string, int)(&map, string_free);    \
    hashmap_set_copy_key_func(string, int)(&map, string_copy);

#define setup_globals \
    int value;

#define run_tests \
    hashmap_insert(string, int)(&map, s, 22); \
    hashmap_insert(string, int)(&map, s2, 64); \
    hashmap_insert(string, int)(&map, s, 28); \
    if (!hashmap_at(string, int)(&map, s, &value)) { exit(1); } \
    printf("Retrieved value: %d\n", value); \
 \
    if (!hashmap_at(string, int)(&map, s2, &value)) { exit(1); } \
    printf("Retrieved value: %d\n", value); \
 \
    if (!hashmap_resize(string, int)(&map, INITIAL_HASHMAP_CAP * 2)) { \
        printf("Couldn't resize.\n"); \
    } else { \
        if (!hashmap_at(string, int)(&map, s, &value)) {  \
            printf("Couldn't grab value: %s\n", s.buffer); \
        } else { \
            printf("Retrieved value: %d\n", value); \
        } \
    } \
 \
    printf("contains s: %s\n", hashmap_contains(string, int)(&map, s) ? "true" : "false"); \
    printf("contains s3: %s\n", hashmap_contains(string, int)(&map, s3) ? "true" : "false"); \
 \
    printf("count s: %zu\n", hashmap_count(string, int)(&map, s));

#define setup \
    setup_test_strings; \
    setup_map; \
    setup_globals;

int main(void) {
    setup;
    run_tests;

    if (!hashmap_remove(string, int)(&map, s)) {
        printf("Didn't remove element.\n");
    } else {
        printf("Removed element.\n");
    }

    run_tests;

    hashmap_free(string, int)(&map);
}
