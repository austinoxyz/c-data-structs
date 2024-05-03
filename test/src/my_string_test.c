#include "util/test.h"

#include "my_string.h"

const char *ref_cstr1 = "Hello, World!";
const char *ref_cstr2 = "This is a much longer string whose length exceeds the value stored in INITIAL_STRING_CAP.";
const char *ref_cstr3 = "FooBar";
const char *ref_cstr4 = "unladen_swallow";

string ref_s1; // "apple"
string ref_s2; // "Tokyo, Japan"
string ref_s3; // "Debussy - Clair de Lune"
string ref_s4; // "Hey, sometimes your opponent is just having a really good day."

void test_string_init_and_free(void) {
    string s;

    bool success = string_init(&s);
    assert(success);
    assert(s.buffer);
    assert(s.size == 0);
    assert(s.capacity == INITIAL_STRING_CAP);
    string_free(&s);
    assert( !(s.buffer) );
}

void test_string_init_from(void) {
    string s;
    bool success;

    // string_init_from_cstr

    success = string_init_from(&s, ref_cstr1);
    assert(success);
    assert(s.buffer);
    assert(s.size == strlen(ref_cstr1));
    assert(s.capacity == INITIAL_STRING_CAP);
    assert( !strcmp(s.buffer, ref_cstr1) );
    string_free(&s);

    success = string_init_from(&s, ref_cstr2);
    assert(success);
    assert(s.buffer);
    assert(s.size == strlen(ref_cstr2));
    assert(s.capacity == 96);
    assert( !strcmp(s.buffer, ref_cstr2) );
    string_free(&s);

    // string_init_from_cstr

    success = string_init_from(&s, ref_cstr1);
    assert(success);
    assert(s.buffer);
    assert(s.size == strlen(ref_cstr1));
    assert(s.capacity == INITIAL_STRING_CAP);
    assert( !strcmp(s.buffer, ref_cstr1) );
    string_free(&s);

    success = string_init_from(&s, ref_cstr2);
    assert(success);
    assert(s.buffer);
    assert(s.size == strlen(ref_cstr2));
    assert(s.capacity == 96);
    assert( !strcmp(s.buffer, ref_cstr2) );
    // NOTE: no string_free

    // string_init_from_string

    string s2;
    success = string_init_from(&s2, s);
    assert(success);
    assert(s.buffer);
    assert(s.size == strlen(ref_cstr2));
    assert(s.capacity == 96);
    assert( !strcmp(s.buffer, ref_cstr2) );

}

void test_string_push_back(void) {
    string s;
    string_init(&s);
    bool success;

    for (int i = 0; i < 10; ++i) {
        success = string_push_back_c(&s, 'a' + i);
        assert(success);
        assert(s.size == (size_t) (i + 1));
        assert(s.capacity == INITIAL_STRING_CAP);
    }
    assert( !strcmp(s.buffer, "abcdefghij") );

    string_free(&s);

    string_init(&s);

    success = string_push_back(&s, ref_cstr1);
    assert(success);
    assert(s.size == strlen(ref_cstr1));
    assert(s.capacity == INITIAL_STRING_CAP);
    assert( !strcmp(s.buffer, "Hello, World!") );

    success = string_push_back(&s, ref_cstr3);
    assert(success);
    assert(s.size == strlen(ref_cstr1) + strlen(ref_cstr3));
    assert(s.capacity == 2 * INITIAL_STRING_CAP);
    assert( !strcmp(s.buffer, "Hello, World!FooBar") );

    string_free(&s);
}

