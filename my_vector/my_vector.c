#include "test.h"

#define TYPE int
#include "my_vector.h"
#undef TYPE

#include "my_string.h"
#define TYPE string
#include "my_vector.h"
#undef TYPE

bool int_equals(int lhs, int rhs) { return lhs == rhs; }

#define prepare_empty_int_vector(name) \
    vector(int) name;                  \
    vector_init(int)(&(name));         \
    name.elem_equals = int_equals;

#define prepare_int_vector(name, n)     \
    prepare_empty_int_vector((name))    \
    for (size_t i = 0; i < n_ints; ++i) \
        vector_push_back(int)(&(name), i);

#define prepare_empty_string_vector(name)   \
    vector(string) name;                    \
    vector_init(string)(&(name));           \
    name.elem_equals = string_equals;       \
    name.elem_copy = string_copy;

#define prepare_rand_string_vector(name, n)       \
    prepare_empty_string_vector((name))           \
    for (size_t i = 0; i < n_ints; ++i) {         \
        string s;                                 \
        string_init_from_cstr(&s, rand_cstr(15)); \
        vector_push_back(string)(&(name), &s);    \
        string_free(&s);                          \
    }

    


void test_int_vector_init_and_free(void) {
    vector(int) vec;
    vector_init(int)(&vec);
    vector_free(int)(&vec);
}

void test_int_vector_resize(void) {
    const size_t n_ints = INITIAL_VEC_CAP;

    prepare_int_vector(vec, n_ints)

    bool success = vector_resize(int)(&vec, 2 * vec.capacity);
    assert(success);

    vector_free(int)(&vec);
}

void test_int_vector_push_back_and_at(void) {
    const size_t n_ints = 10;
    prepare_empty_int_vector(vec)

    for (size_t i = 0; i < n_ints; ++i) {
        vector_push_back(int)(&vec, i);
        assert(vec.size == i + 1);
    }

    int d;
    bool success;
    for (size_t i = 0; i < n_ints; ++i) {
        success = vector_at(int)(&vec, i, &d);
        assert(success);
        assert((size_t) d == i);
    }

    vector_free(int)(&vec);
}

void test_int_vector_push_back_and_at_with_resize(void) {
    const size_t n_ints = 2 * INITIAL_VEC_CAP;
    prepare_empty_int_vector(vec)

    for (size_t i = 0; i < n_ints; ++i) {
        vector_push_back(int)(&vec, i);
        assert(vec.size == i + 1);
    }

    int d;
    bool success;
    for (size_t i = 0; i < n_ints; ++i) {
        success = vector_at(int)(&vec, i, &d);
        assert(success);
        assert((size_t) d == i);
    }

    vector_free(int)(&vec);
}

void test_int_vector_pop_back(void) {
    const size_t n_ints = 10;
    prepare_int_vector(vec, n_ints)

    int d;
    bool success;
    for (size_t i = 0; i < n_ints; ++i) {
        success = vector_pop_back(int)(&vec, &d);
        assert(success);
        assert((size_t) d == n_ints - i - 1);
    }

    vector_free(int)(&vec);
}

void test_int_vector_remove(void) {
    const size_t n_ints = 10;
    prepare_int_vector(vec, n_ints)

    int d;
    bool success;

    success = vector_remove(int)(&vec, 0);
    assert(success);
    success = vector_at(int)(&vec, 0, &d);
    assert((size_t) d == 1);
    assert(vec.size == n_ints - 1);

    const size_t second_idx = 3;
    success = vector_remove(int)(&vec, 3);
    assert(success);
    success = vector_at(int)(&vec, second_idx, &d);
    assert((size_t) d == second_idx + 2);
    assert(vec.size == n_ints - 2);

    success = vector_remove(int)(&vec, vec.size - 1);
    assert(success);
    success = vector_at(int)(&vec, vec.size - 1, &d);
    assert((size_t) d == n_ints - 2);
    assert(vec.size == n_ints - 3);

    vector_free(int)(&vec);
}

void stress_test_int_vector(void) {
    const size_t n_ints = 1000000000;

    prepare_empty_int_vector(vec)

    bool success = vector_resize(int)(&vec, (size_t) n_ints);
    assert(success);

    for (int i = 0; i < (int) n_ints; ++i) 
        vector_push_back(int)(&vec, i);

    assert(vec.size == n_ints);

    vector_free(int)(&vec);
}

void test_string_vector_init_and_free(void) {
    vector(string) vec;
    vector_init(string)(&vec);
    vector_free(string)(&vec);
}

void test_string_vector_resize(void) {
    prepare_empty_string_vector(vec)
    bool success = vector_resize(string)(&vec, 2 * vec.capacity);
    assert(success);
    vector_free(string)(&vec);
}

void test_string_vector_push_back_and_at(void) {
    prepare_empty_string_vector(vec)

    const size_t n_strings = 10;
    const size_t buf_sz = 15;
    char buf[buf_sz];
    string s;
    for (size_t i = 0; i < n_strings; ++i) {
        memset(buf, 'a' + i, buf_sz - 1);
        buf[buf_sz - 1] = '\0';
        string_init_from_cstr(&s, buf);
        vector_push_back(string)(&vec, s);
        assert(vec.size == i + 1);
        string_free(&s);
    }

    bool success;
    for (size_t i = 0; i < n_strings; ++i) {
        memset(buf, 'a' + i, buf_sz - 1);
        buf[buf_sz - 1] = '\0';
        success = vector_at(string)(&vec, i, &s);
        assert(success);
        assert( !strcmp(s.buffer, buf) );
    }

    vector_free(string)(&vec);
}

void test_string_vector_pop_back(void) {

}

void test_string_vector_at(void) {

}

void test_string_vector_remove(void) {

}

void stress_test_string_vector(void) {

}

int main(void) {
    time_function(test_int_vector_init_and_free);
    time_function(test_int_vector_resize);
    time_function(test_int_vector_push_back_and_at);
    time_function(test_int_vector_push_back_and_at_with_resize);
    time_function(test_int_vector_pop_back);
    time_function(test_int_vector_remove);
//    time_function(stress_test_int_vector);

    time_function(test_string_vector_init_and_free);
    time_function(test_string_vector_resize);
    time_function(test_string_vector_push_back_and_at);
}
