#include "util/test.h"

#include "my_string.h"

int main(void) {
    string s;
    string_init(&s);
    for (char c = 'A'; c <= 'Z'; ++c) {
        string_push_back_c(&s, c);
    }

    const char* s2 = "hello";
    string_push_back_cstr(&s, s2);

    string s3;
    string_init_from_cstr(&s3, "world");
    string_push_back_string(&s, &s3);

    string_replace_string(&s, &s3, 2, s3.size);
    string_replace_cstr(&s, s2, 8, 5);

    string_replace_c(&s, 'b', 12);
    string_replace_c(&s, 'a', 13);
    string_replace_c(&s, 'r', 14);

    printf("%s\n", s.buffer);
    printf("Length: %zu\n", my_strlen(s.buffer));

    string_clear(&s);
    string_push_back_cstr(&s, s2);
    string_push_back_string(&s, &s3);

    string test_str; 
    string_init_from_string(&test_str, &s);

    printf("\n\n\n");

    for (int i = 0; i < 100; ++i)
        string_push_back_string(&test_str, &s);

    printf("s: %s\n", s.buffer);
    printf("Length: %zu\n", my_strlen(s.buffer));
    printf("test_str: %s\n", test_str.buffer);
    printf("Length: %zu\n", my_strlen(test_str.buffer));

    printf("\n\n\n");

    const char *replace_contents_test_str = "This is a string that has replaced test_str.";

    bool result = string_replace_contents_cstr(&test_str, 
            replace_contents_test_str, 
            my_strlen(replace_contents_test_str));
    printf("test_str: %s\n", test_str.buffer);
    printf("return val: %s\n", result ? "true" : "false");
    printf("\n");

    const char *replace_contents_s = "Replaced contents of s.";
    result = string_replace_contents_cstr(&s, 
            replace_contents_s, 
            my_strlen(replace_contents_s));
    printf("s: %s\n", s.buffer);
    printf("return val: %s\n", result ? "true" : "false");

    string_free(&s);
    string_free(&s3);
    string_free(&test_str);

    string lhs, rhs;
    string_init_from_cstr(&lhs, "Hello, World\n");
    string_init_from_cstr(&rhs, "Hello, World\n");
    printf("equals: %s\n", string_equals(lhs, rhs) ? "true" : "false");

    string_init_from_cstr(&s, "This is a new s!");
    printf("new s: %s\n", s.buffer);

//    string copyee;
//    string_copy(&copyee, s);
//    printf("copyee contents: %s\n", copyee.buffer);

    const char* random; 
    for (int i = 0; i < 10; ++i) {
        random = rand_cstr(16);
        printf("rand_cstr: %s\n", random);
    }
}
