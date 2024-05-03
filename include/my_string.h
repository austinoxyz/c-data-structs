#ifndef MY_STRING_
#define MY_STRING_

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sys/time.h>

size_t my_strlen(const char *s);

#define MEM_BLOCK_SIZE (16)
#define INITIAL_STRING_CAP (MEM_BLOCK_SIZE)
#define MAX_STRING_CAP ((INITIAL_STRING_CAP) * 32)

#define NEXT_MEM_SIZE(curr, block_size) ((block_size) * (((curr) / (block_size)) + 1))

typedef struct string {
    size_t capacity;
    size_t size;
    char *buffer;
} string;

bool string_init_with_capacity(string *s, const size_t start_cap);
bool string_init_unallocated(string *s);
bool string_init(string* s);
void string_init_no_return(string *s);

#define string_init_from(a, b)                 \
    _Generic ((b),                             \
            string: string_init_from_string,   \
            char*: string_init_from_cstr,      \
            const char*: string_init_from_cstr \
    )((a), (b))

bool string_init_from_cstr(string* s, const char* cs);
bool string_init_from_string(string* s, string s2);

void string_free(string* s);

void string_copy(string *lhs, string rhs);

bool string_resize(string* s, const size_t new_cap);

bool string_clear(string* s);

#define string_push_back(a, b)                 \
    _Generic ((b),                             \
            int: string_push_back_c,           \
            char: string_push_back_c,          \
            string: string_push_back_string,   \
            char*: string_push_back_cstr,      \
            const char*: string_push_back_cstr \
    )((a), (b))

bool string_push_back_c(string* s, const char c);
bool string_push_back_cstr(string* s, const char *s2);
bool string_push_back_string(string* s, string* s2);

#define string_replace(a, b, c, d)            \
    _Generic ((b),                            \
            int: string_replace_c,            \
            char: string_replace_c,           \
            string: string_replace_string,    \
            char*: string_replace_cstr,       \
            const char*: string_replace_cstr  \
    )((a), (b), (c), (d))

bool string_replace_c(string* s, const char c, const size_t start, const size_t count);
bool string_replace_cstr(string* s, const char* s2, const size_t start, const size_t count);
bool string_replace_string(string* s, string s2, const size_t start, const size_t count);

#define string_replace_contents(a, b, c)              \
    _Generic ((b),                                    \
            int: string_replace_contents_c,           \
            char: string_replace_contents_c,          \
            string: string_replace_contents_string,   \
            char*: string_replace_contents_cstr,      \
            const char*: string_replace_contents_cstr \
    )((a), (b), (c))

bool string_replace_contents_c(string* s, const char c, const size_t count);
bool string_replace_contents_cstr(string* s, const char* s2, const size_t count);
bool string_replace_contents_string(string* s, string s2, const size_t count);

bool string_equals(string lhs, string rhs);
int string_compare(string lhs, string rhs);

unsigned long string_hash(string s);

char *rand_cstr(const size_t size);

#endif // MY_STRING_
