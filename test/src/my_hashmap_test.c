#include "util/test.h"

#include "my_string.h"

#define K_TYPE string
#define V_TYPE int
#include "my_hashmap.h"
#undef K_TYPE
#undef V_TYPE

#define prepare_string_int_map(name) \
    map(string, int) _map;                 \
    map_init(string, int)(&_map);          \
    _map.key_free = string_free;           \
    _map.key_init = string_init_no_return; \
    _map.key_copy = string_copy;           \
    _map.key_hash = string_hash;           \
    _map.key_equals = string_equals;

#define prepare_strings() \
    string s;                                      \
    string_init_from_cstr(&s, "Hello, World!");    \
    string s2;                                     \
    string_init_from_cstr(&s2, "FooBar");          \
    string s3;                                     \
    string_init_from_cstr(&s3, "Unladen Swallow");
    
void test_insert_or_assign(void) {
    bool success;
    int value;
    prepare_string_int_map(_map)
    prepare_strings()

    success = map_insert_or_assign(string, int)(&_map, s, 22);
    assert(success);
    success = map_insert_or_assign(string, int)(&_map, s2, 64);
    assert(success);
    success = map_insert_or_assign(string, int)(&_map, s, 28);

    success = map_at(string, int)(&_map, s, &value);
    assert(success);
    assert(value == 28);
    success = map_at(string, int)(&_map, s2, &value);
    assert(success);
    assert(value == 64);
    success = map_at(string, int)(&_map, s3, &value);
    assert(!success);

    map_free(string, int)(&_map);
    string_free(&s);
    string_free(&s2);
    string_free(&s3);
}

void test_insert(void) {
    bool success;
    int value;
    prepare_string_int_map(_map)
    prepare_strings()

    success = map_insert(string, int)(&_map, s, 14);
    assert(success);
    success = map_insert(string, int)(&_map, s2, 92);
    assert(success);
    success = map_insert(string, int)(&_map, s, 100);

    success = map_at(string, int)(&_map, s, &value);
    assert(success);
    assert(value == 14);
    success = map_at(string, int)(&_map, s2, &value);
    assert(success);
    assert(value == 92);
    success = map_at(string, int)(&_map, s3, &value);
    assert(!success);

    map_free(string, int)(&_map);
    string_free(&s);
    string_free(&s2);
    string_free(&s3);
}

void test_count(void) {
    bool success;
    size_t count;
    prepare_string_int_map(_map)
    prepare_strings()

    success = map_insert(string, int)(&_map, s, 14);
    assert(success);
    success = map_insert(string, int)(&_map, s2, 92);
    assert(success);
    success = map_insert(string, int)(&_map, s, 100);

    count = map_count(string, int)(&_map, s);
    assert(count == 2);
    count = map_count(string, int)(&_map, s2);
    assert(count == 1);
    count = map_count(string, int)(&_map, s3);
    assert(count == 0);

    map_free(string, int)(&_map);
    string_free(&s);
    string_free(&s2);
    string_free(&s3);
}

void test_contains(void) {
    bool success;
    prepare_string_int_map(_map)
    prepare_strings()

    success = map_insert(string, int)(&_map, s, 14);
    assert(success);
    success = map_insert(string, int)(&_map, s2, 92);
    assert(success);
    success = map_insert(string, int)(&_map, s, 100);

    success = map_contains(string, int)(&_map, s);
    assert(success);
    success = map_contains(string, int)(&_map, s2);
    assert(success);
    success = map_contains(string, int)(&_map, s3);
    assert(!success);

    map_free(string, int)(&_map);
    string_free(&s);
    string_free(&s2);
    string_free(&s3);
}

void stress_test(void) {
    bool success;
    prepare_string_int_map(_map)

    // one hundred thousand random strings
    const size_t n_strings = 100000;
    string *random_strings = (string *) malloc(n_strings * sizeof(string));
    assert(random_strings);

    for (size_t i = 0; i < n_strings; ++i) {
        char *random_cstr = rand_cstr(16); 
        string_init_from_cstr(&random_strings[i], random_cstr);
        free(random_cstr);
    }

    for (int i = 0; i < (int) n_strings; ++i) {
        success = map_insert(string, int)(&_map, random_strings[i], i);
        assert(success);
    }

    for (size_t i = 0; i < n_strings; ++i) 
        string_free(&random_strings[i]);
    map_free(string, int)(&_map);
    free(random_strings);
}

int main(void) 
{
    time_function(test_insert_or_assign);
    time_function(test_insert);
    time_function(test_count);
    time_function(test_contains);
    time_function(stress_test);
}
