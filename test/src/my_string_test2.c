#include "util/test.h"

#include "my_string.h"

const char *ref_s1 = "Hello, World!";
const char *ref_s2 = "This is a much longer string that's length exceeds the value stored in INITIAL_STRING_CAP.";

void test_string_init_and_free(void) {
    string s;

    bool success;
    success = string_init(&s);
    assert(success);

    assert(s.buffer);
    assert(s.size == 0);
    assert(s.capacity == INITIAL_STRING_CAP);

    string_free(&s);
    assert( !(s.buffer) );
}

void test_string_init_from_cstr_and_free(void) {
    string s;

    bool success;
    success = string_init_from_cstr(&s, ref_s1);
    assert(success);

    assert(s.buffer);
    assert(s.size == strlen(ref_s1));
    assert(s.capacity == INITIAL_STRING_CAP);
    assert( !strcmp(s.buffer, ref_s1) );

    string_free(&s);
}

void test_string_push_back_c(void) {
    string s;

    bool success;
    success = string_init(&s);
    assert(success);

    for (int i = 0; i < 10; ++i) {
        success = string_push_back_c(&s, 'a' + i);
        assert(success);
        assert(s.size == (size_t) i);
        assert(s.capacity == INITIAL_STRING_CAP);
    }

    string_free(&s);
}

int main(void) {
    time_function(test_string_init_and_free);
    time_function(test_string_init_from_cstr_and_free);
    time_function(test_string_push_back_c);
}

