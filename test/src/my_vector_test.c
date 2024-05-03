#define TYPE int
#include "my_vector_test.h"
#undef TYPE

#include "my_string.h"
#define TYPE string
#include "my_vector_test.h"
#undef TYPE

bool int_equals(int lhs, int rhs) { return lhs == rhs; }

void setup_for_int(void) {
    const size_t n_elems = 2 * INITIAL_VEC_CAP;
    int *buffer = (int *) malloc(n_elems * sizeof(int));
    for (int i = 0; i < (int) n_elems; ++i)
        buffer[i] = i;
    set_vec_data(int, buffer, n_elems);
    equals_func(int) = int_equals;
}

void setup_for_string(void) {
    const size_t n_elems = 2 * INITIAL_VEC_CAP;
    string *buffer = (string *) malloc(n_elems * sizeof(string));
    for (size_t i = 0; i < n_elems; ++i) {
        char *temp = rand_cstr(15);
        string_init_from_cstr(&buffer[i], (const char *) temp);
        free(temp);
    }
    set_vec_data(string, buffer, n_elems);
    init_func(string) = string_init_no_return;
    free_func(string) = string_free;
    copy_func(string) = string_copy;
    equals_func(string) = string_equals;
}

void cleanup(void) {
    free(vec_data(int));
    for (size_t i = 0; i < vec_data_size(string); ++i)
        string_free( &vec_data(string)[i] );
    free(vec_data(string));
}

int main(void) {
    setup_for_int();
    setup_for_string();
    run_tests_for(int)();
    run_tests_for(string)();
    cleanup();
}