void test_string_replace(void) {
    string s;
    string_init_from_cstr(&s, ref_cstr1);

    bool success;
    size_t length = s.size;

    // string_replace_c

    success = string_replace(&s, 'a', 0, 5);
    assert(success);
    assert(s.size == length);
    assert( !strcmp(s.buffer, "aaaaa, World!") );

    success = string_replace(&s, 'z', s.size - 3, 5);
    assert(success);
    assert(s.size == length + 2);
    length = length + 2;
    assert( !strcmp(s.buffer, "aaaaa, Worzzzzz") );

    // string_replace_cstr

    success = string_replace(&s, ref_cstr3, 0, strlen(ref_cstr3));
    assert(success);
    assert(s.size == length);
    assert( !strcmp(s.buffer, "FooBar Worzzzzz") );

    success = string_replace(&s, ref_cstr4, s.size - 5, strlen(ref_cstr4));
    assert(success);
    assert(s.size == length + 10);
    length = length + 10;
    assert( !strcmp(s.buffer, "FooBar Worunladen_swallow") );

    // string_replace_string

    success = string_replace(&s, ref_s1, 0, strlen(ref_s1.buffer));
    assert(success);
    assert(s.size == length);
    assert( !strcmp(s.buffer, "appler Worunladen_swallow") );

    success = string_replace(&s, ref_s2, s.size - 2, strlen(ref_s2.buffer));
    assert(success);
    assert(s.size == length + 10);
    length = length + 10;
    assert( !strcmp(s.buffer, "appler Worunladen_swallTokyo, Japan") );

    string_free(&s);
}

void test_string_replace_contents(void) {
    string s;

    bool success;

    // string_replace_contents_c

    string_init_from_cstr(&s, ref_cstr1);

    success = string_replace_contents(&s, 'a', 10);
    assert(success);
    assert(s.size == 10);
    assert(s.capacity == INITIAL_STRING_CAP);
    assert( !strcmp(s.buffer, "aaaaaaaaaa") );

    success = string_replace_contents(&s, 'z', 42);
    assert(success);
    assert(s.size == 42);
    assert(s.capacity == 48);
    assert( !strcmp(s.buffer, "zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz") );

    success = string_replace_contents(&s, 'q', 16);
    assert(success);
    assert(s.size == 16);
    assert(s.capacity == 48);
    assert( !strcmp(s.buffer, "qqqqqqqqqqqqqqqq") );

    string_free(&s);

    // string_replace_contents_cstr

    string_init_from_cstr(&s, ref_cstr1);

    success = string_replace_contents(&s, ref_cstr3, strlen(ref_cstr3));
    assert(success);
    assert(s.size == strlen(ref_cstr3));
    assert(s.capacity == INITIAL_STRING_CAP);
    assert( !strcmp(s.buffer, ref_cstr3) );

    success = string_replace_contents(&s, ref_cstr2, strlen(ref_cstr2));
    assert(success);
    assert(s.size == strlen(ref_cstr2));
    assert(s.capacity == 96);
    assert( !strcmp(s.buffer, ref_cstr2) );

    success = string_replace_contents(&s, ref_cstr4, strlen(ref_cstr4));
    assert(success);
    assert(s.size == strlen(ref_cstr4));
    assert(s.capacity == 96);
    assert( !strcmp(s.buffer, ref_cstr4) );

    string_free(&s);

    // string_replace_contents_string

    string_init_from_cstr(&s, ref_cstr1);

    success = string_replace_contents(&s, ref_s1, ref_s1.size);
    assert(success);
    assert(s.size == strlen(ref_s1.buffer));
    assert(s.capacity == INITIAL_STRING_CAP);
    assert( !strcmp(s.buffer, ref_s1.buffer) );

    success = string_replace_contents(&s, ref_s4, ref_s4.size);
    assert(success);
    assert(s.size == strlen(ref_s4.buffer));
    assert(s.capacity == 64);
    assert( !strcmp(s.buffer, ref_s4.buffer) );

    success = string_replace_contents(&s, ref_s2, ref_s2.size);
    assert(success);
    assert(s.size == strlen(ref_s2.buffer));
    assert(s.capacity == 64);
    assert( !strcmp(s.buffer, ref_s2.buffer) );

    string_free(&s);
}

void init_ref_strings(void) {
    string_init_from(&ref_s1, "apple");
    string_init_from(&ref_s2, "Tokyo, Japan");
    string_init_from(&ref_s3, "Debussy - Clair de Lune");
    string_init_from(&ref_s4, "Hey, sometimes your opponent is just having a really good day.");
}

void cleanup(void) {
    string_free(&ref_s1);
    string_free(&ref_s2);
    string_free(&ref_s3);
    string_free(&ref_s4);
}

int main(void) {
    time_function(test_string_init_and_free);
    time_function(test_string_init_from);

    init_ref_strings();

    time_function(test_string_push_back);
    time_function(test_string_replace);
    time_function(test_string_replace_contents);

    cleanup();
}

