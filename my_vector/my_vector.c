#include "test.h"

#define TYPE int
#include "my_vector.h"
#undef TYPE

#include "my_string.h"
#define TYPE string
#include "my_vector.h"
#undef TYPE

bool int_equals(int lhs, int rhs) { return lhs == rhs; }

#define prepare_int_vector(name, n)   \
    vector(int) name;                 \
    vector_init(int)(&(name));        \
    name.elem_equals = int_equals;      \
    for (size_t i = 0; i < n_ints; ++i) \
        vector_push_back(int)(&vec, i);

#define prepare_string_vector(name)   \
    vector(string) name;              \
    vector_init(string)(&(name));     \
    name.elem_equals = string_equals; \
    name.elem_copy = string_copy;


void test_int_vector_init_and_free(void) {
    vector(int) vec;
    vector_init(int)(&vec);
    vec.elem_equals = int_equals;
    vector_free(int)(&vec);
}

void test_int_vector_resize(void) {
    const size_t n_ints = INITIAL_VEC_CAP;
    bool success;

    prepare_int_vector(vec, n_ints)

    success = vector_resize(int)(&vec, 2 * vec.capacity);
    assert(success);

    vector_free(int)(&vec);
}

void test_int_vector_push_back(void) {
    const size_t n_ints = 10;
    prepare_int_vector(vec, n_ints)
    for (size_t i = 0; i < n_ints; ++i)
        vector_push_back(int)(&vec, i);
    vector_free(int)(&vec);
}

void test_int_vector_at(void) {
    const size_t n_ints = 10;
    bool success;
    int d;

    prepare_int_vector(vec, n_ints)

    for (size_t i = 0; i < n_ints; ++i) {
        success = vector_at(int)(&vec, i, &d);
        assert(success);
        assert((size_t) d == i);
    }
    assert(vec.size == n_ints);

    vector_free(int)(&vec);
}

void test_int_vector_pop_back(void) {
    const size_t n_ints = 10;
    bool success;
    int d;

    prepare_int_vector(vec, n_ints)

    for (size_t i = 0; i < n_ints; ++i) {
        success = vector_pop_back(int)(&vec, &d);
        assert(success);
        assert((size_t) d == n_ints - i - 1);
    }

    vector_free(int)(&vec);
}

void test_int_vector_remove(void) {
    const size_t n_ints = 10;
    bool success;
    int d;

    prepare_int_vector(vec, n_ints)

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
    const size_t n_ints = 10000;
    bool success;

    prepare_int_vector(vec, n_ints)

    success = vector_resize(int)(&vec, (size_t) n_ints);
    assert(success);

    for (int i = 0; i < (int) n_ints; ++i) 
        vector_push_back(int)(&vec, i);

    assert(vec.size == n_ints);

    vector_free(int)(&vec);
}

void test_string_vector_push_back(void) {
    prepare_string_vector(vec)
    const size_t n_cstrings = 10;
    bool success;

    for (size_t i = 0; i < n_cstrings; ++i) {
        string s;
        string_init_from_cstr(&s, rand_cstr(15));
        vector_push_back(string)(&vec, s);
        string_free(&s);
    }

    for (size_t i = 0; i < n_cstrings; ++i) {
        string s;
        string_init(&s);
        success = vector_at(string)(&vec, i, &s);
        assert(success);
        string_free(&s);
    }

    vector_free(string)(&vec);
}

int main(void) {
    time_function(test_int_vector_push_back);
    time_function(test_int_vector_at);
    time_function(test_int_vector_pop_back);
    time_function(test_int_vector_remove);
    time_function(stress_test_int_vector);

    time_function(test_string_vector_push_back);
}
