#include "util/test.h"

#include "my_vector.h"

// vec_data requires atleast 32 elements. (2 * INITIAL_VEC_CAP)
#define vec_data(T) PASTE2(T, _vec_data)
#define vec_data_size(T) PASTE2(T, _vec_data_size)

static TYPE *vec_data(TYPE) = NULL;
static size_t vec_data_size(TYPE) = 0;

#define init_func(T) PASTE2(T, _init_func)
#define free_func(T) PASTE2(T, _free_func)
#define copy_func(T) PASTE2(T, _copy_func)
#define equals_func(T) PASTE2(T, _equals_func)

static void (*init_func(TYPE))(TYPE *_elem)          = NULL;
static void (*free_func(TYPE))(TYPE  *_elem)         = NULL;
static void (*copy_func(TYPE))(TYPE *lhs, TYPE rhs)  = NULL;
static bool (*equals_func(TYPE))(TYPE lhs, TYPE rhs) = NULL;

#define set_vec_data(T, data, size) \
{                                   \
    vec_data(T) = (data);           \
    vec_data_size(T) = (size);      \
}

#define prepare_empty_vector(name) \
    vector(TYPE) name;             \
    vector_init(TYPE)(&(name));    \
    name.elem_init = init_func(TYPE);    \
    name.elem_free = free_func(TYPE);    \
    name.elem_copy = copy_func(TYPE);    \
    name.elem_equals = equals_func(TYPE);

#define prepare_filled_vector(name, n_elems) \
    prepare_empty_vector((name))             \
    assert(n_elems <= vec_data_size(TYPE));  \
    assert(vec_data(TYPE));                  \
    for (size_t i = 0; i < (n_elems); ++i)   \
        vector_push_back(TYPE)(&(name), vec_data(TYPE)[i]);

#define test_vector_init_and_free(T) PASTE3(test_vector_, T, _init_and_free)
void test_vector_init_and_free(TYPE)(void) {
    vector(TYPE) vec;
    vector_init(TYPE)(&vec);
    vector_free(TYPE)(&vec);
}

#define test_vector_resize(T) PASTE3(test_vector_, T, _resize)
void test_vector_resize(TYPE)(void) {
    const size_t n_elems = INITIAL_VEC_CAP;

    prepare_filled_vector(vec, n_elems)

    bool success = vector_resize(TYPE)(&vec, 2 * vec.capacity);
    assert(success);

    vector_free(TYPE)(&vec);
}

#define test_vector_push_back_and_at(T) PASTE3(test_vector_, T, _push_back_and_at)
void test_vector_push_back_and_at(TYPE)(void) {
    const size_t n_elems = 10;
    prepare_empty_vector(vec)

    for (size_t i = 0; i < n_elems; ++i) {
        vector_push_back(TYPE)(&vec, vec_data(TYPE)[i]);
        assert(vec.size == i + 1);
        assert( equals_func(TYPE)(vec_data(TYPE)[i], vec.buffer[i]) );
    }

    TYPE retrieved;
    if (init_func(TYPE)) init_func(TYPE)(&retrieved);
    bool success;
    for (size_t i = 0; i < n_elems; ++i) {
        success = vector_at(TYPE)(&vec, i, &retrieved);
        assert(success);
        TYPE target = vec_data(TYPE)[i];
        assert( equals_func(TYPE)(target, retrieved) );
    }

    vector_free(TYPE)(&vec);
}

#define test_vector_push_back_and_at_with_resize(T) PASTE3(test_vector_, T, _push_back_and_at_with_resize)
void test_vector_push_back_and_at_with_resize(TYPE)(void) {
    // resize is called within call to vector_push_back(TYPE)
    const size_t n_elems = 2 * INITIAL_VEC_CAP;
    prepare_empty_vector(vec)

    for (size_t i = 0; i < n_elems; ++i) {
        vector_push_back(TYPE)(&vec, vec_data(TYPE)[i]);
        assert(vec.size == i + 1);
        assert( equals_func(TYPE)(vec_data(TYPE)[i], vec.buffer[i]) );
    }

    TYPE retrieved;
    bool success;
    for (size_t i = 0; i < n_elems; ++i) {
        success = vector_at(TYPE)(&vec, i, &retrieved);
        assert(success);
        assert( equals_func(TYPE)(vec_data(TYPE)[i], retrieved) );
    }

    vector_free(TYPE)(&vec);
}

#define test_vector_pop_back(T) PASTE3(test_vector_, T, _pop_back)
void test_vector_pop_back(TYPE)(void) {
    const size_t n_elems = 10;
    prepare_filled_vector(vec, n_elems)

    TYPE retrieved;
    bool success;
    for (size_t i = 0; i < n_elems; ++i) {
        success = vector_pop_back(TYPE)(&vec, &retrieved);
        assert(success);
        assert( equals_func(TYPE)(vec_data(TYPE)[n_elems - i - 1], retrieved) );
    }

    vector_free(TYPE)(&vec);
}

#define test_vector_remove(T) PASTE3(test_vector_, T, _remove)
void test_vector_remove(TYPE)(void) {
    const size_t n_elems = 10;
    prepare_filled_vector(vec, n_elems)

    TYPE retrieved;
    bool success;

    success = vector_remove(TYPE)(&vec, 0);
    assert(success);
    success = vector_at(TYPE)(&vec, 0, &retrieved);
    assert( equals_func(TYPE)(retrieved, vec_data(TYPE)[0 + 1]) );
    assert(vec.size == n_elems - 1);

    success = vector_remove(TYPE)(&vec, 3);
    assert(success);
    success = vector_at(TYPE)(&vec, 3, &retrieved);
    assert( equals_func(TYPE)(retrieved, vec_data(TYPE)[3 + 2]) );
    assert(vec.size == n_elems - 2);

    success = vector_remove(TYPE)(&vec, vec.size - 1);
    assert(success);
    success = vector_at(TYPE)(&vec, vec.size - 1, &retrieved);
    assert( equals_func(TYPE)(retrieved, vec_data(TYPE)[n_elems - 2]) );
    assert(vec.size == n_elems - 3);

    vector_free(TYPE)(&vec);
}

#if 1
#define run_tests_for(T) PASTE3(run_, T, _tests)
void run_tests_for(TYPE)(void) {
    time_function(test_vector_init_and_free(TYPE));
    time_function(test_vector_resize(TYPE));
    time_function(test_vector_push_back_and_at(TYPE));
    time_function(test_vector_push_back_and_at_with_resize(TYPE));
    time_function(test_vector_pop_back(TYPE));
    time_function(test_vector_remove(TYPE));
}
#endif

/*
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
*/
