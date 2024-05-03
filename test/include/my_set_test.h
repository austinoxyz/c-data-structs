#include "util/test.h"

#include "my_set.h"

// vec_data requires atleast 32 elements. (2 * INITIAL_VEC_CAP)
#define set_data(T) PASTE2(T, _set_data)
#define set_data_size(T) PASTE2(T, _set_data_size)

static TYPE *set_data(TYPE) = NULL;
static size_t set_data_size(TYPE) = 0;

#define g_init_func(T) PASTE3(_, T, _init_func)
#define g_free_func(T) PASTE3(_, T, _free_func)
#define g_copy_func(T) PASTE3(_, T, _copy_func)
#define g_equals_func(T) PASTE3(_, T, _equals_func)
#define g_compare_func(T) PASTE3(_, T, _compare_func)
#define g_hash_func(T) PASTE3(_, T, _hash_func)

static init_func(TYPE) g_init_func(TYPE) = NULL;
static free_func(TYPE) g_free_func(TYPE) = NULL;
static copy_func(TYPE) g_copy_func(TYPE) = NULL;
static equals_func(TYPE) g_equals_func(TYPE) = NULL;
static compare_func(TYPE) g_compare_func(TYPE) = NULL;
static hash_func(TYPE) g_hash_func(TYPE) = NULL;

#define set_set_data(T, data, size) \
{                                   \
    set_data(T) = (data);           \
    set_data_size(T) = (size);      \
}

#define prepare_empty_set(name)             \
    set(TYPE) name;                         \
    set_init(TYPE)(&(name));                \
    name.key_init = g_init_func(TYPE);       \
    name.key_free = g_free_func(TYPE);       \
    name.key_copy = g_copy_func(TYPE);       \
    name.key_equals = g_equals_func(TYPE);   \
    name.key_compare = g_compare_func(TYPE); \
    name.key_hash = g_hash_func(TYPE);

#define prepare_filled_set(name) \
    prepare_empty_set(name)                           \
    for (size_t i = 0; i < set_data_size(TYPE); ++i)  \
        set_insert(TYPE)(&(name), set_data(TYPE)[i]); \

#define test_set_init_and_free(T) PASTE3(test_set_, T, _init_and_free)
void test_set_init_and_free(TYPE)(void) {
    set(TYPE) _set;
    set_init(TYPE)(&_set);
    assert(_set.buckets);
    assert(_set.size == 0);
    assert(_set.capacity == INITIAL_SET_CAP);
//    assert( (sizeof _set.buckets[0]) == sizeof(TYPE) );

    set_free(TYPE)(&_set);
    assert( !(_set.buckets) );
}

#define test_set_resize(T) PASTE3(test_set_, T, _resize)
void test_set_resize(TYPE)(void) {
    prepare_filled_set(_set)

    const size_t size_before_resize = _set.size;

    set_resize(TYPE)(&_set, 2 * _set.capacity);
    assert( _set.size == size_before_resize );
    assert( _set.capacity == 2 * INITIAL_SET_CAP );
    assert( _set.buckets );

    set_free(TYPE)(&_set);
}

#define test_set_insert(T) PASTE3(test_set_, T, _insert)
void test_set_insert(TYPE)(void) {
    prepare_empty_set(_set)
    bool success;

    for (size_t i = 0; i < set_data_size(TYPE); ++i) {
        success = set_insert(TYPE)(&_set, set_data(TYPE)[i]);
        assert(success);
        assert(_set.size == (size_t) (i + 1));

        const size_t bucket = g_hash_func(TYPE)(set_data(TYPE)[i]) % _set.capacity;
        bool key_found = false;
        set_node(TYPE) *bucketptr = _set.buckets[bucket];
        while ( bucketptr ) {
            if ( g_equals_func(TYPE)(bucketptr->key, set_data(TYPE)[i]) ) {
                key_found = true;
                break;
            }
            bucketptr = bucketptr->next;
        }
        assert(key_found);
    }
    assert(_set.capacity == INITIAL_SET_CAP);

    set_free(TYPE)(&_set);
}

#define test_set_key_exists(T) PASTE3(test_set_, T, _key_exists)
void test_set_key_exists(TYPE)(void) {
    prepare_empty_set(_set)
    bool success;

    const int end = ((set_data_size(TYPE) / 2) % 2 == 0) 
                        ? (set_data_size(TYPE) / 2)
                        : (set_data_size(TYPE) / 2) - 1;
    for (int i = 0; i < end; ++i)
        set_insert(TYPE)(&_set, set_data(TYPE)[i * 2]);

    for (int i = 0; i < end; ++i) {
        success = set_key_exists(TYPE)(&_set, set_data(TYPE)[i]);
        if (i % 2 == 0) assert(success);
        else assert(!success);
    }

    set_free(TYPE)(&_set);
}

#define test_set_erase(T) PASTE3(test_set_, T, _erase)
void test_set_erase(TYPE)(void) {
    prepare_filled_set(_set)

    const int end = ((set_data_size(TYPE) / 2) % 2 == 0) 
                        ? (set_data_size(TYPE) / 2)
                        : (set_data_size(TYPE) / 2) - 1;
    bool success;
    for (int i = 0; i < end; ++i) {
        success = set_erase(TYPE)(&_set, set_data(TYPE)[i * 2 + 1]);
        assert(success);
    }

    for (int i = 0; i < end; ++i) {
        success = set_key_exists(TYPE)(&_set, set_data(TYPE)[i]);
        if (i % 2 == 0) assert(success);
        else assert(!success);
    }

    set_free(TYPE)(&_set);
}

#define run_tests_for(T) PASTE3(run_, T, _tests)
void run_tests_for(TYPE)(void) {
    time_function(test_set_init_and_free(TYPE));
    time_function(test_set_resize(TYPE));
    time_function(test_set_insert(TYPE));
    time_function(test_set_key_exists(TYPE));
    time_function(test_set_erase(TYPE));
}
