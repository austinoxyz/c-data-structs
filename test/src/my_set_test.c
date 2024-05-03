#include <stdio.h>

#include "my_string.h"

#define TYPE int
#include "my_set_test.h"
#undef TYPE

#define TYPE string
#include "my_set_test.h"
#undef TYPE

bool int_equals(int lhs, int rhs) { return lhs == rhs; }
int int_compare(int lhs, int rhs) { return lhs - rhs; }
unsigned long int_hash(int n) { return (unsigned long) n; }

// prepares a buffer of 16 ints to be inserted into set where
// 1. range of keys is [0...16)
void setup_for_int(void) {
    size_t n_keys = INITIAL_SET_CAP;

    int *data = (int *) malloc(n_keys * sizeof(int));
    for (int i = 0; i < (int) n_keys; ++i) data[i] = i;

    set_data(int) = data;
    set_data_size(int) = n_keys;

    g_equals_func(int) = int_equals;
    g_compare_func(int) = int_compare;
    g_hash_func(int) = int_hash;
}

void setup_for_string(void) {
    size_t n_keys = INITIAL_SET_CAP;

    string *data = (string *) malloc(n_keys * sizeof(string));
    for (size_t i = 0; i < n_keys; ++i)
        string_init_from_cstr(&data[i], rand_cstr(16));

    set_data(string) = data;
    set_data_size(string) = n_keys;

    g_init_func(string) = string_init_no_return;
    g_free_func(string) = string_free;
    g_copy_func(string) = string_copy;
    g_equals_func(string) = string_equals;
    g_compare_func(string) = string_compare;
    g_hash_func(string) = string_hash;
}

void cleanup(void) {
    free(set_data(int));
    for (size_t i = 0; i < set_data_size(string); ++i)
        string_free( &set_data(string)[i] );
    free(set_data(string));
}

int main(void) {
#if 1
#endif
    setup_for_int();
    run_tests_for(int)();
    setup_for_string();
    run_tests_for(string)();

}
